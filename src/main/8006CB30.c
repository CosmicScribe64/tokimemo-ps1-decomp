#define MAIN_API_OVERRIDE_func_80053418 /* this object calls it as void (no v0 result in the call block, func_8006EA40); main_api.h has s32 */
#define MAIN_API_OVERRIDE_func_80066104 /* the original callers pass a string the callee ignores; main_api.h has (void) */
#include "common.h"
#include "game.h"

void func_80066104();
void func_80053418();

void schedule_init(void) {
    if (D_800E6280.unk_1109 == 0) {
        _schedule_init();
    }
    func_800578F4(get_last_gamen_mode());
}

void _schedule_init(void) {
    switch (D_800E6280.unk_110A) {
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
        if (D_800E6280.unk_73C[i] != 0) {
            D_800E6280.unk_73C[i] -= 1;
        }
    }
}

void restore_bgm(void) {
    if (func_8004480C() == 0) {
        func_8004500C(0, 0);
    }
}

void func_8006CD14(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 != 0x40) {
            restore_bgm();
        }
        func_80065F34(0);
        last_date_spot_timer_dec();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8006CDD4() != 0) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

s32 func_8006CDD4(void) {
    if (D_800E6280.unk_111B == 1) {
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            D_800E6280.unk_111B = 0;
            func_80053D10();
        } else if (D_800E6280.unk_1120 >= 0x201U || D_800E6280.unk_1115 == 0xF0) {
            func_80044750(0x502);
            D_800E6280.unk_111B = 0;
            func_80053D10();
        }
    } else if (D_800E6280.unk_111B == 0) {
        D_800E6280.unk_035 = 0;
        return 1;
    }
    return 0;
}

void func_8006CE84(void) {
    s32 i;

    for (i = 0; i != 0xE; i++) {
        if (i != D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
            icon_can_use_set(i, 0);
        } else {
            icon_can_use_set(i, 1);
        }
    }
    if ((u8)D_800E6280.unk_F6C == 2 && (D_800E6280.unk_F68.w & 0xF) != 0xEU) {
        icon_can_use_set(0xE, 0);
    }
}

void func_8006CF38(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        *(s32 *)&D_800E6280.unk_69C[i] = 0;
        D_800E6280.unk_69C[i].unk_00 = 0xFF;
    }
    D_800E6280.unk_71C = 0;
}

void func_8006CF80(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.unk_1BC[i].unk_0C.b[0] &= 0xFFFE;
    }
}

void func_8006D00C(void) {
    if (D_800E6280.unk_03F == 9) {
        D_800E6280.unk_0F6.b[0] &= 0xFFFB;
    }
}

void func_8006D038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_72C[i] == 0x23 || D_800E6280.unk_72C[i] == 0x24) {
            D_800E6280.unk_72C[i] = 0;
        }
        D_800E6280.unk_74C[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);

void func_8006D4A0(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.unk_66C[i].unk_03 &= 0xFFFE;
    }
}

void func_8006D52C(s32 *arg0, s32 arg1, s32 arg2) {
    s32 i;

    if (birth_day_check_days(0, D_800E6280.unk_03F + arg1, arg2) == 1) {
        D_800E6280.unk_69C[*arg0].unk_00 = 0x32;
        D_800E6280.unk_69C[*arg0].unk_01 = 0;
        *arg0 += 1;
    }
    for (i = 1; i != 0xB; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0C.f.b6) {
            if (birth_day_check_days(i, D_800E6280.unk_03F + arg1, arg2) == 1) {
                D_800E6280.unk_69C[*arg0].unk_00 = 0x32;
                D_800E6280.unk_69C[*arg0].unk_01 = i;
                *arg0 += 1;
            }
        }
    }
    if ((D_800E6280.unk_0F4.b[1] & 0xF) != 3) {
        if (birth_day_check_days(0xF, D_800E6280.unk_03F + arg1, arg2) == 1) {
            D_800E6280.unk_69C[*arg0].unk_00 = 0x33;
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
    n = D_800E6280.unk_041;
    for (i = 0; i < 32; i++) {
        if (D_800E6280.unk_69C[i].unk_00 == 0xFB) {
            if (n == 0) {
                gs->unk_71C = i;
                if (D_800E6280.unk_69C[i - 1].unk_00 != 0x25) {
                    D_800E6280.unk_71C--;
                    if (D_800E6280.unk_69C[i - 2].unk_00 != 0x25) {
                        D_800E6280.unk_71C--;
                    }
                }
                break;
            }
            n--;
        }
    }
    if (gs->unk_71C != 0) {
        gs->unk_71C--;
    }
    for (i = gs->unk_71C; i < 32; i++) {
        if (D_800E6280.unk_69C[i].unk_00 == 0x25) {
            D_800E6280.unk_69C[i].unk_00 = 0;
        }
        if (D_800E6280.unk_69C[i].unk_00 == 0xFF) {
            break;
        }
    }
}

/* NON_MATCHING: T-23003, 13 rows; idx++ is folded to li, and the second call result is narrowed with andi where the original copies it with or. */
#ifdef NON_MATCHING
/* T-23003: the 3-bit counter in bits 5..7 of the byte at GameState +0xF6 (read with lbu, decremented as a bit-field). */
typedef struct F6Bits {
    u32 pad0 : 5;
    u32 f : 3;
    u32 pad1 : 24;
} F6Bits;

void func_8006D7EC(void) {
    GameState *gs;
    /* FAKE: T-23003, the original frame has seven register locals (two words above idx); res holds the second call result in $a0 (u8: narrows with andi where the original copies with or) */
    u8 res;
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
    gs->unk_120[0x15 + (D_800E6280.unk_F68.w & 0xF) % 9U] += 1;
    if (D_800E6280.unk_1BC[11].unk_14[0] != 0) {
        D_800E6280.unk_1BC[11].unk_14[0] -= 1;
    }
    sel = func_8006F524(gs->unk_F68.w & 0xF) & 0xF;
    for (j = 1, cnt = 0; j < 10; j++) {
        if (D_800E6280.unk_1BC[j].unk_0C.f.b1) {
            cnt++;
        }
    }
    D_800B6740 = -1;
    day = D_800E6280.unk_040;
    monthOff = 0;
    month = D_800E6280.unk_03F;
    monthDays = gs->unk_044[0].unk_00[0];
    if (((F6Bits *)&gs->unk_0F6)->f != 0) {
        ((F6Bits *)&gs->unk_0F6)->f -= 1;
    }
    func_8006CF38();
    wd = (D_800E6280.unk_041 + 1) % 7U;
    idx = 0;
    if (day == monthDays) {
        D_800E6280.unk_69C[0].unk_00 = 0xFE;
        idx++;
        day = 0;
        monthOff = 1;
    }
    day++;
    if (gs->unk_044[0].unk_20[day] == 0xB) {
        gs->unk_69C[idx].unk_00 = 0x41;
        idx++;
        gs->unk_69C[idx].unk_00 = 0x10;
        idx++;
        func_8006D52C(&idx, monthOff, day + 6);
        func_8004284C();
        return;
    }
    if (gs->unk_044[0].unk_20[day] == 8) {
        /* FAKE: T-23003, the unsigned constant is a ucode constant of its own and takes $a1 as in the original */
        if ((gs->unk_0F4.h >> 12) != 0xB && ((u32)gs->unk_F68.w & 0xF) == 4U) {
            if ((gs->unk_0F4.h >> 12) == 6 && (*(u16 *)&D_800E6280.unk_66C[6].unk_02 & 0xF) >= 8) {
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 1);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 2);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_01 = 7;
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 3);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_01 = 7;
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 4);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_01 = 7;
                gs->unk_69C[idx].unk_00 = 0x25;
                idx++;
                func_8006D52C(&idx, monthOff, day + 5);
                gs->unk_69C[idx].unk_00 = 0xFB;
                idx++;
                gs->unk_69C[idx].unk_00 = 0x10;
                idx++;
                func_8006D52C(&idx, monthOff, day + 6);
                func_8004284C();
                return;
            }
            for (j = 0; j < 6; j++) {
                gs->unk_69C[idx].unk_00 = 0x42;
                gs->unk_69C[idx].unk_01 = 4;
                gs->unk_69C[idx].unk_03 = sel;
                gs->unk_69C[idx].unk_02 = func_8006BDA8(wd);
                idx++;
            }
            gs->unk_69C[idx].unk_00 = 0x10;
            idx++;
            func_8006D52C(&idx, monthOff, day + 6);
            func_8004284C();
            return;
        }
        if ((gs->unk_0F4.h >> 12) == 0xB) {
            ((ScheduleClubFlags *)&gs->unk_66C[11])->unk_25 += 1;
        }
    }
    while (wd < 7) {
        if (monthDays < day) {
            gs->unk_69C[idx].unk_00 = 0xFE;
            idx++;
            day = 1;
            monthOff = 1;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 6) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 0xE) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 0x14) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 0xA) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 0x10) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 2) {
            gs->unk_69C[idx++].unk_00 = 0xFD;
        }
        if (gs->unk_044[monthOff].unk_20[day] == 4) {
            gs->unk_69C[idx++].unk_00 = 0x20;
        } else if (gs->unk_044[monthOff].unk_20[day] == 0xC) {
            gs->unk_69C[idx++].unk_00 = 0x24;
        } else if (gs->unk_044[monthOff].unk_20[day] == 0xD) {
            gs->unk_69C[idx++].unk_00 = 0x28;
        } else if (gs->unk_044[monthOff].unk_20[day] == 0xF) {
            gs->unk_69C[idx++].unk_00 = 0x2C;
        } else if (gs->unk_044[monthOff].unk_20[day] == 0x11) {
            gs->unk_69C[idx++].unk_00 = 0x31;
        } else if (gs->unk_044[monthOff].unk_20[day] == 0x13) {
            gs->unk_69C[idx++].unk_00 = 0x30;
        } else if (gs->unk_044[monthOff].unk_20[day] == 2 && D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_1BC[10].unk_0A = 0;
            D_800E6280.unk_1BC[10].unk_06 = 0x32;
            gs->unk_69C[idx].unk_01 = 0xA;
            gs->unk_69C[idx].unk_00 = 0x48;
            idx++;
            func_8006D4A0();
        } else if (gs->unk_044[monthOff].unk_20[day] == 2 && D_800E6280.unk_03E == 0x61) {
            gs->unk_69C[idx++].unk_00 = 0x43;
            func_8006D4A0();
        } else if (gs->unk_044[monthOff].unk_00[day] == 1 || gs->unk_044[monthOff].unk_00[day] == 2) {
            if (func_8006EA40() != 0) {
                D_800E6280.unk_56C[77]++;
                gs->unk_69C[idx].unk_00 = 0x45;
                gs->unk_69C[idx].unk_01 = 0xB;
                gs->unk_69C[idx].unk_02 = 4;
                idx++;
            } else {
                gs->unk_69C[idx++].unk_00 = 0x10;
            }
        } else if (D_800E6280.unk_03E == 0x61 && (month + monthOff) == 0xD && day == 3) {
            if (func_8005352C() == 1 && (gs->unk_0F4.h >> 12) != 6 && (*(u16 *)&gs->unk_66C[gs->unk_0F4.h >> 12].unk_02 & 0xF) >= 0xA) {
                gs->unk_69C[idx++].unk_00 = 0x26;
            } else {
                if ((gs->unk_0F4.h >> 12) < 6 && gs->unk_66C[gs->unk_0F4.h >> 12].unk_00 >= 0x17C) {
                    gs->unk_69C[idx++].unk_00 = 0x27;
                } else {
                    gs->unk_69C[idx].unk_00 = 0;
                    gs->unk_69C[idx].unk_01 = D_800E6280.unk_F68.w & 0xF;
                    gs->unk_69C[idx].unk_03 = sel;
                    if (func_8006BDA8(wd) != 0) {
                        nYes++;
                    } else {
                        nNo++;
                    }
                    idx++;
                }
            }
        } else if (gs->unk_044[monthOff].unk_20[day] == 0x12 && *(u8 *)((u8 *)&D_800E6280 + 0x3E) == 0x62) {
            /* FAKE: T-23003, the original reloads the byte at +0x3E here; a plain D_800E6280.unk_03E is CSE'd with the 0x61 test into $a1 and every later temporary shifts (first-symbol view, aggregate not known) */
            gs->unk_69C[idx++].unk_00 = 0x4A;
        } else if (gs->unk_044[monthOff].unk_00[day] == 0 || gs->unk_044[monthOff].unk_00[day] == 3) {
            if (gs->unk_044[monthOff].unk_20[day] == 5 || D_800E6280.unk_69C[0].unk_00 == 0x40) {
                gs->unk_69C[idx].unk_00 = 0x40;
                idx++;
            } else {
                gs->unk_69C[idx].unk_00 = 0;
                gs->unk_69C[idx].unk_01 = D_800E6280.unk_F68.w & 0xF;
                gs->unk_69C[idx].unk_03 = sel;
                res = func_8006BDA8(wd);
                if (res != 0) {
                    nYes++;
                } else {
                    nNo++;
                }
                if ((D_800E6280.unk_F68.w & 0xF) == 7) {
                    gs->unk_69C[idx].unk_02 = 0;
                } else {
                    gs->unk_69C[idx].unk_02 = res;
                }
                idx++;
                if (gs->unk_69C[idx - 1].unk_02 == 1 && (nYes + nNo) == 6 && gs->unk_044[monthOff].unk_00[day] == 0 && (rand() & 0xF) < 2) {
                    gs->unk_69C[idx].unk_00 = 0x47;
                    gs->unk_69C[idx].unk_01 = 0xC;
                    idx++;
                }
                if (gs->unk_044[monthOff].unk_00[day] == 3) {
                    if (D_800E6280.unk_03D == 2 && D_800E6280.unk_56C[73] < (u16)D_800E6280.unk_042 / 365U && D_800E6280.unk_1BC[10].unk_0C.f.b1 && !(get_g_zyotai_h(0xA) & 0x7F) && (rand() & 0xF) <= 0) {
                        gs->unk_69C[idx].unk_00 = 0x45;
                        gs->unk_69C[idx].unk_01 = 0xA;
                        gs->unk_69C[idx].unk_02 = 7;
                        idx++;
                    }
                    if (D_800E6280.unk_03D == 1 && wd == 2 && (monthOff + D_800E6280.unk_03F) == 8 && (rand() % 100) < 0x14 && cnt != 0
                        && (!((ScheduleGirlFlags *)&gs->unk_0F4)->b18 || (D_800E6280.unk_03E == 0x61 && !((ScheduleGirlFlags *)&gs->unk_0F4)->b18 && day == 0x1C))) {
                        ((ScheduleGirlFlags *)&D_800E6280.unk_0F4)->b18 = 1;
                        gs->unk_69C[idx].unk_00 = 0x49;
                        idx++;
                    }
                }
            }
            if (gs->unk_044[monthOff].unk_00[day] == 0) {
                if (D_800E6280.unk_69C[0].unk_00 != 0x40) {
                    if (D_800E6280.unk_03E != 0x5F || D_800E6280.unk_03F != 4) {
                        if (D_800E6280.unk_042 < 0x2D6 && nSpecial == 0) {
                            if (cnt < 2) {
                                if (func_8006EB64(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->unk_69C[idx].unk_00 = 0x48;
                                    gs->unk_69C[idx].unk_01 = argA;
                                    gs->unk_69C[idx].unk_02 = argB;
                                    idx++;
                                }
                            } else {
                                if (func_8006EEC4(&argA, &argB) != 0) {
                                    nSpecial++;
                                    gs->unk_69C[idx].unk_00 = 0x48;
                                    gs->unk_69C[idx].unk_01 = argA;
                                    gs->unk_69C[idx].unk_02 = argB;
                                    idx++;
                                }
                            }
                            if ((D_800E6280.unk_F68.w & 0xF) == 4 && func_8006F218(&argA, &argB) != 0) {
                                nSpecial++;
                                gs->unk_69C[idx].unk_00 = 0x48;
                                gs->unk_69C[idx].unk_01 = argA;
                                gs->unk_69C[idx].unk_02 = argB;
                                idx++;
                            }
                        }
                    }
                    if (nRand < 2) {
                        if ((rand() & 3) == 3) {
                            nRand++;
                            gs->unk_69C[idx].unk_00 = 0x44;
                            idx++;
                        }
                    }
                }
            }
        }
        if (wd == 5 && D_800E6280.unk_042 >= 0x21) {
            if (D_800E6280.unk_69C[0].unk_00 == 0x40) {
                if ((rand() & 1) && D_800E6280.unk_1BC[9].unk_0C.f.b1 && (rand() % 100) < 0x32 && get_g_zyotai_h(9) < 3U
                    && (D_800E6280.unk_108.unk_02 + D_800E6280.unk_100.unk_02 + D_800E6280.unk_104.unk_02) >= 0x12D) {
                    gs->unk_F68.b[0] = (gs->unk_F68.b[0] & 0xFFF0) | 3;
                    gs->unk_69C[idx].unk_00 = 0x47;
                    gs->unk_69C[idx].unk_01 = 9;
                    idx++;
                }
            } else if (gs->unk_044[monthOff].unk_00[day] == 0 && (D_800E6280.unk_F68.w & 0xF) != 7 && (rand() & 3) && nYes == 0 && nNo == 6) {
                if (sel != 0xF && sel != 0) {
                    if (!((D_800E6280.unk_F68.w & 0xF) == 5 && sel == 0)) {
                        gs->unk_69C[idx].unk_00 = 0x47;
                        gs->unk_69C[idx].unk_01 = sel;
                        idx++;
                    }
                } else {
                    r = D_800E6280.unk_F68.w & 0xF;
                    /* FAKE: T-23003, unsigned 1: the original compares against $at, not the hoisted $s5 */
                    if ((r == 0 || r == 1U || r == 3 || r == 2) && (rand() % 100) < 7 && get_g_zyotai_h(9) < 3U && D_800E6280.unk_1BC[9].unk_0C.f.b1) {
                        gs->unk_69C[idx].unk_00 = 0x47;
                        gs->unk_69C[idx].unk_01 = 9;
                        idx++;
                    } else if (sel == 0 && (D_800E6280.unk_F68.w & 0xF) != 5) {
                        gs->unk_69C[idx].unk_00 = 0x47;
                        gs->unk_69C[idx].unk_01 = 0;
                        idx++;
                    }
                }
            }
        }
        if (birth_day_check_days(0, D_800E6280.unk_03F + monthOff, day) == 1) {
            gs->unk_69C[idx].unk_00 = 0x32;
            gs->unk_69C[idx].unk_01 = 0;
            idx++;
        }
        for (j = 1; j < 0xB; j++) {
            if (D_800E6280.unk_1BC[j].unk_0C.f.b1 && D_800E6280.unk_1BC[j].unk_0C.f.b6) {
                if (birth_day_check_days(j, D_800E6280.unk_03F + monthOff, day) == 1) {
                    gs->unk_69C[idx].unk_00 = 0x32;
                    gs->unk_69C[idx].unk_01 = j;
                    idx++;
                }
            }
        }
        if ((gs->unk_0F4.b[1] & 0xF) != 3) {
            if (birth_day_check_days(0xF, D_800E6280.unk_03F + monthOff, day) == 1) {
                gs->unk_69C[idx].unk_00 = 0x33;
                idx++;
            }
        }
        if (wd == 5) {
            gs->unk_69C[idx].unk_00 = 0xFC;
            idx++;
        }
        if (wd != 6) {
            gs->unk_69C[idx].unk_00 = 0xFB;
            idx++;
        }
        wd++;
        day++;
    }
    func_8004284C();
}
#else
INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D7EC);
#endif

s32 func_8006EA40(void) {
    s32 count;
    s32 i;
    /* FAKE: the helper writes 16 bytes; four extra bytes reproduce the original frame and buffer slots. T-11670. */
    u8 rank[20];
    s16 points[16];

    func_80053418(rank, points);
    count = 0;
    for (i = 0; i != 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && points[i] >= 0x1F5) {
            count++;
        }
    }
    if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 2 && D_800E6280.unk_040 == 0xF && count >= 2 &&
        D_800E6280.unk_1BC[11].unk_0C.f.f9 >= 4 && D_800E6280.unk_56C[0x4C] != 0 && D_800E6280.unk_56C[0x4E] != 0 &&
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
    if (!(D_800E6280.unk_1BC[1].unk_0C.f.b1) && !((u32)D_800E6280.unk_F68.w & 0xF) && (D_800E6280.unk_100.unk_02 >= 0x37)) {
        *arg0 = 1;
        *arg1 = D_800E6280.unk_10F8 & 1;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[2].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 1) && (D_800E6280.unk_104.unk_02 >= 0x3C)) {
        *arg0 = 2;
        *arg1 = D_800E6280.unk_10F8 & 3;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[3].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 2) && (D_800E6280.unk_108.unk_02 >= 0x32)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[5].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 8) && (D_800E6280.unk_10C.unk_02 >= 0x51) && (D_800E6280.unk_114.unk_02 >= 0x51)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[6].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9) && (D_800E6280.unk_10C.unk_02 >= 0x1F) && (D_800E6280.unk_118.unk_02 >= 0x1F)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[7].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 6) && (D_800E6280.unk_114.unk_02 >= 0x65)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[8].unk_0C.f.b1)) {
        if ((g->unk_1BC[8].unk_0C.f.f9 == 0) && (D_800E6280.unk_110.unk_02 >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 1) && (D_800E6280.unk_110.unk_02 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 2) && (D_800E6280.unk_110.unk_02 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 3) && (((u32)D_800E6280.unk_F68.w & 0xF) == 5) && (D_800E6280.unk_110.unk_02 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }
    if (!(D_800E6280.unk_1BC[4].unk_0C.f.b1) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) && (D_800E6280.unk_118.unk_02 >= 0x24)) {
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
    if (!(D_800E6280.unk_1BC[1].unk_0C.f.b1) && !((u32)D_800E6280.unk_F68.w & 0xF) && (D_800E6280.unk_100.unk_02 >= 0x4B)) {
        *arg0 = 1;
        *arg1 = D_800E6280.unk_10F8 & 1;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[2].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 1) && (D_800E6280.unk_104.unk_02 >= 0x55)) {
        *arg0 = 2;
        *arg1 = D_800E6280.unk_10F8 & 3;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[3].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 2) && (D_800E6280.unk_108.unk_02 >= 0x41)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[5].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 8) && (D_800E6280.unk_10C.unk_02 >= 0x54) && (D_800E6280.unk_114.unk_02 >= 0x54)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[6].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9) && (D_800E6280.unk_10C.unk_02 >= 0x47) && (D_800E6280.unk_118.unk_02 >= 0x47)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[7].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 6) && (D_800E6280.unk_114.unk_02 >= 0x7E)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[8].unk_0C.f.b1)) {
        if ((g->unk_1BC[8].unk_0C.f.f9 == 0) && (D_800E6280.unk_110.unk_02 >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 1) && (D_800E6280.unk_110.unk_02 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 2) && (D_800E6280.unk_110.unk_02 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((g->unk_1BC[8].unk_0C.f.f9 == 3) && (((u32)D_800E6280.unk_F68.w & 0xF) == 5) && (D_800E6280.unk_110.unk_02 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }
    if (!(D_800E6280.unk_1BC[4].unk_0C.f.b1) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) && (D_800E6280.unk_118.unk_02 >= 0x60)) {
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
    club = (u16) g->unk_0F4.h >> 0xC;
    if ((rand() % 3) != 0) {
        return 0;
    }
    if (!(g->unk_1BC[1].unk_0C.f.b1) && (club == ((u8) g->unk_1BC[1].unk_0C.b[2] >> 4)) && ((u16) g->unk_66C[club].unk_00 >= 0x15U)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[1].unk_0C)->b18 = 1;
        *arg0 = 1;
        if (club == 0) {
            *arg1 = (g->unk_10F8 & 1) + 2;
            return 1;
        }
        *arg1 = 4;
        return 1;
    }
    if (!(g->unk_1BC[2].unk_0C.f.b1) && (club == ((u8) g->unk_1BC[2].unk_0C.b[2] >> 4)) && ((u16) g->unk_66C[club].unk_00 >= 0x15U)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[2].unk_0C)->b18 = 1;
        *arg0 = 2;
        if (club == 2) {
            *arg1 = 4;
            return 1;
        }
        *arg1 = 5;
        return 1;
    }
    if (!(g->unk_1BC[3].unk_0C.f.b1) && (club == ((u8) g->unk_1BC[3].unk_0C.b[2] >> 4)) && ((u16) g->unk_66C[club].unk_00 >= 0x15U)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[3].unk_0C)->b18 = 1;
        *arg0 = 3;
        if (club == 4) {
            *arg1 = 1;
            return 1;
        }
        *arg1 = 2;
        return 1;
    }
    if (!(g->unk_1BC[4].unk_0C.f.b1) && (club == ((u8) g->unk_1BC[4].unk_0C.b[2] >> 4)) && ((u16) g->unk_66C[club].unk_00 >= 0x15U)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[4].unk_0C)->b18 = 1;
        *arg0 = 4;
        if (club == 6) {
            *arg1 = 1;
            return 1;
        }
        *arg1 = 2;
        return 1;
    }
    if (!(g->unk_1BC[5].unk_0C.f.b1) && (club == 8) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) != 8) && ((u16) g->unk_66C[club].unk_00 >= 0x15U)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[5].unk_0C)->b18 = 1;
        *arg0 = 5;
        *arg1 = 1;
        return 1;
    }
    if (!(g->unk_1BC[6].unk_0C.f.b1) && (club == 9) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) != 9)) {
        ((ScheduleGirlFlags *)&g->unk_1BC[6].unk_0C)->b18 = 1;
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
        if (g->unk_1BC[1].unk_0C.f.b1) {
            firstGirl = 1;
            firstScore = rank[1] - D_800B6748;
        }
        selfScore = rank[0] - D_800B6748;
    default:
        break;
    case 1:
        if (g->unk_1BC[2].unk_0C.f.b1) {
            firstGirl = 2;
            firstScore = rank[2] - D_800B6758;
        }
        selfScore = rank[0] - D_800B6758;
        break;
    case 2:
        if (g->unk_1BC[3].unk_0C.f.b1) {
            firstGirl = 3;
            firstScore = rank[3] - D_800B674C;
        }
        selfScore = rank[0] - D_800B674C;
        break;
    case 3:
        if (g->unk_1BC[5].unk_0C.f.b1) {
            firstGirl = 5;
            firstScore = rank[5] - D_800B6748;
        }
        if (g->unk_1BC[6].unk_0C.f.b1) {
            secondGirl = 6;
            secondScore = rank[6] - D_800B6748;
        }
        selfScore = rank[0] - D_800B6754;
        break;
    case 4:
        if (((u16)g->unk_0F4.h >> 12) == (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4))) {
            selfScore = rank[0] - 0xB;
            if (!(g->unk_1BC[0].unk_0C.f.b2)) {
                selfScore = 0;
            }
            if (((u16)g->unk_0F4.h >> 12) == 0xA) {
                if (g->unk_1BC[0xA].unk_0C.f.b1) {
                    if ((g->unk_1BC[0xA].unk_0C.f.b2) && ((((u8) g->unk_1BC[0xA].unk_0C.b[2] >> 4)) == 0xA)) {
                        firstGirl = 0xA;
                        firstScore = rank[10] - 0xB;
                    }
                }
            }
        } else {
            switch (((u16)g->unk_0F4.h >> 12)) {
            case 0:
                if (g->unk_1BC[1].unk_0C.f.b1) {
                    if ((g->unk_1BC[1].unk_0C.f.b2) && ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4)) == 0)) {
                        firstGirl = 1;
                        firstScore = rank[1] - 0xB;
                    }
                }
                break;
            case 1:
                if ((g->unk_1BC[1].unk_0C.f.b1) && (g->unk_1BC[1].unk_0C.f.b2)) {
                    if ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4)) == 1) {
                        firstGirl = 1;
                        firstScore = rank[1] - 0xB;
                    }
                }
                break;
            case 2:
                if (g->unk_1BC[2].unk_0C.f.b1) {
                    if ((g->unk_1BC[2].unk_0C.f.b2) && ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4)) == 2)) {
                        firstGirl = 2;
                        firstScore = rank[2] - 0xB;
                    }
                }
                break;
            case 3:
                if (g->unk_1BC[2].unk_0C.f.b1) {
                    if ((g->unk_1BC[2].unk_0C.f.b2) && ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4)) == 3)) {
                        firstGirl = 2;
                        firstScore = rank[2] - 0xB;
                    }
                }
                break;
            case 4:
                if (g->unk_1BC[3].unk_0C.f.b1) {
                    if ((g->unk_1BC[3].unk_0C.f.b2) && ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4)) == 4)) {
                        firstGirl = 3;
                        firstScore = rank[3] - 0xB;
                    }
                }
                break;
            case 5:
                if (g->unk_1BC[3].unk_0C.f.b1) {
                    if ((g->unk_1BC[3].unk_0C.f.b2) && ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4)) == 5)) {
                        firstGirl = 3;
                        firstScore = rank[3] - 0xB;
                    }
                }
                break;
            case 6:
                if ((g->unk_1BC[4].unk_0C.f.b1) && (g->unk_1BC[4].unk_0C.f.b2)) {
                    if ((((u8) g->unk_1BC[4].unk_0C.b[2] >> 4)) == 6) {
                        firstGirl = 4;
                        firstScore = rank[4] - 0xB;
                    }
                }
                break;
            case 7:
                if (g->unk_1BC[4].unk_0C.f.b1) {
                    if ((g->unk_1BC[4].unk_0C.f.b2) && ((((u8) g->unk_1BC[4].unk_0C.b[2] >> 4)) == 7)) {
                        firstGirl = 4;
                        firstScore = rank[4] - 0xB;
                    }
                }
                break;
            case 8:
                if (g->unk_1BC[5].unk_0C.f.b1) {
                    if (g->unk_1BC[5].unk_0C.f.b2) {
                        if ((((u8) g->unk_1BC[5].unk_0C.b[2] >> 4)) == 8) {
                            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4)) != 8) {
                                firstGirl = 5;
                                firstScore = rank[5] - 0xB;
                            }
                        }
                    }
                }
                break;
            /* FAKE: signed comparison keeps 9 separate from the unsigned modulo constant. T-10730. */
            case 9:
                if (g->unk_1BC[6].unk_0C.f.b1) {
                    if (g->unk_1BC[6].unk_0C.f.b2) {
                        if (((s32)((u8) g->unk_1BC[6].unk_0C.b[2] >> 4)) == 9) {
                            if (((s32)((u8) g->unk_1BC[0].unk_0C.b[2] >> 4)) != 9) {
                                firstGirl = 6;
                                firstScore = rank[6] - 0xB;
                            }
                        }
                    }
                }
                break;
            case 10:
                if ((g->unk_1BC[0xA].unk_0C.f.b1) && (g->unk_1BC[0xA].unk_0C.f.b2)) {
                    if ((((u8) g->unk_1BC[10].unk_0C.b[2] >> 4)) == 0xA) {
                        firstGirl = 0xA;
                        firstScore = rank[10] - 0xB;
                    }
                }
                break;
            }
        }
        break;
    case 5:
        if (g->unk_1BC[8].unk_0C.f.b1) {
            firstGirl = 8;
            firstScore = rank[8] - D_800B6750;
        }
        selfScore = rank[0] - D_800B6750;
        break;
    case 6:
        if (g->unk_1BC[7].unk_0C.f.b1) {
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
            counts[i] += g->unk_1BC[i].unk_14[event];
        }
    }
    for (girl=0; girl<11; girl++) {
        if ((g->unk_1BC[girl].unk_0C.f.b1) && (get_g_zyotai_h(girl) < 2U) && (counts[girl] != 0) && ((rand() & 0xF) < 5)) {
            switch (girl) {
            case 0:
                if ((rand() % 100) < 3) {
                    if (((GsWord *)&g->unk_10F8)->w & 1) {
                        if ((func_80066ACC() == 1) && (g->unk_56C[0] == 0) && ((u32) (get_g_zyotai_s(girl) & 0x7F) < 2U)) {
                            g->unk_56C[0] = (u8) (g->unk_56C[0] + 1);
                            *arg0 = 0;
                            *arg1 = 1;
                            return 1;
                        }
                        break;
                    }
                    if (g->unk_56C[1] == 0) {
                        g->unk_56C[1] = (u8) (g->unk_56C[1] + 1);
                        *arg0 = 0;
                        *arg1 = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 1:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->unk_56C[8] == 0) {
                        g->unk_56C[8] += 1;
                        *arg0 = 1;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 2:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->unk_56C[14] == 0) {
                        g->unk_56C[14] += 1;
                        *arg0 = 2;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 3:
                if (((rand() % 100) < 7) && (func_80066ACC() == 1)) {
                    if (g->unk_56C[21] == 0) {
                        g->unk_56C[21] += 1;
                        *arg0 = 3;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 4:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    g->unk_56C[27] += 1;
                    *arg0 = 4;
                    *arg1 = 1;
                    return 1;
                }
                break;
            case 5:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1) && (g->unk_56C[33] == 0)) {
                    g->unk_56C[33] += 1;
                    *arg0 = 5;
                    *arg1 = 1;
                    return 1;
                }
                if (((rand() % 100) < 0x32) && (g->unk_56C[37] == 1)) {
                    if (g->unk_56C[34] == 0) {
                        g->unk_56C[34] += 1;
                        *arg0 = 5;
                        *arg1 = 2;
                        return 1;
                    }
                    break;
                }
                break;
            case 6:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1)) {
                    if (g->unk_56C[40] == 0) {
                        g->unk_56C[40] += 1;
                        *arg0 = 6;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 7:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1)) {
                    if (g->unk_56C[46] == 0) {
                        g->unk_56C[46] += 1;
                        *arg0 = 7;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 8:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0)) {
                    if (g->unk_56C[54] == 0) {
                        g->unk_56C[54] += 1;
                        *arg0 = 8;
                        *arg1 = 1;
                        return 1;
                    }
                    break;
                }
                break;
            case 9:
                if (((rand() % 100) < 3) && (func_80066ACC() == 0) && (g->unk_56C[60] == 0)) {
                    g->unk_56C[60] = (u8) (g->unk_56C[60] + 1);
                    *arg0 = 9;
                    *arg1 = 1;
                    return 1;
                }
                if ((g->unk_56C[60] == 1) && (g->unk_1BC[9].unk_0C.f.b14 == 1)) {
                    *(u8 *)((unsigned char *)(&g->unk_56C[60]) + (1)) = (u8) (*(u8 *)((unsigned char *)(&g->unk_56C[60]) + (1)) + 1);
                    g->unk_1BC[9].unk_0C.b[1] = (u8) (g->unk_1BC[9].unk_0C.b[1] & 0xFFBF);
                    *arg0 = 9;
                    *arg1 = 2;
                    return 1;
                }
                break;
            case 10:
                if (((rand() % 100) < 3) && (func_80066ACC() == 1) && (g->unk_56C[27] != 0) && (g->unk_56C[67] == 0)) {
                    g->unk_56C[67] += 1;
                    *arg0 = 0xA;
                    *arg1 = 1;
                    return 1;
                }
                if (((rand() % 100) < 0xA) && (g->unk_03E == 0x61) && (g->unk_03F == 9) && ((u8) g->unk_040 >= 0xEU)) {
                    if (g->unk_56C[68] == 0) {
                        g->unk_56C[68] += 1;
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
    if (((u8) g->unk_1BC[12].unk_0C.b[3] >= 0x15U) && (g->unk_56C[80] == 0) && (func_80066ACC() == 0)) {
        g->unk_56C[80] += 1;
        *arg0 = 0xC;
        *arg1 = 2;
        return 1;
    }
    if ((u8) g->unk_1BC[12].unk_0C.b[3] >= 0x29U) {
        if (g->unk_56C[79] == 0) {
            g->unk_56C[79] += 1;
            *arg0 = 0xC;
            *arg1 = 1;
            return 1;
        }
    }
    if (((u8)g->unk_1BC[12].unk_0C.b[3] >= 0x41U) && (g->unk_56C[81] == 0)) {
        g->unk_56C[81] += 1;
        *arg0 = 0xC;
        *arg1 = 3;
        return 1;
    }
    if (((rand() % 100) < 4) && (g->unk_56C[77] == 0) && (g->unk_1BC[11].unk_14[0] == 0) && ((u32) ((u32) (g->unk_1BC[11].unk_0C.w << 0x14) >> 0x1D) < 5U) && ((u16) g->unk_042 >= 0x1DU)) {
        g->unk_56C[74] += 1;
        g->unk_1BC[11].unk_14[0] = 4;
        *arg0 = 0xB;
        *arg1 = 1;
        return 1;
    }
    if ((g->unk_56C[53] == 0) && (g->unk_56C[52] == 1)) {
        g->unk_56C[53] += 1;
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
    n = (u32)D_800E7D10.unk_000 % 11;
    func_80053418(sp7C, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((D_800E6280.unk_1BC[n].unk_0C.f.b1 != 0) && (D_800E6280.unk_1BC[n].unk_0A < 0x3C) && (get_g_zyotai_s(n) != 4) && (sp7C[n] >= 0x11) && ((rand() % 100) < 0x32)) {
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
    i = D_800E6280.unk_03F;
    idx = D_800E6280.unk_040;
    if (((u32)(idx + 6) / 7 == 2) && ((u32)(D_800E6280.unk_0F4.h >> 12) < 0xB)) {
        return 0;
    }
    if ((n == i) && (idx == 0x11)) {
        return 0;
    }
    if ((n == i) && (idx == 0x18)) {
        return 0;
    }
    if ((D_800E6280.unk_03E == 0x62) && ((u32)i >= 2)) {
        return 0;
    }
    n = (u32)D_800E7D10.unk_000 % 11;
    func_80053418(sp7C, sp5C);
    for (i = 0, idx = n; i != 11; i++, idx++) {
        n = idx % 11;
        if ((D_800E6280.unk_1BC[n].unk_0C.f.b1 != 0) && (D_800E6280.unk_1BC[n].unk_0A < 0x3C) && (get_g_zyotai_s(n) != 4) && (sp7C[n] >= 0x21) && ((rand() % 100) < 8)) {
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
        if (D_800E6280.unk_69C[i].unk_00 == 0x44) {
            if (D_800E6280.unk_69C[i + 1].unk_00 == 0x33 || D_800E6280.unk_69C[i + 1].unk_00 == 0x32 || D_800E6280.unk_69C[i + 1].unk_00 == 0x47) {
                D_800E6280.unk_69C[i].unk_00 = 0xFA;
            } else if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
                D_800E6280.unk_69C[i].unk_00 = 0xFA;
            } else if (special_event_select(&girl, &arg)) {
                D_800E6280.unk_69C[i].unk_00 = 0x45;
                D_800E6280.unk_69C[i].unk_01 = girl;
                D_800E6280.unk_69C[i].unk_02 = arg;
            } else if (date_sasoi_judge(&girl) == 1 && cnt == 0) {
                cnt++;
                D_800E6280.unk_69C[i].unk_00 = 0x46;
                D_800E6280.unk_69C[i].unk_01 = girl;
            } else if (geko_judge(&girl)) {
                D_800E6280.unk_69C[i].unk_01 = girl;
            } else {
                D_800E6280.unk_69C[i].unk_00 = 0xFA;
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
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            q = D_800E6280.unk_040 / 7;
            if (q == 0) {
                if (D_800E6280.unk_1BC[i].unk_06 > 0) {
                    D_800E6280.unk_1BC[i].unk_06 -= 1;
                }
            } else if (two == q && i == 0) {
                if (D_800E6280.unk_1BC[i].unk_06 > 0) {
                    D_800E6280.unk_1BC[i].unk_06 -= 1;
                }
            }
        }
    }
}

void syoushin_up(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && (s32)get_h_yuukou(i) < 0xA) {
            D_800E6280.unk_1BC[i].unk_0A += 5;
        }
    }
}

void schdeule_girls_param(void) {
    if (D_800E6280.unk_03D == 0) {
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
    if (D_800E6280.unk_1109 == 0) {
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
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8007132C();
        break;
    case 1:
        func_80070FD0();
        break;
    }
}
void func_80070FD0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80067438();
        D_800E6280.unk_110D++;
    } else if (func_80044E8C() == 1) {
        func_80044750(0x200);
        main_state_set(0x63);
    }
}

void func_80071038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_02 > 0x80) {
            D_800E6280.unk_1BC[i].unk_02 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_02 < 0) {
            D_800E6280.unk_1BC[i].unk_02 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_06 > 0x80) {
            D_800E6280.unk_1BC[i].unk_06 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_06 < 0) {
            D_800E6280.unk_1BC[i].unk_06 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A > 0x80) {
            D_800E6280.unk_1BC[i].unk_0A = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0) {
            D_800E6280.unk_1BC[i].unk_0A = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0x32) {
            D_800E6280.unk_1BC[i].unk_0C.b[2] &= 0xFFFE;
            D_800E6280.unk_1BC[i].unk_0C.b[2] &= 0xFD;
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
        if (D_800E6280.unk_1BC[12].unk_0C.b[3] >= 0x46U && D_800E6280.unk_56C[79] == 2) {
            return 0;
        }
    }
    if (v[2] < 0x1F4) {
        return 0;
    }
    if (!D_800E6280.unk_1BC[2].unk_0C.f.b1) {
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

    if (D_800E6280.unk_03F == 5 && D_800E6280.unk_040 == 6) {
        for (i = 0; i < 11; i++) {
            D_800E6280.unk_1BC[i].unk_0C.b[0] &= 0xFFEF;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8007132C);

void func_800722C4(void) {
    GameState *state;

    state = &D_800E6280;
    switch (D_800E6280.unk_03F) {
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
        ((CharFlags *)state->unk_F64)->b14 = ((CharFlags *)D_800E6280.unk_F64)->b13;
        ((CharFlags *)state->unk_F64)->b13 = 0;
        break;
    }
}

void func_80072338(void) {
    D_800E6280.unk_041++;
    if (D_800E6280.unk_041 == 7) {
        D_800E6280.unk_041 = 0;
    }
    D_800E6280.unk_040++;
    switch ((*(u8 *)&D_800E6280.unk_03F)) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (D_800E6280.unk_040 >= 0x20) {
            D_800E6280.unk_040 = 1;
            (*(u8 *)&D_800E6280.unk_03F)++;
            func_800722C4();
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (D_800E6280.unk_040 >= 0x1F) {
            D_800E6280.unk_040 = 1;
            (*(u8 *)&D_800E6280.unk_03F)++;
            func_800722C4();
        }
        break;
    case 2:
        if (D_800E6280.unk_03E == 0x60) {
            if (D_800E6280.unk_040 >= 0x1E) {
                D_800E6280.unk_040 = 1;
                (*(u8 *)&D_800E6280.unk_03F)++;
                func_800722C4();
            }
        } else if (D_800E6280.unk_040 >= 0x1D) {
            D_800E6280.unk_040 = 1;
            (*(u8 *)&D_800E6280.unk_03F)++;
            func_800722C4();
        }
        break;
    }
    if ((*(u8 *)&D_800E6280.unk_03F) >= 0xD) {
        D_800E6280.unk_03E++;
        (*(u8 *)&D_800E6280.unk_03F) = 1;
    }
    ((GsHalf *)&D_800E6280.unk_042)->h++;
    if ((*(u8 *)&D_800E6280.unk_03F) % 3 == 0) {
        func_8006D038();
    }
    if (D_800E6280.unk_71C != 0) {
        func_8007259C(0);
        D_800E6280.unk_71F = 0;
    } else {
        func_8007259C(0);
        D_800E6280.unk_71F = 1;
    }
    if ((D_800E6280.unk_041 == 6) && (*(u16 *)&D_800E6280.unk_0F6 & 1) && (D_800E6280.unk_69C[0].unk_00 != 0x40)) {
        *(s8 *)&D_800E6280.unk_0F6 = (u8)D_800E6280.unk_0F6.h & 0xFFFE;
    }
    func_8006B0C8();
}

void func_8007259C(u8 arg0) {
    D_800E6280.unk_71E = arg0;
    D_800E6280.unk_71D = arg0;
}

void func_800725B0(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_800726F0();
        break;
    case 1:
        D_800E6280.unk_71C += 1;
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

    switch (g->unk_110A) {
    case 0:
        func_8004E58C();
        func_80066104("ダミー");
        func_80065F34(0);
        gnsx(&g->unk_0D4);
        flag = g->unk_03D;
        if (flag != 0 && g->unk_040 == 2) {
            g->unk_03D = 0;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->unk_03D != 0) {
            g->unk_03D = 0;
            sndisp("明日から新学期だ）", 0, 0x1F);
        } else if (g->unk_03F == 7) {
            sndisp("明日から夏休みだ）", 0, 0x1F);
            g->unk_03D = 1;
        } else if (g->unk_03F == 0xC) {
            sndisp("明日から冬休みだ）", 0, 0x1F);
            g->unk_03D = 2;
        } else if (g->unk_03F == 3) {
            sndisp("明日から春休みだ）", 0, 0x1F);
            g->unk_03D = 3;
        } else {
            g->unk_03D = 0;
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
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80072944();
        break;
    case 1:
        D_800E6280.unk_71C += 1;
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
    switch (D_800E6280.unk_110A) {
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

    switch (g->unk_110D) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        g->unk_110D++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(g->unk_03E, g->unk_03F, 1);
            if (g->unk_03F % 3 == 0) {
                if (g->unk_03F == 9 && g->unk_040 == 1 && g->unk_03E == 0x60) {
                    func_80042808();
                } else if (g->unk_03F == 9) {
                    func_8004E58C();
                    func_80066104("ダミー");
                    func_80065F34(0);
                    gnsx(&g->unk_0D4);
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
    D_800E6280.unk_71C++;
    main_state_set(0x31);
    /* no return on this path: the original leaves $v0 as the call result */
}

void func_80072BC0(void) {
    switch (D_800E6280.unk_1109) {
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
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
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
    lim = g->unk_118.unk_02;
    lim = lim / 8 + 0xCC;
    if (lim >= 0xFF) {
        lim = 0xFE;
    }
    switch (g->unk_0F4.b[1] & 0xF) {
    case 0:
        state = 0;
        if ((g->unk_0F6.b[0] >> 5) == 0) {
            if ((g->unk_0FC.unk_02 < 0x14) && (lim < (rand() & 0xFF))) {
                state = 1;
                flag = 1;
            } else if (g->unk_0FC.unk_02 == 0) {
                state = 1;
                flag = 1;
            }
            if ((g->unk_11C.unk_02 >= 0x51) && ((u32) lim < (u32) (D_800E7D10.unk_000 & 0xFF))) {
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
        if ((g->unk_11C.unk_02 >= 0x51) && ((u32) lim < (u32) (D_800E7D10.unk_000 & 0xFF))) {
            state = 3;
            flag = 1;
        }
        if (g->unk_0FC.unk_02 >= 0x1F) {
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
        if ((g->unk_0FC.unk_02 < 0x14) && (lim < (rand() & 0xFF))) {
            state = 3;
            flag = 1;
        } else if (g->unk_0FC.unk_02 == 0) {
            state = 3;
            flag = 1;
        }
        if (g->unk_11C.unk_02 < 0x46) {
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
        if ((g->unk_11C.unk_02 < 0x46) && (g->unk_0FC.unk_02 >= 0x1F)) {
            state = 0;
            flag = 1;
        }
        if (state == 0) {
            func_80042940(7);
            break;
        }
        if (((u8 *) g->unk_044)[g->unk_040] == 0) {
            start = rand() % 11;
            for (i = 0; i != 11; i++) {
                k = (start + i) % 11;
                if (g->unk_1BC[k].unk_0C.f.b1 && (get_h_tokimeki(k) >= 0x33U) && (get_g_zyotai_s(start) != 4)) {
                    if ((rand() & 0xFF) < 0x3F) {
                        g->unk_75D = k;
                        main_state_set(0x62);
                        return 1;
                    }
                }
            }
        }
        break;
    default:
        g->unk_0F4.b[1] &= 0xFFF0;
        break;
    }
    if (flag == 0) {
        func_80042908(0xFF);
    }
}

s32 func_80073198(void) {
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("怪我");
        gnsx(&D_800E6280.unk_0D4[0]);
        sndi("怪我をしてしまった）", 0, 0x1F);
        D_800E6280.unk_0F4.b[1] = (D_800E6280.unk_0F4.b[1] & 0xFFF0) | 1;
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        if ((u32)D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32)D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F88 != 0)) {
                D_800E6280.unk_110D += 1;
                return;
            }
        }
        return;
    case 2:
        if (((u32)(D_800E6280.unk_10F8 & 0xFF) < 0x19U) && (D_800E6280.unk_56C[0x4B] == 0) && (D_800E6280.unk_03D == 0)) {
            main_state_set(0x5D);
            D_80122CD4 = 0xB;
            D_80122CD0 = 2;
            D_800E6280.unk_56C[0x4B] = 1;
            return 1;
        }
        func_80042908(0xFF);
        break;
    }
}

s32 func_800732F8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("ノイローゼ");
        gnsx(&D_800E6280.unk_0D4[0]);
        sndi("ノイローゼになってしまった）", 0, 0x1F);
        D_800E6280.unk_0F4.b[1] = (D_800E6280.unk_0F4.b[1] & 0xFFF0) | 2;
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32) D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F80 != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800733D8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("病気");
        gnsx(&D_800E6280.unk_0D4[0]);
        func_8004500C(0, 0x202);
        sndi("病気になってしまった）", 0, 0x1F);
        D_800E6280.unk_0F4.b[1] = (D_800E6280.unk_0F4.b[1] & 0xFFF0) | 3;
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32) D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F80 != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800734C4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.unk_0F6.b[0] = (D_800E6280.unk_0F6.b[0] & 0xFF1F) | 0x60;
        D_800E6280.unk_0F4.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.unk_0D4[0]);
        sndi("怪我が治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32) D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F80 != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800735B8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.unk_0F6.b[0] = (D_800E6280.unk_0F6.b[0] & 0xFF1F) | 0x60;
        D_800E6280.unk_0F4.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.unk_0D4[0]);
        sndi("ノイローゼが治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32) D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F80 != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}

s32 func_800736AC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_80066104("治癒");
        D_800E6280.unk_0F6.b[0] = (D_800E6280.unk_0F6.b[0] & 0xFF1F) | 0x60;
        D_800E6280.unk_0F4.b[1] &= 0xFFF0;
        gnsx(&D_800E6280.unk_0D4[0]);
        sndi("病気が治った）", 0, 0x1F);
        k_disp_start(3);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.w >= 0x3D) {
            if (((u32) D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F80 != 0)) {
                func_80042908(0xFF);
            }
        }
        break;
    }
}
