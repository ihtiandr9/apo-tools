;ОПИСАНИЕ ПРОГРАММЫ
       JMP  START

;             КОНСТАНТЫ И СТРОКИ
MSG:   DB 10, "Priwet", 13, 10, 0
ENDL:  DB 13, 10, 0

;             НАЧАЛО ГЛАВНОЙ ФУНКЦИИ

START:
        CALL SET192
        LHLD VADDR
        LXI  D, 4
        DAD  D
        MVI  E, 78
        DAD  D
        DAD  D
        DAD  D
        DAD  D
        MVI  B, 63
        MVI  A, 0-78

CC1:
        MOV  M, B
        INR  A
        INX  H
        JNZ  CC1
        PUSH H
        LXI  H, 0-78
        MOV  B, H
        MOV  C, L
        DAD  H
        DAD  B
        MOV  B, H
        MOV  C, L
        POP  H
        CALL DRAWCH
        DAD  B
        CALL DRAWCH
        DAD  B
        CALL DRAWCH
        DAD  B
        CALL DRAWCH

EXIT:        ; КОНЕЦ ГЛАВНОЙ ФУНКЦИИ
        JMP EXIT

DRAWCH:
        LXI  D, 77
        INX  H
        MVI  M, 24H
        INX  H
        MVI  M, 29H
        DAD  D
        MVI  M, 2FH
        INX  H
        MVI  M, 0FH
        DAD  D
        MVI  M, 14H
        INX  H
        MVI  M, 11H
        DAD  D
        MVI  M, 09H
        INX  H
        MVI  M, 24H
        RET

SET192:      ; ПЕРЕКЛЮЧЕНИЕ ВИДЕОРЕЖИМА
        LHLD VADDR
        XCHG
        LXI  H, VG75+1
        MVI  M, 0
        DCX  H
        MVI  M, 77
        MVI  M, 59
        MVI  M, 3
        MVI  M, NREZH
        INR  L
        MVI  M, 023H
        CALL WAITIN
        EI
        LXI  H, VT57+8
        MVI  M, 80H
        MVI  L, 4
        MOV  M, E
        MOV  M, D
        INR  L
        LXI  D, VSIZE+4000H
        MOV  M, E
        MOV  M, D
        MVI  L, 8
        MVI  M, 0A4H
        LXI  H, VG75+1
        MVI  M, 23H
        RET
WAITIN:         ; ПОДПРОГРАММА ОЖИДАНИЯ НАЧАЛА КАДРА
        LXI  H, VG75+1
        MOV  A, M
WAIT1:
        MOV  A, M
        ANI  20H
        JZ   WAIT1
        RET
DISCARD:      ; НАЧАЛО НЕИНИЦИАЛИЗИРОАВННОЙ ПАМЯТИ
;VADDR: DW GFX ;BEGIN OF VIDEO MEM
;VADDR: DB 00H, 30H
VADDR:  DW 3000H
GFX:

;             КОНСТАНТЫ И ПОДПРОГРАММЫ МОНИТОРА
PUTC:   EQU 0F809H
PUTS:   EQU 0F818H
WTCHR:  EQU 0F803H
NWTCHR: EQU 0F81BH
MONIT:  EQU 0F875H
PRHEX:  EQU 0F815H
NREZH:  EQU 053H
VSIZE:  EQU 4679
BLACK:  EQU 8DH
RED:    EQU 8CH
GREEN:  EQU 85H
BLUE:   EQU 89H
YELLOW: EQU 84H
PINK:   EQU 88H
AQUA:   EQU 81H
WHIT:   EQU 80H
;             ПОРТЫ
TIMER:  EQU 0EC00H
VG75:   EQU 0EF00H
VT57:   EQU 0F000H

END
;                  КОНЕЦ ПРОГРАММЫ
