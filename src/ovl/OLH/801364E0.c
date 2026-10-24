#include "common.h"
#include "ovl/OLH.h"

void func_801364E0(void) {
    switch (game_state.step) {
    case 0x0:
        func_801365AC();
        break;
    case 0x1:
        func_801366D4();
        break;
    case 0x10:
        func_80136804();
        break;
    case 0x20:
        func_80136960();
        break;
    case 0x30:
        func_80136A30();
        break;
    case 0x40:
        func_80136B00();
        break;
    case 0x50:
        func_80136BD0();
        break;
    case 0x60:
        func_80136CF4();
        break;
    case 0x70:
        func_80136E18();
        break;
    }
}

void func_801365AC(void) {
    s32 i;
    s32 r;
    s32 pad[1]; /* FAKE: unused local above the arrays reproduces the original frame; same as func_80133C4C. T-9350 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"ファイルセレクト画面", 0);
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        r = set_kanji_string(a[i], b[i], 0xF, (&D_8013D6C0)[i], 0);
    }
    menu_set(0, 8, a, b, c, d);
    k_disp_start(r);
    main_step_next();
}

void func_801366D4(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_0 != -1) {
            snd_command(0x501);
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
                main_step_set(0x50);
                break;
            case 5:
                main_step_set(0x60);
                break;
            case 6:
                main_step_set(0x70);
                break;
            case 7:
                main_stage_set(0);
                break;
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_stage_set(0);
    }
}

s32 func_80136804(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6C0, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムと表示されている部分をクリッ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "クすると、システムの作成やシステムの作", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "り直しが出来ます。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　データの表示がおかしい場合や、他のメ", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "モリーカードからデータをコピーした場合", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "は、システムの作り直しを行なってくださ", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "い。", 0);
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

s32 func_80136960(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6C4, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　同じメモリーカード内で、データをコピ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ーすることができます。", 0);
        game_state.substep += 1;
        break;
    case 1:
        text_wait_confirm();
        break;
    case 2:
        main_step_set(0);
        break;
    }
}

s32 func_80136A30(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6C8, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　クリア人数そのままで、ゲームをプロロ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ーグからやり直すことが出来ます。", 0);
        game_state.substep += 1;
        break;
    case 1:
        text_wait_confirm();
        break;
    case 2:
        main_step_set(0);
        break;
    }
}

s32 func_80136B00(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6CC, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　ときめきメモリアルのデータを消去する", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ことが出来ます。", 0);
        game_state.substep += 1;
        break;
    case 1:
        text_wait_confirm();
        break;
    case 2:
        main_step_set(0);
        break;
    }
}

s32 func_80136BD0(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6D0, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムファイルがある場合は、全ての", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "アルバムのデータの合計クリア人数が表示", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "されます。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　システムファイルがない場合は、選んだ", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "アルバムのクリア人数が表示されます。", 0);
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

s32 func_80136CF4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6D4, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "白抜き数字で番号が表示されているのが、", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "現在選択されているアルバムです。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "白抜き数字で番号が表示されているアルバ", 0);
        set_kanji_string(-0x88, 0, 0xF, "ムをもう一度クリックすると、そのアルバ", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "ムでゲームが再開されます。", 0);
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

s32 func_80136E18(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013D6D8, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　△ボタンを押すと、そのアルバムのパラ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "メータや、女の子の評価、クリア回数が表", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "示されます。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　青くパラメータと表示されている部分を", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "クリックしても、表示を変えることができ", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "ます。", 0);
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
