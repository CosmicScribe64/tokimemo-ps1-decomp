/* scheduler.c: original object text 0x8006CB30-0x800737A0 in config/objects/main.txt; matched C implements weekly queue construction/dispatch, calendar advancement, encounter selection and health checks; original object boundary unchanged. Original object 0x8006CB30-0x800737A0 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_girl_scores /* this object calls it as void (no v0 result in the call block, schedule_miharu_scene4_ready); main_api.h has s32 */
#define MAIN_API_OVERRIDE_message_window_set_title_hidden /* the original callers pass a string the callee ignores; main_api.h has (void) */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game/player_flags.h"
#include "game/schedule_records.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/day_event.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/memory_card.h"
#include "main/message.h"
#include "main/schedule.h"
#include "main/school_day.h"
#include "main/speaker.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/schedule.h"
#include "main/day_event.h"
#include "main/game_mode.h"
#include "game/word_views.h"

void message_window_set_title_hidden();
void girl_scores();

/* Mode 0x30 handler: on stage 0 runs the schedule build steps, then draws the HUD set */
void schedule_init(void) {
    if (game_state.mode_stage == 0) {
        _schedule_init();
    }
    dec_bg_draw(get_last_gamen_mode());
}

/* Run the schedule step for the current step number (prepare, build week queue, resolve encounters, weekly girl decay, advance day) */
void _schedule_init(void) {
    switch (game_state.mode_step) {
    case 0:
        schedule_step_prepare();
        break;
    case 1:
        day_event_queue_build_week();
        break;
    case 2:
        schedule_girls_gakko();
        break;
    case 3:
        schdeule_girls_param();
        break;
    case 4:
        schedule_step_advance_day();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    hud_screen_frame_draw(1);
}

/* Once per weekly queue build, decrement each nonzero date-spot timer for girls 0-10 */
void last_date_spot_timer_dec(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (game_state.girl_last_date_spot_timer[i] != 0) {
            game_state.girl_last_date_spot_timer[i] -= 1;
        }
    }
}

/* If no BGM is active, start sound bank 0, sequence 0 */
void restore_bgm(void) {
    if (audio_bgm_is_playing() == 0) {
        audio_bank_start(0, 0);
    }
}

void schedule_step_prepare(void) {
    switch (game_state.substep) {
    case 0:
        if (game_state.day_events[game_state.day_event_index].type != DAY_EVENT_EXAM) {
            restore_bgm();
        }
        message_window_title_show(0);
        last_date_spot_timer_dec();
        game_state.substep += 1;
        break;
    case 1:
        if (schedule_memory_card_save_poll() != 0) {
            game_state.substep += 1;
        }
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 schedule_memory_card_save_poll(void) {
    if (game_state.memory_card_job == 1) {
        if (memory_card_system_write_poll() == 0x13) {
            audio_command(0x50F);
            game_state.memory_card_job = 0;
            memory_card_stop();
        } else if (game_state.memory_card_timer >= 0x201U || game_state.memory_card_result == 0xF0) {
            audio_command(0x502);
            game_state.memory_card_job = 0;
            memory_card_stop();
        }
    } else if (game_state.memory_card_job == 0) {
        game_state.memory_card_busy = 0;
        return 1;
    }
    return 0;
}

void schedule_command_icons_lock_to_record(void) {
    s32 i;

    for (i = 0; i != 0xE; i++) {
        if (i != game_state.day_events[game_state.day_event_index].girl) {
            icon_can_use_set(i, 0);
        } else {
            icon_can_use_set(i, 1);
        }
    }
    if ((u8)game_state.icon_mode == ICON_MODE_FREE && (game_state.last_command.w & 0xF) != 0xEU) {
        icon_can_use_set(0xE, 0);
    }
}

void day_event_queue_clear(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        *(s32 *)&game_state.day_events[i] = 0;
        game_state.day_events[i].type = DAY_EVENT_NONE;
    }
    game_state.day_event_index = 0;
}

void girl_flags_bit0_clear(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        game_state.girls[i].flags.b[0] &= 0xFFFE;
    }
}

void schedule_amusement_park_flag_reset(void) {
    if (game_state.month == 9) {
        game_state.player_flags_upper.b[0] &= 0xFFFB;
    }
}

void schedule_girl_spots_expire(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (game_state.girl_last_date_spot[i] == 0x23 || game_state.girl_last_date_spot[i] == 0x24) {
            game_state.girl_last_date_spot[i] = 0;
        }
        game_state.girl_scene_bits[i] = 0;
    }
}

void schedule_progress_state_update(void) {
    GameState *gs = &game_state;
    u32 flags;

    /* FAKE: T-25040, the empty entry switch and flag/base test preserve the entry load.
     * The empty year checks below retain the original branch layout. */
    switch (gs->month) { default: break; }
    flags = *(u32 *)gs->quarter_flags;
    if (flags | (u32)gs) {}

    switch (gs->month) {
    case 6:
    case 7:
        if (gs->year == 0x5F) {
            if (((QuarterFlagBits *)&flags)->b1) {
                gs->progress_state[1] = 1;
            }
            gs->progress_state[0xE] = 1;
        }
        if ((gs->year == 0x60) && (((QuarterFlagBits *)gs->quarter_flags)->b5)) {
            gs->progress_state[5] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 9:
    case 10:
        if ((gs->year == 0x5F) && (((QuarterFlagBits *)&flags)->b2)) {
            gs->progress_state[7] = 1;
        }
        if ((gs->year == 0x60) && (((QuarterFlagBits *)gs->quarter_flags)->b6)) {
            gs->progress_state[0xA] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 1:
        if ((gs->year == 0x60) && (((QuarterFlagBits *)&flags)->b3)) {
            gs->progress_state[3] = 1;
        }
        if (gs->year == 0x61) {
            /* FAKE: T-25040, this zero offset folds after uopt, retaining the base-relative byte load. */
            if (((QuarterFlagBits *)(gs->quarter_flags + ((u32)gs->month < 0)))->b7) {
                gs->progress_state[6] = 1;
            }
        }
        /* FAKE: T-25040, the late zero keeps constant 98 out of the allocator's interference graph. */
        if (gs->year == 0x62 + ((u32)gs->month < 0)) {
        }
        break;
    case 12:
        if ((gs->year == 0x5F) && (((QuarterFlagBits *)&flags)->b3)) {
            gs->progress_state[3] = 1;
        }
        if ((gs->year == 0x60) && (((QuarterFlagBits *)gs->quarter_flags)->b7)) {
            gs->progress_state[6] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 3:
    case 4:
        if ((gs->year == 0x60) && (((QuarterFlagBits *)&flags)->b4)) {
            gs->progress_state[0xD] = 1;
        }
        if (gs->year == 0x61) {
            /* FAKE: T-25040, the dead byte view occupies uopt's static alias; the late-zero offset
             * below keeps the real load base-relative. Both comparisons are false; no extra load emits. */
            if ((u32)gs->quarter_flags[1] < 0) {}
            if ((gs->quarter_flags + ((u32)gs->month < 0))[1] & 1) {
                gs->progress_state[0xB] = 1;
            }
        }
        break;
    }
    switch (gs->month) {
    case 5:
        if ((gs->year == 0x60) && !(((QuarterFlagBits *)gs->quarter_flags)->b4)) {
            gs->progress_state[0xD] = 0x80;
        }
        if ((gs->year == 0x61) && !(gs->quarter_flags[1] & 1)) {
            gs->progress_state[0xB] = 0x80;
            return;
        }
        return;
    case 8:
        if ((gs->year == 0x5F) && !(((QuarterFlagBits *)gs->quarter_flags)->b1)) {
            gs->progress_state[1] = 0x80;
        }
        if ((gs->year == 0x60) && !(((QuarterFlagBits *)gs->quarter_flags)->b5)) {
            gs->progress_state[5] = 0x80;
        }
        if (gs->year == 0x61) {
            return;
        }
        break;
    case 11:
        gs->quarter_flags[1] &= 0xFF7F;
        if ((gs->year == 0x5F) && !(((QuarterFlagBits *)gs->quarter_flags)->b2)) {
            gs->progress_state[7] = 0x80;
        }
        if ((gs->year == 0x60) && !(((QuarterFlagBits *)gs->quarter_flags)->b6)) {
            gs->progress_state[0xA] = 0x80;
        }
        if (gs->year == 0x61) {
            return;
        }
        break;
    case 2:
        if ((gs->year == 0x60) && !(((QuarterFlagBits *)gs->quarter_flags)->b3)) {
            gs->progress_state[3] = 0x80;
        }
        if ((gs->year == 0x61) && (!((QuarterFlagBits *)gs->quarter_flags)->b7)) {
            gs->progress_state[6] = 0x80;
        }
        break;
    }
}


void schedule_club_flag0_clear(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        game_state.clubs[i].flags &= 0xFFFE;
    }
}

void day_event_queue_push_birthdays(s32 *queue_index, s32 month_offset, s32 day) {
    s32 i;

    if (birth_day_check_days(GIRL_SHIORI, game_state.month + month_offset, day) == 1) {
        game_state.day_events[*queue_index].type = DAY_EVENT_GIRL_BIRTHDAY;
        game_state.day_events[*queue_index].girl = 0;
        *queue_index += 1;
    }
    for (i = 1; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b6) {
            if (birth_day_check_days(i, game_state.month + month_offset, day) == 1) {
                game_state.day_events[*queue_index].type = DAY_EVENT_GIRL_BIRTHDAY;
                game_state.day_events[*queue_index].girl = i;
                *queue_index += 1;
            }
        }
    }
    if ((game_state.player_flags.b[1] & 0xF) != 3) {
        if (birth_day_check_days(GIRL_PLAYER, game_state.month + month_offset, day) == 1) {
            game_state.day_events[*queue_index].type = DAY_EVENT_PLAYER_BIRTHDAY;
            *queue_index += 1;
        }
    }
}

/* s32 with no return (T-17004): $v0 is live at the exit of an int function, so as1 keeps
 * the second loop's start-index copy (move v1,v0) instead of folding it into the sll. */
s32 day_event_queue_resync_after_master(void) {
    GameState *gs;
    s32 n;
    s32 i;

    gs = &game_state;
    n = game_state.weekday;
    for (i = 0; i < 32; i++) {
        if (game_state.day_events[i].type == DAY_EVENT_DAY_END) {
            if (n == 0) {
                gs->day_event_index = i;
                if (game_state.day_events[i - 1].type != DAY_EVENT_CLUB_MASTER_A) {
                    game_state.day_event_index--;
                    if (game_state.day_events[i - 2].type != DAY_EVENT_CLUB_MASTER_A) {
                        game_state.day_event_index--;
                    }
                }
                break;
            }
            n--;
        }
    }
    if (gs->day_event_index != 0) {
        gs->day_event_index--;
    }
    for (i = gs->day_event_index; i < 32; i++) {
        if (game_state.day_events[i].type == DAY_EVENT_CLUB_MASTER_A) {
            game_state.day_events[i].type = DAY_EVENT_SCHOOL_DAY;
        }
        if (game_state.day_events[i].type == DAY_EVENT_NONE) {
            break;
        }
    }
}

/* T-23003: the 3-bit counter in bits 5..7 of the byte at GameState +0xF6 (read with lbu, decremented as a bit-field). */
typedef struct RecoveryCounterBits {
    u32 pad0 : 5;
    u32 f : 3;
    u32 pad1 : 24;
} RecoveryCounterBits;

/* FAKE: T-23003, u8 view of hud_command_roll_poor_result (defined s32 in 800674B0.c, not in main_api.h): the second result is then copied into $a0 with or in the beqz delay slot, as in the original, instead of narrowed with andi */
u8 hud_command_roll_poor_result();

void day_event_queue_build_week(void) {
    GameState *gs;
    /* FAKE: T-23003, the original frame has seven register locals (two words above idx); res holds the second call result in $a0 */
    s32 res;
    s32 idx;
    s32 wd;
    s32 month;
    s32 monthDays;
    s32 day;
    s32 monthOff;
    s32 sel;
    s32 cnt;
    s32 j;
    s32 nNo;
    s32 nYes;
    s32 r;
    s32 nRand;
    u8 argA;
    u8 argB;
    u8 nSpecial;

    gs = &game_state;
    nRand = 0;
    nSpecial = 0;
    argA = 0;
    argB = 0;
    nNo = 0;
    nYes = 0;
    gs->progress_state[0x15 + (game_state.last_command.w & 0xF) % 9U] += 1;
    if (game_state.girls[11].scene_counters[0] != 0) {
        game_state.girls[11].scene_counters[0] -= 1;
    }
    sel = schedule_command_girl_select(gs->last_command.w & 0xF) & 0xF;
    for (j = 1, cnt = 0; j < 10; j++) {
        if (game_state.girls[j].flags.f.met) {
            cnt++;
        }
    }
    schedule_last_picked_girl = -1;
    day = game_state.day;
    monthOff = 0;
    month = game_state.month;
    monthDays = gs->month_calendar[0].day_kind[0];
    if (((RecoveryCounterBits *)&gs->player_flags_upper)->f != 0) {
        ((RecoveryCounterBits *)&gs->player_flags_upper)->f -= 1;
    }
    day_event_queue_clear();
    wd = (game_state.weekday + 1) % 7U;
    /* FAKE: T-25036, unsigned < 0 is zero, but folds after uopt so idx keeps a register dependency. */
    idx = (u32)game_state.current_girl < 0;
    if (day == monthDays) {
        game_state.day_events[0].type = DAY_EVENT_MONTH_START;
        /* FAKE: T-25036, this unreachable identity clears as1's zero substitution; it emits no code. */
        if (idx) { idx = idx + ((u32)day < 0); }
        idx++;
        day = 0;
        monthOff = 1;
    }
    day++;
    if (gs->month_calendar[0].marker[day] == 0xB) {
        gs->day_events[idx].type = DAY_EVENT_SCHOOL_TRIP;
        idx++;
        gs->day_events[idx].type = DAY_EVENT_HOLIDAY;
        idx++;
        day_event_queue_push_birthdays(&idx, monthOff, day + 6);
        mode_step_next();
        return;
    }
    if (gs->month_calendar[0].marker[day] == 8) {
        /* FAKE: T-23003, the unsigned constant is a ucode constant of its own and takes $a1 as in the original */
        if ((gs->player_flags.h >> 12) != 0xB && ((u32)gs->last_command.w & 0xF) == 4U) {
            if ((gs->player_flags.h >> 12) == CLUB_BASEBALL && (*(u16 *)&game_state.clubs[CLUB_BASEBALL].level & 0xF) >= 8) {
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 1);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 2);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 3);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 4);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = DAY_EVENT_CLUB_MASTER_A;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 5);
                gs->day_events[idx].type = DAY_EVENT_DAY_END;
                idx++;
                gs->day_events[idx].type = DAY_EVENT_HOLIDAY;
                idx++;
                day_event_queue_push_birthdays(&idx, monthOff, day + 6);
                mode_step_next();
                return;
            }
            for (j = 0; j < 6; j++) {
                gs->day_events[idx].type = DAY_EVENT_CLUB_CAMP;
                gs->day_events[idx].girl = 4;
                gs->day_events[idx].extra = sel;
                gs->day_events[idx].scene = hud_command_roll_poor_result(wd);
                idx++;
            }
            gs->day_events[idx].type = DAY_EVENT_HOLIDAY;
            idx++;
            day_event_queue_push_birthdays(&idx, monthOff, day + 6);
            mode_step_next();
            return;
        }
        if ((gs->player_flags.h >> 12) == 0xB) {
            ((ClubFlagBits *)&gs->clubs[11])->club_progress_grade += 1;
        }
    }
    while (wd < 7) {
        if (monthDays < day) {
            gs->day_events[idx].type = DAY_EVENT_MONTH_START;
            idx++;
            day = 1;
            monthOff = 1;
        }
        if (gs->month_calendar[monthOff].marker[day] == 6) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0xE) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0x14) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0xA) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0x10) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 2) {
            gs->day_events[idx++].type = DAY_EVENT_DAY_NOTICE;
        }
        if (gs->month_calendar[monthOff].marker[day] == 4) {
            gs->day_events[idx++].type = DAY_EVENT_SPORTS_FESTIVAL;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xC) {
            gs->day_events[idx++].type = DAY_EVENT_CULTURE_FESTIVAL;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xD) {
            gs->day_events[idx++].type = DAY_EVENT_CHRISTMAS_PREP;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xF) {
            gs->day_events[idx++].type = DAY_EVENT_NEW_YEAR_PREP;
        } else if (gs->month_calendar[monthOff].marker[day] == 0x11) {
            gs->day_events[idx++].type = DAY_EVENT_VALENTINE;
        } else if (gs->month_calendar[monthOff].marker[day] == 0x13) {
            gs->day_events[idx++].type = DAY_EVENT_SEASONAL_PARTY;
        } else if (gs->month_calendar[monthOff].marker[day] == 2 && game_state.year == 0x60) {
            game_state.girls[10].syoushin = 0;
            game_state.girls[10].affection = 0x32;
            gs->day_events[idx].girl = 0xA;
            gs->day_events[idx].type = DAY_EVENT_MEETING;
            idx++;
            schedule_club_flag0_clear();
        } else if (gs->month_calendar[monthOff].marker[day] == 2 && game_state.year == 0x61) {
            gs->day_events[idx++].type = DAY_EVENT_CAREER_GUIDANCE;
            schedule_club_flag0_clear();
        } else if (gs->month_calendar[monthOff].day_kind[day] == 1 || gs->month_calendar[monthOff].day_kind[day] == 2) {
            if (schedule_miharu_scene4_ready() != 0) {
                game_state.scene_flags[77]++;
                gs->day_events[idx].type = DAY_EVENT_SPECIAL_EVENT;
                gs->day_events[idx].girl = 0xB;
                gs->day_events[idx].scene = 4;
                idx++;
            } else {
                gs->day_events[idx++].type = DAY_EVENT_HOLIDAY;
            }
        } else if (game_state.year == 0x61 && (month + monthOff) == 0xD && day == 3) {
            if (girl_player_sports_club_check() == 1 && (gs->player_flags.h >> 12) != 6 && (*(u16 *)&gs->clubs[gs->player_flags.h >> 12].level & 0xF) >= 0xA) {
                gs->day_events[idx++].type = DAY_EVENT_CLUB_MASTER_B;
            } else {
                if ((gs->player_flags.h >> 12) < 6 && gs->clubs[gs->player_flags.h >> 12].practice_points >= 0x17C) {
                    gs->day_events[idx++].type = DAY_EVENT_CLUB_MASTER_C;
                } else {
                    gs->day_events[idx].type = DAY_EVENT_SCHOOL_DAY;
                    gs->day_events[idx].girl = game_state.last_command.w & 0xF;
                    gs->day_events[idx].extra = sel;
                    if (hud_command_roll_poor_result(wd) != 0) {
                        nYes++;
                    } else {
                        nNo++;
                    }
                    idx++;
                }
            }
        } else if (gs->month_calendar[monthOff].marker[day] == 0x12 && *(u8 *)((u8 *)&game_state + 0x3E) == 0x62) {
            /* FAKE: T-23003, the original reloads the byte at +0x3E here; a plain game_state.year is CSE'd with the 0x61 test into $a1 and every later temporary shifts (first-symbol view, aggregate not known) */
            gs->day_events[idx++].type = DAY_EVENT_CAREER_CHOICE;
        } else if (gs->month_calendar[monthOff].day_kind[day] == 0 || gs->month_calendar[monthOff].day_kind[day] == 3) {
            if (gs->month_calendar[monthOff].marker[day] == 5 || game_state.day_events[0].type == DAY_EVENT_EXAM) {
                gs->day_events[idx].type = DAY_EVENT_EXAM;
                idx++;
            } else {
                gs->day_events[idx].type = DAY_EVENT_SCHOOL_DAY;
                gs->day_events[idx].girl = game_state.last_command.w & 0xF;
                gs->day_events[idx].extra = sel;
                res = hud_command_roll_poor_result(wd);
                if (res != 0) {
                    nYes++;
                } else {
                    nNo++;
                }
                if ((game_state.last_command.w & 0xF) == 7) {
                    gs->day_events[idx].scene = 0;
                } else {
                    gs->day_events[idx].scene = res;
                }
                idx++;
                if (gs->day_events[idx - 1].scene == 1 && (nYes + nNo) == 6 && gs->month_calendar[monthOff].day_kind[day] == 0 && (rand() & 0xF) < 2) {
                    gs->day_events[idx].type = DAY_EVENT_SICK_VISIT;
                    gs->day_events[idx].girl = 0xC;
                    idx++;
                }
                if (gs->month_calendar[monthOff].day_kind[day] == 3) {
                    if (game_state.vacation == VACATION_WINTER && game_state.scene_flags[73] < (u16)game_state.day_count / 365U && game_state.girls[10].flags.f.met && !(get_g_zyotai_h(GIRL_YUMI) & 0x7F) && (rand() & 0xF) <= 0) {
                        gs->day_events[idx].type = DAY_EVENT_SPECIAL_EVENT;
                        gs->day_events[idx].girl = 0xA;
                        gs->day_events[idx].scene = 7;
                        idx++;
                    }
                    if (game_state.vacation == VACATION_SUMMER && wd == 2 && (monthOff + game_state.month) == 8 && (rand() % 100) < 0x14 && cnt != 0
                        && (!((ScheduleGirlFlagBits *)&gs->player_flags)->b18 || (game_state.year == 0x61 && !((ScheduleGirlFlagBits *)&gs->player_flags)->b18 && day == 0x1C))) {
                        ((ScheduleGirlFlagBits *)&game_state.player_flags)->b18 = 1;
                        gs->day_events[idx].type = DAY_EVENT_AMUSEMENT_PARK;
                        idx++;
                    }
                }
            }
            if (gs->month_calendar[monthOff].day_kind[day] == 0) {
                if (game_state.day_events[0].type != DAY_EVENT_EXAM) {
                    if (game_state.year != 0x5F || game_state.month != 4) {
                        if (game_state.day_count < 0x2D6 && nSpecial == 0) {
                            if (cnt < 2) {
                                if (schedule_meeting_select_catchup(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->day_events[idx].type = DAY_EVENT_MEETING;
                                    gs->day_events[idx].girl = argA;
                                    gs->day_events[idx].scene = argB;
                                    idx++;
                                }
                            } else {
                                if (schedule_meeting_select_normal(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->day_events[idx].type = DAY_EVENT_MEETING;
                                    gs->day_events[idx].girl = argA;
                                    gs->day_events[idx].scene = argB;
                                    idx++;
                                }
                            }
                            if ((game_state.last_command.w & 0xF) == 4 && schedule_meeting_select_club(&argA, &argB) != 0) {
                                nSpecial++;
                                gs->day_events[idx].type = DAY_EVENT_MEETING;
                                gs->day_events[idx].girl = argA;
                                gs->day_events[idx].scene = argB;
                                idx++;
                            }
                        }
                    }
                    if (nRand < 2) {
                        if ((rand() & 3) == 3) {
                            nRand++;
                            gs->day_events[idx].type = DAY_EVENT_GIRL_ENCOUNTER;
                            idx++;
                        }
                    }
                }
            }
        }
        if (wd == 5 && game_state.day_count >= 0x21) {
            if (game_state.day_events[0].type == DAY_EVENT_EXAM) {
                if ((rand() & 1) && game_state.girls[9].flags.f.met && (rand() % 100) < 0x32 && get_g_zyotai_h(GIRL_MEGUMI) < 3U
                    && (game_state.param_art.value + game_state.param_liberal_arts.value + game_state.param_science.value) >= 0x12D) {
                    gs->last_command.b[0] = (gs->last_command.b[0] & 0xFFF0) | 3;
                    gs->day_events[idx].type = DAY_EVENT_SICK_VISIT;
                    gs->day_events[idx].girl = 9;
                    idx++;
                }
            } else if (gs->month_calendar[monthOff].day_kind[day] == 0 && (game_state.last_command.w & 0xF) != 7 && (rand() & 3) && nYes == 0 && nNo == 6) {
                if (sel != 0xF && sel != 0) {
                    if (!((game_state.last_command.w & 0xF) == 5 && sel == 0)) {
                        gs->day_events[idx].type = DAY_EVENT_SICK_VISIT;
                        gs->day_events[idx].girl = sel;
                        idx++;
                    }
                } else {
                    r = game_state.last_command.w & 0xF;
                    /* FAKE: T-23003, unsigned 1: the original compares against $at, not the hoisted $s5 */
                    if ((r == 0 || r == 1U || r == 3 || r == 2) && (rand() % 100) < 7 && get_g_zyotai_h(GIRL_MEGUMI) < 3U && game_state.girls[9].flags.f.met) {
                        gs->day_events[idx].type = DAY_EVENT_SICK_VISIT;
                        gs->day_events[idx].girl = 9;
                        idx++;
                    } else if (sel == 0 && (game_state.last_command.w & 0xF) != 5) {
                        gs->day_events[idx].type = DAY_EVENT_SICK_VISIT;
                        gs->day_events[idx].girl = 0;
                        idx++;
                    }
                }
            }
        }
        if (birth_day_check_days(GIRL_SHIORI, game_state.month + monthOff, day) == 1) {
            gs->day_events[idx].type = DAY_EVENT_GIRL_BIRTHDAY;
            gs->day_events[idx].girl = 0;
            idx++;
        }
        for (j = 1; j < 0xB; j++) {
            if (game_state.girls[j].flags.f.met && game_state.girls[j].flags.f.b6) {
                if (birth_day_check_days(j, game_state.month + monthOff, day) == 1) {
                    gs->day_events[idx].type = DAY_EVENT_GIRL_BIRTHDAY;
                    gs->day_events[idx].girl = j;
                    idx++;
                }
            }
        }
        if ((gs->player_flags.b[1] & 0xF) != 3) {
            if (birth_day_check_days(GIRL_PLAYER, game_state.month + monthOff, day) == 1) {
                gs->day_events[idx].type = DAY_EVENT_PLAYER_BIRTHDAY;
                idx++;
            }
        }
        if (wd == 5) {
            gs->day_events[idx].type = DAY_EVENT_WEEK_END;
            idx++;
        }
        if (wd != 6) {
            gs->day_events[idx].type = DAY_EVENT_DAY_END;
            idx++;
        }
        wd++;
        day++;
    }
    mode_step_next();
}

s32 schedule_miharu_scene4_ready(void) {
    s32 count;
    s32 i;
    /* FAKE: the helper writes 16 bytes; four extra bytes reproduce the original frame and buffer slots. T-11670. */
    u8 rank[20];
    s16 points[16];

    girl_scores(rank, points);
    count = 0;
    for (i = 0; i != 11; i++) {
        if (game_state.girls[i].flags.f.met && points[i] >= 0x1F5) {
            count++;
        }
    }
    if (game_state.year == 0x62 && game_state.month == 2 && game_state.day == 0xF && count >= 2 &&
        game_state.girls[11].flags.f.story_stage >= 4 && game_state.scene_flags[0x4C] != 0 && game_state.scene_flags[0x4E] != 0 &&
        !((u32)(game_state.unk_54C[0].w << 2) >> 31) && (rand() & 0xF) < 5) {
        return 1;
    }
    return 0;
}

s32 schedule_meeting_select_catchup(u8 *selected_girl, u8 *scene_variant) {
    /* The flag word's f9 field is read through a pointer: an indirect load is not a plain variable for uopt, so the
     * compares keep their constant-first order and no phantom local holds $v1 (T-17004). */
    GameState *g = &game_state;

    *selected_girl = 0;
    *scene_variant = 0;
    if ((rand() % 3) != 0) {
        return 0;
    }
    if (!(game_state.girls[GIRL_MIO].flags.f.met) && !((u32)game_state.last_command.w & 0xF) && (game_state.param_liberal_arts.value >= SCHEDULE_MEETING_CATCHUP_MIO_MINIMUM)) {
        *selected_girl = GIRL_MIO;
        *scene_variant = game_state.frame_count & 1;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUINA].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_SCIENCE) && (game_state.param_science.value >= SCHEDULE_MEETING_CATCHUP_YUINA_MINIMUM)) {
        *selected_girl = GIRL_YUINA;
        *scene_variant = game_state.frame_count & 3;
        return 1;
    }
    if (!(game_state.girls[GIRL_AYAKO].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ART) && (game_state.param_art.value >= SCHEDULE_MEETING_CATCHUP_AYAKO_MINIMUM)) {
        *selected_girl = GIRL_AYAKO;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUKARI].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ATHLETICS) && (((u8) game_state.girls[GIRL_SHIORI].flags.b[2] >> 4) != CLUB_TENNIS) && (game_state.param_athletics.value >= SCHEDULE_MEETING_CATCHUP_YUKARI_MINIMUM) && (game_state.param_looks.value >= SCHEDULE_MEETING_CATCHUP_YUKARI_MINIMUM)) {
        *selected_girl = GIRL_YUKARI;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_NOZOMI].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ATHLETICS) && (((u8) game_state.girls[GIRL_SHIORI].flags.b[2] >> 4) != CLUB_SWIMMING) && (game_state.param_athletics.value >= SCHEDULE_MEETING_CATCHUP_NOZOMI_MINIMUM) && (game_state.param_guts.value >= SCHEDULE_MEETING_CATCHUP_NOZOMI_MINIMUM)) {
        *selected_girl = GIRL_NOZOMI;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_MIRA].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_LOOKS) && (game_state.param_looks.value >= SCHEDULE_MEETING_CATCHUP_MIRA_MINIMUM)) {
        *selected_girl = GIRL_MIRA;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUKO].flags.f.met)) {
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 0) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_0_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 0;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 1) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_1_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 1;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 2) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_2_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 2;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 3) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_TRIVIA) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_3_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 3;
            return 1;
        }
    }
    if (!(game_state.girls[GIRL_SAKI].flags.f.met) && (((u16) game_state.player_flags.h >> 0xC) != ((u8) game_state.girls[GIRL_SAKI].flags.b[2] >> 4)) && (game_state.param_guts.value >= SCHEDULE_MEETING_CATCHUP_SAKI_MINIMUM)) {
        *selected_girl = GIRL_SAKI;
        *scene_variant = 0;
        return 1;
    }
    return 0;
}

s32 schedule_meeting_select_normal(u8 *selected_girl, u8 *scene_variant) {
    /* Read through a pointer like schedule_meeting_select_catchup (T-17004). */
    GameState *g = &game_state;

    if (rand() & 7) {
        return 0;
    }
    *selected_girl = 0;
    *scene_variant = 0;
    if (!(game_state.girls[GIRL_MIO].flags.f.met) && !((u32)game_state.last_command.w & 0xF) && (game_state.param_liberal_arts.value >= SCHEDULE_MEETING_NORMAL_MIO_MINIMUM)) {
        *selected_girl = GIRL_MIO;
        *scene_variant = game_state.frame_count & 1;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUINA].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_SCIENCE) && (game_state.param_science.value >= SCHEDULE_MEETING_NORMAL_YUINA_MINIMUM)) {
        *selected_girl = GIRL_YUINA;
        *scene_variant = game_state.frame_count & 3;
        return 1;
    }
    if (!(game_state.girls[GIRL_AYAKO].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ART) && (game_state.param_art.value >= SCHEDULE_MEETING_NORMAL_AYAKO_MINIMUM)) {
        *selected_girl = GIRL_AYAKO;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUKARI].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ATHLETICS) && (((u8) game_state.girls[GIRL_SHIORI].flags.b[2] >> 4) != CLUB_TENNIS) && (game_state.param_athletics.value >= SCHEDULE_MEETING_NORMAL_YUKARI_MINIMUM) && (game_state.param_looks.value >= SCHEDULE_MEETING_NORMAL_YUKARI_MINIMUM)) {
        *selected_girl = GIRL_YUKARI;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_NOZOMI].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_ATHLETICS) && (((u8) game_state.girls[GIRL_SHIORI].flags.b[2] >> 4) != CLUB_SWIMMING) && (game_state.param_athletics.value >= SCHEDULE_MEETING_NORMAL_NOZOMI_MINIMUM) && (game_state.param_guts.value >= SCHEDULE_MEETING_NORMAL_NOZOMI_MINIMUM)) {
        *selected_girl = GIRL_NOZOMI;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_MIRA].flags.f.met) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_LOOKS) && (game_state.param_looks.value >= SCHEDULE_MEETING_NORMAL_MIRA_MINIMUM)) {
        *selected_girl = GIRL_MIRA;
        *scene_variant = 0;
        return 1;
    }
    if (!(game_state.girls[GIRL_YUKO].flags.f.met)) {
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 0) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_0_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 0;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 1) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_1_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 1;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 2) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_2_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 2;
            return 1;
        }
        if ((g->girls[GIRL_YUKO].flags.f.story_stage == 3) && (((u32)game_state.last_command.w & 0xF) == SCHOOL_DAY_COMMAND_TRIVIA) && (game_state.param_trivia.value >= SCHEDULE_MEETING_YUKO_STEP_3_TRIVIA_MINIMUM)) {
            *selected_girl = GIRL_YUKO;
            *scene_variant = 3;
            return 1;
        }
    }
    if (!(game_state.girls[GIRL_SAKI].flags.f.met) && (((u16) game_state.player_flags.h >> 0xC) != ((u8) game_state.girls[GIRL_SAKI].flags.b[2] >> 4)) && (game_state.param_guts.value >= SCHEDULE_MEETING_NORMAL_SAKI_MINIMUM)) {
        *selected_girl = GIRL_SAKI;
        *scene_variant = 0;
        return 1;
    }
    return 0;
}

s32 schedule_meeting_select_club(u8 *selected_girl, u8 *scene_variant) {
    GameState *g;
    s32 club;

    g = &game_state;
    *selected_girl = 0;
    *scene_variant = 0;
    club = (u16) g->player_flags.h >> 0xC;
    if ((rand() % 3) != 0) {
        return 0;
    }
    if (!(g->girls[GIRL_MIO].flags.f.met) && (club == ((u8) g->girls[GIRL_MIO].flags.b[2] >> 4)) && ((u16) g->clubs[club].practice_points >= 0x15U)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_MIO].flags)->b18 = 1;
        *selected_girl = GIRL_MIO;
        if (club == 0) {
            *scene_variant = (g->frame_count & 1) + 2;
            return 1;
        }
        *scene_variant = 4;
        return 1;
    }
    if (!(g->girls[GIRL_YUINA].flags.f.met) && (club == ((u8) g->girls[GIRL_YUINA].flags.b[2] >> 4)) && ((u16) g->clubs[club].practice_points >= 0x15U)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_YUINA].flags)->b18 = 1;
        *selected_girl = GIRL_YUINA;
        if (club == 2) {
            *scene_variant = 4;
            return 1;
        }
        *scene_variant = 5;
        return 1;
    }
    if (!(g->girls[GIRL_AYAKO].flags.f.met) && (club == ((u8) g->girls[GIRL_AYAKO].flags.b[2] >> 4)) && ((u16) g->clubs[club].practice_points >= 0x15U)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_AYAKO].flags)->b18 = 1;
        *selected_girl = GIRL_AYAKO;
        if (club == 4) {
            *scene_variant = 1;
            return 1;
        }
        *scene_variant = 2;
        return 1;
    }
    if (!(g->girls[GIRL_SAKI].flags.f.met) && (club == ((u8) g->girls[GIRL_SAKI].flags.b[2] >> 4)) && ((u16) g->clubs[club].practice_points >= 0x15U)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_SAKI].flags)->b18 = 1;
        *selected_girl = GIRL_SAKI;
        if (club == 6) {
            *scene_variant = 1;
            return 1;
        }
        *scene_variant = 2;
        return 1;
    }
    if (!(g->girls[GIRL_YUKARI].flags.f.met) && (club == 8) && (((u8) g->girls[GIRL_SHIORI].flags.b[2] >> 4) != 8) && ((u16) g->clubs[club].practice_points >= 0x15U)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_YUKARI].flags)->b18 = 1;
        *selected_girl = GIRL_YUKARI;
        *scene_variant = 1;
        return 1;
    }
    if (!(g->girls[GIRL_NOZOMI].flags.f.met) && (club == 9) && (((u8) g->girls[GIRL_SHIORI].flags.b[2] >> 4) != 9)) {
        ((ScheduleGirlFlagBits *)&g->girls[GIRL_NOZOMI].flags)->b18 = 1;
        *selected_girl = GIRL_NOZOMI;
        *scene_variant = 1;
        return 1;
    }
    return 0;
}

s32 schedule_command_girl_select(u32 command_id) {
    /* FAKE: the helper writes 16 bytes; four extra bytes reproduce the original frame. T-10730. */
    u8 rank[20];
    s16 points[16];
    GameState *g;
    s32 firstGirl;
    s32 firstScore;
    s32 selfScore;
    s32 secondGirl;
    s32 secondScore;

    firstScore = 0;
    firstGirl = 0;
    selfScore = 0;
    secondScore = 0;
    secondGirl = 0;
    girl_scores(rank, points);
    g = &game_state;
    switch (command_id % 9U) {
    case 0:
        if (g->girls[1].flags.f.met) {
            firstGirl = 1;
            firstScore = rank[1] - schedule_command_rank_bar_liberal_arts_athletics;
        }
        selfScore = rank[0] - schedule_command_rank_bar_liberal_arts_athletics;
    default:
        break;
    case 1:
        if (g->girls[2].flags.f.met) {
            firstGirl = 2;
            firstScore = rank[2] - schedule_command_rank_bar_science_looks;
        }
        selfScore = rank[0] - schedule_command_rank_bar_science_looks;
        break;
    case 2:
        if (g->girls[3].flags.f.met) {
            firstGirl = 3;
            firstScore = rank[3] - schedule_command_rank_bar_art;
        }
        selfScore = rank[0] - schedule_command_rank_bar_art;
        break;
    case 3:
        if (g->girls[5].flags.f.met) {
            firstGirl = 5;
            firstScore = rank[5] - schedule_command_rank_bar_liberal_arts_athletics;
        }
        if (g->girls[6].flags.f.met) {
            secondGirl = 6;
            secondScore = rank[6] - schedule_command_rank_bar_liberal_arts_athletics;
        }
        selfScore = rank[0] - schedule_command_rank_bar_shiori_athletics;
        break;
    case 4:
        if (((u16)g->player_flags.h >> 12) == (((u8) g->girls[0].flags.b[2] >> 4))) {
            selfScore = rank[0] - 0xB;
            if (!(g->girls[0].flags.f.b2)) {
                selfScore = 0;
            }
            if (((u16)g->player_flags.h >> 12) == 0xA) {
                if (g->girls[0xA].flags.f.met) {
                    if ((g->girls[0xA].flags.f.b2) && ((((u8) g->girls[0xA].flags.b[2] >> 4)) == 0xA)) {
                        firstGirl = 0xA;
                        firstScore = rank[10] - 0xB;
                    }
                }
            }
        } else {
            switch (((u16)g->player_flags.h >> 12)) {
            case 0:
                if (g->girls[1].flags.f.met) {
                    if ((g->girls[1].flags.f.b2) && ((((u8) g->girls[1].flags.b[2] >> 4)) == 0)) {
                        firstGirl = 1;
                        firstScore = rank[1] - 0xB;
                    }
                }
                break;
            case 1:
                if ((g->girls[1].flags.f.met) && (g->girls[1].flags.f.b2)) {
                    if ((((u8) g->girls[1].flags.b[2] >> 4)) == 1) {
                        firstGirl = 1;
                        firstScore = rank[1] - 0xB;
                    }
                }
                break;
            case 2:
                if (g->girls[2].flags.f.met) {
                    if ((g->girls[2].flags.f.b2) && ((((u8) g->girls[2].flags.b[2] >> 4)) == 2)) {
                        firstGirl = 2;
                        firstScore = rank[2] - 0xB;
                    }
                }
                break;
            case 3:
                if (g->girls[2].flags.f.met) {
                    if ((g->girls[2].flags.f.b2) && ((((u8) g->girls[2].flags.b[2] >> 4)) == 3)) {
                        firstGirl = 2;
                        firstScore = rank[2] - 0xB;
                    }
                }
                break;
            case 4:
                if (g->girls[3].flags.f.met) {
                    if ((g->girls[3].flags.f.b2) && ((((u8) g->girls[3].flags.b[2] >> 4)) == 4)) {
                        firstGirl = 3;
                        firstScore = rank[3] - 0xB;
                    }
                }
                break;
            case 5:
                if (g->girls[3].flags.f.met) {
                    if ((g->girls[3].flags.f.b2) && ((((u8) g->girls[3].flags.b[2] >> 4)) == 5)) {
                        firstGirl = 3;
                        firstScore = rank[3] - 0xB;
                    }
                }
                break;
            case 6:
                if ((g->girls[4].flags.f.met) && (g->girls[4].flags.f.b2)) {
                    if ((((u8) g->girls[4].flags.b[2] >> 4)) == 6) {
                        firstGirl = 4;
                        firstScore = rank[4] - 0xB;
                    }
                }
                break;
            case 7:
                if (g->girls[4].flags.f.met) {
                    if ((g->girls[4].flags.f.b2) && ((((u8) g->girls[4].flags.b[2] >> 4)) == 7)) {
                        firstGirl = 4;
                        firstScore = rank[4] - 0xB;
                    }
                }
                break;
            case 8:
                if (g->girls[5].flags.f.met) {
                    if (g->girls[5].flags.f.b2) {
                        if ((((u8) g->girls[5].flags.b[2] >> 4)) == 8) {
                            if ((((u8) g->girls[0].flags.b[2] >> 4)) != 8) {
                                firstGirl = 5;
                                firstScore = rank[5] - 0xB;
                            }
                        }
                    }
                }
                break;
            /* FAKE: signed comparison keeps 9 separate from the unsigned modulo constant. T-10730. */
            case 9:
                if (g->girls[6].flags.f.met) {
                    if (g->girls[6].flags.f.b2) {
                        if (((s32)((u8) g->girls[6].flags.b[2] >> 4)) == 9) {
                            if (((s32)((u8) g->girls[0].flags.b[2] >> 4)) != 9) {
                                firstGirl = 6;
                                firstScore = rank[6] - 0xB;
                            }
                        }
                    }
                }
                break;
            case 10:
                if ((g->girls[0xA].flags.f.met) && (g->girls[0xA].flags.f.b2)) {
                    if ((((u8) g->girls[10].flags.b[2] >> 4)) == 0xA) {
                        firstGirl = 0xA;
                        firstScore = rank[10] - 0xB;
                    }
                }
                break;
            }
        }
        break;
    case 5:
        if (g->girls[8].flags.f.met) {
            firstGirl = 8;
            firstScore = rank[8] - schedule_command_rank_bar_trivia;
        }
        selfScore = rank[0] - schedule_command_rank_bar_trivia;
        break;
    case 6:
        if (g->girls[7].flags.f.met) {
            firstGirl = 7;
            firstScore = rank[7] - schedule_command_rank_bar_science_looks;
        }
        selfScore = rank[0] - schedule_command_rank_bar_science_looks;
        break;
    case 7:
        break;
    }
    if ((firstGirl != 0) && (secondGirl != 0)) {
        if (secondScore < firstScore) {
            if ((selfScore < firstScore) && (firstScore > 0)) {
                return firstGirl;
            }
            goto block_97;
        }
        if ((selfScore < secondScore) && (secondScore > 0)) {
            return secondGirl;
        }
        goto block_97;
    }
    if (firstGirl != 0) {
        if ((selfScore < firstScore) && (firstScore > 0)) {
            return firstGirl;
        }
        goto block_97;
    }
    if ((secondGirl != 0) && (selfScore < secondScore) && (secondScore > 0)) {
        return secondGirl;
    }
block_97:
    if (selfScore > 0) return 0;
    return 0xF;
}

/* Plain char outputs preserve IDO's separate byte-store constant range.
 * Explicit s8/u8 outputs change sharing with signed helper results. T-10730 */
s32 schedule_special_scene_select(char *selected_girl_out, char *scene_variant_out) {
    GameState *g;
    /* FAKE: 11 counts are used; the extra bytes and pad reproduce sp+0x7C
     * in the original 0xA8 frame. T-10730 */
    u8 counts[40];
    u8 framePad[32];
    s32 i;
    s32 event;
    s32 girl;

    g=&game_state;
    *selected_girl_out = 0;
    for (i=0;i<11;i++) {
        counts[i]=0;
        for (event=1;event<21;event++) {
            counts[i] += g->girls[i].scene_counters[event];
        }
    }
    for (girl=0; girl<11; girl++) {
        if ((g->girls[girl].flags.f.met) && (get_g_zyotai_h(girl) < 2U) && (counts[girl] != 0) && ((rand() & 0xF) < 5)) {
            switch (girl) {
            case 0:
                if ((rand() % 100) < 3) {
                    if (((WordView *)&g->frame_count)->w & 1) {
                        if ((hud_summer_phase() == 1) && (g->scene_flags[0] == 0) && ((u32) (get_g_zyotai_s(girl) & 0x7F) < 2U)) {
                            g->scene_flags[0] = (u8) (g->scene_flags[0] + 1);
                            *selected_girl_out = 0;
                            *scene_variant_out = 1;
                            return 1;
                        }
                        break;
                    }
                    if (g->scene_flags[1] == 0) {
                        g->scene_flags[1] = (u8) (g->scene_flags[1] + 1);
                        *selected_girl_out = 0;
                        *scene_variant_out = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 1:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 0)) {
                    if (g->scene_flags[8] == 0) {
                        g->scene_flags[8] += 1;
                        *selected_girl_out = 1;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 2:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 0)) {
                    if (g->scene_flags[14] == 0) {
                        g->scene_flags[14] += 1;
                        *selected_girl_out = 2;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 3:
                if (((rand() % 100) < 7) && (hud_summer_phase() == 1)) {
                    if (g->scene_flags[21] == 0) {
                        g->scene_flags[21] += 1;
                        *selected_girl_out = 3;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 4:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 0)) {
                    g->scene_flags[27] += 1;
                    *selected_girl_out = 4;
                    *scene_variant_out = 1;
                    return 1;
                }
                break;
            case 5:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 1) && (g->scene_flags[33] == 0)) {
                    g->scene_flags[33] += 1;
                    *selected_girl_out = 5;
                    *scene_variant_out = 1;
                    return 1;
                }
                if (((rand() % 100) < 0x32) && (g->scene_flags[37] == 1)) {
                    if (g->scene_flags[34] == 0) {
                        g->scene_flags[34] += 1;
                        *selected_girl_out = 5;
                        *scene_variant_out = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 6:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 1)) {
                    if (g->scene_flags[40] == 0) {
                        g->scene_flags[40] += 1;
                        *selected_girl_out = 6;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 7:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 1)) {
                    if (g->scene_flags[46] == 0) {
                        g->scene_flags[46] += 1;
                        *selected_girl_out = 7;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 8:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 0)) {
                    if (g->scene_flags[54] == 0) {
                        g->scene_flags[54] += 1;
                        *selected_girl_out = 8;
                        *scene_variant_out = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 9:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 0) && (g->scene_flags[60] == 0)) {
                    g->scene_flags[60] = (u8) (g->scene_flags[60] + 1);
                    *selected_girl_out = 9;
                    *scene_variant_out = 1;
                    return 1;
                }
                if ((g->scene_flags[60] == 1) && (g->girls[9].flags.f.b14 == 1)) {
                    *(u8 *)((unsigned char *)(&g->scene_flags[60]) + (1)) = (u8) (*(u8 *)((unsigned char *)(&g->scene_flags[60]) + (1)) + 1);
                    g->girls[9].flags.b[1] = (u8) (g->girls[9].flags.b[1] & 0xFFBF);
                    *selected_girl_out = 9;
                    *scene_variant_out = 2;
                    return 1;
                }
                break;
            case 10:
                if (((rand() % 100) < 3) && (hud_summer_phase() == 1) && (g->scene_flags[27] != 0) && (g->scene_flags[67] == 0)) {
                    g->scene_flags[67] += 1;
                    *selected_girl_out = 0xA;
                    *scene_variant_out = 1;
                    return 1;
                }
                if (((rand() % 100) < 0xA) && (g->year == 0x61) && (g->month == 9) && ((u8) g->day >= 0xEU)) {
                    if (g->scene_flags[68] == 0) {
                        g->scene_flags[68] += 1;
                        *selected_girl_out = 0xA;
                        *scene_variant_out = 2;
                        return 1;
                    }
                    break;
                }
                break;
            default: break;
            }
        }
    }
    if (((u8) g->girls[12].flags.b[3] >= 0x15U) && (g->scene_flags[80] == 0) && (hud_summer_phase() == 0)) {
        g->scene_flags[80] += 1;
        *selected_girl_out = 0xC;
        *scene_variant_out = 2;
        return 1;
    }
    if ((u8) g->girls[12].flags.b[3] >= 0x29U) {
        if (g->scene_flags[79] == 0) {
            g->scene_flags[79] += 1;
            *selected_girl_out = 0xC;
            *scene_variant_out = 1;
            return 1;
        }
    }
    if (((u8)g->girls[12].flags.b[3] >= 0x41U) && (g->scene_flags[81] == 0)) {
        g->scene_flags[81] += 1;
        *selected_girl_out = 0xC;
        *scene_variant_out = 3;
        return 1;
    }
    if (((rand() % 100) < 4) && (g->scene_flags[77] == 0) && (g->girls[11].scene_counters[0] == 0) && ((u32) ((u32) (g->girls[11].flags.w << 0x14) >> 0x1D) < 5U) && ((u16) g->day_count >= 0x1DU)) {
        g->scene_flags[74] += 1;
        g->girls[11].scene_counters[0] = 4;
        *selected_girl_out = 0xB;
        *scene_variant_out = 1;
        return 1;
    }
    if ((g->scene_flags[53] == 0) && (g->scene_flags[52] == 1)) {
        g->scene_flags[53] += 1;
        *selected_girl_out = 7;
        *scene_variant_out = 8;
        return 1;
    }
    return 0;
}


/* Choose a walk-home girl from eligible candidates (50 percent roll), skipping the most recently selected girl */
s32 geko_judge(s8 *selected_girl_out) {
    s32 i;
    s32 idx;
    s32 n;
    /* FAKE: buffer sizes chosen to reproduce the original frame and the 0x5C/0x7C slots; real sizes unknown. T-11670 */
    u8 closeness_by_girl[0x18];
    u8 sp5C[0x20];

    *selected_girl_out = 0;
    n = (u32)system_data.seed % 11;
    girl_scores(closeness_by_girl, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((game_state.girls[n].flags.f.met != 0) && (game_state.girls[n].syoushin < 0x3C) && (get_g_zyotai_s(n) != 4) && (closeness_by_girl[n] >= 0x11) && ((rand() % 100) < 0x32)) {
            if (schedule_last_picked_girl != n) {
                *selected_girl_out = n;
                schedule_last_picked_girl = n;
                return 1;
            }
        }
    }
    return 0;
}

/* Choose a girl who invites the player out (8 percent roll), unless an appointment or a blocked date applies */
s32 date_sasoi_judge(s8 *selected_girl_out) {
    s32 i;
    s32 idx;
    s32 n;
    u8 closeness_by_girl[0x18];
    u8 sp5C[0x20];

    *selected_girl_out = 0;
    if (((u32)(game_state.unk_54C[0].w << 2) >> 31) == 1) {
        return 0;
    }
    /* FAKE: reuse loop locals for the calendar values and comparison constant
     * to preserve register priority, branch operands and stack homes. T-12803. */
    n = 12;
    i = game_state.month;
    idx = game_state.day;
    if (((u32)(idx + 6) / 7 == 2) && ((u32)(game_state.player_flags.h >> 12) < 0xB)) {
        return 0;
    }
    if ((n == i) && (idx == 0x11)) {
        return 0;
    }
    if ((n == i) && (idx == 0x18)) {
        return 0;
    }
    if ((game_state.year == 0x62) && ((u32)i >= 2)) {
        return 0;
    }
    n = (u32)system_data.seed % 11;
    girl_scores(closeness_by_girl, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((game_state.girls[n].flags.f.met != 0) && (game_state.girls[n].syoushin < 0x3C) && (get_g_zyotai_s(n) != 4) && (closeness_by_girl[n] >= 0x21) && ((rand() % 100) < 8)) {
            if (schedule_last_picked_girl != n) {
                *selected_girl_out = n;
                schedule_last_picked_girl = n;
                return 1;
            }
        }
    }
    return 0;
}

/* Resolve girl-encounter placeholders in the day-event queue into special events, invitations, encounters or skips */
void schedule_girls_gakko(void) {
    u8 cnt;
    s32 i;
    char girl;
    char arg;

    cnt = 0;
    for (i = 0; i < 32; i++) {
        if (game_state.day_events[i].type == DAY_EVENT_GIRL_ENCOUNTER) {
            if (game_state.day_events[i + 1].type == DAY_EVENT_PLAYER_BIRTHDAY || game_state.day_events[i + 1].type == DAY_EVENT_GIRL_BIRTHDAY || game_state.day_events[i + 1].type == DAY_EVENT_SICK_VISIT) {
                game_state.day_events[i].type = DAY_EVENT_SKIP;
            } else if ((game_state.player_flags.b[1] & 0xF) == 3) {
                game_state.day_events[i].type = DAY_EVENT_SKIP;
            } else if (schedule_special_scene_select(&girl, &arg)) {
                game_state.day_events[i].type = DAY_EVENT_SPECIAL_EVENT;
                game_state.day_events[i].girl = girl;
                game_state.day_events[i].scene = arg;
            } else if (date_sasoi_judge(&girl) == 1 && cnt == 0) {
                cnt++;
                game_state.day_events[i].type = DAY_EVENT_GIRL_INVITATION;
                game_state.day_events[i].girl = girl;
            } else if (geko_judge(&girl)) {
                game_state.day_events[i].girl = girl;
            } else {
                game_state.day_events[i].type = DAY_EVENT_SKIP;
            }
        }
    }
    mode_step_next();
}

/* Once per queue build (weekly): lower affection by 1 (floor 0) for every met girl when the day of month is 1-6, and
 * for girl 0 also on days 14-20 */
void yuukou_down(void) {
    s32 i;
    u32 q;
    s32 two;

    two = 2; /* FAKE: constant 2 kept in a local gives the original operand order of the bne (T-10510) */
    for (i = 0; i < 0xB; i++) {
        if (game_state.girls[i].flags.f.met) {
            q = game_state.day / 7;
            if (q == 0) {
                if (game_state.girls[i].affection > 0) {
                    game_state.girls[i].affection -= 1;
                }
            } else if (two == q && i == 0) {
                if (game_state.girls[i].affection > 0) {
                    game_state.girls[i].affection -= 1;
                }
            }
        }
    }
}

/* Once per queue build (weekly): add 5 to the syoushin counter of met girls whose weighted affection is below 10 */
void syoushin_up(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        if (game_state.girls[i].flags.f.met && (s32)get_h_yuukou(i) < 0xA) {
            game_state.girls[i].syoushin += 5;
        }
    }
}

/* Weekly girl update: affection decay and syoushin growth, only in school terms (vacation 0), then next step */
void schdeule_girls_param(void) {
    if (game_state.vacation == VACATION_NONE) {
        yuukou_down();
        syoushin_up();
    }
    mode_step_next();
}

void schedule_step_advance_day(void) {
    calendar_advance_day();
    if (cd_audio_state() & 4) {
        cd_audio_command(9, 0, 0);
    }
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

void schedule_day_screen_handler(void) {
    if (game_state.mode_stage == 0) {
        schedule_day_screen_step_dispatch();
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    dec_bg_draw(get_last_gamen_mode());
    hud_screen_frame_draw(1);
}

void schedule_day_screen_step_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        day_event_dispatch();
        break;
    case 1:
        schedule_ending_start();
        break;
    }
}
void schedule_ending_start(void) {
    if (game_state.substep == 0) {
        hud_season_bgm_load();
        game_state.substep++;
    } else if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        game_mode_set(GAME_MODE_ENDING);
    }
}

void girl_counters_clamp(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].tokimeki > 0x80) {
            game_state.girls[i].tokimeki = 0x80;
        }
        if (game_state.girls[i].tokimeki < 0) {
            game_state.girls[i].tokimeki = 0;
        }
        if (game_state.girls[i].affection > 0x80) {
            game_state.girls[i].affection = 0x80;
        }
        if (game_state.girls[i].affection < 0) {
            game_state.girls[i].affection = 0;
        }
        if (game_state.girls[i].syoushin > 0x80) {
            game_state.girls[i].syoushin = 0x80;
        }
        if (game_state.girls[i].syoushin < 0) {
            game_state.girls[i].syoushin = 0;
        }
        if (game_state.girls[i].syoushin < 0x32) {
            game_state.girls[i].flags.b[2] &= 0xFFFE;
            game_state.girls[i].flags.b[2] &= 0xFD;
        }
    }
}

s32 schedule_yuina_highest_points_check(void) {
    /* FAKE: buffer sizes chosen to reproduce the 0x70 frame and the 0x3C/0x5C slots; real sizes unknown. T-7020 */
    u8 buf[20];
    s16 v[16];
    s32 idx;
    s32 best;
    s32 i;

    girl_scores(buf, v);
    idx = 0;
    best = 0;
    if (v[0] < 0x1F4) {
        if (game_state.girls[12].flags.b[3] >= 0x46U && game_state.scene_flags[79] == 2) {
            return 0;
        }
    }
    if (v[2] < 0x1F4) {
        return 0;
    }
    if (!game_state.girls[2].flags.f.met) {
        return 0;
    }
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (best < v[i]) { idx = i; best = v[i]; }
    if (idx == 2) {
        return 1;
    }
    return 0;
}

void girl_flag4_may_clear(void) {
    s32 i;

    if (game_state.month == 5 && game_state.day == 6) {
        for (i = 0; i < 11; i++) {
            game_state.girls[i].flags.b[0] &= 0xFFEF;
        }
    }
}

/* T-25318: the opcode walk and the dispatch read game_state.day_events[game_state.day_event_index] directly, with no record
 * pointer or opcode local: uopt keeps the record address in $a1 and the opcode in $v1 across the tests, as the
 * target does, and the base 0x800E6280 stays out of the allocator. The 0xFF branch has its own hour local (four
 * locals give the target's spill slot at 0x30). */
void day_event_dispatch(void) {
    s32 op;
    u32 t;
    u32 hour;
    s32 i;

    girl_flag4_may_clear();
    girl_counters_clamp();
    /* FAKE: T-25052, the calendar byte views keep the base 0x800E6280 out of a register (plain fields: 901 rows,
     * T-25318). */
    if ((*(u8 *)&game_state.year) >= 0x62U && (*(u8 *)&game_state.month) >= 3U && (*(u8 *)&game_state.day) != 0) {
        mode_step_next();
        return;
    }
    if ((*(u8 *)&game_state.year) >= 0x62U && (*(u8 *)&game_state.month) == 2 && (*(u8 *)&game_state.day) == 0x1A) {
        op = 0;
        if (game_state.scene_flags[82] == 0) {
            for (i = 1; i < 10; i++) {
                if (game_state.girls[i].flags.f.met) {
                    op++;
                }
            }
            if (op >= 6) {
                game_state.day_event_index--;
                scene_work_selector = 0xD;
                talk_work = 0;
                game_mode_set(GAME_MODE_SPECIAL_EVENT);
                return;
            }
        }
    }
    if ((*(u8 *)&game_state.year) >= 0x62U && (*(u8 *)&game_state.month) == 2 && (*(u8 *)&game_state.day) == 0x1C
        && schedule_yuina_highest_points_check() != 0 && game_state.scene_flags[20] == 0) {
        scene_work_selector = 2;
        talk_work = 7;
        game_state.scene_flags[20] += 1;
        game_mode_set(GAME_MODE_SPECIAL_EVENT);
        return;
    }
    schedule_progress_state_update();
    schedule_amusement_park_flag_reset();
    if (((*(u8 *)&game_state.month) != 0xC || (*(u8 *)&game_state.day) != 0x18) && game_state.day_event_count_today == 0 && game_state.weekday != WEEKDAY_SUNDAY
        && game_state.month_calendar[0].day_kind[(*(u8 *)&game_state.day)] == 0 && game_state.day_events[game_state.day_event_index].type == DAY_EVENT_DAY_END
        && game_state.day_events[0].type != DAY_EVENT_EXAM && game_state.day_events[game_state.day_event_index - 1].type == DAY_EVENT_SCHOOL_DAY) {
        /* FAKE: T-25052, 11 + ((u32)i < 0) folds after uopt, so the bound 11 is not held in a register in the
         * three counter loops (plain loops: 92 rows, T-25318). */
        i = 0;
        do {
            if (game_state.girls[i].flags.f.met && game_state.girls[i].syoushin >= 0x4C && !(game_state.girls[i].flags.h[1] & 1)) {
                game_state.girls[i].flags.f.encounter_done = 1;
                if (game_state.day_event_index != 0) {
                    game_state.day_event_index--;
                }
                game_state.day_event_count_today += 1;
                game_state.day_events[game_state.day_event_index].girl = i;
                game_mode_set(GAME_MODE_GIRL_ENCOUNTER);
                return;
            }
            i++;
        } while (i != 11 + ((u32)i < 0));
    }
    if (game_state.day_events[0].type == DAY_EVENT_CLUB_MASTER_A && game_state.day_events[game_state.day_event_index - 1].type == DAY_EVENT_CLUB_MASTER_A) {
        if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_GIRL_BIRTHDAY) {
            game_state.day_event_index++;
        }
        if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_PLAYER_BIRTHDAY) {
            game_state.day_event_index++;
        }
    }
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_GIRL_BIRTHDAY) {
        if (game_state.day_events[game_state.day_event_index].girl == 0 && (game_state.day_event_done_bits & 1)) {
            game_state.day_event_index++;
        }
        if (game_state.day_events[game_state.day_event_index].girl != 0 && (game_state.day_event_done_bits & 2)) {
            game_state.day_event_index++;
        }
    }
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_PLAYER_BIRTHDAY && (game_state.day_event_done_bits & 4)) {
        game_state.day_event_index++;
    }
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_SKIP) {
        game_state.day_event_index++;
    }
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_DAY_END || game_state.day_events[game_state.day_event_index].type == DAY_EVENT_NONE) {
        hud_appointment_list_sort();
        if (hud_appointment_find_today((*(u8 *)&game_state.year), (*(u8 *)&game_state.month), (*(u8 *)&game_state.day)) > 0) {
            if (get_g_zyotai_s(game_state.unk_54C_0.girl) == 4) {
                game_state.unk_54C[0].b[3] &= 0xFFDF;
                game_state.daily_check_done = 1;
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(2);
                return;
            }
            if (game_state.appointment_place_1 != game_state.appointment_place_0) {
                game_state.unk_54C[0].b[3] &= 0xFFDF;
                game_state.daily_check_done = 1;
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(0);
                return;
            }
            if (game_state.unk_54C_0.active) {
                game_state.unk_54C[0].b[3] &= 0xFFDF;
                game_state.daily_check_done = 1;
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(1);
                return;
            }
        }
    }
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_NONE) {
        hour = game_state.player_flags.h >> 12;
        if (hour >= 6 && hour < 0xB) {
            if (((*(u8 *)&game_state.day) + 6) / 7 == 3 && (game_state.last_command.b[0] >> 4) != 4) {
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(8);
                return;
            }
        }
        if (hour < 6 && (*(u8 *)&game_state.month) == 0xA
            && (((*(u8 *)&game_state.year) == 0x5F && (*(u8 *)&game_state.day) == 8)
                || ((*(u8 *)&game_state.year) == 0x60 && (*(u8 *)&game_state.day) == 6)
                || ((*(u8 *)&game_state.year) == 0x61 && (*(u8 *)&game_state.day) == 5))) {
            game_mode_set(GAME_MODE_TELEPHONE);
            mode_stage_set(9);
            return;
        }
        if ((*(u8 *)&game_state.year) == 0x5F && (*(u8 *)&game_state.month) == 0xA && (*(u8 *)&game_state.day) == 0xF) {
            /* FAKE: T-25052, late-bound loop bound as in the first counter loop (92 rows without, T-25318). */
            i = 0;
            do {
                if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b12) {
                    if (get_g_zyotai_s(i) != 4 && hud_appointment_count_for_girl(i) == 0) {
                        game_state.daily_check_done = 1;
                        game_mode_set(GAME_MODE_TELEPHONE);
                        mode_stage_set(6);
                        return;
                    }
                }
                i++;
            } while (i != 11 + ((u32)i < 0));
        }
        if ((*(u8 *)&game_state.year) == 0x60 && (*(u8 *)&game_state.month) == 0xA && (*(u8 *)&game_state.day) == 0xD) {
            /* FAKE: T-25052, late-bound loop bound as in the first counter loop (92 rows without, T-25318). */
            i = 0;
            do {
                if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b13) {
                    if (get_g_zyotai_s(i) != 4 && hud_appointment_count_for_girl(i) == 0) {
                        game_state.daily_check_done = 1;
                        game_mode_set(GAME_MODE_TELEPHONE);
                        mode_stage_set(7);
                        return;
                    }
                }
                i++;
            } while (i != 11 + ((u32)i < 0));
        }
    }
    t = 0;
    if (game_state.day_events[game_state.day_event_index].type == DAY_EVENT_DAY_END) {
        for (i = 0; i < 11; i++) {
            if (game_state.girls[i].flags.f.met && game_state.girls[i].syoushin >= 0x3D && !game_state.girls[i].flags.f.rumor_done) {
                t++;
                game_state.girls[i].flags.f.rumor_done = 1;
            }
        }
        if (t != 0) {
            game_mode_set(GAME_MODE_RUMOR);
            return;
        }
        if (game_state.daily_check_done == 0) {
            if ((*(u8 *)&game_state.year) == 0x61 && (*(u8 *)&game_state.month) == 7 && (*(u8 *)&game_state.day) == 0x1A) {
                game_state.daily_check_done = 1;
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(5);
                return;
            }
            if (game_state.vacation == VACATION_NONE && (*(u8 *)&game_state.year) != 0x62) {
                for (i = 0; i < 11; i++) {
                    if (game_state.girls[i].flags.f.met) {
                        if (game_state.girls[i].syoushin >= 0x5B) {
                            audio_command(0x50E);
                            game_state.daily_check_done = 1;
                            game_mode_set(GAME_MODE_TELEPHONE);
                            mode_stage_set(4);
                            return;
                        }
                    }
                }
            }
            if (game_state.girls[11].flags.f.story_stage != 0 && game_state.scene_flags[77] == 0
                && (game_state.girls[11].flags.b[3] < 7U || (game_state.girls[11].flags.b[3] >= 7U && game_state.day_count >= 0x2DBU && game_state.girls[11].flags.b[3] < 9U))
                && !(rand() & 0x7F)) {
                game_state.daily_check_done = 1;
                game_mode_set(GAME_MODE_TELEPHONE);
                mode_stage_set(3);
                return;
            }
        }
        game_state.daily_check_done = 0;
        game_state.day_event_index++;
        calendar_advance_day();
    }
    op = game_state.day_events[game_state.day_event_index].type;
    switch ((u32)op) {
    case 0x0:
        for (i = 0; i != 0xE; i++) {
            if (i != game_state.day_events[game_state.day_event_index].girl) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        game_mode_set(GAME_MODE_SCHOOL_DAY);
        return;
    case 0x10:
        game_mode_set(GAME_MODE_HOLIDAY);
        return;
    case 0xFE:
        game_mode_set(GAME_MODE_MONTH_START);
        return;
    case 0xFD:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_DAY_NOTICE);
        return;
    case 0x20:
        girl_flags_bit0_clear();
        bzero(overlay_scratch, 0x100);
        game_mode_set(GAME_MODE_SPORTS_FESTIVAL);
        return;
    case 0x24:
        girl_flags_bit0_clear();
        bzero(overlay_scratch, 0x100);
        if ((game_state.player_flags.h >> 12) < 6U) {
            game_mode_set(GAME_MODE_CULTURE_FESTIVAL_A);
            return;
        }
        game_mode_set(GAME_MODE_CULTURE_FESTIVAL_B);
        return;
    case 0x28:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_CHRISTMAS_PREP);
        return;
    case 0x2C:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_NEW_YEAR_PREP);
        return;
    case 0x31:
        game_mode_set(GAME_MODE_VALENTINE);
        return;
    case 0x30:
        game_mode_set(GAME_MODE_SEASONAL_PARTY);
        return;
    case 0x40:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_EXAM);
        return;
    case 0x41:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_SCHOOL_TRIP);
        return;
    case 0x42:
        game_mode_set(GAME_MODE_CLUB_CAMP);
        return;
    case 0x43:
        girl_flags_bit0_clear();
        game_mode_set(GAME_MODE_CAREER_GUIDANCE);
        return;
    case 0x44:
        game_state.day_event_count_today += 1;
        if ((game_state.player_flags.b[1] & 0xF) == 3) {
            day_event_next(0);
            return;
        }
        op = game_state.day_events[game_state.day_event_index - 1].type;
        if (op == 0x28) {
            day_event_next(0);
            return;
        }
        if (op == 0x47) {
            day_event_next(0);
            return;
        }
        if (!game_state.girls[game_state.day_events[game_state.day_event_index].girl].flags.f.met) {
            day_event_next(0);
            return;
        }
        game_mode_set(GAME_MODE_GIRL_ENCOUNTER);
        return;
    case 0xFC:
        game_mode_set(GAME_MODE_WEEK_END);
        return;
    case 0x32:
        game_mode_set(GAME_MODE_GIRL_BIRTHDAY);
        return;
    case 0x33:
        game_mode_set(GAME_MODE_PLAYER_BIRTHDAY);
        return;
    case 0x25:
        game_mode_set(GAME_MODE_CLUB_MASTER_A);
        return;
    case 0x26:
        game_mode_set(GAME_MODE_CLUB_MASTER_B);
        return;
    case 0x27:
        game_mode_set(GAME_MODE_CLUB_MASTER_C);
        return;
    case 0x46:
        game_state.day_event_count_today += 1;
        if (game_state.unk_54C_0.active) {
            day_event_next(0);
            return;
        }
        game_mode_set(GAME_MODE_GIRL_INVITATION);
        return;
    case 0x45:
        scene_work_selector = game_state.day_events[game_state.day_event_index].girl;
        /* FAKE: T-25318, the (s32) index cast keeps uopt from reusing the record address in $a1; the target
         * reloads the index here. */
        talk_work = game_state.day_events[(s32)game_state.day_event_index].scene;
        game_mode_set(GAME_MODE_SPECIAL_EVENT);
        return;
    case 0x47:
        game_state.day_event_count_today += 1;
        game_mode_set(GAME_MODE_SICK_VISIT);
        return;
    case 0x48:
        game_state.current_girl = game_state.day_events[game_state.day_event_index].girl;
        game_state.girls[game_state.current_girl].affection = 0x32;
        game_state.girls[game_state.current_girl].syoushin = 0;
        talk_work = game_state.day_events[game_state.day_event_index].scene;
        game_mode_set(GAME_MODE_MEETING);
        return;
    case 0x49:
        game_mode_set(GAME_MODE_AMUSEMENT_PARK);
        return;
    case 0x4A:
        game_mode_set(GAME_MODE_CAREER_CHOICE);
        return;
    case 0xFF:
    default:
        game_mode_set(GAME_MODE_COMMAND_SCREEN);
        return;
    }
}

void schedule_quarter_flags_rotate(void) {
    GameState *state;

    state = &game_state;
    switch (game_state.month) {
    case 2:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
        break;
    case 3:
    case 6:
    case 9:
    case 12:
        ((QuarterFlagBits *)state->quarter_flags)->b14 = ((QuarterFlagBits *)game_state.quarter_flags)->b13;
        ((QuarterFlagBits *)state->quarter_flags)->b13 = 0;
        break;
    }
}

void calendar_advance_day(void) {
    game_state.weekday++;
    if (game_state.weekday == 7) {
        game_state.weekday = WEEKDAY_MONDAY;
    }
    game_state.day++;
    switch ((*(u8 *)&game_state.month)) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (game_state.day >= 0x20) {
            game_state.day = 1;
            (*(u8 *)&game_state.month)++;
            schedule_quarter_flags_rotate();
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (game_state.day >= 0x1F) {
            game_state.day = 1;
            (*(u8 *)&game_state.month)++;
            schedule_quarter_flags_rotate();
        }
        break;
    case 2:
        if (game_state.year == 0x60) {
            if (game_state.day >= 0x1E) {
                game_state.day = 1;
                (*(u8 *)&game_state.month)++;
                schedule_quarter_flags_rotate();
            }
        } else if (game_state.day >= 0x1D) {
            game_state.day = 1;
            (*(u8 *)&game_state.month)++;
            schedule_quarter_flags_rotate();
        }
        break;
    }
    if ((*(u8 *)&game_state.month) >= 0xD) {
        game_state.year++;
        (*(u8 *)&game_state.month) = 1;
    }
    ((HalfView *)&game_state.day_count)->h++;
    if ((*(u8 *)&game_state.month) % 3 == 0) {
        schedule_girl_spots_expire();
    }
    if (game_state.day_event_index != 0) {
        day_event_today_clear(0);
        game_state.daily_check_done = 0;
    } else {
        day_event_today_clear(0);
        game_state.daily_check_done = 1;
    }
    if ((game_state.weekday == WEEKDAY_SUNDAY) && (*(u16 *)&game_state.player_flags_upper & 1) && (game_state.day_events[0].type != DAY_EVENT_EXAM)) {
        *(s8 *)&game_state.player_flags_upper = (u8)game_state.player_flags_upper.h & 0xFFFE;
    }
    hud_appointment_list_sort();
}

void day_event_today_clear(u8 reset_value) {
    game_state.day_event_done_bits = reset_value;
    game_state.day_event_count_today = reset_value;
}

void schedule_day_notice_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        schedule_vacation_notice_step();
        break;
    case 1:
        game_state.day_event_index += 1;
        game_mode_set(GAME_MODE_DAY_SCREEN);
        break;
    }
    if (D_8011F113 & 0x80) {
        hizuke_show();
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
    if (D_8011ECD3 & 0x80) {
        hud_cursor_update();
    }
    dec_bg_draw(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    hud_screen_frame_draw(0);
}
void schedule_vacation_notice_step(void) {
    GameState *g = &game_state;
    /* FAKE: the first test reads unk_03D through a local copy and the second re-reads the field (found by autotweak).
     * With both tests on the field, unk_03D is promoted into $v0 (sharing it with the month range) instead of the
     * original's $v1; the real source is unknown. T-17004 */
    s32 flag;

    switch (g->mode_step) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("ダミー");
        message_window_title_show(0);
        gnsx(&g->player_family_name);
        flag = g->vacation;
        if (flag != 0 && g->day == 2) {
            g->vacation = VACATION_NONE;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->vacation != VACATION_NONE) {
            g->vacation = VACATION_NONE;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->month == 7) {
            sndisp("明日から夏休みだ）", 0, 0x1F);
            g->vacation = VACATION_SUMMER;
        } else if (g->month == 0xC) {
            sndisp("明日から冬休みだ）", 0, 0x1F);
            g->vacation = VACATION_WINTER;
        } else if (g->month == 3) {
            sndisp("明日から春休みだ）", 0, 0x1F);
            g->vacation = VACATION_SPRING;
        } else {
            g->vacation = VACATION_NONE;
        }
        hud_day_bgm_load();
        k_disp_start(3);
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(0, 0);
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void schedule_month_start_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        schedule_month_start_step_dispatch();
        break;
    case 1:
        game_state.day_event_index += 1;
        game_mode_set(GAME_MODE_DAY_SCREEN);
        break;
    }
    dec_bg_draw(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    hud_screen_frame_draw(1);
}
s32 schedule_month_start_step_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        schedule_month_data_load_step();
        break;
    case 1:
        schedule_month_start_music_wait();
        break;
    }
}

s32 schedule_month_data_load_step(void) {
    /* Fields are read through a pointer, as in schedule_meeting_select_catchup: the month byte keeps the original's a1/v0 allocation across
     * the call and the argument loads keep the original order (T-17004). */
    GameState *g = &game_state;

    switch (g->substep) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        g->substep++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            calendar_month_load(g->year, g->month, 1);
            if (g->month % 3 == 0) {
                if (g->month == 9 && g->day == 1 && g->year == 0x60) {
                    mode_stage_next();
                } else if (g->month == 9) {
                    text_system_reset();
                    message_window_set_title_hidden("ダミー");
                    message_window_title_show(0);
                    gnsx(&g->player_family_name);
                    sndisp("明日から新学期だ）", 0, 0x1F);
                    k_disp_start(3);
                    audio_command(0xC1);
                    hud_day_bgm_load();
                    mode_step_next();
                } else {
                    audio_command(0xC1);
                    hud_day_bgm_load();
                    mode_step_next();
                }
                schedule_amusement_park_flag_reset();
                D_800B5BC8 = 0xEF;
            } else {
                mode_stage_next();
            }
        }
        break;
    }
}

void schedule_month_start_music_wait(void) {
    if (audio_bank_load_poll() == 1) {
        audio_bank_start(0, 0);
        mode_stage_next();
    }
}

s32 day_event_next(s32 play_audio) {
    if (debug_menu_active != 0) {
        game_mode_set(GAME_MODE_OPTION_3);
        return 0;
    }
    if (play_audio == 1) {
        audio_bank_start(0, 0);
    }
    game_state.day_event_index++;
    game_mode_set(GAME_MODE_DAY_SCREEN);
    /* no return on this path: the original leaves $v0 as the call result */
}

void schedule_week_end_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0x0:
        schedule_week_end_hud_init();
        break;
    case 0x1:
        schedule_health_step_dispatch();
        break;
    default:
    case 0xFF:
        school_day_week_end_exit();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    hud_screen_frame_draw(1);
    dec_bg_draw(get_last_gamen_mode());
}
void schedule_week_end_hud_init(void) {
    parameter_show_init();
    hizuke_init();
    message_window_init();
    mode_stage_next();
}

void schedule_health_step_dispatch(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        schedule_health_check_roll();
        return;
    case 1:
        schedule_health_injury_start();
        return;
    case 2:
        schedule_health_neurosis_start();
        return;
    case 3:
        schedule_health_sick_start();
        return;
    case 5:
        schedule_health_injury_recover();
        return;
    case 6:
        schedule_health_neurosis_recover();
        return;
    case 7:
        schedule_health_sick_recover();
        return;
    default:
        mode_stage_set(0xFF);
        return;
    }
}

s32 schedule_health_check_roll(void) {
    s32 state;
    s32 lim;
    s32 i;
    s32 k;
    s32 flag;
    s32 start;
    /* FAKE: every GameState field is read through a local pointer. An indirect load is not a plain variable for uopt, so
     * the compares keep the original's constant-first order and register colours; found by autotweak (gstate), T-20830. T-21520 */
    GameState *g = &game_state;

    flag = 0;
    lim = g->param_guts.value;
    lim = lim / 8 + 0xCC;
    if (lim >= 0xFF) {
        lim = 0xFE;
    }
    switch (g->player_flags.b[1] & 0xF) {
    case 0:
        state = 0;
        if ((g->player_flags_upper.b[0] >> 5) == 0) {
            if ((g->param_condition.value < 0x14) && (lim < (rand() & 0xFF))) {
                state = 1;
                flag = 1;
            } else if (g->param_condition.value == 0) {
                state = 1;
                flag = 1;
            }
            if ((g->param_stress.value >= 0x51) && ((u32) lim < (u32) (system_data.seed & 0xFF))) {
                state += 2;
                flag = 1;
            }
        }
        if (state == 1) {
            mode_step_set(1);
        }
        if (state == 2) {
            mode_step_set(2);
        }
        if (state == 3) {
            mode_step_set(3);
        }
        break;
    case 1:
        state = 1;
        if ((g->param_stress.value >= 0x51) && ((u32) lim < (u32) (system_data.seed & 0xFF))) {
            state = 3;
            flag = 1;
        }
        if (g->param_condition.value >= 0x1F) {
            state = 0;
            flag = 1;
        }
        if (state == 3) {
            mode_step_set(3);
        }
        if (state == 0) {
            mode_step_set(5);
        }
        break;
    case 2:
        state = 2;
        if ((g->param_condition.value < 0x14) && (lim < (rand() & 0xFF))) {
            state = 3;
            flag = 1;
        } else if (g->param_condition.value == 0) {
            state = 3;
            flag = 1;
        }
        if (g->param_stress.value < 0x46) {
            state = 0;
            flag = 1;
        }
        if (state == 3) {
            mode_step_set(3);
        }
        if (state == 0) {
            mode_step_set(6);
        }
        break;
    case 3:
        state = 3;
        if ((g->param_stress.value < 0x46) && (g->param_condition.value >= 0x1F)) {
            state = 0;
            flag = 1;
        }
        if (state == 0) {
            mode_step_set(7);
            break;
        }
        if (((u8 *) g->month_calendar)[g->day] == 0) {
            start = rand() % 11;
            for (i = 0; i != 11; i++) {
                k = (start + i) % 11;
                if (g->girls[k].flags.f.met && (get_h_tokimeki(k) >= 0x33U) && (get_g_zyotai_s(start) != 4)) {
                    if ((rand() & 0xFF) < 0x3F) {
                        g->date_girl = k;
                        game_mode_set(GAME_MODE_SICK_VISIT_2);
                        return 1;
                    }
                }
            }
        }
        break;
    default:
        g->player_flags.b[1] &= 0xFFF0;
        break;
    }
    if (flag == 0) {
        mode_stage_set(0xFF);
    }
}

s32 schedule_health_injury_start(void) {
    switch (*(u8 *)&game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("怪我");
        gnsx(&game_state.player_family_name[0]);
        sndi("怪我をしてしまった）", 0, 0x1F);
        game_state.player_flags.b[1] = (game_state.player_flags.b[1] & 0xFFF0) | 1;
        k_disp_start(3);
        game_state.substep += 1;
        return;
    case 1:
        if ((u32)game_state.mode_step_timer.w >= 0x3D) {
            if (((u32)game_state.mode_step_timer.w >= 0x101) || (game_state.pad_pressed != 0)) {
                game_state.substep += 1;
                return;
            }
        }
        return;
    case 2:
        if (((u32)(game_state.frame_count & 0xFF) < 0x19U) && (game_state.scene_flags[0x4B] == 0) && (game_state.vacation == VACATION_NONE)) {
            game_mode_set(GAME_MODE_SPECIAL_EVENT);
            scene_work_selector = 0xB;
            talk_work = 2;
            game_state.scene_flags[0x4B] = 1;
            return 1;
        }
        mode_stage_set(0xFF);
        break;
    }
}

s32 schedule_health_neurosis_start(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("ノイローゼ");
        gnsx(&game_state.player_family_name[0]);
        sndi("ノイローゼになってしまった）", 0, 0x1F);
        game_state.player_flags.b[1] = (game_state.player_flags.b[1] & 0xFFF0) | 2;
        k_disp_start(3);
        game_state.substep += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x3D) {
            if (((u32) game_state.mode_step_timer.w >= 0x101) || (game_state.pad_held != 0)) {
                mode_stage_set(0xFF);
            }
        }
        break;
    }
}

s32 schedule_health_sick_start(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("病気");
        gnsx(&game_state.player_family_name[0]);
        audio_bank_start(0, 0x202);
        sndi("病気になってしまった）", 0, 0x1F);
        game_state.player_flags.b[1] = (game_state.player_flags.b[1] & 0xFFF0) | 3;
        k_disp_start(3);
        game_state.substep += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x3D) {
            if (((u32) game_state.mode_step_timer.w >= 0x101) || (game_state.pad_held != 0)) {
                mode_stage_set(0xFF);
            }
        }
        break;
    }
}

s32 schedule_health_injury_recover(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("治癒");
        game_state.player_flags_upper.b[0] = (game_state.player_flags_upper.b[0] & 0xFF1F) | 0x60;
        game_state.player_flags.b[1] &= 0xFFF0;
        gnsx(&game_state.player_family_name[0]);
        sndi("怪我が治った）", 0, 0x1F);
        k_disp_start(3);
        game_state.substep += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x3D) {
            if (((u32) game_state.mode_step_timer.w >= 0x101) || (game_state.pad_held != 0)) {
                mode_stage_set(0xFF);
            }
        }
        break;
    }
}

s32 schedule_health_neurosis_recover(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("治癒");
        game_state.player_flags_upper.b[0] = (game_state.player_flags_upper.b[0] & 0xFF1F) | 0x60;
        game_state.player_flags.b[1] &= 0xFFF0;
        gnsx(&game_state.player_family_name[0]);
        sndi("ノイローゼが治った）", 0, 0x1F);
        k_disp_start(3);
        game_state.substep += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x3D) {
            if (((u32) game_state.mode_step_timer.w >= 0x101) || (game_state.pad_held != 0)) {
                mode_stage_set(0xFF);
            }
        }
        break;
    }
}

s32 schedule_health_sick_recover(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title_hidden("治癒");
        game_state.player_flags_upper.b[0] = (game_state.player_flags_upper.b[0] & 0xFF1F) | 0x60;
        game_state.player_flags.b[1] &= 0xFFF0;
        gnsx(&game_state.player_family_name[0]);
        sndi("病気が治った）", 0, 0x1F);
        k_disp_start(3);
        game_state.substep += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x3D) {
            if (((u32) game_state.mode_step_timer.w >= 0x101) || (game_state.pad_held != 0)) {
                mode_stage_set(0xFF);
            }
        }
        break;
    }
}
