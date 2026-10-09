#ifndef OVL_TT_H
#define OVL_TT_H

#include "common.h"
#include "libgpu.h"
extern u8 D_80156784;
extern u8 *D_80158A74;
void func_80138300(void);
void func_8004111C(void);
typedef struct TtRec2 {
    /* 0x0 */ u8 flag;
    /* 0x1 */ u8 pad;
    /* 0x2 */ s16 val;
} TtRec2; /* size 0x4 */
extern u8 *D_80158A6C;
extern u8 *D_80158AA4;
extern u8 *D_80158A8C;
extern u8 *D_80158A78;
extern u8 *D_80158A94;
extern u8 *D_80158A98;
void func_8014D260(s32 arg0, s32 arg1);
void func_800AE080(u8 *arg0, s32 arg1);
extern u8 *D_80158A60;
extern u8 *D_80158AB4;
void func_8013E320(void);
extern s32 D_8014E34C;
void func_800AE0A0(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_8015694C[];
void func_8009C884(RECT *rect, void *arg1);

#endif /* OVL_TT_H */
