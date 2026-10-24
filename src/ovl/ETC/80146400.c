#include "common.h"
#include "ovl/ETC.h"

/* FAKE: unused padding array preserves the frame layout. T-10850 */
s32 func_80146400(void)
{
    s32 x;
    s32 pad[3];
    s32 *p;
    s32 a;
    s32 r;
    s32 d;
    switch (D_800E6280.year)
    {
        case 0x5F:
            x = 0;
            break;

        case 0x60:
            x = 3;
            break;

        case 0x61:
            x = 6;
            break;

        default:
            x = 6;
            break;

    }

    if (D_800E6280.month != 2)
    {
        if (D_800E6280.month != 7)
        {
            if (D_800E6280.month == 0xC)
            {
                x += 1;
            }
        }
    }
    else
    {
        x -= 1;
    }
    /* The assignment inside the argument makes IDO load the format string's lui first (T-22029). */
    printf("補正 %d ", *(p = &D_801506F8[x]));
    r = rand() & 7;
    a = *p;
    d = r + a;
    d -= 8;
    if (d > 0)
    {
        return a;
    }
    /* FAKE: zero expression preserves the comparison live range. T-10850 */
    return (d > 0) * 0;
}

s32 func_801464D4(s32 mode) {
    u32 i;
    GameState *g;

    g = &D_800E6280;
    for (i = 0; i < 256; i++) {
        if (g->event_log[i].year == 0) {
            break;
        }
        if (g->year == g->event_log[i].year &&
            g->month == g->event_log[i].month) {
            if (mode == 8 && g->event_log[i].scene == 2 &&
                g->event_log[i].flag == 0x3B) {
                return 9;
            }
            if (mode == 0xB && g->event_log[i].scene == 0xB &&
                g->event_log[i].flag == 0xF) {
                return 1;
            }
            if (g->event_log[i].scene == 0 && g->event_log[i].flag == 0xF) {
                if (mode == 0xF) {
                    return 1;
                }
                if (mode == g->event_log[i].girl) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void func_801465F8(void) {
    s32 *p;
    s32 y;

    switch (D_800E6280.stage) {
    case 0:
        func_80146B40();
        break;
    case 1:
        func_80146714();
        break;
    case 2:
        func_80148078();
        break;
    }
    func_80066C08(1);
    message_window_show();
    hizuke_show();
    parameter_show();
    func_8006BA40();
    func_80065B0C(1);
    if (D_800E6280.weekday == 5) {
        func_8006764C(0);
    } else {
        func_80067870();
    }
    func_800578F4(0);
    p = &D_801230D0;
    for (y = 0x28; y != 0x58; y += 8) {
        func_8004B19C(p[10], 0x40, y);
        func_8004B19C(p[4], 0x50, y);
        p++;
    }
}
void func_80146714(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.weekday) {
    case 0:
        func_80147650();
        return;
    case 1:
        func_801476DC();
        return;
    case 2:
        func_80147768();
        return;
    case 3:
        func_801477F4();
        return;
    case 4:
        func_80147880();
        return;
    case 5:
        func_80147F7C();
        /* fallthrough */
    default:
        return;
    }
}

void func_801467C4(void) {
    s32 var_s0;

    func_8004E58C();
    func_8006612C((s32) "期末テスト");
    func_80041584();
    func_80048E78();
    tpage_buf_clear();
    func_8006509C();
    icon_disp_switch(1);
    func_8006CE84();
    for (var_s0 = 0; var_s0 != 14; var_s0++) {
        icon_can_use_set(var_s0, 0);
    }
    func_800654D0(*(u8 *)&D_800E6280.icon_count);
    func_8006BD6C(1);
    hizuke_init();
    parameter_show_init();
    parameter_show_init();
    if (D_800E6280.weekday == 0) {
        func_80044890(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    } else if (((EtcStateFlagsView *)&D_800E6280)->records[D_800E6280.day_event_index].type != 0x40) {
        func_80044890(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    }
    func_8004284C();
}

void func_801468FC(void) {
    s32 r2;
    s32 r;

    if (D_800E6280.weekday == 0 || *(u8 *) &D_800E6280.clubs[10 + D_800E6280.day_event_index] != 0x40) {
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0x205);
            r = dec_bg_cd_read(0x3FA4, 0);
            set_dec_bri(0);
            if (r != -1) {
                if (r == 0 || r == 1) {
                    dec_bg_show_set(0, r);
                    set_dec_bri(0x80);
                    func_80042940(3);
                }
            } else {
                func_8004284C();
            }
        }
    } else {
        if (func_8004481C() == 0) {
            func_8004500C(1, 0x205);
        }
        r2 = dec_bg_cd_read(0x3FA4, 0);
        if (r2 != -1) {
            if (r2 == 0 || r2 == 1) {
                dec_bg_show_set(0, r2);
                func_800578F4(0);
                set_dec_bri(0x80);
                func_80042940(3);
            }
        } else {
            func_8004284C();
        }
    }
}

void func_80146A38(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(0x3FA4, 0);
        } else {
            set_dec_bri(0x80);
            func_8004284C();
            func_800578F4(0);
        }
    }
}

void func_80146AC0(void) {
    s32 i;

    if (D_800E6280.weekday == 0) {
        for (i = 0; i < 6; i++) {
            ((EtcWork40 *)&D_801230D0)->unk_10[i] = 0;
            ((EtcWork40 *)&D_801230D0)->unk_28[i] = 0;
        }
    }
    func_80042808();
}

s32 func_80146B40(void) {
    switch (D_800E6280.step) {
    case 0:
        func_801467C4();
        break;
    case 1:
        func_801468FC();
        break;
    case 2:
        func_80146A38();
        break;
    case 3:
        func_80146AC0();
        break;
    }
}

s32 func_80146BC4(void) {
    switch (D_800E6280.substep) {
    case 0:
        cd_read_sectors(9U, 0x801E0000, *(s32 *)&D_80150720[-(func_80066A84() * 4)]);
        D_800E6280.substep = 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            addr_init_else_sd(1 - func_80066A84());
            func_8004284C();
        }
        break;
    }
}

void func_80146C68(s32 arg0) {
    func_80048F64(0x60);
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((DisplayActor44 *)D_80120650)->unk_01 = 1;
    ((DisplayActor44 *)D_80120650)->flags = 4;
    ((DisplayActor44 *)D_80120650)->quad_flags = 0x01000000;
    ((DisplayActor44 *)D_80120650)->draw_flags = 0x84;
    *(u8 **)&D_80120650[0xC] = D_80125CA8;
    *(u8 **)&D_80120650[0x10] = D_80125CAC;
    *(u8 **)&D_80120650[0x34] = D_80125CB0;
    ((DisplayActor44 *)D_80120650)->unk_05 = 2;
    ((DisplayActor44 *)D_80120650)->blink_period = 1;
    ((DisplayActor44 *)D_80120650)->unk_43 = 0x10;
    ((DisplayActor44 *)D_80120650)->sequence_slot = arg0;
    ((DisplayActor44 *)D_80120650)->x = -0x50;
    ((DisplayActor44 *)D_80120650)->y = -0x40;
}

s32 func_80146D4C(void) {
    s32 zero; /* FAKE: T-25045, reuses the original T-25007 frame-padding word. */
    s32 score;
    EtcWork40 *w;
    s32 ret;
    s32 main;
    s32 sub;
    s32 bio;
    GameState *g = &D_800E6280;

    main = 0;
    sub = 0;
    switch (g->weekday) {
    case 0:
        main = g->param_liberal_arts.value;
        bio = func_8006C334(0, 0, 1, 0x10);
        break;
    case 1:
        main = g->param_science.value;
        bio = func_8006C334(0, 0, 1, 0x10);
        break;
    case 2:
        main = g->param_science.value * 7 / 10;
        sub = g->param_trivia.value * 3 / 10;
        if (sub >= 0x27) {
            sub = 0x26;
        }
        bio = func_8006C334(0, 1, 1, 0x10);
        break;
    case 3:
        sub = g->param_liberal_arts.value * 3 / 10;
        main = g->param_trivia.value * 7 / 10;
        bio = func_8006C334(0, 0, 1, 0x10);
        if (sub >= 0x27) {
            sub = 0x26;
        }
        break;
    case 4:
        main = g->param_art.value * 8 / 10;
        sub = g->param_trivia.value * 2 / 10;
        bio = func_8006C334(0, 1, 0, 0x10);
        if (sub >= 0x1A) {
            sub = 0x19;
        }
        break;
    default:
        func_80046500();
        break;
    }
    ret = main + sub + bio - 8;
    /* FAKE: T-25007, the nibble view loads the flag byte into its own temporary as the original does */
    if (((GsByteNibbles *)&g->player_flags.b[1])->low & 2) {
        ret /= 2;
    }
    printf("main %d sub %d bio %d ret %d ", main, sub, bio, ret);
    if (ret < 0) {
        ret = 1;
    }
    ret = func_80146400() + ret;
    sub = func_801464D4(0xF);
    ret = (rand() % 10) * sub + ret;
    w = (EtcWork40 *)&D_801230D0;
    if (ret < 0x80) {
        w->unk_28[g->weekday] = ret * 100 / 128;
    } else {
        w->unk_28[g->weekday] = 100 - (rand() & 3);
    }
    printf("今日の成績 %d\t", w->unk_28[g->weekday]);
    score = w->unk_28[g->weekday] + w->unk_28[5];
    w->unk_28[5] = score;
    /* FAKE: T-25045, the late-folded zero and two identities spend one ugen
     * temporary before as1 removes them, preserving the final index registers. */
    zero = g->weekday * 0;
    printf("合計点 %d\n", (score + zero) | zero);
    ret = w->unk_28[g->weekday];
    if (ret < 0x28) {
        ret = 0;
    } else if (ret < 0x3C) {
        ret = 1;
    } else if (ret < 0x50) {
        ret = 2;
    } else if (ret < 0x5F) {
        ret = 3;
    } else {
        ret = 4;
    }
    if (ret == 0) {
        D_801230F4 += 1;
        /* FAKE: T-25007, an extra read of unk_041 raises the priority of its promoted range so every
         * read sits in a3 as in the original (found by the permuter) */
        if (g->weekday) {
        }
    }
    w->unk_10[g->weekday] = ret;
    return ret;
}

void func_8014716C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    gnsx(D_800E6280.player_family_name);
    k_disp_start(2);
    sndisp("第一日目の試験科目は 語学だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_801471F4(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    gnsx(D_800E6280.player_family_name);
    k_disp_start(2);
    sndisp("第二日目の試験科目は 数学だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_8014727C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    gnsx(D_800E6280.player_family_name);
    k_disp_start(2);
    sndisp("第三日目の試験科目は 理科だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_80147304(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    gnsx(D_800E6280.player_family_name);
    k_disp_start(2);
    sndisp("第四日目の試験科目は 社会だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_8014738C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    gnsx(D_800E6280.player_family_name);
    k_disp_start(2);
    sndisp("第五日目の試験科目は 美術だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_80147414(void) {
    s16 *mode; /* pointer local: the switch reads the flag through it, as the permuter-found form that matches (T-6050) */
    if (D_800E6280.step_timer.w == 0x20) {
        D_80120652 |= 1;
    }
    if (!(D_80120652 & 1) && (D_80120668 != 0)) {
        if (D_80120658 == 0) {
            mode = &D_80120666;
            switch ((*mode) & 1) {
            case 0:
                func_80044750(0x601);
                break;
            case 1:
                func_80044750(0x602);
                break;
            }
        }
        if (D_80120658++ >= 0x11) {
            func_8004284C();
        }
    }
    k_disp_inc();
}

void func_801474FC(void) {
    if (D_800E6280.substep == 0) {
        k_reset(1);
        gnsx(D_800E6280.player_family_name);
        switch (D_801230E0[D_800E6280.weekday]) {
        case 0:
            sndisp("もうだめだー。）", 0, 0x1F);
            break;
        case 1:
            sndisp("ちょっとだめだな。）", 0, 0x1F);
            break;
        case 2:
            sndisp("まあまあのできだな。）", 0, 0x1F);
            break;
        case 3:
            sndisp("いいできだ。）", 0, 0x1F);
            break;
        case 4:
            sndisp("バッチリだ。）", 0, 0x1F);
            break;
        }
        D_800E6280.substep += 1;
    } else {
        k_disp_inc();
        if (check_end_k() != 0) {
            func_8004284C();
        }
    }
}

void func_80147630(void) {
    func_80042808();
}

void func_80147650(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014716C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_801476DC(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_801471F4();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_80147768(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014727C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_801477F4(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_80147304();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_80147880(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014738C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_801480D0(void);
void func_80148F34(void); /* defined later in this file */

/* T-25054: results scene and the fourteen result display records.
 * FAKE: retain the legacy non-void fallthrough to keep the switch copy in v0;
 * the only caller ignores the result. A void definition removes that copy. */
s32 func_8014790C(void) {
    /* FAKE: the 0x10C size only reproduces the 0x188 frame and the buffer at 0x7C (original size unknown). T-22029 */
    u8 buf[0x10C];
    s32 i;
    u8 *r;
    s16 x;
    s16 y;
    s32 v;

    switch (*(u8 *)&D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_8006612C("結果発表 ");
        gnsx(D_800E6280.player_family_name);
        sndisp("今日は、試験結果の 発表の日だ。）", 0, 0x1F);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        func_8004500C(1, 0x202);
        hizuke_init();
        func_80048E78();
        D_800E6280.substep += 1;
        cd_read_sectors(0xD, 0x801E0000, 0x9CA8);
        break;
    case 1:
        if ((u32) D_800E6280.step_timer.w >= 0x101) {
            D_800E6280.substep += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004E58C();
            icon_disp_switch(0);
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            func_80048F64(0x60);
            ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 4;
            ((DisplayActor44 *)D_8011ECD0)[96].flags = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[96].quad_flags = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].draw_flags = 0xA4;
            ((DisplayActor44 *)D_8011ECD0)[96].frame_table = (u8 *) 0x801E64A0;
            ((DisplayActor44 *)D_8011ECD0)[96].sequence_table = (u8 *) 0x801E6520;
            ((DisplayActor44 *)D_8011ECD0)[96].tpage_table = (u8 *) 0x801E6494;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[96].x = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].y = -0x28;
            ((DisplayActor44 *)D_8011ECD0)[96].brightness = 0x80;
            func_80148E98();
            func_80148EE8();
            func_801480D0();
            /* FAKE: T-25054, the always-false unsigned comparison folds after
             * uopt, keeping the walked base separate from the indexed base.
             * Keep i's initialization and the following do on one line for
             * the original address schedule. */
            do { r = D_8011ECD0; } while ((u32)D_800E6280.weekday < 0); i = 0; do {
                x = D_80150630[i];
                y = D_8015064C[i];
                v = D_80150684[i];
                ((DisplayActor44 *)r)[0x61].unk_01 = 3;
                ((DisplayActor44 *)r)[0x61].flags = 0x40;
                ((DisplayActor44 *)r)[0x61].quad_flags = 0;
                ((DisplayActor44 *)r)[0x61].draw_flags = 0x24;
                ((DisplayActor44 *)r)[0x61].frame_table = (u8 *)0x801E64A0;
                ((DisplayActor44 *)r)[0x61].sequence_table = (u8 *)0x801E6520;
                ((DisplayActor44 *)r)[0x61].tpage_table = (u8 *)0x801E6494;
                ((DisplayActor44 *)r)[0x61].unk_18 = 0x1F;
                ((DisplayActor44 *)r)[0x61].unk_05 = 1;
                ((DisplayActor44 *)r)[0x61].unk_04 = 8;
                ((DisplayActor44 *)r)[0x61].brightness = 0x80;
                ((DisplayActor44 *)r)[0x61].x = x;
                ((DisplayActor44 *)r)[0x61].y = y;
                if (v == 0xD) {
                    strncpy((char *) buf, (char *) D_800E6280.player_family_name, 8);
                    strncat((char *) buf, (char *) D_800E6280.player_given_name, 8);
                    set_kanji_string(D_80150630[i] + 0x18, D_8015064C[i], 3, buf, 0);
                    printf("%s\n", D_800E6280.player_full_name);
                } else if (v == 0xA) {
                    set_kanji_string(x + 0x18, y, 3, D_800B3538[13], 0);
                } else if (v == 0xB) {
                    if (D_800E6280.girls[11].flags.f.story_stage != 0) {
                        set_kanji_string(x + 0x18, y, 3, D_800B3538[11], 0);
                    } else {
                        /* FAKE: T-25054, this late-folded zero prevents uopt
                         * from distributing 0x61 into the field displacement.
                         * The same identity is used in the final else arm. */
                        ((DisplayActor44 *) D_8011ECD0)[(i + 0x61) | ((u32)i < 0)].draw_flags = 0xA4;
                    }
                } else if (D_800E6280.girls[v].flags.f.met) {
                    set_kanji_string(x + 0x18, y, 3, D_800B3538[v], 0);
                } else {
                    ((DisplayActor44 *) D_8011ECD0)[(i + 0x61) | ((u32)i < 0)].draw_flags = 0xA4;
                }
                r += 0x44;
                i += 1;
            } while (i != 0xE);
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            func_80148F34();
            func_8004284C();
        }
        break;
    }
}

/* Bits 0 and 1 of the low byte of GameState +0xF6 (T-7010). */
typedef struct {
    u8 f0 : 1;
    u8 f1 : 1;
    u8 rest : 6;
} GsBits0F6;

void func_80147D74(void) {
    func_80148BC8();
    D_800E6280.player_flags_hi.b[0] &= 0xFFFD;
    if (D_800E6280.step_timer.u >= 0x79U) {
        gnsx(D_800E6280.player_family_name);
        if (D_801230F4 >= 3) {
            sndisp("３科目以上赤点だった。 明日から、一週間補習だぁ。 勉強しかできないー！）", 0, 0x1F);
            func_8004500C(1, 0x203);
            ((GsBits0F6 *)&D_800E6280.player_flags_hi.b[0])->f0 = 1;
            D_800E6280.month_calendar[0].marker[D_800E6280.day + 1] = 0x17;
            ((GsBits0F6 *)&D_800E6280.player_flags_hi.b[0])->f1 = 1;
        } else if (D_801506F0 < 2) {
            sndisp("やったー、一番だ！ これで試験の帝王だな。）", 0, 0x1F);
            func_8004500C(1, 0x204);
        } else if (D_801506F4 != 0) {
            sndisp("今回の試験は終了した。）", 0, 0x1F);
            ((GsBits0F6 *)&D_800E6280.player_flags_hi.b[0])->f1 = 1;
            func_8004500C(1, 0x200);
        } else {
            sndisp("今回の試験は終了した。）", 0, 0x1F);
            func_8004500C(1, 0x201);
        }
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        func_8006BD6C(0);
        func_8004284C();
    }
    func_80148E98();
    func_80148EE8();
}

void func_80147F0C(void) {
    if (((D_800E6280.pad_pressed & 0x20) && (u32)D_800E6280.step_timer.w >= 0x101) || (D_800E6280.pad_pressed & 0x40)) {
        func_8006BD6C(1);
        func_80042808();
    }
    func_80148E98();
    func_80148EE8();
    func_80148BC8();
}

s32 func_80147F7C(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
    case 0:
        func_8014790C();
        break;
    case 1:
        func_80147D74();
        break;
    case 2:
        func_80147F0C();
        break;
    }
}

void func_80147FFC(void) {
    parameter_show_init();
    parameter_show_init();
    func_80048E78();
    func_8004E58C();
    func_8004284C();
}

void func_8014803C(void) {
    if (D_800E6280.weekday == 5) {
        func_8004500C(0, 0);
    }
    func_80072B5C(0);
}

void func_80148078(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80147FFC();
        break;
    case 1:
        func_8014803C();
        break;
    }
    parameter_show();
}
/* T-25059: generate exam scores, order and ranks, then apply relationship bonuses. */
void func_801480D0(void) {
    /* FAKE: T-25059, scalar order and the unused word below preserve the
     * original 0xD0 frame and the noise/angle/offset/array stack homes. */
    s32 i;
    s32 j;
    s32 changed;
    s32 a;
    s32 noise;
    s32 first_random;
    s32 selected;
    s32 sine;
    s32 offset;
    s32 minimum;
    s32 angle;
    s32 maximum;
    s32 values[14];
    s32 pad;

    srand(D_800E6280.frame_count);
    for (i = 0; i < 13; i++) {
        first_random = rand();
        noise = (first_random % (s32)D_80150738[i]) * (((rand() & 1) * 2) - 1);
        first_random = rand();
        a = (first_random % (s32)D_80150738[i]) * ((rand() & 1) * 2 - 1);
        D_80150668[i] = ((D_80150728[i] + a) * 5) + noise;
        j = func_801464D4(i);
        if (j != 1) {
            if (j == 9) {
                D_80150668[i] += rand() % 250;
            }
        } else {
            D_80150668[i] += rand() % 50;
        }
        if (D_80150668[i] >= 501) {
            D_80150668[i] = 500;
        }
        printf("[%d:%d]\n", i, D_80150668[i]);
    }
    D_80150668[13] = (s16) ((EtcWork40 *)&D_801230D0)->unk_28[5];
    do {
        changed = 0;
        for (j = 0; j < 14; j++) {
            for (i = j + 1; i < 14; i++) {
                if (D_80150668[i] == D_80150668[j]) {
                    changed++;
                    D_80150668[i]--;
                }
            }
        }
    } while (changed != 0);
    for (i = 0; i < 14; i++) {
        values[i] = D_80150668[i];
    }
    for (j = 0; j < 14; j++) {
        selected = 0;
        maximum = values[0];
        for (i = 1; i < 14; i++) {
            if (maximum < values[i]) {
                maximum = values[i];
                selected = i;
            }
        }
        values[selected] = 0;
        D_80150684[j] = selected;
    }
    printf("順位計算\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, D_80150684[i]);
    }
    printf("順位作成\n ");
    offset = rand() & 3;
    for (i = 0; i < 14; i++) {
        angle = (D_80150668[i] << 12) / 1000;
        sine = func_800A0140(angle);
        D_801506BC[i] = (sine * 200) / 4096 + offset + 200;
        printf("[%d:%d %d %d] \n", i, angle, sine, D_801506BC[i]);
    }
    do {
        changed = 0;
        for (j = 0; j < 14; j++) {
            for (i = j + 1; i < 14; i++) {
                if (D_801506BC[i] == D_801506BC[j]) {
                    changed++;
                    if (D_80150668[i] < D_80150668[j]) {
                        D_801506BC[i]++;
                    } else {
                        D_801506BC[i]--;
                    }
                }
            }
        }
    } while (changed != 0);
    printf("順位ソート\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, D_801506BC[i]);
    }
    minimum = 1000;
    for (i = 0; i < 14; i++) {
        if (D_801506BC[i] < minimum) {
            minimum = D_801506BC[i];
        }
    }
    if (minimum >= 9) {
        a = rand() % (minimum - 8) + minimum - 8;
        for (i = 0; i < 14; i++) {
            D_801506BC[i] -= a;
        }
    } else if (minimum <= 0) {
        a = 1 - minimum;
        for (i = 0; i < 14; i++) {
            D_801506BC[i] += a;
        }
    }
    maximum = D_80150668[0];
    selected = 0;
    for (i = 1; i < 14; i++) {
        if (maximum < D_80150668[i]) {
            maximum = D_80150668[i];
            selected = i;
        }
    }
    if (maximum >= 466) {
        offset = D_801506BC[selected] - 1;
        for (i = 0; i < 14; i++) {
            D_801506BC[i] -= offset;
        }
    }
    printf("順位確定\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, D_801506BC[i]);
    }
    if (D_80150684[0] == 13 && D_80150684[1] == 0) {
        if (D_801506BC[0] < D_801506BC[13]) {
            a = D_801506BC[0];
            D_801506BC[0] = D_801506BC[13];
            D_801506BC[13] = a;
        }
    }
    if (D_80150684[0] == 13 && D_801506BC[13] <= 0) {
        j = 1 - D_801506BC[13];
        D_801506BC[13] = 1;
        for (i = 0; i < 13; i++) {
            D_801506BC[i] += j;
        }
    }
    for (i = 0; i < 11; i++) {
        if (D_80150668[i] < D_80150668[13]) {
            if (D_800E6280.event_flags[0xC8 + i] == 0) {
                D_800E6280.girls[i].tokimeki += 11;
                D_800E6280.event_flags[0xC8 + i] = 1;
            } else {
                D_800E6280.girls[i].tokimeki += 2;
            }
        }
    }
    if (D_801506BC[13] == 1) {
        for (i = 0; i < 11; i++) {
            D_800E6280.girls[i].tokimeki += 2;
        }
    }
}


/* Implicit int in the original (no return value): $v0 stays live at the exit, which orders the loop preheader. T-22029 */
s32 func_80148BC8() {
    s16 *x;
    s16 *y;
    s32 i;

    for (i = 0; i != 0xE; i++) {
        if (D_80150684[i] == 0xD) {
            x = &D_80150630[i];
            y = &D_8015064C[i];
            func_80148F7C(D_80150682, *x + 0x86, *y, 1, 0);
            func_80148F7C(0, *x + 0x8E, *y, 0, 0);
            func_80148F7C(D_801506F0, *x + 4, *y, 0, 1);
            if (i >= 7) {
                D_801506F4 = 1;
            } else {
                D_801506F4 = 0;
            }
        } else if (D_80150684[i] == 0xA) {
            x = &D_80150630[i];
            y = &D_8015064C[i];
            func_80148F7C(D_8015067C, *x + 0x86, *y, 1, 0);
            func_80148F7C(0, *x + 0x8E, *y, 0, 0);
            func_80148F7C(D_801506E4, *x + 4, *y, 0, 1);
        } else if (D_80150684[i] == 0xB) {
            if (D_800E6280.girls[11].flags.f.story_stage != 0) {
                x = &D_80150630[i];
                y = &D_8015064C[i];
                func_80148F7C(D_8015067E, *x + 0x86, *y, 1, 0);
                func_80148F7C(0, *x + 0x8E, *y, 0, 0);
                func_80148F7C(D_801506E8, *x + 4, *y, 0, 1);
            }
        } else if (D_800E6280.girls[D_80150684[i]].flags.f.met) {
            x = &D_80150630[i];
            y = &D_8015064C[i];
            func_80148F7C(D_80150668[D_80150684[i]], *x + 0x86, *y, 1, 0);
            func_80148F7C(0, *x + 0x8E, *y, 0, 0);
            func_80148F7C(D_801506BC[D_80150684[i]], *x + 4, *y, 0, 1);
        }
    }
}

void func_80148E98(void) {
    func_80049A40(-0xA0, -0x68, 0x140, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void func_80148EE8(void) {
    func_80049A40(-0x41, -0x70, 0x84, 0x12, 4, 0x30E00, 0);
    dtd_on(4);
}

void func_80148F34(void) {
    RECT rect;

    rect.x = 0x100;
    rect.y = 0x1F0;
    rect.w = 0x10;
    rect.h = 2;
    func_8009C884(&rect, &D_80150754);
}

/* FAKE: unused pad[2] preserves the 0x78 frame and 0x5C CSE slot (T-24121). */
void func_80148F7C(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 pad[2];
    SPRT *temp_v0;
    u32 var_s1;
    s32 var_s4;

    var_s4 = 0;
    if (arg4 == 1) {
        if (arg0 < 0xAU) {
            arg3 = 0;
        } else if (arg0 < 0x64U) {
            arg3 = 1;
        } else {
            arg3 = 2;
        }
    }
    var_s1 = arg0;
    if (var_s1 >= 0) {
        do {
            temp_v0 = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
            func_8009F0A4(temp_v0);
            SetSemiTrans(temp_v0, 0);
            temp_v0->r0 = 0x80;
            temp_v0->g0 = 0x80;
            temp_v0->b0 = 0x80;
            switch (arg3) {
            case 0:
                temp_v0->x0 = ((D_800E6280.screen_center_x[D_8011ECA0] + arg1) - (var_s4 * 8)) - (arg4 * 2);
                temp_v0->y0 = D_800E6280.screen_center_y[D_8011ECA0] + arg2;
                temp_v0->w = 8;
                temp_v0->h = 0xC;
                temp_v0->u0 = D_80150748[var_s1 % 10U];
                temp_v0->v0 = 0;
                break;
            case 1:
                temp_v0->x0 = ((D_800E6280.screen_center_x[D_8011ECA0] + arg1) - (var_s4 * 8)) + (arg4 * 4);
                temp_v0->y0 = D_800E6280.screen_center_y[D_8011ECA0] + arg2;
                temp_v0->w = 8;
                temp_v0->h = 0xC;
                temp_v0->u0 = D_80150748[var_s1 % 10U];
                temp_v0->v0 = 0x10;
                break;
            case 2:
                temp_v0->x0 = ((D_800E6280.screen_center_x[D_8011ECA0] + arg1) - (var_s4 * 6)) + (arg4 * 4);
                temp_v0->y0 = D_800E6280.screen_center_y[D_8011ECA0] + arg2;
                temp_v0->w = 6;
                temp_v0->h = 0xC;
                temp_v0->u0 = D_80150748[var_s1 % 10U];
                temp_v0->v0 = 0x20;
                break;
            }
            temp_v0->clut = func_8009ED74(0x100, arg4 + 0x1F0);
            AddPrim(D_8011ECA0 * 0x400 + 4 + D_800E8CA0, temp_v0);
            if (var_s1 < 0xAU) {
                dtd_on_tpage(0x140, 0, 1, 0, 0);
                return;
            }
            var_s4 += 1;
            var_s1 = var_s1 / 10U;
        } while (var_s1 >= 0);
    }
}
