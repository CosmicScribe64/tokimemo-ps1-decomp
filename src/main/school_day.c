/* school_day.c: original object text 0x800737A0-0x80075320 in config/objects/main.txt; matched C implements week_day, parameter_up_down, vacation setup and week-end exit; original object boundary unchanged. Original object 0x800737A0-0x80075320 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/day_event.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/day_event.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/holiday.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/school_day.h"
#include "main/sd.h"
#include "main/settings.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/school_day.h"
#include "main/schedule.h"
#include "main/day_event.h"
#include "main/game_mode.h"
#include "main/holiday.h"
#include "game/day_event.h"

void week_day_main(void);
void week_day_exit(void);

void school_day_week_end_exit(void) {
    day_event_next(0);
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(0);
    hud_mascot_update();
    hud_screen_frame_draw(1);
    dec_bg_draw(get_last_gamen_mode());
}

/* Handle mode 0x40 stages for school-day setup, command, exit and vacation, then draw the day HUD */
void week_day(void) {
    switch (game_state.mode_stage) {
    case 0:
        week_day_init();
        break;
    case 1:
        week_day_main();
        break;
    case 2:
        week_day_exit();
        break;
    case 10:
        vacation_day_init();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    hud_icon_bar_update(1);
    dec_bg_draw(0);
    hud_screen_frame_draw(1);
    if (D_8011F3FF[0] & 0x80) {
        hud_mascot_update();
    }
}

/* Stage 2: call week_day_exit0 at step 0 to advance the day-event queue */
void week_day_exit(void) {
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step == 0) {
        week_day_exit0();
    }
}

/* Run screen setup, SD loading and SD actor setup across steps 0-2 */
u8 week_day_init(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        return school_day_screen_init();
    case 1:
        return school_day_sd_load();
    case 2:
        return school_day_sd_actors_init();
    }
}

/* Stage 1: call week_day_main0 at step 0 to wait for the animation and apply the command */
void week_day_main(void) {
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step == 0) {
        week_day_main0();
    }
}

/* Disc sector of the background of today's command: per-command table, or the club table for the club command */
s32 get_weekly_bg_sector(void) {
    u8 x;
    u16 h;

    x = game_state.day_event_index;
    if (game_state.day_events[x].girl != 4) {
        return D_800B6760[game_state.day_events[x].girl];
    }
    h = game_state.player_flags.h;
    return D_800B6780[h >> 0xC];
}

s32 school_day_background_read(void) {
    s32 ret;

    if ((game_state.player_flags.b[1] & 0xF) == 3 && !(*(u16 *)&game_state.player_flags_upper.b[0] & 1) && (game_state.last_command.w & 0xF) == 7) {
        ret = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
    } else {
        ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);
    }
    return ret;
}

u8 school_day_screen_init(void) {
    s32 temp_v0;

    day_event_today_clear(0);
    if (game_state.vacation != VACATION_NONE) {
        sprite_tables_clear();
        mode_stage_set(0xA);
        return 0;
    } else {
        switch (game_state.substep) {                       /* irregular */
        case 0:
            actor_clear_upper();
            set_dec_bri(0x80U);
            back_clear_switch(1);
            tpage_buf_clear();
            sprite_tables_clear();
            hizuke_init();
            hud_icon_bar_init();
            icon_disp_switch(1);
            schedule_command_icons_lock_to_record();
            parameter_show_init();
            hud_cursor_actor_init(0);
            parameter_disp_switch(1);
            message_window_init();
            settings_room_sick_icon_load();
            if (game_state.player_flags_upper.h & 1) {
                sprite_quads->flags = 0x80000000;
            } else {
                settings_room_sick_icon_show();
            }
            text_system_reset();
            message_window_title_show(0);
            k_disp_start(0);
            temp_v0 = school_day_background_read();
            if (temp_v0 != -1) {
                if ((temp_v0 == 0) || (temp_v0 == 1)) {
                    game_state.substep = 2;
                    dec_bg_show_set(0, temp_v0);
                }
            } else {
                game_state.substep = 1;
            }
            break;
        case 1:
            if (cd_read_sync_status() & 1) {
                dec_bg_show_set(0, dec_bg_decode_to_vram(0));
                if (bg_decode_result == -1) {
                    dec_bg_reset();
                    school_day_background_read();
                } else {
                    game_state.substep = 2;
                }
            }
            break;
        default:
        case 2:
            mode_step_next();
            break;
        }
    }
}

void school_day_sd_set_select(void) {
    /* FAKE: T-25049, this saved selection local preserves the original
     * call-spill home; removing it changes four instructions. */
    s32 sp28;
    s32 sd_variant;
    u32 club_id;
    u32 other_season_club_id;
    u8 companion_girl;

    D_801230D0 = 0xFF;
    companion_girl = game_state.day_events[game_state.day_event_index].extra;
    if (companion_girl != 0) {
        sd_variant = 1;
    } else {
        sd_variant = 0;
    }
    if ((game_state.day_events[game_state.day_event_index].girl == 3) && (companion_girl == 5)) {
        sd_variant = 2;
    }
    /* FAKE: T-25049, retain the entry value through this test so the
     * record address takes v0 and its byte value takes a0. No emitted test. */
    if (companion_girl) {}
    sp28 = sd_variant;
    if (hud_in_summer_months() == 1) {
        switch (game_state.day_events[game_state.day_event_index].girl) {
        case 0:
            D_801230D0 = (s32) D_800B68C0[0 + sd_variant];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68C0[2 + sd_variant];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68C0[4 + sd_variant];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68C0[6 + sd_variant];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68C0[9 + sd_variant];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68C0[11 + sd_variant];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68C0[13];
            break;
        }
    } else {
        switch (game_state.day_events[game_state.day_event_index].girl) {
        case 0:
            D_801230D0 = (s32) D_800B68D0[0 + sd_variant];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68D0[2 + sd_variant];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68D0[4 + sd_variant];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68D0[6 + sd_variant];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68D0[9 + sd_variant];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68D0[11 + sd_variant];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68D0[13];
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = sd_variant;
        if (hud_in_summer_months() == 1) {
            club_id = (u16) game_state.player_flags.h >> 0xC;
            switch (club_id) {
            case 0:
                D_801230D0 = (s32) (*D_800B6890)[sd_variant];
                break;
            case 1:
                D_801230D0 = (s32) D_800B6890[1][sd_variant];
                break;
            case 2:
                D_801230D0 = (s32) D_800B6890[2][sd_variant];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][sd_variant];
                break;
            case 4:
                D_801230D0 = (s32) D_800B6890[4][sd_variant];
                break;
            case 5:
                D_801230D0 = (s32) D_800B6890[5][sd_variant];
                break;
            case 6:
                D_801230D0 = (s32) D_800B6890[6][sd_variant];
                break;
            case 7:
                D_801230D0 = (s32) D_800B6890[7][sd_variant];
                break;
            case 8:
                D_801230D0 = (s32) D_800B6890[8][sd_variant];
                break;
            case 9:
                D_801230D0 = (s32) D_800B6890[9][sd_variant];
                break;
            case 10:
                D_801230D0 = (s32) D_800B6890[0xA][sd_variant];
                break;
            }
        } else {
            other_season_club_id = (u16) game_state.player_flags.h >> 0xC;
            switch (other_season_club_id) {
            case 0:
                D_801230D0 = D_800B68A8[0][sd_variant];
                break;
            case 1:
                D_801230D0 = (s32) D_800B68A8[1][sd_variant];
                break;
            case 2:
                D_801230D0 = (s32) D_800B68A8[2][sd_variant];
                break;
            case 3:
                D_801230D0 = (s32) D_800B68A8[3][sd_variant];
                break;
            case 4:
                D_801230D0 = (s32) D_800B68A8[4][sd_variant];
                break;
            case 5:
                D_801230D0 = (s32) D_800B68A8[5][sd_variant];
                break;
            case 6:
                D_801230D0 = (s32) D_800B68A8[6][sd_variant];
                break;
            case 7:
                D_801230D0 = (s32) D_800B68A8[7][sd_variant];
                break;
            case 8:
                D_801230D0 = (s32) D_800B68A8[8][sd_variant];
                break;
            case 9:
                D_801230D0 = (s32) D_800B68A8[9][sd_variant];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][sd_variant];
                break;
            }
        }
    }
}

u8 school_day_sd_load(void) {
    if (game_state.substep == 0) {
        school_day_sd_set_select();
        if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_upper.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            game_state.substep = 1;
        } else if (game_state.day_events[game_state.day_event_index].girl != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B67AC[D_801230D0]);
            game_state.substep = 1;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            game_state.substep = 1;
        }
    } else if (game_state.substep == 1) {
        if (cd_read_sync_status() & 1) {
            if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_upper.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
                if (cd_file_checksum_ok(9, 0x801E0000) != 0) {
                    mode_step_next();
                } else {
                    game_state.substep = 0;
                }
            } else {
                if (cd_file_checksum_ok(0x29, 0x801E0000) != 0) {
                    mode_step_next();
                } else {
                    game_state.substep = 0;
                }
            }
        }
    }
}

u8 school_day_sd_actors_init(void) {
    DayEvent *rec;
    s32 temp;
    u8 *tbl;

    actor_clear(0x60);
    actor_clear(0x61);
    rec = &game_state.day_events[game_state.day_event_index];
    temp = rec->scene;
    if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_upper.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (rec->girl == 4) {
        addr_init_club_sd(D_801230D0);
    } else {
        addr_init_weekly_sd(D_801230D0);
        if ((D_801230D0 == 0xF) || (D_801230D0 == 0x10)) {
            temp = 0;
            game_state.day_events[game_state.day_event_index].scene = 0;
        }
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((Actor *)actor_pool)[96].unk_01 = 1;
    ((Actor *)actor_pool)[96].flags = 5;
    ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[96].draw_flags = 0x84;
    ((Actor *)actor_pool)[96].frame_table = (u8 *)D_80125CA8;
    ((Actor *)actor_pool)[96].sequence_table = (u8 *)D_80125CAC;
    ((Actor *)actor_pool)[96].tpage_table = (u8 *)D_80125CB0;
    ((Actor *)actor_pool)[96].unk_05 = 2;
    /* FAKE: u8 view of the s8 flag unk_F6E (both reads below). The original reloads the flag for the second
     * record; with the field read as s8 the first load is kept in a register and every temporary shifts.
     * Matches; real source unknown. T-17016 */
    if (*(u8 *)&game_state.sd_anim_fast != 0) {
        ((Actor *)actor_pool)[96].blink_period = 0x82;
    }
    ((Actor *)actor_pool)[96].unk_43 = 0x10;
    ((Actor *)actor_pool)[96].sequence_slot = temp;
    ((Actor *)actor_pool)[96].x = -0x50;
    ((Actor *)actor_pool)[96].y = -0x40;
    /* A pointer to the day table keeps its base apart from the record base (the original addresses it with
     * its own lui); game_state.month_calendar[0].unk_00[i] reuses the record base register. T-17016 */
    tbl = game_state.month_calendar[0].day_kind;
    if (game_state.day_events[game_state.day_event_index].girl != 7) {
        if ((tbl[game_state.day] != 1) && (tbl[game_state.day] != 2) && (tbl[game_state.day] != 3) && (game_state.day_events[game_state.day_event_index].extra != 0xF)) {
            ((Actor *)actor_pool)[97].unk_01 = 1;
            ((Actor *)actor_pool)[97].flags = 5;
            ((Actor *)actor_pool)[97].quad_flags = 0x01000000;
            ((Actor *)actor_pool)[97].draw_flags = 0x84;
            ((Actor *)actor_pool)[97].frame_table = (u8 *)D_80125CA8;
            ((Actor *)actor_pool)[97].sequence_table = (u8 *)D_80125CAC;
            ((Actor *)actor_pool)[97].tpage_table = (u8 *)D_80125CB0;
            ((Actor *)actor_pool)[97].unk_05 = 2;
            if (*(u8 *)&game_state.sd_anim_fast != 0) {
                ((Actor *)actor_pool)[97].blink_period = 0x82;
            }
            ((Actor *)actor_pool)[97].unk_43 = 0x10;
            ((Actor *)actor_pool)[97].sequence_slot = (s16)(temp + 2);
            ((Actor *)actor_pool)[97].x = -0x18;
            ((Actor *)actor_pool)[97].y = -0x40;
        }
    }
    mode_stage_next();
}

/* Play the result sound and apply command/practice effects; after animation, advance when the old result timer is >=13; force exit when the step timer reaches 601 */
s32 week_day_main0(void) {
    s16 *state;
    s32 daily_command;

    if (!(D_80120652 & 1)) {
        if (D_80120658 == 0) {
            state = &D_80120666;
            switch ((u32)(*state & 1)) {                      /* irregular */
            case 0:
                audio_command(0x501);
                break;
            case 1:
                audio_command(0x502);
                break;
            }
            daily_command = game_state.day_events[game_state.day_event_index].girl;
            parameter_up_down(daily_command, 0, (s32) game_state.day_events[game_state.day_event_index].scene);
            parameter_show_init();
            if (daily_command == SCHOOL_DAY_COMMAND_CLUB) {
                game_state.clubs[(game_state.player_flags.h >> 12) % 11].practice_points += 1;
            }
        }
        if (D_80120658++ >= 0xD) {
            mode_stage_next();
        }
    }
    if ((u32) game_state.mode_step_timer.u >= 0x259U) {
        cd_command_reset();
        mode_stage_next();
    }
}

/* Advance to the next queue record, clear SD actors A and B, return to the day screen (mode 0x31) */
void week_day_exit0(void) {
    game_state.day_event_index++;
    actor_clear(0x60);
    actor_clear(0x61);
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

/* Apply the command's parameter deltas, scaling gains and stress by the day's result. */
s32 parameter_up_down(s32 command, s32 holiday_factor, s32 result_divisor) {
    s32 i;
    s32 sick_gain_divisor;
    s32 restricted_command_divisor;
    s8 *deltas;
    s8 *delta;
    u32 h;

    sick_gain_divisor = 1;
    restricted_command_divisor = 1;
    /* FAKE: T-25044, the extra raw-byte use keeps the byte in v1 while
     * the nibble is assigned directly to v0, avoiding an extra copy. */
    if (game_state.player_flags.b[1]) {}
    h = game_state.player_flags.b[1] & 0xF;
    if (h == 3) {
        sick_gain_divisor = 2;
    } else if (((h == 1) || (h == 2)) && (command != 5) && (command != 7) && (command != 6)) {
        restricted_command_divisor = 2;
    }
    if (holiday_factor != 0) {
        holiday_factor = 4;
    } else {
        holiday_factor = 1;
    }
    if (result_divisor != 0) {
        result_divisor = 2;
    } else {
        result_divisor = 1;
    }
    if (command == 4) {
        h = game_state.player_flags.h;
        deltas = club_activity_deltas[h >> 12];
        delta = deltas;
        for (i = 0; i != 8; i++) {
            if (*delta > 0) {
                parameter_change(i, *delta * (u32)(holiday_factor * 0x1000 / result_divisor / sick_gain_divisor / restricted_command_divisor));
            } else {
                parameter_change(i, *delta << 12);
            }
            delta++;
        }
        if (deltas[8] < 0) {
            parameter_change(8, deltas[8] * (u32)(holiday_factor * 0x1000 / result_divisor / sick_gain_divisor / restricted_command_divisor) + (result_divisor << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (deltas[8] << 12) + (result_divisor << 16) + 0xFFFF0000);
        }
    } else {
        deltas = school_day_command_deltas[command];
        delta = deltas;
        for (i = 0; i != 8; i++) {
            if (*delta > 0) {
                parameter_change(i, *delta * (u32)(holiday_factor * 0x1000 / result_divisor / sick_gain_divisor / restricted_command_divisor));
            } else {
                parameter_change(i, *delta << 12);
            }
            delta++;
        }
        if (deltas[8] < 0) {
            parameter_change(8, deltas[8] * (u32)(holiday_factor * 0x1000 / result_divisor / sick_gain_divisor / restricted_command_divisor) + (result_divisor << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (deltas[8] << 12) + (result_divisor << 16) + 0xFFFF0000);
        }
    }
}

/* Set up the vacation-day screen, background and SD pack over steps 0-2; also used by KANGEI camp stages */
u8 vacation_day_init(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        return school_day_vacation_screen_init();
    case 1:
        return school_day_vacation_sd_load();
    case 2:
        return school_day_vacation_sd_actors_init();
    }
}

s32 school_day_vacation_background_read(void) {
    s32 ret;

    if ((game_state.day_events[0].type == DAY_EVENT_CLUB_MASTER_A) || (game_state.day_events[0].type == DAY_EVENT_CLUB_CAMP)) {
        ret = dec_bg_cd_read(holiday_activity_bg_sector(), 0);
    } else if (((game_state.player_flags.b[1] & 0xF) == 3) && ((game_state.last_command.w & 0xF) == 7)) {
        ret = dec_bg_cd_read(D_800B5960[hud_season_index()], 0);
    } else {
        ret = dec_bg_cd_read(holiday_activity_bg_sector(), 0);
    }
    return ret;
}

u8 school_day_vacation_screen_init(void) {
    s32 temp_v0;
    GameState *g;

    g = &game_state;

    switch (g->substep) {                           /* irregular */
    case 0:
        actor_clear_upper();
        set_dec_bri(0x80U);
        back_clear_switch(1);
        tpage_buf_clear();
        hizuke_init();
        hud_icon_bar_init();
        icon_disp_switch(1);
        schedule_command_icons_lock_to_record();
        parameter_show_init();
        parameter_disp_switch(1);
        message_window_init();
        sprite_quads->flags = 0x80000000;
        text_system_reset();
        message_window_title_show(0);
        k_disp_start(0);
        temp_v0 = school_day_vacation_background_read();
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                g->substep = 2;
                dec_bg_show_set(0, temp_v0);
            }
        } else {
            g->substep = 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, dec_bg_decode_to_vram(0));
            g->substep = 2;
        }
        break;
    default:
    case 2:
        mode_step_next();
        break;
    }
}

u8 school_day_vacation_sd_load(void) {
    if (game_state.substep == 0) {
        holiday_activity_pick_sd_set();
        if ((game_state.player_flags.b[1] & 0xF) == 3 && !(*(u16 *)&game_state.player_flags_upper.b[0] & 1) && (game_state.last_command.w & 0xF) == 7) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            game_state.substep = 1;
        } else if (game_state.day_events[game_state.day_event_index].girl != 4) {
            cd_read_sectors(0x29, 0x801E0000, holiday_sd_set_sectors[D_801230D0]);
            game_state.substep = 1;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            game_state.substep = 1;
        }
    } else if ((game_state.substep == 1) && (cd_read_sync_status() & 1)) {
        mode_step_next();
    }
}

u8 school_day_vacation_sd_actors_init(void) {
    DayEvent *rec;
    s32 temp;

    actor_clear(0x60);
    rec = &game_state.day_events[game_state.day_event_index];
    temp = rec->scene;
    if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_upper.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (rec->girl == 4) {
        addr_init_club_sd(D_801230D0);
        if (schedule_command_girl_select(4) != 0xF) {
            ((Actor *)actor_pool)[97].unk_01 = 1;
            ((Actor *)actor_pool)[97].flags = 5;
            ((Actor *)actor_pool)[97].quad_flags = 0x01000000;
            ((Actor *)actor_pool)[97].draw_flags = 0x84;
            ((Actor *)actor_pool)[97].frame_table = (u8 *)D_80125CA8;
            ((Actor *)actor_pool)[97].sequence_table = (u8 *)D_80125CAC;
            ((Actor *)actor_pool)[97].tpage_table = (u8 *)D_80125CB0;
            ((Actor *)actor_pool)[97].unk_05 = 2;
            if ((u8)game_state.sd_anim_fast != 0) {
                ((Actor *)actor_pool)[97].blink_period = 0x82;
            }
            ((Actor *)actor_pool)[97].unk_43 = 0x10;
            ((Actor *)actor_pool)[97].sequence_slot = (s16)(temp + 2);
            ((Actor *)actor_pool)[97].x = -0x18;
            ((Actor *)actor_pool)[97].y = -0x40;
        }
    } else {
        addr_init_holiday_sd(D_801230D0);
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            temp = 0;
            game_state.day_events[game_state.day_event_index].scene = 0;
        }
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((Actor *)actor_pool)[96].unk_01 = 1;
    ((Actor *)actor_pool)[96].flags = 5;
    ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[96].draw_flags = 0x84;
    ((Actor *)actor_pool)[96].frame_table = (u8 *)D_80125CA8;
    ((Actor *)actor_pool)[96].sequence_table = (u8 *)D_80125CAC;
    ((Actor *)actor_pool)[96].tpage_table = (u8 *)D_80125CB0;
    ((Actor *)actor_pool)[96].unk_05 = 2;
    if ((u8)game_state.sd_anim_fast != 0) {
        ((Actor *)actor_pool)[96].blink_period = 0x82;
    }
    ((Actor *)actor_pool)[96].unk_43 = 0x10;
    ((Actor *)actor_pool)[96].sequence_slot = temp;
    ((Actor *)actor_pool)[96].x = -0x50;
    ((Actor *)actor_pool)[96].y = -0x40;
    if (game_state.day_events[0].type == DAY_EVENT_CLUB_CAMP) {
        mode_stage_next();
    } else {
        mode_stage_set(1);
    }
}
