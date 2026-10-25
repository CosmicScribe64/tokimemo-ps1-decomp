/* girl_dialogue_setup.c: ; src/ovl/SHOUGATU/8013B5B0.c:4-10,40-79 dispatches setup steps, initializes talk and scene state, and tests met-girl flags. Original object 0x8013B5B0-0x8013BA00 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_girl_dialogue_setup_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80145A14;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void new_year_girl_dialogue_audio_bank_load_start(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x202);
        mode_step_next();
    }
    mode_step_next();
}

void new_year_girl_dialogue_audio_bank_wait(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x202);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void new_year_hallway_resource_read(void) {
    cd_read_sectors(9, 0x80197000, 0xAFB3);
    new_year_girl_talk_resource_addresses_init();
    mode_step_next();
}

void new_year_hallway_scene_state_init(void) {
    s16 i;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "廊下");
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9320 */
    talk_names_set(*(u8 *)&game_state.current_girl = 0xC);
    new_year_girl_talk_resource_addresses_init();
    new_year_hallway_dialogue_text_table = D_8014596C;
    new_year_hallway_dialogue_command_table = D_801459A0;
    new_year_hallway_dialogue_voice_table = D_801459D4;
    new_year_hallway_dialogue_group = 0;
    new_year_hallway_dialogue_line = 0;
    new_year_any_girl_flag_bit8 = 0;
    for (i = 0; i < 0xB; i++) {
        if (game_state.girls[i].flags.b[1] & 1) {
            new_year_any_girl_flag_bit8 = 1;
        }
    }
    mode_step_next();
}

void new_year_hallway_background_and_log(void) {
    event_log_add(7, 0);
    bg_read_sub2(0x4045);
    mode_step_next();
}

void new_year_hallway_year_counter_update(void) {
    new_year_hallway_dialogue_group += ((u8) game_state.year >= 0x61U) * 4;
    mode_step_next();
}

void new_year_hallway_no_flag_counter_update(void) {
    if (new_year_any_girl_flag_bit8 == 0) {
        new_year_hallway_dialogue_group += 1;
    }
    mode_step_next();
}

void new_year_hallway_dialogue_branch(void) {
    if (new_year_any_girl_flag_bit8 != 0) {
        mode_step_next();
        return;
    }
    new_year_girl_dialogue_run();
}

void new_year_hallway_scene_exit_branch(void) {
    if (new_year_any_girl_flag_bit8 != 0) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}
