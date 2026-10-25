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

/* PsyQ 3.x Gouraud triangle initializer (SetPolyG3): length 6, command 0x30.
 * Preserve the SDK symbol from asm/libgpu.s and the original K&R call view. T-24502 */
void func_8009F004();

void func_8009C210(s32 arg0);
void func_8009C5E0();
void func_8009C674(s32 arg0);
void func_8009C7F8(RECT *rect, s32 arg1, s32 arg2, s32 arg3);
void func_8009C884(RECT *rect, void *arg1);
void func_8009C8E0(RECT *rect, void *arg1);
void func_8009C93C(RECT *rect, s32 arg1, s32 arg2);
void func_8009CC04();
void func_8009CCBC();
void func_8009CCF4();
void func_8009D10C();
void func_8009D294(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_8009ECB0(s32 a, s32 b, s32 c, s32 d);
s32 func_8009ED74();
void AddPrim(void *ot, s32 prim);
void SetSemiTrans();
void SetShadeTex();
void func_8009F02C();
void func_8009F054();
void func_8009F0A4();
void func_8009F0B8(u8 *p);
void func_8009F0F4();

#endif /* LIBGPU_H */
