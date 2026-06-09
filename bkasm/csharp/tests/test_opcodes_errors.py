#!/usr/bin/env python3
import subprocess
import unittest
import os
from os.path import abspath, dirname, join

TESTS_DIR = dirname(__file__)
BKASM_DIR = dirname(TESTS_DIR)
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"
ERRORS_PATH = abspath(join(TESTS_DIR, "../../tests/test_opcodes_errors.txt"))


class TestOpcodesErrors(unittest.TestCase):
    def test_errors(self):
        failed = 0
        with open(ERRORS_PATH, "r", encoding="utf-8") as f:
            for line_num, line in enumerate(f, 1):
                line = line.strip()
                if not line or line.startswith(";"):
                    continue
                if ";" not in line:
                    continue

                instr_part, comment_part = line.split(";", 1)
                instr_part = instr_part.strip()
                comment_part = comment_part.strip()
                if not instr_part:
                    continue

                expected_error = None
                if comment_part != "None":
                    expected_error = comment_part

                proc = subprocess.Popen(
                    [MONO, BKASM_EXE],
                    cwd=BKASM_DIR,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.PIPE,
                )
                _, stderr = proc.communicate(input=instr_part.encode())
                stderr_text = stderr.decode()

                err_msg = None
                if stderr_text:
                    err_stream = stderr_text.strip().split("\n")
                    if len(err_stream) >= 2:
                        err_msg = err_stream[1]
                    else:
                        err_msg = err_stream[0]

                ok = (err_msg == expected_error) or (expected_error is None and not stderr_text)
                if ok:
                    print(f"OK   line {line_num}: {instr_part}")
                else:
                    print(f"FAIL line {line_num}: {instr_part}")
                    print(f"      expected: {expected_error}")
                    print(f"      got:      {err_msg}")
                    failed += 1

        self.assertEqual(failed, 0)


if __name__ == '__main__':
    unittest.main()
