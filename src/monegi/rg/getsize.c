#include "PR/region.h"
#include "PR/ultraerror.h"
#include "assert.h"

s32 osGetRegionBufSize(void* region) {
    register OSRegion* rp = region;

#ifdef _DEBUG
    assert(rp != NULL);
    if (((char*)rp + ALIGN(sizeof(OSRegion), rp->r_alignSize)) != rp->r_startBufferAddress) {
        __osError(ERR_OSGETREGIONBUFSIZE, 1, region);
        return 0;
    }
#endif

    return rp->r_bufferSize;
}
