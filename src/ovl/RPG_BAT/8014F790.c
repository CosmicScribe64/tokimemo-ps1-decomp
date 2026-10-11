#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8014F790(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "紐緒「想定強度に問題があったみたいね");
        func_8013E7C0(0x1D, 0x19, 1, 1);
        func_8014B738(D_8015EB70, 1, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x38, 0x09000041, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8013F220();
        func_8014F230();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014F790", func_8014F88C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014F790", func_8014FDBC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014F790", func_80150120);
