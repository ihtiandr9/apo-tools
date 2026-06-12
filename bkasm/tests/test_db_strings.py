#!/usr/bin/env python3
"""Verify DB string literal byte generation.

Assembles snippets with DB "..." and compares prog.bin contents
with expected hex output.
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
    # single character
    ('DB "A"', "41", "single char"),

    # multiple characters
    ('DB "ABC"', "414243", "multiple chars"),
    ('DB "Hello"', "48656c6c6f", "5-char string"),

    # empty string
    ('DB ""', "", "empty string"),

    # multiple string arguments
    ('DB "AB","CD"', "41424344", "two string args"),

    # mixed string and numeric
    ('DB "A",0DH,0AH,"B"', "410d0a42", "mixed string+numeric"),
    ('DB "Hi!",0', "48692100", "string + null terminator"),
    ('DB 10,"Hello",13,10,0', "0a48656c6c6f0d0a00", "CR + string + CRLF + null"),

    # with label
    ('MSG: DB "Test"', "54657374", "label + string"),

    # with ORG
    ("ORG 100H\nDB \"A\"", "41", "ORG + string"),

    # multiple DB statements
    ('DB "A"\nDB "B"', "4142", "two DB statements"),

    # special characters
    ('DB " ",0DH,0AH', "200d0a", "space + CRLF"),

    # string across multiple DB in one line with other params
    ('DB "AB",10,"CD"', "41420a4344", "string+num+string"),
]


def main():
    if not os.path.exists(BIN_PATH):
        print(f"ERROR: assembler not found at {BIN_PATH}")
        sys.exit(1)

    passed = 0
    failed = 0

    for asm_src, expected_hex, desc in TESTS:
        data, err = run(asm_src)

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
