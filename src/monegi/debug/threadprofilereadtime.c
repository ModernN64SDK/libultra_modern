#ifndef _FINALROM
#include "osint_debug.h"

OSTime osThreadProfileReadTime(OSId id) {
    OSTime adjust = 0;
    u32 now_time = osGetCount();

#ifndef NDEBUG
    if (!__osThprofFlag) {
        __osError(ERR_OSTHPROFILEREADTIME_FLAG, 0);
        return 0;
    }
#endif
    if (id >= THPROF_IDMAX) {
#ifndef NDEBUG
        __osError(ERR_OSTHPROFILEREADTIME_LAR, 1, id);
#endif
        return 0;
    }

    if (id == osGetThreadId(NULL) && __osThprofFunc != NULL) {
        adjust = now_time - __osThprofLastTimer;
    }
    return thprof[id].time + adjust;
}

OSTime osThreadProfileReadTimeTh(OSThread* thread) {
    OSId id;
    OSTime adjust = 0;
    u32 now_time = osGetCount();

#ifndef NDEBUG
    if (!__osThprofFlag) {
        __osError(ERR_OSTHPROFILEREADTIMETH_FLAG, 0);
        return 0;
    }
#endif

    id = osGetThreadId(thread);

    if (id >= THPROF_IDMAX) {
#ifndef NDEBUG
        __osError(ERR_OSTHPROFILEREADTIMETH_LAR, 1, id);
#endif
        return 0;
    }

    if (id == osGetThreadId(NULL) && __osThprofFunc != NULL) {
        adjust = now_time - __osThprofLastTimer;
    }
    return thprof[id].time + adjust;
}
#endif
