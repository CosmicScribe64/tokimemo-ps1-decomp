#define RPG_BAT_OVERRIDE_func_8013E7C0 /* implicit int view: the call's $v0 moves the 0x10-scaled value of func_8013F82C to $v1 (T-17114) */
#define MAIN_API_OVERRIDE_D_8012077C /* matched as s16 (main_api.h: s32, EVENT stores a pointer) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s16 D_8012077C;
s32 func_8013E7C0();

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

void func_8013F82C(void) {
    s32 i;
    s32 x;
    s16 h;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "ロボ「ガギグ・・・ッ");
        func_8014B738(D_8015E8F0, 1, 0x5A);
        func_8013E54C(4);
        D_8015ED98 = 0x1000;
        func_8013F15C(0x510, 1, 0);
        func_8014EF3C();
        break;
    case 1:
        func_8014F178(1);
        break;
    case 2:
        D_8015EDE0 += 1;
        D_8015ED14 += 1;
        switch (D_8015EDE0) {
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
        /* FAKE: the slot table D_8015EBAC (D_8015EC74 is its element 50) is reached through its first symbol; the aggregate is not recovered yet. T-17114 */
        for (i = 0; i < 8; i++) {
            if (D_8015ED14 >= 2 && (&D_8015EC74)[i] == 0 && func_8013E9A8(5) == 0) {
                D_8015ED14 = 0;
                x = func_8013E9A8(0x40);
                func_8013E97C(i + 5, x + 0x28, func_8013E9A8(0x40) + 0x40);
                func_8013E810(i + 5, 0x17, 5, 1);
                if (D_8015EDE4 < 0xB0) {
                    func_8013F15C(0x50D, 2, 1);
                }
            }
            if (!(((DisplayActor44 *)D_8011ECD0)[i + 101].unk_02 & 1)) {
                func_8013E810(i + 5, 0xFF, 1, 0);
                (&D_8015EBAC)[i + 50] = 0;
            }
        }
        if (D_8015EDE0 >= 0x3D) {
            D_8015EDE4 += 2;
            func_8013E97C(4, 0, 0);
            func_8013E7C0(4, 0x18, 1, 1);
            D_80120778 = 5;
            h = D_8015EDE4 * 0x10;
            D_8012077A = h;
            D_8012077C = h;
            /* grey level c * 0x101 * 0x101 as two steps c + (c << 8): uopt keeps D / 3 and the inner sum in $t0 and $t1 (a plain * 0x10201 is one shift/add chain in ugen temporaries). T-17114 */
            func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (((D_8015EDE4 / 3) + ((D_8015EDE4 / 3) << 8)) + (((D_8015EDE4 / 3) + ((D_8015EDE4 / 3) << 8)) << 8)), 0x81);
            dtd_on_tpage(0, 0, 2, 1, 0);
        }
        if (D_8015EDE4 == 0xB0) {
            func_8013F15C(0x50C, 1, 0);
        }
        if (D_8015EDE4 >= 0x151) {
            for (i = 0; i != 8; i++) {
                func_8013E810(i + 5, 0xFF, 1, 0);
                func_8013E810(0x3B, 0xFF, 1, 0);
                D_80120778 = 5;
            }
            k_disp_switch(D_8015EDE8, 0);
            k_disp_switch(D_8015EE40, 0);
            func_8014EF3C();
        }
        break;
    case 3:
        D_8015EDE4 += 4;
        func_8013E7C0(4, 0x18, 1, 1);
        D_80120778 = 5;
        x = 0x150 - D_8015EDE4;
        h = x * 0x10;
        D_8012077A = h;
        D_8012077C = h;
        /* same two-step grey level as in case 2 */
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (((x / 3) + ((x / 3) << 8)) + (((x / 3) + ((x / 3) << 8)) << 8)), 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (D_8015EDE4 >= 0x149) {
            func_8013E810(4, 0xFF, 1, 0);
            func_8014EF3C();
        }
        break;
    case 4:
        func_8014F080(0x3C);
        break;
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

/* returns int without a value, as func_8013F4F0: its call above is a statement call of an implicit int function (T-17114) */
s32 func_80140204(void) {
    s32 i;
    s32 *p;

    switch (D_8015EDDC) {
    case 0:
        func_8013E810(0x3B, 0xA, 5, 1);
        func_8013F15C(0x506, 1, 0);
        func_8014EF6C();
        break;
    case 1:
        func_8014F138(0x3B);
        break;
    case 2:
        D_801215FE = 0x40;
        D_80121614 = 0x19;
        func_8013E97C(0x39, 0x60, 0x78);
        func_8013E810(0x39, 0xC, 1, 1);
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 9, 5, 1);
        func_8014EF6C();
        break;
    case 3:
        func_8013E97C(0x39, D_8015EDE4 + 0x62, 0x76 - D_8015EDE4);
        D_8015EDE4 += 2;
        if (!(D_80121532 & 1)) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
        }
        if (D_8015EDE4 + 0x5A >= 0x77) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
            func_8013E810(0x39, 0xD, 5, 1);
            D_80121575 = 2;
            func_8014EF6C();
        }
        break;
    case 4:
        for (i = 0; i != 5; i++) {
            func_8013E97C(i + 4, 0x80, 0x58);
            func_8013E810(i + 4, 0x14, 1, 1);
        }
        func_8014EF6C();
        break;
    case 5:
        if (D_8015EDE4 % 2 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_26 -= 1;
            ((DisplayActor44 *)D_8011ECD0)[103].unk_26 -= 2;
            ((DisplayActor44 *)D_8011ECD0)[102].unk_2A += 1;
            ((DisplayActor44 *)D_8011ECD0)[104].unk_2A += 2;
        }
        if (D_8015EDE4 % 2 == 0) {
            for (i = 0; i < 5; i++) {
                ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_26 += 1;
                ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_2A -= 1;
            }
        }
        D_8015EDE4 += 1;
        if (D_8015EDE4 >= 0x21) {
            func_8014EF6C();
        }
        break;
    case 6:
        if (D_8015EDE4 >= 0x1B) {
            func_8013F15C(0x507, 0, 1);
            ((DisplayActor44 *)D_8011ECD0)[100].unk_26 += 3;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_2A -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(0);
        }
        if (D_8015EDE4 >= 0xE) {
            func_8013F15C(0x507, 0, 2);
            ((DisplayActor44 *)D_8011ECD0)[101].unk_26 += 3;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_2A -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(1);
        }
        if (D_8015EDE4 >= 0xE) {
            ((DisplayActor44 *)D_8011ECD0)[102].unk_26 += 3;
            ((DisplayActor44 *)D_8011ECD0)[102].unk_2A -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(2);
        }
        if (D_8015EDE4 > 0) {
            func_8013F15C(0x507, 0, 3);
            ((DisplayActor44 *)D_8011ECD0)[103].unk_26 += 3;
            ((DisplayActor44 *)D_8011ECD0)[103].unk_2A -= 3;
        }
        if (D_8015EDE4 > 0) {
            ((DisplayActor44 *)D_8011ECD0)[104].unk_26 += 3;
            ((DisplayActor44 *)D_8011ECD0)[104].unk_2A -= 3;
        }
        D_8015EDE4 += 1;
        if (D_8015EDE4 >= 0x51) {
            func_8014EF6C();
        }
        break;
    case 7:
        func_8014F0BC(0x1E);
        break;
    case 8:
        D_8015EDE4 += 1;
        D_8015ED14 += 1;
        /* FAKE: p[50] walks the slot table D_8015EBAC through its first symbol (element 50 on); the aggregate is not recovered yet. T-17114 */
        p = &D_8015EBAC;
        for (i = 0; i != 0xC; i++, p++) {
            if (p[50] == 3 && !(D_8015ED98 & 0x100)) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            if (D_8015EDE4 < 0x78 && D_8015ED14 >= 3 && p[50] == 0 && func_8013E9A8(5) == 0) {
                func_8013F15C(0x508, 1, 0);
                D_8015ED14 = 0;
                if (func_8013E9A8(0x64) < 0x1E) {
                    p[50] = 1;
                    func_8013E97C(i + 4, func_8013E9A8(0x18) + 0x104, 0);
                    func_8013E810(i + 4, 0x16, 1, 1);
                } else {
                    p[50] = 2;
                    func_8013E97C(i + 4, func_8013E9A8(0x40) + 0xF0, 0);
                    func_8013E810(i + 4, 0x16, 1, 1);
                }
            }
            if (p[50] == 1) {
                ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_2A += 5;
                if (func_8013E9A8(4) - 0x22 < ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_2A) {
                    func_8013E810(i + 4, 0xE, 5, 1);
                    p[50] = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if (p[50] == 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_2A += 5;
                if (func_8013E9A8(4) + 8 < ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_2A) {
                    func_8013E810(i + 4, 0xE, 5, 1);
                    p[50] = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if (p[50] == 3 && !(((DisplayActor44 *)D_8011ECD0)[i + 100].unk_02 & 1)) {
                func_8013E810(i + 4, 0xFF, 1, 0);
                p[50] = 0;
            }
        }
        if (D_8015EDE4 >= 0xDD) {
            func_8013D030();
            func_8014EF6C();
        }
        break;
    case 9:
        func_8013E810(0x3B, 0xB, 5, 1);
        for (i = 0; i != 0xC; i++) {
            func_8013E810(i + 4, 0xFF, 1, 0);
        }
        func_8014EF6C();
        break;
    case 10:
        func_8014F138(0x3B);
        break;
    case 11:
        func_8014EF3C();
        break;
    }
}

/* T-14501: retain the earlier unprototyped callers' implicit s32 view. */
s32 func_80140A58(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_8015EDDC) {
    case 0:
        func_8013E97C(0x3A, -8, 0);
        func_8013E7C0(0x3A, 0xF, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF6C();
        return;
    case 1:
        if (!(D_801215BA & 1)) {
            func_8013E7C0(0x3A, 0x10, 5, 1);
            func_8013F15C(0x50A, 1, 0);
            func_8014EF6C();
            return;
        }
    default:
        return;
    case 2:
        if (!(D_801215BA & 1)) {
            func_8013F15C(0x50A, 1, 0);
            func_8013E7C0(0x3A, 0x10, 5, 1);
            temp_v0 = D_8015EDE4 + 1;
            D_8015EDE4 = temp_v0;
            if (temp_v0 >= 4) {
                func_8013E7C0(0x3A, 0x10, 1, 1);
                func_8013E810(0x3B, 0x12, 5, 1);
                func_8014EF6C();
                return;
            }
        }
        break;
    case 3:
        func_8014F138(0x3B);
        return;
    case 4:
        func_8013E7C0(0x3A, 0x11, 5, 1);
        if (!(D_8015ED98 & 0x100)) {
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        func_8013F15C(0x50B, 1, 0);
        func_8014EF6C();
        return;
    case 5:
        if (!(D_801215BA & 1)) {
            func_8013F15C(0x50B, 1, 0);
            func_8013E7C0(0x3A, 0x11, 5, 1);
            temp_v0_2 = D_8015EDE4 + 1;
            D_8015EDE4 = temp_v0_2;
            if (temp_v0_2 >= 9) {
                func_8013E7C0(0x3A, 0xFF, 1, 0);
                func_8013E810(0x3B, 0x13, 5, 1);
                func_8014EF6C();
                return;
            }
        }
        break;
    case 6:
        func_8014F138(0x3B);
        return;
    case 7:
        func_8013E810(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    }
}

void func_80140CC8(void) {
    s32 temp_v0;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        strcat(D_8015E828, "「よせっ！　紐緒さん！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(0);
        return;
    case 2:
        func_8013E810(0x3B, 1, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8013F4F0();
        return;
    case 4:
        func_8013E810(0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 5:
        func_8014F080(0x1E);
        return;
    case 6:
        func_8013E97C(0x3B, 0x48 - D_8015EDE0, 0x70);
        temp_v0 = D_8015EDE0 + 0x10;
        D_8015EDE0 = temp_v0;
        if (temp_v0 >= 0x61) {
            k_disp_switch(D_8015EE14, 0);
            func_8014EF3C();
            return;
        }
    default:
        return;
    case 7:
        func_8014F080(0x1E);
        return;
    case 8:
        func_80140E70();
        return;
    case 9:
        func_8014EF8C();
        k_disp_switch(D_8015EDE8, 1);
        D_8015EDC4 = 0x08000001;
        break;
    }
}

void func_80140E70(void) {
    switch (D_8015EDDC) {
    case 0:
        func_8013E97C(0x3B, 0x48, 0x60);
        func_8013E810(0x3B, 2, 1, 1);
        load_palette(D_8015EEEC, 0x12, 1, 4, 0);
        func_8014EF6C();
        break;
    case 1:
        D_8015EDE4++;
        if (D_8015EDE4 == 0x1E) {
            func_8013F15C(0x504, 1, 0);
        }
        if (!(D_8015EDE4 & 3)) {
            load_palette(D_8015EEEC, 0x12, 1, 4, (D_8015EDE4 / 4) % 5);
        }
        if (D_8015EDE4 >= 0x4F) {
            func_8014EF6C();
        }
        break;
    case 2:
        D_8015EDE4++;
        if (!(D_8015EDE4 & 3)) {
            load_palette(D_8015EEEC, 0x12, 1, 4, (D_8015EDE4 / 4) % 6 + 5);
        }
        if (D_8015EDE4 >= 0x17) {
            func_8014EF6C();
        }
        break;
    case 3:
        load_palette(D_8015EED8, 0x12, 1, 4, 4);
        func_8014EF3C();
        break;
    }
}
