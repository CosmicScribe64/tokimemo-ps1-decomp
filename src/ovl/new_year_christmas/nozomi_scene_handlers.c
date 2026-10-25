/* nozomi_scene_handlers.c: ; src/ovl/SHOUGATU/8013A500.c:4-12 dispatches Nozomi scene setup callbacks. Original object 0x8013A500-0x8013A950 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_nozomi_scene_group_dispatch(void) {
    if (new_year_meeting_choice == 0) {
        new_year_nozomi_scene_primary_mode_step_dispatch();
        return;
    }
    new_year_nozomi_scene_secondary_mode_step_dispatch();
}

void new_year_nozomi_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl27 tbl;

    tbl = D_801456A0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void new_year_nozomi_scene_audio_command_0502(void) {
    audio_command(0x502);
    mode_step_next();
}

void new_year_nozomi_scene_audio_command_0503(void) {
    audio_command(0x503);
    mode_step_next();
}

void new_year_nozomi_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_8014570C;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_nozomi_scene_state_init(void) {
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
    D_80122CE8 = 0;
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    *(u8 *)&game_state.current_girl = 6;
    talk_names_set(*(u8 *)&game_state.current_girl);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_dialogue_text_table = D_80144DA8;
    new_year_meeting_dialogue_command_table = D_80144DCC;
    new_year_meeting_dialogue_voice_table = D_80144DF0;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    mode_step_next();
}

void new_year_nozomi_scene_park_state_init(void) {
    new_year_meeting_dialogue_group = 0;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "近所の公園");
    mode_step_next();
}

void new_year_nozomi_scene_pool_state_init(void) {
    new_year_meeting_dialogue_group = 6;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "プール");
    mode_step_next();
}

void new_year_nozomi_scene_background_load_42d8(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}

void new_year_nozomi_scene_background_load_40a0(void) {
    bg_read_sub2(0x40A0);
    mode_step_next();
}

void new_year_nozomi_scene_portrait_load_6449(void) {
    portrait_read_start(0x6449);
    mode_step_next();
}

void new_year_nozomi_scene_portrait_load_625a(void) {
    portrait_read_start(0x625A);
    mode_step_next();
}

void new_year_nozomi_scene_player_flag_route(void) {
    if (((u32) game_state.player_flags.h >> 0xC) == 9) {
        new_year_meeting_dialogue_group = 5;
    }
    mode_step_next();
}

void new_year_nozomi_scene_selector_set_three(void) {
    new_year_meeting_dialogue_group = 3;
    mode_step_next();
}
