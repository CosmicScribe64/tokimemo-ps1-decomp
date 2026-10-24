#include "common.h"
#include "ovl/OLH.h"

void func_80134500(void) {
    switch (D_800E6280.step) {
    case 0x0:
        func_801345CC();
        break;
    case 0x1:
        func_801346F4();
        break;
    case 0x10:
        func_80134824();
        break;
    case 0x20:
        func_801348F4();
        break;
    case 0x30:
        func_801349FC();
        break;
    case 0x40:
        func_80134B3C();
        break;
    case 0x50:
        func_80134C28();
        break;
    case 0x60:
        func_80134D14();
        break;
    case 0x70:
        func_80134E00();
        break;
    }
}

void func_801345CC(void) {
    s32 i;
    s32 r;
    s32 pad[1]; /* FAKE: unused local above the arrays reproduces the original frame; real source unknown. T-9320 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"アイコン", 0);
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        r = set_kanji_string(a[i], b[i], 0xF, (&D_8013BF60)[i], 0); /* FAKE: string pointer table reached through its first symbol, as in func_80133C4C. T-9320 */
    }
    menu_set(0, 8, a, b, c, d);
    k_disp_start(r);
    func_8004284C();
}

void func_801346F4(void) {
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
            case 5:
                func_80042940(0x60);
                break;
            case 6:
                func_80042940(0x70);
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

s32 func_80134824(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF60, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　明るくアニメーションしているアイコン", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "が、現在選択できるアイコンです。", 0);
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

s32 func_801348F4(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF64, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　テストや修学旅行など、強制スケジュー", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ルを実行するときに使います。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　アイコンモード・フリーのときは、通常", 0);
        set_kanji_string(-0x88, 0, 0xF, "時でも画面表示されます。", 0);
        D_800E6280.substep += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_801349FC(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF68, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムを選択し、アイコンモードをフ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "リーにして下さい。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　Ｌ２ボタンを押してアイコンをつかみ、", 0);
        set_kanji_string(-0x88, 0, 0xF, "好きな場所に移動することが出来ます。", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "　アイコンの位置は、システムファイルを", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "作成した場合のみセーブされます。", 0);
        D_800E6280.substep += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80134B3C(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF6C, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムを選択し、カーソルの移動速度", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "を遅くしてから、アイコンを移動してくだ", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "さい。", 0);
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

s32 func_80134C28(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF70, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　マウスでは、アイコンを動かせません。", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "コントローラにつなぎ替えて、アイコンを", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "移動してから、御遊び下さい。", 0);
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

s32 func_80134D14(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF74, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムを選択し、アイコンモードを", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "　　ＴＹＰＥ‐ＡまたはＴＹＰＥ‐Ｂ", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "にしてください。", 0);
        D_800E6280.substep += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80134E00(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013BF78, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　万一メモリーカードからのデータのロー", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ド時に全てのアイコンが重なってしまった", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "場合、ゲームスタート画面のオプションで", 0);
        set_kanji_string(-0x88, 0, 0xF, "アイコンモードを変更してから、ゲームを", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "始めてください。", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "（オプションでの設定が優先されます）", 0);
        D_800E6280.substep += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}
