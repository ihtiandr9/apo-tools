# rkaexchange

RKA tape format converter for BK-0010. Converts between assembly listings and RKA tape format.
Work only within current directory — do not modify files outside this folder.

## Build

```bash
cmake . && make
```

Output: `rkaexchange` executable in current directory.
Debug build by default (`-ggdb -Wno-switch`).

## Test

```bash
make testpack      # Test pack (asm -> rka) using src/test.asm
make testunpack   # Test unpack (rka -> asm) using src/test.rka
make run_tests    # Runs all test targets above
```

Test files live in `src/` (test.asm, test.rka, errtest.asm) — no separate tests/ directory.

## Architecture

`main.c` → `Edb` struct (edb.h/edb.c) → pack/unpack RKA tape format
- Commands: `rkaexchange pack <asm> <tape>` or `rkaexchange unpack <tape> <asm>`
- Entry points: stdin/stdout default, optional file args
- Error handling: `throw_error()` with codes in errors.h

## Key Files

- `src/main.c` — CLI and file handling
- `src/edb.c` / `src/edb.h` — Core pack/unpack logic
- `src/inbuf.c` / `src/inbuf.h` — Input buffer
- `src/errors.c` / `src/errors.h` — Error codes and messages
- `src/globals.c` / `src/globals.h` — Global state (fd_in, fd_out)
- `CMakeLists.txt` — Build and test targets

## Conventions

- C code, comments and identifiers mix English and Russian
- `throw_error()` for termination with error message
- `exit_nicely(int)` for clean exit
- Error codes: `E_CMDREQ`, `E_UNKNOWNCMD`, `E_NOEXIST`, `E_CREATE`, etc.
