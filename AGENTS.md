# apo-tools

Tools for cross-compilation from x86 to BK-0010 (KR850). Two independent C tools: `bkasm` (assembler) and `rkaexchange` (tape format converter).

## Build

### bkasm
```bash
cd bkasm/Build && cmake .. && make        # CMake (output: build/bkasm)
# or
cd bkasm && make -f Build/Makefile.gnu   # Direct gcc (output: Build/GNU.MinGW/bkasm)
```

### rkaexchange
```bash
cd rkaexchange && cmake . && make         # Output: rkaexchange executable
```

Both build debug builds by default (`-ggdb -Wno-switch`). No release flag documented.

## Test

### bkasm
```bash
cd bkasm/build && make unittest          # Single assembly test (tests/test.asm)
# or via CMake: make unittest (runs from build dir)
cd bkasm/build && make run_tests        # Full test suite: Python script tests/tests.py
                                        # Feeds 263 snippets from tests/errors.txt via stdin
                                        # Validates stderr error messages
                                        # Requires Python venv at tests/python/
```

Test format: `errors.txt` lines are `assembly_code;expected_error` (use `;None` for valid code). Error messages start with `\n`.

### rkaexchange
```bash
cd rkaexchange && cmake . && make testpack      # Test pack (asm -> rka)
cd rkaexchange && cmake . && make testunpack   # Test unpack (rka -> asm)
make run_tests                                    # Runs all test targets above
```

Test files live in `rkaexchange/src/` (test.asm, test.rka) — not a separate tests/ directory.

## Architecture

- **bkasm pipeline**: `main.c` → `Lexer` → `Parser` → `codegen_link()` → binary output
  - Stages: `PARSE_STAGE`, `EVAL_STAGE`, `GENERATE_STAGE` (bkasm.h)
  - Uses `STRUCT()` macro for forward-declared typedef structs
  - Source file read into fixed `MAX_PROG_SIZE` (4096) buffer at startup
  - Entry points: stdin/stdout default, optional file args

- **rkaexchange**: `main.c` → `Edb` struct → pack/unpack RKA tape format
  - Commands: `rkaexchange pack <asm> <tape>` or `rkaexchange unpack <tape> <asm>`

## Conventions

- Code is C (bkasm targets C11: `-std=c11 -Wall`)
- Comments and identifiers mix English and Russian
- `exit_nicely(int)` for bkasm termination; `throw_error()` for rkaexchange
- Error codes: `E_SYNTAXERROR`, `E_LINKERERROR`, `E_CMDREQ`, `E_UNKNOWNCMD`, etc.
- No common framework — each tool has its own build system and conventions

## References

- `bkasm/.github/copilot-instructions.md` — detailed bkasm coding guidelines
- `bkasm/README.md`, `rkaexchange/README.md` — usage basics
