#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_801525B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_801528DC);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80153A60);

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
