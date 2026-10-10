#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142570);

void func_801426F0(void) {
    s32 i;
    u8 *p = D_80158A8C;
    u8 *r = D_80158AA4 + 0xC80;

    p[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

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
