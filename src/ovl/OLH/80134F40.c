#include "common.h"
#include "ovl/OLH.h"

void func_80134F40(void) {
    switch (D_800E6280.step) {
    case 0x0:
        func_8013500C();
        break;
    case 0x1:
        func_80135190();
        break;
    case 0x10:
        func_801352A0();
        break;
    case 0x20:
        func_8013538C();
        break;
    case 0x30:
        func_80135494();
        break;
    case 0x40:
        func_8013559C();
        break;
    case 0x50:
        func_801356DC();
        break;
    case 0x60:
        func_80135828();
        break;
    case 0x70:
        func_80135974();
        break;
    }
}

void func_8013500C(void) {
    s32 i;
    s32 r;
    s32 padx[2]; /* FAKE: two unused words above the arrays reproduce the original frame 0xC0; real source unknown. T-9310 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"サブゲーム", 0);
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        if (i < 4) {
            set_kanji_string(a[i], b[i], 0xF, (&D_8013C780)[i], 0);
        }
    }
    r = set_kanji_string(a[7], b[7], 0xF, D_8013C79C, 0);
    c[4] = c[5] = c[6] = 0;
    if (!(D_800E7D4F & 1)) {
        c[0] = 0;
    }
    if (!(D_800E7D50 & 1)) {
        c[1] = 0;
    }
    menu_set(0, 8, a, b, c, d);
    k_disp_start(r);
    func_8004284C();
}

void func_80135190(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.pad_pressed & 0x20) {
        if (D_800E6280.menu_cursor_0 != -1) {
            func_80044750(0x501);
            switch (*(u8 *)&D_800E6280.menu_cursor_0) {
            case 0:
                func_80042940(0x10);
                break;
            case 1:
                func_80042940(0x20);
                break;
            case 2:
                func_80042940(0x30);
                break;
            case 3:
                func_80042940(0x40);
                break;
            case 4:
                func_80042940(0x50);
                break;
            case 7:
                func_80042908(0);
                break;
            }
        }
    } else if (D_800E6280.pad_pressed & 0x40) {
        func_80042908(0);
    }
}

s32 func_801352A0(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C780, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"　画面左下の対地速度表示をよく見てスク", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"ロールを早くすることが、タイム短縮の秘", 0);
        set_kanji_string(-0x88, -0x10, 0xF, (s32)"訣です。", 0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_8013538C(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C784, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"　ローリングした瞬間の無敵を使いこなす", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"のが重要です。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, (s32)"　アイテムは、全て点数が入るので、でき", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"るだけ回収しましょう。", 0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80135494(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C788, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"　スコアはサブゲームを終了して、ときめ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"きメモリアルに戻るときに、システムファ", 0);
        set_kanji_string(-0x88, -0x10, 0xF, (s32)"イルにセーブされます。", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"（システムファイルは０番です）", 0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_8013559C(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C78C, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"　サブゲームのデータは、電源オン後最初", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"に読み込みしたメモリーカードのものが、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, (s32)"使用されます。", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"　途中でメモリーカードを差し替えると前", 0);
        set_kanji_string(-0x88, 0x10, 0xF, (s32)"のデータのハイスコアなどが、上書きされ", 0);
        set_kanji_string(-0x88, 0x20, 0xF, (s32)"るので注意してください。", 0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_801356DC(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C790, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"い", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"う", 0);
        set_kanji_string(-0x88, 0x10, 0xF, (s32)"え", 0);
        set_kanji_string(-0x88, 0x20, 0xF, (s32)"お", 0);
        set_kanji_string(-0x88, 0x30, 0xF, (s32)"か", 0);
        set_kanji_string(-0x88, 0x40, 0xF, (s32)"き", 0);
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80135828(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C794, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"い", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"う", 0);
        set_kanji_string(-0x88, 0x10, 0xF, (s32)"え", 0);
        set_kanji_string(-0x88, 0x20, 0xF, (s32)"お", 0);
        set_kanji_string(-0x88, 0x30, 0xF, (s32)"か", 0);
        set_kanji_string(-0x88, 0x40, 0xF, (s32)"き", 0);
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80135974(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013C798, 0);
        set_kanji_string(-0x88, -0x30, 0xF, (s32)"あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, (s32)"い", 0);
        set_kanji_string(-0x88, 0, 0xF, (s32)"う", 0);
        set_kanji_string(-0x88, 0x10, 0xF, (s32)"え", 0);
        set_kanji_string(-0x88, 0x20, 0xF, (s32)"お", 0);
        set_kanji_string(-0x88, 0x30, 0xF, (s32)"か", 0);
        set_kanji_string(-0x88, 0x40, 0xF, (s32)"き", 0);
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}
