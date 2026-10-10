#include "common.h"
#include "ovl/TT.h"

void func_80132130(void) {
    u8 *p = D_80158AA4;

    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x44) = 8;
    *(s16 *)(p + 0x14) = -0x38;
    *(s16 *)(p + 0x16) = 0x48;
    *(s16 *)(p + 0x3C) = 0x70;
    *(s16 *)(p + 0x3E) = 0x10;
    p[0x46] = 0;
    p[0x47] = 0x60;
    *(s16 *)(p + 0x40) = 0xA;
    *(s16 *)(p + 0x42) = 0x3C08;
    p[0x3B] = 0x65;
    *(s16 *)(p + 0x10) = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132198);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_8013230C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132810);

void func_80132B88(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    p[0x1D] += 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] >= 0x80U) {
        q = D_80158AA8;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(p + 2) = 0x40;
    }
    func_80132810();
}

void func_80132C24(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    if (p[0x1D] == 0x80U) {
        i = 0;
        q = D_80158AA8;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    p[0x1D] -= 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] == 0) {
        p[9] = 1;
    }
    func_80132810();
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132CCC);
