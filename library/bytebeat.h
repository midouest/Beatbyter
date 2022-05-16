#pragma once

#include "pd_api.h"
#include "op.h"

typedef struct Bytebeat Bytebeat;

Bytebeat *BytebeatAlloc(void);
void BytebeatFree(Bytebeat *bytebeat);
void BytebeatSetProgram(Bytebeat *bytebeat, Op *program, int len);
void BytebeatReset(Bytebeat *bytebeat);
SoundSource *BytebeatAdd(Bytebeat *bytebeat, SoundChannel *channel);
