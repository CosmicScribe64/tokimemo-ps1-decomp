#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_80144010);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_80144480);

s32 func_801445E8(s32 arg0) {
    if (arg0 == 0) {
        return 0x78;
    }
    if (D_8015EDC4 & 0x80000000) {
        return func_8013EA00(0x1E, 0x1E) + 0x1E;
    }
    return func_8013EA00(0x28, 0x1E) + 0x3C;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_80144640);

s32 func_801446F8(void) {
    switch (D_8015EB9C) {
    case 1:
        return 10;
    case 0:
        return 200;
    case 7:
        return 240;
    case 9:
        return 260;
    case 6:
        return 260;
    case 8:
        return 260;
    case 10:
        return 260;
    case 4:
        return 300;
    case 2:
        return 300;
    case 3:
        return 300;
    case 5:
        return 300;
    case 13:
        if (D_8015EDEC < 10) {
            return 110;
        }
        return 170;
    case 14:
        return 0x7FFFFFFF;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_801447B8);
