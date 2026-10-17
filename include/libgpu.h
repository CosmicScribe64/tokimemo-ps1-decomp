#ifndef LIBGPU_H
#define LIBGPU_H

#include "common.h"

/* PsyQ libgpu RECT (LIBGPU.H, PsyQ SDK 3.x; the SDK headers are not in this
 * repo). Frame-memory rectangle in 16-bit pixels. */
typedef struct RECT {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
} RECT; /* size 0x08 */

#endif /* LIBGPU_H */
