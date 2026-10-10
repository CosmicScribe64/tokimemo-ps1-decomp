#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013CC90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D030);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D0D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D1D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D290);

void func_8013D350(void) {
    if (D_8015EDC4 & 0x100000) {
        func_8013EA60(0, 0, D_8015EBB0, 1);
        func_8013EA60(1, 1, D_8015EBB4, 1);
        func_8013EA60(2, 2, D_8015EBB8, 1);
        return;
    }
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

void func_8013D3E4(void) {
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D418);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D59C);
