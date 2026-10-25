/* player_birthday.c: src/ovl/GEKO/80134040.c:4,133,219; birthday mode entry, repeat guard, best-met-girl visitor selection and script/display flow. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80134040-0x80134C90 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void encounter_player_birthday_mode_handler(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        encounter_player_birthday_dispatch();
    } else {
        game_mode_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    if (D_80122D38 == 0) {
        hud_screen_frame_draw(2);
    }
    message_window_show();
    message_window_title_update();
    hizuke_show();
    if (D_80122D38 == 0) {
        func_80083A10();
    }
    if (D_80122D38 != 0) {
        hud_screen_frame_draw(1);
        dec_bg_draw(0);
        parameter_show();
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        if (D_80122D04 != 0) {
            D_8011ECD3 &= 0x7F;
        }
        if (D_8011ECD3 & 0x80) {
            hud_cursor_update();
        }
    }
}

typedef struct {
    void (*callbacks[29])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/80134040.c:40; func_801341A0 at line43 copies this exact29-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterPlayerBirthdaySteps; /* size 0x74 */
extern EncounterPlayerBirthdaySteps encounter_player_birthday_steps;

void encounter_player_birthday_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterPlayerBirthdaySteps mode_step_callbacks;

    mode_step_callbacks = encounter_player_birthday_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

s32 encounter_player_birthday_finish(void) {
    u8 restore_substep;

    if (game_state.day_event_done_bits & 0x40) {
        audio_command(0xCF);
        game_mode_set(GAME_MODE_NEW_YEAR_A);
        return 0;
    }
    /* FAKE: read through a u8 view so the base and the result share $v0 (T-24035). */
    restore_substep = *(u8 *)&game_state.substep;
    if (restore_substep == 0) {
        hud_day_bgm_load();
        restore_substep = game_state.substep + 1;
    }
    game_state.substep = restore_substep;
    if (restore_substep == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            /* FAKE: the dead test gives the byte a second use, so uopt allocates $v0 for the load (T-24035). */
            if ((s32)game_state.substep) {}
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    return day_event_next(1);
}

s32 encounter_player_birthday_audio_load(void) {
    if (game_state.month_calendar[0].marker[game_state.day] == 5) {
        if (game_state.mode_step_timer.w++ == 0) {
            audio_command(0xBF);
        }
        if ((u32)game_state.mode_step_timer.w < 0x80U) {
            return 0;
        }
    }
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_player_birthday_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_player_birthday_audio_start(void) {
    k_reset(1);
    audio_command(0x200);
    mode_step_next();
}

void encounter_player_birthday_scripts_load_with_chime(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    encounter_player_birthday_scripts_bind();
    if (!(game_state.day_event_done_bits & 8)) {
        audio_command(0x603);
        set_kanji_string(-0x28, 0x40, 2, "ピンポーン", 0);
    }
    mode_step_next();
}

void encounter_player_birthday_scripts_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    encounter_player_birthday_scripts_bind();
    mode_step_next();
}

s32 encounter_player_birthday_init(void) {
    s16 i;
    u8 saved_talk_byte;

    if (*(u8 *)&game_state.day_event_done_bits & 4) {
        encounter_player_birthday_finish();
        return 0;
    }
    *(u8 *)&game_state.day_event_done_bits |= 4;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "玄関");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    saved_talk_byte = D_80122D40;
    talk_clear();
    D_80122D40 = saved_talk_byte;
    D_80122D38 = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    hud_icon_bar_init();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    text_marker_hide();
    encounter_player_birthday_group = 0;
    encounter_player_birthday_line = 0;
    mode_step_next();
}

void encounter_player_birthday_closing_init(void) {
    s32 fake_stack_pad; /* FAKE: unused slot above saved_talk_byte, the original frame has it (real source unknown). T-8070 */
    u8 saved_talk_byte;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    saved_talk_byte = D_80122D40;
    talk_clear();
    D_80122D40 = saved_talk_byte;
    strcpy(D_800CA19C, "玄関");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_read_sub2(0x4144);
    event_log_add(0xD, 0x25);
    mode_step_next();
}

void encounter_player_birthday_talk(void) {
    talk_tables = &encounter_player_birthday_group;
    D_800CA138 = &encounter_player_birthday_line;
    D_800CA13C = encounter_player_birthday_text_table;
    D_800CA140 = encounter_player_birthday_command_table;
    D_800CA144 = encounter_player_birthday_voice_table;
    talk_run(D_80122CDC, 1, 0);
}

/* FAKE: T-24503, the two equality wrappers prevent a shared compare copy;
 * the unsigned selector view reverses the branch operands. The empty loop-tail
 * test keeps the s16 conversion in the latch, and the call-result mask spends
 * the deleted narrowing temporary needed by the following ugen registers.
 * cur and newState keep the record base from being hoisted out of the loop. */
s32 encounter_player_birthday_girl_select(void) {
    s16 i;
    s32 next_girl_index;
    s16 best_girl;
    u32 incumbent_girl;
    u32 best_girl_state;
    u32 girl_state_result;
    u32 candidate_girl_state;
    u8 affection;
    u8 tokimeki;
    u8 best_tokimeki;
    u8 selected_girl;

    best_girl = 0xFF;
    best_girl_state = 4;
    i = 0;
    do {
        if (!game_state.girls[i].flags.f.met) {
            next_girl_index = i + 1;
        } else if (!(game_state.day_event_done_bits & 0x80) || (next_girl_index = i + 1, next_girl_index != (*(u32 *)&D_80122D40))) {
            next_girl_index = i + 1;
            girl_state_result = get_g_zyotai_s(i) & 0x7F;
            candidate_girl_state = girl_state_result & 0xFF;
            if ((u8) girl_state_result < best_girl_state ||
                (best_girl_state * 1 == (u8) girl_state_result && (incumbent_girl = best_girl, best_girl_state << 0 == (u8) girl_state_result) &&
                 (affection = get_h_yuukou(i), tokimeki = get_h_tokimeki(i),
                  best_tokimeki = get_h_tokimeki(incumbent_girl),
                  get_h_yuukou(incumbent_girl) + best_tokimeki < tokimeki + affection))) {
                best_girl = (u8) i;
                best_girl_state = (u8) candidate_girl_state;
            }
        }
        i = (s16) next_girl_index; if (i) {}
    } while (i < 11);
    girl_state_result = get_g_zyotai_s(best_girl) & 0xFF;
    D_80122D40 = 0;
    if ((girl_state_result & 0x7F) >= 2 || best_girl == 0xFF || (selected_girl = best_girl, game_state.day_event_done_bits & 8)) {
        mode_step_set(0x17);
        return 0;
    }
    game_state.current_girl = selected_girl;
    talk_names_set(selected_girl);
    encounter_player_birthday_scripts_bind();
    encounter_player_birthday_girl_script_select();
    return mode_step_next();
}

void encounter_player_birthday_default_script_select(void) {
    encounter_player_birthday_scripts_bind();
    talk_names_set(0);
    encounter_player_birthday_text_table = encounter_player_birthday_text_section_11;
    encounter_player_birthday_command_table = encounter_player_birthday_command_section_11;
    encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_11;
    mode_step_next();
}

void encounter_player_birthday_girl_script_select(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_1;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_1;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_1;
        return;
    case GIRL_MIO:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_6;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_6;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_6;
        return;
    case GIRL_YUINA:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_3;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_3;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_3;
        return;
    case GIRL_AYAKO:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_4;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_4;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_4;
        return;
    case GIRL_SAKI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_10;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_10;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_10;
        return;
    case GIRL_YUKARI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_7;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_7;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_7;
        return;
    case GIRL_NOZOMI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_8;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_8;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_8;
        return;
    case GIRL_MIRA:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_5;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_5;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_5;
        return;
    case GIRL_YUKO:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_0;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_0;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_0;
        return;
    case GIRL_MEGUMI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_9;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_9;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_9;
        return;
    case GIRL_YUMI:
        encounter_player_birthday_text_table = encounter_player_birthday_text_section_12;
        encounter_player_birthday_command_table = encounter_player_birthday_command_section_12;
        encounter_player_birthday_voice_table = encounter_player_birthday_voice_section_12;
        /* fallthrough */
    default:
        return;
    }
}
