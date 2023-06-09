#include "PR/os_internal.h"
#include "osint.h"

void osStopThread(OSThread* t) {
    register u32 saveMask = __osDisableInt();
    register u16 state;

    state = (t != NULL) ? t->state : OS_STATE_RUNNING;

    switch (state) {
        case OS_STATE_RUNNING:
            __osRunningThread->state = OS_STATE_STOPPED;
            __osEnqueueAndYield(NULL);
            break;
        case OS_STATE_RUNNABLE:
        case OS_STATE_WAITING:
            t->state = OS_STATE_STOPPED;
            __osDequeueThread(t->queue, t);
            break;
    }

    __osRestoreInt(saveMask);
}
