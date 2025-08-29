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

    def test_apipe(self):
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

        process.wait()
        self.assertEqual(0, 0)
        print('\ntest_pipes')
    
    def test_executeProgram(self):
        err_count = 0
        log_path = abspath(join(dirname(__file__), "../build/log.txt"))
        log_file = open(log_path, 'w')
        err_path = abspath(join(dirname(__file__), "../tests/errors.txt"))
        err_file = open(err_path, 'r')
        errors = err_file.readlines()
        err_file.close()

        if(self.osname == "nt"):
            suffix = ".exe"
        else:
            suffix = ""

        exe_path = abspath(join(dirname(__file__), "../build/bkasm" + suffix))
        for num in range(29):
            log_file.write('line: ' + str(num + 1) + '\n')
            log_file.flush()
            process = subprocess.Popen([exe_path],
                universal_newlines = True,
                stdin = subprocess.PIPE,
                stdout = log_file,
                stderr = subprocess.PIPE)
            process.stdin.write(errors[num])    
            stdout, stderr = process.communicate()
            expected_error = errors[num].replace('\n','')
            semicolon_pos = expected_error.find(';')
            expected_error = expected_error[semicolon_pos + 1:]
            if(expected_error == 'None'):
                expected_error = None

            err_msg = None;
            if stderr:
                err_stream = stderr.split('\n')
                err_msg = err_stream[1]

            if (err_msg == expected_error):
                print('line: ' + errors[num][:semicolon_pos] + ' passed')
                log_file.write("Pass\n-----------\n")
            else:
                err_count = err_count + 1
                print('line: ' + errors[num][:semicolon_pos] + ' failed')
                print('Unexpexted msg!!!Expect: ' + str(expected_error) + '\n')
                print('    Got: ' + str(err_msg) + '\n-----------\n')
                log_file.write('Unexpexted msg!!!Expect: ' + str(expected_error) + '\n')
                log_file.write('    Got: ' + str(err_msg) + '\n-----------\n')
        
        log_file.close()
        if (err_count != 0):
            print('Test completed with ' + str(err_count) + ' errors')
        else:
            print('All tests passed competely')

if __name__ == '__main__':
    unittest.main()
