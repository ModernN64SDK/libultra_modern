#include "osint_debug.h"

#ifndef _FINALROM

void osThreadProfileClear(OSId id) {
    register OSIntMask saveMask = __osDisableInt();

    thprof[id].flag = 0;
    thprof[id].count = 0;
    thprof[id].time = 0;

    __osRestoreInt(saveMask);
}

#endif
