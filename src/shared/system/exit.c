#include "PR/os_internal.h"

#ifndef _FINALROM

void osExit() {
    __osGIOInterrupt(16);

    for (;;) {
        ;
    }
}

#endif
