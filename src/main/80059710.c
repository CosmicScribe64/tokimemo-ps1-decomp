#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059710);

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_800597A0);

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_800597B0);

void func_80059808(u16 arg0, u16 arg1) {
    D_800E36E8 = arg0;
    D_800E36EA = arg1;
}

void func_8005981C(s32 arg0, u32 arg1, u32 arg2) {
    D_800E36C8[arg0] = D_800E36E8 * arg1;
    D_800E36D0[arg0] = D_800E36EA * arg2;
}

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059860);

INCLUDE_ASM("asm/nonmatchings/main/80059710", func_80059938);
