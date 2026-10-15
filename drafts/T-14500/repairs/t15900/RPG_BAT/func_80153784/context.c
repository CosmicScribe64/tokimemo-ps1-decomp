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

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E738A;
void func_80153B08(void);
void func_80153D20(void);
void func_80153A60(void);
/* end bulk_m2c declarations */
void func_80153784(void) {
    s16 sp52;
    s16 sp50;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    s16 sp46;
    s16 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    s32 *var_v0_2;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v0;

    switch (D_800E738A) {
    case 0:
        sp4E = 0x100;
        sp4C = 0x240;
        sp52 = 0x28;
        sp50 = 0x40;
        func_8009C93C((RECT *) &sp4C, 0x180, 0x180);
        sp46 = 0x128;
        sp44 = 0x240;
        sp4A = 0x28;
        sp48 = 0x40;
        func_8009C93C((RECT *) &sp44, 0x1C0, 0x180);
        sp3E = 0x100;
        sp3C = 0x220;
        sp42 = 0x28;
        sp40 = 0x40;
        func_8009C93C((RECT *) &sp3C, 0x200, 0x180);
        sp36 = 0x100;
        sp34 = 0x21F;
        sp3A = 0x80;
        sp38 = 1;
        func_8009C93C((RECT *) &sp34, 0x220, 0x100);
        D_8015EC40 = 0x20;
        D_8015EC44 = 0x40;
        D_8015EC48 = 1;
        D_8015ED2C = -0x80;
        D_8015ED30 = 0x20;
        D_8015ED34 = 0x40;
        D_8015ED38 = 1;
        D_8015EC38 = 0;
        D_8015EC3C = 0x80;
        D_8015ED28 = 0;
        func_8004284C(); return;
    case 1:
        temp_v0 = D_8015EC38 + D_8015EC48;
        D_8015EC38 = temp_v0;
        temp_a0 = D_8015ED28 + D_8015ED38;
        if (temp_v0 >= D_8015EC40) {
            D_8015EC38 = 0;
        }
        D_8015ED28 = temp_a0;
        if (temp_a0 >= D_8015ED30) {
            D_8015ED28 = 0;
        }
        func_80153B08();
        func_80153D20();
        func_80153A60();
        D_8015ED2C += 1;
        temp_v1 = D_8015EC3C - 1;
        D_8015EC3C = temp_v1;
        if (temp_v1 == 0) {
            func_8004284C(); return;
        }
        return;
    case 2:
        sp3E = 0x180;
        sp3C = 0x200;
        sp42 = 0x28;
        sp40 = 1;
        func_8009C93C((RECT *) &sp3C, 0x220, 0x100);
        func_8013E7C0(0x1F, 0, 1, 1);
        func_8004284C(); return;
    case 3:
        D_8015EC74 = 0;
        var_v0_2 = &D_8015EC7C;
        D_8015EC78 = 0;
        do {
            var_v0_2 += 0x10;
            *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
        } while (var_v0_2 != D_8015ED3C);
        var_v0 = func_80042808();
        /* Duplicate return node #16. Try simplifying control flow for better match */
        return;
    default:
        return;
    }
}

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
