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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_801529B4);

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E71DF;
extern u32 D_800E7384;
extern u8 D_800E738A;
s32 func_801535E4(void);
/* end bulk_m2c declarations */
void func_80152F30(void) {
    s32 sp3C;
    s32 temp_a2;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v1;
    u32 temp_v1_2;
    u32 temp_v1_3;
    u32 temp_v1_4;
    u32 temp_v1_5;

    temp_v0 = func_8013E9A8(8);
    switch (temp_v0) {
    case 0:
        sp3C = 0x9F9F9F;
        break;
    case 1:
        sp3C = 0x6F6F6F;
        break;
    case 2:
        sp3C = 0xBF0000;
        break;
    case 3:
        sp3C = 0xBF00;
        break;
    case 4:
        sp3C = 0xBF;
        break;
    case 5:
        sp3C = 0xAFAF00;
        break;
    case 6:
        sp3C = 0xAF00AF;
        break;
    case 7:
        sp3C = 0xAFAF;
        break;
    }
    func_8014B79C();
    switch (D_800E738A) {
    case 0:
        D_800E71DF = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8004284C();
        break;
    case 1:
        func_80044750(0xE4);
        func_8004284C();
        break;
    case 2:
        if (func_800460EC() & 4) {
            func_8004284C();
        }
        break;
    case 3:
        func_8013E810(0x30, 1, 1, 1);
        func_8013E810(0x1D, 0x17, 1, 1);
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        D_8015EC48 = 0;
        D_8015EC44 = 0;
        D_8015EC40 = 0;
        D_8015EC38 = 0;
        D_8015EC3C = -0x1E;
        D_8015EC4C = 0;
        func_8013E97C(0x2F, 0, -0x1E);
        func_8013E810(0x2F, 2, 1, 1);
        func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
        func_8013E810(0x2E, 6, 1, 1);
        func_8004284C();
        break;
    case 4:
        func_8013E97C(0x30, 0x160 - D_800E7384, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E7384, 0x88);
        temp_v1 = D_800E7384 + 4;
        D_800E7384 = temp_v1;
        if (temp_v1 >= 0x51U) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0x18, 1, 1);
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 9, 1, 1);
            D_80121531 = 2;
            func_8004284C();
        }
        break;
    case 5:
        temp_v0_2 = (u32) D_800E7384 >> 1;
        func_80049A40(-0x5C - temp_v0_2, -0x78, temp_v0_2 * 2, 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        temp_v1_2 = D_800E7384 + 1;
        D_800E7384 = temp_v1_2;
        if (temp_v1_2 >= 0x41U) {
            func_8004284C();
        }
        break;
    case 6:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        func_8013E97C(0x3B, 0x48, D_800E7384 - 0x80);
        func_8013E810(0x3B, 8, 1, 1);
        temp_v1_3 = D_800E7384 + 1;
        D_800E7384 = temp_v1_3;
        if (temp_v1_3 >= 0xE9U) {
            k_disp_switch(D_8015EDE8, 1);
            func_8004284C();
        }
        break;
    case 7:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, sp3C, 0x81);
        D_800E7384 += 1;
        dtd_on_tpage(0, 0, 3, 1, 0);
        if ((u32) D_800E7384 >= 0x79U) {
            func_8004284C();
        }
        break;
    case 8:
        func_80049A40(D_800E7384 - 0x7C, -0x78, 0x40 - (D_800E7384 * 2), 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        temp_v1_4 = D_800E7384 + 1;
        D_800E7384 = temp_v1_4;
        if (temp_v1_4 >= 0x21U) {
            func_8004284C();
        }
        break;
    case 9:
        temp_a2 = D_800E7384 - 0x40;
        temp_v1_5 = D_800E7384 + 1;
        D_800E7384 = temp_v1_5;
        D_8015EC38 = 0x40;
        D_8015EC3C = temp_a2;
        if (temp_v1_5 >= 0x59U) {
            func_8004284C();
        }
        break;
    case 10:
        func_800AE0F0(D_8015E9E0, "宇宙人が襲ってきた！");
        func_8014B738((s32) D_8015EB70, 1, 0x5A);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E97C(0x3B, 0, 0);
        func_8013E7C0(0x3B, 0, 1, 1);
        func_8004284C();
        break;
    case 11:
        if (D_8015EE48 == 0) {
            func_8004284C();
        }
        break;
    case 12:
        func_8013E97C(0x1E, 0, 0);
        func_8013E7C0(0x1E, 0, 5, 1);
        func_8004284C();
        break;
    case 13:
        func_801535E4();
        break;
    case 14:
        D_8015EDF0 = 0x10001000;
        func_80042808();
        break;
    }
    if ((u8) D_800E738A >= 9U) {
        func_8015785C();
    }
    if (((u8) D_800E738A >= 5U) && ((u8) D_800E738A < 8U)) {
        func_8013F15C(0x505, 0x6E, 7);
    }
}

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
