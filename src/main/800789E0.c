#include "common.h"
#include "game.h"

void func_800789E0(void) {
    D_800B6D30 = 0;
    D_800B6D34 = 0;
    D_800B6D38 = 0;
    D_800B6D3C = 0;
    D_800B6D40 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078A0C);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078A94);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078C48);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078FE0);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079014);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079070);

s32 func_80079524(void) {
    if (D_800E7388 == 0xD0 || D_800E7388 == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (D_800E7388 == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (D_800E7388 == 0x21) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_8007959C);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079800);
