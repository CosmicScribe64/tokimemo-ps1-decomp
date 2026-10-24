/* profile_and_place_availability.c: src/ovl/TEL/8013CCD0.c:86: girl appraisal sprites, girl profile details and month/place availability table construction; body 101: if (arg0 == 0xB) {; 102: n = 0xC;. Original object 0x8013CCD0-0x8013E430 (config/objects/TEL.txt). */
#include "common.h"
#include "ovl/TEL.h"
#include "game.h"

/* Display record n of the actor_pool pool. */
#define REC(n) (((Actor *)actor_pool)[n])

/* Frame and position of display record n, one source line (as1 schedules the three stores by line). */
#define SET_REC(n, frame, px, py) (REC(n).unk_18 = (frame), REC(n).x = (px), REC(n).y = (py))

/* The packed flag word at GameState +0xF64 of state gs, as single bits. */
#define FLAGS_F64(gs) ((QuarterFlagBits *)(gs)->quarter_flags)

void func_8013CCD0(void) {
    Actor *rec;
    s32 i;
    s32 idx;
    s32 x;
    s32 y;
    s32 stars;
    u8 rank[16];
    s16 points[16];

    for (i = 0; i < 0x22; i++) {
        actor_clear(i + 0x60);
        rec = &REC(i + 0x60);
        rec->unk_01 = 4;
        rec->flags = 0x40;
        rec->quad_flags = 0x40000000;
        rec->draw_flags = 0x84;
        rec->frame_table = sprite_frame_table;
        rec->sequence_table = D_800C9A14;
        rec->tpage_table = D_800C9730;
        rec->unk_14 = D_800C9A54;
        rec->unk_04 = 8;
        rec->unk_05 = 1;
        rec->x = 0x200;
    }
    D_80120668 = 0x42;
    D_80120676 = -0x10;
    D_8012067A = -0x1B;
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, "女の子からの評価", 0);
    idx = 0x61;
    func_80053418(rank, points);
    for (i = 0; i != 11; i++) {
        game_state.girls[0xF].scene_counters[i] = 0;
        if (game_state.girls[i].flags.f.met) {
            x = (i / 6) * 0x82;
            y = (i % 6) * 0x10;
            set_kanji_string(x - 0x82, y - 0x48, 3, girl_full_name_table[i], 0);
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
            if (game_state.girls[i].syoushin >= 0x3C) {
                /* Store, then test the field (not the assignment): the value stays a ugen temporary and ugen
                 * forwards the stored byte with andi 0xff. T-19027 */
                REC(idx).blink_period = -(abs(game_state.girls[i].syoushin - 0x3C) / 10) * 2 + 8;
                if (REC(idx).blink_period == 0) {
                    REC(idx).blink_period = 1;
                }
                SET_REC(idx, 0x60, x - 0x12, y - 0x44);
                idx++;
            }
            game_state.girls[0xF].scene_counters[i] = stars + 1;
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
    GirlRecord *p;
    /* FAKE: volatile copy of the record base; the original reloads the spilled base at every later use. T-24045 */
    GameState *volatile q;

    if (arg0 == 0xB) {
        n = 0xC;
    } else {
        n = arg0;
    }
    p = &game_state.girls[n];
    p->flags.f.b3 = 1;
    p->flags.f.b6 = 1;
    p->flags.f.b2 = 1;
    p->flags.f.b7 = 1;
    game_state.ovl_tpage_table = 0x801B01B0;
    game_state.ovl_frame_table = 0x801B01C0;
    game_state.ovl_sequence_table = 0;
    game_state.ovl_palette_a = 0x801B0200;
    q = (GameState *)((u8 *)p - 0x1BC);
    for (i = 0; i < 0x10; i++) {
        load_palette(game_state.ovl_palette_a, (i + 0x10) & 0xFF, 1, 2, i);
    }
    actor_clear_upper();
    for (i = 0; i < 8; i++) {
        actor_clear(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).flags = 0x40;
        REC(i + 0x60).quad_flags = 0x40000000;
        REC(i + 0x60).draw_flags = 0x84;
        REC(i + 0x60).frame_table = sprite_frame_table;
        REC(i + 0x60).sequence_table = D_800C9A14;
        REC(i + 0x60).tpage_table = D_800C9730;
        REC(i + 0x60).unk_14 = D_800C9A54;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 1;
    }
    SET_REC(0x60, girl_phone_digits[n][0] + 0x4E, 0x27, -0x5A);
    SET_REC(0x61, girl_phone_digits[n][1] + 0x4E, 0x31, -0x5A);
    SET_REC(0x62, girl_phone_digits[n][2] + 0x4E, 0x3B, -0x5A);
    SET_REC(0x63, girl_phone_digits[n][3] + 0x4E, 0x4C, -0x5A);
    SET_REC(0x64, girl_phone_digits[n][4] + 0x4E, 0x56, -0x5A);
    SET_REC(0x65, girl_phone_digits[n][5] + 0x4E, 0x60, -0x5A);
    SET_REC(0x66, girl_phone_digits[n][6] + 0x4E, 0x6A, -0x5A);
    SET_REC(0x67, 0x41, -0x10, -0x1B);
    for (i = 8; i != 9; i++) {
        actor_clear(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).flags = 0x40;
        REC(i + 0x60).quad_flags = 0x01000000;
        REC(i + 0x60).draw_flags = 0x84;
        REC(i + 0x60).frame_table = (u8 *)game_state.ovl_frame_table;
        REC(i + 0x60).sequence_table = (u8 *)game_state.ovl_sequence_table;
        REC(i + 0x60).tpage_table = (u8 *)game_state.ovl_tpage_table;
        REC(i + 0x60).unk_14 = 0;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 0x80;
    }
    SET_REC(0x68, arg0, 0x40, -0x28);
    if (arg0 == 0xB) {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)girl_full_name_table[12], 0);
    } else {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)girl_full_name_table[arg0], 0);
    }
    set_kanji_string(-0x20, -0x62, 5, "電話番号", 0);
    set_kanji_string(-0x8A, -0x4E, 4, "誕生日", 0);
    strcpy(buf, D_800B32CC[q->girls[0].birthday.date.month - 1]);
    strcat(buf, "月");
    day = q->girls[0].birthday.date.day;
    if (day >= 10) {
        strcat(buf, D_800B3288[(s32)day / 10]);
    } else {
        strcat(buf, D_800B3644);
    }
    strcat(buf, D_800B3288[(s32)q->girls[0].birthday.date.day % 10]);
    strcat(buf, "日");
    set_kanji_string(-0x58, -0x4E, 3, buf, 0);
    set_kanji_string(-0x8A, -0x1E, 4, "ＢＷＨ", 0);
    if (game_state.day_count >= 0x2DA) {
        j = 2;
    } else if (game_state.day_count >= 0x16D) {
        j = 1;
    } else {
        j = 0;
    }
    set_kanji_string(-0x58, -0x1E, 3, (u8 *)0x801B2334 + (arg0 * 3 + j) * 0x12, 0);
    set_kanji_string(-0x8A, -0x3E, 4, "星座", 0);
    days = D_80146FD0;
    /* FAKE: remove the local pointer's volatile read view for this pair only;
     * the original shares one birthday word across both field extracts. T-24045 */
    v = (*(GameState **)&q)->girls[0].birthday.date.month - 1;
    if (days.w[v] < (*(GameState **)&q)->girls[0].birthday.date.day) {
        v++;
    }
    set_kanji_string(-0x58, -0x3E, 3, (u8 *)0x801B22BC + (v % 12) * 10, 0);
    set_kanji_string(-0x8A, -0x2E, 4, "血液型", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(game_state.girls[12].birthday.w << 0x15)) >> 0x1E) * 8, 0);
    } else {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(q->girls[0].birthday.w << 0x15)) >> 0x1E) * 8, 0);
    }
    set_kanji_string(-0x8A, -0xE, 4, "クラブ", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0xE, 3, "私設部", 0);
    } else {
        set_kanji_string(-0x58, -0xE, 3, (u8 *)0x801B2200 + (q->girls[0].flags.b[2] >> 4) * 12, 0);
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
 * tests name game_state itself. The two spellings give uopt different base-register and
 * promotion choices, and only this split reproduces the original (measured in T-25133).
 */
void func_8013D88C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    GameState *g;
    s16 x[11];
    s16 y[11];
    s16 avail[11];
    s16 step[11];

    /* FAKE: g and game_state name the same state; the mixed spellings (see the header comment) steer the base
     * register and promotion choices of uopt. The original source is unknown. T-25133 */
    g = &game_state;
    for (i = 0; i < 10; i++) {
        avail[i] = 0;
    }
    /* FAKE: one source line, so as1 stores the four row-10 entries in the original order. T-25133 */
    y[10] = -0x54; avail[10] = 0x64; step[10] = 0x10; x[10] = 0;
    if (arg0 == 0) {
        set_kanji_string(-0x80, -0x52, 3, "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, "次のページへ", 0);
        for (i = 0; i < 10; i++) {
            if (g->progress_state[i] & 0x7F) {
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
                set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[i], 0);
            }
        }
    } else {
        set_kanji_string(-0x80, -0x52, 3, "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, "前のページへ", 0);
        for (i = 0; i < 6; i++) {
            if (g->progress_state[i + 10] & 0x7F) {
                if (i < 3) {
                    avail[i] = 0x74;
                } else if (i == 3) {
                    /* FAKE: the empty arm keeps the original test order and block layout; the != and && form
                     * moves the register of the 0x74 constant. T-25133 */
                    if (arg2 == 12 || arg2 == 2 || arg2 == 0 || arg2 == 14) {
                    } else {
                        if (arg2 == g->month) {
                            if (FLAGS_F64(g)->b14 == 1) {
                                avail[i] = 0x74;
                            }
                        } else if (g->month % 3U == 2) {
                            if (FLAGS_F64(g)->b13 == 1) {
                                avail[i] = 0x74;
                            }
                        } else if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    }
                } else if (i == 4) {
                    if (arg2 == g->month) {
                        if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (g->month % 3U == 2) {
                        if (FLAGS_F64(g)->b13 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (FLAGS_F64(g)->b14 == 1) {
                        avail[i] = 0x74;
                    }
                } else if (i == 5) {
                    if (arg2 == g->month) {
                        if (FLAGS_F64(g)->b14 == 1) {
                            avail[i] = 0x74;
                        }
                    } else if (g->month % 3U == 2) {
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
                    if (FLAGS_F64(&game_state)->b6 == 1 && arg1 == 0x60 && arg2 >= 9) {
                        avail[i] = 0x74;
                    }
                    break;
                case 1:
                    if (FLAGS_F64(&game_state)->b8 == 1 && arg1 == 0x61 && arg2 >= 3) {
                        avail[i] = 0x74;
                    }
                    break;
                case 3:
                    if (FLAGS_F64(&game_state)->b13 == 1 && g->month == 2 && arg2 == 3 && g->year == 0x60) {
                        avail[i] = 0x74;
                    }
                    break;
                case 4:
                    if (FLAGS_F64(&game_state)->b13 == 1 && arg1 == 0x5F && arg2 == 6 && g->month == 5) {
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
                set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3U, place_name_table[i + 10], 0);
            }
        }
        for (i = 6; i < 10; i++) {
            if (i == 6) {
                if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[16], 0);
                    avail[i] = 0x74;
                } else if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[19], 0);
                    avail[i] = 0x74;
                } else {
                    avail[i] = 0;
                }
            }
            if (i == 7) {
                if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[20], 0);
                    avail[i] = 0x74;
                } else if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[17], 0);
                    avail[i] = 0x74;
                } else {
                    avail[i] = 0;
                }
            }
            if (i == 8) {
                avail[i] = 0;
            }
            if (i == 9) {
                if (FLAGS_F64(&game_state)->b15 == 1 && arg2 == 8 && (arg3 + 6) / 7 == 1) {
                    set_kanji_string((1 - i / 5) * -0x88, (i % 5) * 0x10 - 0x38, 3, place_name_table[18], 0);
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
    game_state.menu_cursor_0 = 0xA;
    actor_clear(0x60);
    REC(96).unk_01 = 4;
    REC(96).flags = 0x40;
    REC(96).quad_flags = 0x40000000;
    REC(96).draw_flags = 0x84;
    REC(96).frame_table = sprite_frame_table;
    REC(96).sequence_table = D_800C9A14;
    REC(96).tpage_table = D_800C9730;
    REC(96).unk_04 = 8;
    REC(96).unk_05 = 1;
    REC(96).unk_18 = 0x40;
    REC(96).x = -0x10;
    REC(96).y = -0x13;
}
