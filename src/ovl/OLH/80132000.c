#include "common.h"
#include "ovl/OLH.h"

void func_80132000(void) {
    switch (game_state.mode_stage) {
    case 0x0:
        func_80132114();
        break;
    case 0x1:
        func_80132294();
        break;
    case 0x2:
        func_801323AC();
        break;
    case 0x10:
        func_80132420();
        break;
    case 0x20:
        func_80132FA0();
        break;
    case 0x30:
        func_801364E0();
        break;
    case 0x40:
        func_80133B80();
        break;
    case 0x50:
        func_80134500();
        break;
    case 0x60:
        func_80134F40();
        break;
    case 0x70:
        func_80135AC0();
        break;
    }
    cursor_update();
    if (game_state.pad_pressed & PAD_START) {
        game_mode_set(MODE_COMMAND_SCREEN);
    }
    func_801323CC();
    message_window_title_update();
}

void func_80132114(void) {
    s32 i;
    s32 r;
    u8 col; /* FAKE: the chain below through a local keeps 0xA0 in one register and gives the original's third word above the arrays; real source unknown. T-19005 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    text_system_reset();
    actor_clear_range(0);
    back_clear_switch(1);
    col = (&game_state.clear_color_r)[0] = (&game_state.clear_color_r)[1] = (&game_state.clear_color_r)[2] = 0xA0;
    message_window_set_title("Ｑ＆Ａコーナー");
    set_kanji_string(-0x68, 0x5C, 2, (s32)"スタートボタンでゲームに戻れます", 0);
    k_sub_reset_point_set();
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x3C;
        c[i] = 0x80;
        d[i] = 0x10;
        set_kanji_string(a[i], b[i], 0xF, (&D_80139EE0)[i], 0);
    }
    menu_set(0, 8, a, b, c, d);
    r = set_kanji_string(-0x10, -0x50, 0xF, (s32)"項目を選んでください", 0);
    k_disp_start(r);
    mode_stage_next();
}

void func_80132294(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if ((game_state.pad_pressed & PAD_CIRCLE) && game_state.menu_cursor_0 != -1) {
        audio_command(0x501);
        switch (*(u8 *)&game_state.menu_cursor_0) { /* FAKE: u8 view keeps the selector load as lbu, as in the original (T-5010) */
        case 0:
            mode_stage_set(0x10);
            break;
        case 1:
            mode_stage_set(0x20);
            break;
        case 2:
            mode_stage_set(0x30);
            break;
        case 3:
            mode_stage_set(0x40);
            break;
        case 4:
            mode_stage_set(0x50);
            break;
        case 5:
            mode_stage_set(0x60);
            break;
        case 6:
            mode_stage_set(0x70);
            break;
        case 7:
            mode_stage_set(2);
            break;
        }
    }
}

void func_801323AC(void) {
    game_mode_set(MODE_COMMAND_SCREEN);
}

void func_801323CC(void) {
    prim_gradient_rect(-0x90, -0x58, 0x120, 0xC4, 4, 0xA0A0A0, 2);
    dtd_on(4);
}
