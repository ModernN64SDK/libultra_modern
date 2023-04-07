#include "PR/region.h"
#include "PR/ultraerror.h"
#include "assert.h"

void* osMalloc(void* region) {
    register OSRegion* rp = region;
    char* addr;

#ifdef _DEBUG
    assert(rp != NULL);
    if (((char*)rp + ALIGN(sizeof(OSRegion), rp->r_alignSize)) != rp->r_startBufferAddress) {
        __osError(ERR_OSMALLOC, 1, region);
        return 0;
    }
#endif

    if (rp->r_freeList == MAX_BUFCOUNT) {
        return NULL;
    }

    addr = &rp->r_startBufferAddress[rp->r_freeList * rp->r_bufferSize];
    rp->r_freeList = *(u16*)addr;
    return addr;
}
