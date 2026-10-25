/* christmas_mansion_entry.c: ; src/ovl/SHOUGATU/8013D660.c:101-149 handles Dec 24 birthday detours before entering the Ijuin mansion scene. Original object 0x8013D660-0x8013DD90 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_ijuin_entry_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80145F40;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void new_year_mansion_audio_bank_load_start(void) {
    audio_bank_load_start(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void new_year_mansion_audio_bank_wait(void) {
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

void new_year_mansion_background_resource_read(void) {
    cd_read_sectors(0xE, 0x80197000, 0xAFBC);
    new_year_scene_callback_addresses_init();
    mode_step_next();
}

void new_year_mansion_timeout_init(void) {
    D_80122CFC = 0x1E;
    mode_step_next();
}

void new_year_mansion_story_threshold_route(void) {
    s16 thresh = game_state.girls[12].flags.f.story_stage * 25 + 150;

    if (game_state.param_athletics.value >= thresh &&
        game_state.girls[14].flags.f.story_stage != 0) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        return;
    }
    if (game_state.param_looks.value >= (s16)(game_state.girls[12].flags.f.story_stage * 30 - (game_state.girls[12].flags.f.story_stage >= 2) * 10 + 70)) {
        mode_step_next();
        return;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void new_year_mansion_story_progress_update(void) {
    s16 v;
    /* FAKE: GameState *g splits the second unk_1BC[14].unk_0C access from the first so uopt keeps it in $t2 instead of promoting to $v0. T-24130 */
    GameState *g = &game_state;

    v = game_state.girls[12].flags.f.story_stage * 25 + 150;
    if (game_state.param_athletics.value >= v) {
        christmas_selection_dialogue_group = (game_state.girls[14].flags.f.story_stage >= 1) * 3 + 4;
        g->girls[14].flags.f.story_stage += 1;
        mode_step_next();
        return;
    }
    christmas_selection_dialogue_group += 1;
    game_state.param_guts.value += 10;
    game_state.param_stress.value += 10;
    check_para_limit();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void new_year_girl14_story_flag_talk_gate(void) {
    if (((u32) (game_state.girls[14].flags.w << 0x14) >> 0x1D) == 1) {
        new_year_current_talk_start();
        return;
    }
    mode_step_next();
    mode_step_next();
}

s32 new_year_ijuin_birthday_dispatch(void) {
    GameState *g;

    if (game_state.girls[0].birthday.date.month == 12 &&
        game_state.girls[0].birthday.date.day == 24 &&
        !(*(u8 *)&game_state.day_event_done_bits & 0x20)) {
        *(u8 *)&game_state.day_event_done_bits |= 0x20;
        game_state.day_events[game_state.day_event_index].girl = 0;
        game_mode_set(GAME_MODE_GIRL_BIRTHDAY);
        return 0;
    }
    if (((WordView *)&game_state.player_birthday)->date.month == 12 &&
        ((WordView *)&game_state.player_birthday)->date.day == 24 &&
        !(*(u8 *)&game_state.day_event_done_bits & 0x40)) {
        *(u8 *)&game_state.day_event_done_bits |= 0x40;
        game_mode_set(GAME_MODE_PLAYER_BIRTHDAY);
        return 0;
    }
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    strcpy(scene_place_title, "伊集院邸");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    christmas_selection_dialogue_group = 0;
    christmas_selection_dialogue_line = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    g = &game_state;
    strcpy(name_code_family, g->player_family_name);
    if (g->girls[12].flags.f.story_stage >= 3) {
        g->girls[12].flags.f.story_stage = 2;
    }
    if (g->girls[12].flags.f.story_stage == 0) {
        strcpy(name_code_girl_short, "黒服");
    } else {
        strcpy(name_code_girl_short, "外井");
    }
    game_state.current_girl = GIRL_REI;
    new_year_scene_callback_addresses_init();
    christmas_selection_dialogue_text_table = D_80145AFC;
    christmas_selection_dialogue_command_table = D_80145B30;
    christmas_selection_dialogue_voice_table = D_80145B64;
    mode_step_next();
}

void new_year_mansion_background_read(void) {
    bg_read_sub2(0x41CA);
    mode_step_next();
}

void new_year_mansion_portrait_read(void) {
    portrait_read_start(0x6B24);
    mode_step_next();
}

void new_year_mansion_mode_advance(void) {
    mode_stage_next();
}

void new_year_mansion_mode_select_4(void) {
    mode_stage_set(4);
}
