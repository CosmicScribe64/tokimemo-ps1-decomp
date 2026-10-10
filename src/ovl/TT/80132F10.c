#include "common.h"
#include "ovl/TT.h"

u8 *func_80132F10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x78;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_80132F4C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_80132F80);

void func_80132FC8(u8 *arg0, u16 arg1, s32 arg2) {
    arg0[0] = 1;
    arg0[0x51] = 0;
    arg0[0x64] = 0;
    *(u16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 0x58) = 0xA;
    *(s32 *)(arg0 + 0x38) = (&D_8014E34C)[arg2];
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x56) = 0;
    switch (arg1) {
    case 0x60:
        *(s16 *)(arg0 + 0x5A) = 0x3CC7;
        break;
    case 0x61:
    case 0x62:
        *(s16 *)(arg0 + 0x5A) = 0x3CC8;
        break;
    case 0x63:
        *(s16 *)(arg0 + 0x5A) = 0x3CC9;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_80133058);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801330D8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_80133288);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801333D4);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132F10", func_801336F8);
