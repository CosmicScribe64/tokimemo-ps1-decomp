/* birthday_detours_and_talk.c: src/ovl/SHUGAKU/80132000.c:16-65,299-355; binds per-girl birthday tables, dispatches birthday callbacks, handles saved-stage returns and choice effects. Shared birthday routing/ranking is in trip_birthday_girl_select; no location-table selection claim. Original object 0x80132000-0x80133430 (config/objects/SHUGAKU.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/school_trip.h"

void portrait_layout_set(s32 arg0);
u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

typedef struct {
    u8 s[0xCC3];
} TripTextData; /* size 0xCC3 */
extern TripTextData D_8013B1AC;

void trip_birthday_script_pointer_table_setup(void) {
    D_8013AFD0 = 0x801B0478;
    D_8013AFD4 = 0x801B13A4;
    D_8013AFD8 = 0x801B2998;
    D_8013AFDC = 0x801B38C8;
    D_8013AFE0 = 0x801B4258;
    D_8013AFE4 = 0x801B4B98;
    D_8013AFE8 = 0x801B5574;
    D_8013AFEC = 0x801B6560;
    D_8013AFF0 = 0x801B7B74;
    D_8013AFF4 = 0x801B8B18;
    D_8013AFF8 = 0x801B93D0;
    D_8013AFFC = 0x801B9DA0;
    D_8013B000 = 0x801BA780;
    D_8013B004 = 0x801B0510;
    D_8013B008 = 0x801B14E0;
    D_8013B00C = 0x801B2AD0;
    D_8013B010 = 0x801B3960;
    D_8013B014 = 0x801B42F0;
    D_8013B018 = 0x801B4C38;
    D_8013B01C = 0x801B560C;
    D_8013B020 = 0x801B66A0;
    D_8013B024 = 0x801B7CB4;
    D_8013B028 = 0x801B8BB0;
    D_8013B02C = 0x801B9468;
    D_8013B030 = 0x801B9EA8;
    D_8013B034 = 0x801BA818;
    D_8013B038 = 0x801B0888;
    D_8013B03C = 0x801B1E14;
    D_8013B040 = 0x801B33B0;
    D_8013B044 = 0x801B3CF4;
    D_8013B048 = 0x801B4684;
    D_8013B04C = 0x801B4FEC;
    D_8013B050 = 0x801B59F4;
    D_8013B054 = 0x801B6FB8;
    D_8013B058 = 0x801B85CC;
    D_8013B05C = 0x801B8F28;
    D_8013B060 = 0x801B97E0;
    D_8013B064 = 0x801BA2C8;
    D_8013B068 = 0x801BAB90;
}

void trip_callback_dispatch(void) {
    s32 callback_index; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripModeStepCallbacks tbl;

    tbl = D_8013B094;
    callback_index = game_state.mode_step;
    tbl.f[callback_index](0x80);
}

void trip_talk_step_branch_adjust(void) {
    GameState *state;
    s32 previous;

    state = &game_state;
    previous = (s32) game_state.mode_step;
    trip_sightseeing_hotel_lobby_background_load();
    if (((state->mode_step != (previous + 3)) || ((u8) scene_trip_day_state != 0)) && (state->mode_step == (previous + 4)) && ((u8) scene_trip_day_state == 0)) {
        state->mode_step -= 1;
        dec_bg_show_switch(1);
    }
}

void trip_opening_talk_and_script_state(void) {
    s32 pad; /* FAKE: unused local reserves a stack word (frame home). T-11170 */
    s32 previous_mode_step;

    if (game_state.substep == 0) {
        game_state.substep += 1;
        trip_girl_script_pointer_table_setup();
        trip_dialogue_text_table = D_8013C424;
        trip_dialogue_command_table = D_8013C458;
        trip_dialogue_voice_table = D_8013C48C;
        trip_dialogue_group = 0xE;
        trip_dialogue_line = 0;
    }
    if (game_state.substep == 1) {
        previous_mode_step = (s32) game_state.mode_step;
        trip_response_talk_run();
        if (previous_mode_step != game_state.mode_step) {
            trip_girl_scene_select();
            game_state.mode_step -= 1;
            trip_dialogue_group = 5;
        }
    }
}

s32 trip_birthday_girl_select(void) {
    s16 i;
    u8 best_girl_index;
    u8 best_status;
    u8 candidate_status;

    best_girl_index = 0xFF;
    best_status = 4;
    if (game_state.month != ((WordView *)&game_state.player_birthday)->date.month ||
        game_state.day != ((WordView *)&game_state.player_birthday)->date.day) {
        mode_step_next();
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (select_girl_menu_enabled[i] != 0) {
            candidate_status = get_g_zyotai_s(i) & 0x7F;
            /* FAKE: duplicate `candidate_status == best_status` shifts uopt'candidate_status live-range priority between candidate_status and best_status (T-24122). */
            if (candidate_status < best_status ||
                (candidate_status == best_status && candidate_status == best_status &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_girl_index) + get_h_yuukou(best_girl_index))) {
                best_girl_index = i;
                best_status = candidate_status;
            }
        }
    }
    candidate_status = get_g_zyotai_s(best_girl_index) & 0x7F;
    if (candidate_status < 2U && best_girl_index == 0) {
        mode_step_set(0x32);
        return 0;
    }
    mode_step_next();
}

void trip_audio_bank_load_and_step(void) {
    audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        if (game_state.mode_stage == 6) {
            audio_command(0x200);
        }
        mode_step_next();
    }
    mode_step_next();
}

void trip_audio_bank_poll_retry(void) {
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

void trip_birthday_script_sector_load(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    trip_birthday_script_pointer_table_setup();
    mode_step_next();
}

void trip_birthday_scene_pointer_bind(void) {
    trip_scene_pointer_table_setup();
    trip_dialogue_text_table = D_8013BE74;
    trip_dialogue_command_table = D_8013BEA8;
    trip_dialogue_voice_table = D_8013BEDC;
    trip_dialogue_group = 1;
    trip_dialogue_line = 0;
    mode_step_next();
}

void trip_birthday_event_log_and_pointer_setup(void) {
    if (game_state.saved_mode_stage != 6) {
        event_log_add(0xA, 0x29);
    } else {
        switch (trip_destination) {                       /* irregular */
        case 0:
            event_log_add(0xA, 0x2A);
            break;
        case 1:
            event_log_add(0xA, 0x2B);
            break;
        default:
            event_log_add(0xA, 0x2C);
            break;
        }
    }
    trip_birthday_script_pointer_table_setup();
    trip_girl_scene_select();
    if ((bg_brightness != 0) && (D_80122CF8 != 0)) {
        scene_trip_day_state = 1;
        return;
    }
    scene_trip_day_state = 0;
}

void trip_girl_scene_select(void) {
    trip_dialogue_group = 0;
    trip_dialogue_line = 0;
    talk_names_set(game_state.current_girl);
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        trip_dialogue_text_table = D_8013AFD4;
        trip_dialogue_command_table = D_8013B008;
        trip_dialogue_voice_table = D_8013B03C;
        break;
    case GIRL_MIO:
        trip_dialogue_text_table = D_8013AFE4;
        trip_dialogue_command_table = D_8013B018;
        trip_dialogue_voice_table = D_8013B04C;
        break;
    case GIRL_YUINA:
        trip_dialogue_text_table = D_8013AFD8;
        trip_dialogue_command_table = D_8013B00C;
        trip_dialogue_voice_table = D_8013B040;
        break;
    case GIRL_AYAKO:
        trip_dialogue_text_table = D_8013AFDC;
        trip_dialogue_command_table = D_8013B010;
        trip_dialogue_voice_table = D_8013B044;
        break;
    case GIRL_SAKI:
        trip_dialogue_text_table = D_8013AFF8;
        trip_dialogue_command_table = D_8013B02C;
        trip_dialogue_voice_table = D_8013B060;
        break;
    case GIRL_YUKARI:
        trip_dialogue_text_table = D_8013AFE8;
        trip_dialogue_command_table = D_8013B01C;
        trip_dialogue_voice_table = D_8013B050;
        break;
    case GIRL_NOZOMI:
        trip_dialogue_text_table = D_8013AFF0;
        trip_dialogue_command_table = D_8013B024;
        trip_dialogue_voice_table = D_8013B058;
        if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            trip_dialogue_text_table = D_8013AFEC;
            trip_dialogue_command_table = D_8013B020;
            trip_dialogue_voice_table = D_8013B054;
        }
        break;
    case GIRL_MIRA:
        trip_dialogue_text_table = D_8013AFE0;
        trip_dialogue_command_table = D_8013B014;
        trip_dialogue_voice_table = D_8013B048;
        break;
    case GIRL_YUKO:
        trip_dialogue_text_table = D_8013AFD0;
        trip_dialogue_command_table = D_8013B004;
        trip_dialogue_voice_table = D_8013B038;
        break;
    case GIRL_MEGUMI:
        trip_dialogue_text_table = D_8013AFF4;
        trip_dialogue_command_table = D_8013B028;
        trip_dialogue_voice_table = D_8013B05C;
        break;
    case GIRL_YUMI:
        trip_dialogue_text_table = D_8013B000;
        trip_dialogue_command_table = D_8013B034;
        trip_dialogue_voice_table = D_8013B068;
        break;
    }
    if (game_state.month >= TRIP_YEAR_OFFSET_MONTH_BOUNDARY) {
        trip_dialogue_year_variant = game_state.year - 0x5F;
    } else {
        trip_dialogue_year_variant = game_state.year - 0x60;
    }
    mode_step_next();
}

void trip_background_show_or_resume(void) {
    if ((u8) scene_trip_day_state == 1) {
        normal_date_move_place();
        return;
    }
    if ((u8) scene_trip_day_state == 2) {
        mode_step_next();
        return;
    }
    if (game_state.substep++ == 0) {
        dec_bg_show_switch(1);
    }
    scene_bg_wipe_in();
}

void trip_saved_mode_stage_restore(void) {
    if (game_state.saved_mode_stage != 6) {
        /* FAKE: the comma expression preserves the original argument register.
         * Original spelling unknown; found by permute.py. T-9530. */
        mode_stage_set((0, (s32) game_state.saved_mode_stage));
        mode_step_set((s32) game_state.saved_mode_step);
        return;
    }
    trip_day_event_complete(game_state.saved_mode_stage);
}

void trip_saved_mode_step_advance(void) {
    if (game_state.saved_mode_stage == 6) {
        game_state.mode_step += TRIP_SAVED_MODE_STEP_DELTA;
        return;
    }
    mode_step_next();
}

void trip_response_state_update(void) {
    if (talk_choice != 0) {
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        game_state.mode_step += 0x1B;
        return;
    }
    trip_dialogue_group += 1;
    mode_step_next();
}

void trip_status_dialogue_offset(void) {
    u32 t;

    t = get_g_zyotai_s(game_state.current_girl);
    t = t & 0x7F;
    t = t & 0x7F; /* FAKE: the repeated mask reproduces the original's extra move; real source unknown. T-4070 */
    if (t >= 2U) {
        if (t == 2) {
            trip_dialogue_group += 2;
        } else if (t == 3) {
            trip_dialogue_group += 4;
        } else {
            trip_dialogue_group += 6;
        }
    }
    mode_step_next();
}

void trip_birthday_detour_date_girl_restore_current(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    mode_step_next();
}

void trip_saved_mode_stage_six_gate(void) {
    if (game_state.saved_mode_stage != 6) {
        trip_destination_response_talk_run();
        return;
    }
    mode_step_next();
}

void trip_saved_script_pointer_restore(void) {
    trip_dialogue_group = trip_saved_dialogue_group;
    trip_dialogue_text_table = trip_saved_dialogue_text_table;
    trip_dialogue_command_table = trip_saved_dialogue_command_table;
    trip_dialogue_voice_table = trip_saved_dialogue_voice_table;
    trip_birthday_script_text_copy();
    mode_step_next();
}

void trip_birthday_script_text_copy(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    TripTextData tbl;

    tbl = D_8013B1AC;
    strcpy(name_code_a, &tbl.s[game_state.current_girl * 0x129 + trip_dialogue_year_variant * 0x63 + talk_choice * 0x21]);
}

void trip_response_talk_run(void) {
    talk_tables = &trip_dialogue_group;
    talk_active_line_ptr = &trip_dialogue_line;
    talk_active_text_table = trip_dialogue_text_table;
    talk_active_command_table = trip_dialogue_command_table;
    talk_active_voice_table = trip_dialogue_voice_table;
    talk_run(talk_choice, 1, 0);
}

void trip_default_talk_run(void) {
    talk_tables = &trip_dialogue_group;
    talk_active_line_ptr = &trip_dialogue_line;
    talk_active_text_table = trip_dialogue_text_table;
    talk_active_command_table = trip_dialogue_command_table;
    talk_active_voice_table = trip_dialogue_voice_table;
    talk_run(0xFF, 1, 0);
}

void trip_script_table_save_and_year_slot_select(void) {
    trip_saved_dialogue_group = trip_dialogue_group;
    trip_saved_dialogue_text_table = trip_dialogue_text_table;
    trip_saved_dialogue_command_table = trip_dialogue_command_table;
    trip_saved_dialogue_voice_table = trip_dialogue_voice_table;
    trip_dialogue_group = game_state.current_girl;
    /* (s16) leaves the ugen temporary between sll and subu unused, as in the original; the product sits before the last three copies so their constant loads take t5/t6/t7. T-24017 */
    trip_dialogue_group = (s16)(trip_dialogue_group * 3) + trip_dialogue_year_variant;
    trip_dialogue_text_table = D_8013AFFC;
    trip_dialogue_command_table = D_8013B030;
    trip_dialogue_voice_table = D_8013B064;
    mode_step_next();
}


void trip_response_parameter_update(void) {
    portrait_layout_set(talk_choice + 1);
    if (talk_choice == 0) {
        game_state.girls[game_state.current_girl].tokimeki += 3;
        game_state.girls[game_state.current_girl].affection += 2;
        game_state.girls[game_state.current_girl].syoushin -= 0xF;
    } else if (talk_choice == 1) {
        game_state.girls[game_state.current_girl].tokimeki += 2;
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin -= 0xA;
    } else {
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin += 0xA;
    }
    check_para_limit();
    mode_step_next();
}

void trip_choice_script_offset_and_background_state(void) {
    trip_dialogue_group = (talk_choice * 2) + 0xD;
    scene_trip_day_state = 2;
    mode_step_next();
}

void trip_player_birthday_mode_stage_gate(void) {
    get_g_zyotai_s(game_state.date_girl);
    if (game_state.month == (game_state.player_birthday & 0xF) && game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B)) {
        mode_stage_set(7);
        return;
    }
    mode_step_next();
}

void trip_saved_route_status_gate(void) {
    u16 v;
    u8 c; /* FAKE: copy of trip_destination, gives the original's bne operand order; real source unknown. T-11440 */

    v = get_g_zyotai_s(game_state.date_girl);
    if (game_state.saved_mode_stage == 5 && game_state.date_girl == GIRL_YUINA && (c = trip_destination) == 2) {
        bg_brightness = 0;
        mode_stage_set(8);
        return;
    }
    /* FAKE: the byte view of unk_75D below gives the original's bne operand order; real source unknown. T-11440 */
    if (game_state.saved_mode_stage == 3 && *(u8 *) &game_state.date_girl == 5 && (v & 0x7F) < 2) {
        mode_stage_set(0xB);
        return;
    }
    if (game_state.saved_mode_stage == 4 && *(u8 *) &game_state.date_girl == 4 && (v & 0x7F) < 2) {
        mode_stage_set(0xA);
        return;
    }
    mode_step_next();
}
