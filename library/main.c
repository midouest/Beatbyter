#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "glue.h"

PlaydateAPI *pd;

#ifdef _WINDLL
__declspec(dllexport)
#endif
    int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInitLua)
    {
        pd = playdate;
        registerBytebeat();
    }
    return 0;
}
