/* yukari_scene_handlers.c: ; src/ovl/SHOUGATU/8013A190.c:11-16 dispatches Yukari scene setup callbacks. Original object 0x8013A190-0x8013A500 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

/* Bit 2 of the girl record flag byte. T-9720. */
typedef struct ShougatuFlag4 {
    /* 0x00 */ u8 pad : 2;
    /* 0x00 */ u8 set : 1;
    /* 0x00 */ u8 rest : 5;
} ShougatuFlag4; /* size 0x01 */

void new_year_yukari_scene_group_dispatch(void) {
    if (new_year_meeting_choice == 0) {
        new_year_yukari_scene_primary_mode_step_dispatch();
        return;
    }
    new_year_yukari_scene_secondary_mode_step_dispatch();
}

void new_year_yukari_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_801455E0;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yukari_scene_audio_command_0502(void) {
    audio_command(0x502);
    mode_step_next();
}

void new_year_yukari_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145648;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yukari_scene_state_init(void) {
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
    ((ShougatuFlag4 *) (&game_state.girls[5].flags.b[0]))->set = 1;
    /* FAKE: the dead triple test preserves the original argument/store scheduling.
     * IDO removes all of its reads and tests. T-9720. */
    if ((!game_mode_scene_girl) && (!game_mode_scene_girl) && (!game_mode_scene_girl)) {}
    talk_names_set(*((u8 *) (&game_state.current_girl)) = 5);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_dialogue_text_table = D_80144DA4;
    new_year_meeting_dialogue_command_table = D_80144DC8;
    new_year_meeting_dialogue_voice_table = D_80144DEC;
    mode_step_next();
}

void new_year_yukari_scene_ground_state_init(void) {
    new_year_meeting_dialogue_group = 0;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "グランド");
    mode_step_next();
}

void new_year_yukari_scene_ground_state_init_alt(void) {
    new_year_meeting_dialogue_group = 5;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "グランド");
    mode_step_next();
}

void new_year_yukari_scene_background_load_4097(void) {
    bg_read_sub2(0x4097);
    mode_step_next();
}

void new_year_yukari_scene_portrait_variant_load(void) {
    if (game_state.current_girl == GIRL_YUKARI) {
        portrait_read_start(0x5FE4);
    } else {
        portrait_read_start(0x552B);
    }
    mode_step_next();
}
