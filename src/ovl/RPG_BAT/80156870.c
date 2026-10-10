#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_80156870(void) {
    if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80156870", func_801568A4);
