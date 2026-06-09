#!/usr/bin/env python3
import subprocess
import os
import sys
from os.path import abspath, dirname, join

BKASM_DIR = dirname(dirname(abspath(__file__)))
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"
PROG_BIN = join(BKASM_DIR, "prog.bin")


def run(asm_source):
    proc = subprocess.Popen(
        [MONO, BKASM_EXE],
        cwd=BKASM_DIR,
        stdin=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
    )
    _, stderr = proc.communicate(input=asm_source.encode())
    err = stderr.decode().strip()
    ok = proc.returncode == 0
    data = None
    if ok and os.path.exists(PROG_BIN):
        with open(PROG_BIN, "rb") as f:
            data = f.read()
        os.unlink(PROG_BIN)
    return data, err


TESTS = [
    ('DB "A"', "41", "single char"),
    ('DB "ABC"', "414243", "multiple chars"),
    ('DB "Hello"', "48656c6c6f", "5-char string"),
    ('DB ""', "", "empty string"),
    ('DB "AB","CD"', "41424344", "two string args"),
    ('DB "A",0DH,0AH,"B"', "410d0a42", "mixed string+numeric"),
    ('DB "Hi!",0', "48692100", "string + null terminator"),
    ('DB 10,"Hello",13,10,0', "0a48656c6c6f0d0a00", "CR + string + CRLF + null"),
    ('MSG: DB "Test"', "54657374", "label + string"),
    ("ORG 100H\nDB \"A\"", "41", "ORG + string"),
    ('DB "A"\nDB "B"', "4142", "two DB statements"),
    ('DB " ",0DH,0AH', "200d0a", "space + CRLF"),
    ('DB "AB",10,"CD"', "41420a4344", "string+num+string"),
]


def main():
    if not os.path.exists(BKASM_EXE):
        print(f"ERROR: assembler not found at {BKASM_EXE}")
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
