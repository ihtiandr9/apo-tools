#!/usr/bin/env python3
import subprocess
import os
import sys
import tempfile
from os.path import abspath, dirname, join

BKASM_DIR = dirname(dirname(abspath(__file__)))
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"


def run_with_files(main_asm, files):
    with tempfile.TemporaryDirectory() as tmp:
        for name, content in files.items():
            path = join(tmp, name)
            with open(path, "w") as f:
                f.write(content)

        proc = subprocess.Popen(
            [MONO, BKASM_EXE, main_asm],
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
    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\n',
                   "lib.asm": "NOP\n"},
     "00", "include one NOP"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\n',
                   "lib.asm": "MVI A, 42H\n"},
     "3e42", "include MVI"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nJMP START\n',
                   "lib.asm": "START: NOP\n"},
     "00c30000", "include with label"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "a.asm"\nINCLUDE "b.asm"\n',
                   "a.asm": "MVI A, 1\n",
                   "b.asm": "MVI B, 2\n"},
     "3e010602", "two includes"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "a.asm"\n',
                   "a.asm": 'MVI A, 1\nINCLUDE "b.asm"\n',
                   "b.asm": "MVI B, 2\n"},
     "3e010602", "nested include"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "nonexistent.asm"\n'},
     None, "include file not found error"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nMVI A, 42H\n',
                   "lib.asm": "NOP\n"},
     "003e42", "code after include"),

    ("main.asm", {"main.asm": 'ORG 0\nINCLUDE "lib.asm"\nMVI A, VAL\n',
                   "lib.asm": "VAL: EQU 42H\n"},
     "3e42", "include with EQU"),
]


def main():
    if not os.path.exists(BKASM_EXE):
        print(f"ERROR: assembler not found at {BKASM_EXE}")
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
        sys.exit(1)


if __name__ == "__main__":
    main()
