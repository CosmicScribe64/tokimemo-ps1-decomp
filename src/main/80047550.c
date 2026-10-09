#include "common.h"
#include "game.h"

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80047560);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800482FC);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048390);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800483E8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048514);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800485BC);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800486A4);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048828);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048A90);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048CF8);

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E7392 = 1;
    } else {
        D_800E7392 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048DD0);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048E78);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048EB8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048F64);

u8 func_8004901C(void) {
    return D_800E62BA;
}

void func_8004902C(void) {
    D_800E62BA = 0x80;
    D_800E62BB = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049044);

void func_800490B4(u8 arg0) {
    D_800E62BB = arg0;
}

void func_800490C0(s32 arg0, s32 arg1) {
    u8 *p = D_800E6280 + arg1 * 12;

    *(s32 *)(p + 0x24) = arg0;
    *(s32 *)(p + 0x1C) = 0x10C00;
    *(s32 *)(p + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800490F0);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049140);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800493A8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049450);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800494BC);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_8004955C);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800495DC);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049A40);
