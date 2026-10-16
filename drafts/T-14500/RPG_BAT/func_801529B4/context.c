#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_801525B0);

void func_801528DC(void) {
    s32 i;
    u8 *p;

    p = (u8 *)&D_800E6280.unk_110C;
    if (D_800E6280.unk_1100 == 0) {
        D_800E6280.unk_1100 += 1;
        *p = 0x80;
    }
    i = 0;
    do {
        D_8011ECD0[i * 0x44 + 0x1107] = *p;
        D_800E6280.unk_03A = *p;
        if (D_8015EB98 == 0x20000 && *p >= 0x61) {
            D_80120E93 = 0x60;
        }
        i++;
    } while (D_80120650 != &D_8011ECD0[i * 0x44]);
    *p -= 2;
    if (*p == 0) {
        func_8004E58C();
        D_800E6280.unk_03A = 0x80;
        func_80048EB8(0);
        func_8006BC28(0);
        func_800438F0(1);
        func_80042808();
    }
}

/* bulk_m2c declarations (T-11190) */
extern u32 D_800E7384;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_801529B4(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;

    func_8014B79C();
    switch (D_800E738A) {
    case 0:
        func_8004500C(1, 0x200);
        temp_v0 = D_8015EB98;
        switch (temp_v0) {
        case 0x10000:
            func_800AE0F0(D_8015E9E0, "ハブ・ロードが襲ってきた！");
            break;
        case 0x20000:
            func_800AE0F0(D_8015E9E0, "鹿・グレートが襲ってきた！");
            break;
        case 0x40000:
            func_800AE0F0(D_8015E9E0, "キラーヒグマが襲ってきた！");
            break;
        case 0x100000:
            func_800AE0F0(D_8015E9E0, "不良達が襲ってきた！");
            break;
        }
        if (temp_v0 != 0x08000000) {
            func_8014B738((s32) D_8015EB70, 1, 0x5A);
        }
        func_8013E810(0x30, 1, 1, 1);
        if ((D_8015EB98 != 0x10000) && (D_8015EB98 != 0x20000) && (D_8015EB98 != 0x40000)) {
            if (D_8015EB98 != 0x100000) {
                if (D_8015EB98 == 0x08000000) {
                    goto block_20;
                }
            } else {
                func_8013E810(0x3B, 0, 1, 1);
                func_8013E810(0x3C, 0, 1, 1);
                func_8013E810(0x3D, 0, 1, 1);
            }
        } else {
block_20:
            func_8013E810(0x3B, 0, 1, 1);
        }
        switch (D_8015EB9C) {
        case 0:
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 1:
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 2:
            func_8013E810(0x1D, 9, 1, 1);
            break;
        case 3:
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 4:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 5:
            func_8013E810(0x1D, 0, 1, 1);
            break;
        case 6:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 7:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 8:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 9:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 10:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 13:
            func_8013E810(0x1D, 1, 1, 1);
            break;
        }
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        func_8013E97C(0x3B, -0x58, 0x60);
        temp_v0_2 = D_8015EB98;
        if (temp_v0_2 == 0x100000) {
            func_8013E97C(0x3B, -0x28, 0x68);
            func_8013E97C(0x3C, -0x60, 0x58);
            func_8013E97C(0x3D, -0x68, 0x88);
        }
        if (temp_v0_2 == 0x08000000) {
            func_8013E97C(0x3B, -0x58, 0x70);
        }
        func_8004284C();
        break;
    case 1:
        func_8013E97C(0x30, 0x160 - D_800E7384, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E7384, 0x88);
        func_8013E97C(0x3B, (D_800E7384 * 2) - 0x58, 0x60);
        temp_v0_3 = D_8015EB98;
        if (temp_v0_3 == 0x100000) {
            func_8013E97C(0x3B, (D_800E7384 * 2) - 0x28, 0x68);
            func_8013E97C(0x3C, (D_800E7384 * 2) - 0x60, 0x58);
            func_8013E97C(0x3D, (D_800E7384 * 2) - 0x68, 0x88);
        }
        if (temp_v0_3 == 0x08000000) {
            func_8013E97C(0x3B, (D_800E7384 * 2) - 0x58, 0x70);
        }
        temp_v0_4 = D_800E7384 + 4;
        D_800E7384 = temp_v0_4;
        if (temp_v0_4 >= 0x51U) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8004284C();
        }
        break;
    case 2:
        temp_v0_5 = D_800E7384 + 1;
        D_800E7384 = temp_v0_5;
        if (temp_v0_5 >= 0x3DU) {
            func_80042808();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80152F30);

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801535E4(void) {
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x74);
        if (D_8015EE44.unk_00 >= 3 && D_8015EE44.unk_08 >= 3) {
            D_8015EE44.unk_08 = D_8015E744 + 1;
        }
    }
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "紐緒「私が準備するまで、");
        func_800AE0F0(D_8015EA08, "持ちこたえなさい。");
        func_8013E7C0(0x1D, 0x19, 1, 1);
        func_8014B738(D_8015EB70, 2, 0x96);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x2E, 0x07000033, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x34, 0x08000037, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8013F220();
        func_8004284C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80153784);

void func_80153A60(void) {
    int dead; /* FAKE: dead copy of *p; only reserves $v0 so the table pointer lands in $v1. Found with decomp-permuter. T-10910 */
    s32 i;
    s32 *p;

    i = 0;
    p = &D_8015EC74;
    do {
        D_8011ECD0[i * 0x44 + 0x1107] = *p;
        *(s8 *)&D_800E6280.unk_3A = *p;
        if (D_8015EB98 == 0x20000 && (dead = *p) >= 0x61) {
            D_80120E93 = 0x60;
        }
        if (0) { /* FAKE: empty dead branch steers the allocation. T-10910 */
        }
        i++;
    } while (D_80120650 != &D_8011ECD0[i * 0x44]);
    func_8013E7C0(0x1F, 0, 1, 0);
    D_8015EC74 += 2;
    if (*p > 0x80) {
        D_8015EC74 = 0x80;
    }
}

void func_80153B08(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD, var_s0, temp_s1, 0, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

void func_80153D20(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD, var_s0, temp_s1, 2, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80153F3C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80154220);
