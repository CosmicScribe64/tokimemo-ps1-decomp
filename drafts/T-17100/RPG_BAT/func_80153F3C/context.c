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

void func_80153F3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 temp_v0;
    s32 temp_v1;

    sp6C = arg2;
    sp74 = arg2;
    sp4C = arg2;
    sp54 = arg2;
    sp7C = arg3;
    sp84 = arg3;
    sp5C = arg3;
    sp64 = arg3;
    sp68 = arg0;
    sp78 = arg1;
    sp70 = arg0 + 0x7F;
    sp80 = arg1 + 0x7F;
    sp50 = 0x7F;
    sp48 = 0;
    sp60 = 0x7F;
    sp58 = 0;
    func_80154220((s32) &sp68, (s32) &sp48, (s32) ((u32) (((DisplayActor44 *)D_8011ECD0)[127].unk_07 + arg4) >> 1), arg5, 1);
    sp70 = arg0 + 0xFF;
    sp68 = arg0 + 0x80;
    sp6C = arg2;
    sp74 = arg2;
    sp80 = arg1 + 0xFF;
    sp78 = arg1 + 0x80;
    sp7C = arg3;
    sp84 = arg3;
    sp50 = 0x7F;
    sp48 = 0;
    sp4C = arg2;
    sp54 = arg2;
    sp60 = 0x7F;
    sp58 = 0;
    sp5C = arg3;
    sp64 = arg3;
    func_80154220((s32) &sp68, (s32) &sp48, (s32) ((u32) (((DisplayActor44 *)D_8011ECD0)[127].unk_07 + arg4) >> 1), arg5, 2);
    temp_v0 = arg0 + 0x13F;
    temp_v1 = arg1 + 0x13F;
    sp70 = temp_v0;
    sp68 = arg0 + 0x100;
    sp6C = arg2;
    sp74 = arg2;
    sp80 = temp_v1;
    sp78 = arg1 + 0x100;
    sp7C = arg3;
    sp84 = arg3;
    sp50 = 0x3F;
    sp48 = 0;
    sp4C = arg2;
    sp54 = arg2;
    sp60 = 0x3F;
    sp58 = 0;
    sp5C = arg3;
    sp64 = arg3;
    sp40 = temp_v1;
    sp44 = temp_v0;
    func_80154220((s32) &sp68, (s32) &sp48, (s32) ((u32) (((DisplayActor44 *)D_8011ECD0)[127].unk_07 + arg4) >> 1), arg5, 3);
    sp70 = arg0 - 0x7F;
    sp68 = arg0;
    sp6C = arg2;
    sp74 = arg2;
    sp80 = arg1 - 0x7F;
    sp78 = arg1;
    sp7C = arg3;
    sp84 = arg3;
    sp50 = 0x7F;
    sp48 = 0;
    sp4C = arg2;
    sp54 = arg2;
    sp60 = 0x7F;
    sp58 = 0;
    sp5C = arg3;
    sp64 = arg3;
    func_80154220((s32) &sp68, (s32) &sp48, (s32) ((u32) (((DisplayActor44 *)D_8011ECD0)[127].unk_07 + arg4) >> 1), arg5, 1);
    sp68 = arg0 + 0x17F;
    sp6C = arg2;
    sp74 = arg2;
    sp78 = arg1 + 0x17F;
    sp7C = arg3;
    sp84 = arg3;
    sp50 = 0x3F;
    sp48 = 0;
    sp4C = arg2;
    sp54 = arg2;
    sp60 = 0x3F;
    sp58 = 0;
    sp5C = arg3;
    sp64 = arg3;
    sp70 = sp44;
    sp80 = sp40;
    func_80154220((s32) &sp68, (s32) &sp48, (s32) ((u32) (((DisplayActor44 *)D_8011ECD0)[127].unk_07 + arg4) >> 1), arg5, 3);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80154220);
