#include "common.h"
#include "ovl/TEL.h"
#include "game.h"

/* Display record n of the D_8011ECD0 pool. */
#define REC(n) (((DisplayActor44 *)D_8011ECD0)[n])

/* Frame and position of display record n, one source line (as1 schedules the three stores by line). */
#define SET_REC(n, frame, px, py) (REC(n).unk_18 = (frame), REC(n).unk_26 = (px), REC(n).unk_2A = (py))

/* The packed flag word at GameState +0xF64 of state gs, as single bits. */
#define FLAGS_F64(gs) ((FlagsF64 *)(gs)->unk_F64)

void func_8013CCD0(void) {
    DisplayActor44 *rec;
    s32 i;
    s32 idx;
    s32 x;
    s32 y;
    s32 stars;
    u8 rank[16];
    s16 points[16];

    for (i = 0; i < 0x22; i++) {
        func_80048F64(i + 0x60);
        rec = &REC(i + 0x60);
        rec->unk_01 = 4;
        rec->unk_02 = 0x40;
        rec->unk_38 = 0x40000000;
        rec->unk_03 = 0x84;
        rec->unk_0C = sprite_frame_table;
        rec->unk_10 = D_800C9A14;
        rec->unk_34 = D_800C9730;
        rec->unk_14 = D_800C9A54;
        rec->unk_04 = 8;
        rec->unk_05 = 1;
        rec->unk_26 = 0x200;
    }
    D_80120668 = 0x42;
    D_80120676 = -0x10;
    D_8012067A = -0x1B;
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, "女の子からの評価", 0);
    idx = 0x61;
    func_80053418(rank, points);
    for (i = 0; i != 11; i++) {
        D_800E6280.unk_1BC[0xF].unk_14[i] = 0;
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            x = (i / 6) * 0x82;
            y = (i % 6) * 0x10;
            set_kanji_string(x - 0x82, y - 0x48, 3, D_800B3538[i], 0);
            SET_REC(idx, 0x61, x - 0x32, y - 0x44);
            idx++;
            if (points[i] >= 0x1F4) {
                stars = 0;
            } else if (rank[i] < 6) {
                stars = 6;
            } else if (rank[i] < 0xB) {
                stars = 5;
            } else if (rank[i] < 0x15) {
                stars = 4;
            } else if (rank[i] < 0x29) {
                stars = 3;
            } else if (rank[i] < 0x3D) {
                stars = 2;
            } else {
                stars = 1;
            }
            SET_REC(idx, stars + 0x59, x - 0x22, y - 0x44);
            idx++;
            if (D_800E6280.unk_1BC[i].unk_0A >= 0x3C) {
                /* Store, then test the field (not the assignment): the value stays a ugen temporary and ugen
                 * forwards the stored byte with andi 0xff. T-19027 */
                REC(idx).unk_06 = -(func_800AE0C0(D_800E6280.unk_1BC[i].unk_0A - 0x3C) / 10) * 2 + 8;
                if (REC(idx).unk_06 == 0) {
                    REC(idx).unk_06 = 1;
                }
                SET_REC(idx, 0x60, x - 0x12, y - 0x44);
                idx++;
            }
            D_800E6280.unk_1BC[0xF].unk_14[i] = stars + 1;
        }
    }
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, "女の子からの評価", 0);
}

void func_8013D03C(s32 arg0, s32 arg1) {
    /* FAKE: unused 32-byte local reproduces the frame (slots above n); real use unknown. T-24045 */
    u8 pad[32];
    s32 n;
    u8 buf[32];
    s32 i;
    s32 j;
    u32 day;
    s32 v;
    u8 *notes;
    TelSignDays days;
    Rec38 *p;
    /* FAKE: volatile copy of the record base; the original reloads the spilled base at every later use. T-24045 */
    GameState *volatile q;

    if (arg0 == 0xB) {
        n = 0xC;
    } else {
        n = arg0;
    }
    p = &D_800E6280.unk_1BC[n];
    p->unk_0C.f.b3 = 1;
    p->unk_0C.f.b6 = 1;
    p->unk_0C.f.b2 = 1;
    p->unk_0C.f.b7 = 1;
    D_800E6280.unk_1A3C = 0x801B01B0;
    D_800E6280.unk_1A4C = 0x801B01C0;
    D_800E6280.unk_1A5C = 0;
    D_800E6280.unk_1A6C = 0x801B0200;
    q = (GameState *)((u8 *)p - 0x1BC);
    for (i = 0; i < 0x10; i++) {
        load_palette(D_800E6280.unk_1A6C, (i + 0x10) & 0xFF, 1, 2, i);
    }
    func_80048E78();
    for (i = 0; i < 8; i++) {
        func_80048F64(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).unk_02 = 0x40;
        REC(i + 0x60).unk_38 = 0x40000000;
        REC(i + 0x60).unk_03 = 0x84;
        REC(i + 0x60).unk_0C = sprite_frame_table;
        REC(i + 0x60).unk_10 = D_800C9A14;
        REC(i + 0x60).unk_34 = D_800C9730;
        REC(i + 0x60).unk_14 = D_800C9A54;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 1;
    }
    SET_REC(0x60, D_800B3778[n][0] + 0x4E, 0x27, -0x5A);
    SET_REC(0x61, D_800B3778[n][1] + 0x4E, 0x31, -0x5A);
    SET_REC(0x62, D_800B3778[n][2] + 0x4E, 0x3B, -0x5A);
    SET_REC(0x63, D_800B3778[n][3] + 0x4E, 0x4C, -0x5A);
    SET_REC(0x64, D_800B3778[n][4] + 0x4E, 0x56, -0x5A);
    SET_REC(0x65, D_800B3778[n][5] + 0x4E, 0x60, -0x5A);
    SET_REC(0x66, D_800B3778[n][6] + 0x4E, 0x6A, -0x5A);
    SET_REC(0x67, 0x41, -0x10, -0x1B);
    for (i = 8; i != 9; i++) {
        func_80048F64(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).unk_02 = 0x40;
        REC(i + 0x60).unk_38 = 0x01000000;
        REC(i + 0x60).unk_03 = 0x84;
        REC(i + 0x60).unk_0C = (u8 *)D_800E6280.unk_1A4C;
        REC(i + 0x60).unk_10 = (u8 *)D_800E6280.unk_1A5C;
        REC(i + 0x60).unk_34 = (u8 *)D_800E6280.unk_1A3C;
        REC(i + 0x60).unk_14 = 0;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 0x80;
    }
    SET_REC(0x68, arg0, 0x40, -0x28);
    if (arg0 == 0xB) {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)D_800B3538[12], 0);
    } else {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)D_800B3538[arg0], 0);
    }
    set_kanji_string(-0x20, -0x62, 5, "電話番号", 0);
    set_kanji_string(-0x8A, -0x4E, 4, "誕生日", 0);
    strcpy(buf, D_800B32CC[q->unk_1BC[0].unk_10.date.month - 1]);
    strcat(buf, "月");
    day = q->unk_1BC[0].unk_10.date.day;
    if (day >= 10) {
        strcat(buf, D_800B3288[(s32)day / 10]);
    } else {
        strcat(buf, D_800B3644);
    }
    strcat(buf, D_800B3288[(s32)q->unk_1BC[0].unk_10.date.day % 10]);
    strcat(buf, "日");
    set_kanji_string(-0x58, -0x4E, 3, buf, 0);
    set_kanji_string(-0x8A, -0x1E, 4, "ＢＷＨ", 0);
    if (D_800E6280.unk_042 >= 0x2DA) {
        j = 2;
    } else if (D_800E6280.unk_042 >= 0x16D) {
        j = 1;
    } else {
        j = 0;
    }
    set_kanji_string(-0x58, -0x1E, 3, (u8 *)0x801B2334 + (arg0 * 3 + j) * 0x12, 0);
    set_kanji_string(-0x8A, -0x3E, 4, "星座", 0);
    days = D_80146FD0;
    /* FAKE: remove the local pointer's volatile read view for this pair only;
     * the original shares one birthday word across both field extracts. T-24045 */
    v = (*(GameState **)&q)->unk_1BC[0].unk_10.date.month - 1;
    if (days.w[v] < (*(GameState **)&q)->unk_1BC[0].unk_10.date.day) {
        v++;
    }
    set_kanji_string(-0x58, -0x3E, 3, (u8 *)0x801B22BC + (v % 12) * 10, 0);
    set_kanji_string(-0x8A, -0x2E, 4, "血液型", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(D_800E6280.unk_1BC[12].unk_10.w << 0x15)) >> 0x1E) * 8, 0);
    } else {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(q->unk_1BC[0].unk_10.w << 0x15)) >> 0x1E) * 8, 0);
    }
    set_kanji_string(-0x8A, -0xE, 4, "クラブ", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0xE, 3, "私設部", 0);
    } else {
        set_kanji_string(-0x58, -0xE, 3, (u8 *)0x801B2200 + (q->unk_1BC[0].unk_0C.b[2] >> 4) * 12, 0);
    }
    set_kanji_string(-0x8A, 2, 4, "備考", 0);
    notes = (u8 *)0x801B25F4 + ((arg0 * 6 + j * 2) << 5);
    set_kanji_string(-0x58, 2, 3, notes, 0);
    set_kanji_string(-0x58, 0x12, 3, notes + 0x20, 0);
}

/* Date-spot menu (two pages): fills the x, y, availability and step rows of the eleven entries
 * for menu_set and draws the names of the available ones. arg0 selects the page, arg1..arg3 are
 * the year, month and day of the date. The spot flags, the month and year and the packed-flag
 * tests of page one and of the page-two month ladder are read through g; the other packed-flag
 * tests name D_800E6280 itself. The two spellings give uopt different base-register and
 * promotion choices, and only this split reproduces the original (measured in T-25133).
 */
void func_8013D88C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    GameState *g;
    s16 x[11];
    s16 y[11];
    s16 avail[11];
    s16 step[11];

    g = &D_800E6280;
    for (i = 0; i < 10; i++) {
        avail[i] = 0;
    }
    /* FAKE: one source line, so as1 stores the four row-10 entries in the original order. T-25133 */
    y[10] = -0x54; avail[10] = 0x64; step[10] = 0x10; x[10] = 0;
    if (arg0 == 0) {
        set_kanji_string(-0x80, -0x52, 3, "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, "次のページへ", 0);
        for (i = 0; i < 10; i++) {
            if (g->unk_120[i] & 0x7F) {
                avail[i] = 0x74;
            } else {
                avail[i] = 0;
                switch (i) {
                case 0:
                case 2:
                case 4:
                case 8:
                case 9:
                    break;
                case 1:
                    if (FLAGS_F64(g)->b1 == 1 && arg1 == 0x5F && arg2 == 6) {
                        avail[i] = 0x74;
                    }
                    break;
                case 3:
                    if (FLAGS_F64(g)->b3 == 1 && arg1 == 0x5F && arg2 == 12) {
                        avail[i] = 0x74;
                    }
                    break;
                case 5:
                    if (FLAGS_F64(g)->b5 == 1 && arg1 == 0x60 && arg2 == 6) {
                        avail[i] = 0x74;
                    }
                    break;
                case 6:
                    if (FLAGS_F64(g)->b7 == 1 && arg1 == 0x60 && arg2 == 12) {
                        avail[i] = 0x74;
                    }
                    break;
                case 7:
                    if (FLAGS_F64(g)->b2 == 1 && arg1 == 0x5F && arg2 == 9) {
                        avail[i] = 0x74;
                    }
                    break;
                }
            }
            x[i] = (1 - i / 5) * -0x88;
            y[i] = (i % 5) * 0x10 - 0x38;
            step[i] = 0x10;
            if (avail[i] != 0) {
                set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[i], 0);
            }
        }
    } else {
        set_kanji_string(-0x80, -0x52, 3, "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, "前のページへ", 0);
        for (i = 0; i < 6; i++) {
            if (g->unk_120[i + 10] & 0x7F) {
                if (i < 3) {
                    avail[i] = 0x74;
                } else if (i == 3) {
                    /* The empty arm keeps the original test order and block layout (T-25133). */
                    if (arg2 == 12 || arg2 == 2 || arg2 == 0 || arg2 == 14) {
                    } else {
                        if (arg2 == g->unk_03F) {
                            if (FLAGS_F64(g)->b14 == 1) {
                                avail[i] = 0x74;
                            }
                        } else if (g->unk_03F % 3U == 2) {
                            if (FLAGS_F64(g)->b13 == 1) {
                                avail[i] = 0x74;
                            }
                        } else if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    }
                } else if (i == 4) {
                    if (arg2 == g->unk_03F) {
                        if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (g->unk_03F % 3U == 2) {
                        if (FLAGS_F64(g)->b13 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (FLAGS_F64(g)->b14 == 1) {
                        avail[i] = 0x74;
                    }
                } else if (i == 5) {
                    if (arg2 == g->unk_03F) {
                        if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (g->unk_03F % 3U == 2) {
                        if (FLAGS_F64(g)->b13 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (FLAGS_F64(g)->b14 == 1) {
                        avail[i] = 0x74;
                    }
                }
            } else {
                switch (i) {
                case 2:
                case 5:
                    break;
                case 0:
                    if (FLAGS_F64(&D_800E6280)->b6 == 1 && arg1 == 0x60 && arg2 >= 9) {
                        avail[i] = 0x74;
                    }
                    break;
                case 1:
                    if (FLAGS_F64(&D_800E6280)->b8 == 1 && arg1 == 0x61 && arg2 >= 3) {
                        avail[i] = 0x74;
                    }
                    break;
                case 3:
                    if (FLAGS_F64(&D_800E6280)->b13 == 1 && g->unk_03F == 2 && arg2 == 3 && g->unk_03E == 0x60) {
                        avail[i] = 0x74;
                    }
                    break;
                case 4:
                    if (FLAGS_F64(&D_800E6280)->b13 == 1 && arg1 == 0x5F && arg2 == 6 && g->unk_03F == 5) {
                        avail[i] = 0x74;
                    }
                    break;
                }
            }
            x[i] = (1 - i / 5) * -0x88;
            y[i] = (i % 5) * 0x10 - 0x38;
            step[i] = 0x10;
            if (avail[i] != 0) {
                /* FAKE: 3U makes the colour share the unsigned 3 of the month test (andi of t1). T-25133 */
                set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3U, D_800B342C[i + 10], 0);
            }
        }
        for (i = 6; i < 10; i++) {
            if (i == 6) {
                if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[16], 0);
                    avail[i] = 0x74;
                } else if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[19], 0);
                    avail[i] = 0x74;
                } else {
                    avail[i] = 0;
                }
            }
            if (i == 7) {
                if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[20], 0);
                    avail[i] = 0x74;
                } else if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[17], 0);
                    avail[i] = 0x74;
                } else {
                    avail[i] = 0;
                }
            }
            if (i == 8) {
                avail[i] = 0;
            }
            if (i == 9) {
                if (FLAGS_F64(&D_800E6280)->b15 == 1 && arg2 == 8 && (arg3 + 6) / 7 == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, D_800B342C[18], 0);
                    avail[i] = 0x74;
                } else {
                    avail[i] = 0;
                }
            }
            x[i] = (1 - i / 5) * -0x88;
            y[i] = (i % 5) * 0x10 - 0x38;
            step[i] = 0x10;
        }
    }
    menu_set(0, 0xB, x, y, avail, step);
    D_800E6280.unk_1093 = 0xA;
    func_80048F64(0x60);
    REC(96).unk_01 = 4;
    REC(96).unk_02 = 0x40;
    REC(96).unk_38 = 0x40000000;
    REC(96).unk_03 = 0x84;
    REC(96).unk_0C = sprite_frame_table;
    REC(96).unk_10 = D_800C9A14;
    REC(96).unk_34 = D_800C9730;
    REC(96).unk_04 = 8;
    REC(96).unk_05 = 1;
    REC(96).unk_18 = 0x40;
    REC(96).unk_26 = -0x10;
    REC(96).unk_2A = -0x13;
}
