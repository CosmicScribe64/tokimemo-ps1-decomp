#include "common.h"
#include "ovl/RPG_BAT.h"

u8 func_8013D780(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8004284C();
        break;
    case 1:
        func_80044750(0xE4);
        func_8004284C();
        break;
    case 2:
        if (func_800460EC() & 4) {
            func_80042808();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013D780", func_8013D834);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013D780", func_8013DC60);
