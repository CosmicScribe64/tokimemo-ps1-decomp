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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E639A;
extern s32 D_8015EBD8;
extern s32 D_8015EBE0;
extern s32 D_8015EC00;
extern s32 D_8015EC04;
extern s32 D_8015EC08;
/* end bulk_m2c declarations */
s32 func_80144C84(void) {
    s32 sp2C;
    s32 temp_a1;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a1;
    s32 var_a2;
    s32 var_v0;
    s32 var_v1;

    if (D_800E639A >= 0x97) {
        sp2C = 0x96;
    } else {
        sp2C = (s32) D_800E639A;
    }
    temp_v0 = func_8013E9A8(3);
    switch (temp_v0) {
    case 0:
        D_8015EBAC = 0;
        break;
    case 1:
        D_8015EBAC = 1;
        break;
    case 2:
        D_8015EBAC = 2;
        break;
    }
    if (func_801447B8() != 0) {
        var_a2 = D_8015EBFC.unk_00;
        var_v0 = D_8015EC08;
        var_v1 = D_8015EC00;
        var_a1 = D_8015EC04;
        if ((var_v0 + var_a2 + var_v1 + var_a1) < 0x15) {
            var_a2 = 0xF;
            var_v1 = 0xF;
            var_a1 = 5;
            var_v0 = 0x1E;
        }
    } else {
        var_a2 = D_8015EBFC.unk_00;
        var_v0 = D_8015EC08;
        var_v1 = D_8015EC00;
        var_a1 = D_8015EC04;
        if ((var_v0 + var_a2 + var_v1 + var_a1) < 0x15) {
            var_a2 = 0xA;
            var_v1 = 0xA;
            var_a1 = 0;
            var_v0 = 0x28;
        }
    }
    D_8015EC08 = var_v0;
    D_8015EC00 = var_v1;
    D_8015EC04 = var_a1;
    D_8015EBFC.unk_00 = var_a2;
    temp_v0_2 = func_8013E9A8(D_8015EBE0 + var_a2 + var_v1 + var_a1 + var_v0);
    if (temp_v0_2 < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 1;
    }
    temp_v1 = D_8015EC00;
    if (temp_v0_2 < (temp_v1 + D_8015EBFC.unk_00)) {
        D_8015EBFC.unk_00 = D_8015EBFC.unk_00;
        D_8015EC00 = temp_v1 - 5;
        return 0;
    }
    D_8015EC00 = temp_v1;
    temp_t0 = D_8015EBE0 + D_8015EBFC.unk_00 + temp_v1;
    D_8015EBFC.unk_00 = D_8015EBFC.unk_00;
    if (temp_v0_2 < (temp_t0 + D_8015EC04)) {
        if (D_8015EBCC[D_8015EBAC] == 1) {
            D_8015EBB0 = func_8013EA00(0x64, 0x64);
            return 4;
        }
        temp_a1 = D_8015EC04 - 5;
        D_8015EC04 = temp_a1;
        temp_a3 = D_8015EBE0 - 0x14;
        if (temp_a1 < 0) {
            D_8015EC04 = 0;
        }
        D_8015EBE0 = temp_a3;
        if (temp_a3 < 0) {
            D_8015EBE0 = 0;
        }
        return 3;
    }
    D_8015EC08 -= 5;
    if (func_801447B8() == 0) {
        D_8015EBD8 += 0x19;
    }
    if (D_8015EBCC[D_8015EBAC] == 1) {
        D_8015EBB0 = func_8013EA00(0x64, 0x64);
        return 4;
    }
    if ((D_8015EDF0 != 0x40000000) && (func_8013E9A8(0x64) < 5)) {
        temp_lo = (s32) D_8015EDEC / 10;
        D_8015EBB0 = temp_lo;
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = temp_lo / 10;
        }
        D_8015EC34 += 2;
        return 6;
    }
    if (D_8015ED98 != 0x100) {
        if (func_8013E9A8(0x64) < 5) {
            temp_lo_2 = (s32) (D_8015EBD8 * func_8013EA00(0xA, 0xA)) / 1000;
            D_8015EC34 += 1;
            D_8015EBB0 = temp_lo_2;
            return 5;
        }
        goto block_40;
    }
block_40:
    temp_lo_3 = (s32) (D_8015EBD8 * ((s32) (func_8013EA00(0x7D, 0x4B) * (0xE1 - sp2C)) / 150)) / 1000;
    D_8015EBB0 = temp_lo_3;
    if (D_8015ED98 & 0x100) {
        D_8015EBB0 = temp_lo_3 / 10;
    }
    D_8015EC34 += 1;
    return 2;
}

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
