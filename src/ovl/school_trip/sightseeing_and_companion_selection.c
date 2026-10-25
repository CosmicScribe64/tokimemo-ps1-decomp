/* sightseeing_and_companion_selection.c: src/ovl/SHUGAKU/80138A60.c:9-49,144-177,187-221; sightseeing callbacks/site selection, same-club/girl choice gates, eligible-companion ranking and Ayako detour routing. Original object 0x80138A60-0x80139D00 (config/objects/SHUGAKU.txt). */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/school_trip.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void trip_sightseeing_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripSecondaryCallbacks tbl;

    tbl = D_8013C930;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_group_activity_label_display(void) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 2, "【団体行動中】", 0));
    mode_step_next();
}

void trip_destination_one_mode_step_callback_dispatch(void) {
    /* FAKE: the wrapper reserves the observed extra frame extent. T-10020. */
    ShugakuCallbackLocal12C local;
    u8 state;

    local.callbacks = D_8013C984;
    /* FAKE: the dead branch preserves the later trip_mio_dialogue_background_flag_update comparison
     * operand order in this compilation unit; it emits no code. T-10020. */
    if (1) {}
    if (trip_destination == 1) {
        state = game_state.mode_step;
        if (state >= 0x1FU && state < 0x25U) {
            hud_sprite_light_effect_draw();
        }
    }
    state = game_state.mode_step;
    local.callbacks.callback[state](0x80);
}

void trip_saki_same_club_talk_state_update(void) {
    trip_destination_response_talk_run();
    if (game_state.current_girl == GIRL_SAKI && ((u32)game_state.girls[4].flags.b[2] >> 4) == ((u32)game_state.player_flags.h >> 0xC)) {
        *(u8 *)(*(s32 *)(trip_talk_command_table + trip_talk_group * 4) + 3) = 0xFF;
    }
}

void trip_saki_same_club_script_offset(void) {
    if (game_state.substep == 0) {
        if (trip_saved_girl == 4 && ((u32)game_state.girls[4].flags.b[2] >> 4) == ((u32)game_state.player_flags.h >> 0xC)) {
            trip_talk_group = 0x2E;
        }
        game_state.substep = 1;
    }
    trip_destination_response_talk_run();
}

void trip_destination_dialogue_entry_gate(void) {
    u8 **p;

    p = (u8 **)trip_talk_command_table;
    if (p[trip_talk_group][trip_talk_line] == 9) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void trip_sightseeing_route_flag_gate(void) {
    if (trip_destination_exit_requested == 1) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void trip_destination_dialogue_exit_gate(void) {
    u8 **p;

    p = (u8 **)trip_talk_command_table;
    if (p[trip_talk_group][trip_talk_line] == 9) {
        mode_step_next();
        trip_destination_exit_requested = 1;
        return;
    }
    normal_date_girl_in();
}

void trip_mio_dialogue_background_flag_update(void) {
    u8 value;

    trip_destination_response_talk_run();
    if (game_state.current_girl == GIRL_MIO && trip_talk_line == 3) {
        value = trip_destination;
        if (value == 1) {
            D_80122CF4 = 0;
            return;
        }
    }
    D_80122CF4 = 1;
}

s32 trip_mio_scene_background_and_girl_transition(void) {
    /* FAKE: `pad` reserves the top stack word; `(game_state.substep & 0) +` places an entry load of unk_110D before the first branch so cvt_pass strips its widenings (T-24114). */
    s32 pad;
    s32 saved_phase;
    s32 step;

    if (((game_state.substep & 0) + game_state.current_girl != 4) || (trip_selected_companion_code != 0x36)) {
        normal_date_bg_fadeout();
        return;
    }
    saved_phase = (s32) game_state.mode_step;
    if ((game_state.mode_step_timer.w == 0) && (game_state.substep == 0)) {
        trip_talk_group = 0x10;
    }
    if (game_state.substep == 0) {
        game_state.mode_step_timer.w += 1;
    }
    step = (s32) game_state.substep;
    if (game_state.substep == 0) {
        trip_destination_response_talk_run();
    } else if (game_state.substep == 1) {
        normal_date_girl_in();
    } else if (game_state.substep == 2) {
        trip_destination_response_talk_run();
    } else if (game_state.substep == 3) {
        normal_date_girl_out();
    } else {
        normal_date_bg_fadeout();
    }
    if (saved_phase != game_state.mode_step) {
        if (step < 4) {
            game_state.substep = step + 1;
            game_state.mode_step = saved_phase;
            return;
        }
        trip_talk_group = trip_saved_destination_dialogue_group;
        game_state.substep = step + 1;
    }
}

void trip_ayako_birthday_mode_stage_route(void) {
    if (game_state.current_girl != GIRL_AYAKO || trip_destination != 1) {
        normal_date_girl_out();
        return;
    }
    game_state.saved_mode_stage = game_state.mode_stage + 1;
    game_state.saved_mode_step = 0;
    mode_stage_set(9);
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/school_trip/sightseeing_and_companion_selection.rodata", D_8013AEF0);

void trip_sightseeing_hotel_lobby_background_load(void) {
    bg_read_sub2(0x4225);
    strcpy(scene_place_title, &D_8013AEF0);
    mode_step_next();
}

void trip_sightseeing_place_background_and_log(void) {
    u8 n;

    n = trip_destination;
    switch (n) {
    case 0:
        bg_read_sub2(0x425E);
        strcpy(scene_place_title, "社寺巡り");
        break;
    case 1:
        bg_read_sub2(0x424B);
        strcpy(scene_place_title, "守礼門");
        break;
    default:
        bg_read_sub2(0x4271);
        strcpy(scene_place_title, "時計台前");
        break;
    }
    if (trip_alternate_dialogue_mode == 0) {
        n = trip_destination; /* reload into the switch variable: its reuse gives the original's a1 selector (T-18000) */
        event_log_add(3, n);
    }
    mode_step_next();
}

void trip_low_status_girl_select(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    D_8013C978 = 0;
    best_i = 0xFF;
    best_s = 4;
    trip_selected_companion_code = 0;
    for (i = 0; i < 10; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s6 (T-24122). */
        if (1) {}
        if (select_girl_menu_enabled[i] != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            if (s < best_s ||
                (s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i != 0xFF && s < 2U) {
        trip_selected_companion_code = best_i + 0x32;
        /* FAKE: `(best_i & 0) +` keeps cfe's CVT J L narrowing so uopt emits `andi $a0, $s7, 0xff` (T-24122). */
        game_state.current_girl = (best_i & 0) + best_i;
        talk_names_set(game_state.current_girl);
        trip_current_girl_destination_pointer_bind();
    } else {
        trip_talk_group += 4;
        game_state.mode_step += 0x1E;
        game_state.current_girl = 0xFF;
    }
    mode_step_next();
}

s32 trip_response_parameter_update_alt(void) {
    /* FAKE: `six` reserves the top stack word and `(six & 0) + six` promotes the constant 6 to $v0;
     * `*(s16 *)&trip_talk_group` keeps the first store out of the second block's scalar web (T-24104). */
    s32 six;
    s32 previous_phase;

    if (talk_choice == 0) {
        if (game_state.current_girl == GIRL_YUKO) {
            previous_phase = game_state.mode_step;
            six = 6;
            *(s16 *)&trip_talk_group = (six & 0) + six;
            trip_destination_response_talk_run();
            if (previous_phase == game_state.mode_step) {
                return 0;
            }
            game_state.mode_step--;
        }
        trip_saved_destination_dialogue_group = trip_talk_group = trip_destination + 0x1A;
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else {
        game_state.girls[game_state.current_girl].tokimeki -= 1;
        game_state.girls[game_state.current_girl].affection -= 2;
        check_para_limit();
    }
    mode_step_next();
}

void trip_destination_script_offset_update(void) {
    trip_talk_group = (trip_talk_group - trip_destination) + 2;
    mode_step_next();
}

void trip_destination_label_select(void) {
    /* FAKE: the local selector preserves v0 instead of v1. T-10020. */
    s32 selection;
    trip_destination_exit_requested = 0;
    selection = trip_destination;
    switch (selection) {
    case 0:
        strcpy(name_code_s, "京都");
        break;
    case 1:
        strcpy(name_code_s, "那覇");
        break;
    case 2:
        strcpy(name_code_s, "札幌");
        break;
    }
    mode_step_next();
}

void trip_nonzero_response_audio_select(void) {
    if (talk_choice != 0) {
        if (game_state.current_girl == GIRL_YUINA || game_state.current_girl == GIRL_MIRA || game_state.current_girl == GIRL_MEGUMI) {
            face_expression_4();
            return;
        }
        face_expression_3();
        return;
    }
    mode_step_next();
}

void trip_destination_choice_step_skip(void) {
    if (talk_choice != 0) {
        game_state.mode_step += 0x11;
    }
    mode_step_next();
}

void trip_yoshio_dialogue_route_setup(void) {
    if (talk_choice != 0) {
        trip_talk_group = 0;
        trip_talk_text_table = D_8013C450;
        trip_talk_command_table = D_8013C484;
        trip_talk_voice_table = D_8013C4B8;
        trip_alternate_dialogue_mode = 1;
        game_state.current_girl = GIRL_YOSHIO;
        talk_names_set(0xDU);
        game_state.mode_step = 0x33;
        return;
    }
    mode_step_next();
}

void trip_destination_script_offset_set(void) {
    trip_talk_group = 0xE;
    mode_step_next();
}

void trip_current_girl_status_script_offset(void) {
    s32 status_script_offset;
    u8 x;
    u32 t;

    if (trip_alternate_dialogue_mode == 0) {
        x = get_g_zyotai_s(game_state.current_girl);
        t = x & 0x7F;
        if (t < 2U) {
            status_script_offset = 0;
        } else if (t == 2) {
            status_script_offset = 3;
        } else if (t == 3) {
            status_script_offset = 6;
        } else {
            status_script_offset = 9;
        }
        trip_talk_group += status_script_offset;
    }
    mode_step_next();
}

void trip_destination_script_offset_and_rewind(void) {
    trip_talk_group = trip_destination + 0x1A;
    if (trip_alternate_dialogue_mode != 0) {
        trip_talk_group = trip_destination + 3;
    }
    game_state.mode_step -= 0x2E;
    mode_step_next();
}

void trip_yoshio_portrait_read_gate(void) {
    if (trip_alternate_dialogue_mode != 0) {
        portrait_read_start(0x6C32);
    } else {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 trip_status_four_yoshio_detour_select(void) {
    s32 pad[2]; /* FAKE: unused locals, frame slots only (T-10310) */
    s32 state;
    u8 t; /* FAKE: narrow type shifts the temporary numbering to the original's (permuter, T-10310) */

    t = get_g_zyotai_s(game_state.current_girl);
    trip_saved_girl = 0xFF;
    if ((t & 0x7F) == 4 && trip_alternate_dialogue_mode == 0) {
        trip_saved_girl = game_state.current_girl;
        if (game_state.current_girl == GIRL_MIO) {
            state = game_state.mode_step;
            normal_date_girl_out();
            if (state == game_state.mode_step) {
                return 0;
            }
        }
        trip_alternate_dialogue_mode = 1;
        game_state.current_girl = GIRL_YOSHIO;
        talk_names_set(0xD);
        get_g_name(girl_name_buf, trip_saved_girl);
        mode_step_next();
    } else {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        if (trip_alternate_dialogue_mode != 0) {
            mode_step_next();
            mode_step_next();
            mode_step_next();
            mode_step_next();
        }
    }
}

void trip_yoshio_script_table_setup(void) {
    trip_talk_group = 0;
    trip_talk_text_table = D_8013C450;
    trip_talk_command_table = D_8013C484;
    trip_talk_voice_table = D_8013C4B8;
    game_state.mode_step = 0x33;
}

void trip_current_girl_status_refresh(void) {
    get_g_zyotai_s(game_state.current_girl);
    mode_step_next();
}

s32 trip_companion_selection_and_yoshio_route(void) {
    s32 selection;
    u8 value;

    selection = talk_choice;
    value = game_state.current_girl;
    if (value == selection) {
        value = game_state.mode_step;
        trip_destination_response_talk_run();
        if (value != game_state.mode_step) {
            trip_talk_group -= 2;
            game_state.mode_step -= 3;
        }
        return 0;
    }
    value = selection;
    /* FAKE: second read of unk_F5F across the entry branch causes uopt to promote unk_F5F to $a1 while folding the unsigned >= 0 guard (T-24114). */
    if (game_state.current_girl >= 0) {
        if (selection == 0xB) {
            trip_talk_group = 0;
            trip_talk_text_table = D_8013C450;
            trip_talk_command_table = D_8013C484;
            trip_talk_voice_table = D_8013C4B8;
            trip_alternate_dialogue_mode = 1;
            value = 0xD;
            game_state.current_girl = value;
            talk_names_set(value);
            game_state.mode_step = 0x33;
            return 0;
        }
    }
    game_state.current_girl = value;
    talk_names_set(value);
    trip_current_girl_destination_pointer_bind();
    mode_step_next();
}

void trip_current_girl_destination_pointer_bind(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        trip_talk_text_table = D_8013C424;
        trip_talk_command_table = D_8013C458;
        trip_talk_voice_table = D_8013C48C;
        break;
    case GIRL_MIO:
        trip_talk_text_table = D_8013C434;
        trip_talk_command_table = D_8013C468;
        trip_talk_voice_table = D_8013C49C;
        break;
    case GIRL_YUINA:
        trip_talk_text_table = D_8013C428;
        trip_talk_command_table = D_8013C45C;
        trip_talk_voice_table = D_8013C490;
        break;
    case GIRL_AYAKO:
        trip_talk_text_table = D_8013C42C;
        trip_talk_command_table = D_8013C460;
        trip_talk_voice_table = D_8013C494;
        break;
    case GIRL_SAKI:
        trip_talk_text_table = D_8013C448;
        trip_talk_command_table = D_8013C47C;
        trip_talk_voice_table = D_8013C4B0;
        break;
    case GIRL_YUKARI:
        trip_talk_text_table = D_8013C438;
        trip_talk_command_table = D_8013C46C;
        trip_talk_voice_table = D_8013C4A0;
        break;
    case GIRL_NOZOMI:
        trip_talk_text_table = D_8013C440;
        trip_talk_command_table = D_8013C474;
        trip_talk_voice_table = D_8013C4A8;
        if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            trip_talk_text_table = D_8013C43C;
            trip_talk_command_table = D_8013C470;
            trip_talk_voice_table = D_8013C4A4;
        }
        break;
    case GIRL_MIRA:
        trip_talk_text_table = D_8013C430;
        trip_talk_command_table = D_8013C464;
        trip_talk_voice_table = D_8013C498;
        break;
    case GIRL_YUKO:
        trip_talk_text_table = D_8013C420;
        trip_talk_command_table = D_8013C454;
        trip_talk_voice_table = D_8013C488;
        break;
    case GIRL_MEGUMI:
        trip_talk_text_table = D_8013C444;
        trip_talk_command_table = D_8013C478;
        trip_talk_voice_table = D_8013C4AC;
        break;
    }
}

void trip_sightseeing_callback_dispatch_alt(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripReturnCallbacks tbl;

    tbl = D_8013CAA0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}
