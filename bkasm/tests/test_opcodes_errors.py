#!/usr/bin/env python3
"""Verify assembly/link errors for opcodes.

Reads test_opcodes_errors.txt, feeds each line
to bkasm via stdin and compares stderr with expected error.
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
ERRORS_PATH = abspath(join(dirname(__file__), "test_opcodes_errors.txt"))


def run_assembler(asm_source: str):
    """Run bkasm with asm source via stdin (prog.bin next to binary)."""
    proc = subprocess.Popen(
        [BIN_PATH],
        cwd=BIN_DIR,
        stdin=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
    )
    _, stderr = proc.communicate(input=asm_source.encode())
    return stderr.decode(), proc.returncode


def main():
    if not os.path.exists(BIN_PATH):
        print(f"ERROR: assembler not found at {BIN_PATH}")
        sys.exit(1)
    if not os.path.exists(ERRORS_PATH):
        print(f"ERROR: test file not found at {ERRORS_PATH}")
        sys.exit(1)

    passed = 0
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

            stderr, _ = run_assembler(instr_part)

            err_msg = None
            if stderr:
                err_stream = stderr.strip().split("\n")
                if len(err_stream) >= 2:
                    err_msg = err_stream[1]
                else:
                    err_msg = err_stream[0]

            if err_msg == expected_error or (expected_error is None and not stderr):
                print(f"OK   line {line_num}: {instr_part}")
                passed += 1
            else:
                print(f"FAIL line {line_num}: {instr_part}")
                print(f"      expected: {expected_error}")
                print(f"      got:      {err_msg}")
                failed += 1

    print(f"\n{'='*40}")
    print(f"Total: {passed + failed}, passed: {passed}, failed: {failed}")
    if failed:
        # clean up prog.bin if left
        prog_bin = "prog.bin"
        if os.path.exists(prog_bin):
            os.unlink(prog_bin)
        sys.exit(1)


if __name__ == "__main__":
    main()
