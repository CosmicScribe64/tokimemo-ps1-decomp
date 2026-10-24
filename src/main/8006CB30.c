#define MAIN_API_OVERRIDE_func_80053418 /* this object calls it as void (no v0 result in the call block, func_8006EA40); main_api.h has s32 */
#define MAIN_API_OVERRIDE_func_80066104 /* the original callers pass a string the callee ignores; main_api.h has (void) */
#include "common.h"
#include "game.h"

void func_80066104();
void func_80053418();

void schedule_init(void) {
    if (D_800E6280.stage == 0) {
        _schedule_init();
    }
    func_800578F4(get_last_gamen_mode());
}

void _schedule_init(void) {
    switch (D_800E6280.step) {
    case 0:
        func_8006CD14();
        break;
    case 1:
        func_8006D7EC();
        break;
    case 2:
        schedule_girls_gakko();
        break;
    case 3:
        schdeule_girls_param();
        break;
    case 4:
        func_80070ECC();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
}

void last_date_spot_timer_dec(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.girl_last_date_spot_timer[i] != 0) {
            D_800E6280.girl_last_date_spot_timer[i] -= 1;
        }
    }
}

void restore_bgm(void) {
    if (func_8004480C() == 0) {
        func_8004500C(0, 0);
    }
}

void func_8006CD14(void) {
    switch (D_800E6280.substep) {
    case 0:
        if (D_800E6280.day_events[D_800E6280.day_event_index].type != 0x40) {
            restore_bgm();
        }
        func_80065F34(0);
        last_date_spot_timer_dec();
        D_800E6280.substep += 1;
        break;
    case 1:
        if (func_8006CDD4() != 0) {
            D_800E6280.substep += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

s32 func_8006CDD4(void) {
    if (D_800E6280.card_job == 1) {
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            D_800E6280.card_job = 0;
            func_80053D10();
        } else if (D_800E6280.card_timer >= 0x201U || D_800E6280.card_result == 0xF0) {
            func_80044750(0x502);
            D_800E6280.card_job = 0;
            func_80053D10();
        }
    } else if (D_800E6280.card_job == 0) {
        D_800E6280.card_busy = 0;
        return 1;
    }
    return 0;
}

void func_8006CE84(void) {
    s32 i;

    for (i = 0; i != 0xE; i++) {
        if (i != D_800E6280.day_events[D_800E6280.day_event_index].girl) {
            icon_can_use_set(i, 0);
        } else {
            icon_can_use_set(i, 1);
        }
    }
    if ((u8)D_800E6280.icon_mode == 2 && (D_800E6280.last_command.w & 0xF) != 0xEU) {
        icon_can_use_set(0xE, 0);
    }
}

void func_8006CF38(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        *(s32 *)&D_800E6280.day_events[i] = 0;
        D_800E6280.day_events[i].type = 0xFF;
    }
    D_800E6280.day_event_index = 0;
}

void func_8006CF80(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.girls[i].flags.b[0] &= 0xFFFE;
    }
}

void func_8006D00C(void) {
    if (D_800E6280.month == 9) {
        D_800E6280.player_flags_hi.b[0] &= 0xFFFB;
    }
}

void func_8006D038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.girl_last_date_spot[i] == 0x23 || D_800E6280.girl_last_date_spot[i] == 0x24) {
            D_800E6280.girl_last_date_spot[i] = 0;
        }
        D_800E6280.girl_event_bits[i] = 0;
    }
}

void func_8006D138(void) {
    GameState *gs = &D_800E6280;
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
            if (((GirlFlagBits *)&flags)->met) {
                gs->progress_state[1] = 1;
            }
            gs->progress_state[0xE] = 1;
        }
        if ((gs->year == 0x60) && (((GirlFlagBits *)gs->quarter_flags)->b5)) {
            gs->progress_state[5] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 9:
    case 10:
        if ((gs->year == 0x5F) && (((GirlFlagBits *)&flags)->b2)) {
            gs->progress_state[7] = 1;
        }
        if ((gs->year == 0x60) && (((GirlFlagBits *)gs->quarter_flags)->b6)) {
            gs->progress_state[0xA] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 1:
        if ((gs->year == 0x60) && (((GirlFlagBits *)&flags)->b3)) {
            gs->progress_state[3] = 1;
        }
        if (gs->year == 0x61) {
            /* FAKE: T-25040, this zero offset folds after uopt, retaining the base-relative byte load. */
            if (((GirlFlagBits *)(gs->quarter_flags + ((u32)gs->month < 0)))->b7) {
                gs->progress_state[6] = 1;
            }
        }
        /* FAKE: T-25040, the late zero keeps constant 98 out of the allocator's interference graph. */
        if (gs->year == 0x62 + ((u32)gs->month < 0)) {
        }
        break;
    case 12:
        if ((gs->year == 0x5F) && (((GirlFlagBits *)&flags)->b3)) {
            gs->progress_state[3] = 1;
        }
        if ((gs->year == 0x60) && (((GirlFlagBits *)gs->quarter_flags)->b7)) {
            gs->progress_state[6] = 1;
        }
        if (gs->year == 0x61) {
        }
        break;
    case 3:
    case 4:
        if ((gs->year == 0x60) && (((GirlFlagBits *)&flags)->b4)) {
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
        if ((gs->year == 0x60) && !(((GirlFlagBits *)gs->quarter_flags)->b4)) {
            gs->progress_state[0xD] = 0x80;
        }
        if ((gs->year == 0x61) && !(gs->quarter_flags[1] & 1)) {
            gs->progress_state[0xB] = 0x80;
            return;
        }
        return;
    case 8:
        if ((gs->year == 0x5F) && !(((GirlFlagBits *)gs->quarter_flags)->met)) {
            gs->progress_state[1] = 0x80;
        }
        if ((gs->year == 0x60) && !(((GirlFlagBits *)gs->quarter_flags)->b5)) {
            gs->progress_state[5] = 0x80;
        }
        if (gs->year == 0x61) {
            return;
        }
        break;
    case 11:
        gs->quarter_flags[1] &= 0xFF7F;
        if ((gs->year == 0x5F) && !(((GirlFlagBits *)gs->quarter_flags)->b2)) {
            gs->progress_state[7] = 0x80;
        }
        if ((gs->year == 0x60) && !(((GirlFlagBits *)gs->quarter_flags)->b6)) {
            gs->progress_state[0xA] = 0x80;
        }
        if (gs->year == 0x61) {
            return;
        }
        break;
    case 2:
        if ((gs->year == 0x60) && !(((GirlFlagBits *)gs->quarter_flags)->b3)) {
            gs->progress_state[3] = 0x80;
        }
        if ((gs->year == 0x61) && (!((GirlFlagBits *)gs->quarter_flags)->b7)) {
            gs->progress_state[6] = 0x80;
        }
        break;
    }
}


void func_8006D4A0(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.clubs[i].unk_03 &= 0xFFFE;
    }
}

void func_8006D52C(s32 *arg0, s32 arg1, s32 arg2) {
    s32 i;

    if (birth_day_check_days(0, D_800E6280.month + arg1, arg2) == 1) {
        D_800E6280.day_events[*arg0].type = 0x32;
        D_800E6280.day_events[*arg0].girl = 0;
        *arg0 += 1;
    }
    for (i = 1; i != 0xB; i++) {
        if (D_800E6280.girls[i].flags.f.met && D_800E6280.girls[i].flags.f.b6) {
            if (birth_day_check_days(i, D_800E6280.month + arg1, arg2) == 1) {
                D_800E6280.day_events[*arg0].type = 0x32;
                D_800E6280.day_events[*arg0].girl = i;
                *arg0 += 1;
            }
        }
    }
    if ((D_800E6280.player_flags.b[1] & 0xF) != 3) {
        if (birth_day_check_days(0xF, D_800E6280.month + arg1, arg2) == 1) {
            D_800E6280.day_events[*arg0].type = 0x33;
            *arg0 += 1;
        }
    }
}

/* s32 with no return (T-17004): $v0 is live at the exit of an int function, so as1 keeps
 * the second loop's start-index copy (move v1,v0) instead of folding it into the sll. */
s32 func_8006D6E0(void) {
    GameState *gs;
    s32 n;
    s32 i;

    gs = &D_800E6280;
    n = D_800E6280.weekday;
    for (i = 0; i < 32; i++) {
        if (D_800E6280.day_events[i].type == 0xFB) {
            if (n == 0) {
                gs->day_event_index = i;
                if (D_800E6280.day_events[i - 1].type != 0x25) {
                    D_800E6280.day_event_index--;
                    if (D_800E6280.day_events[i - 2].type != 0x25) {
                        D_800E6280.day_event_index--;
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
        if (D_800E6280.day_events[i].type == 0x25) {
            D_800E6280.day_events[i].type = 0;
        }
        if (D_800E6280.day_events[i].type == 0xFF) {
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

/* FAKE: T-23003, u8 view of func_8006BDA8 (defined s32 in 800674B0.c, not in main_api.h): the second result is then copied into $a0 with or in the beqz delay slot, as in the original, instead of narrowed with andi */
u8 func_8006BDA8();

void func_8006D7EC(void) {
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

    gs = &D_800E6280;
    nRand = 0;
    nSpecial = 0;
    argA = 0;
    argB = 0;
    nNo = 0;
    nYes = 0;
    gs->progress_state[0x15 + (D_800E6280.last_command.w & 0xF) % 9U] += 1;
    if (D_800E6280.girls[11].event_counters[0] != 0) {
        D_800E6280.girls[11].event_counters[0] -= 1;
    }
    sel = func_8006F524(gs->last_command.w & 0xF) & 0xF;
    for (j = 1, cnt = 0; j < 10; j++) {
        if (D_800E6280.girls[j].flags.f.met) {
            cnt++;
        }
    }
    D_800B6740 = -1;
    day = D_800E6280.day;
    monthOff = 0;
    month = D_800E6280.month;
    monthDays = gs->month_calendar[0].day_kind[0];
    if (((RecoveryCounterBits *)&gs->player_flags_hi)->f != 0) {
        ((RecoveryCounterBits *)&gs->player_flags_hi)->f -= 1;
    }
    func_8006CF38();
    wd = (D_800E6280.weekday + 1) % 7U;
    /* FAKE: T-25036, unsigned < 0 is zero, but folds after uopt so idx keeps a register dependency. */
    idx = (u32)D_800E6280.current_girl < 0;
    if (day == monthDays) {
        D_800E6280.day_events[0].type = 0xFE;
        /* FAKE: T-25036, this unreachable identity clears as1's zero substitution; it emits no code. */
        if (idx) { idx = idx + ((u32)day < 0); }
        idx++;
        day = 0;
        monthOff = 1;
    }
    day++;
    if (gs->month_calendar[0].marker[day] == 0xB) {
        gs->day_events[idx].type = 0x41;
        idx++;
        gs->day_events[idx].type = 0x10;
        idx++;
        func_8006D52C(&idx, monthOff, day + 6);
        func_8004284C();
        return;
    }
    if (gs->month_calendar[0].marker[day] == 8) {
        /* FAKE: T-23003, the unsigned constant is a ucode constant of its own and takes $a1 as in the original */
        if ((gs->player_flags.h >> 12) != 0xB && ((u32)gs->last_command.w & 0xF) == 4U) {
            if ((gs->player_flags.h >> 12) == 6 && (*(u16 *)&D_800E6280.clubs[6].level & 0xF) >= 8) {
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 1);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 2);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 3);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 4);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].girl = 7;
                gs->day_events[idx].type = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 5);
                gs->day_events[idx].type = 0xFB;
                idx++;
                gs->day_events[idx].type = 0x10;
                idx++;
                func_8006D52C(&idx, monthOff, day + 6);
                func_8004284C();
                return;
            }
            for (j = 0; j < 6; j++) {
                gs->day_events[idx].type = 0x42;
                gs->day_events[idx].girl = 4;
                gs->day_events[idx].extra = sel;
                gs->day_events[idx].scene = func_8006BDA8(wd);
                idx++;
            }
            gs->day_events[idx].type = 0x10;
            idx++;
            func_8006D52C(&idx, monthOff, day + 6);
            func_8004284C();
            return;
        }
        if ((gs->player_flags.h >> 12) == 0xB) {
            ((ClubFlagBits *)&gs->clubs[11])->unk_25 += 1;
        }
    }
    while (wd < 7) {
        if (monthDays < day) {
            gs->day_events[idx].type = 0xFE;
            idx++;
            day = 1;
            monthOff = 1;
        }
        if (gs->month_calendar[monthOff].marker[day] == 6) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0xE) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0x14) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0xA) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 0x10) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 2) {
            gs->day_events[idx++].type = 0xFD;
        }
        if (gs->month_calendar[monthOff].marker[day] == 4) {
            gs->day_events[idx++].type = 0x20;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xC) {
            gs->day_events[idx++].type = 0x24;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xD) {
            gs->day_events[idx++].type = 0x28;
        } else if (gs->month_calendar[monthOff].marker[day] == 0xF) {
            gs->day_events[idx++].type = 0x2C;
        } else if (gs->month_calendar[monthOff].marker[day] == 0x11) {
            gs->day_events[idx++].type = 0x31;
        } else if (gs->month_calendar[monthOff].marker[day] == 0x13) {
            gs->day_events[idx++].type = 0x30;
        } else if (gs->month_calendar[monthOff].marker[day] == 2 && D_800E6280.year == 0x60) {
            D_800E6280.girls[10].syoushin = 0;
            D_800E6280.girls[10].affection = 0x32;
            gs->day_events[idx].girl = 0xA;
            gs->day_events[idx].type = 0x48;
            idx++;
            func_8006D4A0();
        } else if (gs->month_calendar[monthOff].marker[day] == 2 && D_800E6280.year == 0x61) {
            gs->day_events[idx++].type = 0x43;
            func_8006D4A0();
        } else if (gs->month_calendar[monthOff].day_kind[day] == 1 || gs->month_calendar[monthOff].day_kind[day] == 2) {
            if (func_8006EA40() != 0) {
                D_800E6280.event_flags[77]++;
                gs->day_events[idx].type = 0x45;
                gs->day_events[idx].girl = 0xB;
                gs->day_events[idx].scene = 4;
                idx++;
            } else {
                gs->day_events[idx++].type = 0x10;
            }
        } else if (D_800E6280.year == 0x61 && (month + monthOff) == 0xD && day == 3) {
            if (func_8005352C() == 1 && (gs->player_flags.h >> 12) != 6 && (*(u16 *)&gs->clubs[gs->player_flags.h >> 12].level & 0xF) >= 0xA) {
                gs->day_events[idx++].type = 0x26;
            } else {
                if ((gs->player_flags.h >> 12) < 6 && gs->clubs[gs->player_flags.h >> 12].exp >= 0x17C) {
                    gs->day_events[idx++].type = 0x27;
                } else {
                    gs->day_events[idx].type = 0;
                    gs->day_events[idx].girl = D_800E6280.last_command.w & 0xF;
                    gs->day_events[idx].extra = sel;
                    if (func_8006BDA8(wd) != 0) {
                        nYes++;
                    } else {
                        nNo++;
                    }
                    idx++;
                }
            }
        } else if (gs->month_calendar[monthOff].marker[day] == 0x12 && *(u8 *)((u8 *)&D_800E6280 + 0x3E) == 0x62) {
            /* FAKE: T-23003, the original reloads the byte at +0x3E here; a plain D_800E6280.year is CSE'd with the 0x61 test into $a1 and every later temporary shifts (first-symbol view, aggregate not known) */
            gs->day_events[idx++].type = 0x4A;
        } else if (gs->month_calendar[monthOff].day_kind[day] == 0 || gs->month_calendar[monthOff].day_kind[day] == 3) {
            if (gs->month_calendar[monthOff].marker[day] == 5 || D_800E6280.day_events[0].type == 0x40) {
                gs->day_events[idx].type = 0x40;
                idx++;
            } else {
                gs->day_events[idx].type = 0;
                gs->day_events[idx].girl = D_800E6280.last_command.w & 0xF;
                gs->day_events[idx].extra = sel;
                res = func_8006BDA8(wd);
                if (res != 0) {
                    nYes++;
                } else {
                    nNo++;
                }
                if ((D_800E6280.last_command.w & 0xF) == 7) {
                    gs->day_events[idx].scene = 0;
                } else {
                    gs->day_events[idx].scene = res;
                }
                idx++;
                if (gs->day_events[idx - 1].scene == 1 && (nYes + nNo) == 6 && gs->month_calendar[monthOff].day_kind[day] == 0 && (rand() & 0xF) < 2) {
                    gs->day_events[idx].type = 0x47;
                    gs->day_events[idx].girl = 0xC;
                    idx++;
                }
                if (gs->month_calendar[monthOff].day_kind[day] == 3) {
                    if (D_800E6280.vacation == 2 && D_800E6280.event_flags[73] < (u16)D_800E6280.day_count / 365U && D_800E6280.girls[10].flags.f.met && !(get_g_zyotai_h(0xA) & 0x7F) && (rand() & 0xF) <= 0) {
                        gs->day_events[idx].type = 0x45;
                        gs->day_events[idx].girl = 0xA;
                        gs->day_events[idx].scene = 7;
                        idx++;
                    }
                    if (D_800E6280.vacation == 1 && wd == 2 && (monthOff + D_800E6280.month) == 8 && (rand() % 100) < 0x14 && cnt != 0
                        && (!((GirlFlagBitsSched *)&gs->player_flags)->b18 || (D_800E6280.year == 0x61 && !((GirlFlagBitsSched *)&gs->player_flags)->b18 && day == 0x1C))) {
                        ((GirlFlagBitsSched *)&D_800E6280.player_flags)->b18 = 1;
                        gs->day_events[idx].type = 0x49;
                        idx++;
                    }
                }
            }
            if (gs->month_calendar[monthOff].day_kind[day] == 0) {
                if (D_800E6280.day_events[0].type != 0x40) {
                    if (D_800E6280.year != 0x5F || D_800E6280.month != 4) {
                        if (D_800E6280.day_count < 0x2D6 && nSpecial == 0) {
                            if (cnt < 2) {
                                if (func_8006EB64(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->day_events[idx].type = 0x48;
                                    gs->day_events[idx].girl = argA;
                                    gs->day_events[idx].scene = argB;
                                    idx++;
                                }
                            } else {
                                if (func_8006EEC4(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->day_events[idx].type = 0x48;
                                    gs->day_events[idx].girl = argA;
                                    gs->day_events[idx].scene = argB;
                                    idx++;
                                }
                            }
                            if ((D_800E6280.last_command.w & 0xF) == 4 && func_8006F218(&argA, &argB) != 0) {
                                nSpecial++;
                                gs->day_events[idx].type = 0x48;
                                gs->day_events[idx].girl = argA;
                                gs->day_events[idx].scene = argB;
                                idx++;
                            }
                        }
                    }
                    if (nRand < 2) {
                        if ((rand() & 3) == 3) {
                            nRand++;
                            gs->day_events[idx].type = 0x44;
                            idx++;
                        }
                    }
                }
            }
        }
        if (wd == 5 && D_800E6280.day_count >= 0x21) {
            if (D_800E6280.day_events[0].type == 0x40) {
                if ((rand() & 1) && D_800E6280.girls[9].flags.f.met && (rand() % 100) < 0x32 && get_g_zyotai_h(9) < 3U
                    && (D_800E6280.param_art.value + D_800E6280.param_liberal_arts.value + D_800E6280.param_science.value) >= 0x12D) {
                    gs->last_command.b[0] = (gs->last_command.b[0] & 0xFFF0) | 3;
                    gs->day_events[idx].type = 0x47;
                    gs->day_events[idx].girl = 9;
                    idx++;
                }
            } else if (gs->month_calendar[monthOff].day_kind[day] == 0 && (D_800E6280.last_command.w & 0xF) != 7 && (rand() & 3) && nYes == 0 && nNo == 6) {
                if (sel != 0xF && sel != 0) {
                    if (!((D_800E6280.last_command.w & 0xF) == 5 && sel == 0)) {
                        gs->day_events[idx].type = 0x47;
                        gs->day_events[idx].girl = sel;
                        idx++;
                    }
                } else {
                    r = D_800E6280.last_command.w & 0xF;
                    /* FAKE: T-23003, unsigned 1: the original compares against $at, not the hoisted $s5 */
                    if ((r == 0 || r == 1U || r == 3 || r == 2) && (rand() % 100) < 7 && get_g_zyotai_h(9) < 3U && D_800E6280.girls[9].flags.f.met) {
                        gs->day_events[idx].type = 0x47;
                        gs->day_events[idx].girl = 9;
                        idx++;
                    } else if (sel == 0 && (D_800E6280.last_command.w & 0xF) != 5) {
                        gs->day_events[idx].type = 0x47;
                        gs->day_events[idx].girl = 0;
                        idx++;
                    }
                }
            }
        }
        if (birth_day_check_days(0, D_800E6280.month + monthOff, day) == 1) {
            gs->day_events[idx].type = 0x32;
            gs->day_events[idx].girl = 0;
            idx++;
        }
        for (j = 1; j < 0xB; j++) {
            if (D_800E6280.girls[j].flags.f.met && D_800E6280.girls[j].flags.f.b6) {
                if (birth_day_check_days(j, D_800E6280.month + monthOff, day) == 1) {
                    gs->day_events[idx].type = 0x32;
                    gs->day_events[idx].girl = j;
                    idx++;
                }
            }
        }
        if ((gs->player_flags.b[1] & 0xF) != 3) {
            if (birth_day_check_days(0xF, D_800E6280.month + monthOff, day) == 1) {
                gs->day_events[idx].type = 0x33;
                idx++;
            }
        }
        if (wd == 5) {
            gs->day_events[idx].type = 0xFC;
            idx++;
        }
        if (wd != 6) {
            gs->day_events[idx].type = 0xFB;
            idx++;
        }
        wd++;
        day++;
    }
    func_8004284C();
}

s32 func_8006EA40(void) {
    s32 count;
    s32 i;
    /* FAKE: the helper writes 16 bytes; four extra bytes reproduce the original frame and buffer slots. T-11670. */
    u8 rank[20];
    s16 points[16];

    func_80053418(rank, points);
    count = 0;
    for (i = 0; i != 11; i++) {
        if (D_800E6280.girls[i].flags.f.met && points[i] >= 0x1F5) {
            count++;
        }
    }
    if (D_800E6280.year == 0x62 && D_800E6280.month == 2 && D_800E6280.day == 0xF && count >= 2 &&
        D_800E6280.girls[11].flags.f.story_stage >= 4 && D_800E6280.event_flags[0x4C] != 0 && D_800E6280.event_flags[0x4E] != 0 &&
        !((u32)(D_800E6280.unk_54C[0].w << 2) >> 31) && (rand() & 0xF) < 5) {
        return 1;
    }
    return 0;
}

s32 func_8006EB64(u8 *arg0, u8 *arg1) {
    /* The flag word's f9 field is read through a pointer: an indirect load is not a plain variable for uopt, so the
     * compares keep their constant-first order and no phantom local holds $v1 (T-17004). */
    GameState *g = &D_800E6280;

    *arg0 = 0;
    *arg1 = 0;
    if ((rand() % 3) != 0) {
        return 0;
    }
    if (!(D_800E6280.girls[1].flags.f.met) && !((u32)D_800E6280.last_command.w & 0xF) && (D_800E6280.param_liberal_arts.value >= 0x37)) {
        *arg0 = 1;
        *arg1 = D_800E6280.frame_count & 1;
        return 1;
    }
    if (!(D_800E6280.girls[2].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 1) && (D_800E6280.param_science.value >= 0x3C)) {
        *arg0 = 2;
        *arg1 = D_800E6280.frame_count & 3;
        return 1;
    }
    if (!(D_800E6280.girls[3].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 2) && (D_800E6280.param_art.value >= 0x32)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[5].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 3) && (((u8) D_800E6280.girls[0].flags.b[2] >> 4) != 8) && (D_800E6280.param_athletics.value >= 0x51) && (D_800E6280.param_looks.value >= 0x51)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[6].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 3) && (((u8) D_800E6280.girls[0].flags.b[2] >> 4) != 9) && (D_800E6280.param_athletics.value >= 0x1F) && (D_800E6280.param_guts.value >= 0x1F)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[7].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 6) && (D_800E6280.param_looks.value >= 0x65)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[8].flags.f.met)) {
        if ((g->girls[8].flags.f.story_stage == 0) && (D_800E6280.param_trivia.value >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 1) && (D_800E6280.param_trivia.value >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 2) && (D_800E6280.param_trivia.value >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 3) && (((u32)D_800E6280.last_command.w & 0xF) == 5) && (D_800E6280.param_trivia.value >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }
    if (!(D_800E6280.girls[4].flags.f.met) && (((u16) D_800E6280.player_flags.h >> 0xC) != ((u8) D_800E6280.girls[4].flags.b[2] >> 4)) && (D_800E6280.param_guts.value >= 0x24)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
    return 0;
}

s32 func_8006EEC4(u8 *arg0, u8 *arg1) {
    /* Read through a pointer like func_8006EB64 (T-17004). */
    GameState *g = &D_800E6280;

    if (rand() & 7) {
        return 0;
    }
    *arg0 = 0;
    *arg1 = 0;
    if (!(D_800E6280.girls[1].flags.f.met) && !((u32)D_800E6280.last_command.w & 0xF) && (D_800E6280.param_liberal_arts.value >= 0x4B)) {
        *arg0 = 1;
        *arg1 = D_800E6280.frame_count & 1;
        return 1;
    }
    if (!(D_800E6280.girls[2].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 1) && (D_800E6280.param_science.value >= 0x55)) {
        *arg0 = 2;
        *arg1 = D_800E6280.frame_count & 3;
        return 1;
    }
    if (!(D_800E6280.girls[3].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 2) && (D_800E6280.param_art.value >= 0x41)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[5].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 3) && (((u8) D_800E6280.girls[0].flags.b[2] >> 4) != 8) && (D_800E6280.param_athletics.value >= 0x54) && (D_800E6280.param_looks.value >= 0x54)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[6].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 3) && (((u8) D_800E6280.girls[0].flags.b[2] >> 4) != 9) && (D_800E6280.param_athletics.value >= 0x47) && (D_800E6280.param_guts.value >= 0x47)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[7].flags.f.met) && (((u32)D_800E6280.last_command.w & 0xF) == 6) && (D_800E6280.param_looks.value >= 0x7E)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.girls[8].flags.f.met)) {
        if ((g->girls[8].flags.f.story_stage == 0) && (D_800E6280.param_trivia.value >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 1) && (D_800E6280.param_trivia.value >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 2) && (D_800E6280.param_trivia.value >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((g->girls[8].flags.f.story_stage == 3) && (((u32)D_800E6280.last_command.w & 0xF) == 5) && (D_800E6280.param_trivia.value >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }
    if (!(D_800E6280.girls[4].flags.f.met) && (((u16) D_800E6280.player_flags.h >> 0xC) != ((u8) D_800E6280.girls[4].flags.b[2] >> 4)) && (D_800E6280.param_guts.value >= 0x60)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
    return 0;
}

s32 func_8006F218(u8 *arg0, u8 *arg1) {
    GameState *g;
    s32 club;

    g = &D_800E6280;
    *arg0 = 0;
    *arg1 = 0;
    club = (u16) g->player_flags.h >> 0xC;
    if ((rand() % 3) != 0) {
        return 0;
    }
    if (!(g->girls[1].flags.f.met) && (club == ((u8) g->girls[1].flags.b[2] >> 4)) && ((u16) g->clubs[club].exp >= 0x15U)) {
        ((GirlFlagBitsSched *)&g->girls[1].flags)->b18 = 1;
        *arg0 = 1;
        if (club == 0) {
            *arg1 = (g->frame_count & 1) + 2;
            return 1;
        }
        *arg1 = 4;
        return 1;
    }
    if (!(g->girls[2].flags.f.met) && (club == ((u8) g->girls[2].flags.b[2] >> 4)) && ((u16) g->clubs[club].exp >= 0x15U)) {
        ((GirlFlagBitsSched *)&g->girls[2].flags)->b18 = 1;
        *arg0 = 2;
        if (club == 2) {
            *arg1 = 4;
            return 1;
        }
        *arg1 = 5;
        return 1;
    }
    if (!(g->girls[3].flags.f.met) && (club == ((u8) g->girls[3].flags.b[2] >> 4)) && ((u16) g->clubs[club].exp >= 0x15U)) {
        ((GirlFlagBitsSched *)&g->girls[3].flags)->b18 = 1;
        *arg0 = 3;
        if (club == 4) {
            *arg1 = 1;
            return 1;
        }
        *arg1 = 2;
        return 1;
    }
    if (!(g->girls[4].flags.f.met) && (club == ((u8) g->girls[4].flags.b[2] >> 4)) && ((u16) g->clubs[club].exp >= 0x15U)) {
        ((GirlFlagBitsSched *)&g->girls[4].flags)->b18 = 1;
        *arg0 = 4;
        if (club == 6) {
            *arg1 = 1;
            return 1;
        }
        *arg1 = 2;
        return 1;
    }
    if (!(g->girls[5].flags.f.met) && (club == 8) && (((u8) g->girls[0].flags.b[2] >> 4) != 8) && ((u16) g->clubs[club].exp >= 0x15U)) {
        ((GirlFlagBitsSched *)&g->girls[5].flags)->b18 = 1;
        *arg0 = 5;
        *arg1 = 1;
        return 1;
    }
    if (!(g->girls[6].flags.f.met) && (club == 9) && (((u8) g->girls[0].flags.b[2] >> 4) != 9)) {
        ((GirlFlagBitsSched *)&g->girls[6].flags)->b18 = 1;
        *arg0 = 6;
        *arg1 = 1;
        return 1;
    }
    return 0;
}

s32 func_8006F524(u32 arg0) {
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
    func_80053418(rank, points);
    g = &D_800E6280;
    switch (arg0 % 9U) {
    case 0:
        if (g->girls[1].flags.f.met) {
            firstGirl = 1;
            firstScore = rank[1] - D_800B6748;
        }
        selfScore = rank[0] - D_800B6748;
    default:
        break;
    case 1:
        if (g->girls[2].flags.f.met) {
            firstGirl = 2;
            firstScore = rank[2] - D_800B6758;
        }
        selfScore = rank[0] - D_800B6758;
        break;
    case 2:
        if (g->girls[3].flags.f.met) {
            firstGirl = 3;
            firstScore = rank[3] - D_800B674C;
        }
        selfScore = rank[0] - D_800B674C;
        break;
    case 3:
        if (g->girls[5].flags.f.met) {
            firstGirl = 5;
            firstScore = rank[5] - D_800B6748;
        }
        if (g->girls[6].flags.f.met) {
            secondGirl = 6;
            secondScore = rank[6] - D_800B6748;
        }
        selfScore = rank[0] - D_800B6754;
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
            firstScore = rank[8] - D_800B6750;
        }
        selfScore = rank[0] - D_800B6750;
        break;
    case 6:
        if (g->girls[7].flags.f.met) {
            firstGirl = 7;
            firstScore = rank[7] - D_800B6758;
        }
        selfScore = rank[0] - D_800B6758;
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
s32 special_event_select(char *arg0, char *arg1) {
    GameState *g;
    /* FAKE: 11 counts are used; the extra bytes and pad reproduce sp+0x7C
     * in the original 0xA8 frame. T-10730 */
    u8 counts[40];
    u8 framePad[32];
    s32 i;
    s32 event;
    s32 girl;

    g=&D_800E6280;
    *arg0 = 0;
    for (i=0;i<11;i++) {
        counts[i]=0;
        for (event=1;event<21;event++) {
            counts[i] += g->girls[i].event_counters[event];
        }
    }
    for (girl=0; girl<11; girl++) {
        if ((g->girls[girl].flags.f.met) && (get_g_zyotai_h(girl) < 2U) && (counts[girl] != 0) && ((rand() & 0xF) < 5)) {
            switch (girl) {
            case 0:
                if ((rand() % 100) < 3) {
                    if (((WordView *)&g->frame_count)->w & 1) {
                        if ((func_80066ACC() == 1) && (g->event_flags[0] == 0) && ((u32) (get_g_zyotai_s(girl) & 0x7F) < 2U)) {
                            g->event_flags[0] = (u8) (g->event_flags[0] + 1);
                            *arg0 = 0;
                            *arg1 = 1;
                            return 1;
                        }
                        break;
                    }
                    if (g->event_flags[1] == 0) {
                        g->event_flags[1] = (u8) (g->event_flags[1] + 1);
                        *arg0 = 0;
                        *arg1 = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 1:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->event_flags[8] == 0) {
                        g->event_flags[8] += 1;
                        *arg0 = 1;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 2:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->event_flags[14] == 0) {
                        g->event_flags[14] += 1;
                        *arg0 = 2;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 3:
                if (((rand() % 100) < 7) && (func_80066ACC() == 1)) {
                    if (g->event_flags[21] == 0) {
                        g->event_flags[21] += 1;
                        *arg0 = 3;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 4:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    g->event_flags[27] += 1;
                    *arg0 = 4;
                    *arg1 = 1;
                    return 1;
                }
                break;
            case 5:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1) && (g->event_flags[33] == 0)) {
                    g->event_flags[33] += 1;
                    *arg0 = 5;
                    *arg1 = 1;
                    return 1;
                }
                if (((rand() % 100) < 0x32) && (g->event_flags[37] == 1)) {
                    if (g->event_flags[34] == 0) {
                        g->event_flags[34] += 1;
                        *arg0 = 5;
                        *arg1 = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 6:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1)) {
                    if (g->event_flags[40] == 0) {
                        g->event_flags[40] += 1;
                        *arg0 = 6;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 7:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1)) {
                    if (g->event_flags[46] == 0) {
                        g->event_flags[46] += 1;
                        *arg0 = 7;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 8:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->event_flags[54] == 0) {
                        g->event_flags[54] += 1;
                        *arg0 = 8;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 9:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0) && (g->event_flags[60] == 0)) {
                    g->event_flags[60] = (u8) (g->event_flags[60] + 1);
                    *arg0 = 9;
                    *arg1 = 1;
                    return 1;
                }
                if ((g->event_flags[60] == 1) && (g->girls[9].flags.f.b14 == 1)) {
                    *(u8 *)((unsigned char *)(&g->event_flags[60]) + (1)) = (u8) (*(u8 *)((unsigned char *)(&g->event_flags[60]) + (1)) + 1);
                    g->girls[9].flags.b[1] = (u8) (g->girls[9].flags.b[1] & 0xFFBF);
                    *arg0 = 9;
                    *arg1 = 2;
                    return 1;
                }
                break;
            case 10:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1) && (g->event_flags[27] != 0) && (g->event_flags[67] == 0)) {
                    g->event_flags[67] += 1;
                    *arg0 = 0xA;
                    *arg1 = 1;
                    return 1;
                }
                if (((rand() % 100) < 0xA) && (g->year == 0x61) && (g->month == 9) && ((u8) g->day >= 0xEU)) {
                    if (g->event_flags[68] == 0) {
                        g->event_flags[68] += 1;
                        *arg0 = 0xA;
                        *arg1 = 2;
                        return 1;
                    }
                    break;
                }
                break;
            default: break;
            }
        }
    }
    if (((u8) g->girls[12].flags.b[3] >= 0x15U) && (g->event_flags[80] == 0) && (func_80066ACC() == 0)) {
        g->event_flags[80] += 1;
        *arg0 = 0xC;
        *arg1 = 2;
        return 1;
    }
    if ((u8) g->girls[12].flags.b[3] >= 0x29U) {
        if (g->event_flags[79] == 0) {
            g->event_flags[79] += 1;
            *arg0 = 0xC;
            *arg1 = 1;
            return 1;
        }
    }
    if (((u8)g->girls[12].flags.b[3] >= 0x41U) && (g->event_flags[81] == 0)) {
        g->event_flags[81] += 1;
        *arg0 = 0xC;
        *arg1 = 3;
        return 1;
    }
    if (((rand() % 100) < 4) && (g->event_flags[77] == 0) && (g->girls[11].event_counters[0] == 0) && ((u32) ((u32) (g->girls[11].flags.w << 0x14) >> 0x1D) < 5U) && ((u16) g->day_count >= 0x1DU)) {
        g->event_flags[74] += 1;
        g->girls[11].event_counters[0] = 4;
        *arg0 = 0xB;
        *arg1 = 1;
        return 1;
    }
    if ((g->event_flags[53] == 0) && (g->event_flags[52] == 1)) {
        g->event_flags[53] += 1;
        *arg0 = 7;
        *arg1 = 8;
        return 1;
    }
    return 0;
}


s32 geko_judge(s8 *arg0) {
    s32 i;
    s32 idx;
    s32 n;
    /* FAKE: buffer sizes chosen to reproduce the original frame and the 0x5C/0x7C slots; real sizes unknown. T-11670 */
    u8 sp7C[0x18];
    u8 sp5C[0x20];

    *arg0 = 0;
    n = (u32)D_800E7D10.seed % 11;
    func_80053418(sp7C, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((D_800E6280.girls[n].flags.f.met != 0) && (D_800E6280.girls[n].syoushin < 0x3C) && (get_g_zyotai_s(n) != 4) && (sp7C[n] >= 0x11) && ((rand() % 100) < 0x32)) {
            if (D_800B6740 != n) {
                *arg0 = n;
                D_800B6740 = n;
                return 1;
            }
        }
    }
    return 0;
}

s32 date_sasoi_judge(s8 *arg0) {
    s32 i;
    s32 idx;
    s32 n;
    u8 sp7C[0x18];
    u8 sp5C[0x20];

    *arg0 = 0;
    if (((u32)(D_800E6280.unk_54C[0].w << 2) >> 31) == 1) {
        return 0;
    }
    /* FAKE: reuse loop locals for the calendar values and comparison constant
     * to preserve register priority, branch operands and stack homes. T-12803. */
    n = 12;
    i = D_800E6280.month;
    idx = D_800E6280.day;
    if (((u32)(idx + 6) / 7 == 2) && ((u32)(D_800E6280.player_flags.h >> 12) < 0xB)) {
        return 0;
    }
    if ((n == i) && (idx == 0x11)) {
        return 0;
    }
    if ((n == i) && (idx == 0x18)) {
        return 0;
    }
    if ((D_800E6280.year == 0x62) && ((u32)i >= 2)) {
        return 0;
    }
    n = (u32)D_800E7D10.seed % 11;
    func_80053418(sp7C, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((D_800E6280.girls[n].flags.f.met != 0) && (D_800E6280.girls[n].syoushin < 0x3C) && (get_g_zyotai_s(n) != 4) && (sp7C[n] >= 0x21) && ((rand() % 100) < 8)) {
            if (D_800B6740 != n) {
                *arg0 = n;
                D_800B6740 = n;
                return 1;
            }
        }
    }
    return 0;
}

void schedule_girls_gakko(void) {
    u8 cnt;
    s32 i;
    char girl;
    char arg;

    cnt = 0;
    for (i = 0; i < 32; i++) {
        if (D_800E6280.day_events[i].type == 0x44) {
            if (D_800E6280.day_events[i + 1].type == 0x33 || D_800E6280.day_events[i + 1].type == 0x32 || D_800E6280.day_events[i + 1].type == 0x47) {
                D_800E6280.day_events[i].type = 0xFA;
            } else if ((D_800E6280.player_flags.b[1] & 0xF) == 3) {
                D_800E6280.day_events[i].type = 0xFA;
            } else if (special_event_select(&girl, &arg)) {
                D_800E6280.day_events[i].type = 0x45;
                D_800E6280.day_events[i].girl = girl;
                D_800E6280.day_events[i].scene = arg;
            } else if (date_sasoi_judge(&girl) == 1 && cnt == 0) {
                cnt++;
                D_800E6280.day_events[i].type = 0x46;
                D_800E6280.day_events[i].girl = girl;
            } else if (geko_judge(&girl)) {
                D_800E6280.day_events[i].girl = girl;
            } else {
                D_800E6280.day_events[i].type = 0xFA;
            }
        }
    }
    func_8004284C();
}

void yuukou_down(void) {
    s32 i;
    u32 q;
    s32 two;

    two = 2; /* FAKE: constant 2 kept in a local gives the original operand order of the bne (T-10510) */
    for (i = 0; i < 0xB; i++) {
        if (D_800E6280.girls[i].flags.f.met) {
            q = D_800E6280.day / 7;
            if (q == 0) {
                if (D_800E6280.girls[i].affection > 0) {
                    D_800E6280.girls[i].affection -= 1;
                }
            } else if (two == q && i == 0) {
                if (D_800E6280.girls[i].affection > 0) {
                    D_800E6280.girls[i].affection -= 1;
                }
            }
        }
    }
}

void syoushin_up(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        if (D_800E6280.girls[i].flags.f.met && (s32)get_h_yuukou(i) < 0xA) {
            D_800E6280.girls[i].syoushin += 5;
        }
    }
}

void schdeule_girls_param(void) {
    if (D_800E6280.vacation == 0) {
        yuukou_down();
        syoushin_up();
    }
    func_8004284C();
}

void func_80070ECC(void) {
    func_80072338();
    if (func_800460EC() & 4) {
        func_80045414(9, 0, 0);
    }
    main_state_set(0x31);
}

void func_80070F14(void) {
    if (D_800E6280.stage == 0) {
        func_80070F80();
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_800578F4(get_last_gamen_mode());
    func_80066C08(1);
}

void func_80070F80(void) {
    switch (D_800E6280.step) {
    case 0:
        func_8007132C();
        break;
    case 1:
        func_80070FD0();
        break;
    }
}
void func_80070FD0(void) {
    if (D_800E6280.substep == 0) {
        func_80067438();
        D_800E6280.substep++;
    } else if (func_80044E8C() == 1) {
        func_80044750(0x200);
        main_state_set(0x63);
    }
}

void func_80071038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.girls[i].tokimeki > 0x80) {
            D_800E6280.girls[i].tokimeki = 0x80;
        }
        if (D_800E6280.girls[i].tokimeki < 0) {
            D_800E6280.girls[i].tokimeki = 0;
        }
        if (D_800E6280.girls[i].affection > 0x80) {
            D_800E6280.girls[i].affection = 0x80;
        }
        if (D_800E6280.girls[i].affection < 0) {
            D_800E6280.girls[i].affection = 0;
        }
        if (D_800E6280.girls[i].syoushin > 0x80) {
            D_800E6280.girls[i].syoushin = 0x80;
        }
        if (D_800E6280.girls[i].syoushin < 0) {
            D_800E6280.girls[i].syoushin = 0;
        }
        if (D_800E6280.girls[i].syoushin < 0x32) {
            D_800E6280.girls[i].flags.b[2] &= 0xFFFE;
            D_800E6280.girls[i].flags.b[2] &= 0xFD;
        }
    }
}

s32 func_80071110(void) {
    /* FAKE: buffer sizes chosen to reproduce the 0x70 frame and the 0x3C/0x5C slots; real sizes unknown. T-7020 */
    u8 buf[20];
    s16 v[16];
    s32 idx;
    s32 best;
    s32 i;

    func_80053418(buf, v);
    idx = 0;
    best = 0;
    if (v[0] < 0x1F4) {
        if (D_800E6280.girls[12].flags.b[3] >= 0x46U && D_800E6280.event_flags[79] == 2) {
            return 0;
        }
    }
    if (v[2] < 0x1F4) {
        return 0;
    }
    if (!D_800E6280.girls[2].flags.f.met) {
        return 0;
    }
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (best < v[i]) { idx = i; best = v[i]; }
    if (idx == 2) {
        return 1;
    }
    return 0;
}

void func_80071280(void) {
    s32 i;

    if (D_800E6280.month == 5 && D_800E6280.day == 6) {
        for (i = 0; i < 11; i++) {
            D_800E6280.girls[i].flags.b[0] &= 0xFFEF;
        }
    }
}

/* T-25318: the opcode walk and the dispatch read D_800E6280.day_events[D_800E6280.day_event_index] directly, with no record
 * pointer or opcode local: uopt keeps the record address in $a1 and the opcode in $v1 across the tests, as the
 * target does, and the base 0x800E6280 stays out of the allocator. The 0xFF branch has its own hour local (four
 * locals give the target's spill slot at 0x30). */
void func_8007132C(void) {
    s32 op;
    u32 t;
    u32 hour;
    s32 i;

    func_80071280();
    func_80071038();
    /* FAKE: T-25052, the calendar byte views keep the base 0x800E6280 out of a register (plain fields: 901 rows,
     * T-25318). */
    if ((*(u8 *)&D_800E6280.year) >= 0x62U && (*(u8 *)&D_800E6280.month) >= 3U && (*(u8 *)&D_800E6280.day) != 0) {
        func_8004284C();
        return;
    }
    if ((*(u8 *)&D_800E6280.year) >= 0x62U && (*(u8 *)&D_800E6280.month) == 2 && (*(u8 *)&D_800E6280.day) == 0x1A) {
        op = 0;
        if (D_800E6280.event_flags[82] == 0) {
            for (i = 1; i < 10; i++) {
                if (D_800E6280.girls[i].flags.f.met) {
                    op++;
                }
            }
            if (op >= 6) {
                D_800E6280.day_event_index--;
                D_80122CD4 = 0xD;
                D_80122CD0 = 0;
                main_state_set(0x5D);
                return;
            }
        }
    }
    if ((*(u8 *)&D_800E6280.year) >= 0x62U && (*(u8 *)&D_800E6280.month) == 2 && (*(u8 *)&D_800E6280.day) == 0x1C
        && func_80071110() != 0 && D_800E6280.event_flags[20] == 0) {
        D_80122CD4 = 2;
        D_80122CD0 = 7;
        D_800E6280.event_flags[20] += 1;
        main_state_set(0x5D);
        return;
    }
    func_8006D138();
    func_8006D00C();
    if (((*(u8 *)&D_800E6280.month) != 0xC || (*(u8 *)&D_800E6280.day) != 0x18) && D_800E6280.events_today == 0 && D_800E6280.weekday != 6
        && D_800E6280.month_calendar[0].day_kind[(*(u8 *)&D_800E6280.day)] == 0 && D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFB
        && D_800E6280.day_events[0].type != 0x40 && D_800E6280.day_events[D_800E6280.day_event_index - 1].type == 0) {
        /* FAKE: T-25052, 11 + ((u32)i < 0) folds after uopt, so the bound 11 is not held in a register in the
         * three counter loops (plain loops: 92 rows, T-25318). */
        i = 0;
        do {
            if (D_800E6280.girls[i].flags.f.met && D_800E6280.girls[i].syoushin >= 0x4C && !(D_800E6280.girls[i].flags.h[1] & 1)) {
                D_800E6280.girls[i].flags.f.encounter_done = 1;
                if (D_800E6280.day_event_index != 0) {
                    D_800E6280.day_event_index--;
                }
                D_800E6280.events_today += 1;
                D_800E6280.day_events[D_800E6280.day_event_index].girl = i;
                main_state_set(0x59);
                return;
            }
            i++;
        } while (i != 11 + ((u32)i < 0));
    }
    if (D_800E6280.day_events[0].type == 0x25 && D_800E6280.day_events[D_800E6280.day_event_index - 1].type == 0x25) {
        if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0x32) {
            D_800E6280.day_event_index++;
        }
        if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0x33) {
            D_800E6280.day_event_index++;
        }
    }
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0x32) {
        if (D_800E6280.day_events[D_800E6280.day_event_index].girl == 0 && (D_800E6280.daily_event_bits & 1)) {
            D_800E6280.day_event_index++;
        }
        if (D_800E6280.day_events[D_800E6280.day_event_index].girl != 0 && (D_800E6280.daily_event_bits & 2)) {
            D_800E6280.day_event_index++;
        }
    }
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0x33 && (D_800E6280.daily_event_bits & 4)) {
        D_800E6280.day_event_index++;
    }
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFA) {
        D_800E6280.day_event_index++;
    }
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFB || D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFF) {
        func_8006B0C8();
        if (appointment_find_today((*(u8 *)&D_800E6280.year), (*(u8 *)&D_800E6280.month), (*(u8 *)&D_800E6280.day)) > 0) {
            if (get_g_zyotai_s(D_800E6280.unk_54C_0.unk_25) == 4) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.daily_check_done = 1;
                main_state_set(0x43);
                func_80042908(2);
                return;
            }
            if (D_800E6280.appt_place_1 != D_800E6280.appt_place_0) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.daily_check_done = 1;
                main_state_set(0x43);
                func_80042908(0);
                return;
            }
            if (D_800E6280.unk_54C_0.active) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.daily_check_done = 1;
                main_state_set(0x43);
                func_80042908(1);
                return;
            }
        }
    }
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFF) {
        hour = D_800E6280.player_flags.h >> 12;
        if (hour >= 6 && hour < 0xB) {
            if (((*(u8 *)&D_800E6280.day) + 6) / 7 == 3 && (D_800E6280.last_command.b[0] >> 4) != 4) {
                main_state_set(0x43);
                func_80042908(8);
                return;
            }
        }
        if (hour < 6 && (*(u8 *)&D_800E6280.month) == 0xA
            && (((*(u8 *)&D_800E6280.year) == 0x5F && (*(u8 *)&D_800E6280.day) == 8)
                || ((*(u8 *)&D_800E6280.year) == 0x60 && (*(u8 *)&D_800E6280.day) == 6)
                || ((*(u8 *)&D_800E6280.year) == 0x61 && (*(u8 *)&D_800E6280.day) == 5))) {
            main_state_set(0x43);
            func_80042908(9);
            return;
        }
        if ((*(u8 *)&D_800E6280.year) == 0x5F && (*(u8 *)&D_800E6280.month) == 0xA && (*(u8 *)&D_800E6280.day) == 0xF) {
            /* FAKE: T-25052, late-bound loop bound as in the first counter loop (92 rows without, T-25318). */
            i = 0;
            do {
                if (D_800E6280.girls[i].flags.f.met && D_800E6280.girls[i].flags.f.b12) {
                    if (get_g_zyotai_s(i) != 4 && func_8006B014(i) == 0) {
                        D_800E6280.daily_check_done = 1;
                        main_state_set(0x43);
                        func_80042908(6);
                        return;
                    }
                }
                i++;
            } while (i != 11 + ((u32)i < 0));
        }
        if ((*(u8 *)&D_800E6280.year) == 0x60 && (*(u8 *)&D_800E6280.month) == 0xA && (*(u8 *)&D_800E6280.day) == 0xD) {
            /* FAKE: T-25052, late-bound loop bound as in the first counter loop (92 rows without, T-25318). */
            i = 0;
            do {
                if (D_800E6280.girls[i].flags.f.met && D_800E6280.girls[i].flags.f.b13) {
                    if (get_g_zyotai_s(i) != 4 && func_8006B014(i) == 0) {
                        D_800E6280.daily_check_done = 1;
                        main_state_set(0x43);
                        func_80042908(7);
                        return;
                    }
                }
                i++;
            } while (i != 11 + ((u32)i < 0));
        }
    }
    t = 0;
    if (D_800E6280.day_events[D_800E6280.day_event_index].type == 0xFB) {
        for (i = 0; i < 11; i++) {
            if (D_800E6280.girls[i].flags.f.met && D_800E6280.girls[i].syoushin >= 0x3D && !D_800E6280.girls[i].flags.f.rumor_done) {
                t++;
                D_800E6280.girls[i].flags.f.rumor_done = 1;
            }
        }
        if (t != 0) {
            main_state_set(0x44);
            return;
        }
        if (D_800E6280.daily_check_done == 0) {
            if ((*(u8 *)&D_800E6280.year) == 0x61 && (*(u8 *)&D_800E6280.month) == 7 && (*(u8 *)&D_800E6280.day) == 0x1A) {
                D_800E6280.daily_check_done = 1;
                main_state_set(0x43);
                func_80042908(5);
                return;
            }
            if (D_800E6280.vacation == 0 && (*(u8 *)&D_800E6280.year) != 0x62) {
                for (i = 0; i < 11; i++) {
                    if (D_800E6280.girls[i].flags.f.met) {
                        if (D_800E6280.girls[i].syoushin >= 0x5B) {
                            func_80044750(0x50E);
                            D_800E6280.daily_check_done = 1;
                            main_state_set(0x43);
                            func_80042908(4);
                            return;
                        }
                    }
                }
            }
            if (D_800E6280.girls[11].flags.f.story_stage != 0 && D_800E6280.event_flags[77] == 0
                && (D_800E6280.girls[11].flags.b[3] < 7U || (D_800E6280.girls[11].flags.b[3] >= 7U && D_800E6280.day_count >= 0x2DBU && D_800E6280.girls[11].flags.b[3] < 9U))
                && !(rand() & 0x7F)) {
                D_800E6280.daily_check_done = 1;
                main_state_set(0x43);
                func_80042908(3);
                return;
            }
        }
        D_800E6280.daily_check_done = 0;
        D_800E6280.day_event_index++;
        func_80072338();
    }
    op = D_800E6280.day_events[D_800E6280.day_event_index].type;
    switch ((u32)op) {
    case 0x0:
        for (i = 0; i != 0xE; i++) {
            if (i != D_800E6280.day_events[D_800E6280.day_event_index].girl) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        main_state_set(0x40);
        return;
    case 0x10:
        main_state_set(0x46);
        return;
    case 0xFE:
        main_state_set(0x38);
        return;
    case 0xFD:
        func_8006CF80();
        main_state_set(0x39);
        return;
    case 0x20:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        main_state_set(0x72);
        return;
    case 0x24:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        if ((D_800E6280.player_flags.h >> 12) < 6U) {
            main_state_set(0x70);
            return;
        }
        main_state_set(0x71);
        return;
    case 0x28:
        func_8006CF80();
        main_state_set(0x42);
        return;
    case 0x2C:
        func_8006CF80();
        main_state_set(0x41);
        return;
    case 0x31:
        main_state_set(0x54);
        return;
    case 0x30:
        main_state_set(0x55);
        return;
    case 0x40:
        func_8006CF80();
        main_state_set(0x35);
        return;
    case 0x41:
        func_8006CF80();
        main_state_set(0x51);
        return;
    case 0x42:
        main_state_set(0x5F);
        return;
    case 0x43:
        func_8006CF80();
        main_state_set(0x36);
        return;
    case 0x44:
        D_800E6280.events_today += 1;
        if ((D_800E6280.player_flags.b[1] & 0xF) == 3) {
            func_80072B5C(0);
            return;
        }
        op = D_800E6280.day_events[D_800E6280.day_event_index - 1].type;
        if (op == 0x28) {
            func_80072B5C(0);
            return;
        }
        if (op == 0x47) {
            func_80072B5C(0);
            return;
        }
        if (!D_800E6280.girls[D_800E6280.day_events[D_800E6280.day_event_index].girl].flags.f.met) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x59);
        return;
    case 0xFC:
        main_state_set(0x32);
        return;
    case 0x32:
        main_state_set(0x57);
        return;
    case 0x33:
        main_state_set(0x56);
        return;
    case 0x25:
        main_state_set(0x74);
        return;
    case 0x26:
        main_state_set(0x75);
        return;
    case 0x27:
        main_state_set(0x76);
        return;
    case 0x46:
        D_800E6280.events_today += 1;
        if (D_800E6280.unk_54C_0.active) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x5A);
        return;
    case 0x45:
        D_80122CD4 = D_800E6280.day_events[D_800E6280.day_event_index].girl;
        /* FAKE: T-25318, the (s32) index cast keeps uopt from reusing the record address in $a1; the target
         * reloads the index here. */
        D_80122CD0 = D_800E6280.day_events[(s32)D_800E6280.day_event_index].scene;
        main_state_set(0x5D);
        return;
    case 0x47:
        D_800E6280.events_today += 1;
        main_state_set(0x61);
        return;
    case 0x48:
        D_800E6280.current_girl = D_800E6280.day_events[D_800E6280.day_event_index].girl;
        D_800E6280.girls[D_800E6280.current_girl].affection = 0x32;
        D_800E6280.girls[D_800E6280.current_girl].syoushin = 0;
        D_80122CD0 = D_800E6280.day_events[D_800E6280.day_event_index].scene;
        main_state_set(0x5B);
        return;
    case 0x49:
        main_state_set(0x58);
        return;
    case 0x4A:
        main_state_set(0x37);
        return;
    case 0xFF:
    default:
        main_state_set(0x34);
        return;
    }
}

void func_800722C4(void) {
    GameState *state;

    state = &D_800E6280;
    switch (D_800E6280.month) {
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
        ((GirlFlagBits *)state->quarter_flags)->b14 = ((GirlFlagBits *)D_800E6280.quarter_flags)->b13;
        ((GirlFlagBits *)state->quarter_flags)->b13 = 0;
        break;
    }
}

void func_80072338(void) {
    D_800E6280.weekday++;
    if (D_800E6280.weekday == 7) {
        D_800E6280.weekday = 0;
    }
    D_800E6280.day++;
    switch ((*(u8 *)&D_800E6280.month)) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (D_800E6280.day >= 0x20) {
            D_800E6280.day = 1;
            (*(u8 *)&D_800E6280.month)++;
            func_800722C4();
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (D_800E6280.day >= 0x1F) {
            D_800E6280.day = 1;
            (*(u8 *)&D_800E6280.month)++;
            func_800722C4();
        }
        break;
    case 2:
        if (D_800E6280.year == 0x60) {
            if (D_800E6280.day >= 0x1E) {
                D_800E6280.day = 1;
                (*(u8 *)&D_800E6280.month)++;
                func_800722C4();
            }
        } else if (D_800E6280.day >= 0x1D) {
            D_800E6280.day = 1;
            (*(u8 *)&D_800E6280.month)++;
            func_800722C4();
        }
        break;
    }
    if ((*(u8 *)&D_800E6280.month) >= 0xD) {
        D_800E6280.year++;
        (*(u8 *)&D_800E6280.month) = 1;
    }
    ((HalfView *)&D_800E6280.day_count)->h++;
    if ((*(u8 *)&D_800E6280.month) % 3 == 0) {
        func_8006D038();
    }
    if (D_800E6280.day_event_index != 0) {
        func_8007259C(0);
        D_800E6280.daily_check_done = 0;
    } else {
        func_8007259C(0);
        D_800E6280.daily_check_done = 1;
    }
    if ((D_800E6280.weekday == 6) && (*(u16 *)&D_800E6280.player_flags_hi & 1) && (D_800E6280.day_events[0].type != 0x40)) {
        *(s8 *)&D_800E6280.player_flags_hi = (u8)D_800E6280.player_flags_hi.h & 0xFFFE;
    }
    func_8006B0C8();
}

void func_8007259C(u8 arg0) {
    D_800E6280.daily_event_bits = arg0;
    D_800E6280.events_today = arg0;
}

void func_800725B0(void) {
    switch (D_800E6280.stage) {
    case 0:
        func_800726F0();
        break;
    case 1:
        D_800E6280.day_event_index += 1;
        main_state_set(0x31);
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
        func_80067870();
    }
    if (D_8011ECD3 & 0x80) {
        func_8006BA40();
    }
    func_800578F4(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(0);
}
void func_800726F0(void) {
    GameState *g = &D_800E6280;
    /* FAKE: the first test reads unk_03D through a local copy and the second re-reads the field (found by autotweak).
     * With both tests on the field, unk_03D is promoted into $v0 (sharing it with the month range) instead of the
     * original's $v1; the real source is unknown. T-17004 */
    s32 flag;

    switch (g->step) {
    case 0:
        func_8004E58C();
        func_80066104("ダミー");
        func_80065F34(0);
        gnsx(&g->player_family_name);
        flag = g->vacation;
        if (flag != 0 && g->day == 2) {
            g->vacation = 0;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->vacation != 0) {
            g->vacation = 0;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->month == 7) {
            sndisp("明日から夏休みだ）", 0, 0x1F);
            g->vacation = 1;
        } else if (g->month == 0xC) {
            sndisp("明日から冬休みだ）", 0, 0x1F);
            g->vacation = 2;
        } else if (g->month == 3) {
            sndisp("明日から春休みだ）", 0, 0x1F);
            g->vacation = 3;
        } else {
            g->vacation = 0;
        }
        func_800674B0();
        k_disp_start(3);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(0, 0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_800728A4(void) {
    switch (D_800E6280.stage) {
    case 0:
        func_80072944();
        break;
    case 1:
        D_800E6280.day_event_index += 1;
        main_state_set(0x31);
        break;
    }
    func_800578F4(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
}
s32 func_80072944(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80072998();
        break;
    case 1:
        func_80072B20();
        break;
    }
}

s32 func_80072998(void) {
    /* Fields are read through a pointer, as in func_8006EB64: the month byte keeps the original's a1/v0 allocation across
     * the call and the argument loads keep the original order (T-17004). */
    GameState *g = &D_800E6280;

    switch (g->substep) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        g->substep++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(g->year, g->month, 1);
            if (g->month % 3 == 0) {
                if (g->month == 9 && g->day == 1 && g->year == 0x60) {
                    func_80042808();
                } else if (g->month == 9) {
                    func_8004E58C();
                    func_80066104("ダミー");
                    func_80065F34(0);
                    gnsx(&g->player_family_name);
                    sndisp("明日から新学期だ）", 0, 0x1F);
                    k_disp_start(3);
                    func_80044750(0xC1);
                    func_800674B0();
                    func_8004284C();
                } else {
                    func_80044750(0xC1);
                    func_800674B0();
                    func_8004284C();
                }
                func_8006D00C();
                D_800B5BC8 = 0xEF;
            } else {
                func_80042808();
            }
        }
        break;
    }
}

void func_80072B20(void) {
    if (func_80044E8C() == 1) {
        func_8004500C(0, 0);
        func_80042808();
    }
}

s32 func_80072B5C(s32 arg0) {
    if (D_800B3C6C != 0) {
        main_state_set(0x22);
        return 0;
    }
    if (arg0 == 1) {
        func_8004500C(0, 0);
    }
    D_800E6280.day_event_index++;
    main_state_set(0x31);
    /* no return on this path: the original leaves $v0 as the call result */
}

void func_80072BC0(void) {
    switch (D_800E6280.stage) {
    case 0x0:
        func_80072C68();
        break;
    case 0x1:
        func_80072CA0();
        break;
    default:
    case 0xFF:
        func_800737A0();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
    func_800578F4(get_last_gamen_mode());
}
void func_80072C68(void) {
    parameter_show_init();
    hizuke_init();
    message_window_init();
    func_80042808();
}

void func_80072CA0(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
    case 0:
        func_80072D70();
        return;
    case 1:
        func_80073198();
        return;
    case 2:
        func_800732F8();
        return;
    case 3:
        func_800733D8();
        return;
    case 5:
        func_800734C4();
        return;
    case 6:
        func_800735B8();
        return;
    case 7:
        func_800736AC();
        return;
    default:
        func_80042908(0xFF);
        return;
    }
}

s32 func_80072D70(void) {
    s32 state;
    s32 lim;
    s32 i;
    s32 k;
    s32 flag;
    s32 start;
    /* FAKE: every GameState field is read through a local pointer. An indirect load is not a plain variable for uopt, so
     * the compares keep the original's constant-first order and register colours; found by autotweak (gstate), T-20830. T-21520 */
    GameState *g = &D_800E6280;

    flag = 0;
    lim = g->param_guts.value;
    lim = lim / 8 + 0xCC;
    if (lim >= 0xFF) {
        lim = 0xFE;
    }
    switch (g->player_flags.b[1] & 0xF) {
    case 0:
        state = 0;
        if ((g->player_flags_hi.b[0] >> 5) == 0) {
            if ((g->param_condition.value < 0x14) && (lim < (rand() & 0xFF))) {
                state = 1;
                flag = 1;
            } else if (g->param_condition.value == 0) {
                state = 1;
                flag = 1;
            }
            if ((g->param_stress.value >= 0x51) && ((u32) lim < (u32) (D_800E7D10.seed & 0xFF))) {
                state += 2;
                flag = 1;
            }
        }
        if (state == 1) {
            func_80042940(1);
        }
        if (state == 2) {
            func_80042940(2);
        }
        if (state == 3) {
            func_80042940(3);
        }
        break;
    case 1:
        state = 1;
        if ((g->param_stress.value >= 0x51) && ((u32) lim < (u32) (D_800E7D10.seed & 0xFF))) {
            state = 3;
            flag = 1;
        }
        if (g->param_condition.value >= 0x1F) {
            state = 0;
            flag = 1;
        }
        if (state == 3) {
            func_80042940(3);
        }
        if (state == 0) {
            func_80042940(5);
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
            func_80042940(3);
        }
        if (state == 0) {
            func_80042940(6);
        }
        break;
    case 3:
        state = 3;
        if ((g->param_stress.value < 0x46) && (g->param_condition.value >= 0x1F)) {
            state = 0;
            flag = 1;
        }
        if (state == 0) {
            func_80042940(7);
            break;
        }
        if (((u8 *) g->month_calendar)[g->day] == 0) {
            start = rand() % 11;
            for (i = 0; i != 11; i++) {
                k = (start + i) % 11;
                if (g->girls[k].flags.f.met && (get_h_tokimeki(k) >= 0x33U) && (get_g_zyotai_s(start) != 4)) {
                    if ((rand() & 0xFF) < 0x3F) {
                        g->date_girl = k;
                        main_state_set(0x62);
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
        func_80042908(0xFF);
    }
}

s32 func_80073198(void) {
    switch (*(u8 *)&D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("怪我");
        gnsx(&D_800E6280.player_family_name[0]);
        sndi("怪我をしてしまった）", 0, 0x1F);
        D_800E6280.player_flags.b[1] = (D_800E6280.player_flags.b[1] & 0xFFF0) | 1;
        k_disp_start(3);
        D_800E6280.substep += 1;
        return;
    case 1:
        if ((u32)D_800E6280.step_timer.w >= 0x3D) {
            if (((u32)D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_pressed != 0)) {
                D_800E6280.substep += 1;
                return;
            }
        }
        return;
    case 2:
        if (((u32)(D_800E6280.frame_count & 0xFF) < 0x19U) && (D_800E6280.event_flags[0x4B] == 0) && (D_800E6280.vacation == 0)) {
            main_state_set(0x5D);
            D_80122CD4 = 0xB;
            D_80122CD0 = 2;
            D_800E6280.event_flags[0x4B] = 1;
            return 1;
        }
        func_80042908(0xFF);
        break;
    }
}

s32 func_800732F8(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("ノイローゼ");
        gnsx(&D_800E6280.player_family_name[0]);
        sndi("ノイローゼになってしまった）", 0, 0x1F);
        D_800E6280.player_flags.b[1] = (D_800E6280.player_flags.b[1] & 0xFFF0) | 2;
        k_disp_start(3);
        D_800E6280.substep += 1;
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x3D) {
            if (((u32) D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_held != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800733D8(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("病気");
        gnsx(&D_800E6280.player_family_name[0]);
        func_8004500C(0, 0x202);
        sndi("病気になってしまった）", 0, 0x1F);
        D_800E6280.player_flags.b[1] = (D_800E6280.player_flags.b[1] & 0xFFF0) | 3;
        k_disp_start(3);
        D_800E6280.substep += 1;
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x3D) {
            if (((u32) D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_held != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800734C4(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.player_flags_hi.b[0] = (D_800E6280.player_flags_hi.b[0] & 0xFF1F) | 0x60;
        D_800E6280.player_flags.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.player_family_name[0]);
        sndi("怪我が治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.substep += 1;
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x3D) {
            if (((u32) D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_held != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800735B8(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.player_flags_hi.b[0] = (D_800E6280.player_flags_hi.b[0] & 0xFF1F) | 0x60;
        D_800E6280.player_flags.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.player_family_name[0]);
        sndi("ノイローゼが治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.substep += 1;
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x3D) {
            if (((u32) D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_held != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800736AC(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.player_flags_hi.b[0] = (D_800E6280.player_flags_hi.b[0] & 0xFF1F) | 0x60;
        D_800E6280.player_flags.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.player_family_name[0]);
        sndi("病気が治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.substep += 1;
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x3D) {
            if (((u32) D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_held != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}
