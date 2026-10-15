#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
/* The two mouse port buffers share one owned pointer table. T-12010. */
s8 *D_800E36C0[2] = { 0, 0 };
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36C8);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36D0);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36D8);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36DC);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36E0);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36E4);
INCLUDE_RODATA("asm/data/main/80059710.data", D_800E36E8);

s32 MouseState(s32 port, s8 *out) {
    s8 **p;
    s32 result;
    p = &D_800E36C0[port];
    result = 0;
    if ((*p)[0]) {
        result = -1;
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
    } else {
        out[0] = (*p)[4];
        out[1] = (*p)[5];
        out[2] = (*p)[3];
        out[3] = (*p)[1];
    }
    return result;
}

void InitMouse(s8 *arg0, s8 *arg1) {
    D_800E36C0[0] = arg0;
    D_800E36C0[1] = arg1;
}

void RangeMouse(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 sx = D_800E36E8;
    u32 sy = D_800E36EA;

    D_800E36D8 = arg0 * sx;
    D_800E36DC = arg1 * sx;
    D_800E36E0 = arg2 * sy;
    D_800E36E4 = arg3 * sy;
}

void SenseMouse(u16 arg0, u16 arg1) {
    D_800E36E8 = arg0;
    D_800E36EA = arg1;
}

void SetMouse(s32 arg0, u32 arg1, u32 arg2) {
    D_800E36C8[arg0] = D_800E36E8 * arg1;
    D_800E36D0[arg0] = D_800E36EA * arg2;
}

typedef struct MouseOut {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ s32 unk_8;
} MouseOut; /* size 0xC */

void func_80059860(s32 arg0, MouseOut *arg1) {
    s8 *p;

    func_80059938();
    arg1->unk_0 = (u32)D_800E36C8[arg0] / D_800E36E8;
    arg1->unk_4 = (u32)D_800E36D0[arg0] / D_800E36EA;
    arg1->unk_8 = -0x100;
    p = D_800E36C0[arg0];
    if ((p[0] == 0) && (p[1] == 0x12)) {
        arg1->unk_8 = ~p[3] & 0xC;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059938);
