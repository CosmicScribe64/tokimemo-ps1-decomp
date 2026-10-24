#include "common.h"
#include "ovl/ETC.h"

void func_8014ABD0(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_8014AE20();
        break;
    case 1:
        func_8014B750();
        break;
    case 2:
        func_8014B978();
        break;
    case 3:
        func_8014BB94();
        break;
    case 4:
        func_8014C018();
        break;
    case 5:
        func_8014C250();
        break;
    case 6:
        func_8014C2A4();
        break;
    }
    cursor_update();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    hud_screen_frame_draw(0);
    dec_bg_draw(1);
    hud_mascot_update();
}

void func_8014ACB4(void) {
    s32 temp_v0;
    s32 sp28;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    actor_clear_upper();
    back_clear_switch(1);
    tpage_buf_clear();
    sprite_tables_clear();
    cursor_actor_init(0);
    hizuke_init();
    icon_disp_switch(0);
    parameter_disp_switch(0);
    message_window_init();
    text_system_reset();
    message_window_title_show(0);
    set_dec_bri(0);
    temp_v0 = dec_bg_cd_read(0x4055, 1);
    sp28 = temp_v0;
    if (temp_v0 != -1) {
        if ((temp_v0 == 0) || (temp_v0 == 1)) {
            mode_step_next();
            mode_step_next();
            dec_bg_show_set(1, sp28);
        }
    } else {
        mode_step_next();
    }
}

void func_8014AD94(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        mode_step_next();
    }
}

void func_8014ADDC(void) {
    message_window_set_title("進路指導");
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    set_dec_bri(0x80);
    mode_stage_next();
}

s32 func_8014AE20(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014ACB4();
        break;
    case 1:
        func_8014AD94();
        break;
    case 2:
        func_8014ADDC();
        break;
    }
}

s32 func_8014AE8C(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150EE0, 0, 0xF);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 func_8014AF28(void) {
    s32 index;
    s32 count;

    D_80151870 = 0;
    D_80151874 = 0;
    D_80151878 = 0;
    count = 0;
    D_8015186C = 0;
    /* The nine contiguous parameter records start at GameState +0xFC. */
    for (index = 1; index != 8; index++) {
        printf("param %d %d \n", index, ((ParamValue *)((u8 *)&game_state + 0xFC))[index].value);
        if (((ParamValue *)((u8 *)&game_state + 0xFC))[index].value >= 0x65) {
            count++;
        }
    }
    if (game_state.param_liberal_arts.value < 0x65) {
        D_8015186C += 1;
    }
    if (game_state.param_science.value < 0x65) {
        D_8015186C++;
    }
    if (game_state.param_art.value < 0x65) {
        D_8015186C++;
    }
    if (game_state.param_athletics.value < 0x65) {
        D_80151870 += 1;
    }
    if (game_state.param_guts.value < 0x65) {
        D_80151870 += 2;
    }
    if (game_state.param_trivia.value < 0x65) {
        D_80151874 += 1;
    }
    if (game_state.param_looks.value < 0x65) {
        D_80151878 += 1;
    }
    return count;
}

s32 func_8014B0D0(void) {
    s32 var_s7;
    u8 *var_s1;
    u8 temp_v0;
    s32 var_s2;
    u8 sp70[16];
    s16 sp50[16];
    s32 var_s0;

    func_80053418(sp70, sp50);
    var_s2 = 0x3C;
    var_s7 = 0;
    for (var_s0 = 1, var_s1 = &sp70[1]; var_s0 != 11; var_s0++, var_s1++) {
        printf("%d %d \n", var_s0, *var_s1);
        temp_v0 = *var_s1;
        if ((var_s2 < temp_v0) && game_state.girls[var_s0].flags.f.met) {
            var_s7 = var_s0;
            var_s2 = temp_v0;
        }
    }
    return var_s7;
}

void func_8014B1B4(void) {
    GameState *g = &game_state;
    s32 sp28;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->substep) {
    case 0x0:
        temp_v0 = func_8014AF28();
        if (temp_v0 == 7) {
            g->substep = 0x10;
            return;
        }
        if (temp_v0 == 0) {
            g->substep = 0x20;
            return;
        }
        g->substep = 0x30;
        return;
    case 0x10:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&D_80151674, 0, 0xF);
        g->substep += 1;
        return;
    case 0x11:
        xa_wait();
        return;
    case 0x12:
        mode_stage_next();
        return;
    case 0x20:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&D_801516F0, 0, 0xF);
        g->substep += 1;
        return;
    case 0x21:
        xa_wait();
        return;
    case 0x22:
        g->substep = 0x40;
        return;
    case 0x30:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&D_801516BC, 0, 0xF);
        g->substep += 1;
        return;
    case 0x31:
        xa_wait();
        return;
    case 0x32:
        k_sub_reset();
        if (D_8015186C == 0) {
            g->substep += 2;
        } else if (D_8015186C == 1) {
            gnsx(g->player_family_name);
            sndisp(&D_80151738, 0, 0xF);
            g->substep += 1;
        } else if (D_8015186C == 2) {
            gnsx(g->player_family_name);
            sndisp(&D_80151758, 0, 0xF);
            g->substep += 1;
        } else {
            gnsx(g->player_family_name);
            sndisp(&D_80151778, 0, 0xF);
            g->substep += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x33:
        xa_wait();
        return;
    case 0x34:
        k_sub_reset();
        if (D_80151870 == 0) {
            g->substep += 2;
        } else if (D_80151870 == 1) {
            gnsx(g->player_family_name);
            sndisp(&D_801517B4, 0, 0xF);
            g->substep += 1;
        } else if (D_80151870 == 2) {
            gnsx(g->player_family_name);
            sndisp(&D_801517D4, 0, 0xF);
            g->substep += 1;
        } else {
            gnsx(g->player_family_name);
            sndisp(&D_80151794, 0, 0xF);
            g->substep += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x35:
        xa_wait();
        return;
    case 0x36:
        k_sub_reset();
        if (D_80151874 != 0) {
            gnsx(g->player_family_name);
            sndisp(&D_801517F4, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->substep += 1;
            return;
        }
        g->substep += 2;
        return;
    case 0x37:
        xa_wait();
        return;
    case 0x38:
        k_sub_reset();
        if (D_80151878 != 0) {
            gnsx(g->player_family_name);
            sndisp(&D_80151810, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->substep += 1;
            return;
        }
        g->substep += 2;
        return;
    case 0x39:
        xa_wait();
        return;
    case 0x3A:
        g->substep = 0x40;
        return;
    case 0x40:
        temp_v0_2 = func_8014B0D0();
        sp28 = temp_v0_2;
        if (temp_v0_2 == 0) {
            g->substep = 0x50;
            return;
        }
        k_sub_reset();
        gnsx(g->player_family_name);
        g->current_girl = (s8) sp28;
        sndisp(&D_80151844, 0, 0xF);
        g->substep += 1;
        return;
    case 0x41:
        xa_wait();
        return;
    case 0x42:
        g->substep = 0x50;
        return;
    case 0x50:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&D_8015182C, 0, 0xF);
        g->substep += 1;
        return;
    case 0x51:
        xa_wait();
        return;
    case 0x52:
        mode_stage_next();
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014B750(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014AE8C();
        break;
    case 1:
        func_8014B1B4();
        break;
    }
}

void func_8014B7A4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&D_80150EFC, 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&D_80150F38, 0, 0x1F);
        game_state.substep += 1;
        /* fallthrough */
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(&D_80150F58, &D_80150F6C, &D_80150F80, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}

s32 func_8014B8CC(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            mode_stage_set(4);
            break;
        case 1:
            mode_stage_set(5);
            break;
        case 2:
            mode_stage_set(3);
            break;
        }
    }
}

s32 func_8014B978(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014B7A4();
        break;
    case 1:
        func_8014B8CC();
        break;
    }
}

s32 func_8014B9CC(void) {
    s32 sp34; /* FAKE: unused local, the original frame is 8 bytes larger (T-3330 slot rule); source unknown. T-6040 */
    s32 sp30;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150F98, 0, 0xF);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        menu_girl_taku_set(0, &sp30);
        k_disp_start(sp30);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        break;
    }
}

void func_8014BA8C(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if ((game_state.pad_pressed & PAD_CIRCLE) && game_state.menu_cursor_0 != -1) {
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        sndisp(D_80151358[game_state.menu_cursor_0], 0, 0xF);
        mode_step_next();
    }
}

s32 func_8014BB34(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        break;
    case 1:
        mode_stage_set(2);
        game_state.substep = 2;
        break;
    }
}

s32 func_8014BB94(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014B9CC();
        break;
    case 1:
        func_8014BA8C();
        break;
    case 2:
        func_8014BB34();
        break;
    }
}

void func_8014BC00(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp((u8 *) &D_80150F58, 0, 0xF);
        game_state.player_flags_upper.b[1] = 0;
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&D_80151398, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(D_801514A4[func_8014BDE0() & 3], 0, 0xE);
        talk_voice_play((s32) &D_801514B4, (s32) &D_801514D4, func_8014BDE0() & 3, 0xE);
        game_state.substep += 1;
        return;
    case 5:
        talk_text_step();
        return;
    case 6:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80151664[func_8014BDE0() & 3], 0, 0xF);
        game_state.substep += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        mode_stage_set(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014BDE0(void) {
    s32 pad; /* FAKE: unused local, makes the frame 0x38 and puts the spill at sp+0x2C as in the original; real source unknown. T-4010 */
    s32 sum;
    s32 n;

    sum = 0;
    n = 0;
    if (game_state.param_liberal_arts.value >= 0x79) {
        n = 1;
        sum = game_state.param_liberal_arts.value - 0x78;
    }
    if (game_state.param_science.value >= 0x79) {
        n += 1;
        sum += game_state.param_science.value;
        sum -= 0x78;
    }
    if (game_state.param_art.value >= 0x79) {
        n += 1;
        sum += game_state.param_art.value;
        sum -= 0x78;
    }
    if ((n == 3) && (sum >= 0xC9)) {
        n = 3;
    } else if (n != 0) {
        n -= 1;
    }
    printf("thl %d \n", n);
    return 3 - n;
}

s32 func_8014BE98(void) {
    s32 n;

    n = -func_8014BDE0() * 2 + 6;
    if (game_state.param_trivia.value >= 0x3D) {
        n += 1;
    }
    if (game_state.param_looks.value >= 0x3D) {
        n += 1;
    }
    if (n >= 7) {
        return 0;
    }
    if (n >= 5) {
        return 1;
    }
    if (n >= 3) {
        return 2;
    }
    return 3;
}

/* The fallthrough s32 form preserves the selector/result copies. T-10020. */
s32 func_8014BF2C(void) {
    s32 result;
    GameState *state;

    state = &game_state;
    switch (state->substep) {
    case 0:
        result = dec_bg_cd_read(0x408D, 1);
        if (result != -1) {
            if (result == 0 || result == 1) {
                game_state.substep += 2;
                dec_bg_show_set(1, result);
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            game_state.substep += 1;
        }
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 func_8014C018(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014BF2C();
        break;
    case 1:
        func_8014BC00();
        break;
    }
}

void func_8014C06C(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp((u8 *) &D_80150F6C, 0, 0xF);
        game_state.player_flags_upper.b[1] = 1;
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&D_80151398, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(D_801515E4[func_8014BE98() & 3], 0, 0xE);
        talk_voice_play((s32) &D_801515F4, (s32) &D_80151614, func_8014BE98() & 3, 0xE);
        game_state.substep += 1;
        return;
    case 5:
        talk_text_step();
        return;
    case 6:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80151664[func_8014BE98() & 3], 0, 0xF);
        game_state.substep += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        mode_stage_set(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014C250(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8014BF2C();
        break;
    case 1:
        func_8014C06C();
        break;
    }
}

void func_8014C2A4(void) {
    day_event_next(1);
}
