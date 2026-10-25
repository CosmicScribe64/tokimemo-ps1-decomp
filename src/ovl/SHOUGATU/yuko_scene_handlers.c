/* yuko_scene_handlers.c: ; src/ovl/SHOUGATU/80137BB0.c:6-19 selects Yuko scene step tables; main loader routes the meeting mode here. Original object 0x80137BB0-0x80137FF0 (config/objects/SHOUGATU.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* T-9570: old-style caller preserves the full argument register. */
void portrait_layout_set();
#include "common.h"
#include "ovl/SHOUGATU.h"

void new_year_yuko_scene_callback_group_dispatch(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        new_year_yuko_callback_common_dispatch();
        return;
    case 1:
        new_year_yuko_callback_common_dispatch();
        return;
    case 2:
        new_year_yuko_callback_common_dispatch();
        return;
    default:
        new_year_yuko_callback_default_dispatch();
        return;
    }
}

void new_year_yuko_callback_common_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80144E20;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuko_audio_callback_500(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x500);
    mode_step_next();
}

void new_year_yuko_audio_callback_507(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x507);
    mode_step_next();
}

s32 new_year_yuko_scene_update(void) {
    new_year_meeting_talk_run();
    if (game_state.mode_step_timer.w++ == 1) {
        if (D_80144E0C == 3) {
            portrait_layout_set(3);
        }
    }
}

void new_year_yuko_callback_default_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl34 tbl;

    tbl = D_80144E80;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuko_portrait_layout_update(void) {
    new_year_meeting_talk_run();
    if ((D_80144E0C == 3) || (D_80144E0C == 11)) {
        /* FAKE: dead selector test retains the target promotion. T-9570 */
        if ((D_80144E0C && D_80144E0C) != 0) { }
        portrait_layout_set(3);
    } else if ((D_80144E0C == 5) || (D_80144E0C == 13)) {
        portrait_layout_set(get_g_zyotai_h(GIRL_YUKO));
    }
    /* FAKE: zero increment promotes the selector into v1. T-9570 */
    D_80144E0C += 0;
}

void new_year_yuko_scene_state_init(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    *(u8 *)&game_state.date_girl = *(u8 *)&game_state.current_girl = 8;
    talk_names_set(*(u8 *)&game_state.current_girl);
    if ((u8) D_80144E14 < 3U) {
        strcpy(name_code_girl_short, "女生徒");
    }
    new_year_meeting_dialogue_addresses_init();
    D_80144DFC = D_80144D90;
    D_80144E00 = D_80144DB4;
    D_80144E04 = D_80144DD8;
    mode_step_next();
}

void new_year_yuko_hallway_title_select(void) {
    D_80144E08 = D_80144E14 * 3;
    D_80144E0C = 0;
    strcpy(scene_place_title, "廊下");
    mode_step_next();
}

void new_year_yuko_background_read(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}
