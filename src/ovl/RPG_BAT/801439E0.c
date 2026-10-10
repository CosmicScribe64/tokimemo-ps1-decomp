#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_801439E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143C28);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143CE8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_80143DA8);

void func_80143ECC(void) {
    if (!(D_8015EE0C & 1)) {
        D_8011ECD0[0x2643] |= 0x80;
        D_8011ECD0[0x2137] |= 0x80;
    } else {
        D_8011ECD0[0x2643] &= 0xFF7F;
        D_8011ECD0[0x2137] &= 0xFF7F;
    }
    D_8015EE0C += 1;
    if (D_8015EE0C >= 0x3C) {
        func_8014F230();
    }
}

void func_80143F68(void) {
    if (!(D_8015EE0C & 1)) {
        D_8011ECD0[0x2643] |= 0x80;
        D_8011ECD0[0x2137] |= 0x80;
    } else {
        D_8011ECD0[0x2643] &= 0xFF7F;
        D_8011ECD0[0x2137] &= 0xFF7F;
    }
    D_8015EE0C += 1;
    if (D_8015EE0C >= 0x3D) {
        func_8014F230();
    }
}
