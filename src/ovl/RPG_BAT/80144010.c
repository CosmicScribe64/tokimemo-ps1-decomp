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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_801446F8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_801447B8);
