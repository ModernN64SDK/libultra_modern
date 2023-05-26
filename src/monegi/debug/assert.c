#include "PR/os_internal.h"

extern void __assertBreak(void);

void __assert(const char* exp, const char* filename, int line) {
#ifndef _FINALROM
    osSyncPrintf("\nASSERTION FAULT: %s, %d: \"%s\"\n", filename, line, exp);
	__assertBreak();
#endif
}
