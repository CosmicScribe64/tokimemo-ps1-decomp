#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8013F4F0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "紐緒「真・世界征服ロボ！！");
        func_8014B738(D_8015E8F0, 1, 0xF0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A, 0x05000026, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value, as func_8013F4F0 (T-8080) */
s32 func_8013F5C8(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　　服従ドリルパンチ　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xC8);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C000020, 0x05000029, 1);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F694(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　独裁ミサイルシャワー　　　　");
            func_8014B738(D_8015E8F0, 1, 0x118);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1B000020, 0x0300002F, 2);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F760(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　世界征服メガビーム　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xAA);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C00002A, 0x06000036, 0);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_8012077A;
extern s16 D_8012077C;
/* end bulk_m2c declarations */
void func_8013F82C(void) {
    s32 sp4C;
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_s1;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_s2;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "ロボ「ガギグ・・・ッ");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8013E54C(4);
        D_8015ED98 = 0x1000;
        func_8013F15C(0x510, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(1);
        return;
    case 2:
        temp_t9 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t9;
        D_8015ED14 += 1;
        switch (temp_t9) {
        case 1:
            load_palette(D_8015EEEC, 0x13, 1, 4, 0xA);
            D_8012163F = 0x13;
            break;
        case 10:
            load_palette(D_8015EEEC, 0x13, 1, 4, 9);
            break;
        case 20:
            load_palette(D_8015EEEC, 0x13, 1, 4, 8);
            break;
        case 30:
            load_palette(D_8015EEEC, 0x13, 1, 4, 7);
            break;
        case 40:
            load_palette(D_8015EEEC, 0x13, 1, 4, 6);
            break;
        case 50:
            load_palette(D_8015EEEC, 0x13, 1, 4, 5);
            break;
        }
        var_s2 = D_8011ECD0;
        var_s0 = 0;
        do {
            if ((D_8015ED14 >= 2) && ((&D_8015EC74)[var_s0] == 0) && (func_8013E9A8(5) == 0)) {
                D_8015ED14 = 0;
                temp_s1 = func_8013E9A8(0x40);
                temp_v0 = func_8013E9A8(0x40);
                temp_a0 = var_s0 + 5;
                sp4C = temp_a0;
                func_8013E97C(temp_a0, temp_s1 + 0x28, temp_v0 + 0x40);
                func_8013E810(temp_a0, 0x17, 5, 1);
                if (D_8015EDE4 < 0xB0) {
                    func_8013F15C(0x50D, 2, 1);
                }
            }
            if (!(*(u8 *)((unsigned char *)var_s2 + (6870)) & 1)) {
                func_8013E810(var_s0 + 5, 0xFF, 1, 0);
                *(s32 *)((unsigned char *)(&(&D_8015EBAC)[var_s0]) + (200)) = 0;
            }
            var_s0 += 1;
            var_s2 += 0x44;
        } while (var_s0 < 8);
        if (D_8015EDE0 >= 0x3D) {
            D_8015EDE4 += 2;
            func_8013E97C(4, 0, 0);
            func_8013E7C0(4, 0x18, 1, 1);
            D_80120778 = 5;
            temp_v1 = D_8015EDE4 * 0x10;
            D_8012077A = temp_v1;
            D_8012077C = temp_v1;
            func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, ((s32) D_8015EDE4 / 3) * 0x10201, 0x81);
            dtd_on_tpage(0, 0, 2, 1, 0);
        }
        if (D_8015EDE4 == 0xB0) {
            func_8013F15C(0x50C, 1, 0);
        }
        var_s0_2 = 0;
        if (D_8015EDE4 >= 0x151) {
            do {
                func_8013E810(var_s0_2 + 5, 0xFF, 1, 0);
                func_8013E810(0x3B, 0xFF, 1, 0);
                var_s0_2 += 1;
                D_80120778 = 5;
            } while (var_s0_2 != 8);
            k_disp_switch(D_8015EDE8, 0);
            k_disp_switch(D_8015EE40, 0);
            func_8014EF3C();
            return;
        }
    default:
        return;
    case 3:
        D_8015EDE4 += 4;
        func_8013E7C0(4, 0x18, 1, 1);
        D_80120778 = 5;
        temp_v1_2 = 0x150 - D_8015EDE4;
        temp_t0 = temp_v1_2 * 0x10;
        D_8012077A = temp_t0;
        D_8012077C = temp_t0;
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (temp_v1_2 / 3) * 0x10201, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (D_8015EDE4 >= 0x149) {
            func_8013E810(4, 0xFF, 1, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 4:
        func_8014F080(0x3C);
        return;
    case 5:
        func_80042808();
        break;
    }
}

void func_8013FD8C(void) {
    switch (D_8015EDD8) {
    case 0:
        func_801447E0();
        D_80121311 = 6;
        func_8013E54C(4);
        func_8014EF3C();
        break;
    case 1:
        func_8013F5C8();
        func_8013FF28();
        break;
    case 2:
        func_8014F080(0x3C);
        break;
    case 3:
        func_8013F220();
        func_8014EF3C();
        break;
    case 4:
        func_8013F694();
        func_80140204();
        break;
    case 5:
        func_8013F220();
        func_8014EF3C();
        break;
    case 6:
        func_8013F760();
        func_80140A58();
        break;
    case 7:
        func_8013F220();
        func_8014EF3C();
        break;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        func_8014EF3C();
        break;
    case 9:
        func_8014F080(0x1E);
        break;
    case 10:
        func_8013D030();
    case 11:
        func_8014F080(0x14);
        break;
    case 12:
        if (D_8015ED98 & 0x1000) {
            D_8015ED98 = D_8015ED98 & ~0x1000;
            D_8015ED9C = 0xFFFF;
        }
        func_8013E54C(0xFF);
        func_8014EF8C();
        break;
    }
}

void func_8013FF28(void) {
    /* FAKE: case 6 on one line retains the global-store scheduling. T-9750. */
    s32 temp_v0;

    switch (D_8015EDDC) {
    case 0:
        func_8013E810(0x3B, 4, 1, 1);
        func_8014EF6C();

    default:
        break;

    case 1:
        func_8014F0BC(7);
        break;

    case 2:
        func_8013E810(0x3B, 3, 1, 1);
        func_8013E97C(0x3A, 0x70, 0x48);
        func_8013E810(0x3A, 5, 5, 1);
        func_8014EF6C();
        break;

    case 3:
        func_8014F138(0x3A);
        break;

    case 4:
        func_8013E810(0x3A, 6, 1, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF6C();
        break;

    case 5:
        func_8013E97C(0x3A, D_8015EDE4 + 0x70, 0x48);
        D_8015EDE4 += 8;
        if ((D_8015EDE4 >= 0x91) && (!(D_8015ED98 & 0x100))) {
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        if (D_8015EDE4 >= 0x129) {
            func_8013E810(0x3A, 7, 1, 1);
            func_8014EF6C();
        }
        break;

    case 6: func_8013E97C(0x3A, 0x198 - D_8015EDE4, 0x48); temp_v0 = D_8015EDE4 + 8; D_8015EDE4 = temp_v0; if ((0x198 - temp_v0) < 0x70) { func_8013D030(); func_8014EF6C(); } break;

    case 7:
        func_8013E97C(0x39, 0x48, 0x60);
        func_8013E810(0x39, 8, 1, 1);
        func_8014EF6C();
        break;

    case 8:
        func_8014F0BC(0xB);
        break;

    case 9:
        func_8013E810(0x3B, 4, 1, 1);
        func_8013E810(0x3A, 0xFF, 1, 0);
        func_8013E7C0(0x39, 0xFF, 1, 0);
        func_8014EF6C();
        break;

    case 10:
        func_8014F0BC(7);
        break;

    case 11:
        func_8013E810(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    }
}

void func_801401D0(s32 i) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x1AB6) += 1;
    *(s16 *)(p + 0x1ABA) -= 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140204);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140A58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140CC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140E70);
