#ifndef _FINALROM

#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "osint.h"

#include "macros.h"

typedef struct {
    /* 0x0 */ unsigned int inst1;
    /* 0x4 */ unsigned int inst2;
    /* 0x8 */ unsigned int inst3;
    /* 0xC */ unsigned int inst4;
} __osExceptionVector;

extern __osExceptionVector __isExpJP;

void MonitorInitBreak(void);

typedef struct {
    /* 0x00 */ u32 magic; // IS64
    /* 0x04 */ u32 get;
    /* 0x08 */ u8 unk_08[0x14 - 0x08];
    /* 0x14 */ u32 put;
    /* 0x18 */ u8 unk_18[0x20 - 0x18];
    /* 0x20 */ u8 data[0x10000 - 0x20];
} ISVDbg;

#define IS64_MAGIC  0x49533634  // 'IS64'

ISVDbg* gISVDbgPrnAdrs;

u32 leoComuBuffAdd;
u32 gISVFlag;
u16 gISVChk;

__osExceptionVector ramOldVector ALIGNED(8);

static OSPiHandle* is_Handle;

void isPrintfInit(void) {
    is_Handle = osCartRomInit();

    osEPiWriteIo(is_Handle, &gISVDbgPrnAdrs->put, 0);
    osEPiWriteIo(is_Handle, &gISVDbgPrnAdrs->get, 0);
    osEPiWriteIo(is_Handle, gISVDbgPrnAdrs, IS64_MAGIC);
}

static void* is_proutSyncPrintf(void* arg, const u8* str, u32 count) {
    u32 data;
    s32 p;
    s32 start;
    s32 end;

    if (gISVDbgPrnAdrs == NULL) {
        return 0;
    }

    osEPiReadIo(is_Handle, (u32)&gISVDbgPrnAdrs->magic, &data);
    if (data != IS64_MAGIC) {
        return 1;
    }
    osEPiReadIo(is_Handle, (u32)&gISVDbgPrnAdrs->get, &data);
    p = data;
    osEPiReadIo(is_Handle, (u32)&gISVDbgPrnAdrs->put, &data);

    start = data;
    end = start + count;

    if (end >= 0xffe0) {
        end -= 0xffe0;
        if (p < end || start < p) {
            return 1;
        }
    } else {
        if (start < p && p < end) {
            return 1;
        }
    }
    while (count) {
        if (*str != '\0') {
            s32 shift = start & 3;
            u32 addr = (u32)&gISVDbgPrnAdrs->data[start & 0xFFFFFFC];

            shift = (3 - shift) * 8;

            osEPiReadIo(is_Handle, addr, &data);
            osEPiWriteIo(is_Handle, addr, (data & ~(0xff << shift)) | (*str << shift));

            start++;
            if (start >= 0xffe0) {
                start -= 0xffe0;
            }
        }
        count--;
        str++;
    }
    osEPiWriteIo(is_Handle, (u32)&gISVDbgPrnAdrs->put, start);

    return 1;
}

/* ISViewer buffer */
#define ISVIEWER_BUFFER ((volatile u32 *)0xB3FF0020)

int __checkHardware_isv(void) {
	ISVIEWER_BUFFER[0] = 0x12345678;
	return ISVIEWER_BUFFER[0] == 0x12345678;
}

void __osInitialize_isv(void) {
    void (*fn)(void);
    OSPiHandle* hnd;

    if (gISVFlag == IS64_MAGIC || __checkHardware_isv()) {
        if (gISVDbgPrnAdrs != NULL) {
            __printfunc = is_proutSyncPrintf;
            isPrintfInit();
        }
        if (gISVChk & 2) {
            hnd = osCartRomInit();

            ramOldVector = *(__osExceptionVector*)E_VEC;
            *(__osExceptionVector*)E_VEC = __isExpJP;

            osWritebackDCache(&ramOldVector, 0x10);
            osInvalICache(&ramOldVector, 0x10);
            osWritebackDCache(0x80000000, 0x190);
            osInvalICache(0x80000000, 0x190);
            osEPiReadIo(hnd, 0xBFF00010, &fn);
            fn();
        }
        if (gISVChk & 2) {
            MonitorInitBreak();
        }
    }
}

#endif
