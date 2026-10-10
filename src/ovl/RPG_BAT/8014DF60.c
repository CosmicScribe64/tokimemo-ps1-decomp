#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014DF60);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014E1A4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014E28C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014E300);

void func_8014E5B4(s32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014E64C);
