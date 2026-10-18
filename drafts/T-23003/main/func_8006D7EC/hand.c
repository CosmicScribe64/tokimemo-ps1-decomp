/* T-23003 hand draft of main:func_8006D7EC (4,692 B). Not built from this file: src/main/8006CB30.c carries the same body under
 * NON_MATCHING. Typed by hand from the m2c draft of T-16100 (GameState fields, gs-> where the original has $s2 as base).
 * Opcode-aligned against the original: 1173 of 1173 instructions, 2 differing instructions (idx++ folded to li,
 * the second func_8006BDA8 result narrowed with andi where the original copies it with or). The local typedef F6Bits is
 * file-local in src. Needs scratch compile + drafts/T-23003/adiff.py to iterate (see matching-notes, section T-23003). */
/* T-23003: the 3-bit counter in bits 5..7 of the byte at GameState +0xF6 (read with lbu, decremented as a bit-field). */
typedef struct F6Bits {
    u32 pad0 : 5;
    u32 f : 3;
    u32 pad1 : 24;
} F6Bits;

void func_8006D7EC(void) {
    GameState *gs;
    u8 res; /* FAKE: T-23003, the original frame has seven register locals (two words above idx); res holds the second call result in $a0 (u8: narrows with andi where the original copies with or) */
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
        if ((gs->unk_0F4.h >> 12) != 0xB && /* FAKE: T-23003, the unsigned constant is a ucode constant of its own and takes $a1 as in the original */ ((u32)gs->unk_F68.w & 0xF) == 4U) {
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
        /* FAKE: T-23003, the original reloads the byte at +0x3E here; a plain D_800E6280.unk_03E is CSE'd with the 0x61 test into $a1 and every later temporary shifts (first-symbol view, aggregate not known) */
        } else if (gs->unk_044[monthOff].unk_20[day] == 0x12 && *(u8 *)((u8 *)&D_800E6280 + 0x3E) == 0x62) {
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
                    if ((r == 0 || r == 1U /* FAKE: T-23003, unsigned 1: the original compares against $at, not the hoisted $s5 */ || r == 3 || r == 2) && (rand() % 100) < 7 && get_g_zyotai_h(9) < 3U && D_800E6280.unk_1BC[9].unk_0C.f.b1) {
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
