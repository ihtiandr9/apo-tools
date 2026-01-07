# BKASM Assembler - AI Coding Guidelines

## Project Overview
BKASM is a C-based assembler for the BK-0010 retro computer architecture. It processes assembly source files into binary machine code through a three-stage pipeline: lexical analysis → parsing → code generation.

## Architecture
- **Pipeline Flow**: `main.c` orchestrates `Lexer` → `Parser` → `codegen_link()` producing `unsigned char*` binary output
- **Data Structures**: Uses `STRUCT()` macro for forward-declared typedef structs (e.g., `STRUCT(Parser) { ASTree *ast; int level; };`)
- **Stages**: Processing divided into `PARSE_STAGE`, `EVAL_STAGE`, `GENERATE_STAGE` (defined in `bkasm.h`)
- **Memory**: Entire source file read into fixed `MAX_PROG_SIZE` (4096) buffer at startup

## Build System
- **CMake-based**: Standard `cmake .. && make` in `build/` directory
- **Targets**:
  - `make` - Build `bkasm` executable
  - `make unittest` - Run single test on `tests/test.asm`
  - `make run_tests` - Execute Python test suite (`tests/tests.py`)
  - `make debug` - Launch GDB debugger
  - `make valgrind` - Memory leak checking

## Testing
- **Error Testing**: `tests/tests.py` feeds 263 assembly snippets from `errors.txt` to `bkasm` via stdin, validates stderr error messages
- **Format**: Each `errors.txt` line is `assembly code;expected_error` (use `;None` for valid code)
- **Execution**: Tests run `bkasm` as subprocess, compare actual vs expected stderr

## Code Patterns
- **Entry Point**: `main.c` handles file I/O (stdin/stdout default, args for files)
- **Error Handling**: `exit_nicely(int)` for clean termination with error codes
- **Assembly Syntax**: BK-0010 opcodes (NOP, LXI, JMP), ORG directives, labels, hex constants (29H)
- **MOV Instruction**: Opcode template in `node->u.op.opcode` modified with `|=` to encode registers: `(dest_reg << 3) | src_reg` in `codegen_evaluate_ast()`
- **Similar Instructions**: ADD/ADC/ANA/AND/CMP/ORA/SBB/SUB/XRA use `|=` for src_reg; INR/DCR use `|=` for `(reg << 3)`; DAD/LDAX/STAX/POP/PUSH/INX/DCX use `|=` for `(pair << 4)`
- **Comments**: Mix of English and Russian in test files
- **Throw exceptions**: Invoke `throw_error()` function for all syntax/linker errors with appropriate error codes (e.g., `E_SYNTAXERROR`, `E_LINKERERROR`) Error messages will start with a newline character for consistency. 
Pattern: 
`sprintf(err_msg,"\nDescriptive error message"); throw_error(ERROR_CODE, err_msg);`

## Development Workflow
1. Edit source in `src/`
2. `cd build && make` to rebuild
3. `make run_tests` to validate (focus on stderr accuracy)
4. `make debug` for GDB sessions
5. `make valgrind` for memory checks

## Key Files
- `src/main.c` - Program flow and I/O
- `src/parser.c` - AST construction
- `src/codegen.c` - Binary output generation
- `tests/tests.py` - Error message validation
- `CMakeLists.txt` - Build configuration</content>
<parameter name="filePath">/workspaces/apo-tools/bkasm/.github/copilot-instructions.md