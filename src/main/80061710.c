#include "common.h"
#include "game.h"

void func_80061710(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

void func_80061790(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = 0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0x80;
    D_8012276E = 0x80;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = 0;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0xFF;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0x64;
    D_80122784 = 0;
    D_80122788 = 0;
    D_8012278C = 0x80;
    D_8012278D = 0xFF;
    D_8012278E = 0x80;
    func_8009AD70(2, &D_80122780);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

void func_800618B0(void) {
    TaiikuBig *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_801227A0; do {
        func_80099E30(0, p);
        p->unk20 = 0xFA0;
        p->unk0 = 0;
        p++;
    } while (p != D_80122CA0);
}

/* HYPOTHESIS: the TMD walk of initModelingData_init (func_8009B3C0 = GsMapModelingData, func_8009B430 = GsLinkObject4)
 * on the model loaded at 0x801F0000: map the data, check the object count word (13), then link the 16 and 13 objects.
 * The address is an integer (`m`), not a pointer: a pointer is folded into `lui; lw 8(t6)` by uopt, and the `u32`
 * view keeps the count compare's `li at,13` apart from the divisor. T-17010 */
s32 func_8006190C(void) {
    s32 m;
    s32 r;
    s32 i;

    m = 0x801F0004;
    func_8009B3C0(m);
    m += 4;
    if (*(u32 *)m != 0xD) {
        return 0;
    }
    m += 4;
    for (i = 0; i < 16; i++) {
        func_8009B430(m, (u8 *)&D_80122640[i * 4], 0);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0;
    }
    r = rand();
    for (i = 0; i < 13; i++) {
        func_8009B430(m, (u8 *)&D_80122640[i * 4], (r + i) % 13);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0;
    }
}

/* Setup steps selected by game_state.substep: 0 resets the display state, 1 starts the CD
 * read of the model data to 0x801F0000, 2 duplicates its first sector into the next twelve and reads the
 * textures to 0x80180000, 3 loads palettes and texture pages, sets up actor 96 and the 13 models. T-25119 */
void func_80061A3C(void) {
    s32 i;
    u8 *p;
    s32 tbl[8];
    /* FAKE: unused; two words below tbl give the original 136-byte frame with tbl at sp+0x60. T-25119 */
    s32 pad[2];

    switch (game_state.substep) {
    case 0:
        D_800B3CFC = 0;
        D_800B3D00 = 0;
        D_800B3CF8 = 0;
        tpage_buf_clear();
        actor_clear_range(0);
        dec_bg_reset();
        func_80041168(0xFE);
        text_system_reset();
        message_window_title_show(0);
        back_clear_switch(1);
        actors_draw_enable(1);
        *(s32 *)&game_state.menu_hit[3] = 0x9F;
        cursor_show(0);
        game_state.clear_color_g = 0;
        game_state.clear_color_r = 0;
        game_state.screen_brightness = 0;
        game_state.clear_color_b = 0;
        snd_command(0x7F);
        snd_bank_load_start(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        game_state.substep++;
        break;
    case 1:
        if (snd_bank_load_poll() == 1) {
            snd_bank_start(1, 0);
            cd_read_sectors(1, 0x801F0000, 0xBE13);
            game_state.substep++;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            /* FAKE: the pointer cast keeps the source apart from the integer 0x801F0000 of case 1; one
             * shared constant makes as1 put that call's `lui a1` before its `li a0`. T-25119 */
            for (p = (u8 *)0x801F0800; p != (u8 *)0x801F6800; p += 0x800) {
                memcpy(p, (void *)0x801F0000, 0x800);
            }
            game_state.substep++;
            cd_read_sectors(0xD6, 0x80180000, 0xA0CC);
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            for (i = 1; i < 14; i++) {
                load_palette(0x801CC200, (i + 15) & 0xFF, 1, 2, i);
            }
            load_palette(0x801CC200, 0x1F, 1, 2, 0xF);
            load_palette(0x801CC200, 0x1E, 1, 2, 0xE);
            for (i = 0; i < 6; i++) {
                load_tpage_buf_lock(i + 0x15, 0x80180000 + i * 0x4000, 1);
            }
            for (i = 0; i < 4; i++) {
                load_tpage_buf_lock(i + 0x35, 0x80198000 + i * 0x4000, 1);
            }
            for (i = 0; i < 3; i++) {
                load_tpage_buf_lock(i + 0x25, 0x801A8000 + i * 0x4000, 1);
            }
            for (i = 0; i < 8; i++) {
                tbl[i] = 0x801B4000 + i * 0x4000;
            }
            func_8004A1A0(tbl, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette(0x801CC000, 0xF, 1, 3, 0);
            actor_clear(0x60);
            ((Actor *)actor_pool)[96].unk_01 = 0xD;
            ((Actor *)actor_pool)[96].flags = 1;
            ((Actor *)actor_pool)[96].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[96].draw_flags = 0xA4;
            ((Actor *)actor_pool)[96].frame_table = (u8 *)0x801EAB3C;
            ((Actor *)actor_pool)[96].sequence_table = (u8 *)0x801EAB6C;
            ((Actor *)actor_pool)[96].tpage_table = (u8 *)0x801EAB20;
            ((Actor *)actor_pool)[96].sequence_slot = 0;
            ((Actor *)actor_pool)[96].unk_04 = 8;
            ((Actor *)actor_pool)[96].unk_05 = 0x80;
            ((Actor *)actor_pool)[96].x = -0xA0;
            ((Actor *)actor_pool)[96].y = -0x78;
            D_801220E1 = 0;
            D_801220E0 = 0;
            game_state.bg_layer_order[0] = 0x7F;
            D_801220E2 = 0;
            back_clear_switch(0);
            srand(game_state.timer_ticks);
            draw2d3d(1, 1);
            func_80061710();
            func_80061790();
            func_800618B0();
            /* FAKE: an empty test of the int result ends the block after the call; otherwise its
             * return register v0 is reserved there and the copy pointers below take v1/a0 instead of
             * v0/v1. T-25119 */
            if (func_8006190C()) {
            }
            /* FAKE: positions 1..13 of the D_801230D0 work area, reached through its first word as in
             * func_80062948 (its other words are not declared as an aggregate yet); the first copy is a
             * statement of its own, the loop of 12 is unrolled by four. T-25119 */
            (&D_801230D0)[1] = D_800B5A40[0];
            for (i = 1; i < 13; i++) {
                (&D_801230D0)[i + 1] = D_800B5A40[i];
            }
            main_stage_next();
        }
        break;
    }
}

void func_80061EFC(void) {
    u32 c;
    c = game_state.screen_brightness;
    if (c < 0x80U) {
        D_801220E1 = c;
        D_801220E2 = c;
        D_801220E0 = c;
        game_state.screen_brightness = c + 1;
    }
    if ((game_state.substep == 0) && ((u32)game_state.step_timer.w >= 0x259U)) {
        main_step_next();
    }
    if (game_state.pad_pressed & 0x860) {
        /* Chain written as element stores: one li for all three (T-9210). */
        (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0xFF;
        back_clear_switch(1);
        func_80059048();
        game_state.screen_brightness = 0x80;
        main_stage_next();
    }
}

void func_80061FC4(void) {
    func_8004AC18((s32)((u32)game_state.step_timer.w >> 1));
    if (((u32)game_state.step_timer.w >> 1 >= 0xFFU) || (game_state.pad_pressed & 0x860)) {
        bg_layers = 0x80000000;
        func_80059048();
        cursor_actor_init(0);
        cursor_show(0);
        back_clear_switch(1);
        /* Chain written as element stores: one li for all three (T-9210). */
        (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0xFF;
        actor_clear(0x60);
        D_80120651 = 0xD;
        D_80120652 = 0x40;
        D_80120688 = 0x01000000;
        D_80120653 = 0xA4;
        D_8012065C = (u8 *)0x801EAB3C;
        D_80120660 = (u8 *)0x801EAB6C;
        D_80120684 = (u8 *)0x801EAB20;
        D_80120654 = 8;
        D_80120655 = 0x80;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        if (game_state.pad_pressed & 0x860) {
            main_stage_next();
            return;
        }
        main_step_next();
    }
}

void func_8006211C(void) {
    func_8004AC18(0xFF - game_state.step_timer.w);
    if ((u32)game_state.step_timer.w >= 0xFFU) {
        main_stage_next();
    } else if (game_state.pad_pressed & 0x860) {
        main_stage_next();
    }
}

void func_8006218C(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        func_80061EFC();
        break;
    case 1:
        func_80061FC4();
        break;
    case 2:
        func_8006211C();
        break;
    }
    func_80062948();
}

void func_80062210(void) {
    s16 pad[4]; /* FAKE: unused pad grows the frame to the original 0x80; real source unknown. T-17010 */
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];
    s32 i;
    s32 j;

    /* FAKE: same-line stores per row let as1 schedule sh in h, w, x, y order while li loads y, x, w, h. T-24111 */
    y[0] = 0x42; x[0] = -0x30; w[0] = 0xA0; h[0] = 0x14;
    y[1] = 0x52; x[1] = -0x30; w[1] = 0xA0; h[1] = 0x14;
    menu_set(0, 2, x, y, w, h);
    D_801230D0 = 0;
    (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0xFF;
    sprite_tables_clear();
    (&game_state.screen_brightness)[0] = 0x80;
    cursor_actor_init(0);
    cursor_show(1);
    func_800623E4();
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[i + 0x60].unk_01 = 0xD;
        ((Actor *)actor_pool)[i + 0x60].flags = 0x40;
        ((Actor *)actor_pool)[i + 0x60].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[i + 0x60].draw_flags = 0xA4;
        ((Actor *)actor_pool)[i + 0x60].frame_table = (u8 *)0x801EAB3C;
        ((Actor *)actor_pool)[i + 0x60].sequence_table = (u8 *)0x801EAB6C;
        ((Actor *)actor_pool)[i + 0x60].tpage_table = (u8 *)0x801EAB20;
        ((Actor *)actor_pool)[i + 0x60].unk_18 = j;
        ((Actor *)actor_pool)[i + 0x60].unk_04 = 8;
        ((Actor *)actor_pool)[i + 0x60].unk_05 = 0x80;
        ((Actor *)actor_pool)[i + 0x60].x = -0xA0;
        ((Actor *)actor_pool)[i + 0x60].y = -0x78;
    }
    D_801206BE = -0x6E;
    D_80120702 = -0x78;
    D_801206BA = -0xAA;
    D_801206FE = -0xAA;
    main_step_next();
}

void func_800623E4(void) {
    if (game_state.input_device != INPUT_MOUSE) {
        cursor_y = D_801230D0 * 16 + 0x46;
        cursor_x = 0x40;
        switch (game_state.pad_pressed & 0x5000) {
        case 0x1000:
            game_state.step_timer.w = 0;
            D_801230D0 = 0;
            break;
        case 0x4000:
            game_state.step_timer.w = 0;
            D_801230D0 = 1;
            break;
        }
    } else {
        menu_check(0, cursor_x, cursor_y);
        menu_cursor_update(0);
        switch (game_state.menu_cursor_0) {
        case 0:
            game_state.step_timer.w = 0;
            D_801230D0 = 0;
            break;
        case 1:
            game_state.step_timer.w = 0;
            D_801230D0 = 1;
            break;
        }
    }
    if (D_801230D0 != 0) {
        D_801206AC = 2;
        D_801206F0 = 3;
    } else {
        D_801206AC = 1;
        D_801206F0 = 4;
    }
}

void func_80062524(void) {
    func_800623E4();
    if (game_state.pad_pressed & 0x820) {
        cursor_show(0);
        main_step_next();
        snd_command(0x501);
    }
    if ((u32)game_state.step_timer.w >= 0x259U || (game_state.pad_pressed & PAD_SELECT)) {
        actor_clear_range(0);
        snd_command(0x7F);
        cursor_show(0);
        main_state_set(MODE_PROLOGUE);
    }
}

void func_800625C0(void) {
    u8 *p = actor_pool + D_801230D0 * 0x44;

    p[0x19C7] |= 0x40;
}

void func_800625F4(void) {
    func_800625C0();
    if ((u32)game_state.step_timer.w >= 0x21U) {
        main_stage_next();
    }
}

void func_80062634(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        func_80062210();
        return;
    case 1:
        func_80062524();
        return;
    case 2:
        func_800625F4();
        return;
    }
}
void func_800626B0(void) {
    func_8004AC18(0xFF);
    switch (*(u32 *)&D_801230D0) {
    case 0:
        main_state_set(MODE_NAME_ENTRY);
        break;
    case 1:
        main_state_set(MODE_OPTION);
        if (D_800B3220 == 1) {
            if (game_state.pad_held & PAD_RIGHT) {
                main_state_set(MODE_OPTION_2);
            } else if (game_state.pad_held & PAD_LEFT) {
                main_state_set(MODE_OPTION_3);
            }
        }
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80062764(void) {
    snd_command(0xD1);
    func_8004AC18(game_state.step_timer.w * 2);
    if ((u32)game_state.step_timer.w >= 0x80U) {
        func_80048DD0(0);
        cursor_actor_init(0);
        cursor_show(0);
        sprite_tables_clear();
        main_step_next();
    }
}

void func_800627DC(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        func_80062764();
        return;
    case 1:
        func_800626B0();
        return;
    }
}

void mode_title(void) {
    game_state.stage_timer += 1;
    switch (game_state.stage) {
    case 0:
        func_80061A3C();
        break;
    case 1:
        func_8006218C();
        break;
    case 2:
        func_80062634();
        break;
    case 3:
        func_800627DC();
        break;
    default:
        game_reset_to_title();
        break;
    }
    if (D_800B3CFC != 0) {
        if (func_80045288() == 1) {
            if (game_state.stage >= 2) {
                cursor_show(0);
                snd_command(0x7F);
                main_state_set(MODE_PROLOGUE);
                main_stage_set(1);
                snd_command(0x501);
            }
        }
    }
}

/* Places the 13 rotating models: model i (D_801227A0[i], a GsCOORDINATE2 record) gets a local
 * matrix from a rotation that depends on the frame counter, an identity scale and a translation
 * from position word i + 1 of the D_801230D0 work area, which then moves on by 2 or 3.
 * s32 because func_8006218C calls it before this definition (implicit int). T-25119 */
s32 func_80062948(void) {
    s32 i;
    s32 j;
    SVECTOR rot;
    MATRIX m;
    MATRIX s;
    VECTOR scale;
    s32 v;
    s32 x;

    for (i = 0; i < 13; i++) {
        j = i & 3;
        /* FAKE: `| 0` puts the counter first in the multu, as in the original; IDO's front end
         * otherwise orders the heavier (j + 2) first. T-25119 */
        rot.vz = (game_state.frame_count | 0) * (j + 2) * -2 - 2;
        rot.vx = rsin(game_state.frame_count * 10 * (j + 1)) / 12 + 0x7FD;
        rot.vy = func_800A0140(game_state.frame_count * 10 * (j + 1)) / 12 - 5;
        m = *(MATRIX *)D_80125C60;
        s = *(MATRIX *)D_80125C60;
        RotMatrix(&rot, &m);
        m.t[0] = 0;
        m.t[1] = 0;
        m.t[2] = 0;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        ScaleMatrix(&s, &scale);
        MulMatrix(&m, &s);
        /* FAKE: D_801230D0 is a work area each module lays out its own way (main_api.h declares
         * only its first word); here the positions follow that word. T-25119 */
        m.t[0] = (&D_801230D0)[i + 1];
        v = rsin((game_state.frame_count & 0xFFF) * (j + 10));
        x = (&D_801230D0)[i + 1];
        m.t[1] = v / 32 + -x / 2;
        /* FAKE: the unsigned negation keeps the original's second negu; a signed -x is shared
         * with the line above. T-25119 */
        m.t[2] = -(u32)x * 4 - 0x190;
        v = abs(rsin(game_state.frame_count));
        /* FAKE: `- (0 - p)` puts (v & 1) before the reloaded position in the addu, as in the
         * original; `+ p` is ordered position first. T-25119 */
        (&D_801230D0)[i + 1] = (v & 1) - (0 - (&D_801230D0)[i + 1]) + 2;
        *(MATRIX *)D_801227A0[i].unk4 = m;
        D_801227A0[i].unk0 = 0;
    }
}

void func_80062C28(void) {
    set_movie_offset(0, 0);
    func_80056BA8(0, 0xA7, 0x190);
    func_80088070();
    cd_audio_command(9, 0, 0);
    cd_audio_command(0xA, 0, 0);
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    func_8009C210(1);
    func_80041168(0);
    func_8009C674(0);
    back_clear_switch(1);
    main_state_set(MODE_BOOT);
}
