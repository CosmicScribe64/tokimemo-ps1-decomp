#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_801321D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132348);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_801328C4);

void func_80132A94(s32 arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E97C(i + 0x28, arg0, arg1);
        func_8013E810(i + 0x28, 9, 1, 1);
        D_8011ECD0[0x2421 + i * 0x44] = 4;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132B34);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132CE4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132DE8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132EE0);
