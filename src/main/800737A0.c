#include "common.h"
#include "game.h"

void week_day_main(void);
void week_day_exit(void);

void func_800737A0(void) {
    day_event_next(0);
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_bar_update(0);
    mascot_update();
    screen_frame_draw(1);
    dec_bg_draw(get_last_gamen_mode());
}

void week_day(void) {
    switch (game_state.stage) {
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
        game_reset_to_title();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_bar_update(1);
    dec_bg_draw(0);
    screen_frame_draw(1);
    if (D_8011F3FF[0] & 0x80) {
        mascot_update();
    }
}

void week_day_exit(void) {
    game_state.step_timer.w += 1;
    if (game_state.step == 0) {
        week_day_exit0();
    }
}

u8 week_day_init(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        return func_80073AD8();
    case 1:
        return func_800741B8();
    case 2:
        return func_8007437C();
    }
}

void week_day_main(void) {
    game_state.step_timer.w += 1;
    if (game_state.step == 0) {
        week_day_main0();
    }
}

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

s32 func_80073A40(void) {
    s32 ret;

    if ((game_state.player_flags.b[1] & 0xF) == 3 && !(*(u16 *)&game_state.player_flags_hi.b[0] & 1) && (game_state.last_command.w & 0xF) == 7) {
        ret = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);
    }
    return ret;
}

u8 func_80073AD8(void) {
    s32 temp_v0;

    daily_flags_clear(0);
    if (game_state.vacation != VACATION_NONE) {
        sprite_tables_clear();
        main_stage_set(0xA);
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
            func_8006509C();
            icon_disp_switch(1);
            func_8006CE84();
            parameter_show_init();
            cursor_actor_init(0);
            parameter_disp_switch(1);
            message_window_init();
            func_8005AD1C();
            if (game_state.player_flags_hi.h & 1) {
                sprite_quads->flags = 0x80000000;
            } else {
                func_8005AD70();
            }
            text_system_reset();
            message_window_title_show(0);
            k_disp_start(0);
            temp_v0 = func_80073A40();
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
                if (D_800B594C == -1) {
                    dec_bg_reset();
                    func_80073A40();
                } else {
                    game_state.substep = 2;
                }
            }
            break;
        default:
        case 2:
            main_step_next();
            break;
        }
    }
}

void func_80073CB0(void) {
    /* FAKE: T-25049, this saved selection local preserves the original
     * call-spill home; removing it changes four instructions. */
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;

    D_801230D0 = 0xFF;
    temp_a0 = game_state.day_events[game_state.day_event_index].extra;
    if (temp_a0 != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if ((game_state.day_events[game_state.day_event_index].girl == 3) && (temp_a0 == 5)) {
        var_v1 = 2;
    }
    /* FAKE: T-25049, retain the entry value through this test so the
     * record address takes v0 and its byte value takes a0. No emitted test. */
    if (temp_a0) {}
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        switch (game_state.day_events[game_state.day_event_index].girl) {
        case 0:
            D_801230D0 = (s32) D_800B68C0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68C0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68C0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68C0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68C0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68C0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68C0[13];
            break;
        }
    } else {
        switch (game_state.day_events[game_state.day_event_index].girl) {
        case 0:
            D_801230D0 = (s32) D_800B68D0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68D0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68D0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68D0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68D0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68D0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68D0[13];
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) game_state.player_flags.h >> 0xC;
            switch (temp_t6) {
            case 0:
                D_801230D0 = (s32) (*D_800B6890)[var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B6890[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B6890[2][var_v1];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B6890[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B6890[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B6890[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B6890[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B6890[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B6890[9][var_v1];
                break;
            case 10:
                D_801230D0 = (s32) D_800B6890[0xA][var_v1];
                break;
            }
        } else {
            temp_t9 = (u16) game_state.player_flags.h >> 0xC;
            switch (temp_t9) {
            case 0:
                D_801230D0 = D_800B68A8[0][var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B68A8[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B68A8[2][var_v1];
                break;
            case 3:
                D_801230D0 = (s32) D_800B68A8[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B68A8[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B68A8[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B68A8[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B68A8[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B68A8[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B68A8[9][var_v1];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][var_v1];
                break;
            }
        }
    }
}

u8 func_800741B8(void) {
    if (game_state.substep == 0) {
        func_80073CB0();
        if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_hi.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
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
            if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_hi.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
                if (file_checksum_ok(9, 0x801E0000) != 0) {
                    main_step_next();
                } else {
                    game_state.substep = 0;
                }
            } else {
                if (file_checksum_ok(0x29, 0x801E0000) != 0) {
                    main_step_next();
                } else {
                    game_state.substep = 0;
                }
            }
        }
    }
}

u8 func_8007437C(void) {
    DayEvent *rec;
    s32 temp;
    u8 *tbl;

    actor_clear(0x60);
    actor_clear(0x61);
    rec = &game_state.day_events[game_state.day_event_index];
    temp = rec->scene;
    if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_hi.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
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
    main_stage_next();
}

s32 week_day_main0(void) {
    s16 *state;
    s32 sp28;

    if (!(D_80120652 & 1)) {
        if (D_80120658 == 0) {
            state = &D_80120666;
            switch ((u32)(*state & 1)) {                      /* irregular */
            case 0:
                snd_command(0x501);
                break;
            case 1:
                snd_command(0x502);
                break;
            }
            sp28 = game_state.day_events[game_state.day_event_index].girl;
            parameter_up_down(sp28, 0, (s32) game_state.day_events[game_state.day_event_index].scene);
            parameter_show_init();
            if (sp28 == 4) {
                game_state.clubs[(game_state.player_flags.h >> 12) % 11].exp += 1;
            }
        }
        if (D_80120658++ >= 0xD) {
            main_stage_next();
        }
    }
    if ((u32) game_state.step_timer.u >= 0x259U) {
        cd_cmd_reset();
        main_stage_next();
    }
}

void week_day_exit0(void) {
    game_state.day_event_index++;
    actor_clear(0x60);
    actor_clear(0x61);
    main_state_set(MODE_DAY_SCREEN);
}

s32 parameter_up_down(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 div_a;
    s32 div_b;
    s8 *row;
    s8 *p;
    u32 h;

    div_a = 1;
    div_b = 1;
    /* FAKE: T-25044, the extra raw-byte use keeps the byte in v1 while
     * the nibble is assigned directly to v0, avoiding an extra copy. */
    if (game_state.player_flags.b[1]) {}
    h = game_state.player_flags.b[1] & 0xF;
    if (h == 3) {
        div_a = 2;
    } else if (((h == 1) || (h == 2)) && (arg0 != 5) && (arg0 != 7) && (arg0 != 6)) {
        div_b = 2;
    }
    if (arg1 != 0) {
        arg1 = 4;
    } else {
        arg1 = 1;
    }
    if (arg2 != 0) {
        arg2 = 2;
    } else {
        arg2 = 1;
    }
    if (arg0 == 4) {
        h = game_state.player_flags.h;
        row = club_activity_deltas[h >> 12];
        p = row;
        for (i = 0; i != 8; i++) {
            if (*p > 0) {
                parameter_change(i, *p * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, *p << 12);
            }
            p++;
        }
        if (row[8] < 0) {
            parameter_change(8, row[8] * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (row[8] << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    } else {
        row = command_deltas[arg0];
        p = row;
        for (i = 0; i != 8; i++) {
            if (*p > 0) {
                parameter_change(i, *p * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, *p << 12);
            }
            p++;
        }
        if (row[8] < 0) {
            parameter_change(8, row[8] * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (row[8] << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    }
}

u8 vacation_day_init(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        return func_80074DE0();
    case 1:
        return func_80074F24();
    case 2:
        return func_8007505C();
    }
}

s32 func_80074D28(void) {
    s32 ret;

    if ((game_state.day_events[0].type == DAY_EVENT_CLUB_MASTER_A) || (game_state.day_events[0].type == DAY_EVENT_CLUB_CAMP)) {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    } else if (((game_state.player_flags.b[1] & 0xF) == 3) && ((game_state.last_command.w & 0xF) == 7)) {
        ret = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return ret;
}

u8 func_80074DE0(void) {
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
        func_8006509C();
        icon_disp_switch(1);
        func_8006CE84();
        parameter_show_init();
        parameter_disp_switch(1);
        message_window_init();
        sprite_quads->flags = 0x80000000;
        text_system_reset();
        message_window_title_show(0);
        k_disp_start(0);
        temp_v0 = func_80074D28();
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
        main_step_next();
        break;
    }
}

u8 func_80074F24(void) {
    if (game_state.substep == 0) {
        func_80076FDC();
        if ((game_state.player_flags.b[1] & 0xF) == 3 && !(*(u16 *)&game_state.player_flags_hi.b[0] & 1) && (game_state.last_command.w & 0xF) == 7) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            game_state.substep = 1;
        } else if (game_state.day_events[game_state.day_event_index].girl != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B69E0[D_801230D0]);
            game_state.substep = 1;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            game_state.substep = 1;
        }
    } else if ((game_state.substep == 1) && (cd_read_sync_status() & 1)) {
        main_step_next();
    }
}

u8 func_8007505C(void) {
    DayEvent *rec;
    s32 temp;

    actor_clear(0x60);
    rec = &game_state.day_events[game_state.day_event_index];
    temp = rec->scene;
    if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_hi.h & 1) && ((game_state.last_command.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (rec->girl == 4) {
        addr_init_club_sd(D_801230D0);
        if (func_8006F524(4) != 0xF) {
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
        main_stage_next();
    } else {
        main_stage_set(1);
    }
}
