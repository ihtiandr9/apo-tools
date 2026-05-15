; test_opcodes.asm -- проверка генерации опкодов i8080
; формат: мнемоника ; ожидаемые байты в hex
;
; Внимание: арифметические выражения (0-78) вычисляются,
; поэтому ожидаемый байт -- результат вычисления.

; === Без операндов ===
    NOP          ; 00
    HLT          ; 76
    DI           ; F3
    EI           ; FB
    RIM          ; 20
    SIM          ; 30
    CMA          ; 2F
    STC          ; 37
    CMC          ; 3F
    RLC          ; 07
    RRC          ; 0F
    RAL          ; 17
    RAR          ; 1F
    DAA          ; 27
    RET          ; C9
    PCHL         ; E9
    SPHL         ; F9
    XCHG         ; EB
    XTHL         ; E3
    RC           ; D8
    RNC          ; D0
    RZ           ; C8
    RNZ          ; C0
    RP           ; F0
    RM           ; F8
    RPE          ; E8
    RPO          ; E0

; === MOV dst, src ===
    MOV A, B     ; 78
    MOV A, C     ; 79
    MOV A, D     ; 7A
    MOV A, E     ; 7B
    MOV A, H     ; 7C
    MOV A, L     ; 7D
    MOV A, M     ; 7E
    MOV A, A     ; 7F
    MOV B, A     ; 47
    MOV C, A     ; 4F
    MOV D, A     ; 57
    MOV E, A     ; 5F
    MOV H, A     ; 67
    MOV L, A     ; 6F
    MOV M, A     ; 77
    MOV B, B     ; 40
    MOV B, C     ; 41
    MOV B, D     ; 42
    MOV B, E     ; 43
    MOV B, H     ; 44
    MOV B, L     ; 45
    MOV B, M     ; 46
    MOV C, B     ; 48
    MOV C, C     ; 49
    MOV C, D     ; 4A
    MOV C, E     ; 4B
    MOV C, H     ; 4C
    MOV C, L     ; 4D
    MOV C, M     ; 4E
    MOV D, B     ; 50
    MOV D, C     ; 51
    MOV D, D     ; 52
    MOV D, E     ; 53
    MOV D, H     ; 54
    MOV D, L     ; 55
    MOV D, M     ; 56
    MOV E, B     ; 58
    MOV E, C     ; 59
    MOV E, D     ; 5A
    MOV E, E     ; 5B
    MOV E, H     ; 5C
    MOV E, L     ; 5D
    MOV E, M     ; 5E
    MOV H, B     ; 60
    MOV H, C     ; 61
    MOV H, D     ; 62
    MOV H, E     ; 63
    MOV H, H     ; 64
    MOV H, L     ; 65
    MOV H, M     ; 66
    MOV L, B     ; 68
    MOV L, C     ; 69
    MOV L, D     ; 6A
    MOV L, E     ; 6B
    MOV L, H     ; 6C
    MOV L, L     ; 6D
    MOV L, M     ; 6E

; === Арифметические с регистром ===
    ADD B        ; 80
    ADD C        ; 81
    ADD D        ; 82
    ADD E        ; 83
    ADD H        ; 84
    ADD L        ; 85
    ADD M        ; 86
    ADD A        ; 87
    ADC B        ; 88
    ADC C        ; 89
    ADC D        ; 8A
    ADC E        ; 8B
    ADC H        ; 8C
    ADC L        ; 8D
    ADC M        ; 8E
    ADC A        ; 8F
    SUB B        ; 90
    SUB C        ; 91
    SUB D        ; 92
    SUB E        ; 93
    SUB H        ; 94
    SUB L        ; 95
    SUB M        ; 96
    SUB A        ; 97
    SBB B        ; 98
    SBB C        ; 99
    SBB D        ; 9A
    SBB E        ; 9B
    SBB H        ; 9C
    SBB L        ; 9D
    SBB M        ; 9E
    SBB A        ; 9F

; === Логические с регистром ===
    ANA B        ; A0
    ANA C        ; A1
    ANA D        ; A2
    ANA E        ; A3
    ANA H        ; A4
    ANA L        ; A5
    ANA M        ; A6
    ANA A        ; A7
    XRA B        ; A8
    XRA C        ; A9
    XRA D        ; AA
    XRA E        ; AB
    XRA H        ; AC
    XRA L        ; AD
    XRA M        ; AE
    XRA A        ; AF
    ORA B        ; B0
    ORA C        ; B1
    ORA D        ; B2
    ORA E        ; B3
    ORA H        ; B4
    ORA L        ; B5
    ORA M        ; B6
    ORA A        ; B7
    CMP B        ; B8
    CMP C        ; B9
    CMP D        ; BA
    CMP E        ; BB
    CMP H        ; BC
    CMP L        ; BD
    CMP M        ; BE
    CMP A        ; BF

; === INR / DCR ===
    INR B        ; 04
    INR C        ; 0C
    INR D        ; 14
    INR E        ; 1C
    INR H        ; 24
    INR L        ; 2C
    INR M        ; 34
    INR A        ; 3C
    DCR B        ; 05
    DCR C        ; 0D
    DCR D        ; 15
    DCR E        ; 1D
    DCR H        ; 25
    DCR L        ; 2D
    DCR M        ; 35
    DCR A        ; 3D

; === Регистровые пары ===
    DAD B        ; 09
    DAD D        ; 19
    DAD H        ; 29
    DAD SP       ; 39
    LDAX B       ; 0A
    LDAX D       ; 1A
    STAX B       ; 02
    STAX D       ; 12
    INX B        ; 03
    INX D        ; 13
    INX H        ; 23
    INX SP       ; 33
    DCX B        ; 0B
    DCX D        ; 1B
    DCX H        ; 2B
    DCX SP       ; 3B
    POP B        ; C1
    POP D        ; D1
    POP H        ; E1
    POP PSW      ; F1
    PUSH B       ; C5
    PUSH D       ; D5
    PUSH H       ; E5
    PUSH PSW     ; F5

; === RST n ===
    RST 0        ; C7
    RST 1        ; CF
    RST 2        ; D7
    RST 3        ; DF
    RST 4        ; E7
    RST 5        ; EF
    RST 6        ; F7
    RST 7        ; FF

; === 2-байтовые (opcode + immediate) ===
    ADI 20H      ; C6 20
    ACI 40H      ; CE 40
    SUI 67H      ; D6 67
    SBI 20H      ; DE 20
    ANI 0AAH     ; E6 AA
    ORI 20H      ; F6 20
    XRI 80H      ; EE 80
    CPI 89H      ; FE 89
    OUT 23H      ; D3 23
    IN 23H       ; DB 23

; === MVI r, data ===
    MVI A, 20H   ; 3E 20
    MVI B, 29H   ; 06 29
    MVI C, 10H   ; 0E 10
    MVI D, 10H   ; 16 10
    MVI E, 0     ; 1E 00
    MVI H, 0     ; 26 00
    MVI L, 20H   ; 2E 20
    MVI M, 20H   ; 36 20

; === 3-байтовые: переходы ===
    JMP 1000H    ; C3 00 10
    JNZ 1000H    ; C2 00 10
    JZ 1000H     ; CA 00 10
    JNC 1000H    ; D2 00 10
    JC 1000H     ; DA 00 10
    JPO 1000H    ; E2 00 10
    JPE 1000H    ; EA 00 10
    JP 1000H     ; F2 00 10
    JM 1000H     ; FA 00 10

; === 3-байтовые: вызовы ===
    CALL 1000H   ; CD 00 10
    CNZ 1000H    ; C4 00 10
    CZ 1000H     ; CC 00 10
    CNC 1000H    ; D4 00 10
    CC 1000H     ; DC 00 10
    CPO 1000H    ; E4 00 10
    CPE 1000H    ; EC 00 10
    CP 1000H     ; F4 00 10
    CM 1000H     ; FC 00 10

; === 3-байтовые: память ===
    LDA 1000H    ; 3A 00 10
    STA 1000H    ; 32 00 10
    LHLD 1000H   ; 2A 00 10
    SHLD 1000H   ; 22 00 10

; === 3-байтовые: LXI rp, data ===
    LXI B, 1234H  ; 01 34 12
    LXI D, 1234H  ; 11 34 12
    LXI H, 1234H  ; 21 34 12
    LXI SP, 1234H ; 31 34 12

; === Математические выражения в immediate ===
    ADI 10H+20H   ; C6 30
    ADI 40H-10H   ; C6 30
    SUI 20H+8     ; D6 28
    SBI 100-10H   ; DE 54
    ANI 0FFH-0F0H ; E6 0F
    ORI 0FH+10H   ; F6 1F
    MVI A, 10H+2FH ; 3E 3F
    MVI B, 8+7    ; 06 0F
    DB 10H+20H    ; 30
    DB 40H-10H    ; 30
    DW 1000H+2000H ; 00 30
    DW 3000H-1000H ; 00 20
    JMP 1000H+2000H ; C3 00 30
    JMP 2000H-1000H ; C3 00 10
    LDA 1000H+500H  ; 3A 00 15
    STA 2000H+1000H ; 32 00 30
    START: STA START+1000H ; 32 00 10

; === Метки + математические выражения ===
    START: JMP START+2000H   ; C3 00 20
    START: LDA START+1000H+2000H ; 3A 00 30
    START: JMP START+2*1000H ; C3 00 20

; === Приоритет операций ===
    ADI 10+2*3      ; C6 10
    ADI 2*3+10      ; C6 10
    ADI 10+10-5     ; C6 0F
    ADI 20-10-5     ; C6 05
