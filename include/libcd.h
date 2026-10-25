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

/* libcd StHEADER, the sector header StGetNext returns (PsyQ movie sample layout). T-17014 */
typedef struct StHEADER {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 type;
    /* 0x04 */ u16 secCount;
    /* 0x06 */ u16 nSectors;
    /* 0x08 */ u32 frameCount;
    /* 0x0C */ u32 frameSize;
    /* 0x10 */ u16 width;
    /* 0x12 */ u16 height;
    /* 0x14 */ u32 headm;
    /* 0x18 */ u32 headv;
} StHEADER; /* size 0x1C */

extern u8 D_80089C84;
s32 func_80087954(s32 arg0, u8 *arg1);
s32 func_800879D0();
s32 func_80087C64();
/* Four-byte CD mixer input; wrapper returns 1 (asm/libcd_rest.s, T-24062). */
s32 func_80087DC8(u8 *arg0);
s32 func_80087E4C(s32 arg0, u8 *arg1);
s32 func_80087E6C(s32 arg0, s32 arg1, s32 arg2);
void func_80088070();
s32 func_800880C0();
void func_80088150(u8 *p, s32 n);
void func_80088180(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80089200();
void func_80089244();
void D_800896E0();
void D_800898F0();
void D_80089B6C();
void D_80089BD4();
void func_8008A0D4(s32 arg0);
void func_8008A148();
#ifndef MAIN_API_OVERRIDE_func_8008B750
void func_8008B750();
#endif

#endif /* LIBCD_H */
