#include "common.h"
#include "game.h"

void func_80057390(u8 arg0) {
    D_800B593C = arg0;
}

s32 func_8005739C(void) {
    return D_800B5948;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_800573AC);

void func_800573F8(s32 arg0) {
    D_800B5938[arg0] = 1 - D_800B5938[arg0];
}

void func_80057418(s32 arg0, s32 arg1) {
    D_800B5938[arg0] = arg1 & 1;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_8005742C);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_8005751C);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057640);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057710);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_800578F4);

void func_80057D1C(u8 arg0) {
    D_800B5940 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057D28);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80058398);
