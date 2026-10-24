/* debug_game_data_editors.c: src/ovl/OPTION/8013A550.c:65: debug pages edit game settings, date, girl records and flags through menu controls; body 75: label_or_month = game_state.weekday; 76: menu_check(0, cursor_x, cursor_y);. Original object 0x8013A550-0x8013C880 (config/objects/OPTION.txt). */
#include "common.h"
#include "ovl/OPTION.h"


s32 func_8013A550(s32 arg0, s32 arg1, s32 arg2) {
    if (game_state.pad_pressed & PAD_SQUARE) {
        arg0 += 1;
        if (arg1 < arg0) {
            arg0 = arg2;
        }
    } else if ((game_state.pad_held & PAD_SQUARE) && (game_state.pad_held & 5)) {
        arg0 += 1;
        if (arg1 < arg0) {
            arg0 = arg2;
        }
    }
    if (game_state.pad_pressed & PAD_TRIANGLE) {
        arg0 -= 1;
        if (arg0 < arg2) {
            arg0 = arg1;
        }
    } else if ((game_state.pad_held & PAD_TRIANGLE) && (game_state.pad_held & 5)) {
        arg0 -= 1;
        if (arg0 < arg2) {
            arg0 = arg1;
        }
    }
    return arg0;
}

void func_8013A604(void) {
    s32 i;
    s32 padx[1]; /* FAKE: one unused word above the arrays reproduces the original frame and array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    D_800B3C6C = 1;
    hud_cursor_actor_init(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    menu_set(0, 0x10, a, b, c, d);
    (&game_state.clear_color_r)[0] = (&game_state.clear_color_r)[1] = (&game_state.clear_color_r)[2] = 0x80;
    func_8004ADAC(4);
    strcpy(game_state.player_family_name, "おぷ");
    strcpy(game_state.player_given_name, "ション");
    strcpy(game_state.player_nickname, "起動");
    text_system_reset();
    set_kanji_string(-0x80, -0x18, 0xF, (s32)"設定メニュー１", 0);
    set_kanji_string(-0x80, 0, 0xF, (s32)"月日関連", 0);
    set_kanji_string(0x20, -0x18, 0xF, (s32)game_state.player_family_name, 0);
    set_kanji_string(0x20, -0xC, 0xF, (s32)game_state.player_given_name, 0);
    set_kanji_string(0x20, 0, 0xF, (s32)game_state.player_nickname, 0);
    k_disp_start(5);
    mode_step_next();
}

/* FAKE: integer return type with fallthrough keeps v0 live across the calendar flag
 * branch, preserving its nop delay slot. No meaningful return. T-24606. */
s32 func_8013A7EC(void) {
    GameState *game = &game_state;
    /* FAKE: share the label pointer/month and drawing row/loop bound
     * words to preserve the original saved-register lifetimes. T-24606. */
    s32 label_or_month;
    s32 row_or_limit;
    s32 day;

    /* FAKE: discarded entry read enables the conversion pass to preserve
     * the unsigned weekday global across display and comparison. T-24606. */
    label_or_month = game_state.weekday;
    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    label_or_month = (s32)D_8013E744;
    row_or_limit = -0x60;
    do {
        func_8004B338(*(s32 *)label_or_month, -0x20, row_or_limit);
        row_or_limit += 0xC;
        label_or_month += 4;
    } while (row_or_limit < 0x60);
    draw_debug_number((s32) game->year, 0x20, -0x60);
    draw_debug_number((s32) game->month, 0x20, -0x54);
    draw_debug_number((s32) game->day, 0x20, -0x48);
    draw_debug_number((s32) game_state.weekday, 0x20, -0x3C);
    draw_debug_number((s32) game->vacation, 0x20, -0x30);
    draw_debug_number((s32) game->day_count, 0x20, -0x24);
    draw_debug_number(game->player_birthday & 0xF, 0x20, 0xC);
    draw_debug_number((s32) ((u32) (game->player_birthday << 0x17) >> 0x1B), 0x20, 0x18);
    draw_debug_number((s32) ((u32) (game->player_birthday << 0x15) >> 0x1E), 0x20, 0x24);
    draw_debug_number((s32) ((u16) game->player_flags.h >> 0xC), 0x20, 0x30);
    draw_debug_number(game->girls[0].birthday.w & 0xF, 0x20, 0x3C);
    draw_debug_number((s32) ((u32) (game->girls[0].birthday.w << 0x17) >> 0x1B), 0x20, 0x48);
    draw_debug_number((s32) ((u32) (game->girls[0].birthday.w << 0x15) >> 0x1E), 0x20, 0x54);
    if (game->pad_held & 0x90) {
        /* FAKE: the unsigned byte bitfield keeps the selector in a ugen
         * temporary instead of a promoted local in v0. T-24606. */
        switch (((OptionByteBf *)&game->menu_cursor_0)->x) {
        case 0:
        case 1:
            break;
        case 2:
            if (game->pad_pressed & 0x90) {
                label_or_month = game->month;
                day = 0;
                if (game_state.weekday != WEEKDAY_SUNDAY) {
                    day = game_state.weekday;
                    if ((s32) game_state.weekday < WEEKDAY_SUNDAY) {
                        do {
                            calendar_advance_day();
                            day++;
                        } while (day < 6);
                    }
                } else {
                    row_or_limit = 7;
                    do {
                        calendar_advance_day();
                        day += 1;
                    } while (day != row_or_limit);
                }
                if (label_or_month != game->month) {
                    mode_step_set(0xFE);
                }
            }
            break;
        case 4:
            game->vacation = func_8013A550((s32) game->vacation, 3, 0);
            break;
        case 9:
            ((OptionDateBits *)&game->player_birthday)->f0 = func_8013A550(((OptionDateBits *)&game->player_birthday)->f0, 12, 1);
            break;
        case 10:
            ((OptionDateBits *)&game->player_birthday)->f4 = func_8013A550(((OptionDateBits *)&game->player_birthday)->f4, 31, 1);
            break;
        case 11:
            ((OptionDateBits *)&game->player_birthday)->f9 = func_8013A550(((OptionDateBits *)&game->player_birthday)->f9, 3, 0);
            break;
        case 12:
            ((PlayerFlagBits *)&game->player_flags)->club = func_8013A550(((PlayerFlagBits *)&game->player_flags)->club, 11, 0);
            break;
        case 13:
            game->girls[0].birthday.date.month = func_8013A550(game->girls[0].birthday.date.month, 12, 1);
            break;
        case 14:
            game->girls[0].birthday.date.day = func_8013A550(game->girls[0].birthday.date.day, 31, 1);
            break;
        case 15:
            game->girls[0].birthday.date.blood_type = func_8013A550(game->girls[0].birthday.date.blood_type, 3, 0);
            break;
        }
    }
    if (game->pad_pressed & PAD_START) {
        func_80041878();
        mode_step_set(0);
    }
}

/* No return value on purpose: the original is an implicit-int function (the `or v0,v1,zero` in the first delay slot). T-6070 */
s32 func_8013AC30(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            calendar_month_load(game_state.year, game_state.month, 1);
            D_800B5BC8 = 0xEF;
            hud_day_bgm_load();
            mode_step_next();
        }
        break;
    }
}

void func_8013ACD4(void) {
    if (audio_bank_load_poll() == 1) {
        audio_bank_start(0, 0);
        mode_step_set(1);
    }
}

s32 func_8013AD10(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013A604();
        break;
    case 1:
        func_8013A7EC();
        break;
    case 254:
        func_8013AC30();
        break;
    case 255:
        func_8013ACD4();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD24);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD28);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD34);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD3C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD40);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD44);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD4C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD54);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD58);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD74);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CD7C);

void func_8013AD94(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    hud_cursor_actor_init(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    menu_set(0, 0x10, a, b, c, d);
    text_system_reset();
    set_kanji_string(-0x80, -0x18, 0xF, (s32)"設定メニュー２", 0);
    set_kanji_string(-0x80, 0, 0xF, (s32)"プレイヤーデータ", 0);
    set_kanji_string(0x60, -0x18, 0xF, (s32)game_state.player_family_name, 0);
    set_kanji_string(0x60, -0xC, 0xF, (s32)game_state.player_given_name, 0);
    set_kanji_string(0x60, 0, 0xF, (s32)game_state.player_nickname, 0);
    k_disp_start(5);
    mode_step_next();
}

void func_8013AF10(void) {
    GameState *g = &game_state;
    s32 *var_s1;
    s32 var_s0;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    var_s1 = D_8013E784;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1++;
    } while (var_s0 != 0x60);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->health, 0x20, -0x60);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->club, 0x20, -0x54);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->b16, 0x20, -0x48);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->b17, 0x20, -0x3C);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->b18, 0x20, -0x30);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->b19, 0x20, -0x24);
    draw_debug_number(((PlayerFlagBits *)&g->player_flags)->b20, 0x20, -0x18);
    draw_debug_number((s32) g->param_condition.value, 0x20, -0xC);
    draw_debug_number((s32) g->param_liberal_arts.value, 0x20, 0);
    draw_debug_number((s32) g->param_science.value, 0x20, 0xC);
    draw_debug_number((s32) g->param_art.value, 0x20, 0x18);
    draw_debug_number((s32) g->param_athletics.value, 0x20, 0x24);
    draw_debug_number((s32) g->param_trivia.value, 0x20, 0x30);
    draw_debug_number((s32) g->param_looks.value, 0x20, 0x3C);
    draw_debug_number((s32) g->param_guts.value, 0x20, 0x48);
    draw_debug_number((s32) g->param_stress.value, 0x20, 0x54);
    if (g->pad_held & 0x90) {
        switch (g->menu_cursor_0) {
        case 0:
            ((PlayerFlagBits *)&g->player_flags)->health = func_8013A550(((PlayerFlagBits *)&g->player_flags)->health, 3, 0);
            break;
        case 1:
            ((PlayerFlagBits *)&g->player_flags)->club = func_8013A550(((PlayerFlagBits *)&g->player_flags)->club, 11, 0);
            break;
        case 2:
            ((PlayerFlagBits *)&g->player_flags)->b16 = func_8013A550(((PlayerFlagBits *)&g->player_flags)->b16, 1, 0);
            break;
        case 3:
            ((PlayerFlagBits *)&g->player_flags)->b17 = func_8013A550(((PlayerFlagBits *)&g->player_flags)->b17, 1, 0);
            break;
        case 4:
            ((PlayerFlagBits *)&g->player_flags)->b18 = func_8013A550(((PlayerFlagBits *)&g->player_flags)->b18, 1, 0);
            break;
        case 5:
            ((PlayerFlagBits *)&g->player_flags)->b19 = func_8013A550(((PlayerFlagBits *)&g->player_flags)->b19, 1, 0);
            break;
        case 6:
            ((PlayerFlagBits *)&g->player_flags)->b20 = func_8013A550(((PlayerFlagBits *)&g->player_flags)->b20, 1, 0);
            break;
        case 7:
            g->param_condition.value = func_8013A550((s32) g->param_condition.value, 0x3E7, 0);
            break;
        case 8:
            g->param_liberal_arts.value = func_8013A550((s32) g->param_liberal_arts.value, 0x3E7, 0);
            break;
        case 9:
            g->param_science.value = func_8013A550((s32) g->param_science.value, 0x3E7, 0);
            break;
        case 10:
            g->param_art.value = func_8013A550((s32) g->param_art.value, 0x3E7, 0);
            break;
        case 11:
            g->param_athletics.value = func_8013A550((s32) g->param_athletics.value, 0x3E7, 0);
            break;
        case 12:
            g->param_trivia.value = func_8013A550((s32) g->param_trivia.value, 0x3E7, 0);
            break;
        case 13:
            g->param_looks.value = func_8013A550((s32) g->param_looks.value, 0x3E7, 0);
            break;
        case 14:
            g->param_guts.value = func_8013A550((s32) g->param_guts.value, 0x3E7, 0);
            break;
        case 15:
            g->param_stress.value = func_8013A550((s32) g->param_stress.value, 0x3E7, 0);
            break;
        }
    }
}


s32 func_8013B3BC(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AD94();
        break;
    case 1:
        func_8013AF10();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDAC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDB8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDC0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDCC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDD4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDE0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDE8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDF4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CDFC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE08);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE10);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE28);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE38);

void func_8013B410(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    hud_cursor_actor_init(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    menu_set(0, 0x10, a, b, c, d);
    text_system_reset();
    set_kanji_string(-0x80, -0x18, 0xF, (s32)"設定メニュー３", 0);
    set_kanji_string(-0x80, 0, 0xF, (s32)"女の子データ", 0);
    set_kanji_string(0x40, -0x18, 0xF, (&girl_full_name_table)[D_8013E740 & 0xF], 0);
    k_disp_start(3);
    mode_step_next();
}

void func_8013B568(void) {
    s32 *var_s1;
    s32 var_s0;
    u8 *var_v1;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    var_s1 = D_8013E7C4;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1++;
    } while (var_s0 != 0x60);
    draw_debug_number((s32) D_8013E740, 0x20, -0x60);
    draw_debug_number((s32) game_state.girls[D_8013E740].tokimeki, 0x20, -0x54);
    draw_debug_number((s32) game_state.girls[D_8013E740].affection, 0x20, -0x48);
    draw_debug_number((s32) game_state.girls[D_8013E740].syoushin, 0x20, -0x3C);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b0, 0x20, -0x30);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b1, 0x20, -0x24);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b2, 0x20, -0x18);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b3, 0x20, -0xC);
    draw_debug_number((s32) ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f24, 0x20, 0);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b6, 0x20, 0xC);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b7, 0x20, 0x18);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b8, 0x20, 0x24);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f9, 0x20, 0x30);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f20, 0x20, 0x3C);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b12, 0x20, 0x48);
    draw_debug_number(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b13, 0x20, 0x54);
    if (game_state.pad_held & 0x90) {
        switch (game_state.menu_cursor_0) {
        case 0:
            D_8013E740 = func_8013A550((s32) D_8013E740, 0xB, 0);
            mode_step_set(0);
            break;
        case 1:
            game_state.girls[D_8013E740].tokimeki = func_8013A550((s32) game_state.girls[D_8013E740].tokimeki, 0x7F, 0);
            break;
        case 2:
            game_state.girls[D_8013E740].affection = func_8013A550((s32) game_state.girls[D_8013E740].affection, 0x7F, 0);
            break;
        case 3:
            game_state.girls[D_8013E740].syoushin = func_8013A550((s32) game_state.girls[D_8013E740].syoushin, 0x7F, 0);
            break;
        case 4:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b0 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b0, 1, 0);
            break;
        case 5:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b1 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b1, 1, 0);
            break;
        case 6:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b2 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b2, 1, 0);
            break;
        case 7:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b3 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b3, 1, 0);
            break;
        case 8:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f24 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f24, 999, 0);
            break;
        case 9:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b6 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b6, 1, 0);
            break;
        case 10:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b7 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b7, 1, 0);
            break;
        case 11:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b8 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b8, 1, 0);
            break;
        case 12:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f9 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f9, 7, 0);
            break;
        case 13:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f20 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->f20, 11, 0);
            break;
        case 14:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b12 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b12, 1, 0);
            break;
        case 15:
            ((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b13 = func_8013A550(((OptionCharBits *)&game_state.girls[D_8013E740].flags)->b13, 1, 0);
            break;
        }
    }
}


s32 func_8013BE78(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013B410();
        break;
    case 1:
        func_8013B568();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE74);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE7C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE88);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE94);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CE9C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEA4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEAC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEB0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEB8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEC0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEC8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CECC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CED4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CEDC);

void func_8013BECC(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    hud_cursor_actor_init(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    menu_set(0, 0x10, a, b, c, d);
    text_system_reset();
    set_kanji_string(-0x80, -0x18, 0xF, (s32)"設定メニュー４", 0);
    set_kanji_string(-0x80, 0, 0xF, (s32)"その他データ", 0);
    k_disp_start(2);
    mode_step_next();
    game_state.day_event_index = 0;
}

void func_8013BFF8(void) {
    GameState *g = &game_state;
    s32 *var_s1;
    s32 var_s0;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    var_s1 = D_8013E804;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1++;
    } while (var_s0 != 0x60);
    draw_debug_number((s32) g->day_events[0].girl, 0x20, -0x60);
    draw_debug_number((s32) g->day_events[0].scene, 0x20, -0x54);
    draw_debug_number(((AppointmentBits *)&g->unk_54C[0])->flag, 0x20, -0x30);
    draw_debug_number(((AppointmentBits *)&g->unk_54C[0])->girl, 0x20, -0x24);
    draw_debug_number(((AppointmentBits *)&g->unk_54C[0])->place, 0x20, -0x18);
    draw_debug_number((s32) g->appointment_place_0, 0x20, 0);
    draw_debug_number((s32) g->appointment_place_1, 0x20, 0xC);
    draw_debug_number((s32) g->appointment_place_2, 0x20, 0x18);
    draw_debug_number((s32) g->appointment_place_3, 0x20, 0x24);
    draw_debug_number((s32) g->current_girl, 0x20, 0x30);
    draw_debug_number((s32) g->date_girl, 0x20, 0x3C);
    draw_debug_number(D_80122CD4, 0x20, 0x48);
    draw_debug_number(talk_work, 0x20, 0x54);
    if (g->pad_held & 0x90) {
        switch (g->menu_cursor_0) {
        case 0:
            g->day_events[0].girl = func_8013A550((s32) g->day_events[0].girl, 0xA, 0);
            break;
        case 1:
            g->day_events[0].scene = func_8013A550((s32) g->day_events[0].scene, 0x24, 0);
            break;
        case 4:
            ((AppointmentBits *)&g->unk_54C[0])->flag = func_8013A550(((AppointmentBits *)&g->unk_54C[0])->flag, 1, 0);
            break;
        case 5:
            ((AppointmentBits *)&g->unk_54C[0])->girl = func_8013A550(((AppointmentBits *)&g->unk_54C[0])->girl, 0xA, 0);
            break;
        case 6:
            ((AppointmentBits *)&g->unk_54C[0])->place = func_8013A550(((AppointmentBits *)&g->unk_54C[0])->place, 0x15, 0);
            break;
        case 8:
            g->appointment_place_0 = func_8013A550((s32) g->appointment_place_0, 0x15, 0);
            break;
        case 9:
            g->appointment_place_1 = func_8013A550((s32) g->appointment_place_1, 0x15, 0);
            break;
        case 10:
            g->appointment_place_2 = func_8013A550((s32) g->appointment_place_2, 0x15, 0);
            break;
        case 11:
            g->appointment_place_3 = func_8013A550((s32) g->appointment_place_3, 0x15, 0);
            break;
        case 12:
            g->current_girl = func_8013A550((s32) g->current_girl, 0xA, 0);
            break;
        case 13:
            g->date_girl = func_8013A550((s32) g->date_girl, 0xA, 0);
            break;
        case 14:
            D_80122CD4 = func_8013A550(D_80122CD4, 0xC, 0);
            break;
        case 15:
            talk_work = func_8013A550(talk_work, 8, 1);
            break;
        }
    }
}


s32 func_8013C3B0(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013BECC();
        break;
    case 1:
        func_8013BFF8();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF04);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF0C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF24);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF38);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF40);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF48);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF50);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF58);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF70);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF78);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CF80);

void func_8013C404(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    hud_cursor_actor_init(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    menu_set(0, 0x10, a, b, c, d);
    text_system_reset();
    set_kanji_string(-0x80, -0x18, 0xF, (s32)"設定メニューＦ", 0);
    set_kanji_string(-0x80, 0, 0xF, (s32)"目標ステップ", 0);
    k_disp_start(5);
    mode_step_next();
}

void func_8013C52C(void) {
    s32 i;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    for (i = 0; i < 16; i++) {
        func_8004B338(D_8013E844[i], -0x20, i * 0xC - 0x60);
    }
    bzero(D_80122DD0, 0x100);
    if (game_state.pad_pressed & 0x90) {
        switch (game_state.menu_cursor_0) {
        case 0:
            D_800B3C6C = 0;
            game_mode_set(GAME_MODE_COMMAND_SCREEN);
        default:
            break;
        case 1:
            game_mode_set(GAME_MODE_DATE);
            break;
        case 2:
            text_system_reset();
            func_80048DD0(0);
            game_mode_set(GAME_MODE_SPECIAL_EVENT);
            break;
        case 3:
            game_mode_set(GAME_MODE_GIRL_BIRTHDAY);
            break;
        case 4:
            game_mode_set(GAME_MODE_PLAYER_BIRTHDAY);
            break;
        case 5:
            game_mode_set(GAME_MODE_GIRL_INVITATION);
            break;
        case 6:
            game_mode_set(GAME_MODE_SPORTS_FESTIVAL);
            break;
        case 7:
            game_mode_set(GAME_MODE_CULTURE_FESTIVAL_B);
            if (game_state.player_flags.h >> 0xC < 6) {
                game_mode_set(GAME_MODE_CULTURE_FESTIVAL_A);
            }
            break;
        case 8:
            game_mode_set(GAME_MODE_SCHOOL_TRIP);
            break;
        case 9:
            game_mode_set(GAME_MODE_NEW_YEAR_B);
            break;
        case 10:
            game_mode_set(GAME_MODE_SEASONAL_PARTY);
            break;
        case 11:
            game_mode_set(GAME_MODE_VALENTINE);
            break;
        case 12:
            game_mode_set(GAME_MODE_AMUSEMENT_PARK);
            break;
        case 13:
            game_mode_set(GAME_MODE_NEW_YEAR_A);
            break;
        case 14:
            game_mode_set(GAME_MODE_DATE_SCENE);
            break;
        case 15:
            game_mode_set(GAME_MODE_GIRL_ENCOUNTER);
            break;
        }
    }
}

s32 func_8013C72C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013C404();
        break;
    case 1:
        func_8013C52C();
        break;
    }
}

void func_8013C780(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_8013AD10();
        break;
    case 1:
        func_8013B3BC();
        break;
    case 2:
        func_8013BE78();
        break;
    case 3:
        func_8013C3B0();
        break;
    case 0xFF:
    default:
        func_8013C72C();
        break;
    }
    if (game_state.mode_stage != 0xFF && (game_state.pad_pressed & PAD_CIRCLE)) {
        mode_stage_next();
    }
    if (game_state.mode_stage != 0 && (game_state.pad_pressed & PAD_CROSS)) {
        mode_stage_set((game_state.mode_stage - 1) & 0xFF);
    }
    hud_screen_frame_draw(1);
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFA8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFB4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFBC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFC4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFCC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFD4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFE0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFE8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFF0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013CFF8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D000);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D008);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D010);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D01C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D024);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/debug_game_data_editors.rodata", D_8013D02C);
