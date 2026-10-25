/* walk_home.c: src/ovl/GEKO/801323E0.c:9,178,284,364,454; approach callbacks, future-calendar/birthday topic selection, choice counter changes and queue completion. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801323E0-0x801338F0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 encounter_walk_home_approach_steps;

void encounter_walk_home_approach_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl33 mode_step_callbacks;

    mode_step_callbacks = encounter_walk_home_approach_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

void encounter_walk_home_scripts_load(void) {
    cd_read_sectors(0x24, 0x80197000, 0xAEC8);
    encounter_walk_home_scripts_bind();
    mode_step_next();
}

void encounter_walk_home_audio_load(void) {
    audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[game_state.current_girl], D_800B36C8[game_state.current_girl], D_800B3708[game_state.current_girl]);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_walk_home_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_walk_home_audio_start(void) {
    audio_command(0x200);
    mode_step_next();
}

s32 encounter_walk_home_init(void) {
    game_state.current_girl = game_state.day_events[game_state.day_event_index].girl;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "校門前");
    strcpy(D_800CA1DC, "近所の公園");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    encounter_walk_home_followup_allowed = 0;
    encounter_walk_home_group = 0;
    /* T-9510: this initializer and encounter_walk_home_girl_script_select store a full halfword. */
    *(s16 *)&encounter_walk_home_line = 0;
    talk_names_set(game_state.current_girl);
    encounter_walk_home_scripts_bind();
    encounter_walk_home_text_table = encounter_walk_home_text_section_10;
    encounter_walk_home_command_table = encounter_walk_home_command_section_10;
    encounter_walk_home_voice_table = encounter_walk_home_voice_section_10;
    encounter_walk_home_topic_select();
    if ((encounter_walk_home_topic == 0) && (game_state.current_girl == GIRL_MIRA) && !(rand() & 3)) {
        encounter_walk_home_topic = 0x64;
    }
    if (game_state.girls[game_state.current_girl].syoushin >= 0x4C) {
        encounter_walk_home_high_girl_counter = 1;
    } else {
        encounter_walk_home_high_girl_counter = 0;
    }
    mode_step_next();
    cd_command_push(9, 0, 0);
}

void encounter_walk_home_girl_script_select(void) {
    encounter_walk_home_group = 0;
    /* T-9510: a full halfword store, as in the initializer above (T-16510). */
    *(s16 *)&encounter_walk_home_line = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_1;
        encounter_walk_home_command_table = encounter_walk_home_command_section_1;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_1;
        break;
    case GIRL_MIO:
        encounter_walk_home_text_table = encounter_walk_home_text_section_5;
        encounter_walk_home_command_table = encounter_walk_home_command_section_5;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_5;
        break;
    case GIRL_YUINA:
        encounter_walk_home_text_table = encounter_walk_home_text_section_2;
        encounter_walk_home_command_table = encounter_walk_home_command_section_2;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_2;
        break;
    case GIRL_AYAKO:
        encounter_walk_home_text_table = encounter_walk_home_text_section_3;
        encounter_walk_home_command_table = encounter_walk_home_command_section_3;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_3;
        break;
    case GIRL_SAKI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_9;
        encounter_walk_home_command_table = encounter_walk_home_command_section_9;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_9;
        break;
    case GIRL_YUKARI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_6;
        encounter_walk_home_command_table = encounter_walk_home_command_section_6;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_6;
        break;
    case GIRL_NOZOMI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_7;
        encounter_walk_home_command_table = encounter_walk_home_command_section_7;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_7;
        get_g_zyotai_s(GIRL_NOZOMI);
        break;
    case GIRL_MIRA:
        encounter_walk_home_text_table = encounter_walk_home_text_section_4;
        encounter_walk_home_command_table = encounter_walk_home_command_section_4;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_4;
        break;
    case GIRL_YUKO:
        encounter_walk_home_text_table = encounter_walk_home_text_section_0;
        encounter_walk_home_command_table = encounter_walk_home_command_section_0;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_0;
        break;
    case GIRL_MEGUMI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_8;
        encounter_walk_home_command_table = encounter_walk_home_command_section_8;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_8;
        break;
    case GIRL_YUMI:
        encounter_walk_home_text_table = encounter_walk_home_text_section_11;
        encounter_walk_home_command_table = encounter_walk_home_command_section_11;
        encounter_walk_home_voice_table = encounter_walk_home_voice_section_11;
        break;
    }
    mode_step_next();
}

/* FAKE: s32 fall-through preserves narrowing through v0 (T-9780). */
s32 encounter_walk_home_topic_offer(s16 topic) {
    /* FAKE: place the 22-byte girl_thresholds at sp+0x14 (T-9780). */
    s32 fake_stack_pad;
    EncounterGirlThresholds girl_thresholds;

    girl_thresholds = encounter_walk_home_topic_replace_thresholds;
    if (encounter_walk_home_topic == 0) {
        encounter_walk_home_topic = topic;
    } else if (encounter_walk_home_topic == 10) {
        if ((game_state.frame_count & 7) >= (u32)girl_thresholds.by_girl[game_state.current_girl]) {
            encounter_walk_home_topic = topic;
        }
    } else if (game_state.frame_count & 1) {
        encounter_walk_home_topic = topic;
    }
}

/* FAKE: T-11910 s32 with no return statement: the original leaves girl in $v0 at
 * every exit but the first. A `return girl` makes uopt keep the selector out of $v0
 * (girl in $v1, the record pointer and day in $v0, a `move v0,v1` at the exits). */
s32 encounter_walk_home_birthday_topic_check(void) {
    u32 birthday_word;
    u8 girl_id;
    u8 month;
    EncounterGirlThresholds girl_thresholds;
    EncounterMonthDays month_lengths;
    u32 day; /* its home after the two tables gives the 0x50 frame (T-24709) */

    girl_thresholds = encounter_walk_home_birthday_thresholds;
    month_lengths = encounter_walk_home_birthday_month_days;
    if (game_state.year == 0x60) {
        month_lengths.days_in_month[1] = 29;
    }
    girl_id = game_state.current_girl;
    if (!game_state.girls[girl_id].flags.f.b6) {
        return 0;
    }
    if ((game_state.frame_count & 7) < girl_thresholds.by_girl[girl_id] || encounter_walk_home_topic == 0) {
        birthday_word = game_state.girls[girl_id].birthday.w;
        month = game_state.month;
        day = game_state.day;
        if (month == (birthday_word & 0xF)) {
            if (((birthday_word << 23) >> 27) - day < 21 && ((birthday_word << 23) >> 27) - day != 0) {
                encounter_walk_home_topic = 10;
            }
        } else if (((birthday_word & 0xF) - month + 12) % 12 == 1 &&
                   /* FAKE: T-11910 the no-op `birthday_word = birthday_word` side effect makes cfe emit the
                    * day bits before the table load (the original's ugen temporary order) */
                   month_lengths.days_in_month[(month + 10) % 12] + ((birthday_word = birthday_word) << 23 >> 27) - day < 20) {
            encounter_walk_home_topic = 10;
        }
    }
    /* FAKE: T-11910 keeps girl_id live to the exit so it holds $v0 there and day takes $v1 */
    if (girl_id) {}
}

/* Old-style definition: encounter_walk_home_topic_select calls it with four arguments (it reads two), so
 * no prototype may be in scope for that call; the bytes are the same as with
 * `(s16 arg0, s16 arg1)` (T-25401). */
void encounter_walk_home_calendar_topic_check(calendar_row, day_index)
s16 calendar_row;
s16 day_index;
{
    s32 fake_stack_pad; /* FAKE: unused local that places girl_state at sp+0x33 and the frame at 0x38 (T-3330) */
    u8 girl_state;

    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    /* FAKE: indirect byte reads keep encounter_walk_home_topic in $t7 across the two compares instead of promoting it to $v1 (T-24115) */
    if ((*(u8 *)&encounter_walk_home_topic != 1) && (*(u8 *)&encounter_walk_home_topic != 0x10)) {
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0xC) {
            if (game_state.current_girl == GIRL_SHIORI) {
                if (((u32) ((u16) game_state.player_flags.h >> 0xC) < 6U) || ((u32) (game_state.girls[0].flags.b[2] >> 4) < 6U)) {
                    encounter_walk_home_topic_offer(2);
                }
            } else {
                encounter_walk_home_topic_offer(2);
            }
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 4) {
            encounter_walk_home_topic_offer(6);
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0xB) {
            if ((girl_state < 2U) || (game_state.current_girl == GIRL_YUMI)) {
                encounter_walk_home_topic_offer(7);
            } else {
                encounter_walk_home_topic_offer(8);
            }
        }
        if ((game_state.year == 0x61) && (game_state.month == 9) && ((u8) game_state.day < 0xEU) && (game_state.current_girl == GIRL_YUMI)) {
            encounter_walk_home_topic_offer(8);
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 6) {
            encounter_walk_home_topic_offer(0xB);
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0xE) {
            encounter_walk_home_topic_offer(0xC);
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0x14) {
            encounter_walk_home_topic_offer(0xD);
        }
        /* FAKE: indirect byte reads stop uopt from CSE-sharing unk_03E with the earlier 0x61 compare across calls (T-24115) */
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.month_calendar[calendar_row].marker[day_index + 1] == 2)) {
            encounter_walk_home_topic_offer(0xE);
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.month_calendar[calendar_row].marker[day_index + 1] == 2)) {
            encounter_walk_home_topic_offer(0xF);
        }
        if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0xF) {
            encounter_walk_home_topic_offer(9);
        }
    }
    if ((game_state.month_calendar[calendar_row].marker[day_index + 1] == 5) && (encounter_walk_home_topic != 0x10)) {
        encounter_walk_home_topic = 0;
        encounter_walk_home_topic_offer(1);
    }
    if (game_state.month_calendar[calendar_row].marker[day_index + 1] == 0x15) {
        encounter_walk_home_topic = 0;
        encounter_walk_home_topic_offer(0x10);
    }
}

/* The calls pass the day and month as two extra arguments that encounter_walk_home_calendar_topic_check does
 * not read: the values sit in $a2/$a3, where the original keeps them (T-25401).
 * FAKE: T-25401, the day and month are read into the locals in each arm (a store,
 * so the loads stay plain byte loads for an unprototyped call), while the test
 * reads the globals directly so its sum stays apart from the arms' sums. */
s32 encounter_walk_home_topic_select(void) {
    u8 month;
    u8 day;
    s16 i;
    s32 days_in_month;
    EncounterMonthDays month_lengths;

    month_lengths = encounter_walk_home_scan_month_days;
    if (game_state.year == 0x60) {
        month_lengths.days_in_month[1] = 29;
    }
    encounter_walk_home_topic = 0;
    for (i = 1; i < 20; i++) {
        days_in_month = month_lengths.days_in_month[game_state.month - 1];
        if (days_in_month >= game_state.day + i) {
            day = game_state.day;
            month = game_state.month;
            encounter_walk_home_calendar_topic_check(0, (s16) (day + i - 1), day, month);
        } else {
            day = game_state.day;
            month = game_state.month;
            encounter_walk_home_calendar_topic_check(1, (s16) (day + i - days_in_month - 1), day, month);
        }
    }
    encounter_walk_home_birthday_topic_check();
}

void encounter_walk_home_bg_load(void) {
    bg_read_sub2(0x4032);
    mode_step_next();
}

void encounter_walk_home_high_girl_counter_talk(void) {
    if (encounter_walk_home_high_girl_counter != 0) {
        encounter_walk_home_talk();
        return;
    }
    mode_step_next();
}

void encounter_walk_home_girl_state_group_select(void) {
    u8 girl_state_result;

    girl_state_result = get_g_zyotai_s(game_state.current_girl);
    if (((girl_state_result & 0x7F) == 4) || (encounter_walk_home_high_girl_counter != 0)) {
        encounter_walk_home_group += 0x12;
        portrait_layout_set(4U);
        encounter_walk_home_followup_allowed = 0;
    } else {
        if ((girl_state_result & 0x7F) == 2) {
            encounter_walk_home_group += 6;
        }
        if ((girl_state_result & 0x7F) == 3) {
            encounter_walk_home_group += 0xC;
        }
    }
    mode_step_next();
}

void encounter_walk_home_choice_skip_check(void) {
    u8 girl_state_result;

    girl_state_result = get_g_zyotai_s(game_state.current_girl);
    if (((girl_state_result & 0x7F) == 4) || (encounter_walk_home_high_girl_counter != 0)) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if ((game_state.current_girl == GIRL_NOZOMI) && ((girl_state_result & 0x7F) < 2U)) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

/* FAKE: T-24503, the empty index test controls the saved constant ranges;
 * placing the loop initialization on the do line keeps the base address setup
 * before it. The byte view of the selected girl reverses the branch operands. */
void encounter_walk_home_choice_apply(void) {
    s16 other_girl;
    s32 tokimeki_band;
    s32 counter_random_offset;
    s32 girl_state_result;
    s32 other_counter_base;

    girl_state_result = get_g_zyotai_s(game_state.current_girl);
    if (D_80122CDC != 0) {
        encounter_walk_home_followup_allowed = 0;
        tokimeki_band = ((s16) game_state.girls[game_state.current_girl].tokimeki / 20) & 0xFF;
        counter_random_offset = ((u32) game_state.frame_count % (u32) (tokimeki_band + 1)) & 0xFF;
        if ((u32) (girl_state_result & 0x7F & 0xFF) < 2U) {
            game_state.girls[game_state.current_girl].syoushin = game_state.girls[game_state.current_girl].syoushin + counter_random_offset + 0xA;
        } else {
            game_state.girls[game_state.current_girl].syoushin = game_state.girls[game_state.current_girl].syoushin + counter_random_offset + 6;
        }
        encounter_walk_home_group += 2;
    } else {
        encounter_walk_home_followup_allowed = 1;
        if ((game_state.current_girl == GIRL_SHIORI) || (game_state.current_girl == GIRL_MIRA) || (game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKO) || (other_counter_base = 4, (game_state.current_girl == GIRL_MEGUMI))) {
            other_counter_base = 0;
            if ((u32) (girl_state_result & 0x7F & 0xFF) >= 3U) {
                encounter_walk_home_followup_allowed = 0;
            } else {
                other_counter_base = 4;
            }
        }
        other_girl = 0; do {
            if (other_girl) {}
            if ((((u32) (game_state.girls[other_girl].flags.w << 0x1E) >> 0x1F) != 0) && (other_girl != (*(u8 *)&game_state.current_girl))) {
                tokimeki_band = ((s16) game_state.girls[other_girl].tokimeki / 20) & 0xFF;
                game_state.girls[other_girl].syoushin = ((rand() % (u32) (tokimeki_band + 1)) & 0xFF) + (game_state.girls[other_girl].syoushin + other_counter_base);
            }
            other_girl += 1;
        } while (other_girl < 0xB);
    }
    check_para_limit();
    mode_step_next();
}

void encounter_walk_home_followup_check(void) {
    s32 girl_state_result;
    u8 girl_state;

    girl_state_result = get_g_zyotai_s(game_state.current_girl);
    girl_state = girl_state_result & 0x7F;
    if ((girl_state == 4) || (encounter_walk_home_high_girl_counter != 0) || ((game_state.current_girl == GIRL_NOZOMI) && (girl_state < 2U))) {
        audio_command(0x24);
        audio_command(0x507);
    }
    if (((u8) encounter_walk_home_followup_allowed != 0) && ((encounter_walk_home_topic == 0) || (game_state.girls[game_state.current_girl].flags.w & 1))) {
        mode_step_next();
        return;
    }
    if (((u8) encounter_walk_home_followup_allowed == 0) && (game_state.current_girl == GIRL_NOZOMI) && (girl_state < 2U)) {
        mode_step_next();
        return;
    }
    if ((u8) encounter_walk_home_followup_allowed == 0) {
        if ((girl_state < 2U) && (encounter_walk_home_high_girl_counter == 0)) {
            encounter_walk_home_text_table = encounter_walk_home_text_section_10;
            encounter_walk_home_command_table = encounter_walk_home_command_section_10;
            encounter_walk_home_voice_table = encounter_walk_home_voice_section_10;
            encounter_walk_home_group = game_state.girls[game_state.current_girl].syoushin / 20 + 1;
            if (encounter_walk_home_group >= 6) {
                encounter_walk_home_group = 5;
            }
        }
        mode_step_next();
        return;
    }
    mode_stage_next();
}

void encounter_walk_home_choice_exit(void) {
    u8 girl_state_result;

    girl_state_result = get_g_zyotai_s(game_state.current_girl);
    if (((u32)(girl_state_result & 0x7F) < 2U) && (D_80122CDC != 0)) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_MIRA) || (game_state.current_girl == GIRL_MEGUMI)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    mode_step_next();
}

s32 encounter_walk_home_finish(void) {
    if (game_state.substep == 0) {
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    game_state.girls[game_state.current_girl].affection += 2;
    check_para_limit();
    return day_event_next(1);
}
