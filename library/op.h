#pragma once

typedef enum Op
{
    Push = 0,
    T,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Inc,
    Dec,
    Eq,
    Neq,
    Gt,
    Lt,
    Gte,
    Lte,
    And,
    Or,
    Not,
    Band,
    Bor,
    Bxor,
    Bnot,
    Bsl,
    Bsr,
} Op;

Op *parse(const char *input, int *len);
