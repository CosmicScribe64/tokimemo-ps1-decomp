#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_801447E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_80144B7C);

s32 func_80144C54(void) {
    if (++D_8015EDD4 < 3) {
        return 1;
    }
    return 2;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_80144C84);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_80145180);

s32 func_801452C0(void) {
    s32 total;
    s32 roll;

    total = D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04;
    if (total < 0xB) {
        D_8015EBFC.unk_08 = 0xF;
        D_8015EBFC.unk_00 = 0x19;
        D_8015EBFC.unk_04 = 0xF;
        total = 0x37;
    }
    roll = func_8013E9A8(total);
    if (roll < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 0;
    }
    if (roll < D_8015EBFC.unk_04 + D_8015EBFC.unk_00) {
        D_8015EBFC.unk_04 -= 5;
        return 1;
    }
    D_8015EBFC.unk_08 -= 5;
    D_8015EBFC.unk_38 += 2;
    return 4;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_801453B4);
