#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135360);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_801356C8);

void func_80135848(s32 arg0) {
    D_80158A8C[0x3E] = ((arg0 + 0x40) & 0xFFF) / 128;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135870);

void func_80135A54(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_80135AC4(void) {
    u8 *p = D_80158A8C;

    switch (*(u16 *)(p + 8)) {
    case 0:
        p[0x54] = 0x80;
        *(u16 *)(p + 8) = 0x40;
        break;
    case 0x40:
        func_80142AF0(D_80150978, 1);
        p[0x54] &= 1;
        if (p[0x54] != 0) {
            *(u8 **)(p + 0x218) = D_80155B54;
            p[0x343] = 1;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 4) = 0x40;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135B5C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135E9C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135FA8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80136244);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_801364EC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_801365C4);
