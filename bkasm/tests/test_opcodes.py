#!/usr/bin/env python3
"""Verify i8080 opcode generation correctness.

Reads test_opcodes.asm, assembles each instruction individually
via bkasm and compares prog.bin output with expected bytes.
"""

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
ASM_PATH = abspath(join(dirname(__file__), "test_opcodes.asm"))
PROG_BIN = "prog.bin"


def parse_expected(comment: str):
    """Parse expected hex bytes from ';' comment."""
    comment = comment.strip()
    if not comment:
        return None
    try:
        return bytes(int(b, 16) for b in comment.split())
    except ValueError:
        return None


BIN_DIR = dirname(BIN_PATH)
PROG_BIN = join(BIN_DIR, "prog.bin")

def run_assembler(asm_source: str):
    """Run bkasm with asm source via stdin (prog.bin next to binary)."""
    proc = subprocess.Popen(
        [BIN_PATH],
        cwd=BIN_DIR,
        stdin=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
    )
    _, stderr = proc.communicate(input=asm_source.encode())
    if proc.returncode != 0:
        return None, stderr.decode().strip()
    if not os.path.exists(PROG_BIN):
        return None, "prog.bin not created"
    with open(PROG_BIN, "rb") as f:
        data = f.read()
    os.unlink(PROG_BIN)
    return data, None


def main():
    if not os.path.exists(BIN_PATH):
        print(f"ERROR: assembler not found at {BIN_PATH}")
        sys.exit(1)
    if not os.path.exists(ASM_PATH):
        print(f"ERROR: test file not found at {ASM_PATH}")
        sys.exit(1)

    passed = 0
    failed = 0

    with open(ASM_PATH, "r", encoding="utf-8") as f:
        for line_num, line in enumerate(f, 1):
            line = line.strip()

            # skip empty lines and section headers
            if not line or line.startswith(";") or line.startswith("ORG"):
                continue

            # look for separator: mnemonic ; expected_bytes
            if ";" not in line:
                continue

            instr_part, comment_part = line.split(";", 1)
            instr_part = instr_part.strip()
            comment_part = comment_part.strip()

            if not instr_part or not comment_part:
                continue

            expected = parse_expected(comment_part)
            if expected is None:
                print(f"SKIP line {line_num}: cant parse expected bytes: {comment_part}")
                continue

            asm = f"ORG 0\n{instr_part}\n"
            result, err = run_assembler(asm)

            if result is None:
                print(f"FAIL line {line_num}: {instr_part}")
                print(f"      assembler error: {err}")
                failed += 1
                continue

            if result == expected:
                print(f"OK   line {line_num}: {instr_part}")
                passed += 1
            else:
                got_hex = result.hex()
                exp_hex = expected.hex()
                print(f"FAIL line {line_num}: {instr_part}")
                print(f"      expected: {exp_hex}")
                print(f"      got:      {got_hex}")
                failed += 1

    print(f"\n{'='*40}")
    print(f"Total: {passed + failed}, passed: {passed}, failed: {failed}")
    if failed:
        # remove prog.bin if left over
        if os.path.exists(PROG_BIN):
            os.unlink(PROG_BIN)
        sys.exit(1)


if __name__ == "__main__":
    main()
