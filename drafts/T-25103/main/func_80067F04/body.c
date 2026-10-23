/* One mark of the two-month calendar: frame `frame` in the cell of day index `d` of column `side` (0 this month,
 * 1 next month), then the next record. The four statements expand on one source line, which as1 schedules as one
 * group (the frame store after the argument copy where the record address is a constant). T-25103 */
#define CAL_MARK(frame, side, d) \
    D_8011ECD0[n + 0x61].unk_18 = (frame); D_8011ECD0[n + 0x61].unk_26 = func_80068898((side), (d)); \
    D_8011ECD0[n + 0x61].unk_2A = func_800688F0((side), (d)); n++
/* Frame of the schedule entry of day i + 1 in column `side`; every use reads it again (the original reloads it
 * after each call). T-25103 */
#define DAY_KIND(side) D_800B5C10[D_800E6280.unk_044[side].unk_20[i + 1]]

/* Calendar marks in records 0x61.. of D_8011ECD0 (at most 0x3F): the date word at +0xF8, the flagged Rec38 dates,
 * the active +0x54C entries, two 0x6F marks in periods 6..10, the day-schedule frames of both months (a multi-day
 * event repeats its frame on the following empty days) and the +0x75E records of this month. */
void func_80067F04(void) {
    GameState *gs;
    s32 i;
    s32 n;
    s32 left;
    /* FAKE: keeps kind in the original's sixth local slot (sp+0x68) and the frame at 0x80; real local unknown.
     * T-25103 */
    s32 pad;
    s32 kind;
    u32 month;
    u32 day;
    s32 day0;
    s32 day1;
    u32 year;

    gs = &D_800E6280;
    for (i = 0; i < 0x20; i++) {
        func_80048F64(i + 0x61);
        D_8011ECD0[i + 0x61].unk_01 = 3;
        D_8011ECD0[i + 0x61].unk_02 = 0x40;
        D_8011ECD0[i + 0x61].unk_38 = 0;
        D_8011ECD0[i + 0x61].unk_03 = 0x8C;
        D_8011ECD0[i + 0x61].unk_0C = sprite_frame_table;
        D_8011ECD0[i + 0x61].unk_10 = D_800C9A14;
        D_8011ECD0[i + 0x61].unk_34 = D_800C9730;
        D_8011ECD0[i + 0x61].unk_14 = D_800C9A54;
        D_8011ECD0[i + 0x61].unk_04 = 8;
        D_8011ECD0[i + 0x61].unk_05 = 1;
        D_8011ECD0[i + 0x61].unk_06 = 8;
        D_8011ECD0[i + 0x61].unk_18 = 0x62;
        D_8011ECD0[i + 0x61].unk_26 = 0x100;
        D_8011ECD0[i + 0x61].unk_2A = 0x100;
        D_8011ECD0[i + 0x61].unk_07 = 0x80;
    }
    left = 0;
    n = 0;
    month = ((GsWord *)&D_800E6280.unk_0F8)->date.month;
    day = ((GsWord *)&D_800E6280.unk_0F8)->date.day;
    if (month == gs->unk_03F) {
        CAL_MARK(0x6E, 0, day - 1);
    }
    if (month == D_800E6280.unk_03F + 1) {
        CAL_MARK(0x6E, 1, day - 1);
    }
    for (i = 0; i < 11; i++) {
        month = D_800E6280.unk_1BC[i].unk_10.date.month;
        day = D_800E6280.unk_1BC[i].unk_10.date.day;
        if (D_800E6280.unk_1BC[i].unk_0C.f.b6) {
            if (month == gs->unk_03F) {
                CAL_MARK(0x6D, 0, day - 1);
            }
            if (month == D_800E6280.unk_03F + 1 || D_800E6280.unk_03F + 1 == month + 12) {
                CAL_MARK(0x6D, 1, day - 1);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        month = D_800E6280.unk_54C[i].h[0] >> 12;
        day = D_800E6280.unk_54C[i].h[1] & 0x1F;
        if (((GsEntry54C *)&D_800E6280.unk_54C[i])->active) {
            if (month == gs->unk_03F) {
                CAL_MARK(0x62, 0, day - 1);
            }
            if (month == D_800E6280.unk_03F % 12 + 1) {
                CAL_MARK(0x62, 1, day - 1);
            }
        }
    }
    if ((D_800E6280.unk_0F4.h >> 12) >= 6 && (D_800E6280.unk_0F4.h >> 12) < 11) {
        day0 = 0x15 - D_800E6280.unk_044[0].unk_20[0];
        day1 = 0x15 - D_800E6280.unk_044[1].unk_20[0];
        CAL_MARK(0x6F, 0, day0 - 1);
        if (D_800E6280.unk_03E != 0x62 || D_800E6280.unk_03F != 2) {
            CAL_MARK(0x6F, 1, day1 - 1);
        }
    }
    for (i = 0; i < gs->unk_044[0].unk_00[0]; i++) {
        if (D_800E6280.unk_044[0].unk_20[i + 1] != 0 && DAY_KIND(0) != 0xFF) {
            if (DAY_KIND(0) == 0x6F) {
                if (D_800E6280.unk_03F == 8 && (D_800E6280.unk_0F4.h >> 12) < 11) {
                    CAL_MARK(DAY_KIND(0), 0, i);
                }
            } else {
                CAL_MARK(DAY_KIND(0), 0, i);
            }
            if (DAY_KIND(0) == 0x66 && D_800E6280.unk_03F != 2) {
                kind = DAY_KIND(0);
                left = 6;
            } else if (DAY_KIND(0) == 0x6C) {
                kind = DAY_KIND(0);
                left = 6;
            } else if (DAY_KIND(0) == 0x67) {
                kind = DAY_KIND(0);
                left = 7;
            } else if (DAY_KIND(0) == 0x6F && D_800E6280.unk_03F == 8 &&
                       (D_800E6280.unk_0F4.h >> 12) < 11) {
                kind = DAY_KIND(0);
                left = 6;
            }
        } else if (left > 0) {
            CAL_MARK(kind, 0, i);
        }
        left--;
        if (n + 0x61 >= 0xA0) {
            return;
        }
    }
    left = 0;
    for (i = 0; i < gs->unk_044[1].unk_00[0]; i++) {
        if (D_800E6280.unk_044[1].unk_20[i + 1] != 0 && DAY_KIND(1) != 0xFF) {
            if (DAY_KIND(1) == 0x6F) {
                if (D_800E6280.unk_03F == 7 && (D_800E6280.unk_0F4.h >> 12) < 11) {
                    CAL_MARK(DAY_KIND(1), 1, i);
                }
            } else {
                CAL_MARK(DAY_KIND(1), 1, i);
            }
            if (DAY_KIND(1) == 0x66 && D_800E6280.unk_03F != 1) {
                kind = DAY_KIND(1);
                left = 6;
            } else if (DAY_KIND(1) == 0x6C) {
                kind = DAY_KIND(1);
                left = 6;
            } else if (DAY_KIND(1) == 0x67) {
                kind = DAY_KIND(1);
                left = 7;
            } else if (DAY_KIND(1) == 0x6F && D_800E6280.unk_03F == 7 &&
                       (D_800E6280.unk_0F4.h >> 12) < 11) {
                kind = DAY_KIND(1);
                left = 6;
            }
        } else if (left > 0) {
            CAL_MARK(kind, 1, i);
        }
        left--;
        if (n + 0x61 >= 0xA0) {
            return;
        }
    }
    for (i = 0; i < 0x100; i++) {
        year = D_800E6280.unk_75E[i].unk_04;
        month = D_800E6280.unk_75E[i].unk_05;
        day = D_800E6280.unk_75E[i].unk_06;
        if (year == D_800E6280.unk_03E && month == D_800E6280.unk_03F) {
            if (D_800E6280.unk_75E[i].unk_00 == 0xC) {
                if (D_800E7D10.flags[12] != 0 || D_800E6280.unk_53C[12] != 0) {
                    CAL_MARK(0x62, 0, day - 1);
                }
            } else {
                CAL_MARK(0x62, 0, day - 1);
            }
        }
        if (n + 0x61 >= 0xA0) {
            return;
        }
        if (D_800E6280.unk_75E[i].unk_04 == 0) {
            return;
        }
    }
}

#undef CAL_MARK
#undef DAY_KIND