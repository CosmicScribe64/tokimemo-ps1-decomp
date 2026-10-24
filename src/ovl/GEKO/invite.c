/* invite.c: src/ovl/GEKO/80142760.c:9,117,391,452,492,533; invitation script binding, destination eligibility/weights, choices, packed appointment creation and Sunday/override rules. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80142760-0x80144710 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

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
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = encounter_invite_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
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
    u8 *p = &game_state.day_events[game_state.day_event_index].girl;

    audio_bank_load_start(1, 0xBF98, 0xBF79, D_800B3688[*p], D_800B36C8[*p], D_800B3708[*p]);
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
    s32 pad0;
    u8 day;
    u8 spill;
    u8 month;
    u8 year;
    u8 *q;
    u8 *p;
    u8 sel;
    u8 idx;
    u8 r;
    u8 sum;
    s16 i;
    u8 pad1[20];
    s16 j;
    u8 total;
    s16 slot;
    GekoBytes209 weights;
    u8 flags[21];
    s32 pad2;
    GekoMonthDays monthDays;
    GekoBytes132 tblA;
    GekoBytes132 tblB;
    GekoBytes21 minYear;
    GekoBytes21 minMonth;
    GekoText627 names;
    GekoBytes37 banned;
    GekoBytes21 ids;

    weights = encounter_invite_girl_topic_weights;
    monthDays = encounter_invite_month_days;
    tblA = encounter_invite_movie_theater_availability;
    tblB = encounter_invite_concert_hall_availability;
    minYear = encounter_invite_place_minimum_year;
    minMonth = encounter_invite_place_minimum_month;
    names = encounter_invite_topic_names;
    banned = encounter_invite_previous_place_exclusion;
    ids = encounter_invite_place_to_topic;
    if (*(u8 *)&game_state.year == 0x60) {
        monthDays.day[1] = 0x1D;
    }
    spill = 0;
    if ((u32) monthDays.day[*(u8 *)&game_state.month - 1] < (u32) ((*(u8 *)&game_state.day - *(u8 *)&game_state.weekday) + 6)) {
        spill = 1;
    }
    year = encounter_invite_sunday_component(0);
    month = encounter_invite_sunday_component(1);
    day = encounter_invite_sunday_component(2);
    *(u8 *)&game_state.current_girl = game_state.day_events[*(u8 *)&game_state.day_event_index].girl;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
    event_log_add(9, 0);
    if (year == 0x5F) {
        if ((month >= 4U) && (month < 6U)) {
            slot = 0;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 1;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 2;
        } else {
            slot = 3;
        }
    } else if (year == 0x60) {
        if ((month != 0) && (month < 3U)) {
            slot = 3;
        } else if ((month >= 3U) && (month < 6U)) {
            slot = 4;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 5;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 6;
        } else {
            slot = 7;
        }
    } else if (year == 0x61) {
        if ((month != 0) && (month < 3U)) {
            slot = 7;
        } else if ((month >= 3U) && (month < 6U)) {
            slot = 8;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 9;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 10;
        } else {
            slot = 11;
        }
    } else {
        slot = 11;
    }
    total = 0;
    for (i = 0; i < 0x15; i++) {
        if (&game_state) {}
        if (&flags[i]) {}
        flags[i] = 0;
        if ((game_state.progress_state[i] == 1) &&
            (year >= minYear.v[i]) &&
            ((year != minYear.v[i]) || (month >= minMonth.v[i])) &&
            ((i != 0xE) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 1)) &&
            ((i != 0xF) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 2)) &&
            ((i != 7) || !(game_state.girl_scene_bits[*(u8 *)&game_state.current_girl] & 4)) &&
            ((i != 0xE) || (tblA.v[*(u8 *)&game_state.current_girl * 0xC + slot] != 0)) &&
            ((i != 0xF) || (tblB.v[*(u8 *)&game_state.current_girl * 0xC + slot] != 0)) &&
            ((i != 7) || (((year != 0x61) || (month < 9U) || (month >= 0xCU) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xC] == 0)) &&
                          ((year != 0x61) || (month < 3U) || (month >= 6U) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xD] == 0)) &&
                          ((year != 0x5F) || (month < 9U) || (month >= 0xCU) || (game_state.girls[*(u8 *)&game_state.current_girl].scene_counters[0xE] == 0)))) &&
            (i != 0) && (i != 1) && (i != 0x12) &&
            ((i != 0xD) || ((month != 2) && (month != 0xC))) &&
            ((i != 0xE) || (year != 0x5F) || (month >= 6U)) &&
            (((i != 0x10) && (i != 0x11)) || (game_state.month_calendar[spill].day_kind[day] == 2) || (month == 7)) &&
            (((i != 0x13) && (i != 0x14)) || (game_state.month_calendar[spill].day_kind[day] == 2) || (month == 0xC)) &&
            (i != banned.v[game_state.girl_last_date_spot[*(u8 *)&game_state.current_girl]]) &&
            ((D_800E71DF != (((u32) (*(u32 *)&game_state.unk_54C[0].w * 8)) >> 0x1C)) || (i != (*(u32 *)&game_state.unk_54C[0].w & 0x1F))) &&
            ((*(u8 *)&game_state.current_girl != 1) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x10) && (i != 0x11) && (i != 0x14))) &&
            ((*(u8 *)&game_state.current_girl != 2) || (i != 0x14)) &&
            ((*(u8 *)&game_state.current_girl != 3) || ((i != 0x10) && (i != 0x11))) &&
            ((*(u8 *)&game_state.current_girl != 4) || (i != 7)) &&
            ((*(u8 *)&game_state.current_girl != 5) || (i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) &&
            ((*(u8 *)&game_state.current_girl != 9) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x14))) &&
            ((*(u8 *)&game_state.current_girl != 0xA) || ((i != 7) && (i != 0xB)))) {
            /* FAKE: T-25403, ids.v[i] read through names (ids sits 0x40 below it). The &ids address
             * constant then has no use in this loop, so uopt gives it $t4 in the second loop only. */
            idx = ((u8 *)&names)[i - 0x40];
            p = &weights.v[*(u8 *)&game_state.current_girl * 0x13];
            if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
                if (i == 0xD) {
                    idx++;
                }
            }
            total += p[idx];
            flags[i] = 1;
        }
    }
    srand(*(u32 *)&game_state.frame_count);
    r = rand() % total;
    total = 0;
    sel = 2;
    for (i = 0; i < 0x15; i++) {
        if (flags[i] == 0) {
            continue;
        }
        idx = ids.v[i];
        p = &weights.v[*(u8 *)&game_state.current_girl * 0x13];
        if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
            if (i == 0xD) {
                idx++;
            }
        }
        total += p[idx];
        if (total >= r) {
            sel = i;
            break;
        }
    }
    q = &ids.v[sel];
    encounter_invite_topic = *q;
    if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
        /* FAKE: T-25403, comparing two variables keeps q first in the ucode, so uopt keeps
         * &ids.v[13] as its own constant (addiu t7,sp,0x41) instead of folding it into q. */
        p = &ids.v[13];
        if (q == p) {
            encounter_invite_topic += 1;
        }
    }
    encounter_invite_progress_place_override();
    k_reset(1);
    *(s8 *)&game_state.tile_layer_enabled = 0;
    *(s32 *)&game_state.quads_enabled = 1;
    talk_clear();
    *(u8 *)&game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
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
    strcpy(D_800CA280, names.v[encounter_invite_topic]);
    mode_step_next();
}

void encounter_invite_corridor_bg_load(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_invite_talk(void) {
    talk_tables = &encounter_invite_group;
    D_800CA138 = &encounter_invite_line;
    D_800CA13C = encounter_invite_text_table;
    D_800CA140 = encounter_invite_command_table;
    D_800CA144 = encounter_invite_voice_table;
    talk_run(D_80122CDC, 1, 0);
}

void encounter_invite_choice_talk(void) {
    talk_tables = &encounter_invite_group;
    D_800CA138 = &encounter_invite_line;
    D_800CA13C = encounter_invite_text_table;
    D_800CA140 = encounter_invite_command_table;
    D_800CA144 = encounter_invite_voice_table;
    talk_run(0xFF, 1, 0);
}

void encounter_invite_first_choice_apply(void) {
    if (D_80122CDC != 0) {
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
    u8 a;
    u8 b;

    if (D_80122CDC != 0) {
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
                a = game_state.girls[i].tokimeki / 20;
                b = rand() % (u32)(a + 1);
                if ((u8)(get_g_zyotai_s(i) & 0x7F) < 2) {
                    game_state.girls[i].syoushin = game_state.girls[i].syoushin + b + 6;
                } else {
                    game_state.girls[i].syoushin = game_state.girls[i].syoushin + b + 4;
                }
            }
        }
        mode_step_next();
    }
    check_para_limit();
}

void encounter_invite_choice_exit(void) {
    if (D_80122CDC != 0) {
        if (game_state.current_girl == GIRL_YUINA || game_state.current_girl == GIRL_MIRA || game_state.current_girl == GIRL_MEGUMI || game_state.current_girl == GIRL_YUMI) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    game_state.girls[game_state.current_girl].affection += 1;
    mode_step_next();
}

void encounter_invite_appointment_store(void) {
    u8 kind;
    GekoDateEntry entry;
    GekoBytes21 tbl;
    s32 pad; /* FAKE: unused local below tbl that sizes the frame to 0x50 while keeping entry at sp+0x48 and tbl at sp+0x30 (T-3330) */

    tbl = encounter_invite_topic_to_place;
    if (D_80122CDC != 0) {
        encounter_invite_text_table = encounter_invite_text_section_11;
        encounter_invite_command_table = encounter_invite_command_section_11;
        encounter_invite_voice_table = encounter_invite_voice_section_11;
        encounter_invite_group = game_state.girls[game_state.current_girl].syoushin / 20;
        if (encounter_invite_group >= 5) {
            encounter_invite_group = 4;
        }
    } else {
        entry.bits.place = tbl.v[encounter_invite_topic];
        entry.bits.year = encounter_invite_sunday_component(0);
        entry.bits.month = encounter_invite_sunday_component(1);
        entry.bits.day = encounter_invite_sunday_component(2);
        entry.bits.unk_21 = 6;
        entry.bits.unk_24 = 1;
        entry.bits.girl = game_state.current_girl;
        entry.bits.active = 1;
        entry.bits.unk_30 = 0;
        hud_appointment_add(entry.word);
        kind = tbl.v[encounter_invite_topic];
        if (kind == 0xE) {
            game_state.girl_scene_bits[game_state.current_girl] |= 1;
        }
        if (kind == 0xF) {
            game_state.girl_scene_bits[game_state.current_girl] |= 2;
        }
        if (kind == 7) {
            game_state.girl_scene_bits[game_state.current_girl] |= 4;
        }
    }
    mode_step_next();
}

s32 encounter_invite_sunday_component(s32 selector) {
    s16 days_in_month;
    s32 date[3];
    GekoMonthDays days;
    u8 year;
    u8 month;

    year = game_state.year;
    /* FAKE: T-24602, overwritten initial values and the empty test establish
     * result/selector live ranges. The array also keeps the day calculation
     * in one register; separate scalar results create an extra range. */
    date[2] = 0;
    date[1] = 0;
    date[0] = year;
    if (selector) {}
    days = encounter_invite_sunday_month_days;
    if (year == 0x60) {
        days.day[1] = 29;
    }
    month = game_state.month;
    date[1] = month;
    date[2] = game_state.day - game_state.weekday + 6;
    days_in_month = days.day[month - 1];
    date[0] = year;
    if (days_in_month < date[2]) {
        date[1] = month + 1;
        date[2] -= days_in_month;
        if (date[1] >= 13) {
            date[1] = 1;
            date[0] = year + 1;
        }
    }
    if (selector == 0) {
        return date[0];
    }
    if (selector == 1) {
        return date[1];
    }
    return date[2];
}

void encounter_invite_progress_place_override(void) {
    s32 pad; /* FAKE: unused local that moves the table to sp+0x2C and the frame to 0x48 (T-3330 rule, T-23005) */
    GekoBytes21 tbl;

    tbl = encounter_invite_progress_place_to_topic;
    if ((*(u8 *)&game_state.current_girl == 1) && (*(u8 *)&game_state.year == 0x60) && ((*(u8 *)&game_state.month == 0xC) || (*(u8 *)&game_state.month == 1) || ((*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0xC] == 0)) {
        encounter_invite_topic = tbl.v[0xE];
        game_state.progress_state[0xE] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 3) && (*(u8 *)&game_state.year == 0x61) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0x18] == 0)) {
        encounter_invite_topic = tbl.v[7];
        game_state.progress_state[7] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 4) && ((*(u8 *)&game_state.month == 9) || ((*(u8 *)&game_state.month == 0xA) && (*(u8 *)&game_state.day < 0x15U))) && (*(u8 *)&game_state.year >= 0x60U) && (game_state.scene_flags[0x1E] == 0)) {
        encounter_invite_topic = tbl.v[0xD];
        game_state.progress_state[0xD] = 1;
    }
    if ((*(u8 *)&game_state.current_girl == 7) && ((*(u8 *)&game_state.month == 3) || (*(u8 *)&game_state.month == 4) || ((*(u8 *)&game_state.month == 5) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.scene_flags[0x2F] == 0)) {
        encounter_invite_topic = tbl.v[2];
        game_state.progress_state[2] = 1;
    }
    if (*(u8 *)&game_state.current_girl == 0xA) {
        if (*(u8 *)&game_state.year >= 0x60U) {
            if ((*(u8 *)&game_state.month == 0xA) && (*(u8 *)&game_state.day < 0x15U) && (game_state.scene_flags[0x47] == 0)) {
                encounter_invite_topic = tbl.v[0xD];
                game_state.progress_state[0xD] = 1;
            }
        }
    }
    if (*(u8 *)&game_state.current_girl == 8) {
        if ((((*(u8 *)&game_state.year == 0x5F) && (*(u8 *)&game_state.month == 0xC)) || ((*(u8 *)&game_state.year == 0x60) && (*(u8 *)&game_state.month == 1)) || ((*(u8 *)&game_state.year == 0x60) && (*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0x15U))) && (game_state.girls[8].scene_counters[6] == 0)) {
            encounter_invite_topic = tbl.v[3];
            game_state.progress_state[3] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.girls[8].scene_counters[9] == 0) && ((*(u8 *)&game_state.month == 6) || (*(u8 *)&game_state.month == 7))) {
            encounter_invite_topic = tbl.v[5];
            game_state.progress_state[5] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.girls[8].scene_counters[0x12] == 0) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = tbl.v[0xA];
            game_state.progress_state[0xA] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.girls[8].scene_counters[0x13] == 0) && ((*(u8 *)&game_state.month == 3) || (*(u8 *)&game_state.month == 4) || ((*(u8 *)&game_state.month == 5) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = tbl.v[0xB];
            game_state.progress_state[0xB] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.scene_flags[0x39] == 0) && ((*(u8 *)&game_state.month == 9) || (*(u8 *)&game_state.month == 0xA) || ((*(u8 *)&game_state.month == 0xB) && (*(u8 *)&game_state.day < 0x15U)))) {
            encounter_invite_topic = tbl.v[0xF];
            game_state.progress_state[0xF] = 1;
        }
        if ((*(u8 *)&game_state.year == 0x61) && ((*(u8 *)&game_state.month == 0xC) || (*(u8 *)&game_state.month == 1) || ((*(u8 *)&game_state.month == 2) && (*(u8 *)&game_state.day < 0xBU)))) {
            encounter_invite_topic = tbl.v[0xC];
            game_state.progress_state[0xC] = 1;
        }
        if (((encounter_invite_sunday_component(0) == 0x5F) && (encounter_invite_sunday_component(1) == 0xC) && (encounter_invite_sunday_component(2) == 0xA)) || ((encounter_invite_sunday_component(0) == 0x60) && (encounter_invite_sunday_component(1) == 0xC) && (encounter_invite_sunday_component(2) == 8)) || ((encounter_invite_sunday_component(0) == 0x61) && (encounter_invite_sunday_component(1) == 0xC) && (encounter_invite_sunday_component(2) == 7))) {
            encounter_invite_topic = tbl.v[8];
            game_state.progress_state[8] = 1;
        }
    }
}
