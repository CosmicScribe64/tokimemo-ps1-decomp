#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013AD50);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013AF1C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013B2F0);

void func_8013B534(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x30, 6, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (func_8013E90C(0x30) != 0) {
            func_8014EBA8();
        }
        break;
    }
}
