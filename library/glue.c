#include "pd_api.h"
#include "glue.h"
#include "bytebeat.h"
#include "op.h"

extern PlaydateAPI *pd;

typedef struct Glue
{
    Bytebeat *bytebeat;
    SoundChannel *channel;
    SoundSource *source;
} Glue;

#define BYTEBEAT_TYPE "bytebeat"

static int GlueNew(lua_State *L)
{
    Glue *glue = malloc(sizeof(Glue));
    glue->bytebeat = BytebeatAlloc();
    glue->channel = pd->sound->channel->newChannel();
    pd->sound->addChannel(glue->channel);
    pd->sound->channel->setVolume(glue->channel, 1.0);
    glue->source = NULL;
    pd->lua->pushObject(glue, BYTEBEAT_TYPE, 0);
    return 1;
}

static int GlueGC(lua_State *L)
{
    Glue *glue = pd->lua->getArgObject(1, BYTEBEAT_TYPE, NULL);
    if (glue->source != NULL)
    {
        pd->sound->channel->removeSource(glue->channel, glue->source);
    }
    pd->sound->channel->freeChannel(glue->channel);
    BytebeatFree(glue->bytebeat);
    free(glue);
    return 0;
}

static int GlueExec(lua_State *L)
{
    Glue *glue = pd->lua->getArgObject(1, BYTEBEAT_TYPE, NULL);
    const char *input = pd->lua->getArgString(2);

    int len;
    Op *program = parse(input, &len);
    if (len == 0)
    {
        pd->system->logToConsole("Failed to parse program: %s", input);
        return 0;
    }

    BytebeatSetProgram(glue->bytebeat, program, len);
    return 0;
}

static int GlueStart(lua_State *L)
{
    Glue *glue = pd->lua->getArgObject(1, BYTEBEAT_TYPE, NULL);
    BytebeatReset(glue->bytebeat);
    glue->source = BytebeatAdd(glue->bytebeat, glue->channel);
    return 0;
}

static int GlueStop(lua_State *L)
{

    Glue *glue = pd->lua->getArgObject(1, BYTEBEAT_TYPE, NULL);
    if (glue->source != NULL)
    {
        pd->sound->channel->removeSource(glue->channel, glue->source);
        glue->source = NULL;
    }
    return 0;
}

static int GlueReset(lua_State *L)
{
    Glue *glue = pd->lua->getArgObject(1, BYTEBEAT_TYPE, NULL);
    BytebeatReset(glue->bytebeat);
    return 0;
}

static const lua_reg glueLib[] = {
    {"new", GlueNew},
    {"__gc", GlueGC},
    {"exec", GlueExec},
    {"start", GlueStart},
    {"stop", GlueStop},
    {"reset", GlueReset},
    {NULL, NULL}};

void registerBytebeat(void)
{
    const char *err;
    if (!pd->lua->registerClass(BYTEBEAT_TYPE, glueLib, NULL, 0, &err))
    {
        pd->system->logToConsole("%s:%i: registerClass failed, %s", __FILE__, __LINE__, err);
    }
}
