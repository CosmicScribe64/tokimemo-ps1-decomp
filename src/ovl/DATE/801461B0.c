#include "common.h"
#include "ovl/DATE.h"

void func_801461B0(void) {
    D_8015CF80 = 0x801C6000;
    D_8015CF84 = 0x801AE000;
    D_8015CF88 = 0x801B2000;
    D_8015CF8C = 0x801B6000;
    D_8015CF90 = 0x801BA000;
    D_8015CF94 = 0x801BE000;
    D_8015CF98 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80146224);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_801462B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_8014718C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80147958);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80148924);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_801489D0);

void func_801491E4(s32 arg0) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = func_8005742C(arg0, 1);
    if (temp_v0 == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (temp_v0 == (1 - D_800B5939)) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80149268);
