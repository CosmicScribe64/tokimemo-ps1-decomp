#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013A650", func_8013A650);

void func_8013A98C(void) {
    func_8013E7C0(0x2E, 8, 1, 1);
    D_801212CB = 0x1B;
    D_80121685 = 4;
    if (D_8015EDB4 < 0x3C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, D_8015EDB4 / 10);
    }
    if (D_8015EDB4 >= 0x3C) {
        func_8013F0F4(0x506, 0, 1);
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB4 - 0x3C) % 8 / 2 + 6);
    }
    if (D_8015EDB4 >= 0x44) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB4 - 0x44) % 8 / 2 + 6);
    }
    if (D_8015EDB4 >= 0x4C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB4 - 0x4C) % 8 / 2 + 6);
    }
    if (D_8015EDB4 >= 0x54) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB4 - 0x54) % 8 / 2 + 6);
    }
    if (D_8015EDB4 >= 0x5C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB4 - 0x5C) % 12 / 2 + 0xA);
    }
    D_8015EDB4 = D_8015EDB4 + 1;
    if (D_8015EDB4 >= 0x69) {
        D_801212CB = 0x1D;
        func_8013E7C0(0x2E, 9, 1, 1);
        func_8013E7C0(0x2F, 0xFF, 1, 0);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8013E7C0(0x2D, 0xFF, 1, 0);
        D_80121238 = 0x41000000;
        D_8012127C = 0x41000000;
        func_8014EBA8();
    }
}

void func_8013ACA4(void) {
    s32 temp_v0;

    func_8013F0F4(0x507, 0, 2);
    func_8013E7C0(0x2E, 0xA, 1, 1);
    func_8013E97C(0x2E, -D_8015EDB4 * 8, 0);
    temp_v0 = D_8015EDB4 + 1;
    D_8015EDB4 = temp_v0;
    if (temp_v0 >= 0x27) {
        D_80121685 = 5;
        func_8013E7C0(0x2E, 0xA, 1, 0);
        func_8014EBA8();
    }
}
