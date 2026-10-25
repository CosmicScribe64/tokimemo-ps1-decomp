/* ayako_scene_handlers.c: ; src/ovl/SHOUGATU/80138B40.c:4-18 dispatches Ayako scene step tables. Original object 0x80138B40-0x801390C0 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void new_year_ayako_scene_group_dispatch(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        new_year_ayako_scene_primary_mode_step_dispatch();
        return;
    case 1:
        new_year_ayako_scene_secondary_mode_step_dispatch();
        return;
    default:
        new_year_ayako_scene_default_mode_step_dispatch();
        return;
    }
}

void new_year_ayako_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80145170;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_ayako_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801451C8;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_ayako_scene_default_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145218;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_ayako_scene_state_init(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-10520 */
    talk_names_set(*(u8 *)&game_state.current_girl = 3);
    new_year_meeting_dialogue_addresses_init();
    D_80144DFC = D_80144D98;
    D_80144E00 = D_80144DBC;
    D_80144E04 = D_80144DE0;
    mode_step_next();
}

void new_year_ayako_scene_courtyard_state_init(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(scene_place_title, "中庭");
    mode_step_next();
}

void new_year_ayako_scene_art_room_state_init(void) {
    ((Bits64B8 *)&game_state.girls[game_state.current_girl].flags.b[0])->f = 1;
    D_80144E08 = 8;
    D_80144E0C = 0;
    strcpy(scene_place_title, "美術室");
    mode_step_next();
}

void new_year_ayako_scene_music_room_state_init(void) {
    ((Bits64B8 *)&game_state.girls[game_state.current_girl].flags.b[0])->f = 1;
    D_80144E08 = 0xB;
    D_80144E0C = 0;
    strcpy(scene_place_title, "音楽室");
    mode_step_next();
}

void new_year_ayako_scene_background_load_4071(void) {
    bg_read_sub2(0x4071);
    mode_step_next();
}

void new_year_ayako_scene_background_load_40e2(void) {
    bg_read_sub2(0x40E2);
    mode_step_next();
}

void new_year_ayako_scene_background_load_40d9(void) {
    bg_read_sub2(0x40D9);
    mode_step_next();
}

void new_year_ayako_scene_flag_state_branch(void) {
    s32 isA; /* high nibble of GameState.player_flags is 4 */
    s32 isB; /* high nibble of the GirlRecord flag byte +2 of record 3 is 4 */

    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    if ((game_state.player_flags.h >> 0xC) == 4) {
        isA = 1;
    } else {
        isA = 0;
    }
    if ((game_state.girls[3].flags.b[2] >> 4) == 4) {
        isB = 1;
    } else {
        isB = 0;
    }
    if ((isA == 0) || (isB == 0)) {
        if (isB != 0) {
            D_80144E08 += 2;
        } else {
            D_80144E08 += 4;
            game_state.girls[game_state.current_girl].flags.f.b2 = 0;
        }
    }
    mode_step_next();
}
