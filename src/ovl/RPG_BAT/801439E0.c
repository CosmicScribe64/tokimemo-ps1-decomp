#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_801439E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143C28);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143CE8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143DA8);

/* FAKE: D_80121313 reached as D_80120E07 + 0x50C (one base symbol); separate symbols let as1 hoist the second lbu. Real source unknown. T-4010 */
void func_80143ECC(void) {
    if (!(D_8015EE0C & 1)) {
        ((u8 *)&D_80120E07)[0x50C] |= 0x80;
        D_80120E07 |= 0x80;
    } else {
        ((u8 *)&D_80120E07)[0x50C] &= 0xFF7F;
        D_80120E07 &= 0xFF7F;
    }
    D_8015EE0C += 1;
    if (D_8015EE0C >= 0x3C) {
        func_8014F230();
    }
}

/* FAKE: D_80121313 reached as D_80120E07 + 0x50C (one base symbol); separate symbols let as1 hoist the second lbu. Real source unknown. T-4010 */
void func_80143F68(void) {
    if (!(D_8015EE0C & 1)) {
        ((u8 *)&D_80120E07)[0x50C] |= 0x80;
        D_80120E07 |= 0x80;
    } else {
        ((u8 *)&D_80120E07)[0x50C] &= 0xFF7F;
        D_80120E07 &= 0xFF7F;
    }
    D_8015EE0C += 1;
    if (D_8015EE0C >= 0x3D) {
        func_8014F230();
    }
}
