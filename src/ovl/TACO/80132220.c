#include "common.h"
#include "ovl/TACO.h"

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801322AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013240C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801328B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_80132FE8);

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_80133410);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801334BC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013356C);
