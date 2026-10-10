#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013B5B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013B910);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013BBF8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013BEA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013C1CC);

void func_8013C2A0(void) {
    if (D_8015EE1C == 1) {
        switch (D_8015EB98) {
        case 0x100000:
            if (D_8015EDC4 & 0x100000) {
                func_80139510();
                return;
            }
            func_801596E8();
            return;
        case 0x8000000:
            func_8013F82C();
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013C32C);
