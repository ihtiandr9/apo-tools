# BKASM TODO

## ✓ Выполнено

- ~~**JM** — перенесён в группу 3-байтовых инструкций.~~
- ~~**RST** — вынесен в отдельный case: `opcode |= (n << 3)`, size=1.~~
- ~~**DW** — добавлен case TOK_DW в `codegen_generate`: пишет 2 байта без opcode.~~
- ~~**Комментарии размера** — исправлены; добавлены описания операндов.~~
- ~~**NULL check immediate_param** в DB/DW — добавлена проверка с ошибкой.~~
- ~~**fall through** — добавлен комментарий.~~
- ~~**is_register_pair()** — переписана, использует `opcodes.h`.~~
- ~~**Сдвиг регистровых пар** — значения в lexer.c изменены (BC=0, DE=2, HL=4, SP=6).~~
- ~~**TOK_PSW** — добавлен как отдельный регистр для PUSH/POP.~~

## P0 — Correctness (wrong code = unusable)

- [x] **Fix ORG** — `size = code_org` → `size = 0`; pc выставляется в `codegen_link`.
- [x] **Проверка диапазона immediate** — RST 0-7, DB 0-255, size=2 0-255 в `codegen_generate`+`evaluate_ast`.
- [x] **Проверка границ буфера `prog[]`** — `pc - code_org + size > MAX_PROG_SIZE` в `codegen_generate`.
- [x] **Безопасное копирование в хеш-таблицу** — `strncpy` + null-terminate в `hashtbl.c:22`.
- [x] **NULL-безопасность в mathexpr.c** — `lparam`/`rparam` проверяются в `math_evaluate_multiplication` и `math_evaluate_addition`.

## P1 — Usability (нельзя использовать всерьёз)

- ~~**Поддержка строчных букв** — неактуально: архитектура ohne Kleinbuchstaben.~~
- [x] **`--help` / `-h`** — список опций, краткая справка в `main.c`.
- [x] **Именованный выходной файл** — `argv[2]` пишется через `outfile`, иначе `prog.bin`.
- [x] **Увеличить `MAX_PROG_SIZE`** — 4096 → 65536 в `bkasm.h`.
- [x] **Добавить `.gitignore`** — `Build/*`, `prog.bin`, `test_log.txt`, `*.o`, `*.lst`.

## P2 — Missing features

- [ ] **DS** — резервирование памяти без инициализации (`DS 100`).
- [ ] **INCLUDE** — включение других файлов исходника.
- [ ] **Макросы** — `MACRO`/`ENDM`.
- [ ] **Условная сборка** — `IF`/`ELSE`/`ENDIF`, `IFDEF`.
- [ ] **Скобки в выражениях** — `(1+2)*3`.
- [ ] **SET** — переопределяемые символы (в отличие от EQU).
- [ ] **Listing file** — листинг с адресами и байтами.
- [ ] **Таблица символов** — опция вывода `--sym`.

## P3 — Nice to have

- [x] **Авто-тесты на ORG, EQU, range checking** — test_org_equ_range.py + граничные значения в test_opcodes.asm.
- [x] **Проверка переопределения меток** — `asmvars_add` в `PARSE_STAGE` кидает `Duplicate label`.
- [x] **Valgrind в CI** — `.github/workflows/valgrind.yml` с `--error-exitcode=1`.
