#!/usr/bin/env python3
import subprocess
import unittest
import os
from os.path import abspath, dirname, join

TESTS_DIR = dirname(__file__)
BKASM_DIR = dirname(TESTS_DIR)
BKASM_EXE = join(BKASM_DIR, "bkasm.exe")
MONO = "mono"


class TestErrors(unittest.TestCase):
    def test_executeProgram(self):
        err_count = 0
        err_path = abspath(join(TESTS_DIR, "../../tests/errors.txt"))
        with open(err_path, 'r') as f:
            errors = f.readlines()

        print("\n ---------- \n Test keywords started\n")

        for num in range(len(errors)):
            line = errors[num].rstrip('\n')
            instr_part, comment_part = line.split(";", 1)
            instr_part = instr_part.strip()
            comment_part = comment_part.strip()
            proc = subprocess.Popen(
                [MONO, BKASM_EXE],
                cwd=BKASM_DIR,
                stdin=subprocess.PIPE,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
            )
            _, stderr = proc.communicate(input=line.encode())

            err_msg = None
            if stderr:
                err_stream = stderr.decode().split('\n')
                err_msg = err_stream[1]
            else:
                if proc.returncode != 0:
                    err_msg = 'Nozero exitcode in ' + line

            if comment_part == 'None':
                comment_part = None

            if err_msg == comment_part:
                print(f"Passed line {num + 1}: {instr_part}")
            else:
                err_count += 1
                print('Failed')
                print('Unexpexted msg!!!Expect: ' + str(comment_part))
                print('    Got: ' + str(err_msg) + '\n-----------\n')

        if err_count != 0:
            print('Test completed with ' + str(err_count) + ' errors')
        else:
            print('All tests passed competely')

        self.assertEqual(err_count, 0)


if __name__ == '__main__':
    unittest.main()
