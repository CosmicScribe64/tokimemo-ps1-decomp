/* invite.c: src/ovl/GEKO/80142760.c:9,117,391,452,492,533; invitation script binding, destination eligibility/weights, choices, packed appointment creation and Sunday/override rules. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80142760-0x80144710 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/girl_encounters.h"

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 encounter_invite_steps;

void encounter_invite_scripts_bind(void) {
    encounter_invite_text_section_0 = 0x80197558;
    encounter_invite_text_section_1 = 0x80197F60;
    encounter_invite_text_section_2 = 0x80198830;
    encounter_invite_text_section_3 = 0x801991A8;
    encounter_invite_text_section_4 = 0x80199C00;
    encounter_invite_text_section_5 = 0x8019A598;
    encounter_invite_text_section_6 = 0x8019B0D4;
    encounter_invite_text_section_7 = 0x8019BAEC;
    encounter_invite_text_section_8 = 0x8019C4FC;
    encounter_invite_text_section_9 = 0x8019CF64;
    encounter_invite_text_section_10 = 0x8019D970;
    encounter_invite_text_section_11 = 0x8019DF94;
    encounter_invite_text_section_12 = 0x8019E5E4;
    encounter_invite_command_section_0 = 0x80197624;
    encounter_invite_command_section_1 = 0x80198024;
    encounter_invite_command_section_2 = 0x801988FC;
    encounter_invite_command_section_3 = 0x8019926C;
    encounter_invite_command_section_4 = 0x80199CCC;
    encounter_invite_command_section_5 = 0x8019A664;
    encounter_invite_command_section_6 = 0x8019B1A0;
    encounter_invite_command_section_7 = 0x8019BBB8;
    encounter_invite_command_section_8 = 0x8019C5C8;
    encounter_invite_command_section_9 = 0x8019D030;
    encounter_invite_command_section_10 = 0x8019DA3C;
    encounter_invite_command_section_11 = 0x8019DFBC;
    encounter_invite_command_section_12 = 0x8019E6B0;
    encounter_invite_voice_section_0 = 0x801979D0;
    encounter_invite_voice_section_1 = 0x801983B0;
    encounter_invite_voice_section_2 = 0x80198CA8;
    encounter_invite_voice_section_3 = 0x801995F8;
    encounter_invite_voice_section_4 = 0x8019A078;
    encounter_invite_voice_section_5 = 0x8019AA2C;
    encounter_invite_voice_section_6 = 0x8019B54C;
    encounter_invite_voice_section_7 = 0x8019BF64;
    encounter_invite_voice_section_8 = 0x8019C974;
    encounter_invite_voice_section_9 = 0x8019D3DC;
    encounter_invite_voice_section_10 = 0x8019DE04;
    encounter_invite_voice_section_11 = 0x8019E05C;
    encounter_invite_voice_section_12 = 0x8019EA5C;
}

void encounter_invite_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl32 mode_step_callbacks;

    mode_step_callbacks = encounter_invite_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

s32 encounter_invite_finish(void) {
    if (game_state.substep == 0) {
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.mode_step_timer.u++ >= 0x400U) {
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
    return day_event_next(1);
}

void encounter_invite_audio_load(void) {
    u8 *queued_girl = &game_state.day_events[game_state.day_event_index].girl;

    audio_bank_load_start(1, 0xBF98, 0xBF79, D_800B3688[*queued_girl], D_800B36C8[*queued_girl], D_800B3708[*queued_girl]);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_invite_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void encounter_invite_audio_start(void) {
    audio_command(0x200);
    mode_step_next();
}

void encounter_invite_scripts_load(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAF23);
    encounter_invite_scripts_bind();
    mode_step_next();
}

/* FAKE: T-24503, the unused locals pad0, pad1 and pad2 keep the original's stack slots.
 * FAKE: T-11900, the empty tests `if (&game_state) {}` and `if (&flags[i]) {}` at the top of the first
 * loop body set the allocation priorities of the GameState base and of the flag pointer; the packed-appointment
 * compare reads D_800E71DF, which the original reloads with an absolute lui at that test.
 * T-25403: matched; drafts/T-25403/GEKO/encounter_invite_place_select_init/NOTES.md (pass 2) explains the two FAKEs below. */
void encounter_invite_place_select_init(void) {
    s32 fake_leading_pad;
    u8 sunday_day;
    u8 calendar_row;
    u8 sunday_month;
    u8 sunday_year;
    u8 *selected_topic_entry;
    u8 *weight_or_topic_entry;
    u8 selected_place;
    u8 topic_index;
    u8 random_weight;
    u8 sum;
    s16 i;
    u8 fake_array_pad[20];
    s16 j;
    u8 total_weight;
    s16 season_index;
    EncounterGirlInviteWeights girl_topic_weights;
    u8 place_eligible[21];
    s32 fake_trailing_pad;
    EncounterMonthDays month_lengths;
    EncounterGirlSeasonAvailability movie_availability;
    EncounterGirlSeasonAvailability concert_availability;
    EncounterInvitationByteMap place_min_year;
    EncounterInvitationByteMap place_min_month;
    EncounterInviteTopicNames topic_names;
    EncounterPreviousSpotExclusion previous_spot_exclusion;
    EncounterInvitationByteMap place_to_topic;

    girl_topic_weights = encounter_invite_girl_topic_weights;
    month_lengths = encounter_invite_month_days;
    movie_availability = encounter_invite_movie_theater_availability;
    concert_availability = encounter_invite_concert_hall_availability;
    place_min_year = encounter_invite_place_minimum_year;
    place_min_month = encounter_invite_place_minimum_month;
    topic_names = encounter_invite_topic_names;
    previous_spot_exclusion = encounter_invite_previous_place_exclusion;
    place_to_topic = encounter_invite_place_to_topic;
    if (*(u8 *)&game_state.year == 0x60) {
        month_lengths.days_in_month[1] = 0x1D;
    }
    calendar_row = 0;
    if ((u32) month_lengths.days_in_month[*(u8 *)&game_state.month - 1] < (u32) ((*(u8 *)&game_state.day - *(u8 *)&game_state.weekday) + 6)) {
        calendar_row = 1;
    }
    sunday_year = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_YEAR);
    sunday_month = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_MONTH);
    sunday_day = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_DAY);
    *(u8 *)&game_state.current_girl = game_state.day_events[*(u8 *)&game_state.day_event_index].girl;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "廊下");
    event_log_add(9, 0);
    if (sunday_year == 0x5F) {
        if ((sunday_month >= 4U) && (sunday_month < 6U)) {
            season_index = 0;
        } else if ((sunday_month >= 6U) && (sunday_month < 9U)) {
            season_index = 1;
        } else if ((sunday_month >= 9U) && (sunday_month < 0xCU)) {
            season_index = 2;
        } else {
            season_index = 3;
        }
    } else if (sunday_year == 0x60) {
        if ((sunday_month != 0) && (sunday_month < 3U)) {
            season_index = 3;
        } else if ((sunday_month >= 3U) && (sunday_month < 6U)) {
            season_index = 4;
        } else if ((sunday_month >= 6U) && (sunday_month < 9U)) {
            season_index = 5;
        } else if ((sunday_month >= 9U) && (sunday_month < 0xCU)) {
            season_index = 6;
        } else {
            season_index = 7;
        }
    } else if (sunday_year == 0x61) {
        if ((sunday_month != 0) && (sunday_month < 3U)) {
            season_index = 7;
        } else if ((sunday_month >= 3U) && (sunday_month < 6U)) {
            season_index = 8;
        } else if ((sunday_month >= 6U) && (sunday_month < 9U)) {
            season_index = 9;
        } else if ((sunday_month >= 9U) && (sunday_month < 0xCU)) {
            season_index = 10;
        } else {
            season_index = 11;
        }
    } else {
        season_index = 11;
    }
    total_weight = 0;
    for (i = 0; i < 0x15; i++) {
        if (&game_state) {}
        if (&place_eligible[i]) {}
        place_eligible[i] = 0;
        if ((game_state.progress_state[i] == 1) &&
            (sunday_year >= place_min_year.values[i]) &&
            ((sunday_year != place_min_year.values[i]) || (sunday_month >= place_min_month.values[i])) &&
            ((i != 0xE) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 1)) &&
            ((i != 0xF) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 2)) &&
            ((i != 7) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 4)) &&
            ((i != 0xE) || (movie_availability.available[*(u8 *)&game_state.current_girl * 0xC + season_index] != 0)) &&
            ((i != 0xF) || (concert_availability.available[*(u8 *)&game_state.current_girl * 0xC + season_index] != 0)) &&
            ((i != 7) || (((sunday_year != 0x61) || (sunday_month < 9U) || (sunday_month >= 0xCU) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xC] == 0)) &&
                          ((sunday_year != 0x61) || (sunday_month < 3U) || (sunday_month >= 6U) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xD] == 0)) &&
                          ((sunday_year != 0x5F) || (sunday_month < 9U) || (sunday_month >= 0xCU) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xE] == 0)))) &&
            (i != 0) && (i != 1) && (i != 0x12) &&
            ((i != 0xD) || ((sunday_month != 2) && (sunday_month != 0xC))) &&
            ((i != 0xE) || (sunday_year != 0x5F) || (sunday_month >= 6U)) &&
            (((i != 0x10) && (i != 0x11)) || (game_state.month_calendar[calendar_row].day_kind[sunday_day] == 2) || (sunday_month == 7)) &&
            (((i != 0x13) && (i != 0x14)) || (game_state.month_calendar[calendar_row].day_kind[sunday_day] == 2) || (sunday_month == 0xC)) &&
            (i != previous_spot_exclusion.excluded_place[game_state.girl_last_date_spot[*(u8 *)&game_state.current_girl]]) &&
            ((D_800E71DF != (((u32) (*(u32 *)&game_state.unk_54C[0].w * 8)) >> 0x1C)) || (i != (*(u32 *)&game_state.unk_54C[0].w & 0x1F))) &&
            ((*(u8 *)&game_state.current_girl != 1) || (((i != 0xD) || ((sunday_month != 1) && (sunday_month != 3) && (sunday_month != 6) && (sunday_month != 0xB))) && (i != 0x10) && (i != 0x11) && (i != 0x14))) &&
            ((*(u8 *)&game_state.current_girl != 2) || (i != 0x14)) &&
            ((*(u8 *)&game_state.current_girl != 3) || ((i != 0x10) && (i != 0x11))) &&
            ((*(u8 *)&game_state.current_girl != 4) || (i != 7)) &&
            ((*(u8 *)&game_state.current_girl != 5) || (i != 0xD) || ((sunday_month != 1) && (sunday_month != 3) && (sunday_month != 6) && (sunday_month != 0xB))) &&
            ((*(u8 *)&game_state.current_girl != 9) || (((i != 0xD) || ((sunday_month != 1) && (sunday_month != 3) && (sunday_month != 6) && (sunday_month != 0xB))) && (i != 0x14))) &&
            ((*(u8 *)&game_state.current_girl != 0xA) || ((i != 7) && (i != 0xB)))) {
            /* FAKE: T-25403, place_to_topic.v[i] read through topic_names (place_to_topic sits 0x40 below it). The &place_to_topic address
             * constant then has no use in this loop, so uopt gives it $t4 in the second loop only. */
            topic_index = ((u8 *)&topic_names)[i - 0x40];
            weight_or_topic_entry = &girl_topic_weights.weights[*(u8 *)&game_state.current_girl * 0x13];
            if ((sunday_month == 1) || (sunday_month == 3) || (sunday_month == 6) || (sunday_month == 0xB)) {
                if (i == 0xD) {
                    topic_index++;
                }
            }
            total_weight += weight_or_topic_entry[topic_index];
            place_eligible[i] = 1;
        }
    }
    srand(*(u32 *)&game_state.frame_count);
    random_weight = rand() % total_weight;
    total_weight = 0;
    selected_place = 2;
    for (i = 0; i < 0x15; i++) {
        if (place_eligible[i] == 0) {
            continue;
        }
        topic_index = place_to_topic.values[i];
        weight_or_topic_entry = &girl_topic_weights.weights[*(u8 *)&game_state.current_girl * 0x13];
        if ((sunday_month == 1) || (sunday_month == 3) || (sunday_month == 6) || (sunday_month == 0xB)) {
            if (i == 0xD) {
                topic_index++;
            }
        }
        total_weight += weight_or_topic_entry[topic_index];
        if (total_weight >= random_weight) {
            selected_place = i;
            break;
        }
    }
    selected_topic_entry = &place_to_topic.values[selected_place];
    encounter_invite_topic = *selected_topic_entry;
    if ((sunday_month == 1) || (sunday_month == 3) || (sunday_month == 6) || (sunday_month == 0xB)) {
        /* FAKE: T-25403, comparing two variables keeps selected_topic_entry first in the ucode, so uopt keeps
         * &place_to_topic.v[13] as its own constant (addiu t7,sp,0x41) instead of folding it into selected_topic_entry. */
        weight_or_topic_entry = &place_to_topic.values[13];
        if (selected_topic_entry == weight_or_topic_entry) {
            encounter_invite_topic += 1;
        }
    }
    encounter_invite_progress_place_override();
    k_reset(1);
    *(s8 *)&game_state.tile_layer_enabled = 0;
    *(s32 *)&game_state.quads_enabled = 1;
    talk_clear();
    *(u8 *)&game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    encounter_invite_group = 0;
    encounter_invite_line = 0;
    talk_names_set(*(u8 *)&game_state.current_girl);
    encounter_invite_scripts_bind();
    switch (*(u8 *)&game_state.current_girl) {
    case 0:
        encounter_invite_text_table = encounter_invite_text_section_1;
        encounter_invite_command_table = encounter_invite_command_section_1;
        encounter_invite_voice_table = encounter_invite_voice_section_1;
        break;
    case 1:
        encounter_invite_text_table = encounter_invite_text_section_5;
        encounter_invite_command_table = encounter_invite_command_section_5;
        encounter_invite_voice_table = encounter_invite_voice_section_5;
        break;
    case 2:
        encounter_invite_text_table = encounter_invite_text_section_2;
        encounter_invite_command_table = encounter_invite_command_section_2;
        encounter_invite_voice_table = encounter_invite_voice_section_2;
        break;
    case 3:
        encounter_invite_text_table = encounter_invite_text_section_3;
        encounter_invite_command_table = encounter_invite_command_section_3;
        encounter_invite_voice_table = encounter_invite_voice_section_3;
        break;
    case 4:
        encounter_invite_text_table = encounter_invite_text_section_10;
        encounter_invite_command_table = encounter_invite_command_section_10;
        encounter_invite_voice_table = encounter_invite_voice_section_10;
        break;
    case 5:
        encounter_invite_text_table = encounter_invite_text_section_6;
        encounter_invite_command_table = encounter_invite_command_section_6;
        encounter_invite_voice_table = encounter_invite_voice_section_6;
        break;
    case 6:
        encounter_invite_text_table = encounter_invite_text_section_8;
        encounter_invite_command_table = encounter_invite_command_section_8;
        encounter_invite_voice_table = encounter_invite_voice_section_8;
        if (((u8) get_g_zyotai_s(*(u8 *)&game_state.current_girl) & 0x7F) < 2) {
            encounter_invite_text_table = encounter_invite_text_section_7;
            encounter_invite_command_table = encounter_invite_command_section_7;
            encounter_invite_voice_table = encounter_invite_voice_section_7;
        }
        break;
    case 7:
        encounter_invite_text_table = encounter_invite_text_section_4;
        encounter_invite_command_table = encounter_invite_command_section_4;
        encounter_invite_voice_table = encounter_invite_voice_section_4;
        break;
    case 8:
        encounter_invite_text_table = encounter_invite_text_section_0;
        encounter_invite_command_table = encounter_invite_command_section_0;
        encounter_invite_voice_table = encounter_invite_voice_section_0;
        break;
    case 9:
        encounter_invite_text_table = encounter_invite_text_section_9;
        encounter_invite_command_table = encounter_invite_command_section_9;
        encounter_invite_voice_table = encounter_invite_voice_section_9;
        break;
    case 10:
        encounter_invite_text_table = encounter_invite_text_section_12;
        encounter_invite_command_table = encounter_invite_command_section_12;
        encounter_invite_voice_table = encounter_invite_voice_section_12;
        break;
    }
    strcpy(name_code_m, topic_names.by_topic[encounter_invite_topic]);
    mode_step_next();
}

void encounter_invite_corridor_bg_load(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_invite_talk(void) {
    talk_tables = &encounter_invite_group;
    talk_active_line_ptr = &encounter_invite_line;
    talk_active_text_table = encounter_invite_text_table;
    talk_active_command_table = encounter_invite_command_table;
    talk_active_voice_table = encounter_invite_voice_table;
    talk_run(talk_choice, 1, 0);
}

void encounter_invite_choice_talk(void) {
    talk_tables = &encounter_invite_group;
    talk_active_line_ptr = &encounter_invite_line;
    talk_active_text_table = encounter_invite_text_table;
    talk_active_command_table = encounter_invite_command_table;
    talk_active_voice_table = encounter_invite_voice_table;
    talk_run(0xFF, 1, 0);
}

void encounter_invite_first_choice_apply(void) {
    if (talk_choice != 0) {
        encounter_invite_group = 0x17;
        game_state.girls[game_state.current_girl].syoushin += 0xF;
        mode_step_set(0x18);
    } else {
        encounter_invite_group += encounter_invite_topic;
        mode_step_next();
    }
    check_para_limit();
}

void encounter_invite_second_choice_group(void) {
    encounter_invite_group = 0x14;
    mode_step_next();
}

void encounter_invite_second_choice_apply(void) {
    s16 i;
    u8 tokimeki_band;
    u8 counter_random_offset;

    if (talk_choice != 0) {
        if (game_state.current_girl == GIRL_YUINA) {
            encounter_invite_group = 0x18;
        } else {
            encounter_invite_group = 0x17;
        }
        game_state.girls[game_state.current_girl].syoushin += 0x14;
        mode_step_set(0x18);
    } else {
        encounter_invite_group = 0x15;
        for (i = 0; i < 0xB; i++) {
            if (game_state.girls[i].flags.f.met && game_state.current_girl != i) {
                tokimeki_band = game_state.girls[i].tokimeki / 20;
                counter_random_offset = rand() % (u32)(tokimeki_band + 1);
                if ((u8)(get_g_zyotai_s(i) & 0x7F) < 2) {
                    game_state.girls[i].syoushin = game_state.girls[i].syoushin + counter_random_offset + 6;
                } else {
                    game_state.girls[i].syoushin = game_state.girls[i].syoushin + counter_random_offset + 4;
                }
            }
        }
        mode_step_next();
    }
    check_para_limit();
}

void encounter_invite_choice_exit(void) {
    if (talk_choice != 0) {
        if (game_state.current_girl == GIRL_YUINA || game_state.current_girl == GIRL_MIRA || game_state.current_girl == GIRL_MEGUMI || game_state.current_girl == GIRL_YUMI) {
            face_expression_4();
            return;
        }
        face_expression_3();
        return;
    }
    game_state.girls[game_state.current_girl].affection += 1;
    mode_step_next();
}

void encounter_invite_appointment_store(void) {
    u8 selected_place;
    EncounterAppointmentEntry appointment_word;
    EncounterInvitationByteMap topic_to_place;
    s32 fake_stack_pad; /* FAKE: unused local below topic_to_place that sizes the frame to 0x50 while keeping appointment_word at sp+0x48 and topic_to_place at sp+0x30 (T-3330) */

    topic_to_place = encounter_invite_topic_to_place;
    if (talk_choice != 0) {
        encounter_invite_text_table = encounter_invite_text_section_11;
        encounter_invite_command_table = encounter_invite_command_section_11;
        encounter_invite_voice_table = encounter_invite_voice_section_11;
        encounter_invite_group = game_state.girls[game_state.current_girl].syoushin / 20;
        if (encounter_invite_group >= 5) {
            encounter_invite_group = 4;
        }
    } else {
        appointment_word.bits.place = topic_to_place.values[encounter_invite_topic];
        appointment_word.bits.year = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_YEAR);
        appointment_word.bits.month = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_MONTH);
        appointment_word.bits.day = encounter_invite_sunday_component(ENCOUNTER_SUNDAY_DAY);
        appointment_word.bits.unk_21 = 6;
        appointment_word.bits.unk_24 = 1;
        appointment_word.bits.girl = game_state.current_girl;
        appointment_word.bits.active = 1;
        appointment_word.bits.unk_30 = 0;
        hud_appointment_add(appointment_word.word);
        selected_place = topic_to_place.values[encounter_invite_topic];
        if (selected_place == 0xE) {
            game_state.girl_scene_bits[game_state.current_girl] |= 1;
        }
        if (selected_place == 0xF) {
            game_state.girl_scene_bits[game_state.current_girl] |= 2;
        }
        if (selected_place == 7) {
            game_state.girl_scene_bits[game_state.current_girl] |= 4;
        }
    }
    mode_step_next();
}

s32 encounter_invite_sunday_component(s32 calendar_component) {
    s16 days_in_month;
    s32 sunday_components[3];
    EncounterMonthDays month_lengths;
    u8 year;
    u8 month;

    year = game_state.year;
    /* FAKE: T-24602, overwritten initial values and the empty test establish
     * result/calendar_component live ranges. The array also keeps the day calculation
     * in one register; separate scalar results create an extra range. */
    sunday_components[2] = 0;
    sunday_components[1] = 0;
    sunday_components[0] = year;
    if (calendar_component) {}
    month_lengths = encounter_invite_sunday_month_days;
    if (year == 0x60) {
        month_lengths.days_in_month[1] = 29;
    }
    month = game_state.month;
    sunday_components[1] = month;
    sunday_components[2] = game_state.day - game_state.weekday + 6;
    days_in_month = month_lengths.days_in_month[month - 1];
    sunday_components[0] = year;
    if (days_in_month < sunday_components[2]) {
        sunday_components[1] = month + 1;
        sunday_components[2] -= days_in_month;
        if (sunday_components[1] >= 13) {
            sunday_components[1] = 1;
            sunday_components[0] = year + 1;
        }
    }
    if (calendar_component == 0) {
        return sunday_components[0];
    }
    if (calendar_component == 1) {
        return sunday_components[1];
    }
    return sunday_components[2];
}

void encounter_invite_progress_place_override(void) {
    s32 fake_stack_pad; /* FAKE: unused local that moves the table to sp+0x2C and the frame to 0x48 (T-3330 rule, T-23005) */
    EncounterInvitationByteMap place_to_topic;

    place_to_topic = encounter_invite_progress_place_to_topic;
    if ((*(u8 *)&game_state.current_girl == 1) && (*(u8 *)&game_state.year == 0x60) && ((*(u8 *)&game_state.month == 0xC) || (*(u8 *)&game_state.month == 1) || ((*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0xC] == 0)) {
        encounter_invite_topic = place_to_topic.values[0xE];
        game_state.progress_state[0xE] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 3) && (*(u8 *)&game_state.year == 0x61) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0x18] == 0)) {
        encounter_invite_topic = place_to_topic.values[7];
        game_state.progress_state[7] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 4) && ((*(u8 *)&game_state.month == 9) || ((*(u8 *)&game_state.month == 0xA) && (*(u8 *)&game_state.day < 0x15U))) && (*(u8 *)&game_state.year >= 0x60U) && (game_state.scene_flags[0x1E] == 0)) {
        encounter_invite_topic = place_to_topic.values[0xD];
        game_state.progress_state[0xD] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 7) && ((*(u8 *)&game_state.month == 3) || (*(u8 *)&game_state.month == 4) || ((*(u8 *)&game_state.month == 5) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0x2F] == 0)) {
        encounter_invite_topic = place_to_topic.values[2];
        game_state.progress_state[2] = 1;
    }
    if (*(u8 *)&game_state.current_girl == 0xA) {
        if (*(u8 *)&game_state.year >= 0x60U) {
            if ((*(u8 *)&game_state.month == 0xA) && (*(u8 *)&game_state.day < 0x15U) && (game_state.scene_flags[0x47] == 0)) {
                encounter_invite_topic = place_to_topic.values[0xD];
                game_state.progress_state[0xD] = 1;
            }
        }
    }
    if (*(u8 *)&game_state.current_girl == 8) {
        if ((((*(u8 *)&game_state.year == 0x5F) && (*(u8 *)&game_state.month == 0xC)) || ((*(u8 *)&game_state.year == 0x60) && (*(u8 *)&game_state.month == 1)) || ((*(u8 *)&game_state.year == 0x60) && (*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.girls[8].scene_counters[6] == 0)) {
            encounter_invite_topic = place_to_topic.values[3];
            game_state.progress_state[3] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.girls[8].scene_counters[9] == 0) && ((*(u8 *)&game_state.month == 6) || (*(u8 *)&game_state.month == 7))) {
            encounter_invite_topic = place_to_topic.values[5];
            game_state.progress_state[5] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.girls[8].scene_counters[0x12] == 0) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = place_to_topic.values[0xA];
            game_state.progress_state[0xA] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.girls[8].scene_counters[0x13] == 0) && ((*(u8 *)&game_state.month == 3) || (*(u8 *)&game_state.month == 4) || ((*(u8 *)&game_state.month == 5) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = place_to_topic.values[0xB];
            game_state.progress_state[0xB] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.scene_flags[0x39] == 0) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = place_to_topic.values[0xF];
            game_state.progress_state[0xF] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && ((*(u8 *)&game_state.month == 0xC) || (*(u8 *)&game_state.month == 1) || ((*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0xBU)))) {
            encounter_invite_topic = place_to_topic.values[0xC];
            game_state.progress_state[0xC] = 1;
        }
        if (((encounter_invite_sunday_component(ENCOUNTER_SUNDAY_YEAR) == 0x5F) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_MONTH) == 0xC) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_DAY) == 0xA)) || ((encounter_invite_sunday_component(ENCOUNTER_SUNDAY_YEAR) == 0x60) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_MONTH) == 0xC) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_DAY) == 8)) || ((encounter_invite_sunday_component(ENCOUNTER_SUNDAY_YEAR) == 0x61) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_MONTH) == 0xC) && (encounter_invite_sunday_component(ENCOUNTER_SUNDAY_DAY) == 7))) {
            encounter_invite_topic = place_to_topic.values[8];
            game_state.progress_state[8] = 1;
        }
    }
}
