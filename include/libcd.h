#ifndef LIBCD_H
#define LIBCD_H

#include "common.h"

/* PsyQ libcd CdlFILE, the result of CdSearchFile (LIBCD.H, PsyQ SDK 3.x; the SDK headers are
 * not in this repo). T-17014, T-25118 */
typedef struct CdlFILE {
    /* 0x00 */ u8 pos[4];
    /* 0x04 */ u32 size;
    /* 0x08 */ char name[16];
} CdlFILE; /* size 0x18 */

#endif /* LIBCD_H */
