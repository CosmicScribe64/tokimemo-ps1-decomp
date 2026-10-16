#define MAIN_API_OVERRIDE_func_80049A40 /* T-9740: original caller passes full-width coordinates without a prototype. */
#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80049A40();

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8014F790(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "紐緒「想定強度に問題があったみたいね");
        func_8013E7C0(0x1D, 0x19, 1, 1);
        func_8014B738(D_8015EB70, 1, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x38, 0x09000041, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8013F220();
        func_8014F230();
        return;
    }
}

void func_8014F88C(void) {
    s32 sp28; /* FAKE (T-17100): dead copy of the loop index; without it the 0x28 spill and the frame change */
    s32 var_a0;

    D_8015EE00.unk_00 += 1;
    ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 3;
    ((DisplayActor44 *)D_8011ECD0)[126].unk_01 = 3;
    if ((D_8015EE00.unk_04 >= 2) && (D_8015EE00.unk_04 < 0x15) && (((DisplayActor44 *)D_8011ECD0)[125].unk_08 == 1)) {
        /* u16 view of the s16 unk_18: the original loads it with lhu and range-checks with sltiu */
        switch (*(u16 *)&((DisplayActor44 *)D_8011ECD0)[125].unk_18) {
        case 0:
        case 2:
            if (func_8013E9A8(3) == 0) {
                func_8013F15C(0x50C, 1, 9);
            }
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50B, 1, 9);
            }
            break;
        case 4:
        case 8:
        case 12:
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50A, 1, 9);
            }
            break;
        }
    }
    switch (D_8015EE00.unk_04) {
    case 0:
        func_8013E54C(5);
        var_a0 = 4;
        do {
            sp28 = var_a0;
            func_8013E97C(var_a0, -0x20, 0);
            var_a0 += 1;
        } while (var_a0 != 0x10);
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8013E7C0(0x1E, 2, 1, 1);
        func_8013E97C(0x1D, -0x20, 0);
        func_8013F15C(0x509, 1, 0);
        func_8014F230();
        break;
    case 1:
        if (D_8015EE00.unk_00 >= 0x3D) {
            func_8014F230();
        }
        break;
    case 2:
        func_8013E7C0(8, 7, 1, 1);
        func_8014F230();
        break;
    case 3:
        if (D_8015EE00.unk_00 >= 0x4B1) {
            func_8014F230();
        }
        break;
    case 4:
        func_8013E7C0(8, 8, 1, 1);
        func_8014F230();
        break;
    case 5:
        if (D_8015EE00.unk_00 >= 0x961) {
            func_8014F230();
        }
        break;
    case 6:
        func_8013E7C0(8, 9, 1, 1);
        func_8014F230();
        break;
    case 7:
        if (D_8015EE00.unk_00 >= 0xE11) {
            func_8014F230();
        }
        break;
    case 8:
        func_8013E7C0(0x1E, 3, 1, 1);
        func_8013E7C0(8, 0xA, 1, 1);
        func_8014F230();
        break;
    case 9:
        if (D_8015EE00.unk_00 >= 0x12C1) {
            func_8014F230();
        }
        break;
    case 10:
        func_8013E7C0(8, 0xB, 1, 1);
        func_8014F230();
        break;
    case 11:
        if (D_8015EE00.unk_00 >= 0x1771) {
            func_8014F230();
        }
        break;
    case 12:
        func_8013E7C0(0x1E, 4, 1, 1);
        func_8013E7C0(8, 0xC, 1, 1);
        func_8014F230();
        break;
    case 13:
        if (D_8015EE00.unk_00 >= 0x1C21) {
            func_8014F230();
        }
        break;
    case 14:
        func_8013E7C0(8, 0xD, 1, 1);
        func_8014F230();
        break;
    case 15:
        if (D_8015EE00.unk_00 >= 0x20D1) {
            func_8014F230();
        }
        break;
    case 16:
        func_8013E7C0(0x1E, 5, 1, 1);
        func_8013E7C0(8, 0xE, 1, 1);
        func_8014F230();
        break;
    case 17:
        if (D_8015EE00.unk_00 >= 0x2581) {
            func_8014F230();
        }
        break;
    case 18:
        func_8013E7C0(8, 0xF, 1, 1);
        func_8014F230();
        break;
    case 19:
        if (D_8015EE00.unk_00 >= 0x2A31) {
            func_8014F230();
        }
        break;
    case 20:
        if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF)) {
            func_8014F278();
            func_8013F15C(0x50D, 1, 0);
            D_8015EDF0 = 0x10001001;
        }
        break;
    }
}

void func_8014FDBC(void) {
    D_80120E05 = 3;
    D_80120E49 = 3;
    switch (D_8015EE04) {
    case 0:
        func_8013E97C(0x1D, 0, 0);
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8014F230();
        return;
    case 1:
        func_8014F350(0x3C);
        return;
    case 2:
        func_8013E7C0(7, 0x10, 5, 1);
        func_8014F230();
        return;
    case 3:
        func_8014F3C8(7);
        return;
    case 4:
        func_8013E7C0(7, 0x11, 1, 1);
        func_8014F230();
        return;
    case 5:
        func_8013E7C0(6, 0x12, 1, 1);
        func_8013F15C(0x50F, 1, 0);
        func_8014F230();
        return;
    case 6:
        func_8014F350(0x3C);
        return;
    case 7:
        func_8013E7C0(6, 0x13, 1, 1);
        func_8013F15C(0x50F, 1, 0);
        func_8014F230();
        return;
    case 8:
        func_8014F350(0x3C);
        return;
    case 9:
        func_8013E7C0(6, 0x14, 1, 1);
        func_8014F230();
        return;
    case 10:
        func_8013F15C(0x50E, 0xF, 1);
        func_8014F350(0xB4);
        return;
    case 11:
        func_8013E7C0(6, 0xFF, 1, 0);
        func_8013E7C0(7, 0xFF, 1, 0);
        func_8013E7C0(8, 0xF, 1, 1);
        func_8013EA60(0, 5, 0x270F, 1);
        D_8015EDC0 = 0;
        func_8014F230();
        return;
    case 12:
        func_8014F350(0x3C);
        return;
    case 13:
        func_80150120();
        return;
    case 14:
        func_8014F350(0x3C);
        return;
    case 15:
        func_800AE0F0(D_8015E9E0, "宇宙人は去っていった。");
        func_8014B738(D_8015EB70, 1, 0x3C);
        func_8013E7C0(0x30, 9, 1, 1);
        func_8004500C(1, 0x202);
        func_8014F230();
        return;
    case 16:
        func_8014F448(0);
        return;
    case 17:
        func_8014F350(0x78);
        return;
    case 18:
        func_8013E7C0(0x30, 0, 1, 1);
        func_8013E7C0(8, 0x15, 5, 1);
        func_8013F15C(0x510, 1, 0);
        func_8014F230();
        return;
    case 19:
        func_8014F3C8(8);
        return;
    case 20:
        func_8014F350(0x1E);
        return;
    case 21:
        func_8014F790();
        return;
    case 22:
        func_8014F350(0x3C);
        return;
    case 23:
        D_8015EC14 = 0;
        func_80042808();
        /* fallthrough */
    default:
        return;
    }
}

void func_80150120(void) {
    u32 shade;
    s32 count;
    RpgColorWord color;
    u32 selector;

    selector = func_8013E9A8(8);
    switch (selector) {
    case 0:
        color.word = 0x9F9F9F;
        break;
    case 1:
        color.word = 0x6F6F6F;
        break;
    case 2:
        color.word = 0xBF0000;
        break;
    case 3:
        color.word = 0xBF00;
        break;
    case 4:
        color.word = 0xBF;
        break;
    case 5:
        color.word = 0xAFAF00;
        break;
    case 6:
        color.word = 0xAF00AF;
        break;
    case 7:
        color.word = 0xAFAF;
        break;
    }
    D_8015ED98 = 0x1000;
    shade = color.word;
    switch (((RpgTransition0C *)&D_8015EE08)->unk_00) {
    case 0:
        func_800AE0F0(D_8015E828, "宇宙人「仝Щ△※〒ヰヱΞ＆");
        func_800AE0F0(D_8015E850, "ΠΣΨБДЗЦЧ＠Ъ◇Ш");
        func_8014B738(D_8015E8F0, 2, 0x5A);
        func_8013F15C(0x513, 1, 0);
        func_8014F258();
    default:
        break;
    case 1:
        func_8014F4A4(1);
        break;
    case 2:
        func_8013F15C(0x512, 1, 0);
        func_8014F258();
        break;
    case 3:
        func_8014F4A4(0);
        break;
    case 4:
        D_8015EC3C = 0x18 - ((RpgTransition0C *)&D_8015EE08)->unk_08;
        count = ((RpgTransition0C *)&D_8015EE08)->unk_08 += 2;
        if (count >= 0x79) {
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 9, 1, 1);
            D_80121531 = 2;
            func_8013E7C0(0x3B, 8, 1, 1);
            func_8014F258();
        }
        break;
    case 5:
        func_80049A40(-0x5C - ((RpgTransition0C *)&D_8015EE08)->unk_08, -0x78, ((RpgTransition0C *)&D_8015EE08)->unk_08 * 2, 0xA8, 3, shade, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        count = ((RpgTransition0C *)&D_8015EE08)->unk_08 += 2;
        if (count >= 0x21) {
            func_8014F258();
        }
        break;
    case 6:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, shade, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        func_8013F15C(0x511, 0, 6);
        func_8013E97C(0x3B, 0x48, 0x68 - ((RpgTransition0C *)&D_8015EE08)->unk_08);
        func_8013E810(0x3B, 8, 1, 1);
        count = ((RpgTransition0C *)&D_8015EE08)->unk_08 += 2;
        if (count >= 0xE9) {
            func_8004E93C(D_8015EDE8, 0);
            func_8014F258();
        }
        break;
    case 7:
        func_80049A40(((RpgTransition0C *)&D_8015EE08)->unk_08 - 0x7C, -0x78, 0x40 - (((RpgTransition0C *)&D_8015EE08)->unk_08 * 2), 0xA8, 3, shade, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        count = ((RpgTransition0C *)&D_8015EE08)->unk_08 += 2;
        if (count >= 0x21) {
            func_8014F230();
        }
        break;
    }
}
