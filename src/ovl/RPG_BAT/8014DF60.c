#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8014DF60(void) {
    switch (D_8015EE04) {
    case 0:
        func_8014E64C();
        func_8014F230();
        return;
    case 1:
        func_80143C28(1, 0);
        return;
    case 2:
        D_80120E49 = 4;
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8014F230();
        return;
    case 3:
        if (D_80120E1C == 5) {
            func_8013F15C(0x50E, 0, 4);
        }
        func_8014F3C8(0x1D);
        return;
    case 4:
        func_8013E97C(0x1E, 0xE0, 0x80);
        func_8013E810(0x1E, 7, 5, 1);
        func_8014F230();
        return;
    case 5:
        func_8014F3C8(0x1E);
        return;
    case 6:
        func_8013E810(0x1E, 8, 1, 1);
        func_8014F230();
        return;
    case 7:
        func_8014E300();
        return;
    case 8:
        func_8013E810(0x1E, 9, 5, 1);
        func_8013F15C(0x510, 1, 2);
        func_8014F230();
        return;
    case 9:
        func_8014F3C8(0x1E);
        return;
    case 10:
        func_8013E810(0x1E, 0xFF, 1, 0);
        if (D_8015EDF0 == 1) {
            func_8014E1A4();
            return;
        }
        func_8014E28C();
        return;
    case 11:
        if (func_800460EC() == 2) {
            func_8013E7C0(0x1D, 0, 1, 1);
        }
        func_8014F448(1);
        return;
    case 12:
        func_8014F350(0x14);
        return;
    case 13:
        func_80143CE8(3, 0);
        return;
    case 14:
        func_8014F350(0x14);
        return;
    case 15:
        func_8014F278();
        /* fallthrough */
    default:
        return;
    }
}

void func_8014E1A4(void) {
    func_800AE0F0(D_8015E904, "鏡「また私のとりこが一人。");
    func_8014B738(D_8015E9CC, 1, 0x8C);
    func_8013F1C4(0x0300001D, 0x04000025, 0);
    switch (D_8015EBAC) {
    case 0:
        D_8015EBCC[0] = 1;
        break;
    case 1:
        D_8015EBD0 = 1;
        break;
    case 2:
        D_8015EBD4 = 1;
        break;
    }
    D_8011ECD0[D_8015EBAC * 0x44 + 0x292F] |= 1;
    func_8013E7C0(0x1D, 6, 1, 1);
    func_8014F230();
}

void func_8014E28C(void) {
    func_800AE0F0(D_8015E904, "鏡「でなおしてらっしゃい！");
    func_8014B738(D_8015E9CC, 1, 0x64);
    func_8013F1C4(0x1F000069, 0x1300006D, 0);
    func_8013E7C0(0x1D, 6, 1, 1);
    func_8014F230();
}

/* NON_MATCHING: T-10310, near miss */
#ifdef NON_MATCHING
void func_8014E300(void) {
    func_8014E5B4(D_8015EBAC);
    D_8015EC50 = D_8015EC28 - 0xE0;
    D_8015EC54 = D_8015EC2C - 0x8E;
    func_8013F15C(0x50F, 0, 1);
    if (D_8015EE0C >= 0x140) {
        func_8013F15C(0x514, 0x10, 3);
    }
    if (D_8015EBAC == 0) {
        D_8015EE0C += 7;
        func_8013E97C(0x1E, (D_8015EC50 * D_8015EE0C) / 1000 + 0xE0, (func_800A0070((D_8015EE0C << 0xD) / 1000) >> 8) + (D_8015EC54 * D_8015EE0C) / 1000 + 0x80);
    } else {
        D_8015EE0C += 5;
        func_8013E97C(0x1E, (D_8015EC50 * D_8015EE0C) / 1000 + 0xE0, (func_800A0070((D_8015EE0C * 0x3000) / 1000) >> 8) + (D_8015EC54 * D_8015EE0C) / 1000 + 0x80);
    }
    if (D_8015EE0C >= 0x3E9) {
        func_8013E97C(0x1E, D_8015EC28, D_8015EC2C - 0xE);
        func_8014F230();
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014E300);
#endif

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
