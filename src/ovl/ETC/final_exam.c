/* final_exam.c: ; src/ovl/ETC/80146400.c:5-12 begins exam scoring and ranking logic. Original object 0x80146400-0x801492C0 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

/* FAKE: unused padding array preserves the frame layout. T-10850 */
s32 exam_correction_bonus(void)
{
    s32 x;
    s32 pad[3];
    s32 *p;
    s32 a;
    s32 r;
    s32 d;
    switch (game_state.year)
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

    if (game_state.month != 2)
    {
        if (game_state.month != 7)
        {
            if (game_state.month == 0xC)
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
    printf("補正 %d ", *(p = &exam_correction_table[x]));
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

s32 exam_event_log_bonus(s32 mode) {
    u32 i;
    GameState *g;

    g = &game_state;
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

void exam_entry(void) {
    s32 *p;
    s32 y;

    switch (game_state.mode_stage) {
    case 0:
        exam_stage_setup();
        break;
    case 1:
        exam_stage_day();
        break;
    case 2:
        exam_stage_end();
        break;
    }
    hud_screen_frame_draw(1);
    message_window_show();
    hizuke_show();
    parameter_show();
    hud_cursor_update();
    hud_icon_bar_update(1);
    if (game_state.weekday == WEEKDAY_SATURDAY) {
        hud_mascot_display_switch(0);
    } else {
        hud_mascot_update();
    }
    dec_bg_draw(0);
    p = &D_801230D0;
    for (y = 0x28; y != 0x58; y += 8) {
        func_8004B19C(p[10], 0x40, y);
        func_8004B19C(p[4], 0x50, y);
        p++;
    }
}
void exam_stage_day(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.weekday) {
    case WEEKDAY_MONDAY:
        exam_day_step_language();
        return;
    case WEEKDAY_TUESDAY:
        exam_day_step_math();
        return;
    case WEEKDAY_WEDNESDAY:
        exam_day_step_science();
        return;
    case WEEKDAY_THURSDAY:
        exam_day_step_social_studies();
        return;
    case WEEKDAY_FRIDAY:
        exam_day_step_art();
        return;
    case WEEKDAY_SATURDAY:
        exam_results_step();
        /* fallthrough */
    default:
        return;
    }
}

void exam_setup_init_screen(void) {
    s32 var_s0;

    text_system_reset();
    message_window_set_title((s32) "期末テスト");
    sprite_tables_clear();
    actor_clear_upper();
    tpage_buf_clear();
    hud_icon_bar_init();
    icon_disp_switch(1);
    schedule_command_icons_lock_to_record();
    for (var_s0 = 0; var_s0 != 14; var_s0++) {
        icon_can_use_set(var_s0, 0);
    }
    hud_icon_layout_apply(*(u8 *)&game_state.icon_count);
    hud_cursor_show(1);
    hizuke_init();
    parameter_show_init();
    parameter_show_init();
    if (game_state.weekday == WEEKDAY_MONDAY) {
        audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    } else if (((ExamQueueView *)&game_state)->queue_back2[game_state.day_event_index].type != DAY_EVENT_EXAM) {
        audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    }
    mode_step_next();
}

void exam_setup_background_start(void) {
    s32 r2;
    s32 r;

    if (game_state.weekday == WEEKDAY_MONDAY || *(u8 *) &game_state.clubs[10 + game_state.day_event_index] != 0x40) {
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(1, 0x205);
            r = dec_bg_cd_read(0x3FA4, 0);
            set_dec_bri(0);
            if (r != -1) {
                if (r == 0 || r == 1) {
                    dec_bg_show_set(0, r);
                    set_dec_bri(0x80);
                    mode_step_set(3);
                }
            } else {
                mode_step_next();
            }
        }
    } else {
        if (func_8004481C() == 0) {
            audio_bank_start(1, 0x205);
        }
        r2 = dec_bg_cd_read(0x3FA4, 0);
        if (r2 != -1) {
            if (r2 == 0 || r2 == 1) {
                dec_bg_show_set(0, r2);
                dec_bg_draw(0);
                set_dec_bri(0x80);
                mode_step_set(3);
            }
        } else {
            mode_step_next();
        }
    }
}

void exam_setup_background_wait(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(0x3FA4, 0);
        } else {
            set_dec_bri(0x80);
            mode_step_next();
            dec_bg_draw(0);
        }
    }
}

void exam_setup_scores_reset(void) {
    s32 i;

    if (game_state.weekday == WEEKDAY_MONDAY) {
        for (i = 0; i < 6; i++) {
            ((ExamWork *)&D_801230D0)->subject_grade[i] = 0;
            ((ExamWork *)&D_801230D0)->subject_score[i] = 0;
        }
    }
    mode_stage_next();
}

s32 exam_stage_setup(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_setup_init_screen();
        break;
    case 1:
        exam_setup_background_start();
        break;
    case 2:
        exam_setup_background_wait();
        break;
    case 3:
        exam_setup_scores_reset();
        break;
    }
}

s32 exam_day_sd_pack_load(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(9U, 0x801E0000, *(s32 *)&misc_sd_pack_lba_other[-(hud_in_summer_months() * 4)]);
        game_state.substep = 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            addr_init_else_sd(1 - hud_in_summer_months());
            mode_step_next();
        }
        break;
    }
}

void exam_player_face_set(s32 arg0) {
    actor_clear(0x60);
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((Actor *)actor_pool_96)->unk_01 = 1;
    ((Actor *)actor_pool_96)->flags = 4;
    ((Actor *)actor_pool_96)->quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)->draw_flags = 0x84;
    *(u8 **)&actor_pool_96[0xC] = D_80125CA8;
    *(u8 **)&actor_pool_96[0x10] = D_80125CAC;
    *(u8 **)&actor_pool_96[0x34] = D_80125CB0;
    ((Actor *)actor_pool_96)->unk_05 = 2;
    ((Actor *)actor_pool_96)->blink_period = 1;
    ((Actor *)actor_pool_96)->unk_43 = 0x10;
    ((Actor *)actor_pool_96)->sequence_slot = arg0;
    ((Actor *)actor_pool_96)->x = -0x50;
    ((Actor *)actor_pool_96)->y = -0x40;
}

s32 exam_score_day(void) {
    s32 zero; /* FAKE: T-25045, reuses the original T-25007 frame-padding word. */
    s32 week_total_score;
    ExamWork *exam_work;
    s32 ret;
    s32 primary_score;
    s32 sub;
    s32 biorhythm_score;
    GameState *state = &game_state;

    primary_score = 0;
    sub = 0;
    switch (state->weekday) {
    case WEEKDAY_MONDAY:
        primary_score = state->param_liberal_arts.value;
        biorhythm_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
        break;
    case WEEKDAY_TUESDAY:
        primary_score = state->param_science.value;
        biorhythm_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
        break;
    case WEEKDAY_WEDNESDAY:
        primary_score = state->param_science.value * 7 / 10;
        sub = state->param_trivia.value * 3 / 10;
        if (sub >= 0x27) {
            sub = 0x26;
        }
        biorhythm_score = hud_biorhythm_value(0, 1, 1, HUD_BIORHYTHM_SCORE_SCALE);
        break;
    case WEEKDAY_THURSDAY:
        sub = state->param_liberal_arts.value * 3 / 10;
        primary_score = state->param_trivia.value * 7 / 10;
        biorhythm_score = hud_biorhythm_value(0, 0, 1, HUD_BIORHYTHM_SCORE_SCALE);
        if (sub >= 0x27) {
            sub = 0x26;
        }
        break;
    case WEEKDAY_FRIDAY:
        primary_score = state->param_art.value * 8 / 10;
        sub = state->param_trivia.value * 2 / 10;
        biorhythm_score = hud_biorhythm_value(0, 1, 0, HUD_BIORHYTHM_SCORE_SCALE);
        if (sub >= 0x1A) {
            sub = 0x19;
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    ret = primary_score + sub + biorhythm_score - 8;
    /* FAKE: T-25007, the nibble view loads the flag byte into its own temporary as the original does */
    if (((NibbleView *)&state->player_flags.b[1])->low & 2) {
        ret /= 2;
    }
    printf("main %d sub %d bio %d ret %d ", primary_score, sub, biorhythm_score, ret);
    if (ret < 0) {
        ret = 1;
    }
    ret = exam_correction_bonus() + ret;
    sub = exam_event_log_bonus(0xF);
    ret = (rand() % 10) * sub + ret;
    exam_work = (ExamWork *)&D_801230D0;
    if (ret < 0x80) {
        exam_work->subject_score[state->weekday] = ret * 100 / 128;
    } else {
        exam_work->subject_score[state->weekday] = 100 - (rand() & 3);
    }
    printf("今日の成績 %d\t", exam_work->subject_score[state->weekday]);
    week_total_score = exam_work->subject_score[state->weekday] + exam_work->subject_score[5];
    exam_work->subject_score[5] = week_total_score;
    /* FAKE: T-25045, the late-folded zero and two identities spend one ugen
     * temporary before as1 removes them, preserving the final index registers. */
    zero = state->weekday * 0;
    printf("合計点 %d\n", (week_total_score + zero) | zero);
    ret = exam_work->subject_score[state->weekday];
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
        if (state->weekday) {
        }
    }
    exam_work->subject_grade[state->weekday] = ret;
    return ret;
}

void exam_day_start_language(void) {
    text_system_reset();
    message_window_set_title("期末試験");
    gnsx(game_state.player_family_name);
    k_disp_start(2);
    sndisp("第一日目の試験科目は 語学だ。）", 0, 0x1F);
    if (exam_score_day() < 2) {
        exam_player_face_set(1);
    } else {
        exam_player_face_set(0);
    }
    mode_step_next();
}

void exam_day_start_math(void) {
    text_system_reset();
    message_window_set_title("期末試験");
    gnsx(game_state.player_family_name);
    k_disp_start(2);
    sndisp("第二日目の試験科目は 数学だ。）", 0, 0x1F);
    if (exam_score_day() < 2) {
        exam_player_face_set(1);
    } else {
        exam_player_face_set(0);
    }
    mode_step_next();
}

void exam_day_start_science(void) {
    text_system_reset();
    message_window_set_title("期末試験");
    gnsx(game_state.player_family_name);
    k_disp_start(2);
    sndisp("第三日目の試験科目は 理科だ。）", 0, 0x1F);
    if (exam_score_day() < 2) {
        exam_player_face_set(1);
    } else {
        exam_player_face_set(0);
    }
    mode_step_next();
}

void exam_day_start_social_studies(void) {
    text_system_reset();
    message_window_set_title("期末試験");
    gnsx(game_state.player_family_name);
    k_disp_start(2);
    sndisp("第四日目の試験科目は 社会だ。）", 0, 0x1F);
    if (exam_score_day() < 2) {
        exam_player_face_set(1);
    } else {
        exam_player_face_set(0);
    }
    mode_step_next();
}

void exam_day_start_art(void) {
    text_system_reset();
    message_window_set_title("期末試験");
    gnsx(game_state.player_family_name);
    k_disp_start(2);
    sndisp("第五日目の試験科目は 美術だ。）", 0, 0x1F);
    if (exam_score_day() < 2) {
        exam_player_face_set(1);
    } else {
        exam_player_face_set(0);
    }
    mode_step_next();
}

void exam_day_wait_anim(void) {
    s16 *mode; /* pointer local: the switch reads the flag through it, as the permuter-found form that matches (T-6050) */
    if (game_state.mode_step_timer.w == 0x20) {
        D_80120652 |= 1;
    }
    if (!(D_80120652 & 1) && (D_80120668 != 0)) {
        if (D_80120658 == 0) {
            mode = &D_80120666;
            switch ((*mode) & 1) {
            case 0:
                audio_command(0x601);
                break;
            case 1:
                audio_command(0x602);
                break;
            }
        }
        if (D_80120658++ >= 0x11) {
            mode_step_next();
        }
    }
    k_disp_inc();
}

void exam_day_comment(void) {
    if (game_state.substep == 0) {
        k_reset(1);
        gnsx(game_state.player_family_name);
        switch (D_801230E0[game_state.weekday]) {
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
        game_state.substep += 1;
    } else {
        k_disp_inc();
        if (check_end_k() != 0) {
            mode_step_next();
        }
    }
}

void exam_day_end(void) {
    mode_stage_next();
}

void exam_day_step_language(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_day_sd_pack_load();
        return;
    case 1:
        exam_day_start_language();
        return;
    case 2:
        exam_day_wait_anim();
        return;
    case 3:
        exam_day_comment();
        return;
    case 4:
        exam_day_end();
        /* fallthrough */
    default:
        return;
    }
}

void exam_day_step_math(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_day_sd_pack_load();
        return;
    case 1:
        exam_day_start_math();
        return;
    case 2:
        exam_day_wait_anim();
        return;
    case 3:
        exam_day_comment();
        return;
    case 4:
        exam_day_end();
        /* fallthrough */
    default:
        return;
    }
}

void exam_day_step_science(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_day_sd_pack_load();
        return;
    case 1:
        exam_day_start_science();
        return;
    case 2:
        exam_day_wait_anim();
        return;
    case 3:
        exam_day_comment();
        return;
    case 4:
        exam_day_end();
        /* fallthrough */
    default:
        return;
    }
}

void exam_day_step_social_studies(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_day_sd_pack_load();
        return;
    case 1:
        exam_day_start_social_studies();
        return;
    case 2:
        exam_day_wait_anim();
        return;
    case 3:
        exam_day_comment();
        return;
    case 4:
        exam_day_end();
        /* fallthrough */
    default:
        return;
    }
}

void exam_day_step_art(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_day_sd_pack_load();
        return;
    case 1:
        exam_day_start_art();
        return;
    case 2:
        exam_day_wait_anim();
        return;
    case 3:
        exam_day_comment();
        return;
    case 4:
        exam_day_end();
        /* fallthrough */
    default:
        return;
    }
}

void exam_ranking_build(void);
void exam_digit_palette_upload(void); /* defined later in this file */

/* T-25054: results scene and the fourteen result display records.
 * FAKE: retain the legacy non-void fallthrough to keep the switch copy in v0;
 * the only caller ignores the result. A void definition removes that copy. */
s32 exam_results_show(void) {
    /* FAKE: the 0x10C size only reproduces the 0x188 frame and the buffer at 0x7C (original size unknown). T-22029 */
    u8 buf[0x10C];
    s32 i;
    u8 *r;
    s16 x;
    s16 y;
    s32 v;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title("結果発表 ");
        gnsx(game_state.player_family_name);
        sndisp("今日は、試験結果の 発表の日だ。）", 0, 0x1F);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        audio_bank_start(1, 0x202);
        hizuke_init();
        actor_clear_upper();
        game_state.substep += 1;
        cd_read_sectors(0xD, 0x801E0000, 0x9CA8);
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.w >= 0x101) {
            game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            text_system_reset();
            icon_disp_switch(0);
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            actor_clear(0x60);
            ((Actor *)actor_pool)[96].unk_01 = 4;
            ((Actor *)actor_pool)[96].flags = 0x40;
            ((Actor *)actor_pool)[96].quad_flags = 0;
            ((Actor *)actor_pool)[96].draw_flags = 0xA4;
            ((Actor *)actor_pool)[96].frame_table = (u8 *) 0x801E64A0;
            ((Actor *)actor_pool)[96].sequence_table = (u8 *) 0x801E6520;
            ((Actor *)actor_pool)[96].tpage_table = (u8 *) 0x801E6494;
            ((Actor *)actor_pool)[96].unk_18 = 0;
            ((Actor *)actor_pool)[96].unk_05 = 1;
            ((Actor *)actor_pool)[96].unk_04 = 8;
            ((Actor *)actor_pool)[96].x = 0;
            ((Actor *)actor_pool)[96].y = -0x28;
            ((Actor *)actor_pool)[96].brightness = 0x80;
            exam_panel_draw_big();
            exam_panel_draw_header();
            exam_ranking_build();
            /* FAKE: T-25054, the always-false unsigned comparison folds after
             * uopt, keeping the walked base separate from the indexed base.
             * Keep i's initialization and the following do on one line for
             * the original address schedule. */
            do { r = actor_pool; } while ((u32)game_state.weekday < 0); i = 0; do {
                x = exam_row_x[i];
                y = exam_row_y[i];
                v = exam_ranking_order[i];
                ((Actor *)r)[0x61].unk_01 = 3;
                ((Actor *)r)[0x61].flags = 0x40;
                ((Actor *)r)[0x61].quad_flags = 0;
                ((Actor *)r)[0x61].draw_flags = 0x24;
                ((Actor *)r)[0x61].frame_table = (u8 *)0x801E64A0;
                ((Actor *)r)[0x61].sequence_table = (u8 *)0x801E6520;
                ((Actor *)r)[0x61].tpage_table = (u8 *)0x801E6494;
                ((Actor *)r)[0x61].unk_18 = 0x1F;
                ((Actor *)r)[0x61].unk_05 = 1;
                ((Actor *)r)[0x61].unk_04 = 8;
                ((Actor *)r)[0x61].brightness = 0x80;
                ((Actor *)r)[0x61].x = x;
                ((Actor *)r)[0x61].y = y;
                if (v == 0xD) {
                    strncpy((char *) buf, (char *) game_state.player_family_name, 8);
                    strncat((char *) buf, (char *) game_state.player_given_name, 8);
                    set_kanji_string(exam_row_x[i] + 0x18, exam_row_y[i], 3, buf, 0);
                    printf("%s\n", game_state.player_full_name);
                } else if (v == 0xA) {
                    set_kanji_string(x + 0x18, y, 3, girl_full_name_table[13], 0);
                } else if (v == 0xB) {
                    if (game_state.girls[11].flags.f.story_stage != 0) {
                        set_kanji_string(x + 0x18, y, 3, girl_full_name_table[11], 0);
                    } else {
                        /* FAKE: T-25054, this late-folded zero prevents uopt
                         * from distributing 0x61 into the field displacement.
                         * The same identity is used in the final else arm. */
                        ((Actor *) actor_pool)[(i + 0x61) | ((u32)i < 0)].draw_flags = 0xA4;
                    }
                } else if (game_state.girls[v].flags.f.met) {
                    set_kanji_string(x + 0x18, y, 3, girl_full_name_table[v], 0);
                } else {
                    ((Actor *) actor_pool)[(i + 0x61) | ((u32)i < 0)].draw_flags = 0xA4;
                }
                r += 0x44;
                i += 1;
            } while (i != 0xE);
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            exam_digit_palette_upload();
            mode_step_next();
        }
        break;
    }
}

/* Bits 0 and 1 of the low byte of GameState +0xF6 (T-7010). */
typedef struct {
    u8 f0 : 1;
    u8 f1 : 1;
    u8 rest : 6;
} ExamPlayerFlagBits;

void exam_results_react(void) {
    exam_ranking_numbers_draw();
    game_state.player_flags_upper.b[0] &= 0xFFFD;
    if (game_state.mode_step_timer.u >= 0x79U) {
        gnsx(game_state.player_family_name);
        if (D_801230F4 >= 3) {
            sndisp("３科目以上赤点だった。 明日から、一週間補習だぁ。 勉強しかできないー！）", 0, 0x1F);
            audio_bank_start(1, 0x203);
            ((ExamPlayerFlagBits *)&game_state.player_flags_upper.b[0])->f0 = 1;
            game_state.month_calendar[0].marker[game_state.day + 1] = 0x17;
            ((ExamPlayerFlagBits *)&game_state.player_flags_upper.b[0])->f1 = 1;
        } else if (exam_rank_player < 2) {
            sndisp("やったー、一番だ！ これで試験の帝王だな。）", 0, 0x1F);
            audio_bank_start(1, 0x204);
        } else if (exam_player_in_lower_half != 0) {
            sndisp("今回の試験は終了した。）", 0, 0x1F);
            ((ExamPlayerFlagBits *)&game_state.player_flags_upper.b[0])->f1 = 1;
            audio_bank_start(1, 0x200);
        } else {
            sndisp("今回の試験は終了した。）", 0, 0x1F);
            audio_bank_start(1, 0x201);
        }
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        hud_cursor_show(0);
        mode_step_next();
    }
    exam_panel_draw_big();
    exam_panel_draw_header();
}

void exam_results_wait(void) {
    if (((game_state.pad_pressed & PAD_CIRCLE) && (u32)game_state.mode_step_timer.w >= 0x101) || (game_state.pad_pressed & PAD_CROSS)) {
        hud_cursor_show(1);
        mode_stage_next();
    }
    exam_panel_draw_big();
    exam_panel_draw_header();
    exam_ranking_numbers_draw();
}

s32 exam_results_step(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        exam_results_show();
        break;
    case 1:
        exam_results_react();
        break;
    case 2:
        exam_results_wait();
        break;
    }
}

void exam_end_init(void) {
    parameter_show_init();
    parameter_show_init();
    actor_clear_upper();
    text_system_reset();
    mode_step_next();
}

void exam_end_leave(void) {
    if (game_state.weekday == WEEKDAY_SATURDAY) {
        audio_bank_start(0, 0);
    }
    day_event_next(0);
}

void exam_stage_end(void) {
    switch (game_state.mode_step) {
    case 0:
        exam_end_init();
        break;
    case 1:
        exam_end_leave();
        break;
    }
    parameter_show();
}
/* T-25059: generate exam scores, order and ranks, then apply relationship bonuses. */
void exam_ranking_build(void) {
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

    srand(game_state.frame_count);
    for (i = 0; i < 13; i++) {
        first_random = rand();
        noise = (first_random % (s32)exam_score_spread[i]) * (((rand() & 1) * 2) - 1);
        first_random = rand();
        a = (first_random % (s32)exam_score_spread[i]) * ((rand() & 1) * 2 - 1);
        exam_ranking_value[i] = ((exam_base_score[i] + a) * 5) + noise;
        j = exam_event_log_bonus(i);
        if (j != 1) {
            if (j == 9) {
                exam_ranking_value[i] += rand() % 250;
            }
        } else {
            exam_ranking_value[i] += rand() % 50;
        }
        if (exam_ranking_value[i] >= 501) {
            exam_ranking_value[i] = 500;
        }
        printf("[%d:%d]\n", i, exam_ranking_value[i]);
    }
    exam_ranking_value[13] = (s16) ((ExamWork *)&D_801230D0)->subject_score[5];
    do {
        changed = 0;
        for (j = 0; j < 14; j++) {
            for (i = j + 1; i < 14; i++) {
                if (exam_ranking_value[i] == exam_ranking_value[j]) {
                    changed++;
                    exam_ranking_value[i]--;
                }
            }
        }
    } while (changed != 0);
    for (i = 0; i < 14; i++) {
        values[i] = exam_ranking_value[i];
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
        exam_ranking_order[j] = selected;
    }
    printf("順位計算\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, exam_ranking_order[i]);
    }
    printf("順位作成\n ");
    offset = rand() & 3;
    for (i = 0; i < 14; i++) {
        angle = (exam_ranking_value[i] << 12) / 1000;
        sine = func_800A0140(angle);
        exam_ranking_rank[i] = (sine * 200) / 4096 + offset + 200;
        printf("[%d:%d %d %d] \n", i, angle, sine, exam_ranking_rank[i]);
    }
    do {
        changed = 0;
        for (j = 0; j < 14; j++) {
            for (i = j + 1; i < 14; i++) {
                if (exam_ranking_rank[i] == exam_ranking_rank[j]) {
                    changed++;
                    if (exam_ranking_value[i] < exam_ranking_value[j]) {
                        exam_ranking_rank[i]++;
                    } else {
                        exam_ranking_rank[i]--;
                    }
                }
            }
        }
    } while (changed != 0);
    printf("順位ソート\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, exam_ranking_rank[i]);
    }
    minimum = 1000;
    for (i = 0; i < 14; i++) {
        if (exam_ranking_rank[i] < minimum) {
            minimum = exam_ranking_rank[i];
        }
    }
    if (minimum >= 9) {
        a = rand() % (minimum - 8) + minimum - 8;
        for (i = 0; i < 14; i++) {
            exam_ranking_rank[i] -= a;
        }
    } else if (minimum <= 0) {
        a = 1 - minimum;
        for (i = 0; i < 14; i++) {
            exam_ranking_rank[i] += a;
        }
    }
    maximum = exam_ranking_value[0];
    selected = 0;
    for (i = 1; i < 14; i++) {
        if (maximum < exam_ranking_value[i]) {
            maximum = exam_ranking_value[i];
            selected = i;
        }
    }
    if (maximum >= 466) {
        offset = exam_ranking_rank[selected] - 1;
        for (i = 0; i < 14; i++) {
            exam_ranking_rank[i] -= offset;
        }
    }
    printf("順位確定\n ");
    for (i = 0; i < 14; i++) {
        printf("[%d:%d] \n", i, exam_ranking_rank[i]);
    }
    if (exam_ranking_order[0] == 13 && exam_ranking_order[1] == 0) {
        if (exam_ranking_rank[0] < exam_ranking_rank[13]) {
            a = exam_ranking_rank[0];
            exam_ranking_rank[0] = exam_ranking_rank[13];
            exam_ranking_rank[13] = a;
        }
    }
    if (exam_ranking_order[0] == 13 && exam_ranking_rank[13] <= 0) {
        j = 1 - exam_ranking_rank[13];
        exam_ranking_rank[13] = 1;
        for (i = 0; i < 13; i++) {
            exam_ranking_rank[i] += j;
        }
    }
    for (i = 0; i < 11; i++) {
        if (exam_ranking_value[i] < exam_ranking_value[13]) {
            if (game_state.scene_flags[0xC8 + i] == 0) {
                game_state.girls[i].tokimeki += 11;
                game_state.scene_flags[0xC8 + i] = 1;
            } else {
                game_state.girls[i].tokimeki += 2;
            }
        }
    }
    if (exam_ranking_rank[13] == 1) {
        for (i = 0; i < 11; i++) {
            game_state.girls[i].tokimeki += 2;
        }
    }
}


/* Implicit int in the original (no return value): $v0 stays live at the exit, which orders the loop preheader. T-22029 */
s32 exam_ranking_numbers_draw() {
    s16 *x;
    s16 *y;
    s32 i;

    for (i = 0; i != 0xE; i++) {
        if (exam_ranking_order[i] == 0xD) {
            x = &exam_row_x[i];
            y = &exam_row_y[i];
            exam_number_draw(exam_value_player, *x + 0x86, *y, 1, 0);
            exam_number_draw(0, *x + 0x8E, *y, 0, 0);
            exam_number_draw(exam_rank_player, *x + 4, *y, 0, 1);
            if (i >= 7) {
                exam_player_in_lower_half = 1;
            } else {
                exam_player_in_lower_half = 0;
            }
        } else if (exam_ranking_order[i] == 0xA) {
            x = &exam_row_x[i];
            y = &exam_row_y[i];
            exam_number_draw(exam_value_friend, *x + 0x86, *y, 1, 0);
            exam_number_draw(0, *x + 0x8E, *y, 0, 0);
            exam_number_draw(exam_rank_friend, *x + 4, *y, 0, 1);
        } else if (exam_ranking_order[i] == 0xB) {
            if (game_state.girls[11].flags.f.story_stage != 0) {
                x = &exam_row_x[i];
                y = &exam_row_y[i];
                exam_number_draw(exam_value_miharu, *x + 0x86, *y, 1, 0);
                exam_number_draw(0, *x + 0x8E, *y, 0, 0);
                exam_number_draw(exam_rank_miharu, *x + 4, *y, 0, 1);
            }
        } else if (game_state.girls[exam_ranking_order[i]].flags.f.met) {
            x = &exam_row_x[i];
            y = &exam_row_y[i];
            exam_number_draw(exam_ranking_value[exam_ranking_order[i]], *x + 0x86, *y, 1, 0);
            exam_number_draw(0, *x + 0x8E, *y, 0, 0);
            exam_number_draw(exam_ranking_rank[exam_ranking_order[i]], *x + 4, *y, 0, 1);
        }
    }
}

void exam_panel_draw_big(void) {
    prim_gradient_rect(-0xA0, -0x68, 0x140, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void exam_panel_draw_header(void) {
    prim_gradient_rect(-0x41, -0x70, 0x84, 0x12, 4, 0x30E00, 0);
    dtd_on(4);
}

void exam_digit_palette_upload(void) {
    RECT rect;

    rect.x = 0x100;
    rect.y = 0x1F0;
    rect.w = 0x10;
    rect.h = 2;
    func_8009C884(&rect, &exam_digit_palette);
}

/* FAKE: unused pad[2] preserves the 0x78 frame and 0x5C CSE slot (T-24121). */
void exam_number_draw(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
            temp_v0 = (SPRT *)GetWorkBase(0x14, draw_buffer);
            func_8009F0A4(temp_v0);
            SetSemiTrans(temp_v0, 0);
            temp_v0->r0 = 0x80;
            temp_v0->g0 = 0x80;
            temp_v0->b0 = 0x80;
            switch (arg3) {
            case 0:
                temp_v0->x0 = ((game_state.screen_center_x[draw_buffer] + arg1) - (var_s4 * 8)) - (arg4 * 2);
                temp_v0->y0 = game_state.screen_center_y[draw_buffer] + arg2;
                temp_v0->w = 8;
                temp_v0->h = 0xC;
                temp_v0->u0 = exam_digit_u[var_s1 % 10U];
                temp_v0->v0 = 0;
                break;
            case 1:
                temp_v0->x0 = ((game_state.screen_center_x[draw_buffer] + arg1) - (var_s4 * 8)) + (arg4 * 4);
                temp_v0->y0 = game_state.screen_center_y[draw_buffer] + arg2;
                temp_v0->w = 8;
                temp_v0->h = 0xC;
                temp_v0->u0 = exam_digit_u[var_s1 % 10U];
                temp_v0->v0 = 0x10;
                break;
            case 2:
                temp_v0->x0 = ((game_state.screen_center_x[draw_buffer] + arg1) - (var_s4 * 6)) + (arg4 * 4);
                temp_v0->y0 = game_state.screen_center_y[draw_buffer] + arg2;
                temp_v0->w = 6;
                temp_v0->h = 0xC;
                temp_v0->u0 = exam_digit_u[var_s1 % 10U];
                temp_v0->v0 = 0x20;
                break;
            }
            temp_v0->clut = func_8009ED74(0x100, arg4 + 0x1F0);
            AddPrim(draw_buffer * 0x400 + 4 + D_800E8CA0, temp_v0);
            if (var_s1 < 0xAU) {
                dtd_on_tpage(0x140, 0, 1, 0, 0);
                return;
            }
            var_s4 += 1;
            var_s1 = var_s1 / 10U;
        } while (var_s1 >= 0);
    }
}
