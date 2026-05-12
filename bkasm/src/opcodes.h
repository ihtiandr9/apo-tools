#ifndef H_OPCODES_H
#define H_OPCODES_H

enum opRegs
{
    OP_REGB = 0x0,
    OP_REGC = 0x1,
    OP_REGD = 0x2,
    OP_REGE = 0x3,
    OP_REGH = 0x4,
    OP_REGL = 0x5,
    OP_REGM = 0x6,
    OP_REGA = 0x7,
};

enum opRegPairs
{
    OP_REGBC = 0x0,
    OP_REGDE = 0x2,
    OP_REGHL = 0x4,
    OP_REGSP = 0x6,
};

#endif
