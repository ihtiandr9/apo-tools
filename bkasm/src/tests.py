# -*- encoding: utf-8 -*-

## Test script on python
import subprocess
import unittest
from os.path import abspath, dirname, join
import os

class TestUM(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        self.osname = os.name
        super().__init__(methodName)

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_1pipe(self):
        if(self.osname == "nt"):
            cmd_str = ['C:\\windows\\system32\\cmd.exe', '/c', 'dir']
            enc = 'cp866'
        else:
            cmd_str = ['ls', '-la']
            enc = 'utf-8'

        process = subprocess.Popen(cmd_str,
            stdout = subprocess.PIPE)

        if process.stdout:
            output = process.stdout.readlines()
            process.stdout.close()
            for i in output:
                print( i.decode(encoding = enc).replace("\r", "").replace("\n", ""))

        process.kill()
        self.assertEqual(0, 0)
        print('\ntest_pipes')
    
    def test_executeProgram(self):
        log_path = abspath(join(dirname(__file__), "log.txt"))
        log_file = open('log.txt', 'w')
        if(self.osname == "nt"):
            suffix = ".exe"
        else:
            suffix = ""

        path = abspath(join(dirname(__file__), "../build/bkasm" + suffix))
        testfile = abspath(join(dirname(__file__), "../src/tests/test.asm"))
        process = subprocess.Popen([path, testfile],
                    universal_newlines = True,
                    stderr = subprocess.PIPE,
                    stdout = log_file)

        stdout, stderr = process.communicate()

        log_file.close()

        if stderr:
            errstr = stderr.split('\n')
##            print(errstr[1])
            self.assertEqual(errstr[1],
                "Error: Unexpected symbol: \",\" (may be in lowercase)",
                "Is errors!! None expected")
        print('\ntest.asm')


if __name__ == '__main__':
    unittest.main()
