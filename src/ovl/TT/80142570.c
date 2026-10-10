#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142570);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_801426F0);

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_801427A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142AF0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142C70);
