#define RPG_BAT_OVERRIDE_func_8013E7C0 /* implicit int view: the calls' $v0 moves the D_8015EDE0 web of func_80133108 to $v1 (T-17118) */
#define RPG_BAT_OVERRIDE_func_8013E97C /* implicit int view, same reason (T-17118) */
#define RPG_BAT_OVERRIDE_func_8013F15C /* implicit int view, same reason (T-17118) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013E7C0();
s32 func_8013E97C();
s32 func_8013F15C();

void func_80132FE0(void) {
    switch (*D_8015ED8C) {
    case 0:
        func_800AE0F0(D_8015E828, "マタギ「こったらとこ来たらだめだべ！");
        func_800AE0F0(D_8015E850, "早くけえれ！");
        func_8014B738((s32) D_8015E8F0, 2, 0xAA);
        *D_8015ED8C += 1;
        return;
    case 1:
        func_8013F1C4(0x0B000001, 0x02000008, 0);
        *D_8015ED8C += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1E000047, 0x0F00004B, 1);
        *D_8015ED8C += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        /* fallthrough */
    default:
        return;
    }
}

void func_80133108(void) {
    s32 i;
    s32 ang;
    s32 tmp; /* the sine of cases 11/13/15 and case 19's call result: one local, one register (s2) */
    s32 cs;
    s32 r;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　サーモンブーメラン　　　　");
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 1, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 1:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 1, 1, 0);
            func_8014B738((s32) D_8015E8F0, 1, 0x3C);
            func_8014EF3C();
        }
        break;
    case 2:
        func_8014F178(1);
        break;
    case 3:
        D_80121531 = 4;
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 2, 1, 1);
        func_8013E810(0x3B, 4, 1, 1);
        func_8014EF3C();
        break;
    case 4:
        D_8015EDE0 += 2;
        func_8013E97C(0x38, (s32) (D_8015EDE0 * 0xE0) / 100, (s32) (D_8015EDE0 * -0x40) / 100);
        if (D_80121548 == 0) {
            func_8013F15C(0x506, 1, 0);
        }
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x38, 0xE0, 0x40);
            func_8014EF3C();
        }
        if (D_8015EDE0 == 0x46) {
            func_8013F15C(0x50B, 1, 6);
            D_8015ED98 = 0x1000;
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        break;
    case 5:
        D_8015EDE0 += 3;
        func_8013E97C(0x38, 0xE0 - ((s32) (D_8015EDE0 * 0xE0) / 100), ((s32) (D_8015EDE0 << 6) / 100) - 0x40);
        if (D_8015EDE0 < 0x50) {
            if (D_80121548 == 0) {
                func_8013F15C(0x506, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 2, 1, 0);
            func_8013E810(0x3B, 0, 1, 1);
            func_8014EF3C();
        }
        if (D_8015EDE0 == 0x33) {
            func_8013E7C0(0x30, 4, 1, 1);
            func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        }
        break;
    case 6:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「だめだ！　喰われてしまう！");
        func_800AE0F0(D_8015E850, "くそったれめー！");
        func_8014B738((s32) D_8015E8F0, 2, 0x78);
        func_8014EF3C();
        break;
    case 7:
        func_8014F178(0);
        break;
    case 8:
        D_8015EDE0 += 1;
        if (D_8015EDE0 < 0x3D) {
            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x30, 0xFF, 1, 0);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 0);
                }
            } else {
                func_8013E7C0(0x30, 0xFF, 1, 1);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 1);
                }
            }
        }
        func_8013E97C(0x28, 0, 0);
        if (D_8015EDE0 >= 0x3D) {
            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x28, 0, 1, 1);
            } else {
                func_8013E7C0(0x28, 0, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x78) {
            D_8015EE18 ^= 0x10000000;
            func_800AE0F0(D_8015E828, "正義のマタギ見参！");
            func_800AE0F0(D_8015E850, "名銃「次郎丸」ショット");
            func_800AE0F0(D_8015E878, "「次郎丸」エネルギー充填");
            func_8014B738((s32) D_8015E8F0, 3, 0x78);
            func_8014EF3C();
        }
        break;
    case 9:
        for (i = 0; i < 32; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_0C = (u8 *) D_8015EEAC;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_10 = (u8 *) D_8015EEC4;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_34 = (u8 *) D_8015EE94;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_43 = 0x18;
        }
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(2);
        break;
    case 11:
        func_8013F15C(0x507, 0, 1);
        ang = (D_8015EDE0 << 0xA) / 60;
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, ((D_8015EDE0 * 6) / 60) + 1, 1, 1);
            tmp = func_800A0070(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x3C - D_8015EDE0;
            func_8013E97C(i - 0x20, ((tmp * r) >> 0xD) - 0x10, (cs * r) >> 0xD);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 == 0x3C) {
            func_8014EF3C();
        }
        break;
    case 12:
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, 0xFF, 1, 0);
        }
        func_8014F080(0xF);
        break;
    case 13:
        func_8013F15C(0x50C, 0, 2);
        ang = (D_8015EDE0 << 0xA) / 45;
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, ((D_8015EDE0 * 6) / 45) + 1, 1, 1);
            tmp = func_800A0070(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x2D - D_8015EDE0;
            func_8013E97C(i - 0x20, ((tmp * r * 0x3C / 0x2D) >> 0xD) - 0x10, (cs * r * 0x3C / 0x2D) >> 0xD);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 == 0x2D) {
            func_8014EF3C();
        }
        break;
    case 14:
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, 0xFF, 1, 0);
        }
        func_8014F080(0xA);
        break;
    case 15:
        func_8013F15C(0x50D, 0, 3);
        ang = (D_8015EDE0 << 0xA) / 30;
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, ((D_8015EDE0 * 6) / 30) + 1, 1, 1);
            tmp = func_800A0070(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x1E - D_8015EDE0;
            func_8013E97C(i - 0x20, ((tmp * r * 2) >> 0xD) - 0x10, (cs * r * 2) >> 0xD);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 == 0x1E) {
            func_8014EF3C();
        }
        break;
    case 16:
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, 0xFF, 1, 0);
        }
        func_8014F080(5);
        break;
    case 17:
        for (i = 0; i < 8; i++) {
            func_8013E7C0(i - 0x20, 0xFF, 1, 0);
        }
        func_8013E7C0(0x28, 7, 5, 1);
        func_8013F15C(0x508, 1, 4);
        func_8014EF3C();
        break;
    case 18:
        func_8014F080(0x14);
        break;
    case 19:
        if (!(D_8015EDE0 & 3)) {
            tmp = func_8013E9A8(0x40);
            func_8013E97C((D_8015EDE0 / 4) - 0x20, tmp - 0xE0, func_8013E9A8(0x60) - 0x40);
            func_8013E7C0((D_8015EDE0 / 4) - 0x20, 8, 5, 1);
        }
        if (D_8015EDE0 >= 0x11 && D_8015EDE0 < 0x60) {
            func_8013F15C(0x509, 0xF, 5);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 >= 0x81) {
            func_8014EF3C();
        }
        break;
    case 20:
        D_8015EDE0 += 1;
        if (!(D_8015EDE0 & 1)) {
            func_8013E97C(0x3B, 0, 0);
            func_8013E7C0(0x3B, 0xFF, 1, 1);
            for (i = 0; i < 0x20; i++) {
                func_8013E7C0(i - 0x20, 0xFF, 0x21, 1);
            }
        } else {
            func_8013E97C(0x3B, 0, 0);
            func_8013E7C0(0x3B, 0xFF, 1, 0);
            for (i = 0; i < 0x20; i++) {
                func_8013E7C0(i - 0x20, 0xFF, 0x21, 0);
            }
        }
        if (D_8015EDE0 >= 0x3D) {
            k_disp_switch(D_8015EDE8, 0);
            func_8014EF3C();
        }
        break;
    case 21:
        func_80132FE0();
        break;
    case 22:
        D_8015EDE0 += 1;
        if (!(D_8015EDE0 & 1)) {
            func_8013E7C0(0x28, 0, 1, 1);
        } else {
            func_8013E7C0(0x28, 0, 1, 0);
        }
        if (D_8015EDE0 >= 0x5B) {
            D_8015EE18 ^= 0x10000000;
            func_8013E7C0(0x30, 0, 1, 1);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8014EF3C();
        }
        break;
    case 23:
        func_8014F080(0x1E);
        break;
    case 24:
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、マタギのおじさん");
        func_800AE0F0(D_8015EA30, "経験値　：１０２４ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：５３ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 25:
        func_8014F080(0x3C);
        break;
    case 26:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 27:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 28:
        func_8014F178(0);
        break;
    case 29:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
