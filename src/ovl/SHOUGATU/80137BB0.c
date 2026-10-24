#define MAIN_API_OVERRIDE_bustup_layout_set /* T-9570: old-style caller preserves the full argument register. */
void bustup_layout_set();
#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80137BB0(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80137C28();
        return;
    case 1:
        func_80137C28();
        return;
    case 2:
        func_80137C28();
        return;
    default:
        func_80137D68();
        return;
    }
}

void func_80137C28(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80144E20;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80137C9C(void) {
    snd_command(0x23);
    snd_command(0x24);
    snd_command(0x500);
    main_step_next();
}

void func_80137CD4(void) {
    snd_command(0x23);
    snd_command(0x24);
    snd_command(0x507);
    main_step_next();
}

s32 func_80137D0C(void) {
    func_80137AB4();
    if (game_state.step_timer.w++ == 1) {
        if (D_80144E0C == 3) {
            bustup_layout_set(3);
        }
    }
}

void func_80137D68(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl34 tbl;

    tbl = D_80144E80;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80137DE4(void) {
    func_80137AB4();
    if ((D_80144E0C == 3) || (D_80144E0C == 11)) {
        /* FAKE: dead selector test retains the target promotion. T-9570 */
        if ((D_80144E0C && D_80144E0C) != 0) { }
        bustup_layout_set(3);
    } else if ((D_80144E0C == 5) || (D_80144E0C == 13)) {
        bustup_layout_set(get_g_zyotai_h(GIRL_YUKO));
    }
    /* FAKE: zero increment promotes the selector into v1. T-9570 */
    D_80144E0C += 0;
}

void func_80137E54(void) {
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    dialogue_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    *(u8 *)&game_state.date_girl = *(u8 *)&game_state.current_girl = 8;
    dialogue_names_set(*(u8 *)&game_state.current_girl);
    if ((u8) D_80144E14 < 3U) {
        strcpy(name_buf_girl_short, "女生徒");
    }
    func_80136F90();
    D_80144DFC = D_80144D90;
    D_80144E00 = D_80144DB4;
    D_80144E04 = D_80144DD8;
    main_step_next();
}

void func_80137F70(void) {
    D_80144E08 = D_80144E14 * 3;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "廊下");
    main_step_next();
}

void func_80137FC4(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}
