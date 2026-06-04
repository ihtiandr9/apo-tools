#!/usr/bin/env python3
"""Verify ORG, EQU, and range boundary behaviour.

Tests multi-line assembly scenarios that test_opcodes.py can't handle
(since it prepends ORG 0 to every single line).
"""

import subprocess
import os
import sys
from os.path import abspath, dirname, join

_CMAKE_BIN = abspath(join(dirname(__file__), "../Build/CMake.MinGW/bkasm/bkasm"))
_OLD_BIN = abspath(join(dirname(__file__), "../Build/GNU.MinGW/bkasm"))
BIN_PATH = os.environ.get("BKASM_BINARY") or _CMAKE_BIN
if not os.path.exists(BIN_PATH):
    BIN_PATH = _OLD_BIN
BIN_DIR = dirname(BIN_PATH)
PROG_BIN = join(BIN_DIR, "prog.bin")


def run(asm_source: str):
    proc = subprocess.Popen(
        [BIN_PATH],
        cwd=BIN_DIR,
        stdin=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = proc.communicate(input=asm_source.encode())
    err = stderr.decode().strip()
    ok = proc.returncode == 0
    data = None
    if ok and os.path.exists(PROG_BIN):
        with open(PROG_BIN, "rb") as f:
            data = f.read()
        os.unlink(PROG_BIN)
    return data, err


TESTS = [
    # ORG tests
    ("ORG 0\nNOP", "00", "ORG 0 + NOP"),
    ("ORG 100H\nNOP", "00", "ORG 100H + NOP (code at prog[0])"),
    ("ORG 100H\nLABEL: NOP\nJMP LABEL", "00c30001", "ORG + label resolution"),

    # EQU tests
    ("VAL: EQU 42H\nMVI A, VAL", "3e42", "EQU const in MVI"),
    ("VAL: EQU 10H+20H\nMVI A, VAL", "3e30", "EQU with expression"),
    ("VAL: EQU 0\nMVI A, VAL", "3e00", "EQU zero"),
    ("VAL: EQU 0FFH\nMVI A, VAL", "3eff", "EQU 0FFH"),
    ("VAL1: EQU 1000H\nVAL2: EQU 2000H\nJMP VAL1+VAL2", "c30030", "EQU arithmetic"),

    # Range boundary tests (multi-line)
    ("ORG 0\nMVI A, 0", "3e00", "MVI A, 0"),
    ("ORG 0\nMVI A, 0FFH", "3eff", "MVI A, 0FFH"),
    ("ORG 0\nADI 0FFH", "c6ff", "ADI 0FFH"),
    ("ORG 0\nLXI B, 0FFFFH", "01ffff", "LXI B, 0FFFFH"),
    ("ORG 0\nDB 0FFH", "ff", "DB 0FFH"),
    ("ORG 0\nDW 0FFFFH", "ffff", "DW 0FFFFH"),

    # DS tests
    ("ORG 0\nDS 5\nNOP", "000000000000", "DS 5 + NOP"),
    ("ORG 0\nNOP\nDS 10\nNOP", "000000000000000000000000", "NOP + DS 10 + NOP"),
    ("ORG 0\nDS 0\nNOP", "00", "DS 0 + NOP"),
    ("ORG 0\nDS 10H\nNOP", "0000000000000000000000000000000000", "DS 10H + NOP"),
    ("ORG 0\nNOP\nDS 5+5\nNOP", "000000000000000000000000", "NOP + DS 5+5 + NOP"),

    # Duplicate label error
    ("LABEL: NOP\nLABEL: NOP", None, "duplicate label error"),
]


def main():
    if not os.path.exists(BIN_PATH):
        print(f"ERROR: assembler not found at {BIN_PATH}")
        sys.exit(1)

    passed = 0
    failed = 0

    for asm_src, expected_hex, desc in TESTS:
        data, err = run(asm_src)

        # Error tests (expected_hex is None)
        if expected_hex is None:
            if data is not None:
                print(f"FAIL {desc}")
                print(f"      expected error, got success (data={data.hex()})")
                failed += 1
            else:
                print(f"OK   {desc}  [{err.split(chr(10))[-2] if err else '?'}]")
                passed += 1
            continue

        # Success tests
        if data is None:
            print(f"FAIL {desc}")
            print(f"      error: {err}")
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
