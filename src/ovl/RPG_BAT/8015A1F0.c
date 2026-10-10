#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A1F0);

void func_8015A6D4(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E810(i + 0x28, 7, 1, 1);
        func_8013E97C(i + 0x28, -0x19, 0x109);
    }
    func_8014EBA8();
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A744);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A8B4);
