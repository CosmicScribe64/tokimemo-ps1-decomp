/* new_year_dialogue.c: ; src/ovl/SHOUGATU/801330C0.c:31-70 selects and prepares New Year talk tables. Original object 0x801330C0-0x80133D10 (config/objects/SHOUGATU.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* Word argument at this call (new_year_target_dialogue_setup); T-17500. */
#include "common.h"
#include "ovl/new_year_christmas.h"
void talk_names_set();

void new_year_dialogue_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before new_year_dialogue_callbacks: its stack slot sits above new_year_dialogue_callbacks (T-3330) */
    FnTbl60 new_year_dialogue_callbacks;

    new_year_dialogue_callbacks = D_80143BC4;
    mode_step_index = game_state.mode_step;
    new_year_dialogue_callbacks.f[mode_step_index](0x80);
}

void new_year_dialogue_prompt_sound(void) {
    audio_command(0x603);
    mode_step_next();
}

s32 new_year_dialogue_first_tick_audio(void) {
    new_year_talk_run_current();
    if (game_state.mode_step_timer.w++ == 0) {
        if (((u8 **)new_year_dialogue_command_table)[new_year_dialogue_group][new_year_dialogue_line - 1] == 7) {
            if (strcmp(((s32 **)new_year_dialogue_text_table)[new_year_dialogue_group][new_year_dialogue_line - 1], "ガチャ") == 0) {
                audio_command(0x60C);
            }
        }
    }
}

s32 new_year_dialogue_phone_digit_audio(void) {
    s32 sound_id;

    if ((game_state.pad_pressed & 0x400040) &&
        new_year_dialogue_line > 0 && new_year_dialogue_line < 5 &&
        (u32)game_state.mode_step_timer.w >= 2U) {
        audio_command(0x1508);
        talk_line_auto = 0;
        k_reset(1);
        talk_voice_wait_frames = 0;
        new_year_dialogue_line = 4;
    } else {
        if ((check_end_k() != 0) && (new_year_dialogue_line < 5)) {
            if (new_year_dialogue_line == 0) {
                if (new_year_dialogue_delay_frames < 0x5A) {
                    new_year_dialogue_delay_frames += 1;
                    if (new_year_dialogue_delay_frames == 1) {
                        audio_command(0x60A);
                    }
                    if (new_year_dialogue_delay_frames == 0x1E) {
                        audio_command(0x604);
                    }
                    return 0;
                }
                if (!(game_state.substep & 7)) {
                    sound_id = (((girl_phone_digits[game_state.current_girl][game_state.substep >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                    /* FAKE: retain the original unchanged-byte store before the call.
                     * Direct self-assignment is removed; original spelling unknown. T-24052. */
                    game_state.substep |= 0;
                    audio_command(sound_id);
                }
            }
            if (game_state.substep++ < 0x28U) {
                return 0;
            }
            game_state.substep = 0;
            k_reset(1);
        }
    }
    new_year_talk_run_current();
    if ((new_year_dialogue_line < 5) && (game_state.mode_step_timer.w == 1)) {
        audio_command(0x608);
    }
    game_state.mode_step_timer.w += 1;
}

s32 new_year_dialogue_input_update_short(void) {
    s32 sound_id;

    if ((game_state.pad_pressed & 0x400040) &&
        new_year_dialogue_line > 0 && new_year_dialogue_line < 3 &&
        (u32)game_state.mode_step_timer.w >= 2U) {
        audio_command(0x1508);
        talk_line_auto = 0;
        k_reset(1);
        talk_voice_wait_frames = 0;
        new_year_dialogue_line = 2;
    } else {
        if ((check_end_k() != 0) && (new_year_dialogue_line < 3)) {
            if (new_year_dialogue_line == 0) {
                if (new_year_dialogue_delay_frames < 0x5A) {
                    new_year_dialogue_delay_frames += 1;
                    if (new_year_dialogue_delay_frames == 1) {
                        audio_command(0x60A);
                    }
                    if (new_year_dialogue_delay_frames == 0x1E) {
                        audio_command(0x604);
                    }
                    return 0;
                }
                if (!(game_state.substep & 7)) {
                    sound_id = (((girl_phone_digits[game_state.current_girl][game_state.substep >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                    /* FAKE: retain the original unchanged-byte store before the call.
                     * Direct self-assignment is removed; original spelling unknown. T-24052. */
                    game_state.substep |= 0;
                    audio_command(sound_id);
                }
            }
            if (game_state.substep++ < 0x28U) {
                return 0;
            }
            game_state.substep = 0;
            k_reset(1);
        }
    }
    new_year_talk_run_current();
    if ((new_year_dialogue_line < 3) && (game_state.mode_step_timer.w == 1)) {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        new_year_dialogue_line = new_year_dialogue_line;
        audio_command(0x608);
    } else {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        new_year_dialogue_line = new_year_dialogue_line;
        if ((new_year_dialogue_line == 3) && (game_state.mode_step_timer.w == 1)) {
            audio_command(0x60B);
        }
    }
    game_state.mode_step_timer.w += 1;
}

void new_year_dialogue_resource_state_init(void) {
    strcpy(scene_place_title_2, "正月");
    new_year_dialogue_delay_frames = 0;
    new_year_fallback_dialogue_flag = 0;
    new_year_shrine_invitation_flag = 1;
    new_year_dialogue_group = 6;
    new_year_dialogue_text_table = D_80143A48;
    new_year_dialogue_command_table = D_80143A84;
    new_year_dialogue_voice_table = D_80143AC0;
    mode_step_next();
}

void new_year_dialogue_background_select(void) {
    if (new_year_candidate_selected == 0) {
        bg_read_sub2(0x4167);
        strcpy(scene_place_title, "正月");
        k_reset(0);
        message_window_set_title(scene_place_title);
    } else {
        game_state.mode_step += 3;
    }
    mode_step_next();
}

void new_year_dialogue_background_read_primary(void) {
    bg_read_sub2(0x4167);
    mode_step_next();
}

void new_year_dialogue_background_read_alternate(void) {
    bg_read_sub2(0x4144);
    mode_step_next();
}

s32 new_year_target_dialogue_setup(void) {
    s32 girl;
    u8 previous;

    if (talk_choice == 0xB) {
        new_year_dialogue_fallback_table_setup();
        return 0;
    }
    girl = talk_choice;
    previous = game_state.current_girl;
    if (previous == girl && new_year_candidate_selected != 0) {
        mode_step_next();
        return 0;
    }
    if (game_state.girls[talk_choice].flags.f.b3 == 0) {
        new_year_dialogue_group = 0;
        new_year_dialogue_text_table = D_80143A78;
        new_year_dialogue_command_table = D_80143AB4;
        new_year_dialogue_voice_table = D_80143AF0;
        talk_names_set(talk_choice);
        mode_step_next();
        return 0;
    }
    game_state.current_girl = (u8)girl;
    new_year_girl_dialogue_tables_select();
    talk_names_set(game_state.current_girl);
    new_year_dialogue_group = 9;
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void new_year_dialogue_group_reset(void) {
    new_year_dialogue_group = 7;
    new_year_dialogue_text_table = D_80143A48;
    new_year_dialogue_command_table = D_80143A84;
    new_year_dialogue_voice_table = D_80143AC0;
    mode_step_next();
    game_state.mode_step -= 5;
}

void new_year_dialogue_status_branch_update(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)get_g_zyotai_s(game_state.current_girl) & 0x7F) != 4) {
        new_year_dialogue_group += 1;
        mode_step_next();
        mode_step_next();
        mode_step_next();
        return;
    }
    mode_step_next();
}

void new_year_dialogue_conditional_step_advance(void) {
    if (talk_choice == 0) {
        mode_step_next();
    }
    mode_step_next();
}

void new_year_dialogue_fallback_table_setup(void) {
    new_year_fallback_dialogue_flag = 1;
    new_year_dialogue_group = 0;
    new_year_dialogue_text_table = D_80143A40;
    new_year_dialogue_command_table = D_80143A7C;
    new_year_dialogue_voice_table = D_80143AB8;
    mode_stage_next();
}

void new_year_dialogue_status_group_update(void) {
    s32 girl_rank;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    girl_rank = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    switch (girl_rank) {
    case 0:
        break;
    case 1:
    case 2:
        new_year_dialogue_group += 1;
        break;
    case 3:
        new_year_dialogue_group += 2;
        break;
    }
    mode_step_next();
}

void new_year_dialogue_group_select_0x27(void) {
    new_year_dialogue_group = 0x27;
    mode_step_next();
}

void new_year_shrine_invitation_outcome_update(void) {
    if (talk_choice != 0) {
        new_year_dialogue_group += 1;
        game_state.girls[0].tokimeki -= 1;
        game_state.girls[0].affection -= 1;
        game_state.girls[0].syoushin += 0xA;
        new_year_shiori_route_flag = 0;
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else {
        game_state.girls[0].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}

void new_year_shrine_invitation_prompt(void) {
    /* FAKE: two unused slots preserve the original local-buffer placement. T-24523 */
    s32 pad1;
    s32 pad2;
    s32 previous_mode_step;
    u8 sp40[3] = "　";

    /* FAKE: narrowing is redundant after the final s16 cast, but spends the two
     * temporary slots needed by the original register sequence. T-24523 */
    if (game_state.current_girl != GIRL_SHIORI) {
        if (game_state.substep == 0) {
            set_kanji_string((s16)(-0x68 - (s16)(strlen(name_code_family) * 4)), 0x30, 0, (s32)name_code_family, 0);
            set_kanji_string(-0x56, 0x30, 0, (s32)"『 ", 0);
            talk_render(-0x48, 0x30, 0, (s32)"それじゃ、神社に行くか。", 0, "", "", 0);
            new_year_dialogue_group = 0;
            new_year_dialogue_line = 0;
            ((u8 **)new_year_dialogue_command_table)[0][0] = 7;
            ((u8 ***)new_year_dialogue_text_table)[0][0][0] = sp40[0];
            ((u8 ***)new_year_dialogue_text_table)[0][0][1] = sp40[1];
            ((u8 ***)new_year_dialogue_text_table)[0][0][2] = 0;
            game_state.substep = 1;
        }
        previous_mode_step = game_state.mode_step;
        new_year_talk_run_current();
        if (previous_mode_step != game_state.mode_step) {
            mode_stage_next();
        }
    } else {
        mode_step_next();
    }
}

void new_year_dialogue_group_select_0x0f(void) {
    new_year_dialogue_group = 0xF;
    mode_step_next();
}
