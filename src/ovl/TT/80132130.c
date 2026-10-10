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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132B88);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132C24);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132130", func_80132CCC);
