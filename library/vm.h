#pragma once

#include "pd_api.h"
#include "op.h"

typedef struct VM VM;

VM *VMAlloc(void);
void VMFree(VM *vm);
void VMReset(VM *vm);
void VMSetProgram(VM *vm, Op *program, int len);
uint8_t VMEval(VM *vm, int t);
