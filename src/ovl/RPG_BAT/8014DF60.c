#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014DF60", func_8014DF60);

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
