/* home_screens.c: src/main/8005A0B0.c; T-27059: original object [0x8005A0B0,0x80061710); command/calendar/card/settings screens plus club, rumor and Christmas/New Year prep; reuse prior reviewed m-sysmenu recommendation. Original object 0x8005A0B0-0x80061710 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#include "common.h"
#include "game.h"

void prim_gradient_rect();

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void schedule_command_screen_handler(void) {
    switch (*(u8 *)&game_state.mode_stage) {
    case 0:
        func_8005A2A8();
        break;
    case 1:
        func_8005AE60();
        break;
    case 2:
        func_8005B798();
        break;
    case 3:
        func_8005B908();
        break;
    case 4:
        func_8005D9B4();
        break;
    case 5:
        join_club();
        break;
    case 0xFF:
        func_8005A1A0();
        break;
    }
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
    hud_screen_frame_draw(0);
}

/* FAKE: u8 views of the selector and of the counter give the original's global (LOD) access: compares on $v1 and the `andi`/`or v0` copy of the increment (T-5010) */
s32 func_8005A1A0(void) {
    game_state.mode_step_timer.w += 1;
    switch (*(u8 *)&game_state.mode_step) {
    case 0:
        func_8005A37C();
        break;
    case 1:
        func_8005A560();
        break;
    case 2:
        func_8005A668();
        break;
    case 3:
        func_8005ABD0();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    cursor_update();
    if (game_state.mode_step >= 2) {
        message_window_title_update();
        if (*(u8 *)&game_state.screen_brightness < 0x80) {
            *(u8 *)&game_state.screen_brightness += 4;
        }
    }
}

void func_8005A2A8(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_8005A37C();
        break;
    case 1:
        func_8005A3E8();
        break;
    case 2:
        func_8005A668();
        break;
    case 3:
        func_8005ABD0();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    cursor_update();
    message_window_title_update();
    dec_bg_draw(get_last_gamen_mode());
}

void func_8005A37C(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_upper();
    game_state.tile_layer_enabled = 0;
    game_state.boot_hold = 0;
    set_dec_bri(0);
    text_system_reset();
    k_disp_start(2);
    mode_step_next();
}

void func_8005A3E8(void) {
    k_disp_inc();
    mode_step_next();
}

void func_8005A410(void) {
    /* FAKE: unused words preserve the 0x40 stack frame and the saved spill at sp+0x28. T-24142 */
    s32 pad[2];
    s32 i;
    s32 err;
    s32 zero;
    s32 saved;

    err = 0;
    zero = 0;
    /* FAKE: pointer view on unk_F6C emits lbu (declared s8 in header). T-24142 */
    if (*(u8 *)&game_state.icon_mode == 2) {
        for (i = 0; i < 8; i++) {
            if (((s16 *)D_80125C10)[i * 4 + 0] >= 0x7D) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 0] < -0x86) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 1] < -0x54) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 1] >= 0x55) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 0] == 0 && ((s16 *)D_80125C10)[i * 4 + 1] == 0) {
                zero++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] >= 0x7D) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] < -0x86) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 3] < -0x54) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 3] >= 0x55) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] == 0 && ((s16 *)D_80125C10)[i * 4 + 3] == 0) {
                zero++;
            }
        }
    }
    if (zero >= 2) {
        if (err != 0) {
            game_state.icon_count = 0xF;
            saved = *(u8 *)&game_state.icon_mode;
            *(u8 *)&game_state.icon_mode = 0;
            func_8006509C();
            icon_disp_switch(0);
            *(u8 *)&game_state.icon_mode = saved;
        }
    }
}

void func_8005A560(void) {
    if (game_state.substep == 0) {
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(0, 0);
            game_state.substep += 1;
        }
    } else if (game_state.substep == 1) {
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        game_state.substep += 1;
    } else if (game_state.substep == 2) {
        if (cd_read_sync_status() & 1) {
            func_80068938(game_state.year, game_state.month, 0);
            game_state.menu_mode &= 3;
            D_800B5BC8 = 0xFE;
            if (hud_mascot_choice < 0xE) {
                func_800676AC(hud_mascot_choice);
                func_8006764C(1);
            }
            mode_step_next();
        }
    }
}

/* Weekday command screen setup. The calendar reads are written from `unk_040 - 1` (the original keeps
 * that sum and adds the offsets 2 and 7 to it, T-24301). */
void func_8005A668(void) {
    GameState *gs = &game_state;
    u8 event;

    back_clear_switch(0);
    actor_clear_upper();
    sprite_tables_clear();
    gs->clear_color_r = 0;
    gs->clear_color_g = 0;
    gs->clear_color_b = 0;
    text_system_reset();
    func_80066104("平日コマンド画面");
    gnsx(&gs->player_family_name);
    parameter_show_init();
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    tpage_buf_clear();
    event = gs->month_calendar[0].marker[gs->day - 1 + 2];
    if (event == 5) {
        gs->icon_count = 0xF;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(1);
        gs->menu_cursor_0 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp("明日からテストだ）", 0, 0x1F);
    } else if (event == 8 && (gs->player_flags.h >> 12) != 0xB) {
        gs->icon_count = 0xE;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(1);
        icon_can_use_set(4, 1);
        gs->menu_cursor_0 = 4;
        if ((gs->player_flags.h >> 12) == 6 && gs->year == 0x61 && (*(u16 *)&gs->clubs[6].level & 0xF) >= 8) {
            sndisp("明日から、念願の 全国高校野球大会だ）", 0, 0x1F);
        } else {
            sndisp("明日から夏合宿だ）", 0, 0x1F);
        }
    } else if (gs->month_calendar[0].marker[gs->day - 1 + 7] == 0xC && (gs->player_flags.h >> 12) < 6) {
        if ((gs->player_flags.b[1] & 0xF) != 3) {
            gs->icon_count = 0xE;
            func_8006509C();
            func_800654D0((u8)gs->icon_count);
            func_8005AC70(1);
            icon_can_use_set(4, 1);
            gs->menu_cursor_0 = 4;
            sndisp("文化祭に向けて部活をせねば）", 0, 0x1F);
        } else {
            gs->icon_count = 0xE;
            func_8006509C();
            func_800654D0((u8)gs->icon_count);
            func_8005AC70(1);
            icon_can_use_set(7, 1);
            gs->menu_cursor_0 = 7;
            func_8005AD1C();
            sndisp("文化祭に向けて部活をせねば でも病気で身体が動かない）", 0, 0x1F);
        }
    } else if (event == 0xB) {
        gs->icon_count = 0xF;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(1);
        gs->menu_cursor_0 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp("明日から修学旅行だ）", 0, 0x1F);
    } else if (gs->player_flags_upper.h & 1) {
        gs->icon_count = 0xE;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(1);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        gs->menu_cursor_0 = 0;
        sndisp("一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((gs->player_flags.b[1] & 0xF) == 3) {
        gs->icon_count = 0xE;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(1);
        icon_can_use_set(7, 1);
        gs->menu_cursor_0 = 7;
        func_8005AD1C();
        sndisp("病気で身体が動かない）", 0, 0x1F);
    } else {
        gs->icon_count = 0xE;
        func_8006509C();
        func_800654D0((u8)gs->icon_count);
        func_8005AC70(0);
        gs->menu_cursor_0 = gs->last_command.w & 0xF;
        sndisp("明日から何をしようかな？）", 0, 0x1F);
    }
    k_disp_start(4);
    cursor_actor_init(0);
    if (gs->year == 0x5F && gs->month == 4 && gs->day == 4) {
        icon_can_use_set(0xD, 0);
    }
    menu_bar_color(0x10, 0, 0, 8);
    func_8005AB4C();
}

void func_8005AB4C(void) {
    s32 r;

    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    switch (r) {
    case 0:
    case 1:
        dec_bg_show_set(0, r);
        set_dec_bri(0x80);
        game_state.screen_brightness = 0x80;
        mode_stage_next();
        break;
    case -1:
        mode_step_next();
        break;
    }
}

void func_8005ABD0(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            game_state.screen_brightness = 0x80;
            mode_stage_set(1);
        }
    }
}

void func_8005AC70(s32 arg0) {
    s32 i;

    icon_can_use_set(0xA, 0);
    icon_can_use_set(8, 0);
    icon_can_use_set(9, 0);
    if (*(u8 *)&game_state.icon_mode == 2) {
        icon_can_use_set(0xE, 0);
    }
    if ((game_state.player_flags.h >> 0xC) == 0xB) {
        icon_can_use_set(4, 0);
    }
    if (arg0 != 0) {
        for (i = 0; i != 8; i++) {
            icon_can_use_set(i, 0);
        }
    }
}

void func_8005AD1C(void) {
    LoadSquare(0x3F0, 0x160, 0x10, 0x18, D_800C9A60);
    load_palette(D_800C9D60, 0xF, 1, 1, 0);
}

void func_8005AD70(void) {
    if ((game_state.player_flags.b[1] & 0xF) == 3) {
        sprite_quads[0].flags = 0x01000000;
        sprite_quads[0].x = -0xC;
        sprite_quads[0].y = -0xA;
        sprite_quads[0].w = 0x1E;
        sprite_quads[0].h = 0x18;
        sprite_quads[0].unk_0C = 0x1E;
        sprite_quads[0].unk_0E = 0xE0;
        sprite_quads[0].unk_0F = 0x60;
        sprite_quads[0].unk_10 = 0;
        sprite_quads[0].unk_12 = 0x1EF;
        sprite_quads[0].tint_r = 0x80;
        sprite_quads[0].tint_g = 0x80;
        sprite_quads[0].tint_b = 0x80;
        sprite_quads[0].anchor_x = 0;
        sprite_quads[0].anchor_y = 0;
        sprite_quads[0].unk_1C = 0x1000;
        sprite_quads[0].unk_1E = 0x1000;
        sprite_quads[0].unk_20 = 0;
        game_state.quad_layer[0] = 0xA;
    }
}

void func_8005AE60(void) {
    u8 state; /* T-9410: keep the selector in v1 across the compare chain. */
    if (game_state.mode_step == 0) {
        func_8005B39C();
    }
    func_8005AD70();
    state = game_state.memory_card_job;
    if (state == 1) {
        if (func_80054388() == 0x13) {
            audio_command(0x50F);
            game_state.memory_card_job = 0;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            audio_command(0x502);
            game_state.memory_card_job = 0;
            memory_card_stop();
        }
    } else if (state == 0) {
        func_80065900(0U);
    }
}

s32 func_8005AF28(void) {
    if (cursor_x < 0x22 && cursor_x >= -0xD && cursor_y < 8 && cursor_y >= -0xB) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xB1);
            func_80044774(0);
            cd_audio_request_set(0x1D000185, 0x17000397, 0);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005AFC8(void) {
    if (cursor_x < 0x50 && cursor_x >= 0x41 && cursor_y < -0x10 && cursor_y >= -0x1F) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            game_mode_set(GAME_MODE_HELP);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B040(void) {
    u32 *p;

    if ((cursor_x < 8) && (cursor_x >= -0x27) && (cursor_y < 0x24) && (cursor_y >= 0x19)) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xD1);
            func_80044774(0);
            p = D_800B5970 + ((u16)game_state.day_count & 7) * 2;
            cd_audio_request_set(p[0], p[1], 0xE);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B0F4(void) {
    u32 *p;

    if ((cursor_x < 8) && (cursor_x >= -0x27) && (cursor_y < 0x24) && (cursor_y >= 0x19)) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xD1);
            func_80044774(0);
            p = D_800B59B0 + ((u16)game_state.day_count & 7) * 2;
            cd_audio_request_set(p[0], p[1], 0xE);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B1A8(void) {
    /* FAKE: unused word homes preserve girl at sp+0x2C and frame 0x38. T-10010. */
    s32 pad[2];
    s32 girl;
    s32 variant;
    s32 *pair;

    if ((*D_8011F3FF & 0x80) && (cursor_x < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < cursor_x) && (cursor_y < (D_8011F426 + 0x30)) && (D_8011F426 < cursor_y) && (game_state.pad_pressed & PAD_CIRCLE) && ((u32) (game_state.frame_count - D_800B59F0) >= 0xF1U)) {
        game_state.current_girl = D_8011F414;
        girl = (s32) D_8011F414;
        if (get_g_zyotai_s((s32) D_8011F414) & 0x80) {
            variant = 1;
        } else {
            variant = 0;
        }
        pair = D_800B3B9C + girl * 4 + variant * 2;
        cd_audio_request_set(pair[0], pair[1], girl);
        audio_command(0x300);
        return 1;
    }
    return 0;
}

s32 func_8005B2BC(void) {
    s32 *pair;

    if ((*D_8011F3FF & 0x80) && (cursor_x < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < cursor_x) && (cursor_y < (D_8011F426 + 0x30)) && (D_8011F426 < cursor_y) && (game_state.pad_pressed & PAD_CIRCLE) && ((u32) (game_state.frame_count - D_800B59F4) >= 0xF1U)) {
        pair = D_800B37D4 + D_8011F414 * 2;
        cd_audio_request_set(pair[0], pair[1], (s32)(game_state.current_girl = D_8011F414));
        audio_command(0x300);
        return 1;
    }
    return 0;
}

void func_8005B39C(void) {
    k_disp_inc();
    parameter_show();
    hizuke_show();
    message_window_show();
    hud_icon_bar_update(0);
    cursor_update();
    message_window_title_update();
    dec_bg_draw(0);
    if (func_8005B43C() == 0 && func_8005B2BC() == 0 && func_8005B040() == 0 && func_8005AF28() == 0) {
        func_8005AFC8();
    }
}

s32 func_8005B43C(void) {
    s32 pad[3]; /* FAKE: T-25004, frame 0x38 with the result at sp+0x28 (T-24301). */
    s32 sp28;
    s32 temp_v0;
    u32 temp_v1;
    u8 temp_v0_2;

    if (((u8) game_state.icon_mode == ICON_MODE_FREE) && (game_state.pad_held & PAD_L2)) {
        return 1;
    }
    sp28 = 0;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_0 >= 0) {
            sp28 = 1;
            cd_audio_command(9, 0, 0);
            audio_command(3);
            if (func_8004480C() == 0) {
                audio_bank_start(0, 0);
            }
        }
        switch (game_state.menu_cursor_0) {
        case 4:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                audio_command(0x500);
                if ((game_state.player_flags.h >> 0xC) == 0xB) {
                    game_state.last_command.date.month = game_state.menu_cursor_0;
                    mode_stage_set(5);
                } else {
                    /* T-25039: define the call argument only on this path so
                     * the shared shift stays in $v1 until the call. */
                    temp_v1 = game_state.player_flags.h >> 0xC;
                    /* FAKE: T-25004, a second read of the halfword makes uopt keep the load in a register
                     * ($a1), so the shift costs no ugen temporary (permuter find). */
                    if (game_state.player_flags.h) {
                    }
                    temp_v0 = func_80053564(temp_v1);
                    if ((temp_v0 != -1) && game_state.girls[temp_v0].flags.f.b2) {
                        game_state.girls[temp_v0].affection += 1;
                    }
                    game_mode_set(GAME_MODE_SCHEDULE);
                    game_state.last_command.date.month = game_state.menu_cursor_0;
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                if ((game_state.menu_cursor_0 != 8) && (game_state.menu_cursor_0 != 0xA)) {
                    audio_command(0x500);
                } else {
                    audio_command(0x50D);
                }
                game_mode_set(GAME_MODE_SCHEDULE);
                game_state.last_command.date.month = game_state.menu_cursor_0;
            }
            break;
        case 12:
            audio_command(0x50D);
            mode_stage_set(2);
            break;
        case 13:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                if (game_state.memory_card_job == 0) {
                    func_8005C4CC(0);
                    audio_command(0x50D);
                    mode_stage_set(3);
                }
            }
            break;
        case 11:
            audio_command(0x50D);
            mode_stage_set(4);
            break;
        case 14:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                game_state.last_command.date.month = 0xE;
                temp_v0_2 = (&game_state.month_calendar[0].marker[1])[game_state.day];
                if (temp_v0_2 == 5) {
                    audio_command(0x500);
                    game_mode_set(GAME_MODE_SCHEDULE);
                    sndisp("明日からテストだ）", 0, 0x1F);
                } else if (temp_v0_2 == 0xB) {
                    audio_command(0x500);
                    game_mode_set(GAME_MODE_SCHEDULE);
                    sndisp("明日から修学旅行だ）", 0, 0x1F);
                } else {
                    game_mode_set(GAME_MODE_SCHEDULE);
                    game_state.last_command.date.month = 7;
                }
            }
            break;
        }
    }
    return sp28;
}

void func_8005B798(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8005B830();
        break;
    case 1:
        func_8005B8A0();
        break;
    case 2:
        func_8005B8E0();
        break;
    }
    func_80068EC0();
    parameter_show();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    cursor_update();
}
INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800AFF6C);

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800AFF78);

void func_8005B830(void) {
    text_system_reset();
    message_window_set_title(D_800AFF6C);
    set_kanji_string(-0x80, 0x32, 0, D_800AFF78, 0);
    k_disp_start(2);
    cal_sprite_init();
    func_80067E34();
    icon_disp_switch(0);
    mode_step_next();
}

void func_8005B8A0(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void func_8005B8E0(void) {
    mode_stage_set(0);
    mode_step_set(2);
}

void func_8005B908(void) {
    u8 temp;
    temp = (u8) game_state.mode_step;
    switch (temp) {
    case 0x0:
        func_8005BCEC();
        break;
    case 0x1:
        func_8005BE8C();
        break;
    case 0x2:
        func_8005C414();
        break;
    case 0x3:
        func_8005C5BC();
        break;
    case 0x4:
        func_8005CA94();
        break;
    case 0x5:
        func_8005CC40();
        break;
    case 0x6:
        func_8005CF6C();
        break;
    case 0x7:
        func_8005D448();
        break;
    case 0x8:
        func_8005D56C();
        break;
    case 0x9:
        func_8005D804();
        break;
    case 0xA0:
        func_8005C14C();
        break;
    case 0xB0:
        func_8005D31C();
        break;
    case 0xC0:
        func_8005C968();
        break;
    case 0xE0:
        func_8005BAE0();
        break;
    case 0xF0:
        func_8005BC38();
        break;
    case 0xFF:
        func_8005BB84();
        break;
    }
    message_window_title_update();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    cursor_update();
}

s32 func_8005BAE0(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "データが壊れています", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            mode_step_set(6);
        }
        break;
    }
}

s32 func_8005BB84(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            func_8005C4CC(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

s32 func_8005BC38(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            func_8005C4CC(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

void func_8005BCEC(void) {
    /* FAKE: reserves the observed stack home; grouped stores preserve scheduling. T-12030. */
    s32 stack_home;
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    text_system_reset();
    func_80066104("メモリーカード");
    k_sub_reset_point_set();
    icon_disp_switch(0);
    data_save_load_class_init();
    x[0] = -0x50;
    x[1] = 0x10;
    y[0] = -0x10;
    y[1] = -0x10;
    w[0] = 0x30; w[1] = 0x30;
    h[0] = 0x14; h[1] = 0x14;
    set_kanji_string(x[0], y[0], 0xFU, "セーブ", 0);
    set_kanji_string(x[1], y[1], 0xFU, "ロード", 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, D_800B59F8, 0);
    set_kanji_string(-0x80, 0x42, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x4A, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x52, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x5A, 0U, D_800B5A1C, 0);
    k_sub_disp_start(1);
    func_80056284();
    menu_set(1, 2, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    mode_step_next();
}

s32 func_8005BE8C(void) {
    u8 *status;

    status = &game_state.memory_card_result;
    switch (game_state.substep) {
    case 0:
        if (func_80053F04() != 0) {
            if (*status == 1) {
                memory_card_stop();
                game_state.substep++;
            } else if (*status == 0xF1) {
                memory_card_stop();
                mode_step_set(0xA0);
            } else if (*status == 0xFF) {
                memory_card_stop();
                mode_step_set(0xFF);
            }
        } else if ((u32)game_state.memory_card_timer > 0x200) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 1:
        memory_card_start();
        game_state.substep++;
        break;
    case 2:
        if (func_80054140() == 2) {
            game_state.substep++;
        } else if ((u32)game_state.memory_card_timer > 0x200) {
            mode_step_set(0xFF);
        }
        break;
    case 3:
        if ((u8)D_800E8BEE != 0) {
            memory_card_stop();
            memory_card_start();
            game_state.substep++;
        } else {
            memory_card_stop();
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            mode_step_next();
        }
        break;
    case 4:
        if (func_80054284() == 3) {
            func_800552DC(0);
            func_8005C4CC(1);
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            mode_step_next();
            memory_card_stop();
        } else if (((u32)game_state.memory_card_timer > 0x200) || (*status == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    }
    k_disp_inc();
}

s32 func_8005C14C(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep++;
    default:
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep++;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "初期化していいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2, func_8006CAE0(), 0);
        k_sub_disp_start(5);
        game_state.substep++;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (func_80054108() == 1) {
                mode_step_set(0);
            } else {
                func_8005C4CC(1);
                mode_stage_set(0);
                mode_step_set(2);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep++;
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "別のメモリーカードを", 0);
        set_kanji_string(-0x80, 0x42, 1, "差し込み口１", 0);
        set_kanji_string(-0x80, 0x42, 0xF, "　　　　　　にセットして、", 0);
        set_kanji_string(-0x80, 0x52, 0xF, "○ボタンを押してください。", 0);
        k_sub_disp_start(4);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            func_8005C4CC(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

void func_8005C414(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        func_8005C4CC(1);
        mode_stage_set(0);
        mode_step_set(2);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            mode_step_set(3);
            return;
        case 1:
            mode_step_set(6);
            break;
        }
    }
}

void func_8005C4CC(s32 arg0) {
    if (arg0 != 0) {
        memcpy(D_80125C10, game_state.scratch_words, 0x40);
        D_80125C50 = game_state.scratch_words[32];
        D_80125C52 = game_state.scratch_words[33];
        hud_mascot_choice = *(u8 *)&game_state.scratch_words[34]; /* the low byte of the word stored below */
        D_80125C54 = game_state.scratch_words[35];
        D_80125C55 = game_state.scratch_words[36];
        return;
    }
    memcpy(game_state.scratch_words, D_80125C10, 0x40);
    game_state.scratch_words[32] = D_80125C50;
    game_state.scratch_words[33] = D_80125C52;
    game_state.scratch_words[34] = hud_mascot_choice;
    game_state.scratch_words[35] = D_80125C54;
    game_state.scratch_words[36] = D_80125C55;
}

void func_8005C5BC(void) {
    s32 i;
    s32 d;
    SystemData *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    text_system_reset();
    func_80066104("データセーブ");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32 - 124;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_reset_point_set();
    work = &system_data;
    for (i = 1; i != 15; i++) {
        if (work->slot_status[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        game_state.menu_cursor_1 = D_800E7D36 - 1;
        game_state.save_slot = (s8) D_800E7D36;
        mode_step_set(4);
        func_800696DC(0, 0);
        func_8005C7C0();
    } else if (i == 0xF) {
        func_8005C7C0();
        mode_step_set(4);
    } else {
        game_state.save_slot = i;
        mode_step_set(0xC0);
    }
    func_8005C7FC();
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B00FC);

void func_8005C7C0(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B00FC, 0);
    k_sub_disp_start(1);
}

void func_8005C7FC(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&system_data; i < 0xE; i++) {
        if (p[5] == 0) {
            prim_gradient_rect((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            prim_gradient_rect((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void func_8005C968(void) {
    switch (*(u8 *)&game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (game_state.memory_card_timer >= 0x201U || game_state.memory_card_result == 0xD0) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            mode_step_set(4);
            func_800696DC(0, 0);
            func_8005C7C0();
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_step_set(0xFF);
        }
        break;
    }
    func_8005C7FC();
}

/* The menu handler of ETC func_80144DF0 with the cancel arm first (T-24301). */
void func_8005CA94(void) {
    s32 pad[2]; /* FAKE: T-24301, with `pad2` reproduces the 0x40 frame and the slots of status/slot */
    u8 *status;
    s32 slot;
    s32 pad2; /* FAKE: T-24301, see pad */

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    slot = game_state.menu_cursor_1 + 1;
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        func_8005C4CC(1);
        mode_stage_set(0);
        mode_step_set(2);
        game_state.memory_card_job = 0;
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_1 != -1) {
            status = &system_data.slot_status[slot];
            if (*status == 0) {
                /* FAKE: T-24301 (as T-24209), byte view store + plain re-read promote unk_1118 to v0 */
                ((u8 *)&game_state)[0x1118] = slot;
                if (func_80054694(game_state.save_slot) == 1) {
                    func_80054704(game_state.save_slot);
                    *status = 0xFF;
                    memory_card_start();
                    mode_step_next();
                } else {
                    mode_step_set(0xF0);
                }
            } else if (slot == game_state.save_slot) {
                memory_card_start();
                mode_step_next();
            } else {
                game_state.save_slot = slot;
                if ((u8) D_800E8BEE != 0) {
                    /* FAKE: T-24301 (as T-24209), the repeated view store keeps unk_1118 promoted in both arms */
                    ((u8 *)&game_state)[0x1118] = slot;
                    D_800E7D36 = game_state.save_slot;
                    func_800696DC(0, 0);
                    game_state.menu_cursor_1 = D_800E7D36 - 1;
                } else {
                    mode_step_set(0xC0);
                }
            }
        }
    }
    k_disp_inc();
    func_8005C7FC();
}

/* FAKE: u8 view of the switch selector keeps the compares on the global ($v1), as in the original (T-5010, T-24111) */
s32 func_8005CC40(void) {
    s32 temp_v0;
    u32 temp_hi;
    u8 temp_a0;
    s32 *temp_v1_2;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "上書きしてもいいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, func_8006CAE0(), 0);
        k_sub_disp_start(5);
        game_state.substep = game_state.substep + 1;
        break;
    case 1:
        temp_v0 = game_state.pad_pressed;
        if (temp_v0 & 0x20) {
            game_state.substep = game_state.substep + 1;
        } else if (temp_v0 & 0x40) {
            mode_step_set(3);
            temp_a0 = game_state.save_slot;
            if (system_data.slot_status[temp_a0] == 0xFF) {
                system_data.slot_status[temp_a0] = 0;
            }
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "データセーブ中です", 0);
        if ((u8) D_800E8BEE != 0) {
            set_kanji_string(-0x80, 0x42, 1U, "アイコンの点滅が終わるまで", 0);
            set_kanji_string(-0x80, 0x52, 1U, "電源を切らないで下さい", 0);
        }
        k_sub_disp_start(3);
        game_state.substep = game_state.substep + 1;
        break;
    case 3:
        if (func_80054590() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep = game_state.substep + 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            temp_hi = (u8) game_state.current_girl % 13U;
            temp_v1_2 = (temp_hi * 2) + D_800B39C4;
            cd_audio_request_set(temp_v1_2[0], temp_v1_2[1], (s32) temp_hi);
            audio_command(0x300);
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 4:
        if ((u8) D_800E8BEE != 0) {
            memory_card_start();
            game_state.memory_card_job = 1;
            D_800E7D36 = game_state.save_slot;
            game_state.save_slot = 0;
            func_80054E6C();
            func_8005C4CC(1);
            mode_stage_set(0);
            mode_step_set(2);
        } else {
            func_8005C4CC(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
    func_8005C7FC();
}

void func_8005CF6C(void) {
    s32 i;
    s32 d;
    SystemData *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    text_system_reset();
    func_80066104("データロード");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_reset_point_set();
    work = &system_data;
    for (i = 1; i != 15; i++) {
        if (work->slot_status[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        game_state.menu_cursor_1 = D_800E7D36 - 1;
        mode_step_next();
        game_state.save_slot = (s8) D_800E7D36;
        func_800696DC(0, 1);
        func_8005D174();
    } else if (i == 0xF) {
        func_8005D174();
        mode_step_next();
    } else {
        game_state.save_slot = i;
        mode_step_set(0xB0);
    }
    func_8005D1B0();
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0194);

void func_8005D174(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B0194, 0);
    k_sub_disp_start(1);
}

void func_8005D1B0(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&system_data; i < 0xE; i++) {
        if (p[5] == 0) {
            prim_gradient_rect((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            prim_gradient_rect((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

void func_8005D31C(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_800696DC(0, 1);
            mode_step_set(7);
            func_8005D174();
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_step_set(0xFF);
        }
        break;
    }
    func_8005D1B0();
}

s32 func_8005D448(void) {
    /* FAKE: two-word local reserves the observed sp+0x2C spill home. T-12030. */
    s32 stack_home[2];
    s32 selection;

    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    selection = game_state.menu_cursor_1 + 1;
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (D_800E7D14[selection] != 0) {
            if (selection == (u8) game_state.save_slot) {
                memory_card_start();
                mode_step_next();
            } else {
                game_state.save_slot = (s8) selection;
                if ((u8) D_800E8BEE != 0) {
                    D_800E7D36 = (u8) game_state.save_slot;
                    func_800696DC(0, 1);
                } else {
                    mode_step_set(0xB0);
                }
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        func_8005C4CC(1);
        mode_stage_set(0);
        mode_step_set(2);
    }
    k_disp_inc();
    func_8005D1B0();
}

void func_8005D56C(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 2U, "データロード中です", 0);
        set_kanji_string(-0x80, 0x42, 2U, "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        game_state.substep += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            game_state.substep += 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            game_state.substep += 1;
        } else {
            mode_step_set(0xE0);
        }
        break;
    case 3:
        k_sub_reset();
        func_800696DC(0, 1);
        set_kanji_string(-0x80, 0x32, 1U, "このデータの続きをプレイしますか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, func_8006CAE0(), 0);
        k_sub_disp_start(0xB);
        game_state.substep += 1;
        break;
    case 4:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0x501);
            game_state.substep += 1;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            mode_step_set(6);
        }
        break;
    case 5:
        if (func_80055AFC(0) == 1) {
            mode_step_next();
        } else {
            mode_step_set(0xE0);
        }
        break;
    }
    func_8005D1B0();
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original; the s32 return keeps the switch temporary copy in $v0. T-5010 */
s32 func_8005D804(void) {
    switch (*(u8 *)&game_state.substep) {
    default:
        game_state.substep += 1;
        break;
    case 0:
        if (game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            game_state.substep += 1;
        } else if (game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            game_state.substep += 1;
        } else {
            audio_command(0xB1);
            game_state.substep = 0xC0;
        }
        break;
    case 1:
        audio_command(5);
        audio_command(0xB1);
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        game_state.substep += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            func_80041878();
            func_80041F48();
            game_state.event_log_count = 0;
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            message_disp_switch(0);
            func_8006764C(0);
            audio_bank_start(0, 0);
            game_mode_set(GAME_MODE_NAME_ENTRY);
        }
        break;
    case 0xFF:
        message_window_title_show(0);
        game_mode_set(GAME_MODE_COMMAND_SCREEN);
        mode_stage_set(0xFF);
        func_800674B0();
        break;
    }
}

void func_8005D9B4(void) {
    switch (*(u8 *)&game_state.mode_step) { /* FAKE: scalar byte view of the switch selector. T-11170 */
    case 0x0:
        system_settings_init();
        break;
    case 0x1:
        func_8005E018();
        break;
    case 0x10:
        func_8005E150();
        break;
    case 0x11:
        func_8005E2C0();
        break;
    case 0x12:
        func_8005E40C();
        break;
    case 0x20:
        func_8005E454();
        break;
    case 0x21:
        func_8005E5C8();
        break;
    case 0x22:
        func_8005E7A4();
        break;
    case 0x30:
        func_8005E7F0();
        break;
    case 0x31:
        func_8005E924();
        break;
    case 0x32:
        func_8005E9EC();
        break;
    case 0x33:
        func_8005EA38();
        break;
    case 0x34:
        func_8005EB74();
        break;
    case 0x35:
        func_8005EC78();
        break;
    case 0x36:
        func_8005ECB8();
        break;
    case 0x37:
        func_8005EE00();
        break;
    case 0x38:
        func_8005EF04();
        break;
    case 0x39:
        func_8005EF44();
        break;
    case 0x3A:
        func_8005F09C();
        break;
    case 0x3B:
        func_8005F22C();
        break;
    case 0x40:
        func_8005F26C();
        break;
    case 0x41:
        func_8005F478();
        break;
    case 0x42:
        func_8005F6E4();
        break;
    case 0x50:
        func_8005F730();
        break;
    case 0x51:
        func_8005FA20();
        break;
    case 0x52:
        func_8005FDA0();
        break;
    case 0x60:
        func_8005FDEC();
        break;
    case 0x61:
        func_8005FF68();
        break;
    case 0x62:
        func_80060104();
        break;
    case 0x70:
        func_80060178();
        break;
    case 0x71:
        func_800603C0();
        break;
    case 0x72:
        func_80060504();
        break;
    case 0xFF:
        func_80060150();
        break;
    }
    message_window_title_update();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    cursor_update();
}

s32 system_settings_init(void) {
    s32 i;
    s32 available;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];

    text_system_reset();
    message_window_set_title("システム設定画面");
    icon_disp_switch(0);
    data_save_load_class_init();
    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
    }
    func_8005DEA0(-1);
    available = func_8005DDB4();
    if (available == 0) {
        w[6] = 0;
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "システム設定を変更します", 0);
    menu_set(0, 8, x, y, w, h);
    game_state.menu_cursor_0 = (s8)game_state.menu_cursor_0 % 8;
    k_sub_disp_start(0);
    mode_step_next();
}

/* FAKE: the no-op index mask preserves the GameState loop base.
 * T-9560; original indexing expression unknown. */
s32 func_8005DDB4(void) {
    typedef struct TailFlags {
        /* 0x00 */ u8 pad00[0x13];
        /* 0x13 */ u8 first;
        /* 0x14 */ u8 flags[12];
    } TailFlags; /* size 0x20; observed byte view */
    TailFlags *tail;
    s32 count;
    s32 index;

    count = 0;
    if (((TailFlags *)D_800E7D11)->first != 0) {
        count = 1;
    }
    if (game_state.girl_cleared[0] != 0) {
        count++;
    }
    tail = (TailFlags *)D_800E7D11;
    for (index = 0; index < 12; index++) {
        if (tail->flags[index] != 0) {
            count++;
        }
        if (game_state.girl_cleared[(index + 1) & 0xFFFFFFFF] != 0) {
            count++;
        }
    }
    return count;
}

s32 func_8005DEA0(s32 arg0) {
    s32 i;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];
    u8 colour[8];

    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
        if (i == arg0) {
            colour[i] = 4;
        } else {
            colour[i] = 3;
        }
    }
    set_kanji_string(x[0], y[0], colour[0], "文字表示速度", 0);
    set_kanji_string(x[1], y[1], colour[1], "アイコンモード", 0);
    set_kanji_string(x[2], y[2], colour[2], "マウス設定", 0);
    set_kanji_string(x[3], y[3], colour[3], "メニューモード", 0);
    set_kanji_string(x[4], y[4], colour[4], "カーソル速度", 0);
    set_kanji_string(x[5], y[5], colour[5], "音声モード", 0);
    if (func_8005DDB4() != 0) {
        set_kanji_string(x[6], y[6], colour[6], "マスコット", 0);
    }
    set_kanji_string(x[7], y[7], colour[7], "設定終了", 0);
}

void func_8005E018(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        mode_step_set(2);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            mode_step_set(0x10);
            break;
        case 1:
            mode_step_set(0x20);
            break;
        case 2:
            if (game_state.input_device == INPUT_MOUSE) {
                mode_step_set(0x30);
            }
            break;
        case 3:
            mode_step_set(0x40);
            break;
        case 4:
            mode_step_set(0x50);
            break;
        case 5:
            mode_step_set(0x60);
            break;
        case 6:
            mode_step_set(0x70);
            break;
        case 7:
            mode_step_set(0xFF);
            break;
        }
    }
}

s32 func_8005E150(void) {
    s32 i = 0;
    s32 speed;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    text_system_reset();
    message_window_set_title("文字速度");
    func_8005DEA0(0);
    if (i) {} /* FAKE: empty test splits the live range of i. T-12030 */
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "遅い", 0);
    set_kanji_string(x[1], y[1], 3U, "普通", 0);
    set_kanji_string(x[2], y[2], 3U, "速い", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "文字出力のスピードを変更します", 0);
    menu_set(1, 3, x, y, w, h);
    speed = get_k_speed();
    switch (speed) {
    default:
    case 0:
        game_state.menu_cursor_1 = 2;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    case 3:
        game_state.menu_cursor_1 = 0;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005E2C0(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを遅くしました", 0);
            k_speed_set(3);
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを普通にしました", 0);
            k_speed_set(1);
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを速くしました", 0);
            k_speed_set(0);
            mode_step_next();
            break;
        }
    }
}

void func_8005E40C(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 0;
    }
}

void func_8005E454(void) {
    u8 sel;
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("アイコンモード");
    func_8005DEA0(1);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "ＴＹＰＥ‐Ａ", 0);
    set_kanji_string(x[1], y[1], 3, "ＴＹＰＥ‐Ｂ", 0);
    set_kanji_string(x[2], y[2], 3, "フリー", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "アイコンの位置の設定をします", 0);
    menu_set(1, 3, x, y, w, h);
    sel = game_state.icon_mode;
    switch (sel) {
    default:
    case 0:
        game_state.menu_cursor_1 = 0;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    case 2:
        game_state.menu_cursor_1 = 2;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

/* the u8 views: the original reads these two bytes unsigned, the struct declares them s8 (T-8080) */
void func_8005E5C8(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ａにしました", 0);
            game_state.icon_mode = ICON_MODE_TYPE_A;
            func_8006509C();
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ｂにしました", 0);
            game_state.icon_mode = ICON_MODE_TYPE_B;
            func_8006509C();
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "フリーにしました", 0);
            if (*(u8 *)&game_state.icon_mode != 2) {
                game_state.icon_count = 0xF;
            }
            game_state.icon_mode = ICON_MODE_FREE;
            func_800654D0(*(u8 *)&game_state.icon_count);
            mode_step_next();
            break;
        }
    }
}

void func_8005E7A4(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 1;
    }
}

void func_8005E7F0(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];
    s32 i;

    text_system_reset();
    message_window_set_title("マウス設定");
    func_8005DEA0(2);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "決定ボタン", 0);
    set_kanji_string(x[1], y[1], 3, "クリックモード", 0);
    set_kanji_string(x[2], y[2], 3, "クリックスピード", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "マウス使用時の各種設定を選択します", 0);
    menu_set(1, 3, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005E924(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            mode_step_set(0x33);
            return;
        case 1:
            mode_step_set(0x36);
            return;
        case 2:
            mode_step_set(0x39);
            break;
        }
    }
}

void func_8005E9EC(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 2;
    }
}

void func_8005EA38(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    text_system_reset();
    message_window_set_title("決定ボタン設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "右ボタンで決定", 0);
    set_kanji_string(x[1], y[1], 3, "左ボタンで決定", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "決定ボタンを選択します", 0);
    menu_set(1, 2, x, y, w, h);
    if (game_state.confirm_button == 0 || game_state.confirm_button != 1) {
        game_state.menu_cursor_1 = 1;
    } else {
        game_state.menu_cursor_1 = 0;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005EB74(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "右ボタンで決定にしました", 0);
            game_state.confirm_button = 1;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "左ボタンで決定にしました", 0);
            game_state.confirm_button = 0;
            mode_step_next();
            break;
        }
    }
}

void func_8005EC78(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void func_8005ECB8(void) {
    u8 sel; /* FAKE: copy of the unsigned byte (the field is s8) kept in $v0 as the original does (T-5010) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("クリックモード設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "シングルクリック", 0);
    set_kanji_string(x[1], y[1], 3, "ダブルクリック", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックモードを設定します", 0);
    menu_set(1, 2, x, y, w, h);
    sel = game_state.click_mode;
    switch (sel) {
    case 0:
        game_state.menu_cursor_1 = 0;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005EE00(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "シングルクリックにしました", 0);
            game_state.click_mode = 0;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "ダブルクリックにしました", 0);
            game_state.click_mode = 1;
            mode_step_next();
            break;
        }
    }
}

void func_8005EF04(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void func_8005EF44(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    text_system_reset();
    message_window_set_title("クリックスピード設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "初期状態", 0);
    set_kanji_string(x[1], y[1], 3, "変更", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックスピードを設定します", 0);
    set_kanji_string(-0x80, 0x42, 0, "変更する場合、変更にカーソルを", 0);
    set_kanji_string(-0x80, 0x52, 0, "合わせて、ダブルクリックして下さい", 0);
    menu_set(1, 2, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    D_801230E4 = 0;
    k_sub_disp_start(0);
    mode_step_next();
}

/* FAKE: scalar-byte view lets IDO keep the clamp value in v0 across stores. T-12030. */
void func_8005F09C(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (((game_state.confirm_button != 0) && (game_state.mouse_buttons & MOUSE_RIGHT)) || ((game_state.confirm_button == 0) && (game_state.mouse_buttons & MOUSE_LEFT))) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを初期状態にしました", 0);
            *(u8 *)&game_state.double_click_interval = 0x20;
            mode_step_next();
            break;
        case 1:
            if (D_801230E4 != 0) {
                *(u8 *)&game_state.double_click_interval = 0xFF - D_801230E4;
                if (*(u8 *)&game_state.double_click_interval < 0x10) {
                    *(u8 *)&game_state.double_click_interval = 0x10;
                }
                if (*(u8 *)&game_state.double_click_interval > 0x40) {
                    *(u8 *)&game_state.double_click_interval = 0x40;
                }
                D_801230E4 = 0;
                k_sub_reset();
                set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを変更しました", 0);
                mode_step_next();
                break;
            }
            D_801230E4 = 0xFF;
            break;
        }
    }
}

void func_8005F22C(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void func_8005F26C(void) {
    s32 pad; /* FAKE: unused local, the original frame has 4 more bytes above the arrays (T-3330) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("メニューモード設定");
    func_8005DEA0(3);
    for (i = 0; i < 4; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "１：マウスライク", 0);
    set_kanji_string(x[1], y[1], 3, "２：メニューＡ", 0);
    set_kanji_string(x[2], y[2], 3, "３：メニューＢ", 0);
    set_kanji_string(x[3], y[3], 3, "４：１＋２", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "コントローラでの、メニュー間の", 0);
    set_kanji_string(-0x80, 0x42, 0, "（２択・３択・アイコンなど）", 0);
    set_kanji_string(-0x80, 0x52, 0, "カーソルの動かし方を設定します", 0);
    menu_set(1, 4, x, y, w, h);
    switch (game_state.menu_mode) {
    default:
    case MENU_MODE_A:
        game_state.menu_cursor_1 = 1;
        break;
    case MENU_MODE_MOUSE_LIKE:
        game_state.menu_cursor_1 = 0;
        break;
    case MENU_MODE_B:
        game_state.menu_cursor_1 = 2;
        break;
    case MENU_MODE_MOUSE_PLUS_A:
        game_state.menu_cursor_1 = 3;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005F478(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_A;
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＢ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_B;
            mode_step_next();
            return;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク＋メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_MOUSE_PLUS_A;
            mode_step_next();
            break;
        }
    }
}

void func_8005F6E4(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 3;
    }
}

/* s32 with no return, like the sibling menus (func_8005FDEC). */
s32 func_8005F730(void) {
    s32 pad[2]; /* FAKE: T-24301, the frame has 8 more bytes above the arrays (as func_8005F26C) */
    s32 i;
    s16 x[5];
    s16 y[5];
    s16 w[5];
    s16 h[5];

    text_system_reset();
    message_window_set_title("カーソル速度");
    func_8005DEA0(4);
    for (i = 0; i < 5; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    if (D_800E7D2C >= 9 || game_state.girl_cleared[8] >= 2) {
        set_kanji_string(x[0], y[0], 3U, "超遅い", 0);
        set_kanji_string(x[1], y[1], 3U, "遅い", 0);
        set_kanji_string(x[2], y[2], 3U, "普通", 0);
        set_kanji_string(x[3], y[3], 3U, "速い", 0);
        set_kanji_string(x[4], y[4], 3U, "超速い", 0);
    } else {
        set_kanji_string(x[0], y[0], 3U, "遅い", 0);
        set_kanji_string(x[1], y[1], 3U, "少し遅い", 0);
        set_kanji_string(x[2], y[2], 3U, "普通", 0);
        set_kanji_string(x[3], y[3], 3U, "少し速い", 0);
        set_kanji_string(x[4], y[4], 3U, "速い", 0);
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "カーソルの移動速度を設定します", 0);
    menu_set(1, 5, x, y, w, h);
    switch (game_state.cursor_speed) {
    default:
    case 1:
        game_state.menu_cursor_1 = 0;
        break;
    case 2:
        game_state.menu_cursor_1 = 1;
        break;
    case 3:
        game_state.menu_cursor_1 = 2;
        break;
    case 4:
        game_state.menu_cursor_1 = 3;
        break;
    case 6:
        game_state.menu_cursor_1 = 4;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005FA20(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (*(u8 *)&game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "遅くしました", 0);
            }
            game_state.cursor_speed = 1;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し遅くしました", 0);
            }
            game_state.cursor_speed = 2;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            set_kanji_string(-0x80, 0x42, 0, "ふつうにしました", 0);
            game_state.cursor_speed = 3;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し速くしました", 0);
            }
            game_state.cursor_speed = 4;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 4:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "速くしました", 0);
            }
            game_state.cursor_speed = 6;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        }
    }
}

void func_8005FDA0(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 4;
    }
}

s32 func_8005FDEC(void) {
    s32 i;
    s32 mode;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    text_system_reset();
    message_window_set_title("音声モード");
    func_8005DEA0(5);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "常にＯＮ", 0);
    set_kanji_string(x[1], y[1], 3U, "常にＯＦＦ", 0);
    set_kanji_string(x[2], y[2], 3U, "×ボタンでカット", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "音声のモードを設定します", 0);
    menu_set(1, 3, x, y, w, h);
    mode = (u8)game_state.voice_mode;
    switch (mode) {
    case 1:
        game_state.menu_cursor_1 = 0;
        break;
    case 0:
        game_state.menu_cursor_1 = 1;
        break;
    default:
    case 2:
        game_state.menu_cursor_1 = 2;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8005FF68(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＮにしました", 0);
            game_state.voice_mode = VOICE_ON;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＦＦにしました", 0);
            game_state.voice_mode = VOICE_OFF;
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "×ボタンでカットにしました", 0);
            game_state.voice_mode = VOICE_CANCELABLE;
            mode_step_next();
            break;
        }
    }
}

void func_80060104(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 5;
    }
}

void func_80060150(void) {
    mode_stage_set(0);
    mode_step_set(2);
}

void func_80060178(void) {
    s32 i;
    s32 d;
    s32 pad; /* FAKE: unused local reserves the third home word above the arrays (frame 0xE8); real source unknown. T-17005 */
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];

    text_system_reset();
    message_window_set_title("マスコット");
    func_8005DEA0(6);
    d = 3;
    for (i = 0; i < 14; i++) {
        x[i] = (i % d) * 0x2B - 8;
        y[i] = (i / d) * 0x10 - 0x40;
        w[i] = 0x28;
        h[i] = 0x10;
        if (i == 13) {
            w[i] = 0x46;
            set_kanji_string(x[i], y[i], 5, "表示しない", 0);
        } else if (system_data.flags[i] != 0 || game_state.girl_cleared[i] != 0) {
            set_kanji_string(x[i], y[i], 3, girl_short_name_table[i], 0);
        } else {
            w[i] = 0;
        }
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "マスコットを選びます", 0);
    menu_set(1, 14, x, y, w, h);
    if (hud_mascot_choice == 0 || hud_mascot_choice >= 14) {
        game_state.menu_cursor_1 = 0;
    } else {
        game_state.menu_cursor_1 = hud_mascot_choice;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

s32 func_800603C0(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.menu_cursor_1 != -1) {
        func_800676AC(0);
        if (system_data.flags[game_state.menu_cursor_1] != 0 || game_state.girl_cleared[game_state.menu_cursor_1] != 0) {
            func_800676AC(game_state.menu_cursor_1 + 1);
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if ((game_state.pad_pressed & PAD_CIRCLE) && (game_state.menu_cursor_1 != -1)) {
        if (game_state.menu_cursor_1 == 0xD) {
            mode_step_set(0);
        } else if ((system_data.flags[game_state.menu_cursor_1] != 0) || (game_state.girl_cleared[game_state.menu_cursor_1] != 0)) {
            func_800676AC(0);
            func_800676AC(game_state.menu_cursor_1 + 1);
            mode_step_next();
        }
    }
}

void func_80060504(void) {
    mode_step_set(0);
    game_state.menu_cursor_0 = 6;
}

void join_club(void) {
    switch (game_state.mode_step) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        join_club_exit();
        break;
    }
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    cursor_update();
}

void join_club_init(void) {
    message_window_title_show(0);
    icon_disp_switch(0);
    text_system_reset();
    club_list_menu();
    mode_step_next();
}

s32 join_club_select(void) {
    menu_check(1, (s32) cursor_x, (s32) cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        if (game_state.game_mode == 0x34) {
            mode_step_set(2);
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_1 == 0xB) {
            mode_stage_set(0);
            if (game_state.game_mode == 0x34) {
                mode_step_set(2);
            }
        } else if (game_state.menu_cursor_1 != -1) {
            ((NibbleView *)&game_state.player_flags.b[1])->high = game_state.menu_cursor_1;
            mode_step_next();
        }
    }
}

/* FAKE: scalar-byte selector view keeps the switch index in v1; s32 fall-through
 * preserves its v0 copy. T-12030. */
s32 join_club_message(void) {
    u8 buf[0x44];

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        strcpy(buf, club_name_table[game_state.player_flags.h >> 0xC]);
        strcat(buf, "部に入部した。");
        sndisp(buf, 0, 0xFF);
        k_sub_disp_start(1);
        game_state.substep++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        if ((game_state.player_flags.h >> 0xC) >= 6) {
            k_sub_reset();
            gnsx("部長");
            set_kanji_string(-0x56, 0x32, 0, "「毎月", 0);
            set_kanji_string(-0x2C, 0x32, 1, "第３日曜日", 0);
            set_kanji_string(0x1A, 0x32, 0, "は、", 0);
            set_kanji_string(-0x48, 0x42, 0, "クラブの日なので、", 0);
            set_kanji_string(-0x48, 0x52, 1, "休まない", 0);
            set_kanji_string(-0x10, 0x52, 0, "ように。", 0);
            talk_voice_play(0x0D000011, 0x0300001F, -1, 0xE);
            game_state.substep++;
        } else {
            game_state.substep = 0xFF;
        }
        break;
    case 3:
        talk_text_step();
        break;
    case 4:
        game_state.substep = 0xFF;
        break;
    case 0xFF:
        mode_step_next();
        break;
    }
}

void join_club_exit(void) {
    actor_clear_upper();
    game_mode_set(GAME_MODE_SCHEDULE);
}

void func_80060958(void) {
    s32 r;

    text_system_reset();
    message_window_set_title("自宅");
    gnsx(game_state.player_family_name);
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    cursor_actor_init(0);
    func_8006509C();
    icon_disp_switch(1);
    schedule_command_icons_lock_to_record();
    icon_can_use_set(0, 0);
    icon_can_use_set(game_state.last_command.w & 0xF, 1);
    tpage_buf_clear();
    sndisp("最近女の子の間で 変な噂が流れているらしい…）", 0, 0x1F);
    k_disp_start(1);
    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    if (r != -1) {
        if (r == 0 || r == 1) {
            dec_bg_show_set(0, r);
            set_dec_bri(0x80);
            mode_stage_next();
        }
    } else {
        mode_step_next();
    }
    k_disp_start(2);
}

void func_80060A84(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            game_state.screen_brightness = 0x80;
            mode_stage_set(1);
        }
    }
}

s32 func_80060B24(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80060958();
        break;
    case 1:
        func_80060A84();
        break;
    }
}

void func_80060B78(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    default:
    case 1:
        mode_step_next();
        return;
    }
}
void uwasa_exit0(void) {
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

s32 uwasa0(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80060B78();
        break;
    case 1:
        uwasa_exit0();
        break;
    }
}

void uwasa_main(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80060B24();
        break;
    case 1:
        uwasa0();
        break;
    }
    hud_icon_bar_update(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    dec_bg_draw(0);
    hud_screen_frame_draw(1);
}
s32 pre_xmas_init(void) {
    switch (game_state.mode_step) {
    case 0:
        text_system_reset();
        actor_clear_upper();
        sprite_tables_clear();
        func_80066104("自宅");
        gnsx(game_state.player_family_name);
        parameter_show_init();
        parameter_show_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(1);
        icon_disp_switch(0);
        tpage_buf_clear();
        sndisp("今日は伊集院の家で、 クリスマスパーティだ。 会場に行こうかな）", 0, 0x1F);
        k_disp_start(6);
        cursor_actor_init(0);
        func_8005AB4C();
        return;
    case 1:
        func_8005ABD0();
        return;
    }
}

s32 pre_xmas(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        break;
    case 1:
        game_mode_set(GAME_MODE_NEW_YEAR_A);
        break;
    }
}

void func_80060DF8(void) {
    switch (game_state.mode_stage) {
    case 0:
        pre_xmas_init();
        break;
    case 1:
        pre_xmas();
        break;
    }
    parameter_show();
    hizuke_show();
    message_window_show();
    cursor_update();
    message_window_title_update();
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
    dec_bg_draw(0);
    hud_screen_frame_draw(1);
}

s32 func_80060EA0(void) {
    s32 pad[2]; /* FAKE: unused, gives the 0x48 frame and the n/tbl slots. T-7020 */
    u8 tbl[12];
    s32 i;
    s32 n;

    n = 0;
    get_h_yuukou_table(tbl);
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (tbl[i] >= 0x4DU && game_state.girls[i].flags.f.met) n++;
    return n;
}

void func_80061044(void) {
    if (func_80060EA0() != 0) {
        set_dec_bri(0);
        text_system_reset();
        func_80057640(0xA0);
        actor_clear_upper();
        sprite_tables_clear();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        func_8006764C(0);
        audio_command(0xC1);
        icon_disp_switch(0);
        tpage_buf_clear();
        cursor_actor_init(0);
        audio_bank_load_start(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
        mode_step_next();
        return;
    }
    set_dec_bri(0);
    game_mode_set(GAME_MODE_NEW_YEAR_B);
}

void pre_syogatu_init1(void) {
    s32 t;

    if (audio_bank_load_poll() == 1) {
        t = dec_bg_cd_read(0x4167, 1);
        if (t != -1) {
            if (t == 0 || t == 1) {
                dec_bg_show_set(1, t);
                set_dec_bri(0x80);
                mode_step_next();
                mode_step_next();
            }
        } else {
            mode_step_next();
        }
    }
    func_80057710();
}

void pre_syogatu_init2(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        set_dec_bri(0x80);
        mode_step_next();
    }
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0B64);

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0B6C);

void pre_syogatu_init3(void) {
    text_system_reset();
    message_window_set_title(D_800B0B64);
    k_sub_reset_point_set();
    sndisp(D_800B0B6C, 0, 0xFF);
    audio_command(0x603);
    k_disp_start(6);
    mode_stage_next();
}

s32 pre_syogatu_init(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80061044();
        break;
    case 1:
        pre_syogatu_init1();
        break;
    case 2:
        pre_syogatu_init2();
        break;
    case 3:
        pre_syogatu_init3();
        break;
    }
}

void pre_syogatu0(void) {
    s32 temp_v0;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp((s32) "ん？ 誰だろう）", 0, 0x1F);
        game_state.substep += 1;
    default:
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx("〒配達");
        sndisp((s32) "年賀状でーす。", 0, 0xE);
        k_sub_disp_start(1);
        talk_voice_play(0x08000005, 0x03000009, -1, 0xE);
        game_state.substep += 1;
        break;
    case 3:
        talk_text_step();
        break;
    case 4:
        temp_v0 = dec_bg_cd_read(0x4144, 1);
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                dec_bg_show_set(1, temp_v0);
                set_dec_bri(0x80U);
                game_state.substep = 6;
            }
        } else {
            game_state.substep = 5;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            set_dec_bri(0x80U);
            game_state.substep += 1;
        }
        break;
    case 6:
        mode_step_next();
        break;
    }
}

s32 pre_syogatu1(void) {
    s32 var_s0;
    s32 var_s1;
    /* FAKE: two words recover the observed frame without moving the buffers. T-17005 */
    s32 stack_home[2];
    u8 sp16C[0xC];
    u8 sp6C[0x100];
    u8 sp5C[0x10];

    switch (game_state.substep) {
    case 0:
        get_h_yuukou_table(sp16C);
        k_sub_reset();
        strcpy(sp6C, "やった！ ");
        var_s1 = 0;
        for (var_s0 = 0; var_s0 < 0xB; var_s0++) {
            if ((sp16C[var_s0] >= 0x4DU) && (game_state.girls[var_s0].flags.f.met)) {
                if (var_s1 != 0) {
                    strcat(sp6C, "と、 ");
                }
                get_g_name(sp5C, var_s0);
                strcat(sp6C, sp5C);
                var_s1 += 1;
            }
        }
        strcat(sp6C, "から、 年賀状をもらったラッキー。）");
        gnsx(game_state.player_family_name);
        sndisp(sp6C, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        game_mode_set(GAME_MODE_NEW_YEAR_B);
        return;
    }
}

s32 func_80061634(void) {
    switch (game_state.mode_step) {
    case 0:
        pre_syogatu0();
        break;
    case 1:
        pre_syogatu1();
        break;
    }
}

void func_80061688(void) {
    switch (game_state.mode_stage) {
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        func_80061634();
        break;
    }
    hizuke_show();
    message_window_show();
    cursor_update();
    message_window_title_update();
    dec_bg_draw(1);
    hud_screen_frame_draw(1);
}
