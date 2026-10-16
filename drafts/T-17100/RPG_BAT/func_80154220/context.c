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

void func_80154220(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v0;

    temp_v0 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    SetShadeTex(temp_v0, 0);
    if (arg3 != 0) {
        SetSemiTrans(temp_v0, 1);
    } else {
        SetSemiTrans(temp_v0, 0);
    }
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) arg2;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) arg2;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg2;
    if (arg3 != 0) {
        var_v0 = func_8009ED74(0, 0x1F7);
    } else {
        var_v0 = func_8009ED74(0, 0x1F5);
    }
    *(s16 *)((unsigned char *)temp_v0 + (14)) = (s16) var_v0;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(1, arg3 + 4, (arg4 << 6) + 0x140, 0x100);
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (*(s32 *)arg0);
    temp_v1 = (D_8011ECA0 * -0xF0) + 0xF0;
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (*(s32 *)((unsigned char *)arg0 + (4)) + temp_v1);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) (*(s32 *)((unsigned char *)arg0 + (8)) + 1);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (*(s32 *)((unsigned char *)arg0 + (12)) + temp_v1);
    *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) *(s32 *)((unsigned char *)arg0 + (16));
    *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) (*(s32 *)((unsigned char *)arg0 + (20)) + temp_v1 + 1);
    *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (*(s32 *)((unsigned char *)arg0 + (24)) + 1);
    *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (*(s32 *)((unsigned char *)arg0 + (28)) + temp_v1 + 1);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (*(s32 *)arg1);
    *(s8 *)((unsigned char *)temp_v0 + (13)) = (s8) *(s32 *)((unsigned char *)arg1 + (4));
    *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (*(s32 *)((unsigned char *)arg1 + (8)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (21)) = (s8) *(s32 *)((unsigned char *)arg1 + (12));
    *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) *(s32 *)((unsigned char *)arg1 + (16));
    *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (*(s32 *)((unsigned char *)arg1 + (20)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (*(s32 *)((unsigned char *)arg1 + (24)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (37)) = (s8) (*(s32 *)((unsigned char *)arg1 + (28)) + 1);
    AddPrim((D_8011ECA0 << 0xA) + 0x20 + D_800E8CA0, temp_v0);
}
