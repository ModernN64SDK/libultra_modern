#include "osint_debug.h"

u32 osThreadProfileReadCount(OSId id) {
#ifndef NDEBUG
    if (!__osThprofFlag) {
        __osError(ERR_OSTHPROFILEREADCOUNT_FLAG, 0);
        return 0;
    }
#endif

    if (id >= THPROF_IDMAX) {
#ifndef NDEBUG
        __osError(ERR_OSTHPROFILEREADCOUNT_LAR, 1, id);
#endif
        return 0;
    }
    return thprof[id].count;
}

u32 osThreadProfileReadCountTh(OSThread* thread) {
    OSId id;

#ifndef NDEBUG
    if (!__osThprofFlag) {
        __osError(ERR_OSTHPROFILEREADCOUNTTH_FLAG, 0);
        return 0;
    }
#endif

    id = osGetThreadId(thread);

    if (id >= THPROF_IDMAX) {
#ifndef NDEBUG
        __osError(ERR_OSTHPROFILEREADCOUNTTH_LAR, 1, id);
#endif
        return 0;
    }
    return thprof[id].count;
}
