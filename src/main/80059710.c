#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80059710", MouseState);

INCLUDE_ASM("asm/nonmatchings/main/80059710", InitMouse);

INCLUDE_ASM("asm/nonmatchings/main/80059710", RangeMouse);

void SenseMouse(u16 arg0, u16 arg1) {
    D_800E36E8 = arg0;
    D_800E36EA = arg1;
}

void SetMouse(s32 arg0, u32 arg1, u32 arg2) {
    D_800E36C8[arg0] = D_800E36E8 * arg1;
    D_800E36D0[arg0] = D_800E36EA * arg2;
}

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059860);

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059938);
