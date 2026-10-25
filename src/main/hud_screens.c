/* hud_screens.c: src/main/800674B0.c; T-27059: original object [0x800674B0,0x8006CB30); calendar/month view, magazine, save/load panel, appointment and club screens, command roll and shared HUD helpers; reuse prior reviewed m-hud name. Original object 0x800674B0-0x8006CB30 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_line /* Line draw calls narrow four coordinates at the call site. T-11060. */
#define MAIN_API_OVERRIDE_D_8011F414    /* Stored as s16 in hud_mascot_update. T-11100. */
#define MAIN_API_OVERRIDE_actor_pool    /* Indexed as Actor[] in hud_magazine_page_build. T-11100. */
#define MAIN_API_OVERRIDE_strncat       /* hud_save_slot_info_show calls strncat with 2 args for club name. T-11100. */
#include "common.h"
#include "common.h"
#include "game/album.h"
#include "game/appointment.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/player_flags.h"
#include "game/schedule_records.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/club.h"
#include "main/cursor.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/resident_views.h"
#include "main/import_views/retained_views.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/memory_card.h"
#include "main/menu.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/settings.h"
#include "main/speaker.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/text.h"
#include "main/vram.h"
#include "main/game_mode.h"
#include "game/appointment.h"
#include "game/word_views.h"
#include "main/calendar.h"
#include "main/hud.h"

extern s16 D_8011F414;
s32 prim_gradient_line(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);
u8 *strncat();
extern Actor actor_pool[];


void hud_day_bgm_load(void) {
    u8 vacation_type;
    s32 season_index;

    audio_command(5);
    audio_command(0x71);
    vacation_type = game_state.vacation;
    if (game_state.year == 0x60 && game_state.month == 9 && game_state.day == 1) {
        vacation_type = 1;
    }
    switch (vacation_type) {
    case 0:
        season_index = hud_season_index() & 3;
        audio_bank_load_start(0, 0xBF98, 0xBF79, D_800B5BE8[season_index], D_800B5BF8[season_index], D_800B5BD8[season_index]);
        break;
    case 1:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD7D1, 0xD7AF, 0xD7A4);
        break;
    case 2:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD73F, 0xD71E, 0xD717);
        break;
    case 3:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD780, 0xD752, 0xD743);
        break;
    }
}

void hud_bgm_load_finish_step(void) {
    if (audio_bank_load_poll() == 1) {
        audio_bank_start(0, 0);
        mode_stage_next();
    }
}

void hud_mascot_display_switch(s32 visible) {
    /* D_8011F443 is element 1 (stride 0x44) of the table at D_8011F3FF; the array form keeps IDO from hoisting the second load */
    if (visible) {
        D_8011F3FF[0] |= 0x80;
        D_8011F3FF[0x44] |= 0x80;
    } else {
        D_8011F3FF[0] &= 0x7F;
        D_8011F3FF[0x44] &= 0x7F;
    }
}

/* FAKE: the empty `if (arg0) {}` after the store gives the parameter a second use, so uopt leaves it in $a0 (without it
 * IDO copies it to $a1 at entry and hoists `li a0,27` into the bnez delay slot). The test emits no code. T-17002 */
void hud_mascot_set(s32 mascot_selection) {
    if (mascot_selection == 0 || mascot_selection >= 0xE) {
        D_80125C50 = 0x78;
        D_80125C52 = -0x37;
        D_80125C54 = 0xF;
        D_80125C55 = 8;
        hud_mascot_choice = 0;
        hud_mascot_display_switch(0);
        return;
    }
    hud_mascot_choice = mascot_selection;
    if (mascot_selection) {
    }
    actor_clear(0x1B);
    actor_pool[0x1B].unk_01 = 1;
    actor_pool[0x1B].flags = 0x40;
    actor_pool[0x1B].quad_flags = 0x40000000;
    actor_pool[0x1B].draw_flags = 0x84;
    actor_pool[0x1B].frame_table = D_800C5FD4;
    actor_pool[0x1B].sequence_table = D_800C6010;
    actor_pool[0x1B].tpage_table = (u8 *)&D_800C5FCC;
    actor_pool[0x1B].unk_14 = D_800C6014;
    actor_pool[0x1B].unk_04 = 8;
    actor_pool[0x1B].unk_05 = 1;
    actor_pool[0x1B].sequence_slot = 0;
    actor_pool[0x1B].unk_18 = hud_mascot_choice - 1;
    actor_clear(0x1C);
    actor_pool[0x1C].unk_01 = 1;
    actor_pool[0x1C].flags = 0x40;
    actor_pool[0x1C].quad_flags = 0x40000000;
    actor_pool[0x1C].draw_flags = 0x84;
    actor_pool[0x1C].frame_table = D_800C5FD4;
    actor_pool[0x1C].sequence_table = D_800C6010;
    actor_pool[0x1C].tpage_table = (u8 *)&D_800C5FCC;
    actor_pool[0x1C].unk_14 = D_800C6014;
    actor_pool[0x1C].unk_04 = 8;
    actor_pool[0x1C].unk_05 = 1;
    actor_pool[0x1C].sequence_slot = 0;
    actor_pool[0x1C].unk_18 = 0xD;
}

void hud_mascot_update(void) {
    s32 x1;
    s16 dx;
    s16 dy;
    s32 y1;
    s32 r;

    if (hud_mascot_choice == 0 || hud_mascot_choice >= 0xE) {
        hud_mascot_set(0);
        return;
    }
    hud_mascot_set(hud_mascot_choice);
    if (hud_mascot_choice < 0xE) {
        if ((u32)game_state.pad_pressed >> 16) {
            D_80125C55 = (D_80125C54 + 1) / 2;
        }
        if ((game_state.frame_count & 0x3F) == 0x3F && D_80125C55 != 0) {
            D_80125C55--;
        }
        dx = (rsin((game_state.frame_count & 0x7F) << 5) * D_80125C55) >> 12;
        D_8011F41C = ((-dx * 2) / 3) << 12;
        /* FAKE T-25406: one factor through a byte view. tools/cvt_pass.py writes no CVT for that read, so
         * D_80125C54 stays one $a2 range with no cvt(D_80125C54) range beside it (T-0018 gap); both factors
         * plain is 33 rows. */
        r = SquareRoot0(*(u8 *)&D_80125C54 * D_80125C54 - dx * dx);
        dy = 0xF - r;
        /* FAKE T-25406: with a plain D_80125C50 read, cvt(D_80125C54) comes back as its own $v0 range and
         * every colour after it moves (146 rows). */
        x1 = *(s16 *)&D_80125C50 + dx;
        actor_pool[0x1B].x = x1;
        /* FAKE T-25406: both operands through views. The byte view keeps D_80125C54 out of a cvt range; the
         * s16 view gives cfe equal operand weights, so the sum comes out as `addu t1,a0,a2` (D_80125C52 first). */
        y1 = *(u8 *)&D_80125C54 + *(s16 *)&D_80125C52;
        actor_pool[0x1B].y = y1 - dy;
        actor_pool[0x1B].unk_18 = hud_mascot_choice - 1;
        actor_pool[0x1C].x = D_80125C50;
        actor_pool[0x1C].y = D_80125C52;
        if (game_state.pad_held & PAD_R2) {
            actor_pool[0x1C].unk_18 = 0xE;
            switch (game_state.pad_held & 0x5000) {
            case 0x1000:
                if (D_80125C54 >= 3) {
                    D_80125C54 -= 2;
                    D_80125C55 = (D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            case 0x4000:
                if (D_80125C54 < 0xC4) {
                    D_80125C54 += 2;
                    D_80125C55 = (D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            }
        } else if (game_state.pad_held & PAD_R1) {
            actor_pool[0x1C].unk_18 = 0xE;
            /* FAKE T-25103 layout, kept by T-25406: each store and sum on one line keeps as1's order (4 rows split). */
            switch (game_state.pad_held & 0x5000) {
            case 0x1000:
                D_80125C52 -= 4; y1 = D_80125C52 + D_80125C54;
                break;
            case 0x4000:
                D_80125C52 += 4; y1 = D_80125C52 + D_80125C54;
                break;
            }
            switch (game_state.pad_held & 0xA000) {
            case 0x8000:
                D_80125C50 -= 4;
                x1 -= 4;
                break;
            case 0x2000:
                D_80125C50 += 4;
                x1 += 4;
                break;
            }
        } else {
            actor_pool[0x1C].unk_18 = 0xD;
        }
        prim_gradient_line(D_80125C50, D_80125C52, x1, y1 - dy, 0xF8B0C8, 0xF8B0C8, 1);
        if (D_80125C50 >= 0x201) {
            D_80125C50 = -0x200;
        }
        if (D_80125C50 < -0x200) {
            D_80125C50 = 0x200;
        }
        if (D_80125C52 >= 0x201) {
            D_80125C52 = -0x200;
        }
        if (D_80125C52 < -0x200) {
            D_80125C52 = 0x200;
        }
    }
}

/* FAKE: the second `max` loop is dead (max is never read) and the u8 view of the s8 field unk_03C gives the original lbu;
 * both are needed for the bytes, real source unknown. T-11690 */
void portrait_event_face_mouth_lipsync(void) {
    s32 max;
    s32 level;
    s32 n;
    s32 i;
    u8 girl_id;

    if (*(u8 *)&game_state.voice_mode != 0) {
        if (cd_audio_state() & 4) {
            level = getCDlevel() + 1;
        } else {
            level = 0;
        }
        D_80125C80[game_state.frame_count & 3] = level;
        max = 0;
        for (i = 0; i < 4; i++) {
            if (max < D_80125C80[i]) {
                max = D_80125C80[i];
            }
        }
        max = 0;
        for (i = 0; i < 4; i++) {
            if (max < D_80125C80[i]) {
                max = D_80125C80[i];
            }
        }
        n = 0;
        girl_id = game_state.current_girl;
        for (i = 0; i < 3; i++) {
            if (D_800B5A68[girl_id][i] < level) {
                n = i + 1;
            }
        }
        if (n >= 4) {
            n = 3;
        }
        if (D_80120668 == 0 || !(game_state.frame_count & 1)) {
            if (level == 0) {
                D_80120668 = 0;
            } else if (n == 0) {
                D_80120668 = 0;
            } else if (n == 1) {
                D_80120668 = 1;
            } else if (n == 2) {
                D_80120668 = 2;
            } else if (n >= 3) {
                D_80120668 = 3;
            }
        }
    }
}

/* FAKE: promoted halfword and byte alias retain the final andi before flag storage. T-9550 */
s32 portrait_event_face_eyes_animation_start(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_80120696;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_80120696 = *byte;
  }
}

/* FAKE: promoted halfword and byte alias retain the final andi before flag storage. T-9550 */
s32 portrait_event_face_mouth_animation_start(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_80120652;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_80120652 = *byte;
  }
}

void calendar_screen_init(void) {
    actor_clear(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = actor_default_sequence_table;
    D_80120684 = actor_default_texture_blocks;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x43;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
    calendar_marks_build();
}

/* One mark of the two-month calendar: frame `frame` in the cell of day index `d` of column `side` (0 this month,
 * 1 next month), then the next record. The four statements expand on one source line, which as1 schedules as one
 * group (the frame store after the argument copy where the record address is a constant). T-25103 */
#define CAL_MARK(frame, side, d) \
    actor_pool[n + 0x61].unk_18 = (frame); actor_pool[n + 0x61].x = calendar_cell_x((side), (d)); \
    actor_pool[n + 0x61].y = calendar_cell_y((side), (d)); n++
/* Frame of the schedule entry of day i + 1 in column `side`; every use reads it again (the original reloads it
 * after each call). T-25103 */
#define DAY_KIND(side) D_800B5C10[game_state.month_calendar[side].marker[i + 1]]

/* Calendar marks in records 0x61.. of actor_pool (at most 0x3F): the date word at +0xF8, the flagged GirlRecord dates,
 * the active +0x54C entries, two 0x6F marks in periods 6..10, the day-schedule frames of both months (a multi-day
 * event repeats its frame on the following empty days) and the +0x75E records of this month.
 * FAKE: the first month test of each pair reads unk_03F through `gs` and the second through game_state. They are
 * separate expressions, so uopt does not share cvt(unk_03F) across the mark calls (519 rows to 16); the original
 * keeps gs in a stack slot for the loop bounds, but which tests used it is unknown. T-25103 */
void calendar_marks_build(void) {
    GameState *gs;
    s32 i;
    s32 n;
    s32 repeat_days_remaining;
    /* FAKE: keeps repeated_calendar_frame in the original's sixth local slot (sp+0x68) and the frame at 0x80; real local unknown.
     * T-25103 */
    s32 pad;
    s32 repeated_calendar_frame;
    u32 calendar_month;
    u32 day_of_month;
    s32 current_month_third_sunday_day;
    s32 next_month_third_sunday_day;
    u32 calendar_year;

    gs = &game_state;
    for (i = 0; i < 0x20; i++) {
        actor_clear(i + 0x61);
        actor_pool[i + 0x61].unk_01 = 3;
        actor_pool[i + 0x61].flags = 0x40;
        actor_pool[i + 0x61].quad_flags = 0;
        actor_pool[i + 0x61].draw_flags = 0x8C;
        actor_pool[i + 0x61].frame_table = sprite_frame_table;
        actor_pool[i + 0x61].sequence_table = actor_default_sequence_table;
        actor_pool[i + 0x61].tpage_table = actor_default_texture_blocks;
        actor_pool[i + 0x61].unk_14 = D_800C9A54;
        actor_pool[i + 0x61].unk_04 = 8;
        actor_pool[i + 0x61].unk_05 = 1;
        actor_pool[i + 0x61].blink_period = 8;
        actor_pool[i + 0x61].unk_18 = 0x62;
        actor_pool[i + 0x61].x = 0x100;
        actor_pool[i + 0x61].y = 0x100;
        actor_pool[i + 0x61].brightness = 0x80;
    }
    repeat_days_remaining = 0;
    n = 0;
    calendar_month = ((WordView *)&game_state.player_birthday)->date.month;
    day_of_month = ((WordView *)&game_state.player_birthday)->date.day;
    if (calendar_month == gs->month) {
        CAL_MARK(0x6E, 0, day_of_month - 1);
    }
    if (calendar_month == game_state.month + 1) {
        CAL_MARK(0x6E, 1, day_of_month - 1);
    }
    for (i = 0; i < 11; i++) {
        calendar_month = game_state.girls[i].birthday.date.month;
        day_of_month = game_state.girls[i].birthday.date.day;
        if (game_state.girls[i].flags.f.b6) {
            if (calendar_month == gs->month) {
                CAL_MARK(0x6D, 0, day_of_month - 1);
            }
            if (calendar_month == game_state.month + 1 || game_state.month + 1 == calendar_month + 12) {
                CAL_MARK(0x6D, 1, day_of_month - 1);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        calendar_month = game_state.unk_54C[i].h[0] >> 12;
        day_of_month = game_state.unk_54C[i].h[1] & 0x1F;
        if (((AppointmentEntry *)&game_state.unk_54C[i])->active) {
            if (calendar_month == gs->month) {
                CAL_MARK(0x62, 0, day_of_month - 1);
            }
            if (calendar_month == game_state.month % 12 + 1) {
                CAL_MARK(0x62, 1, day_of_month - 1);
            }
        }
    }
    if ((game_state.player_flags.h >> 12) >= 6 && (game_state.player_flags.h >> 12) < 11) {
        current_month_third_sunday_day = 0x15 - game_state.month_calendar[0].marker[0];
        next_month_third_sunday_day = 0x15 - game_state.month_calendar[1].marker[0];
        CAL_MARK(0x6F, 0, current_month_third_sunday_day - 1);
        if (game_state.year != 0x62 || game_state.month != 2) {
            CAL_MARK(0x6F, 1, next_month_third_sunday_day - 1);
        }
    }
    for (i = 0; i < gs->month_calendar[0].day_kind[0]; i++) {
        if (game_state.month_calendar[0].marker[i + 1] != 0 && DAY_KIND(0) != 0xFF) {
            if (DAY_KIND(0) == 0x6F) {
                if (game_state.month == 8 && (game_state.player_flags.h >> 12) < 11) {
                    CAL_MARK(DAY_KIND(0), 0, i);
                }
            } else {
                CAL_MARK(DAY_KIND(0), 0, i);
            }
            if (DAY_KIND(0) == 0x66 && game_state.month != 2) {
                repeated_calendar_frame = DAY_KIND(0);
                repeat_days_remaining = 6;
            } else if (DAY_KIND(0) == 0x6C) {
                repeated_calendar_frame = DAY_KIND(0);
                repeat_days_remaining = 6;
            } else if (DAY_KIND(0) == 0x67) {
                repeated_calendar_frame = DAY_KIND(0);
                repeat_days_remaining = 7;
            } else if (DAY_KIND(0) == 0x6F && game_state.month == 8 &&
                       (game_state.player_flags.h >> 12) < 11) {
                repeated_calendar_frame = DAY_KIND(0);
                repeat_days_remaining = 6;
            }
        } else if (repeat_days_remaining > 0) {
            CAL_MARK(repeated_calendar_frame, 0, i);
        }
        repeat_days_remaining--;
        if (n + 0x61 >= 0xA0) {
            return;
        }
    }
    repeat_days_remaining = 0;
    for (i = 0; i < gs->month_calendar[1].day_kind[0]; i++) {
        if (game_state.month_calendar[1].marker[i + 1] != 0 && DAY_KIND(1) != 0xFF) {
            if (DAY_KIND(1) == 0x6F) {
                if (game_state.month == 7 && (game_state.player_flags.h >> 12) < 11) {
                    CAL_MARK(DAY_KIND(1), 1, i);
                }
            } else {
                CAL_MARK(DAY_KIND(1), 1, i);
            }
            if (DAY_KIND(1) == 0x66 && game_state.month != 1) {
                repeated_calendar_frame = DAY_KIND(1);
                repeat_days_remaining = 6;
            } else if (DAY_KIND(1) == 0x6C) {
                repeated_calendar_frame = DAY_KIND(1);
                repeat_days_remaining = 6;
            } else if (DAY_KIND(1) == 0x67) {
                repeated_calendar_frame = DAY_KIND(1);
                repeat_days_remaining = 7;
            } else if (DAY_KIND(1) == 0x6F && game_state.month == 7 &&
                       (game_state.player_flags.h >> 12) < 11) {
                repeated_calendar_frame = DAY_KIND(1);
                repeat_days_remaining = 6;
            }
        } else if (repeat_days_remaining > 0) {
            CAL_MARK(repeated_calendar_frame, 1, i);
        }
        repeat_days_remaining--;
        if (n + 0x61 >= 0xA0) {
            return;
        }
    }
    for (i = 0; i < 0x100; i++) {
        calendar_year = game_state.event_log[i].year;
        calendar_month = game_state.event_log[i].month;
        day_of_month = game_state.event_log[i].day;
        if (calendar_year == game_state.year && calendar_month == game_state.month) {
            if (game_state.event_log[i].girl == 0xC) {
                if (system_data.flags[12] != 0 || game_state.girl_cleared[12] != 0) {
                    CAL_MARK(0x62, 0, day_of_month - 1);
                }
            } else {
                CAL_MARK(0x62, 0, day_of_month - 1);
            }
        }
        if (n + 0x61 >= 0xA0) {
            return;
        }
        if (game_state.event_log[i].year == 0) {
            return;
        }
    }
}

#undef CAL_MARK
#undef DAY_KIND

s16 calendar_cell_x(s32 month_side, s32 day_index) {
    s32 v = game_state.month_calendar[0].marker[month_side * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return month_side * 0x88 + ((v + day_index + 1) % 7) * 16 - 0x78;
}

s16 calendar_cell_y(s32 month_side, s32 day_index) {
    s32 v = game_state.month_calendar[0].marker[month_side * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return ((v + day_index + 1) / 7) * 16 - 0x30;
}

/* FAKE: the loop stores go through the pointer `gs` so uopt reloads month_data_table on every trip as the original does (named
 * stores to game_state keep it in a register); `q` is the +0x800 base of the length/start words. T-11100 */
void calendar_month_load(u8 calendar_year, u8 calendar_month, s32 populate_next_month) {
    s32 month_data_index;
    s32 next_month_day_count;
    s32 i;
    s32 next_month_first_weekday;
    GameState *gs;
    s32 *q;

    month_data_index = (calendar_year - 95) * 12 + calendar_month - 4;
    if (month_data_index < 35) {
        LoadSquare(0x3A0, 0x100, 0x20, 0x80, month_data_table[month_data_index]);
        LoadSquare(0x3A0, 0x180, 0x20, 0x80, month_data_table[month_data_index + 1]);
        if (populate_next_month != 0) {
            memcpy(&game_state.month_calendar[0], &game_state.month_calendar[1], 0x40);
            gs = &game_state;
            q = month_data_table[month_data_index + 1] + 0x800;
            next_month_day_count = q[0];
            next_month_first_weekday = q[1];
            game_state.month_calendar[1].day_kind[0] = next_month_day_count;
            game_state.month_calendar[1].marker[0] = next_month_first_weekday;
            for (i = 0; i < next_month_day_count; i++) {
                gs->month_calendar[1].day_kind[i + 1] = month_data_table[month_data_index + 1][0x802 + i];
                gs->month_calendar[1].marker[i + 1] = month_data_table[month_data_index + 1][0x802 + next_month_day_count + i];
            }
        }
    }
}

void hud_biorhythm_graph_draw(void) {
    /* FAKE: unused stack locals match original 0x158 frame size and slot offsets. T-11060 */
    u16 top_pad1;
    u16 top_pad2;
    u16 top_pad3;
    u16 top_pad4;
    u16 saved;
    s32 sp10C[16];
    s32 spCC[16];
    s32 sp8C[16];
    s32 half;
    s32 i;
    s32 phase;
    s32 pad;

    phase = game_state.frame_count;
    if ((s32) game_state.frame_count & 0x80) {
        half = (0x7F - ((s32) game_state.frame_count & 0x7F)) / 2;
    } else {
        half = ((s32) game_state.frame_count & 0x7F) / 2;
    }
    prim_gradient_line(-0x80, -0xC, 0x7C, -0xC, half * 0x10101, half * 0x10101, 0x81);
    saved = game_state.day_count;
    game_state.day_count = saved - 8;
    for (i = 0; i != 0x10; i++) {
        sp10C[i] = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_GRAPH_SCALE);
        spCC[i] = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_GRAPH_SCALE);
        sp8C[i] = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_GRAPH_SCALE);
        game_state.day_count += 1;
        if (i != 0) {
            prim_gradient_line(i * 0x10 - 0x90, sp10C[i - 1] - 0xC, i * 0x10 - 0x80, sp10C[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x101, (((i + phase) & 0xF) * 4 + half) * 0x101, 0x81);
            prim_gradient_line(i * 0x10 - 0x90, spCC[i - 1] - 0xC, i * 0x10 - 0x80, spCC[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x10100 + 0xF0F00, (((i + phase) & 0xF) * 4 + half) * 0x10100 + 0xF0F00, 0x81);
            prim_gradient_line(i * 0x10 - 0x90, sp8C[i - 1] - 0xC, i * 0x10 - 0x80, sp8C[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x10001, (((i + phase) & 0xF) * 4 + half) * 0x10001, 0x81);
        }
    }
    game_state.day_count = saved;
}

void calendar_view_update(void) {
    s32 i;
    s32 t;

    hud_biorhythm_graph_draw();
    dtd_on_tpage(0, 0, 1, 2, 0);
    cal_base_show();
    t = ((Actor *)actor_pool)[0x61].unk_08++;
    for (i = 0x61; i < 0xA0; i++) {
        switch ((t >> 6) & 3) {
        case 0:
            ((Actor *)actor_pool)[i].scale_x = (t & 0x3F) << 6;
            ((Actor *)actor_pool)[i].scale_y = (t & 0x3F) << 6;
            break;
        case 1:
            ((Actor *)actor_pool)[i].scale_x = 0x1000;
            ((Actor *)actor_pool)[i].scale_y = 0x1000;
            break;
        case 2:
            ((Actor *)actor_pool)[i].scale_x = (0x3F - (t & 0x3F)) << 6;
            ((Actor *)actor_pool)[i].scale_y = (0x3F - (t & 0x3F)) << 6;
            break;
        case 3:
            ((Actor *)actor_pool)[i].scale_x = 0;
            ((Actor *)actor_pool)[i].scale_y = 0;
            break;
        }
    }
}

/* Draws the calendar screen background panel. */
void cal_base_show(void) {
    prim_gradient_rect(-0xA0, -0x57, 0x11B, 0x7D, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

/* Draws the appraisal screen background panel. */
void appraisal_base_show(void) {
    prim_gradient_rect(-0xA0, -0x68, 0x11B, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

/* Draws the magazine screen background panel. */
void magazine_base_show(void) {
    prim_gradient_rect(-0xA0, -0x78, 0x140, 0x9E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

/* Shows or hides the four calendar sprites. */
void cal_sprite_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        sprite_quads[0].flags &= 0x7FFFFFFF;
        sprite_quads[1].flags &= 0x7FFFFFFF;
        sprite_quads[2].flags &= 0x7FFFFFFF;
        sprite_quads[3].flags &= 0x7FFFFFFF;
        return;
    }
    sprite_quads[0].flags |= 0x80000000;
    sprite_quads[1].flags |= 0x80000000;
    sprite_quads[2].flags |= 0x80000000;
    sprite_quads[3].flags |= 0x80000000;
}

/* Initializes the calendar screen sprites and their display attributes. */
void cal_sprite_init(void) {
    sprite_quads[2].flags = 0;
    sprite_quads[2].x = -0x88;
    sprite_quads[2].y = -0x58;
    sprite_quads[2].w = 0x7E;
    sprite_quads[2].h = 0x7E;
    sprite_quads[2].unk_0C = 0x1E;
    sprite_quads[2].unk_0E = 0x80;
    sprite_quads[2].unk_0F = 0;
    sprite_quads[2].unk_10 = 0xC0;
    sprite_quads[2].unk_12 = 0x1E0;
    sprite_quads[2].tint_r = 0x80;
    sprite_quads[2].tint_g = 0x80;
    sprite_quads[2].tint_b = 0x80;
    sprite_quads[2].anchor_x = 0;
    sprite_quads[2].anchor_y = 0;
    sprite_quads[2].unk_1C = 0x1000;
    sprite_quads[2].unk_1E = 0x1000;
    sprite_quads[2].unk_20 = 0;
    game_state.quad_layer[2] = 0x84;
    sprite_quads[3].flags = 0;
    sprite_quads[3].x = 0;
    sprite_quads[3].y = -0x58;
    sprite_quads[3].w = 0x7F;
    sprite_quads[3].h = 0x7F;
    sprite_quads[3].unk_0C = 0x1E;
    sprite_quads[3].unk_0E = 0x80;
    sprite_quads[3].unk_0F = 0x80;
    sprite_quads[3].unk_10 = 0xC0;
    sprite_quads[3].unk_12 = 0x1E0;
    sprite_quads[3].tint_r = 0x80;
    sprite_quads[3].tint_g = 0x80;
    sprite_quads[3].tint_b = 0x80;
    sprite_quads[3].anchor_x = 0;
    sprite_quads[3].anchor_y = 0;
    sprite_quads[3].unk_1C = 0x1000;
    sprite_quads[3].unk_1E = 0x1000;
    sprite_quads[3].unk_20 = 0;
    game_state.quad_layer[3] = 0x84;
    sprite_quads[0].flags = 0;
    sprite_quads[0].x = -0x58;
    sprite_quads[0].y = -0x58;
    sprite_quads[0].w = 0x18;
    sprite_quads[0].h = 0x10;
    sprite_quads[0].unk_0C = 0x1E;
    sprite_quads[0].unk_0E = 0xB0;
    sprite_quads[0].unk_0F = 0;
    sprite_quads[0].unk_10 = 0xB0;
    sprite_quads[0].unk_12 = 0x1E0;
    sprite_quads[0].tint_r = 0x80;
    sprite_quads[0].tint_g = 0x80;
    sprite_quads[0].tint_b = 0x80;
    sprite_quads[0].anchor_x = 0;
    sprite_quads[0].anchor_y = 0;
    sprite_quads[0].unk_1C = 0x1000;
    sprite_quads[0].unk_1E = 0x1000;
    sprite_quads[0].unk_20 = 0;
    game_state.quad_layer[0] = 0x84;
    sprite_quads[1].flags = 0;
    sprite_quads[1].x = 0x30;
    sprite_quads[1].y = -0x58;
    sprite_quads[1].w = 0x18;
    sprite_quads[1].h = 0x10;
    sprite_quads[1].unk_0C = 0x1E;
    sprite_quads[1].unk_0E = 0xB0;
    sprite_quads[1].unk_0F = 0x80;
    sprite_quads[1].unk_10 = 0xB0;
    sprite_quads[1].unk_12 = 0x1E0;
    sprite_quads[1].tint_r = 0x80;
    sprite_quads[1].tint_g = 0x80;
    sprite_quads[1].tint_b = 0x80;
    sprite_quads[1].anchor_x = 0;
    sprite_quads[1].anchor_y = 0;
    sprite_quads[1].unk_1C = 0x1000;
    sprite_quads[1].unk_1E = 0x1000;
    sprite_quads[1].unk_20 = 0;
    game_state.quad_layer[1] = 0x84;
}

/* Initializes the actor used by the data save/load screen. */
void data_save_load_class_init(void) {
    actor_clear(0x60);
    D_80120651 = 3;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = actor_default_sequence_table;
    D_80120684 = actor_default_texture_blocks;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x43;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
}

/* FAKE: KEEP_UNK_1117 is an empty test that gives the byte a second use in each call-free stretch of the else branch, so uopt
 * promotes it to $a3 and reloads it after every call as the original does (register cheat sheet, "narrow selector the
 * original reloads after every call"); the test emits no code. T-17002 */
#define KEEP_UNK_1117() if ((s32)game_state.album_slot) { }
void hud_save_slot_info_show(s32 arg0, s32 left_summary_column) {
    s32 i;
    s32 cleared_girl_count;
    s32 summary_x;
    s32 j; /* FAKE: unused; keeps the original frame slot between summary_x and slot_date_text (every declared local reserves a slot), real local unknown. T-17002 */
    u8 slot_date_text[32];
    u8 player_name_text[32];
    u8 nickname_text[32];
    u8 club_name_text[32];
    u8 clear_count_text[32];
    s32 sp44; /* FAKE: unused; keeps the original frame slot below clear_count_text, real local unknown. T-17002 */

    if (left_summary_column != 0) {
        summary_x = -0x8C;
    } else {
        summary_x = -8;
    }
    k_sub_reset();
    if ((u8) D_800E8BEE != 0) {
        cleared_girl_count = 0;
        for (i = 0; i < 13; i++) {
            if (system_data.flags[i] != 0) {
                cleared_girl_count += 1;
            }
        }
        strcpy(slot_date_text, D_800B3288[D_800E7D36]);
        strncat(slot_date_text, "番アルバム", 2);
        set_kanji_string((s16) (summary_x + 0xC), -0x50, 0xFU, slot_date_text, 0);
        /* FAKE: the byte D_800E7D36 read through the SystemData view; uopt does not CSE it with the D_800E7D36 loads
         * in the else arm, the original reloads it there. Real source unknown. T-17002 */
        if (system_data.selected_slot == 0) {
            set_kanji_string((s16) summary_x, -0x40, 0xFU, "システム", 0);
        } else {
            if (system_data.slot_status[D_800E7D36] == 0) {
                set_kanji_string((s16) summary_x, -0x40, 0xFU, "未使用", 0);
            } else if (system_data.slot_status[D_800E7D36] == 0xFF) {
                set_kanji_string((s16) summary_x, -0x40, 0xFU, "データ異常", 0);
            } else {
                strcpy(slot_date_text, D_800B3288[(u8) system_data.year[D_800E7D36] / 10U]);
                strncat(slot_date_text, D_800B3288[(u8) system_data.year[D_800E7D36] % 10U], 2);
                strncat(slot_date_text, "年", 2);
                strncat(slot_date_text, D_800B3288[system_data.month[D_800E7D36]], 4);
                strncat(slot_date_text, "月", 2);
                if (system_data.day[D_800E7D36] >= 0xAU) {
                    strncat(slot_date_text, D_800B3288[system_data.day[D_800E7D36] / 10U], 2);
                }
                strncat(slot_date_text, D_800B3288[system_data.day[D_800E7D36] % 10U], 2);
                strncat(slot_date_text, "日", 2);
                set_kanji_string((s16) summary_x, -0x40, 0xFU, slot_date_text, 0);
                strcpy(player_name_text, "名前…");
                strncat(player_name_text, system_data.family_name[D_800E7D36], 6);
                strncat(player_name_text, system_data.given_name[D_800E7D36], 6);
                strcpy(nickname_text, "あだ名…");
                strncat(nickname_text, system_data.nickname[D_800E7D36], 0xD);
                set_kanji_string((s16) summary_x, -0x30, 0xFU, player_name_text, 0);
                set_kanji_string((s16) summary_x, -0x20, 0xFU, nickname_text, 0);
                strcpy(club_name_text, "所属部‥");
                /* two arguments as in the original: no count is loaded into $a2 before this call */
                strncat(club_name_text, club_name_table[system_data.club[D_800E7D36]]);
                set_kanji_string((s16) summary_x, -0x10, 0xFU, club_name_text, 0);
                strcpy(clear_count_text, "クリア人数‥");
                if (cleared_girl_count >= 0x64) {
                    cleared_girl_count = 0x63;
                }
                if (cleared_girl_count >= 0xA) {
                    strncat(clear_count_text, D_800B3288[cleared_girl_count / 10], 2);
                }
                strncat(clear_count_text, D_800B3288[cleared_girl_count % 10], 2);
                set_kanji_string((s16) summary_x, 0, 0xFU, clear_count_text, 0);
            }
        }
    } else {
        cleared_girl_count = 0;
        KEEP_UNK_1117();
        for (i = 0; i < 13; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].flags[i] != 0) {
                cleared_girl_count += 1;
            }
        }
        if (system_data.slot_status[game_state.save_slot] == 0) {
            set_kanji_string((s16) summary_x, -0x40, 0xFU, "未使用", 0);
        } else if (system_data.slot_status[game_state.save_slot] == 0xFF) {
            set_kanji_string((s16) summary_x, -0x40, 0xFU, "データ異常", 0);
        } else {
            strcpy(slot_date_text, D_800B3288[game_state.save_slot]);
            strncat(slot_date_text, "番アルバム", 2);
            set_kanji_string((s16) (summary_x + 0xC), -0x50, 0xFU, slot_date_text, 0);
            KEEP_UNK_1117();
            strcpy(slot_date_text, D_800B3288[(u8) ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].year / 10U]);
            KEEP_UNK_1117();
            strncat(slot_date_text, D_800B3288[(u8) ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].year % 10U], 2);
            strncat(slot_date_text, "年", 2);
            KEEP_UNK_1117();
            strncat(slot_date_text, D_800B3288[((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].month], 4);
            strncat(slot_date_text, "月", 2);
            KEEP_UNK_1117();
            if (((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].day >= 0xAU) {
                strncat(slot_date_text, D_800B3288[((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].day / 10U], 2);
            }
            strncat(slot_date_text, D_800B3288[((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].day % 10U], 2);
            strncat(slot_date_text, "日", 2);
            set_kanji_string((s16) summary_x, -0x40, 0xFU, slot_date_text, 0);
            strcpy(player_name_text, "名前…");
            KEEP_UNK_1117();
            strncat(player_name_text, ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].family_name, 6);
            KEEP_UNK_1117();
            strncat(player_name_text, ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].given_name, 6);
            strcpy(nickname_text, "あだ名…");
            KEEP_UNK_1117();
            strncat(nickname_text, ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].nickname, 0xD);
            set_kanji_string((s16) summary_x, -0x30, 0xFU, player_name_text, 0);
            set_kanji_string((s16) summary_x, -0x20, 0xFU, nickname_text, 0);
            strcpy(club_name_text, "所属部‥");
            KEEP_UNK_1117();
            /* two arguments as in the original: no count is loaded into $a2 before this call */
            strncat(club_name_text, club_name_table[((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].club >> 12]);
            set_kanji_string((s16) summary_x, -0x10, 0xFU, club_name_text, 0);
            strcpy(clear_count_text, "クリア人数‥");
            if (cleared_girl_count >= 0x64) {
                cleared_girl_count = 0x63;
            }
            if (cleared_girl_count >= 0xA) {
                strncat(clear_count_text, D_800B3288[cleared_girl_count / 10], 2);
            }
            strncat(clear_count_text, D_800B3288[cleared_girl_count % 10], 2);
            set_kanji_string((s16) summary_x, 0, 0xFU, clear_count_text, 0);
        }
    }
    k_sub_disp_start(6);
}

/* Implicit int in the original (K&R definition, no return): $v0 stays live at the exit, so the last
 * bit store of each case arm also goes through the promoted byte in $v0. T-25103. */
s32 hud_magazine_mark_read(arg0, arg1)
s32 arg0;
s32 arg1;
{
    u8 x;

    /* FAKE: T-25103, the dead read numbers the flag byte before the month (uopt colours unconstrained
     * ranges in that order), so the byte takes $v0 and the month $v1; uopt then removes the load. */
    x = game_state.quarter_flags[1];
    if (game_state.month != 2 && game_state.month != 5 && game_state.month != 8 && game_state.month != 11) {
        ((QuarterFlagBits *)game_state.quarter_flags)->b14 = 1;
    } else {
        ((QuarterFlagBits *)game_state.quarter_flags)->b13 = 1;
    }
    if (game_state.year == 0x5F && game_state.month == 4) {
        ((QuarterFlagBits *)game_state.quarter_flags)->b13 = 1;
        ((QuarterFlagBits *)game_state.quarter_flags)->b14 = 1;
    }
    switch (game_state.month) {
    case 2:
    case 3:
    case 4:
        if (game_state.year == 0x5F) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b0 = 1;
        }
        if (game_state.year == 0x60) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b4 = 1;
        }
        if (game_state.year == 0x61) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b8 = 1;
        }
        break;
    case 5:
    case 6:
    case 7:
        if (game_state.year == 0x5F) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b1 = 1;
        }
        if (game_state.year == 0x60) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b5 = 1;
        }
        if (game_state.year == 0x61) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b9 = 1;
        }
        ((QuarterFlagBits *)game_state.quarter_flags)->b15 = 1;
        break;
    case 8:
    case 9:
    case 10:
        if (game_state.year == 0x5F) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b2 = 1;
        }
        if (game_state.year == 0x60) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b6 = 1;
        }
        if (game_state.year == 0x61) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b10 = 1;
        }
        break;
    case 11:
    case 12:
    case 13:
        if (game_state.year == 0x5F) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b3 = 1;
        }
        if (game_state.year == 0x60) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b7 = 1;
        }
        if (game_state.year == 0x61) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b11 = 1;
        }
        break;
    case 1:
        if (game_state.year == 0x60) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b3 = 1;
        }
        if (game_state.year == 0x61) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b7 = 1;
        }
        if (game_state.year == 0x62) {
            ((QuarterFlagBits *)game_state.quarter_flags)->b11 = 1;
        }
        break;
    }
}

/* FAKE: each record triple (unk_18/unk_26/unk_2A) after the loop is written on one line, as from a macro (none is known):
 * as1 schedules by source line and the grouping sets the store order in each branch. T-11690 */
void hud_magazine_page_build(void) {
    u8 eight; /* FAKE: a named 8 gives the original `beq a1,t0` in cases 8-10 (a literal gives `beq t0,a1`). T-11690 */
    s32 i;
    s32 calendar_actor_index;
    s32 pad; /* FAKE: keeps the original frame slot between calendar_actor_index and magazine_issue_index; real local unknown. T-11690 */
    s32 magazine_issue_index;

    eight = 8;
    /* FAKE: dead reads of both selectors; without them uopt does not promote unk_03E and gives unk_03F $a2 instead
     * of $t0. T-11690 */
    i = game_state.month;
    calendar_actor_index = game_state.year;
    actor_clear_upper();
    actor_clear(0x60);
    actor_pool[96].unk_01 = 5;
    actor_pool[96].flags = 0x40;
    actor_pool[96].quad_flags = 0x40000000;
    actor_pool[96].draw_flags = 0x84;
    actor_pool[96].frame_table = sprite_frame_table;
    actor_pool[96].sequence_table = actor_default_sequence_table;
    actor_pool[96].tpage_table = actor_default_texture_blocks;
    actor_pool[96].unk_14 = D_800C9A54;
    actor_pool[96].unk_04 = 8;
    actor_pool[96].unk_05 = 1;
    actor_pool[96].unk_18 = 0x76;
    actor_pool[96].x = 0;
    actor_pool[96].y = -0x28;
    for (i = 0; i < 0x20; i++) {
        actor_clear(i + 0x61);
        actor_pool[i + 0x61].unk_01 = 4;
        actor_pool[i + 0x61].flags = 0x40;
        actor_pool[i + 0x61].quad_flags = 0;
        actor_pool[i + 0x61].draw_flags = 0x84;
        actor_pool[i + 0x61].frame_table = sprite_frame_table;
        actor_pool[i + 0x61].sequence_table = actor_default_sequence_table;
        actor_pool[i + 0x61].tpage_table = actor_default_texture_blocks;
        actor_pool[i + 0x61].unk_14 = D_800C9A54;
        actor_pool[i + 0x61].unk_04 = 8;
        actor_pool[i + 0x61].unk_05 = 1;
        actor_pool[i + 0x61].blink_period = 8;
        actor_pool[i + 0x61].unk_18 = 0;
        actor_pool[i + 0x61].x = 0x100;
        actor_pool[i + 0x61].y = 0x100;
        actor_pool[i + 0x61].brightness = 0x80;
    }
    calendar_actor_index = 0x61;
    switch (game_state.month) {
    case 2:
    case 3:
    case 4:
        if (game_state.year == 0x5F) {
            actor_pool[0x61].unk_18 = 0x79;
            actor_pool[0x61].x = 0;
            magazine_issue_index = 0;
            actor_pool[0x61].y = -0x28;
            calendar_actor_index++;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E0C, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E25, 0);
        }
        if (game_state.year == 0x60) {
            magazine_issue_index = 4;
            if (game_state.month != 2) {
                game_state.progress_state[13] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x7A; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
            actor_pool[calendar_actor_index].unk_18 = 0x81; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x61) {
            magazine_issue_index = 8;
            if (game_state.month != 2) {
                game_state.progress_state[11] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x7A; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
            actor_pool[calendar_actor_index].unk_18 = 0x84; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x62) {
            magazine_issue_index = 0xC;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        actor_pool[0x61].unk_18 = 0x7B; actor_pool[0x61].x = 0; actor_pool[0x61].y = -0x28;
        calendar_actor_index++;
        if (game_state.year == 0x5F) {
            magazine_issue_index = 1;
            if (game_state.month != 5) {
                game_state.progress_state[1] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x7E; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x60) {
            magazine_issue_index = 5;
            if (game_state.month != 5) {
                game_state.progress_state[5] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x82; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x61) {
            magazine_issue_index = 9;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 8:
    case 9:
    case 10:
        actor_pool[0x61].unk_18 = 0x7C; actor_pool[0x61].x = 0; actor_pool[0x61].y = -0x28;
        calendar_actor_index++;
        if (game_state.year == 0x5F) {
            magazine_issue_index = 2;
            if (game_state.month != eight) {
                game_state.progress_state[7] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x7F; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x60) {
            magazine_issue_index = 6;
            if (game_state.month != eight) {
                game_state.progress_state[10] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x83; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x61) {
            magazine_issue_index = 0xA;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 11:
    case 12:
        actor_pool[0x61].unk_18 = 0x7D; actor_pool[0x61].x = 0; actor_pool[0x61].y = -0x28;
        calendar_actor_index++;
        if (game_state.year == 0x5F) {
            magazine_issue_index = 3;
            if (game_state.month == 0xC) {
                game_state.progress_state[3] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x80; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x60) {
            magazine_issue_index = 7;
            if (game_state.month == 0xC) {
                game_state.progress_state[6] = 1;
            }
            actor_pool[calendar_actor_index].unk_18 = 0x85; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x61) {
            magazine_issue_index = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 1:
        actor_pool[0x61].unk_18 = 0x7D; actor_pool[0x61].x = 0; actor_pool[0x61].y = -0x28;
        calendar_actor_index++;
        if (game_state.year == 0x60) {
            magazine_issue_index = 3;
            game_state.progress_state[3] = 1;
            actor_pool[calendar_actor_index].unk_18 = 0x80; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x61) {
            magazine_issue_index = 7;
            game_state.progress_state[6] = 1;
            actor_pool[calendar_actor_index].unk_18 = 0x85; actor_pool[calendar_actor_index].x = 8; actor_pool[calendar_actor_index].y = -0x28;
            calendar_actor_index++;
        }
        if (game_state.year == 0x62) {
            magazine_issue_index = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    }
    if ((game_state.year != 0x5F) || (game_state.month >= 6U)) {
        actor_pool[calendar_actor_index].unk_18 = 0x77; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    }
    if (game_state.year == 0x5F) {
        actor_pool[calendar_actor_index].unk_18 = (s16) (magazine_issue_index + 0x97); actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    } else if ((game_state.year == 0x60) && (game_state.month < 2U)) {
        actor_pool[calendar_actor_index].unk_18 = (s16) (magazine_issue_index + 0x97); actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    } else {
        actor_pool[calendar_actor_index].unk_01++;
        actor_pool[calendar_actor_index].unk_18 = 0x78; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
        actor_pool[calendar_actor_index].unk_18 = (s16) (D_800B3750[magazine_issue_index][0] + 0x90); actor_pool[calendar_actor_index].x = 0x110; actor_pool[calendar_actor_index].y = 0x18;
        calendar_actor_index++;
        actor_pool[calendar_actor_index].unk_18 = (s16) (D_800B3750[magazine_issue_index][1] + 0x90); actor_pool[calendar_actor_index].x = 0x110; actor_pool[calendar_actor_index].y = 0x28;
        calendar_actor_index++;
        actor_pool[calendar_actor_index].unk_18 = (s16) (D_800B3750[magazine_issue_index][2] + 0x90); actor_pool[calendar_actor_index].x = 0x110; actor_pool[calendar_actor_index].y = 0x38;
        calendar_actor_index++;
        set_kanji_string(0x100, 0x10, 6U, D_800B32CC[(magazine_issue_index * 3 + 2) % 12], 0);
        set_kanji_string(0x100, 0x20, 6U, D_800B32CC[(magazine_issue_index * 3 + 3) % 12], 0);
        set_kanji_string(0x100, 0x30, 6U, D_800B32CC[(magazine_issue_index * 3 + 4) % 12], 0);
    }
    if (D_800B5DFC[magazine_issue_index] != 0xFF) {
        actor_pool[calendar_actor_index].unk_18 = (s16) D_800B5DFC[magazine_issue_index]; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    }
    set_kanji_string(0x20, -0x5E, 7U, D_800B5D1C[magazine_issue_index], 0);
    if (D_800B5D0C[magazine_issue_index] != 0xFF) {
        actor_pool[calendar_actor_index].unk_18 = (s16) D_800B5D0C[magazine_issue_index]; actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    }
    set_kanji_string(0x20, -0x3E, 8U, D_800B5C2C[magazine_issue_index], 0);
    if (magazine_issue_index < 4) {
        actor_pool[calendar_actor_index].unk_18 = (s16) ((magazine_issue_index & 3) + 0x97); actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    }
    if (magazine_issue_index >= 4) {
        actor_pool[calendar_actor_index].unk_18 = (s16) ((magazine_issue_index & 3) + 0x93); actor_pool[calendar_actor_index].x = 0; actor_pool[calendar_actor_index].y = -0x28;
        calendar_actor_index++;
    }
    set_kanji_string(-0x8F, -0xE, 6U, D_800B5EA4[magazine_issue_index][0], 0);
    set_kanji_string(-0x8F, 2, 6U, D_800B5EA4[magazine_issue_index][1], 0);
    set_kanji_string(-0x8F, 0x12, 6U, D_800B5EA4[magazine_issue_index][2], 0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    gnsx(&game_state.player_family_name[0]);
    sndisp(D_800B66A4[magazine_issue_index], 0, 0x1F);
}

s32 hud_appointment_find_today(s32 calendar_year, s32 calendar_month, s32 day_of_month) {
    s32 i;
    s32 r;

    r = 0;
    if (calendar_month == 0xD) {
        calendar_month = 1;
    }
    if (calendar_month == 0xC && day_of_month == 0x18) {
        return -1;
    }
    if (calendar_month == 0xD) {
        if (day_of_month == 1) {
            return -1;
        }
    }
    if (calendar_month == 1 && day_of_month == 1) {
        return -1;
    }
    if (calendar_month >= 3 && calendar_year >= 0x62) {
        return -1;
    }
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 8; i++) if (calendar_month == ((u32) game_state.unk_54C[i].h[0] >> 12) && day_of_month == (game_state.unk_54C[i].h[1] & 0x1F)) r = i + 1;
    return r;
}


s32 hud_appointment_count_for_girl(s32 girl_id) {
    s32 i;
    s32 appointment_count;

    appointment_count = 0;
    for (i = 0; i < 8; i++) {
        if (((AppointmentBits *)&game_state.unk_54C[i])->active && girl_id == ((AppointmentBits *)&game_state.unk_54C[i])->girl) {
            appointment_count++;
        }
    }
    return appointment_count;
}

/* Packs the active schedule entries to the front, clears the rest, and sorts the first n by month * 100 + day
 * (selection sort). While unk_03F is 12, month-1 entries count as month 13 during the sort, so they go last.
 * Every access goes through the GameState pointer g (T-25608: the original's loop preheaders). */
#define SCHED_DATE(i) ((AppointmentDate *)&g->unk_54C[i])
void hud_appointment_list_sort(void) {
    s32 i;
    s32 j;
    s32 active_count;
    s32 earliest_slot_index;
    WordView appointment_entry_temp;
    s32 appointment_date_keys[8];
    s32 earliest_date_key;
    s32 pad[9]; /* FAKE: T-25608, unused; the original frame (0xB0, a0 spill at 0x40) has nine more words below g. */
    GameState *g;

    g = &game_state;
    for (active_count = 0, j = 0; j < 8; j++) {
        if (((AppointmentBits *)&g->unk_54C[j])->active) {
            g->unk_54C[active_count++] = g->unk_54C[j];
        }
    }
    for (j = active_count; j < 8; j++) {
        bzero(&g->unk_54C[j], 4);
    }
    if (g->month == 12) {
        for (j = 0; j < active_count; j++) {
            if (SCHED_DATE(j)->month == 1) {
                SCHED_DATE(j)->month = 13;
            }
        }
    }
    i = 0;
    while (i < active_count) {
        earliest_date_key = SCHED_DATE(i)->day + SCHED_DATE(i)->month * 100;
        earliest_slot_index = i;
        for (j = i + 1; j < active_count; j++) {
            appointment_date_keys[j] = SCHED_DATE(j)->day + SCHED_DATE(j)->month * 100;
            if (appointment_date_keys[j] < earliest_date_key) {
                earliest_date_key = appointment_date_keys[j];
                earliest_slot_index = j;
            }
        }
        /* FAKE: T-25608 (from T-25103), the label splits the swap off the inner-loop exit; without it uopt keeps
         * &game_state in a register from the first loop to the swap, the original never does. */
        goto swap;
    swap:
        appointment_entry_temp = g->unk_54C[i];
        g->unk_54C[i] = g->unk_54C[earliest_slot_index];
        g->unk_54C[earliest_slot_index] = appointment_entry_temp;
        i++;
    }
    for (j = 0; j < active_count; j++) {
        if (SCHED_DATE(j)->month == 13) {
            SCHED_DATE(j)->month = 1;
        }
    }
}
#undef SCHED_DATE

void hud_appointment_add(WordView appointment_entry) {
    s32 insertion_slot_index;

    insertion_slot_index = 0;
    if (((AppointmentBits *)&game_state.unk_54C[insertion_slot_index])->active) {
        if (insertion_slot_index) { /* FAKE: the empty test keeps insertion_slot_index from folding to 0, the original has `sll t9,zero,2; addu v0,t9,base`; real source unknown. T-9340 */
        }
        do {
            insertion_slot_index++;
        } while (((AppointmentBits *)&game_state.unk_54C[insertion_slot_index])->active && insertion_slot_index != 8);
    }
    game_state.unk_54C[insertion_slot_index] = appointment_entry;
    hud_appointment_list_sort();
}

void hud_appointment_prompt_noop(void) {
}

void hud_club_list_menu(void) {
    s32 pad[2]; /* FAKE: reproduces the original's 8-byte gap above `first_available_club_id`; real locals unknown. T-11690 */
    s32 first_available_club_id;
    s32 i;
    s16 x[12];
    s16 y[12];
    s16 w[12];
    s16 h[12];

    actor_clear(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = actor_default_sequence_table;
    D_80120684 = actor_default_texture_blocks;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x3F;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
    set_kanji_string(-0x80, -0x52, 3, "部活一覧", 0);
    first_available_club_id = -1;
    for (i = 0; i < 0xB; i++) {
        x[i] = (i / 5) * 0x54 - 0x80;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x40;
        h[i] = 0x10;
        if (!(game_state.clubs[i].flags & 1)) {
            set_kanji_string(x[i], y[i], 3, club_name_table[i], 0);
            if (first_available_club_id == -1) {
                first_available_club_id = i;
            }
        } else {
            w[i] = 0;
        }
    }
    i = 11;
    x[i] = 0x28;
    y[i] = 8;
    w[i] = 0x40;
    h[i] = 0x10;
    set_kanji_string(x[i], y[i], 3, "取り消し", 0);
    menu_set(1, 0xC, x, y, w, h);
    game_state.menu_cursor_1 = first_available_club_id;
    k_sub_reset_point_set();
    sndi("入部する部活を選択してください", 0, 0xFF);
    k_sub_disp_start(1);
}

void hud_sprite_light_effect_draw(void) {
    s32 i;

    _sprite_set_light_effect1(-0x120, -0xF8, D_800B66E0[0], 0x3C, 0xA0, 0x3C, 9, 0xA0A0A0, D_800B66F4[0]);
    for (i = 1; i < 9; i++) {
        _sprite_set_light_effect1(-0x120, -0xF8, D_800B66E0[i], 0x3C, 0, 0xB4, 9, 0xA0A0A0, D_800B66F4[i]);
    }
    dtd_on_tpage(0, 0, 9, 1, 0);
    _sprite_set_light_effect1(-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
}

void hud_cursor_update(void) {
    GameState *g = &game_state;
    u8 base_speed;
    s32 movement_step;
    u8 temp; /* FAKE: T-25003, first test through a u8 copy keeps the D_800B6708 value in $v0 */

    if ((u8)D_8011ECA4 == 0) {
        base_speed = g->cursor_speed;
        D_8011ECA4 = (u8)D_8011ECA4 + 1;
        if (base_speed == 0) {
            base_speed = 1;
        }
        g->cursor_speed = base_speed;
        movement_step = base_speed;
        if (!(g->menu_mode & 3) && (g->pad_held & 0xFF)) {
            movement_step = base_speed * 2;
        }
        /* FAKE: T-25008, the u8 cast spends the ugen temporary the original spends before the switch */
        if ((u8)g->pad_held & 0xF) {
        } else {
            switch (g->pad_held & 0x5000) {
            case 0x1000:
                cursor_y -= movement_step;
                break;
            case 0x4000:
                cursor_y += movement_step;
                break;
            }
            switch (g->pad_held & 0xA000) {
            case 0x2000:
                cursor_x += movement_step;
                break;
            case 0x8000:
                cursor_x -= movement_step;
                break;
            }
        }
        if (cursor_x >= 0x8D) {
            cursor_x = 0x8C;
        }
        if (cursor_x < -0x96) {
            cursor_x = -0x96;
        }
        if (cursor_y < -0x64) {
            cursor_y = -0x64;
        }
        if (cursor_y >= 0x65) {
            cursor_y = 0x64;
        }
        if (g->pad_pressed & PAD_SQUARE) {
            temp = D_800B6708;
            if (temp == 0) {
                D_800B6708 = 0x9B;
            } else if (D_800B6708 == 0xAC) {
                D_800B6708 = 0;
            } else {
                D_800B6708++;
            }
            D_8011ECE8 = D_800B6708;
        }
    }
}

/* FAKE: the empty `if ((s32)D_800B6708) {}` gives the byte a use in the dispatch block, so uopt promotes it and loads it
 * once into $v0 before the arg0 tests; without it each arm loads it. The test emits no code. T-17002 */
void hud_cursor_actor_init(s32 arg0) {
    s16 sp2E;
    s16 sp2C;

    sp2E = actor_pool[0].x;
    sp2C = actor_pool[0].y;
    actor_clear(0);
    actor_pool[0].unk_01 = 1;
    actor_pool[0].flags = 0x40;
    actor_pool[0].quad_flags = 0;
    actor_pool[0].draw_flags = 0x84;
    actor_pool[0].frame_table = sprite_frame_table;
    actor_pool[0].sequence_table = actor_default_sequence_table;
    actor_pool[0].tpage_table = actor_default_texture_blocks;
    actor_pool[0].unk_14 = D_800C9A54;
    actor_pool[0].unk_04 = 8;
    actor_pool[0].unk_05 = 1;
    actor_pool[0].sequence_slot = 0;
    if ((s32)D_800B6708) {
    }
    switch (arg0) {
    case 0:
    default:
        if (D_800B6708 == 0 || (s32)D_800B6708 < 0x9B || (actor_pool[0].unk_18 = D_800B6708, (s32)D_800B6708 >= 0xAE)) {
            actor_pool[0].unk_18 = 0;
        }
        break;
    case 1:
        if (D_800B6708 == 0 || (s32)D_800B6708 < 0x9B || (actor_pool[0].unk_18 = D_800B6708, (s32)D_800B6708 >= 0xAE)) {
            actor_pool[0].unk_18 = 0;
        }
        break;
    }
    actor_pool[0].x = sp2E;
    actor_pool[0].y = sp2C;
}

void hud_cursor_show(s32 arg0) {
    if (arg0 == 1) {
        D_8011ECD3 |= 0x80;
    } else {
        D_8011ECD3 &= 0x7F;
    }
}

s32 hud_command_roll_poor_result(s32 days_to_advance) {
    /* FAKE: unused stack word matches original 0x40 frame size and local offsets. T-11060 */
    s32 pad;
    s32 command_score;
    s32 zero_wave_count;
    u16 day_count_to_restore;
    u16 original_day_count;
    s32 stress_factor;

    original_day_count = game_state.day_count;
    day_count_to_restore = original_day_count;
    game_state.day_count = original_day_count + days_to_advance;
    command_score = 0;
    switch (game_state.last_command.w & 15) {
    case 0:
        command_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_liberal_arts.value / 16;
        break;
    case 1:
        command_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_science.value / 16;
        break;
    case 2:
        command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_art.value / 16;
        break;
    case 3:
        command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_athletics.value / 16;
        break;
    case 4:
        switch ((u16)game_state.player_flags.h >> 12) {
        case 0:
            command_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 1:
            command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 2:
            command_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 3:
            command_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 4:
            command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 5:
            command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 6:
            command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 7:
            command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 8:
            command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 9:
            command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        case 10:
            command_score = hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_SCORE_SCALE);
            break;
        }
        command_score += (u16)game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >> 4;
        break;
    case 5:
        command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_trivia.value / 16;
        break;
    case 6:
        command_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE) + game_state.param_looks.value / 16;
        break;
    case 7:
        hud_biorhythm_value(1, 1, 1, HUD_BIORHYTHM_SCORE_SCALE);
        command_score = 16;
        break;
    }
    command_score += 6;
    if (game_state.param_stress.value >= 97) {
        stress_factor = 4;
    } else {
        stress_factor = game_state.param_stress.value / 32;
    }
    command_score = rand() % (command_score + 4);
    command_score = (8 - stress_factor) * command_score / 8;
    zero_wave_count = HUD_BIORHYTHM_ZERO_FAILURES_NONE;
    if (hud_biorhythm_value(1, 0, 0, HUD_BIORHYTHM_ZERO_WAVE_PROBE_SCALE) == 0) {
        zero_wave_count++;
    }
    if (hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_ZERO_WAVE_PROBE_SCALE) == 0) {
        zero_wave_count++;
    }
    if (hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_ZERO_WAVE_PROBE_SCALE) == 0) {
        zero_wave_count++;
    }
    if (zero_wave_count == HUD_BIORHYTHM_ZERO_FAILURES_ALL_THREE) {
        command_score /= 4;
    }
    game_state.day_count = day_count_to_restore;
    if (command_score >= 4) {
        return 0;
    }
    return 1;
}

void hud_biorhythm_probe_discard(void) {
    hud_biorhythm_value(1, 1, 1, HUD_BIORHYTHM_DISCARDED_PROBE_SCALE);
}

s32 hud_biorhythm_value(s32 include_23_day_wave, s32 include_28_day_wave, s32 include_33_day_wave, s32 scale) {
    s32 absolute_wave_magnitude;
    s32 birthday_phase_offset;
    s32 biorhythm_day;
    s32 wave_23_day;
    s32 wave_28_day;
    s32 wave_33_day;
    s32 critical_wave_count;
    s32 selected_wave_count;
    s32 combined_wave_total;

    wave_23_day = rsin((biorhythm_day = (birthday_phase_offset = hud_biorhythm_birth_offset()) + (u16)game_state.day_count) * 0xB2);
    wave_28_day = rsin(biorhythm_day * 0x92);
    wave_33_day = rsin(biorhythm_day * 0x7C);
    critical_wave_count = 0;
    selected_wave_count = 0;
    combined_wave_total = 0;
    if (include_23_day_wave != 0) {
        selected_wave_count++;
        if ((absolute_wave_magnitude = abs(wave_23_day)) < 0x199) {
            combined_wave_total -= 0x1000;
            critical_wave_count++;
        } else if (wave_23_day > 0) {
            combined_wave_total += wave_23_day;
        } else {
            combined_wave_total -= wave_23_day / 2;
        }
    }
    if (include_28_day_wave != 0) {
        selected_wave_count++;
        if ((absolute_wave_magnitude = abs(wave_28_day)) < 0x199) {
            combined_wave_total -= 0x1000;
            critical_wave_count++;
        } else if (wave_28_day > 0) {
            combined_wave_total += wave_28_day;
        } else {
            combined_wave_total -= wave_28_day / 2;
        }
    }
    if (include_33_day_wave != 0) {
        selected_wave_count++;
        if ((absolute_wave_magnitude = abs(wave_33_day)) < 0x199) {
            combined_wave_total -= 0x1000;
            critical_wave_count++;
        } else if (wave_33_day > 0) {
            combined_wave_total += wave_33_day;
        } else {
            combined_wave_total -= wave_33_day / 2;
        }
    }
    if (selected_wave_count == 0) return 1;
    if (scale > 0) {
        if (combined_wave_total / selected_wave_count < 0) return 0;
        return (combined_wave_total * scale / 4096) / selected_wave_count;
    } else if (scale < 0) {
        if (include_23_day_wave != 0) return wave_23_day * scale / 4096;
        if (include_28_day_wave != 0) return wave_28_day * scale / 4096;
        if (include_33_day_wave != 0) return wave_33_day * scale / 4096;
    } else {
        /* The original retains this selected_wave_count check in the zero-scale path. T-10880. */
        if (selected_wave_count == 0) return 1;
        if (selected_wave_count == 1) {
            if (critical_wave_count != 0) return -1;
            return 1;
        }
        if (selected_wave_count == 2) {
            if (critical_wave_count != 0) return -1;
            return 1;
        }
        if (selected_wave_count == 3) {
            if (critical_wave_count >= 2) return -1;
            else return 1;
        }
        return 1;
    }
}

s32 hud_biorhythm_birth_offset(void) {
    s32 birthday_phase_offset;
    birthday_phase_offset = -(s32)D_800B670A[game_state.player_birthday & 0xF] - (((u32)game_state.player_birthday << 23) >> 27) + 0x1563;
    if ((game_state.player_birthday & 0xF) == 4 && (((u32)game_state.player_birthday << 23) >> 27) < 4U) {
        birthday_phase_offset = -(s32)D_800B670A[3] - (((u32)game_state.player_birthday << 23) >> 27) + 0x1582;
    }
    return birthday_phase_offset;
}

void hud_corner_icon_set(s32 arg0) {
    actor_clear(0xF);
    D_8011F0CD = 0;
    D_8011F0CE = 0x40;
    D_8011F104 = 0;
    if (arg0) {
        D_8011F0CF = 0x8C;
    } else {
        D_8011F0CF = 0;
    }
    D_8011F0D8 = sprite_frame_table;
    D_8011F0DC = actor_default_sequence_table;
    D_8011F100 = actor_default_texture_blocks;
    D_8011F0E0 = D_800C9A54;
    D_8011F0D0 = 8;
    D_8011F0D1 = 1;
    D_8011F10F = 0;
    D_8011F0E2 = 0;
    D_8011F0E4 = 0xAD;
    D_8011F0F2 = -0x86;
    D_8011F0F6 = -0x5C;
}

void hud_text_marker_set(s32 arg0) {
    actor_clear(2);
    D_8011ED59 = 2;
    D_8011ED5A = 0x40;
    D_8011ED90 = 0x40000000;
    D_8011ED64 = sprite_frame_table;
    D_8011ED68 = actor_default_sequence_table;
    D_8011ED8C = actor_default_texture_blocks;
    D_8011ED70 = 1;
    D_8011ED5C = 8;
    D_8011ED5D = 1;
    D_8011ED7E = 0x74;
    D_8011ED82 = 0x48;
    if (arg0 == 1) {
        D_8011ED5B |= 0x80;
    } else {
        D_8011ED5B &= 0x7F;
    }
}

s16 hud_text_marker_bob(void) {
    if (D_8011ED82 < 0x4F) {
        if (!(game_state.frame_count & 3)) {
            D_8011ED82 += 1;
        }
    } else {
        D_8011ED82 = 0x48;
    }
}

/* Scans the event records of game_state.event_log (unk_04 == 0 ends the list): `last` gets the
 * unk_00 of the final record, `first` that of the final record whose type unk_02 is 0, 1, 2 or
 * 0xF. Clears unk_04 of every record after the list and returns the count, at most 0xFF.
 * T-25103: the for loop orders the induction init before `i = 0`, and `n++` ahead of the join copy
 * keeps `cur = val` (s32, a plain move) at the join instead of a narrowing copy in each arm. */
s32 hud_event_log_scan(u8 *last_qualifying_girl_out, u8 *final_event_girl_out, s32 arg2) {
    GameState *g = &game_state;
    s32 i;
    s32 event_record_count;
    u8 last_qualifying_girl_id;
    s32 final_event_girl_id;
    u8 current_event_girl_id;
    u8 scene_id;

    /* FAKE: T-25103 (from T-24928); the dead tests give last_qualifying_girl_out/final_event_girl_out an early use so their webs are
     * coloured $a0/$a1 before the value local's web, which then takes $t0 and pushes the record
     * base to $t1 as in the original. Without them current_event_girl_id takes $a0 and last_qualifying_girl_out is spilled. The tests
     * emit no code. */
    if (last_qualifying_girl_out) {
    }
    if (final_event_girl_out) {
    }
    last_qualifying_girl_id = 0xFF;
    final_event_girl_id = 0;
    event_record_count = 0;
    for (i = 0; i < 0x100; i++) {
        if (g->event_log[event_record_count].year == 0) {
            break;
        }
        scene_id = g->event_log[event_record_count].scene;
        if (scene_id != 0 && scene_id != 1 && scene_id != 2 && scene_id != 0xF) {
            current_event_girl_id = g->event_log[event_record_count].girl;
        } else {
            current_event_girl_id = g->event_log[event_record_count].girl;
            last_qualifying_girl_id = current_event_girl_id;
        }
        event_record_count++;
        final_event_girl_id = current_event_girl_id;
    }
    for (i = event_record_count; i < 0x100; i++) {
        g->event_log[i].year = 0;
    }
    *last_qualifying_girl_out = last_qualifying_girl_id;
    *final_event_girl_out = final_event_girl_id;
    if (event_record_count >= 0xFF) {
        event_record_count = 0xFF;
    }
    return event_record_count;
}

u8 *hud_confirm_button_label(void) {
    if (game_state.input_device == INPUT_MOUSE) {
        if (game_state.confirm_button != SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT) {
            return D_800B672C;
        }
        return D_800B6730;
    }
    return D_800B6724;
}

u8 *hud_cancel_button_label(void) {
    if (game_state.input_device == INPUT_MOUSE) {
        if (game_state.confirm_button != SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT) {
            return D_800B6730;
        }
        return D_800B672C;
    }
    return D_800B6728;
}
