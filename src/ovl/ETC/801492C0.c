#include "common.h"
#include "ovl/ETC.h"

void func_801492C0(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_801498BC();
        break;
    case 1:
        func_8014A6D4();
        break;
    case 2:
        func_8014A9A4();
        break;
    case 3:
        func_8014AB5C();
        break;
    case 4:
        func_8014AB88();
        break;
    }
    cursor_update();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    hud_screen_frame_draw(0);
    dec_bg_draw(get_last_gamen_mode());
    func_8014988C();
    hud_mascot_update();
}

s32 func_80149394(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    actor_clear_upper();
    back_clear_switch(1);
    tpage_buf_clear();
    sprite_tables_clear();
    icon_disp_switch(0);
    parameter_show_init();
    parameter_disp_switch(1);
    message_window_init();
    text_system_reset();
    message_window_set_title("就職・進学");
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    message_window_title_show(0);
    dec_bg_draw(0);
    D_80150E94 = 0;
    D_80150EA0 = rand() % 12;
    D_80150E98 = 0;
    D_80150E9C = 0;
    mode_step_next();
}


s32 func_80149468(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (((ClubFlagBits *)&game_state.clubs[i])->unk_25 >= 3 && ((ClubFlagBits *)&game_state.clubs[i])->b30) {
            return i;
        }
    }
    return -1;
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
s32 func_801495CC(void) {
    s32 sp28;
    s32 r;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        if (game_state.player_flags_upper.b[1] != 0) {
            sndisp(D_80150954, 0, 0xF);
        } else {
            sndisp(D_801508EC, 0, 0xF);
        }
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        r = func_80149468();
        if (r != -1) {
            sp28 = r;
            x_taku_menu_set(0, 3);
            if (game_state.player_flags_upper.b[1] != 0) {
                x_taku_string_set(D_8015096C, D_80150984, D_801508BC[sp28], 3);
            } else {
                x_taku_string_set(D_80150904, D_8015091C, D_801508BC[sp28], 3);
            }
            k_sub_disp_start(3);
            game_state.menu_cursor_0 = 0;
        } else {
            x_taku_menu_set(0, 2);
            if (game_state.player_flags_upper.b[1] != 0) {
                x_taku_string_set(D_8015096C, D_80150984, 0, 2);
            } else {
                x_taku_string_set(D_80150904, D_8015091C, 0, 2);
            }
            k_sub_disp_start(2);
            game_state.menu_cursor_0 = 0;
        }
        mode_step_next();
        return;
    }
}

s32 func_8014979C(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            if (game_state.player_flags_upper.b[1] != 0) {
                mode_stage_set(2);
            } else {
                mode_stage_set(1);
            }
            break;
        case 1:
            if (game_state.player_flags_upper.b[1] != 0) {
                mode_stage_set(1);
            } else {
                mode_stage_set(2);
            }
            break;
        case 2:
            mode_stage_set(3);
            break;
        }
    }
}

void func_8014988C(void) {
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
}

s32 func_801498BC(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80149394();
        break;
    case 1:
        func_801495CC();
        break;
    case 2:
        func_8014979C();
        break;
    }
}

s32 func_80149928(void) {
    switch (game_state.substep) {
    case 0:
        game_state.player_flags_upper.b[1] = 0;
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        sndisp(&D_8015093C, 0, 0x1F);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set((s32)D_8015099C, (s32)D_801509B0, (s32)D_801509C4, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        break;
    }
}

s32 func_80149A08(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            D_80150E98 = 0;
            mode_step_next();
            break;
        case 1:
            D_80150E98 = 1;
            mode_step_next();
            break;
        case 2:
            D_80150E98 = 2;
            mode_step_next();
            break;
        }
    }
}

void func_80149AC4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150A9C[D_80150E98], 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&D_80150CFC, 0, 0xF);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}


/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
s32 func_80149BC0(void) {
    s32 r;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150D20, 0, 0x1F);
        k_sub_disp_start(2);
        r = dec_bg_cd_read(0x463B, 1);
        if (r != -1) {
            if (r == 0 || r == 1) {
                game_state.substep += 2;
                dec_bg_show_set(1, r);
                set_dec_bri(0x80);
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (D_800B594C == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x463B, 1);
            } else {
                set_dec_bri(0x80);
                game_state.substep += 1;
            }
        }
        break;
    case 2:
        parameter_disp_switch(0);
        dec_bg_draw(1);
        set_dec_bri(0x80);
        hizuke_init();
        mode_step_next();
        break;
    }
}

s32 func_80149D3C(void) {
    switch (game_state.substep) {
    case 0:
        audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
        game_state.substep += 1;
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(1, 0x205);
            mode_step_next();
        }
        break;
    }
}

s32 func_80149DD8(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(9U, 0x801E0000, 0x7D81);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            addr_init_else_sd(1);
            func_80149E68();
            mode_step_next();
        }
        break;
    }
}

void func_80149E68(void) {
    actor_clear(0x60);
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
    ((Actor *)actor_pool_96)->sequence_slot = 0;
    ((Actor *)actor_pool_96)->x = -0x50;
    ((Actor *)actor_pool_96)->y = -0x40;
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
}

void func_80149F48(void) {
    /* FAKE: dead register load from unsigned global before switch triggers OPTN zcopy:0 in cvt_pass.py (T-24129). */
    register s32 pad;
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;

    pad = game_state.substep;
    var_a1 = 0;
    switch (D_80150E94 % 3) {
    case 0:
        var_v0 = game_state.clubs[0].practice_points >> 4;
        var_v1 = game_state.param_liberal_arts.value;
        if ((var_v0 > 0) && (var_v0 >= 0x15)) {
            var_v0 = 0x14;
        }
        var_v1 += var_v0;
        if (game_state.player_flags.b[1] & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    case 1:
        var_v0 = game_state.clubs[2].practice_points >> 4;
        var_v1 = game_state.param_science.value;
        if ((var_v0 > 0) && (var_v0 >= 0x15)) {
            var_v0 = 0x14;
        }
        var_v1 += var_v0;
        if (game_state.player_flags.b[1] & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    default:
        printf("DEFAUT:");
        /* fallthrough */
    case 2:
        var_v0 = game_state.clubs[4].practice_points >> 4;
        var_v1 = game_state.param_art.value;
        if ((var_v0 > 0) && (var_v0 >= 0x15)) {
            var_v0 = 0x14;
        }
        var_v1 += var_v0;
        if (game_state.player_flags.b[1] & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    }
    if (D_80150E98 == 1) {
        var_v1 += 0x26;
    }
    if (D_80150E98 == 2) {
        var_v1 += 0x36;
    }
    D_80150E94 += 1;
    if (var_v1 < (&D_80150EA4)[(D_80150EA0 + D_80150E94) % 12]) {
        var_a1 = 1;
    }
    if (var_a1 == 0) {
        D_80150E9C += 1;
    }
    ((Actor *)actor_pool_96)->flags = 5;
    ((Actor *)actor_pool_96)->unk_08 = 0;
    ((Actor *)actor_pool_96)->sequence_slot = var_a1;
    ((Actor *)actor_pool_96)->unk_18 = 0;
    game_state.substep += 1;
}

s32 func_8014A180(void) {
    if (D_80120652 & 1) {
        game_state.mode_step_timer.w = 0;
        return;
    }
    if (D_80120658 == 0) {
        if (D_80120666 != 0) {
            audio_command(0x502);
        } else {
            audio_command(0x501);
        }
    }
    if (D_80120658++ >= 0x3D) {
        if (D_80150E94 >= 0xA) {
            game_state.substep += 1;
            return;
        }
        game_state.substep -= 1;
    }
}

s32 func_8014A258(void) {
    switch (game_state.substep) {
    case 0:
        func_80149F48();
        break;
    case 1:
        func_8014A180();
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 func_8014A2C4(void) {
    switch (D_80150E9C) {
    case 10:
        return 0;
    case 8:
    case 9:
        return 1;
    case 7:
        return 2;
    case 5:
    case 6:
        return 3;
    case 3:
    case 4:
        return 4;
    case 1:
    case 2:
    default:
        return 5;
    }
}

s32 func_8014A32C(void) {
    if (game_state.param_trivia.value + game_state.param_looks.value > 0x100) {
        return 1;
    }
    return 0;
}

void func_8014A360(void) {
    if (game_state.player_flags_upper.b[1]) {
        switch (D_80150E98) {
        case 2:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 0x20;
            } else if (func_8014A2C4() == 2 && func_8014A32C()) {
                game_state.player_flags_upper.b[1] = 0x20;
            } else {
                game_state.player_flags_upper.b[1] = 0xA0;
            }
            break;
        case 1:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 0x10;
            } else if (func_8014A2C4() == 2 && func_8014A32C()) {
                game_state.player_flags_upper.b[1] = 0x10;
            } else {
                game_state.player_flags_upper.b[1] = 0x90;
            }
            break;
        case 0:
        default:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 8;
            } else if (func_8014A2C4() == 2 && func_8014A32C()) {
                game_state.player_flags_upper.b[1] = 8;
            } else {
                game_state.player_flags_upper.b[1] = 0x88;
            }
            break;
        }
    } else {
        switch (D_80150E98) {
        case 2:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 4;
            } else {
                game_state.player_flags_upper.b[1] = 0x84;
            }
            break;
        case 1:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 2;
            } else {
                game_state.player_flags_upper.b[1] = 0x82;
            }
            break;
        case 0:
        default:
            if (func_8014A2C4() < 2) {
                game_state.player_flags_upper.b[1] = 1;
            } else {
                game_state.player_flags_upper.b[1] = 0x81;
            }
            break;
        }
    }
    printf("shinro %x \n", game_state.player_flags_upper.b[1]);
}

void func_8014A534(void) {
    u32 temp_v0;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150E60[func_8014A2C4()], 0, 0x1F);
        temp_v0 = func_8014A2C4();
        switch (temp_v0) {
        case 0:
            audio_bank_start(1, 0x204);
            break;
        case 1:
            audio_bank_start(1, 0x201);
            break;
        case 2:
            audio_bank_start(1, 0x201);
            break;
        case 3:
            audio_bank_start(1, 0x200);
            break;
        case 4:
            audio_bank_start(1, 0x203);
            break;
        case 5:
            audio_bank_start(1, 0x203);
            break;
        }
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&D_80150E78, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        func_8014A360();
        mode_stage_set(4);
        /* fallthrough */
    default:
        return;
    }
}


void func_8014A6D4(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80149928();
        return;
    case 1:
        func_80149A08();
        return;
    case 2:
        func_80149AC4();
        return;
    case 3:
        func_80149BC0();
        return;
    case 4:
        func_80149D3C();
        return;
    case 5:
        func_80149DD8();
        return;
    case 6:
        func_8014A258();
        return;
    case 7:
        func_8014A534();
        /* fallthrough */
    default:
        return;
    }
}

void func_8014A7A4(void) {
    switch (game_state.substep) {
    case 0:
        game_state.player_flags_upper.b[1] = 1;
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&D_801509D8, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(&D_801509F4, &D_80150A0C, &D_80150A24, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void func_8014A8A8(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80150B08[D_80150E98], 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&D_80150CFC, 0, 0xF);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}


void func_8014A9A4(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_8014A7A4();
        return;
    case 1:
        func_80149A08();
        return;
    case 2:
        func_8014A8A8();
        return;
    case 3:
        func_80149BC0();
        return;
    case 4:
        func_80149D3C();
        return;
    case 5:
        func_80149DD8();
        return;
    case 6:
        func_8014A258();
        return;
    case 7:
        func_8014A534();
        /* fallthrough */
    default:
        return;
    }
}

s32 func_8014AA74(void) {
    s32 temp_v0;
    s32 sp28;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        temp_v0 = func_80149468();
        sp28 = temp_v0;
        sndisp(D_80150CCC[temp_v0], 0, 0xF);
        if ((u16)game_state.clubs[sp28].practice_points >= 0x19BU) {
            game_state.player_flags_upper.b[1] = 0x40;
        } else {
            game_state.player_flags_upper.b[1] = 0xC0;
        }
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        mode_stage_set(4);
        break;
    }
}

void func_8014AB5C(void) {
    if (game_state.mode_step == 0) {
        func_8014AA74();
    }
}

void func_8014AB88(void) {
    actor_clear_upper();
    printf("shinro %x \n", game_state.player_flags_upper.b[1]);
    day_event_next(1);
}
