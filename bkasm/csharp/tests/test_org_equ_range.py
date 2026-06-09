#!/usr/bin/env python3
import subprocess
import unittest
import os
from os.path import abspath, dirname, join

TESTS_DIR = dirname(__file__)
BKASM_DIR = dirname(TESTS_DIR)
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"
PROG_BIN = join(BKASM_DIR, "prog.bin")


TESTS = [
    ("ORG 0\nNOP", "00", "ORG 0 + NOP"),
    ("ORG 100H\nNOP", "00", "ORG 100H + NOP (code at prog[0])"),
    ("ORG 100H\nLABEL: NOP\nJMP LABEL", "00c30001", "ORG + label resolution"),
    ("VAL: EQU 42H\nMVI A, VAL", "3e42", "EQU const in MVI"),
    ("VAL: EQU 10H+20H\nMVI A, VAL", "3e30", "EQU with expression"),
    ("VAL: EQU 0\nMVI A, VAL", "3e00", "EQU zero"),
    ("VAL: EQU 0FFH\nMVI A, VAL", "3eff", "EQU 0FFH"),
    ("VAL1: EQU 1000H\nVAL2: EQU 2000H\nJMP VAL1+VAL2", "c30030", "EQU arithmetic"),
    ("ORG 0\nMVI A, 0", "3e00", "MVI A, 0"),
    ("ORG 0\nMVI A, 0FFH", "3eff", "MVI A, 0FFH"),
    ("ORG 0\nADI 0FFH", "c6ff", "ADI 0FFH"),
    ("ORG 0\nLXI B, 0FFFFH", "01ffff", "LXI B, 0FFFFH"),
    ("ORG 0\nDB 0FFH", "ff", "DB 0FFH"),
    ("ORG 0\nDW 0FFFFH", "ffff", "DW 0FFFFH"),
    ("ORG 0\nDS 5\nNOP", "000000000000", "DS 5 + NOP"),
    ("ORG 0\nNOP\nDS 10\nNOP", "000000000000000000000000", "NOP + DS 10 + NOP"),
    ("ORG 0\nDS 0\nNOP", "00", "DS 0 + NOP"),
    ("ORG 0\nDS 10H\nNOP", "0000000000000000000000000000000000", "DS 10H + NOP"),
    ("ORG 0\nNOP\nDS 5+5\nNOP", "000000000000000000000000", "NOP + DS 5+5 + NOP"),
    ("LABEL: NOP\nLABEL: NOP", None, "duplicate label error"),
]


class TestOrgEquRange(unittest.TestCase):
    def test_org_equ_range(self):
        failed = 0
        for asm_src, expected_hex, desc in TESTS:
            proc = subprocess.Popen(
                [MONO, BKASM_EXE],
                cwd=BKASM_DIR,
                stdin=subprocess.PIPE,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
            )
            _, stderr = proc.communicate(input=asm_src.encode())
            err = stderr.decode().strip()
            ok = proc.returncode == 0

            data = None
            if ok and os.path.exists(PROG_BIN):
                with open(PROG_BIN, "rb") as f:
                    data = f.read()
                os.unlink(PROG_BIN)

            if expected_hex is None:
                if data is not None:
                    print(f"FAIL {desc}")
                    print(f"      expected error, got success (data={data.hex()})")
                    failed += 1
                else:
                    print(f"OK   {desc}")
                continue

            if data is None:
                print(f"FAIL {desc}")
                print(f"      error: {err}")
                failed += 1
                continue

            got = data.hex()
            if got == expected_hex:
                print(f"OK   {desc}")
            else:
                print(f"FAIL {desc}")
                print(f"      expected: {expected_hex}")
                print(f"      got:      {got}")
                failed += 1

        self.assertEqual(failed, 0)


if __name__ == '__main__':
    unittest.main()
