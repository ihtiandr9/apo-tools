#!/usr/bin/env python3
import subprocess
import unittest
import os
from os.path import abspath, dirname, join

TESTS_DIR = dirname(__file__)
BKASM_DIR = dirname(TESTS_DIR)
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"
ASM_PATH = abspath(join(TESTS_DIR, "../../tests/test_opcodes.asm"))
PROG_BIN = join(BKASM_DIR, "prog.bin")


def parse_expected(comment):
    comment = comment.strip()
    if not comment:
        return None
    try:
        return bytes(int(b, 16) for b in comment.split())
    except ValueError:
        return None


class TestOpcodes(unittest.TestCase):
    def test_opcodes(self):
        failed = 0
        with open(ASM_PATH, "r", encoding="utf-8") as f:
            for line_num, line in enumerate(f, 1):
                line = line.strip()
                if not line or line.startswith(";") or line.startswith("ORG"):
                    continue
                if ";" not in line:
                    continue

                instr_part, comment_part = line.split(";", 1)
                instr_part = instr_part.strip()
                comment_part = comment_part.strip()
                if not instr_part or not comment_part:
                    continue

                expected = parse_expected(comment_part)
                if expected is None:
                    continue

                asm = f"ORG 0\n{instr_part}\n"
                proc = subprocess.Popen(
                    [MONO, BKASM_EXE],
                    cwd=BKASM_DIR,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.PIPE,
                )
                _, stderr = proc.communicate(input=asm.encode())
                if proc.returncode != 0:
                    print(f"FAIL line {line_num}: {instr_part}")
                    print(f"      error: {stderr.decode().strip()}")
                    failed += 1
                    continue

                if not os.path.exists(PROG_BIN):
                    print(f"FAIL line {line_num}: {instr_part}")
                    print(f"      prog.bin not created")
                    failed += 1
                    continue

                with open(PROG_BIN, "rb") as f:
                    data = f.read()
                os.unlink(PROG_BIN)

                if data == expected:
                    print(f"Passed line {line_num}: {instr_part}")
                else:
                    print(f"FAIL line {line_num}: {instr_part}")
                    print(f"      expected: {expected.hex()}")
                    print(f"      got:      {data.hex()}")
                    failed += 1

        self.assertEqual(failed, 0)


if __name__ == '__main__':
    unittest.main()
