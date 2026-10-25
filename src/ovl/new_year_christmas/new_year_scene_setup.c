/* new_year_scene_setup.c: ; src/ovl/SHOUGATU/80132550.c:40-69 loads New Year resources and sets the scene title. Original object 0x80132550-0x80132D00 (config/objects/SHOUGATU.txt). */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24128). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24128). */
#include "common.h"
#include "ovl/new_year_christmas.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void new_year_scene_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_80143B28;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void new_year_audio_bank_load_start(void) {
    audio_bank_load_start(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    mode_step_next();
}

void new_year_audio_load_retry(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void new_year_scene_resources_load(void) {
    cd_read_sectors(0x34, 0x80197000, 0xAF48);
    new_year_resource_addresses_init();
    mode_step_next();
}

void new_year_scene_init(void) {
    s16 i;
    GameState *g;
    u8 *flags;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "正月");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    day_event_today_clear(0);
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    new_year_resource_addresses_init();
    talk_names_set(0U);
    new_year_dialogue_group = 0;
    new_year_dialogue_line = 0;
    new_year_shrine_invitation_flag = 0;
    new_year_best_rank_girl = 0xFF;
    new_year_fallback_dialogue_flag = 0;
    new_year_shiori_route_flag = 1;
    g = &game_state;
    flags = select_girl_menu_enabled;
    for (i = 0; i < 11; i++) {
        flags[i] = g->girls[i].flags.f.met >= 1;
    }
    mode_step_next();
}

void new_year_scene_background_load(void) {
    bg_read_sub2(0x4167);
    mode_step_next();
}

void new_year_eligible_girl_select(void) {
    s32 girl_index;
    u8 best_rank_girl;
    u8 best_girl_rank;
    u8 candidate_girl_rank;

    best_rank_girl = 0xFF;
    best_girl_rank = 4;
    talk_girl_selector_plus_one = 0;
    for (girl_index = 0; girl_index < 11; girl_index++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_girl_rank to $s5 (T-24122, T-24128). */
        if (1) {}
        if (game_state.girls[girl_index].flags.f.met != 0) {
            candidate_girl_rank = get_g_zyotai_s(girl_index) & 0x7F;
            /* FAKE: duplicate `candidate_girl_rank == best_girl_rank` shifts uopt'candidate_girl_rank live-range priority between candidate_girl_rank and best_girl_rank (T-24122). */
            if (candidate_girl_rank < best_girl_rank ||
                (candidate_girl_rank == best_girl_rank && candidate_girl_rank == best_girl_rank &&
                 get_h_tokimeki(girl_index) + get_h_yuukou(girl_index) >
                     get_h_tokimeki(best_rank_girl) + get_h_yuukou(best_rank_girl))) {
                best_rank_girl = girl_index;
                best_girl_rank = candidate_girl_rank;
            }
        }
    }
    candidate_girl_rank = get_g_zyotai_s(best_rank_girl) & 0x7F;
    if (best_rank_girl != 0xFF && candidate_girl_rank < 2U) {
        new_year_best_rank_girl = best_rank_girl;
        /* FAKE: `(best_rank_girl & 0) +` keeps cfe'candidate_girl_rank CVT J L narrowing so uopt emits `andi $a0, $fp, 0xff` (T-24122). */
        game_state.current_girl = (best_rank_girl & 0) + best_rank_girl;
        talk_names_set(game_state.current_girl);
        new_year_girl_dialogue_tables_select();
        new_year_dialogue_group = 0;
        new_year_dialogue_line = 0;
        new_year_candidate_selected = 1;
        talk_girl_selector_plus_one = best_rank_girl + 1;
        game_state.day_event_done_bits |= 0x80;
        mode_stage_set(1);
    } else {
        new_year_candidate_selected = 0;
        mode_stage_set(2);
    }
}

void new_year_girl_dialogue_tables_select(void) {
    u8 state;

    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        new_year_dialogue_text_table = D_80143A48;
        new_year_dialogue_command_table = D_80143A84;
        new_year_dialogue_voice_table = D_80143AC0;
        break;
    case GIRL_MIO:
        new_year_dialogue_text_table = D_80143A58;
        new_year_dialogue_command_table = D_80143A94;
        new_year_dialogue_voice_table = D_80143AD0;
        break;
    case GIRL_YUINA:
        new_year_dialogue_text_table = D_80143A4C;
        new_year_dialogue_command_table = D_80143A88;
        new_year_dialogue_voice_table = D_80143AC4;
        break;
    case GIRL_AYAKO:
        new_year_dialogue_text_table = D_80143A50;
        new_year_dialogue_command_table = D_80143A8C;
        new_year_dialogue_voice_table = D_80143AC8;
        break;
    case GIRL_SAKI:
        new_year_dialogue_text_table = D_80143A6C;
        new_year_dialogue_command_table = D_80143AA8;
        new_year_dialogue_voice_table = D_80143AE4;
        break;
    case GIRL_YUKARI:
        new_year_dialogue_text_table = D_80143A5C;
        new_year_dialogue_command_table = D_80143A98;
        new_year_dialogue_voice_table = D_80143AD4;
        break;
    case GIRL_NOZOMI:
        new_year_dialogue_text_table = D_80143A64;
        new_year_dialogue_command_table = D_80143AA0;
        new_year_dialogue_voice_table = D_80143ADC;
        state = get_g_zyotai_s(GIRL_NOZOMI) & 0x7F;
        if (state < 2) {
            new_year_dialogue_text_table = D_80143A60;
            new_year_dialogue_command_table = D_80143A9C;
            new_year_dialogue_voice_table = D_80143AD8;
        }
        break;
    case GIRL_MIRA:
        new_year_dialogue_text_table = D_80143A54;
        new_year_dialogue_command_table = D_80143A90;
        new_year_dialogue_voice_table = D_80143ACC;
        break;
    case GIRL_YUKO:
        new_year_dialogue_text_table = D_80143A44;
        new_year_dialogue_command_table = D_80143A80;
        new_year_dialogue_voice_table = D_80143ABC;
        break;
    case GIRL_MEGUMI:
        new_year_dialogue_text_table = D_80143A68;
        new_year_dialogue_command_table = D_80143AA4;
        new_year_dialogue_voice_table = D_80143AE0;
        break;
    case GIRL_YUMI:
        new_year_dialogue_text_table = D_80143A74;
        new_year_dialogue_command_table = D_80143AB0;
        new_year_dialogue_voice_table = D_80143AEC;
        break;
    }
}
