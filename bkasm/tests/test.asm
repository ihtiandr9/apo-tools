; ОПИСАНИЕ ПРОГРАММЫ
ORG 1100H
  JMP START

;             КОНСТАНТЫ И СТРОКИ
ENDL: 
        DB 13, 10, 0

;             НАЧАЛО ГЛАВНОЙ ФУНКЦИИ

START:
EXIT:        ; КОНЕЦ ГЛАВНОЙ ФУНКЦИИ

SET192:      ; ПЕРЕКЛЮЧЕНИЕ ВИДЕОРЕЖИМА
        LXI  H, VG75+1
        MVI  M, 0
        DCX  H
        MVI  M, 77
        MVI  M, 59
        MVI  M, 3
        MVI  M, NREZH
        INR  L
        MVI  M, 23
        CALL WAITIN
        EI
        LXI  H, VT75+8
        MVI  M, 80
        MVI  L, 4
        LHLD VADDR
        XCHG
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

;             КОНСТАНТЫ И ПОДПРОГРАММЫ МОНИТОРА
PUTC:   EQU 0F809H
PUTS:   EQU 0F818H
WTCHR:  EQU 0F803H
NWTCHR: EQU 0F81BH
MONIT:  EQU 0F875H
PRHEX:  EQU 0F815H
;             ПОРТЫ
TIMER:  EQU 0EC00H
END
;                  КОНЕЦ ПРОГРАММЫ
