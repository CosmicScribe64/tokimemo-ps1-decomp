#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_801447E0);

s32 func_80144B7C(void) {
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return func_80144C54();
    case 0x100000:
        return func_80144C84();
    case 0x200000:
        return func_80145180();
    case 0x400000:
        return func_801452C0();
    case 0x8000000:
        D_8015EBFC.unk_38 += 2;
        return 1;
    case 0x80000000:
        return func_801453B4();
    }
}

s32 func_80144C54(void) {
    if (++D_8015EDD4 < 3) {
        return 1;
    }
    return 2;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801447E0", func_80144C84);

s32 func_80145180(void) {
    s32 total;
    s32 roll;

    if (D_8015EDC0 < D_8015EDCC / 3) {
        D_8015EBFC.unk_08 = 0xF;
        D_8015EBFC.unk_04 = 5;
        D_8015EBFC.unk_00 = 0;
        return 3;
    }
    total = D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04;
    if (total < 0xB) {
        D_8015EBFC.unk_00 = 0x14;
        D_8015EBFC.unk_08 = 0x14;
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
    return 2;
}

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
