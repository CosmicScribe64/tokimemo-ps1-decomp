#include "common.h"
#include "ovl/OLH.h"

void func_80135AC0(void) {
    switch (game_state.step) {
    case 0x0:
        func_80135B8C();
        break;
    case 0x1:
        func_80135CB4();
        break;
    case 0x10:
        func_80135DD4();
        break;
    case 0x20:
        func_80135EC0();
        break;
    case 0x30:
        func_80136000();
        break;
    case 0x40:
        func_80136124();
        break;
    case 0x50:
        func_80136210();
        break;
    case 0x60:
        func_801362C4();
        break;
    case 0x70:
        func_801363CC();
        break;
    }
}

void func_80135B8C(void) {
    s32 i;
    s32 r;
    s32 pad[1]; /* FAKE: unused local above the arrays reproduces the original frame 0xC0; same as OLH func_801324EC. T-19005 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"ミニ知識", 0);
    for (i = 0; i < 7; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        r = set_kanji_string(a[i], b[i], 0xF, (s32)(&D_8013CEA0)[i], 0);
    }
    menu_set(0, 7, a, b, c, d);
    k_disp_start(r);
    main_step_next();
}

void func_80135CB4(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_0 != -1) {
            snd_command(0x501);
            /* FAKE: lbu view of the s8 field (the compare above is lb); original declaration unknown. T-9180 */
            switch (*(u8 *)&game_state.menu_cursor_0) {
            case 0:
                main_step_set(0x10);
                break;
            case 1:
                main_step_set(0x20);
                break;
            case 2:
                main_step_set(0x30);
                break;
            case 3:
                main_step_set(0x40);
                break;
            case 4:
                main_step_set(0x60);
                break;
            case 5:
                main_step_set(0x70);
                break;
            case 6:
                main_stage_set(0);
                break;
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_stage_set(0);
    }
}

s32 func_80135DD4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEA0, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　Ｌ１＋Ｒ１＋セレクト＋スタートボタン", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "を少しの間押し続けることにより、ソフト", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "ウェアリセットがかけられます。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_80135EC0(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEA4, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　２Ｐコントローラの", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "　　○＋×＋□＋△", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "ボタンを同時に押すと、１Ｐコントローラ", 0);
        set_kanji_string(-0x88, 0, 0xF, "の○ボタンが連射される状態になります。", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "　連射の解除は、１Ｐコントローラの任意", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "のボタンを押すことによってできます。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_80136000(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEA8, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　２Ｐコントローラのスタートボタンを押", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "すと、通常コマンド実行のＳＤキャラの、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "アニメスピードが少し速くなります。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　もう一度押すと、通常のスピードに戻り", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "ます。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_80136124(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEAC, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　デートなどで、場所を知りたいときは１", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "Ｐコントローラの△ボタンで、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "場所ウィンドウを呼び出すことができます。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_80136210(void) {
    switch (game_state.substep) {                           /* irregular */
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEB0, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　歌詞が出ないときは高解像モードです。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_801362C4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEB0, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　普通の漢和辞典のように、部首別に漢字", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "がならんでいます。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　また、一部の漢字は訓読みでも登録され", 0);
        set_kanji_string(-0x88, 0, 0xF, "ています。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}

s32 func_801363CC(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013CEB4, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　極まれに、サウンドの転送に失敗するこ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "とがありますが、一旦データをセーブし、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "ゲームを再立ち上げすれば、正常に復帰し", 0);
        set_kanji_string(-0x88, 0, 0xF, "ます。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        main_step_set(0);
        return;
    }
}
