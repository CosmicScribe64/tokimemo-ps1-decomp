#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801498C0", func_801498C0);

void func_80149AA8(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x28, 0xFF, 1, 0);
        func_8013E810(0x29, 0xE, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (D_8012114C + 3 == D_8015EC74) {
            func_8013EA30(0xFF);
            switch (D_8015EC34) {
            case 0:
            default:
                func_8013F0F4(0x505, 1, 0);
                break;
            case 1:
                func_8013F0F4(0x505, 1, 0);
                break;
            case 2:
                func_8013F0F4(0x506, 1, 0);
                break;
            case 3:
                func_8013F0F4(0x507, 1, 0);
                break;
            case 4:
                func_8013F0F4(0x508, 1, 0);
                break;
            case 5:
                func_8013F0F4(0x509, 1, 0);
                break;
            }
            func_8014EBD0();
        }
        break;
    case 2:
        D_8012114C = D_8015EC74 - 3;
        func_8014ED80(0xA);
        break;
    case 3:
        func_8013EA30(0x80);
        func_8013E810(0x29, 0xFF, 1, 0);
        func_8014EBA8();
        break;
    }
}

void func_80149C5C(void) {
    s32 sp2C;
    s32 sp28;
    s32 temp_a1;
    s32 var_a1;
    s32 var_at;
    s32 var_at_2;
    s32 var_v1;
    s32 var_v1_2;

    temp_a1 = D_8015EDB4;
    sp2C = ((s32) ((D_8015EC28 - 0xE0) * temp_a1) / 1000) + 0xE0;
    sp28 = ((s32) ((D_8015EC2C - 0x58) * temp_a1) / 1000) + 0x58;
    if (D_8015EBAC == 0) {
        var_v1 = (s32) (temp_a1 * 0x1800) / 1000;
        var_at = var_v1 < 0x401;
        if (var_v1 > 0) {
            func_8013F0F4(0x504, 0, 2);
            var_v1 = (s32) (D_8015EDB4 * 0x1800) / 1000;
            var_at = var_v1 < 0x401;
        }
        if (var_at == 0) {
            func_8013F0F4(0x504, 0, 3);
            var_v1 = (s32) (D_8015EDB4 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x801) {
            func_8013F0F4(0x504, 0, 4);
            var_v1 = (s32) (D_8015EDB4 * 0x1800) / 1000;
        }
        if (var_v1 >= 0xC01) {
            func_8013F0F4(0x504, 0, 5);
            var_v1 = (s32) (D_8015EDB4 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x1001) {
            func_8013F0F4(0x504, 0, 6);
            var_v1 = (s32) (D_8015EDB4 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x1401) {
            func_8013F0F4(0x504, 0, 7);
        }
    } else {
        var_v1_2 = (s32) (temp_a1 << 0xD) / 1000;
        var_at_2 = var_v1_2 < 0x401;
        if (var_v1_2 > 0) {
            func_8013F0F4(0x504, 0, 2);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
            var_at_2 = var_v1_2 < 0x401;
        }
        if (var_at_2 == 0) {
            func_8013F0F4(0x504, 0, 3);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x801) {
            func_8013F0F4(0x504, 0, 4);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0xC01) {
            func_8013F0F4(0x504, 0, 5);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1001) {
            func_8013F0F4(0x504, 0, 6);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1401) {
            func_8013F0F4(0x504, 0, 7);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1801) {
            func_8013F0F4(0x504, 0, 8);
            var_v1_2 = (s32) (D_8015EDB4 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1C01) {
            func_8013F0F4(0x504, 0, 9);
        }
    }
    if (D_8015EBAC == 0) {
        D_8015EDB4 = temp_a1;
        func_8013E97C(0x29, sp2C, (func_800A0070((s32) (temp_a1 * 0x1800) / 1000) >> 8) + sp28);
        func_8013E97C(0x28, sp2C, (func_800A0070((s32) (D_8015EDB4 * 0x1800) / 1000) >> 8) + sp28);
    } else {
        D_8015EDB4 = temp_a1;
        func_8013E97C(0x29, sp2C, (func_800A0070((s32) (temp_a1 << 0xD) / 1000) >> 8) + sp28);
        func_8013E97C(0x28, sp2C, (func_800A0070((s32) (D_8015EDB4 << 0xD) / 1000) >> 8) + sp28);
    }
    if (D_8015EBAC == 0) {
        var_a1 = D_8015EDB4 + 0xC;
    } else {
        var_a1 = D_8015EDB4 + 8;
    }
    D_8015EDB4 = var_a1;
    if (var_a1 >= 0x3E9) {
        func_8014EBA8();
    }
}

void func_8014A16C(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x2A, 0xFF, 1, 0);
        func_8013E7C0(0x2B, 0xFF, 1, 0);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8013E7C0(0x2D, 0xFF, 1, 0);
        func_8013E7C0(0x2E, 0xFF, 1, 0);
        func_8013E7C0(0x2F, 0xFF, 1, 0);
        func_8013E7C0(0x34, 5, 5, 1);
        func_8013E97C(0x29, 0xFB, 0x3D);
        func_8013E97C(0x28, 0xFB, 0x3D);
        func_8014EBD0();
        break;
    case 1:
        func_8014ED80(3);
        break;
    case 2:
        func_8013E97C(0x29, 0x102, 0x45);
        func_8013E97C(0x28, 0x102, 0x45);
        func_8014EBD0();
        break;
    case 3:
        func_8014ED80(3);
        break;
    case 4:
        func_8013E97C(0x29, 0x107, 0x4C);
        func_8013E97C(0x28, 0x107, 0x4C);
        func_8014EBD0();
        break;
    case 5:
        func_8014ED80(5);
        break;
    case 6:
        func_8013E97C(0x29, 0x109, 0x53);
        func_8013E97C(0x28, 0x109, 0x53);
        func_8014EBD0();
        break;
    case 7:
        func_8014ED80(5);
        break;
    case 8:
        func_8013E97C(0x29, 0x108, 0x58);
        func_8013E97C(0x28, 0x108, 0x58);
        func_8014EBD0();
        break;
    case 9:
        func_8014ED80(7);
        break;
    case 10:
        func_8013E97C(0x29, 0x108 - D_8015EDB8, 0x58);
        func_8013E97C(0x28, 0x108 - D_8015EDB8, 0x58);
        D_8015EDB8 += 2;
        if (D_8015EDB8 >= 0x28) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801498C0", func_8014A3DC);
