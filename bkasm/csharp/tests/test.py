#!/usr/bin/env python3
import unittest
from os.path import abspath, dirname, join
import sys

TESTS_DIR = dirname(abspath(__file__))
sys.path.insert(0, TESTS_DIR)

if __name__ == '__main__':
    loader = unittest.TestLoader()
    suite = loader.discover(TESTS_DIR, pattern='test_*.py')
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    sys.exit(0 if result.wasSuccessful() else 1)
