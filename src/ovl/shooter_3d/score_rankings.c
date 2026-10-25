/* score_rankings.c: src/ovl/TACO/80136B40.c:4 dispatches ranking-screen fade/zoom steps; :119 renders stored ranking names shooter_high_score_name_1/shooter_high_score_name_2/shooter_high_score_name_3 and scores; :157 formats score digits. Original object 0x80136B40-0x80137590 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_scores(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_scores_step_init();
        return;
    case 1:
        shooter_scores_step_stars_in();
        return;
    case 2:
        shooter_scores_zoom = 0;
        mode_step_next();
        return;
    case 3:
        shooter_scores_step_zoom_in();
        return;
    case 4:
        shooter_scores_step_show();
        return;
    case 5:
        shooter_scores_step_zoom_out();
        return;
    case 6:
        shooter_scores_step_stars_out();
        return;
    case 7:
        shooter_scores_step_exit();
        /* fallthrough */
    default:
        return;
    }
}

void shooter_scores_step_init(void) {
    shooter_init_stars();
    audio_bgm_play_short(0, 0x202);
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    back_clear_switch(1);
    shooter_background_flags = 0;
    view_model_slots_clear();
    mode_step_next();
}

s32 shooter_scores_step_stars_out(void) {
    shooter_update_stars(1, 0);
    shooter_star_brightness = 0x80 - game_state.substep * 3;
    game_state.substep++;
    if (game_state.substep * 3 > 0x80) {
        mode_step_next();
    }
}

void shooter_scores_step_exit(void) {
    audio_command(0xC1);
    mode_stage_set(2);
}

void shooter_scores_step_stars_in(void) {
    if (shooter_scores_fade < 0x100) {
        shooter_star_brightness = shooter_scores_fade / 2;
    }
    shooter_update_stars(1, 0);
    shooter_scores_fade += 4;
    if (shooter_scores_fade > 0x100) {
        shooter_star_brightness = 0x80;
        mode_step_next();
    }
}

void shooter_scores_step_zoom_in(void) {
    if (game_state.pad_held & PAD_CROSS) {
        shooter_scores_zoom += 0x78;
    } else {
        shooter_scores_zoom += 0x3C;
    }
    shooter_update_stars(1, 0);
    shooter_draw_ranking(shooter_scores_zoom, 3);
    if (shooter_scores_zoom > 0x1000) {
        mode_step_next();
    }
}

void shooter_scores_step_zoom_out(void) {
    shooter_scores_zoom += 0x1F4;
    shooter_update_stars(1, 0);
    shooter_draw_ranking(shooter_scores_zoom, 0);
    if (shooter_scores_zoom >= 0x9001) {
        shooter_scores_zoom = 0;
        mode_step_next();
    }
}

void shooter_draw_ranking_glyph(u8 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 v;

    if (arg4 < 0x1800) {
        v = (arg4 << 7) / 4096;
    } else {
        v = 0xC0;
    }
    shooter_draw_glyph(arg0, arg1, 3, v, (arg2 * 12 - 0x6C) * arg4 / 4096, (arg3 * 22 - 0x42) * arg4 / 4096 - 0x14, arg4, arg4, arg5);
}

void shooter_scores_step_show(void) {
    shooter_update_stars(1, 0);
    shooter_draw_ranking(0x1000, 3);
    if (game_state.pad_held & PAD_CROSS) {
        mode_step_next();
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_next();
    }
}

void shooter_draw_ranking(s32 arg0, s32 arg1) {
    shooter_draw_ranking_text("PLAYERS RANKING", arg0, 0, arg1, 2, 1);
    shooter_draw_ranking_text("1ST", arg0, 1, arg1, 1, 4);
    shooter_draw_ranking_text("2ND", arg0, 4, arg1, 1, 5);
    shooter_draw_ranking_text("3RD", arg0, 3, arg1, 1, 6);
    shooter_draw_ranking_text(shooter_high_score_name_1, arg0, 1, arg1, 0xF, 4);
    shooter_draw_ranking_text(shooter_high_score_name_2, arg0, 4, arg1, 0xF, 5);
    shooter_draw_ranking_text(shooter_high_score_name_3, arg0, 3, arg1, 0xF, 6);
    shooter_format_score(shooter_score_text_1, shooter_high_score_1);
    shooter_format_score(shooter_score_text_2, shooter_high_score_2);
    shooter_format_score(shooter_score_text_3, shooter_high_score_3);
    shooter_draw_ranking_text(shooter_score_text_1, arg0, 2, arg1, 6, 4);
    shooter_draw_ranking_text(shooter_score_text_2, arg0, 5, arg1, 6, 5);
    shooter_draw_ranking_text(shooter_score_text_3, arg0, 6, arg1, 6, 6);
}

void shooter_draw_ranking_text(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 i;
    u8 *p;
    s32 x;

    i = 0;
    if (*arg0 != 0) {
        p = arg0;
        x = arg4;
        do {
            shooter_draw_ranking_glyph(*p, arg2, x, arg5, arg1, arg3);
            i++;
            p++;
            x++;
        } while (i < 0x15 && *p != 0);
    }
}

/* Right-aligns num (at most 8 digits) in an 8-character field at str: spaces for the missing
 * digits, then the digits from the left. `base` is one variable on purpose: 10000000 for the
 * first threshold and the copy into `place`, then 10 as the radix; IDO keeps it in a register
 * and divides by it with the zero checks, as the original does. T-19029 */
void shooter_format_score(u8 *str, s32 num) {
    s32 place;
    ShooterScoreTemplate digits;
    s32 base;

    /* FAKE: stands in for the original's unused local char digits[] = "0123456789" (11 bytes,
     * copied with lw/lw/lwl); its initializer lives in the overlay .data, not in this object,
     * so the copy reads the data symbol. Declaration order sets the frame offset. T-19029 */
    digits = shooter_score_text_template;
    if (num >= 0x5F5E100) {
        num = 0x5F5E0FF;
    }
    base = 0x989680;
    if (num >= base) {
        strcpy(str, "");
    } else if (num >= 0xF4240) {
        strcpy(str, " ");
    } else if (num >= 0x186A0) {
        strcpy(str, "  ");
    } else if (num >= 0x2710) {
        strcpy(str, "   ");
    } else if (num >= 0x3E8) {
        strcpy(str, "    ");
    } else if (num >= 0x64) {
        strcpy(str, "     ");
    } else if (num >= 0xA) {
        strcpy(str, "      ");
    } else {
        strcpy(str, "       ");
    }
    if (num == 0) {
        strcat(str, "0");
    }
    place = base;
    base = 10;
    do {
        if (num / place != 0) {
            switch ((u32)(num / place) % base) {
            case 0:
                strcat(str, "0");
                break;
            case 1:
                strcat(str, "1");
                break;
            case 2:
                strcat(str, "2");
                break;
            case 3:
                strcat(str, "3");
                break;
            case 4:
                strcat(str, "4");
                break;
            case 5:
                strcat(str, "5");
                break;
            case 6:
                strcat(str, "6");
                break;
            case 7:
                strcat(str, "7");
                break;
            case 8:
                strcat(str, "8");
                break;
            case 9:
                strcat(str, "9");
                break;
            }
        }
        place = place / base;
    } while (place > 0);
}
