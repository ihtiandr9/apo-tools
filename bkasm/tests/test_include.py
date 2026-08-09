#!/usr/bin/env python3
"""Verify INCLUDE directive assembles included files correctly."""

import subprocess
import os
import sys
import tempfile
from os.path import abspath, dirname, join

_CMAKE_BIN = abspath(join(dirname(__file__), "../Build/CMake.MinGW/bkasm/bkasm"))
_OLD_BIN = abspath(join(dirname(__file__), "../Build/GNU.MinGW/bkasm"))
BIN_PATH = os.environ.get("BKASM_BINARY") or _CMAKE_BIN
if not os.path.exists(BIN_PATH):
    BIN_PATH = _OLD_BIN
BIN_DIR = dirname(BIN_PATH)
PROG_BIN = join(BIN_DIR, "prog.bin")

def run_with_files(main_asm, files):
    """Write files to a temp dir, run bkasm on main_asm, return (data, err)."""
    with tempfile.TemporaryDirectory() as tmp:
        for name, content in files.items():
            path = join(tmp, name)
            with open(path, "w") as f:
                f.write(content)

        proc = subprocess.Popen(
            [BIN_PATH, main_asm],
            cwd=tmp,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
        )
        _, stderr = proc.communicate()
        err = stderr.decode().strip()
        ok = proc.returncode == 0
        prog_bin = join(tmp, "prog.bin")
        data = None
        if ok and os.path.exists(prog_bin):
            with open(prog_bin, "rb") as f:
                data = f.read()
        return data, err


TESTS = [
    # simple include
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\n',
                   "lib.asm": "NOP\n"},
     "00", "include one NOP"),

    # include with multiple instructions
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\n',
                   "lib.asm": "MVI A, 42H\n"},
     "3e42", "include MVI"),

    # include with label and jump
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nJMP START\n',
                   "lib.asm": "START: NOP\n"},
     "00c30000", "include with label"),

    # multiple includes
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "a.asm"\nINCLUDE "b.asm"\n',
                   "a.asm": "MVI A, 1\n",
                   "b.asm": "MVI B, 2\n"},
     "3e010602", "two includes"),

    # nested include
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "a.asm"\n',
                   "a.asm": 'MVI A, 1\nINCLUDE "b.asm"\n',
                   "b.asm": "MVI B, 2\n"},
     "3e010602", "nested include"),

    # include file not found → error
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "nonexistent.asm"\n'},
     None, "include file not found error"),

    # main asm after include still works
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nMVI A, 42H\n',
                   "lib.asm": "NOP\n"},
     "003e42", "code after include"),

    # include with EQU
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nMVI A, VAL\n',
                   "lib.asm": "VAL: EQU 42H\n"},
     "3e42", "include with EQU"),
]


def main():
    if not os.path.exists(BIN_PATH):
        print(f"ERROR: assembler not found at {BIN_PATH}")
        sys.exit(1)

    passed = 0
    failed = 0

    for main_name, files, expected_hex, desc in TESTS:
        data, err = run_with_files(main_name, files)

        if expected_hex is None:
            if data is not None:
                print(f"FAIL {desc}")
                print(f"      expected error, got success (data={data.hex()})")
                failed += 1
            else:
                print(f"OK   {desc}  [error expected]")
                passed += 1
            continue

        if data is None:
            print(f"FAIL {desc}")
            print(f"      assembler error: {err}")
            failed += 1
            continue

        got = data.hex()
        if got == expected_hex:
            print(f"OK   {desc}")
            passed += 1
        else:
            print(f"FAIL {desc}")
            print(f"      expected: {expected_hex}")
            print(f"      got:      {got}")
            failed += 1

    print(f"\n{'='*40}")
    print(f"Total: {passed + failed}, passed: {passed}, failed: {failed}")
    if failed:
        if os.path.exists(PROG_BIN):
            os.unlink(PROG_BIN)
        sys.exit(1)


if __name__ == "__main__":
    main()
