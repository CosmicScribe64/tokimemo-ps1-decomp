#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80156CC0(void) {
    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "宇宙人「Ж○ξ♂×♀？〆£∞Й！");
        func_8014B738(D_8015E8F0, 1, 0x5A);
        func_8013F15C(0x512, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(0);
        return;
    case 2:
        func_8014F080(0x14);
        return;
    case 3:
        func_8014EF8C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80156CC0", func_80156D78);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6354;
extern s32 D_80121128;
extern u8 D_80121136;
/* end bulk_m2c declarations */
void func_80157158(void) {
    s32 sp34;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;

    temp_v1 = D_8015EDD8;
    if (temp_v1 < 0x50) {
        if (D_8015ED98 == 0x02000000) {
            D_8015ED98 = 0x100;
        }
        func_8013D030();
    }
    if ((temp_v1 >= 4) && (temp_v1 < 0x51)) {
        func_8013F15C(0x507, 8, 3);
    }
    switch (temp_v1) {
    case 0x0:
        func_800AE0F0(D_8015E828, "宇宙人「ЛΦΘζ□Э℃¢Λ");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        D_8015ED98 |= 0x1000;
        func_8013F15C(0x513, 1, 0);
        func_8014EF3C();
        break;
    case 0x1:
        func_8014F178(0);
        break;
    case 0x2:
        D_8015ED98 &= ~0x1000;
        D_8015EC38 = D_8015EDE0 + 0x40;
        D_8015EDE0 += 4;
        func_8013F15C(0x506, 0, 2);
        if (D_8015EDE0 >= 0xD1) {
            func_8014EF3C();
        }
        break;
    case 0x3:
        func_8014F080(0x14);
        break;
    case 0x4:
        func_8013E97C(0x28, 0, 0);
        D_80121128 = 0x51000000;
        func_8013E7C0(0x28, 3, 5, 1);
        func_8014EF3C();
        break;
    case 0x5:
        func_8014F0F8(0x28);
        break;
    case 0x6:
        func_8013E7C0(0x28, 4, 1, 1);
        D_8015EC74 = func_8013E9A8(4) + 3;
        if (D_8015ED98 != 0x100) {
            func_8013E7C0(0x30, 5, 1, 1);
        }
        sp34 = func_8013E9A8(0x10);
        func_8013E97C(0x29, sp34 + 0x108, func_8013E9A8(0x10) + 0x50);
        func_8013E810(0x29, 5, 5, 1);
        func_8013F15C(0x508, 1, 0);
        D_8015EDD8 = 0x50;
        break;
    case 0x50:
        if (!(D_80121136 & 1)) {
            func_8013F15C(0x508, 1, 0);
            temp_v1_2 = D_8015EC74 - 1;
            D_8015EC74 = temp_v1_2;
            if (temp_v1_2 == 0) {
                D_80121128 = 0x41000000;
                func_8013E810(0x28, 0xFF, 1, 0);
                func_8013E810(0x29, 0xFF, 1, 0);
                func_8014EF3C();
            } else {
                sp34 = func_8013E9A8(0x10);
                func_8013E97C(0x29, sp34 + 0x108, func_8013E9A8(0x10) + 0x50);
                func_8013E810(0x29, 5, 5, 1);
            }
        }
        break;
    case 0x51:
        func_801447E0();
        if (D_8015EBB0 >= D_8015ED94) {
            D_8015EBB0 = D_8015ED94 - 1;
        }
        func_8014D11C((s32) D_8015E828, (s32) &D_800E6354, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        break;
    case 0x52:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 1) {
            D_8015ED98 = 0x1000;
            D_8015EDC4 = (D_8015EDC4 & 0xFFFF0000) | 3;
            D_8015EDD8 = 0;
            func_8013E7C0(0x30, 4, 1, 1);
        } else {
            func_8014EF3C();
        }
        break;
    case 0x53:
        func_8014F178(0);
        break;
    case 0x54:
        D_8015EC38 = 0x110 - D_8015EDE0;
        temp_v1_3 = D_8015EDE0 + 4;
        D_8015EDE0 = temp_v1_3;
        if (temp_v1_3 >= 0xD1) {
            func_8014EF3C();
        }
        break;
    case 0x55:
        func_8014F080(0x14);
        break;
    case 0x56:
        func_8014EF8C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80156CC0", func_80157640);

void func_8015785C(void) {
    func_8013E97C(0x2F, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2A, D_8015EC38 - 0x67, D_8015EC3C - 6);
    func_8013E97C(0x2B, D_8015EC38 - 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2C, D_8015EC38 + 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2D, D_8015EC38 + 0x67, D_8015EC3C - 6);
    if (!(D_8012117A & 1)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
        D_8015EC40 = 0;
    }
    if (!(D_801211BE & 1)) {
        func_8013E810(0x2B, 0xFF, 1, 0);
        D_8015EC44 = 0;
    }
    if (!(D_80121202 & 1)) {
        func_8013E810(0x2C, 0xFF, 1, 0);
        D_8015EC48 = 0;
    }
    if (!(D_80121246 & 1)) {
        func_8013E810(0x2D, 0xFF, 1, 0);
        D_8015EC4C = 0;
    }
    if ((D_8015EC40 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2A, 7, 5, 1);
        D_8015EC40 = 1;
    }
    if ((D_8015EC44 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2B, 7, 5, 1);
        D_8015EC44 = 1;
    }
    if ((D_8015EC48 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2C, 7, 5, 1);
        D_8015EC48 = 1;
    }
    if (D_8015EC4C == 0) {
        if (func_8013E9A8(0x64) == 0) {
            func_8013E810(0x2D, 7, 5, 1);
            D_8015EC4C = 1;
        }
    }
}
