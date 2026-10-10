#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801439E0", func_801439E0);

void func_80143C28(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (D_8015EE08) {
    case 0:
        func_8013E7C0(0x1D, arg0, 1, 1);
        func_8014F258();
        return;
    case 1:
        func_8013E97C(0x1D, -D_8015EE10, 0);
        temp_v0 = D_8015EE10 + 2;
        D_8015EE10 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8013E7C0(0x1D, arg1, 1, 1);
            func_8013E97C(0x1D, -0x20, 0);
            func_8014F230();
        }
        return;
    }
}

void func_80143CE8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (D_8015EE08) {
    case 0:
        func_8013E7C0(0x1D, arg0, 1, 1);
        func_8014F258();
        return;
    case 1:
        func_8013E97C(0x1D, D_8015EE10 - 0x20, 0);
        temp_v0 = D_8015EE10 + 2;
        D_8015EE10 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8013E7C0(0x1D, arg1, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            func_8014F230();
        }
        return;
    }
}

void func_80143DA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    D_8015EC50 = arg3 - arg1;
    D_8015EC54 = arg4 - arg2;
    func_8013E97C(arg0, (D_8015EC50 * D_8015EE10) / 1000 + arg1, (D_8015EC54 * D_8015EE10) / 1000 + arg2);
    D_8015EE10 += arg5;
    if (D_8015EE10 >= 0x3E9) {
        func_8013E97C(arg0, arg3, arg4);
        func_8014F230();
    }
}

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
