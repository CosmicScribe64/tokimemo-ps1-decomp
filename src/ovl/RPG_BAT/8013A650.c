#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8013A650(void) {
    s32 temp_v0;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　サイバーファング　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EEF4();
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
    default:
        break;
    case 2:
        func_8013D418();
        break;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8013F0F4(0x505, 1, 0);
        func_8014EBA8();
        break;
    case 4:
        func_8014EDBC(0x34);
        break;
    case 5:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        break;
    case 6:
        func_8014EDBC(0x34);
        break;
    case 7:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        break;
    case 8:
        func_8014EDBC(0x34);
        break;
    case 9:
        func_8013E7C0(0x34, 4, 1, 1);
        func_8013E7C0(0x2F, 5, 1, 1);
        D_80121238 = 0x71000000;
        D_8012127C = 0x71000000;
        func_8013E7C0(0x2C, 6, 1, 1);
        func_8013F0F4(0x50D, 0, 4);
        func_8014EBA8();
        break;
    case 10:
        func_8014ED44(6);
        break;
    case 11:
        func_8013E7C0(0x2D, 7, 1, 1);
        func_8014EBA8();
        break;
    case 12:
        func_8014ED44(0x38);
        break;
    case 13:
        func_8013A98C();
        break;
    case 14:
        func_8014ED44(0x28);
        break;
    case 15:
        func_8013ACA4();
        break;
    case 16:
        func_8013E97C(0x2E, D_8015EC28, D_8015EC2C);
        func_8013E810(0x2E, 0xB, 5, 1);
        func_8013F0F4(0x508, 0, 3);
        func_8014EBA8();
        break;
    case 17:
        func_8013EA30(0xFF);
        temp_v0 = D_8015EDB0.unk_04 + 1;
        D_8015EDB0.unk_04 = temp_v0;
        if (temp_v0 >= 5) {
            func_8013EA30(0x80);
            func_8014EBA8();
        }
        break;
    case 18:
        func_8014EDBC(0x2E);
        break;
    case 19:
        func_8013D3E4();
        func_8013E810(0x2E, 0xFF, 1, 0);
        func_8014EBA8();
        break;
    case 20:
        func_8014ED44(0x14);
        break;
    case 21:
        func_8013D59C();
        break;
    case 22:
        func_8014EBEC();
        break;
    }
}

void func_8013A98C(void) {
    func_8013E7C0(0x2E, 8, 1, 1);
    D_801212CB = 0x1B;
    D_80121685 = 4;
    if (D_8015EDB0.unk_04 < 0x3C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, D_8015EDB0.unk_04 / 10);
    }
    if (D_8015EDB0.unk_04 >= 0x3C) {
        func_8013F0F4(0x506, 0, 1);
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB0.unk_04 - 0x3C) % 8 / 2 + 6);
    }
    if (D_8015EDB0.unk_04 >= 0x44) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB0.unk_04 - 0x44) % 8 / 2 + 6);
    }
    if (D_8015EDB0.unk_04 >= 0x4C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB0.unk_04 - 0x4C) % 8 / 2 + 6);
    }
    if (D_8015EDB0.unk_04 >= 0x54) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB0.unk_04 - 0x54) % 8 / 2 + 6);
    }
    if (D_8015EDB0.unk_04 >= 0x5C) {
        func_80043914(D_8015EEE8, 0x1B, 1, 4, (D_8015EDB0.unk_04 - 0x5C) % 12 / 2 + 0xA);
    }
    D_8015EDB0.unk_04 = D_8015EDB0.unk_04 + 1;
    if (D_8015EDB0.unk_04 >= 0x69) {
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
    func_8013E97C(0x2E, -D_8015EDB0.unk_04 * 8, 0);
    temp_v0 = D_8015EDB0.unk_04 + 1;
    D_8015EDB0.unk_04 = temp_v0;
    if (temp_v0 >= 0x27) {
        D_80121685 = 5;
        func_8013E7C0(0x2E, 0xA, 1, 0);
        func_8014EBA8();
    }
}
