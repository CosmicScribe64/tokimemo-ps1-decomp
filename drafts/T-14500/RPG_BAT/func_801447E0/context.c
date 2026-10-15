#include "common.h"
#include "ovl/RPG_BAT.h"

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E639A;
extern s32 D_8015EBD8;
/* end bulk_m2c declarations */
void func_801447E0(void) {
    s32 sp38;
    s32 sp2C;
    s16 var_a2;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t6;
    s32 temp_t7;
    s32 var_v1;
    s32 var_v1_2;

    if (D_800E639A >= 0x97) {
        var_a2 = 0x96;
    } else {
        var_a2 = D_800E639A;
    }
    if (D_8015EB98 & 0x80000000) {
        temp_t7 = D_8015EDC4 & 0xFFFF;
        if ((temp_t7 == 1) || (temp_t7 != 2)) {
            sp38 = (s32) var_a2;
            var_v1 = func_8013EA00((s32) D_8015EDA0 / 10, (s32) (D_8015EDA0 * 5) / 100) + 0x64;
        } else {
            temp_lo = (s32) D_8015EDA0 / 10;
            sp38 = (s32) var_a2;
            var_v1 = func_8013EA00(temp_lo, temp_lo) + 0x96;
        }
        temp_lo_2 = (s32) (D_8015EBD8 * ((s32) (var_v1 * (0xE1 - var_a2)) / 150)) / 1000;
        D_8015EBB0 = temp_lo_2;
        if (D_8015ED98 & 0x100) {
            D_8015EBD8 += 0x64;
            D_8015EBB0 = temp_lo_2 / 2;
        }
    }
    temp_a1 = D_8015EB98;
    temp_a0 = D_8015ED98;
    if (temp_a1 & 0x100000) {
        temp_t6 = D_8015EDC4 & 0xFFFF;
        sp2C = 0xE1 - var_a2;
        if ((temp_t6 == 2) || (temp_t6 != 4)) {
            var_v1_2 = func_8013E9A8(((s32) (D_8015EDA0 * 0x19) / 100) - 0x1F4) + 0x1F4;
            if (D_8015ED98 & 0x100) {
                var_v1_2 = var_v1_2 / 3;
            }
        } else {
            var_v1_2 = func_8013E9A8(((s32) (D_8015EDA0 * 0x32) / 100) - 0x3E8) + 0x3E8;
            if (D_8015ED98 & 0x100) {
                var_v1_2 = var_v1_2 / 10;
            }
        }
        D_8015EBB0 = (s32) (D_8015EBD8 * ((s32) (var_v1_2 * sp2C) / 150)) / 1000;
    }
    if (temp_a1 & 0xF0000) {
        if (temp_a0 & 0x100) {
            D_8015EBB0 = (s32) D_8015ED94 / 100;
        } else {
            D_8015EBB0 = (s32) D_8015ED94 / 10;
        }
    }
    if (temp_a1 & 0x08000000) {
        if (temp_a0 & 0x1000) {
            D_8015EBB0 = (s32) D_8015ED94 / 2;
            return;
        }
        if (temp_a0 & 0x100) {
            D_8015EBB0 = (s32) D_8015ED94 / 50;
        } else {
            D_8015EBB0 = (s32) D_8015ED94 / 5;
        }
        if (D_8015ED94 < 0x14) {
            D_8015EBB0 = 0;
        }
    }
}

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

/* FAKE: the word view at +0x0C fills an unrecovered RpgStat field; the
 * comma return preserves the original decrement scheduling. T-10010. */
s32 func_801453B4(void) {
    s32 roll;
    s32 total;

    total = (*(s32 *)&D_8015EBFC.pad_0C[0]) + D_8015EBFC.unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08;
    if (total < 6) {
        (*(s32 *)&D_8015EBFC.pad_0C[0]) = 0xA;
        D_8015EBFC.unk_04 = 5;
        D_8015EBFC.unk_08 = 0xA;
        D_8015EBFC.unk_00 = 0xF;
        total = 0x28;
    }
    roll = func_8013E9A8(total);
    if (roll < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 0;
    }
    if (roll < (D_8015EBFC.unk_04 + D_8015EBFC.unk_00)) {
        return (D_8015EBFC.unk_04 -= 5, 4);
    }
    if (roll < (D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04)) {
        D_8015EBFC.unk_08 -= 5;
        return 1;
    }
    (*(s32 *)&D_8015EBFC.pad_0C[0]) -= 5;
    return 2;
}
