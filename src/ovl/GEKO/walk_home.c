/* walk_home.c: src/ovl/GEKO/801323E0.c:9,178,284,364,454; approach callbacks, future-calendar/birthday topic selection, choice counter changes and queue completion. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801323E0-0x801338F0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80144C5C;

void func_801323E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80144C5C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80132454(void) {
    cd_read_sectors(0x24, 0x80197000, 0xAEC8);
    func_80132000();
    mode_step_next();
}

void func_80132490(void) {
    audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[game_state.current_girl], D_800B36C8[game_state.current_girl], D_800B3708[game_state.current_girl]);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void func_80132518(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_80132598(void) {
    audio_command(0x200);
    mode_step_next();
}

s32 func_801325C0(void) {
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
    D_80144C54 = 0;
    D_80144C3C = 0;
    /* T-9510: this initializer and func_801327AC store a full halfword. */
    *(s16 *)&D_80144C40 = 0;
    talk_names_set(game_state.current_girl);
    func_80132000();
    D_80144C30 = D_80144BC8;
    D_80144C34 = D_80144BF8;
    D_80144C38 = D_80144C28;
    func_80133024();
    if ((D_80144C50 == 0) && (game_state.current_girl == GIRL_MIRA) && !(rand() & 3)) {
        D_80144C50 = 0x64;
    }
    if (game_state.girls[game_state.current_girl].syoushin >= 0x4C) {
        D_80144C58 = 1;
    } else {
        D_80144C58 = 0;
    }
    mode_step_next();
    cd_command_push(9, 0, 0);
}

void func_801327AC(void) {
    D_80144C3C = 0;
    /* T-9510: a full halfword store, as in the initializer above (T-16510). */
    *(s16 *)&D_80144C40 = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_80144C30 = D_80144BA4;
        D_80144C34 = D_80144BD4;
        D_80144C38 = D_80144C04;
        break;
    case GIRL_MIO:
        D_80144C30 = D_80144BB4;
        D_80144C34 = D_80144BE4;
        D_80144C38 = D_80144C14;
        break;
    case GIRL_YUINA:
        D_80144C30 = D_80144BA8;
        D_80144C34 = D_80144BD8;
        D_80144C38 = D_80144C08;
        break;
    case GIRL_AYAKO:
        D_80144C30 = D_80144BAC;
        D_80144C34 = D_80144BDC;
        D_80144C38 = D_80144C0C;
        break;
    case GIRL_SAKI:
        D_80144C30 = D_80144BC4;
        D_80144C34 = D_80144BF4;
        D_80144C38 = D_80144C24;
        break;
    case GIRL_YUKARI:
        D_80144C30 = D_80144BB8;
        D_80144C34 = D_80144BE8;
        D_80144C38 = D_80144C18;
        break;
    case GIRL_NOZOMI:
        D_80144C30 = D_80144BBC;
        D_80144C34 = D_80144BEC;
        D_80144C38 = D_80144C1C;
        get_g_zyotai_s(GIRL_NOZOMI);
        break;
    case GIRL_MIRA:
        D_80144C30 = D_80144BB0;
        D_80144C34 = D_80144BE0;
        D_80144C38 = D_80144C10;
        break;
    case GIRL_YUKO:
        D_80144C30 = D_80144BA0;
        D_80144C34 = D_80144BD0;
        D_80144C38 = D_80144C00;
        break;
    case GIRL_MEGUMI:
        D_80144C30 = D_80144BC0;
        D_80144C34 = D_80144BF0;
        D_80144C38 = D_80144C20;
        break;
    case GIRL_YUMI:
        D_80144C30 = D_80144BCC;
        D_80144C34 = D_80144BFC;
        D_80144C38 = D_80144C2C;
        break;
    }
    mode_step_next();
}

/* FAKE: s32 fall-through preserves narrowing through v0 (T-9780). */
s32 func_80132A4C(s16 arg0) {
    /* FAKE: place the 22-byte table at sp+0x14 (T-9780). */
    s32 pad;
    GekoThresholds table;

    table = D_80144CE0;
    if (D_80144C50 == 0) {
        D_80144C50 = arg0;
    } else if (D_80144C50 == 10) {
        if ((game_state.frame_count & 7) >= (u32)table.threshold[game_state.current_girl]) {
            D_80144C50 = arg0;
        }
    } else if (game_state.frame_count & 1) {
        D_80144C50 = arg0;
    }
}

/* FAKE: T-11910 s32 with no return statement: the original leaves girl in $v0 at
 * every exit but the first. A `return girl` makes uopt keep the selector out of $v0
 * (girl in $v1, the record pointer and day in $v0, a `move v0,v1` at the exits). */
s32 func_80132B24(void) {
    u32 date;
    u8 girl;
    u8 month;
    GekoThresholds thr;
    GekoMonthDays days;
    u32 day; /* its home after the two tables gives the 0x50 frame (T-24709) */

    thr = D_80144CF8;
    days = D_80144D10;
    if (game_state.year == 0x60) {
        days.day[1] = 29;
    }
    girl = game_state.current_girl;
    if (!game_state.girls[girl].flags.f.b6) {
        return 0;
    }
    if ((game_state.frame_count & 7) < thr.threshold[girl] || D_80144C50 == 0) {
        date = game_state.girls[girl].birthday.w;
        month = game_state.month;
        day = game_state.day;
        if (month == (date & 0xF)) {
            if (((date << 23) >> 27) - day < 21 && ((date << 23) >> 27) - day != 0) {
                D_80144C50 = 10;
            }
        } else if (((date & 0xF) - month + 12) % 12 == 1 &&
                   /* FAKE: T-11910 the no-op `date = date` side effect makes cfe emit the
                    * day bits before the table load (the original's ugen temporary order) */
                   days.day[(month + 10) % 12] + ((date = date) << 23 >> 27) - day < 20) {
            D_80144C50 = 10;
        }
    }
    /* FAKE: T-11910 keeps girl live to the exit so it holds $v0 there and day takes $v1 */
    if (girl) {}
}

/* Old-style definition: func_80133024 calls it with four arguments (it reads two), so
 * no prototype may be in scope for that call; the bytes are the same as with
 * `(s16 arg0, s16 arg1)` (T-25401). */
void func_80132D04(arg0, arg1)
s16 arg0;
s16 arg1;
{
    s32 pad; /* FAKE: unused local that places status at sp+0x33 and the frame at 0x38 (T-3330) */
    u8 status;

    status = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    /* FAKE: indirect byte reads keep D_80144C50 in $t7 across the two compares instead of promoting it to $v1 (T-24115) */
    if ((*(u8 *)&D_80144C50 != 1) && (*(u8 *)&D_80144C50 != 0x10)) {
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0xC) {
            if (game_state.current_girl == GIRL_SHIORI) {
                if (((u32) ((u16) game_state.player_flags.h >> 0xC) < 6U) || ((u32) (game_state.girls[0].flags.b[2] >> 4) < 6U)) {
                    func_80132A4C(2);
                }
            } else {
                func_80132A4C(2);
            }
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 4) {
            func_80132A4C(6);
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0xB) {
            if ((status < 2U) || (game_state.current_girl == GIRL_YUMI)) {
                func_80132A4C(7);
            } else {
                func_80132A4C(8);
            }
        }
        if ((game_state.year == 0x61) && (game_state.month == 9) && ((u8) game_state.day < 0xEU) && (game_state.current_girl == GIRL_YUMI)) {
            func_80132A4C(8);
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 6) {
            func_80132A4C(0xB);
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0xE) {
            func_80132A4C(0xC);
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0x14) {
            func_80132A4C(0xD);
        }
        /* FAKE: indirect byte reads stop uopt from CSE-sharing unk_03E with the earlier 0x61 compare across calls (T-24115) */
        if ((*(u8 *)&game_state.year == 0x60) && (game_state.month_calendar[arg0].marker[arg1 + 1] == 2)) {
            func_80132A4C(0xE);
        }
        if ((*(u8 *)&game_state.year == 0x61) && (game_state.month_calendar[arg0].marker[arg1 + 1] == 2)) {
            func_80132A4C(0xF);
        }
        if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0xF) {
            func_80132A4C(9);
        }
    }
    if ((game_state.month_calendar[arg0].marker[arg1 + 1] == 5) && (D_80144C50 != 0x10)) {
        D_80144C50 = 0;
        func_80132A4C(1);
    }
    if (game_state.month_calendar[arg0].marker[arg1 + 1] == 0x15) {
        D_80144C50 = 0;
        func_80132A4C(0x10);
    }
}

/* The calls pass the day and month as two extra arguments that func_80132D04 does
 * not read: the values sit in $a2/$a3, where the original keeps them (T-25401).
 * FAKE: T-25401, the day and month are read into the locals in each arm (a store,
 * so the loads stay plain byte loads for an unprototyped call), while the test
 * reads the globals directly so its sum stays apart from the arms' sums. */
s32 func_80133024(void) {
    u8 month;
    u8 day;
    s16 i;
    s32 n;
    GekoMonthDays days;

    days = D_80144D28;
    if (game_state.year == 0x60) {
        days.day[1] = 29;
    }
    D_80144C50 = 0;
    for (i = 1; i < 20; i++) {
        n = days.day[game_state.month - 1];
        if (n >= game_state.day + i) {
            day = game_state.day;
            month = game_state.month;
            func_80132D04(0, (s16) (day + i - 1), day, month);
        } else {
            day = game_state.day;
            month = game_state.month;
            func_80132D04(1, (s16) (day + i - n - 1), day, month);
        }
    }
    func_80132B24();
}

void func_8013313C(void) {
    bg_read_sub2(0x4032);
    mode_step_next();
}

void func_80133164(void) {
    if (D_80144C58 != 0) {
        func_801322E8();
        return;
    }
    mode_step_next();
}

void func_801331A0(void) {
    u8 temp_v0;

    temp_v0 = get_g_zyotai_s(game_state.current_girl);
    if (((temp_v0 & 0x7F) == 4) || (D_80144C58 != 0)) {
        D_80144C3C += 0x12;
        portrait_layout_set(4U);
        D_80144C54 = 0;
    } else {
        if ((temp_v0 & 0x7F) == 2) {
            D_80144C3C += 6;
        }
        if ((temp_v0 & 0x7F) == 3) {
            D_80144C3C += 0xC;
        }
    }
    mode_step_next();
}

void func_80133250(void) {
    u8 temp_v0;

    temp_v0 = get_g_zyotai_s(game_state.current_girl);
    if (((temp_v0 & 0x7F) == 4) || (D_80144C58 != 0)) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if ((game_state.current_girl == GIRL_NOZOMI) && ((temp_v0 & 0x7F) < 2U)) {
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
void func_80133308(void) {
    s16 var_s1;
    s32 temp_s2;
    s32 temp_t3;
    s32 temp_v0;
    s32 var_s6;

    temp_v0 = get_g_zyotai_s(game_state.current_girl);
    if (D_80122CDC != 0) {
        D_80144C54 = 0;
        temp_s2 = ((s16) game_state.girls[game_state.current_girl].tokimeki / 20) & 0xFF;
        temp_t3 = ((u32) game_state.frame_count % (u32) (temp_s2 + 1)) & 0xFF;
        if ((u32) (temp_v0 & 0x7F & 0xFF) < 2U) {
            game_state.girls[game_state.current_girl].syoushin = game_state.girls[game_state.current_girl].syoushin + temp_t3 + 0xA;
        } else {
            game_state.girls[game_state.current_girl].syoushin = game_state.girls[game_state.current_girl].syoushin + temp_t3 + 6;
        }
        D_80144C3C += 2;
    } else {
        D_80144C54 = 1;
        if ((game_state.current_girl == GIRL_SHIORI) || (game_state.current_girl == GIRL_MIRA) || (game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKO) || (var_s6 = 4, (game_state.current_girl == GIRL_MEGUMI))) {
            var_s6 = 0;
            if ((u32) (temp_v0 & 0x7F & 0xFF) >= 3U) {
                D_80144C54 = 0;
            } else {
                var_s6 = 4;
            }
        }
        var_s1 = 0; do {
            if (var_s1) {}
            if ((((u32) (game_state.girls[var_s1].flags.w << 0x1E) >> 0x1F) != 0) && (var_s1 != (*(u8 *)&game_state.current_girl))) {
                temp_s2 = ((s16) game_state.girls[var_s1].tokimeki / 20) & 0xFF;
                game_state.girls[var_s1].syoushin = ((rand() % (u32) (temp_s2 + 1)) & 0xFF) + (game_state.girls[var_s1].syoushin + var_s6);
            }
            var_s1 += 1;
        } while (var_s1 < 0xB);
    }
    check_para_limit();
    mode_step_next();
}

void func_80133598(void) {
    s32 result;
    u8 status;

    result = get_g_zyotai_s(game_state.current_girl);
    status = result & 0x7F;
    if ((status == 4) || (D_80144C58 != 0) || ((game_state.current_girl == GIRL_NOZOMI) && (status < 2U))) {
        audio_command(0x24);
        audio_command(0x507);
    }
    if (((u8) D_80144C54 != 0) && ((D_80144C50 == 0) || (game_state.girls[game_state.current_girl].flags.w & 1))) {
        mode_step_next();
        return;
    }
    if (((u8) D_80144C54 == 0) && (game_state.current_girl == GIRL_NOZOMI) && (status < 2U)) {
        mode_step_next();
        return;
    }
    if ((u8) D_80144C54 == 0) {
        if ((status < 2U) && (D_80144C58 == 0)) {
            D_80144C30 = D_80144BC8;
            D_80144C34 = D_80144BF8;
            D_80144C38 = D_80144C28;
            D_80144C3C = game_state.girls[game_state.current_girl].syoushin / 20 + 1;
            if (D_80144C3C >= 6) {
                D_80144C3C = 5;
            }
        }
        mode_step_next();
        return;
    }
    mode_stage_next();
}

void func_8013375C(void) {
    u8 temp_v0;

    temp_v0 = get_g_zyotai_s(game_state.current_girl);
    if (((u32)(temp_v0 & 0x7F) < 2U) && (D_80122CDC != 0)) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_MIRA) || (game_state.current_girl == GIRL_MEGUMI)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    mode_step_next();
}

s32 func_801337EC(void) {
    if (game_state.substep == 0) {
        func_800674B0();
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
