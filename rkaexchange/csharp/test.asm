SIMPLEBASIC; ПРОСТОЙ ИНТЕРПРЕТАТОР BASIC
        JMP START

;             КОНСТАНТЫ И СТРОКИ
ENDL:   DB   13, 10, 0
MSGSYR: DB   13, 10, 'SYNTAX ERROR',0
MSGUKEY:DB   'UNKNOWN KEYWORD',0
MSGVKEY:DB   'VALID KEYWORD', 13, 10, 0

;             НАЧАЛО ГЛАВНОЙ ФУНКЦИИ

START:
        LXI  H, SOURCE
EXEIT:  CALL EXECLINE
        MOV  A, M
        CPI  255
        JNZ  EXEIT
        JMP  MONIT
; ***********   КОНЕЦ ГЛАВНОЙ ФУНКЦИИ   ************

; ***********   ИСКЛЮЧИТЕЛЬНЫЕ СИТУАЦИИ  ************
SYNTER:
        LXI  H, MSGSYR
        CALL PUTS
        JMP  MONIT

UNKKEY:                 ; UNKNOWN KEYWORD 
;                         NO RETURN PROCEDURE
        MVI  C, 13
        CALL PUTC
        MVI  C, 10
        CALL PUTC
        LDA  CURLINE
        CALL PRHEX
        MVI  C, ' '
        CALL PUTC
        LXI  H, MSGUKEY
        CALL PUTS
        JMP  MONIT

EXECLINE:       ; EXECUTE CURRENT LINE
;                 IN:
;                    HL => CURRENT LINE
;                 OUT:
;                    HL => NEXT LINE

        CALL RNUM       ; READ NUMBER OF LINE
        STA  CURLINE    ; STORE NUMBER OF LINE
        CALL SKIPSP
        CALL READKEYWORD
        JNZ  UNKKEY
        CALL VALKEY
        RET

VALKEY:               ;  CALL (DE)
;                         IN:
;                            DE => KEY PROCEDURE
;                         OUT:
;                            HL => NEXT LINE
        XCHG
        PCHL
        RET                     ; NEVER REACHED

READKEYWORD:    ; READ KEYWORD
;                         IN:
;                            HL => CURRENT CHAR IN LINE
;                         OUT
;                            HL => CHAR AFTER IDENTIFIER
;                            DE => ADDRES OF PROCEDURE

        CALL READIDENT
        SHLD CURCHR
        XCHG
        MOV  A, C               ; A = LENGTH
        CPI  3
        JZ   KEY3
        CPI  5
        JZ   KEY5
        JMP  UNKKEY
KEY3:
FREM:   LXI  D, KWREM-3
        CALL STRCMP
        JNZ  FCLS
        INX  D
        LHLD CURCHR
        RET
FCLS:   LXI  D, KWCLS-3
        CALL STRCMP
        JNZ  FEND
        INX  D
        LHLD CURCHR
        RET
FEND:   LXI  D, KWEND-3
        CALL STRCMP
        JNZ  UNKKEY
        INX  D
        LHLD CURCHR
        RET
KEY5:
FPRINT: LXI  D, KWPRINT-5
        CALL STRCMP
        JNZ  UNKKEY
        INX  D
        LHLD CURCHR
        RET

READIDENT:      ; READ IDENTIFIER
;                 IN:
;                    HL => CURRENT CHAR IN LINE
;                 OUT:
;                    HL => CHAR AFTER IDENTIFIER
;                    BC =  LENGTH B=0 ALWAYS
;                    DE => IDENTIFIER

        MOV  D, H
        MOV  E, L
        LXI  B, 0
IDRD:   MOV  A, M
        CPI  ' '
        RZ
        CPI  'A'
        JC   NOLTR
        CPI  'Z'+1
        JNC  NOLTR
        JMP  VALID
VALID:
        MVI  A, 1
        ADD  C
        JC   SYNTER
        MOV  C, A
        INX  H
        JMP  IDRD
NOLTR:
        MOV  A, C
        ORA  A
        JZ   SYNTER
        MOV  A, M
        CPI  '0'
        RC
        CPI  '9'+1
        RNC
        JMP  VALID       ; JUMP CONTINUE IDENT VERIFY

;       READ WHITESPACE - CHECK AT LEAST ONE
;       SKIP WHITESPACE - SKIPS WHITESPACE IF PRESENT
;       IN:
;          HL => CURRENT CHAR IN LINE
;       OUT:
;          HL => CHAR AFTER ALL SPACES
READSP: MOV  A, M
        CPI  ' '
        JNZ   SYNTER
NXTSPC: INR  C
        INX  H
SKIPSP: MOV  A, M
        CPI  ' '
        JNZ  NWSPCS
        JMP  NXTSPC
NWSPCS: RET

RNUM:    ; READ NUMBER
;           IN:
;               HL => SOURCE LINE
;           OUT:
;               HL => NEXT CHAR IN LINE
;               DE => CURRENT CHAR IN LINE
;               A = NUMBER READED FROM LINE
        PUSH H
        MVI  E, 1       ; E = IS FIRST DIGIT
        PUSH D
        XRA  A          ; ZERO A
NXTD:   MOV  B, A       ; B = CURRENT NUMBER
        CALL RDIG       ; A = CURRENT DIGIT
        JC   ERN        ; CF = IS NOT DIGIT IN A
        POP  D
        MVI  E, 0       ; NOW IS NOT FIRST DIGIT
        PUSH D
        MOV  C, A       ; SAVE DIGIT IN C
        MOV  A, B       ; LOAD CURR NUMBER AND MUL 10
        ADD  A          ; A = PREVDIGIT * 2
        ADD  A          ; A = PREVDIGIT * 4
        ADD  A          ; A = PREVDIGIT * 8
        ADD  B          ; A = PREVDIGIT * 9
        ADD  B          ; A = PREVDIGIT * 10
        ADD  C          ; A = PREVDIGIT * 10 + CURRENTDIGIT
        JMP  NXTD
ERN:
        POP  D
        MOV  A, E
        ORA  A
        JNZ  SYNTER
        MOV  A, B
        POP  D
        RET

RDIG:   ; READ DIGIT
;          IN:
;              HL => SOURCE LINE
;          OUT:
;              HL => NEXT CHAR
;              DE => CURRENT CHAR
;              A = DIGIT
;              CF = CURRENT CHAR IS NOT DIGIT
        PUSH H
        MOV  A, M
        SBI  '0'
        JC   NODIG
        CPI  10
        JNC  NODIG
        INX  H
        ORA  A           ; CLEAR CF
        JMP  ENDRDG
NODIG:  STC
ENDRDG: POP  D
        RET

STRCMP:                 ; COMPARE STRINGS 
;                         IN:
;                            HL => SOURCE STRING
;                            DE => DEST STRING
;                            C = LENGTH
;                         OUT:
;                            Z = 0 IF EQAL
;                            A = NUMBER OF FIRST NON 
;                                EQUAL SIMBOL BEGIN FROM 1

        PUSH H
        MOV  A, C
        CMA
        INR  A
        MOV  B, A
CMPLOO: LDAX D
        CMP  M
        JNZ  CMPEX1
        INR  B
        JZ   CMPEX1
        INX  H
        INX  D
        JMP  CMPLOO
CMPEX1: MOV  A, B
        JZ   CMPEX
        ADD  C
        INR  A
CMPEX:  POP  H
        RET

;            ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ

DATA:
CURCHR:  DW 0
CURLINE:   DB 0
;;;;;;;;;;;;;;;; KEY PROCEDURES
        DB 'REM'
KWREM:
        LHLD CURCHR
REM1:   MVI  B, 13
        CALL BYTFND
        RNZ
        INX  H
        RET

        DB   'END'
KWEND:  JMP  KWREM

        DB 'CLS'
KWCLS:
        MVI  C, 1FH
        CALL PUTC
        MVI  C, 0CH
        CALL PUTC
        LHLD CURCHR
        MVI  B, 13
        CALL BYTFND
        RNZ
        INX  H
        RET

        DB 'PRINT'
KWPRINT: NOP
        LXI  H, KWPRINT-5
        CALL PUTS
        LHLD CURCHR
        MVI  B, 13
        CALL BYTFND
        RNZ
        INX H
        RET

BYTFND:                         ; FIND BYTE IN ARRAY
;                                 IN:
;                                    HL => ARRAY
;                                     B =  VALUE
;                                 OUT:
;                                    HL => BYTE IN ARRAY
FNDLOOP:MOV  A, M
        CPI  255
        JZ   FNDEOF
        CMP  B
        RZ
        INX  H
        JMP  FNDLOOP
FNDEOF: ORA  A
        RET
        RET
DISCARD:      ; НАЧАЛО НЕИНИЦИАЛИЗИРОАВННОЙ ПАМЯТИ

SOURCE: DB '10 REM PROGRAM', 13
        DB '20 CLS', 13
        DB '30 PRINT "HELLO"', 13
        DB '40 END'
        DB 255

;             КОНСТАНТЫ И ПОДПРОГРАММЫ МОНИТОРА
PUTC:   EQU 0F809H
PUTS:   EQU 0F818H
WTCHR:  EQU 0F803H
NWTCHR: EQU 0F81BH
MONIT:  EQU 0F875H
PRHEX:  EQU 0F815H
;             ПОРТЫ
TIMER:  EQU 0EC00H
END                              ; КОНЕЦ ПРОГРАММЫ
