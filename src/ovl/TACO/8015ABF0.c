#include "common.h"
#include "ovl/TACO.h"

void func_8015ABF0(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        D_80161D9C[i] = 0;
        D_80127080[i].unk0 = 0x80000000;
    }
    for (i = 16; i != 63; i++) {
        func_80143B34(i);
        *(s32 *) D_8015EDB4[i].pad4 = 0x80000000;
        D_8015EDB4[i].unk84[2] = 1;
    }
    D_801604C0 = 0;
    D_801604C4 = 0;
    D_801604C8 = 0;
    D_801604CC = 0;
    D_801604D0 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015ACCC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B1B8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B2AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B3B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B59C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BD30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BF70);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C0A0);

void func_8015C208(s32 arg0) {
    s32 i;

    func_8015C0A0(0, arg0);
    D_8015EDB4[arg0].pad0[1] = 0;
    D_8015EDB4[arg0].unk84[2] = 0;
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_80161D9C[i]) {
            D_80161D9C[i] = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C2CC);

void func_8015C698(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        D_8015EDB4[i].unk84[1] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C718);

s32 func_8015CC54(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (D_80161D9C[i] == 0) {
            n += 1;
        }
    }
    return n;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CCC8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CE64);
