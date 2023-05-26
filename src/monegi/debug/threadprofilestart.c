#ifndef _FINALROM
#include "osint_debug.h"

void osThreadProfileStart(void) {
    register u32 saveMask;

#ifndef NDEBUG
    if (!__osThprofFlag) {
        __osError(ERR_OSTHPROFILESTART_FLAG, 0);
        return;
    }
    if (__osThprofFunc != NULL) {
        __osError(ERR_OSTHPROFILESTART_START, 0);
        return;
    }
#endif

    saveMask = __osDisableInt();

    __osThprofLastTimer = osGetCount();
    __osThprofFunc = osThreadProfileCallback;
    __osRestoreInt(saveMask);
}
#endif
