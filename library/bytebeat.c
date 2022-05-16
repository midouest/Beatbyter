#include "bytebeat.h"
#include "vm.h"

#include <math.h>
#include <limits.h>

extern PlaydateAPI *pd;

typedef struct Bytebeat
{
    VM *vm;
    uint32_t t;
    int16_t sample;
    double frame;
    double acc;
} Bytebeat;

Bytebeat *BytebeatAlloc()
{
    Bytebeat *bytebeat = malloc(sizeof(Bytebeat));
    bytebeat->vm = VMAlloc();
    bytebeat->t = 0;
    bytebeat->sample = 0;
    bytebeat->frame = -1.0;
    bytebeat->acc = 0.0;
    return bytebeat;
}

void BytebeatFree(Bytebeat *bytebeat)
{
    VMFree(bytebeat->vm);
    free(bytebeat);
}

void BytebeatSetProgram(Bytebeat *bytebeat, Op *program, int len)
{
    VMSetProgram(bytebeat->vm, program, len);
}

void BytebeatReset(Bytebeat *bytebeat)
{
    VMReset(bytebeat->vm);
    bytebeat->t = 0;
    bytebeat->sample = 0;
    bytebeat->frame = -1.0;
    bytebeat->acc = 0.0;
}

#define ACC_DELTA 0.1814058957 // 8khz / 44.1khz

int BytebeatCallback(void *context, int16_t *left, int16_t *right, int len)
{
    Bytebeat *bytebeat = context;
    for (int i = 0; i < len; i++)
    {
        double frame = floor(bytebeat->acc);
        if (frame > bytebeat->frame)
        {
            bytebeat->frame = frame;
            uint8_t byte = VMEval(bytebeat->vm, bytebeat->t);
            bytebeat->t++;
            int16_t sample = UCHAR_MAX * byte - SHRT_MAX;
            bytebeat->sample = sample;
            bytebeat->frame = frame;
        }

        left[i] = bytebeat->sample;
        bytebeat->acc += ACC_DELTA;
    }
    return 1;
}

SoundSource *BytebeatAdd(Bytebeat *bytebeat, SoundChannel *channel)
{
    return pd->sound->channel->addCallbackSource(channel, BytebeatCallback, bytebeat, 0);
}
