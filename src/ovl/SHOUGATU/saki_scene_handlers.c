/* saki_scene_handlers.c: ; src/ovl/SHOUGATU/8013A950.c:4-15 dispatches Saki scene setup callbacks. Original object 0x8013A950-0x8013AF10 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void new_year_saki_scene_callback_group_dispatch(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        new_year_saki_callback_group_0_dispatch();
        return;
    case 1:
        new_year_saki_callback_group_1_dispatch();
        return;
    default:
        new_year_saki_callback_group_default_dispatch();
        return;
    }
}

void new_year_saki_callback_group_0_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80145760;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_saki_callback_group_1_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145808;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_saki_callback_group_default_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145858;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_saki_portrait_read(void) {
    portrait_read_start(0x66EC);
    mode_step_next();
}

void new_year_saki_scene_state_init(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9120 */
    talk_names_set(*(u8 *)&game_state.current_girl = 4);
    new_year_meeting_dialogue_addresses_init();
    D_80144DFC = D_80144DAC;
    D_80144E00 = D_80144DD0;
    D_80144E04 = D_80144DF4;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    mode_step_next();
}

void new_year_saki_classroom_scene_select(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(scene_place_title, "教室");
    mode_step_next();
}

void new_year_saki_ground_scene_select_0x0c(void) {
    D_80144E08 = 0xC;
    D_80144E0C = 0;
    strcpy(scene_place_title, "グランド");
    mode_step_next();
}

void new_year_saki_ground_scene_select_0x0f(void) {
    D_80144E08 = 0xF;
    D_80144E0C = 0;
    strcpy(scene_place_title, "グランド");
    mode_step_next();
}

void new_year_saki_ground_background_select(void) {
    if (((u8) game_state.month >= 6U) && ((u8) game_state.month < 0xAU)) {
        bg_read_sub2(0x404D);
    } else {
        bg_read_sub2(0x4055);
    }
    mode_step_next();
}

void new_year_saki_background_read_4097(void) {
    bg_read_sub2(0x4097);
    mode_step_next();
}

void new_year_saki_conditional_selector_6(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) == 7) {
        D_80144E08 = 6;
    }
    mode_step_next();
}

void new_year_saki_conditional_selector_8_or_10(void) {
    u8 raw;
    s32 nibble;
    raw = game_state.girls[4].flags.b[2];
    if (raw) {} /* FAKE: dead raw-value test shapes allocation, T-9540 */
    nibble = raw >> 4;
    if (nibble == (game_state.player_flags.h >> 12)) {
        if (nibble == 7) {
            D_80144E08 = 10;
        } else {
            D_80144E08 = 8;
        }
    }
    mode_step_next();
}

void new_year_saki_conditional_selector_4(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) != ((u32) game_state.player_flags.h >> 0xC)) {
        D_80144E08 = 4;
    }
    mode_step_next();
}

void new_year_saki_flag_match_step_route(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) == ((u32) game_state.player_flags.h >> 0xC)) {
        game_state.mode_step += 0xD;
        return;
    }
    mode_step_next();
}
