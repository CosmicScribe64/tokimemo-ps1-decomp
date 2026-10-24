/* settings_sound_test_and_extras.c: src/ovl/OPTION/80132000.c:557: options stage dispatcher, page input and draw, sound-test track playback and extra-mode launch; body 558: game_state.mode_stage_timer += 1; 560: switch (game_state.mode_stage) {. Original object 0x80132000-0x801389A0 (config/objects/OPTION.txt). */
#define MAIN_API_OVERRIDE_memory_card_stop /* implicit int view: the call's $v0 keeps unk_1115 in $v1 in func_80132BA8 (T-24301) */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* unprototyped view avoids 16-bit coordinate narrowing in func_80132000 (T-24134) */
#include "common.h"
#include "ovl/OPTION.h"
/* FAKE: scoped minimum aggregate view of the word and adjacent flags, based on observed +0x14 indexing. T-9540 */

void prim_gradient_rect();
s32 memory_card_stop();

void func_80132000(void) {
    if ((game_state.mode_step != 0x21) && (game_state.mode_step != 0x22) && (game_state.mode_step != 0x23)) {
        if (game_state.screen_brightness != 0) {
            /* FAKE: dead tests extend unk_110A in $v0 and promote unk_03A ($v1), D_80120676 ($v0), D_8012067A ($v1). T-24134 */
            if ((s32)game_state.mode_step) {}
            if ((s32)game_state.screen_brightness) {}
            if ((u8) D_801220E0 >= 0x7FU) {
                if (D_80120676) {}
                if (D_8012067A) {}
                prim_gradient_rect(D_80120676 + 0x18, D_8012067A + 0x10, 0x7C, 0xBC, 0xA, 0x10907, 0x83);
                dtd_on_tpage(0, 0, 0xA, 0, 0);
            }
        }
    }
}

void func_801320B0(void) {
}

void func_801320B8(void) {
}

void func_801320C0(void) {
    if ((game_state.mode_step != 0x21) && (game_state.mode_step != 0x22) && (game_state.mode_step != 0x23)) {
        if (game_state.screen_brightness != 0) {
            switch (game_state.mode_stage) {
            case 0:
                break;
            case 1:
                prim_gradient_rect(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            case 2:
            case 4:
                prim_gradient_rect(-0x28, -0x48, 0xB0, 0x80, 8, 0x10907, 0x80);
                break;
            case 3:
                prim_gradient_rect(-0x80, -0x30, 0x100, 0x80, 8, 0x10907, 0x80);
                break;
            case 5:
                prim_gradient_rect(8, -0x20, 0x40, 0x80, 8, 0x10907, 0x80);
                break;
            case 6:
                prim_gradient_rect(-0x10, 0x10, 0x90, 0x50, 8, 0x10907, 0x80);
                break;
            case 7:
                if ((u8) game_state.mode_step < 0xAU) {
                    prim_gradient_rect(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                } else if ((u8) game_state.mode_step < 0x14U) {
                    prim_gradient_rect(-0x58, -8, 0xDC, 0x40, 8, 0x10907, 0x80);
                } else {
                    prim_gradient_rect(-0x90, -0x58, 0x130, 0x90, 8, 0x10907, 0x80);
                }
                break;
            case 8:
                prim_gradient_rect(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                break;
            default:
                prim_gradient_rect(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            }
            dtd_on_tpage(0, 0, 8, 0, 0);
        }
    }
}

void func_80132384(void) {
    s32 i;
    s32 n;
    s32 y;
    s16 *p;
    Arrs4x8 sp50;
    s32 pad[2]; /* FAKE: places sp50 at sp+0x50 (0xA0 frame); real source unknown. T-24140 */

    if (D_80120676 < -0xA0) {
        D_80120676 += 0xC;
    } else {
        D_80120676 = -0xA0;
    }
    func_801325A0(&sp50);
    i = 0;
    do {
        n = i + 1;
        if ((n ^ 0) != game_state.mode_stage) { /* FAKE: '^ 0' swaps the beq operand order to the original's; real source unknown. T-24140 */
            p = get_k_work(i);
            if (p[0] < -0x78) {
                p[0] += 0xC;
            } else {
                p[0] = -0x78;
            }
        } else {
            /* FAKE: 20-byte array stride (Uixa 20) and i++ prevent uopt from hoisting 20 into $s5 or strength-reducing i * 20; real source unknown. T-24140 */
            y = (s32)(*(u8 (*)[8][20])0)[i] - 0x58;
            p = get_k_work(i++);
            if (p[1] < y) {
                p[1] += 0xC;
                D_801206BE += 0xC;
                if (p[1] >= y) {
                    p[1] = y;
                    D_801206BE = sp50.b[game_state.menu_cursor_0] + 6;
                }
            } else {
                p[1] = y;
                D_801206BE = sp50.b[game_state.menu_cursor_0] + 6;
            }
        }
        i = n;
    } while (n != 8);
}

void func_801324E8(void) {
    s32 i;
    s32 n;
    s16 *p;

    if (D_80120676 >= -0x11F) {
        D_80120676 -= 8;
    }
    i = 0;
    do {
        n = i + 1;
        if ((n ^ 0) != game_state.mode_stage) { /* FAKE: '^ 0' swaps the beq operand order to the original's; real source unknown. T-4070 */
            p = get_k_work(i);
            if (p[0] >= -0xF7) {
                p[0] -= 8;
            }
        } else {
            p = get_k_work(i);
            if (p[1] >= -0x57) {
                p[1] -= 8;
                D_801206BE -= 8;
            }
        }
        i = n;
    } while (n != 8);
}

void func_801325A0(Arrs4x8 *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        p->a[i] = -0x78;
        p->b[i] = -0x58 + i * 0x14;
        p->c[i] = 0x60;
        p->d[i] = 0x14;
    }
}

void func_80132624(OptionGrid80 *grid) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if (i < 8) {
            grid->x[i] = -0x20;
            grid->y[i] = i * 20 - 0x46;
            grid->w[i] = 0x50;
            grid->h[i] = 0x14;
        } else {
            grid->x[i] = 0x30;
            grid->y[i] = i * 20 - 0xE6;
            grid->w[i] = 0x50;
            grid->h[i] = 0x14;
        }
    }
}

/* Palette cycle: four 128-word palettes at 0x801CBEF0 (RAM above the overlay, filled at load). */
#define OPTION_PALETTES ((s32 *)0x801CBEF0)

void func_801326BC(void) {
    s32 i;
    GameState *g;

    if (D_8013D300 != 0) {
        ((BgLayer *)&bg_layers)->unk_10 = game_state.screen_brightness;
        ((BgLayer *)&bg_layers)->unk_11 = game_state.screen_brightness;
        ((BgLayer *)&bg_layers)->unk_12 = game_state.screen_brightness;
        if (game_state.frame_count & 1) {
            ((BgLayer *)&bg_layers)->unk_0C += 1;
            ((BgLayer *)&bg_layers)->unk_0E -= 1;
        }
        /* The palette stores go through a pointer: uopt then keeps the `% 4` index in every
         * iteration and every statement, as the original does. */
        g = &game_state;
        D_8013D308++;
        if (D_8013D308 > 0x100) {
            for (i = 0; i < 0x80; i++) {
                g->scratch_words[i] = OPTION_PALETTES[D_8013D304 % 4 * 128 + i];
            }
            for (i = 0; i <= D_8013D308 - 0x100; i++) {
                g->scratch_words[i + 0x10] = OPTION_PALETTES[(D_8013D304 + 1) % 4 * 128 + i + 0x10];
                g->scratch_words[i + 0x38] = OPTION_PALETTES[(D_8013D304 + 1) % 4 * 128 + i + 0x38];
                if (i + 0x60 < 0x80) {
                    g->scratch_words[i + 0x60] = OPTION_PALETTES[(D_8013D304 + 1) % 4 * 128 + i + 0x60];
                }
            }
            if (D_8013D308 > 0x128) {
                D_8013D304 += 1;
                D_8013D308 = 0;
            }
            load_palette(game_state.scratch_words, 0x1A, 1, 1, 0);
        } else {
            /* FAKE: T-11900 empty `else return`: the promoted D_8013D308 is then stored back on
             * both arms after the compare instead of right after the increment. */
            return;
        }
    }
}
#undef OPTION_PALETTES

void func_801328C0(void) {
    s32 i;
    s32 j;
    u8 t; /* FAKE: single load of game_state.screen_brightness into $v0 for unk_10..unk_12; real source may use macro. T-24119 */
    void *p; /* FAKE: dead pointer stores order outer/inner induction variables in uopt bittab; real source unknown. T-24119 */

    ((BgLayer *)&bg_layers)->unk_00 = 0x01000000;
    ((BgLayer *)&bg_layers)->unk_04 = -0xA0;
    ((BgLayer *)&bg_layers)->unk_06 = -0x78;
    ((BgLayer *)&bg_layers)->unk_08 = 0x200;
    ((BgLayer *)&bg_layers)->unk_0A = 0x140;
    ((BgLayer *)&bg_layers)->unk_0C = 0;
    ((BgLayer *)&bg_layers)->unk_0E = 0;
    t = game_state.screen_brightness;
    ((BgLayer *)&bg_layers)->unk_10 = t;
    ((BgLayer *)&bg_layers)->unk_11 = t;
    ((BgLayer *)&bg_layers)->unk_12 = t;
    ((BgLayer *)&bg_layers)->unk_14 = (u32)&D_80122120[0];
    ((BgLayer *)&bg_layers)->unk_18 = 0;
    ((BgLayer *)&bg_layers)->unk_1A = 0;
    ((BgLayer *)&bg_layers)->unk_1C = 0x1000;
    ((BgLayer *)&bg_layers)->unk_1E = 0x1000;
    ((BgLayer *)&bg_layers)->unk_20 = 0;
    D_80122120[0].cellw = 0x80;
    D_80122120[0].cellh = 0x40;
    D_80122120[0].ncellw = 4;
    D_80122120[0].ncellh = 5;
    D_80122120[0].base = D_80122140[0];
    D_80122120[0].index = D_80122540[0].v;

    p = &D_80122540[0].v[i * 4];
    for (i = 0; i < 4; i++) {
        p = &D_80122140[0][j + i * 4];
        for (j = 0; j < 5; j++) {
            D_80122540[0].v[j + i * 4] = 0;
            D_80122140[0][j + i * 4].cba = func_8009ED74(0, 0x1FA);
            D_80122140[0][j + i * 4].u = 0;
            D_80122140[0][j + i * 4].v = 0;
            D_80122140[0][j + i * 4].tpage = 5;
            D_80122140[0][j + i * 4].flag = 0;
        }
    }
    game_state.bg_layer_order[0] = 0xD;
}

void func_80132A8C(void) {
    if (game_state.mode_step == 0) {
        func_80132AB8();
    }
}

void func_80132AB8(void) {
    if (game_state.screen_brightness >= 2U) {
        game_state.screen_brightness -= 2;
    }
    if (game_state.screen_brightness < 0x41U) {
        text_system_reset();
    }
    if (game_state.screen_brightness < 2U) {
        game_state.screen_brightness = 0;
        actor_clear_range(0);
        game_mode_set(GAME_MODE_TITLE);
    }
}

void func_80132B30(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132BA8();
        return;
    case 1:
        func_80133010();
        return;
    case 2:
        func_80133330();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

/* Option screen state machine (loader, CD checks). */
void func_80132BA8(void) {
    s32 pad[5]; /* FAKE: T-24301, puts `rows` at sp+0x3C in the 0x90 frame */
    Arrs4x8 rows;
    u8 r;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        game_state.options_dirty = 1;
        draw2d3d(1, 0);
        actor_draw_enable(1);
        sprite_tables_clear();
        actor_clear_upper();
        *(s32 *)&game_state.menu_hit[3] = 0xA0;
        back_clear_switch(1);
        game_state.clear_color_g = 0;
        game_state.clear_color_r = 0;
        game_state.clear_color_b = 0;
        tpage_buf_clear();
        game_state.tile_layer_enabled = 0;
        text_system_reset();
        game_state.menu_cursor_0 = 0;
        game_state.menu_cursor_1 = 0;
        cursor_actor_init(0);
        game_state.screen_brightness = 0xFF;
        D_8013D300 = 0;
        D_800B3220 = 0;
        D_801230D0 = 0;
        text_set_advance(0xE);
        func_801325A0(&rows);
        menu_set(0, 8, rows.a, rows.b, rows.c, rows.d);
        set_kanji_string(rows.a[0], rows.b[0], 0, "文字表示速度", 0);
        set_kanji_string(rows.a[1], rows.b[1], 0, "アイコンモード", 0);
        set_kanji_string(rows.a[2], rows.b[2], 0, "マウス設定", 0);
        set_kanji_string(rows.a[3], rows.b[3], 0, "メニューモード", 0);
        set_kanji_string(rows.a[4], rows.b[4], 0, "カーソル速度", 0);
        set_kanji_string(rows.a[5], rows.b[5], 0, "音声モード", 0);
        set_kanji_string(rows.a[6], rows.b[6], 0, "おまけ", 0);
        set_kanji_string(rows.a[7], rows.b[7], 0, "オプション終了", 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        ((Actor *)actor_pool)[96].unk_18 = 0;
        ((Actor *)actor_pool)[96].x = -0xA0;
        ((Actor *)actor_pool)[96].y = -0x78;
        game_state.substep = 1;
        memory_card_start();
        break;
    case 1:
        if (func_80053F04() != 0) {
            r = game_state.memory_card_result;
            if (r == 1) {
                game_state.substep += 1;
            } else if (r == 0xF1) {
                game_state.substep += 1;
            } else if (r == 0xFF) {
                game_state.substep += 1;
            }
        } else if ((u32)game_state.memory_card_timer > 0x200) {
            game_state.substep += 1;
        }
        break;
    case 2:
        memory_card_stop();
        if (game_state.memory_card_result == 1) {
            if (func_80054AF4(0) == 1) {
                game_state.substep += 1;
                memory_card_start();
            } else {
                game_state.substep = 0xFF;
            }
            if (func_80054AF4(0xF) == 1) {
                D_800B3220 = 1;
            } else {
                D_800B3220 = 0;
            }
        } else {
            game_state.substep = 0xFF;
        }
        break;
    case 3:
        if (func_80054284() == 3) {
            memory_card_stop();
            game_state.substep += 1;
        } else if ((u32)game_state.memory_card_timer > 0x200 || game_state.memory_card_result == 0xF0) {
            memory_card_stop();
            game_state.substep = 0xFF;
        }
        break;
    case 4:
        if (func_800552DC(0) == -1) {
            game_state.substep = 0xFF;
        } else {
            game_state.substep = 0xFF;
        }
        break;
    case 0xFF:
    default:
        cd_read_sectors(0x3D, 0x801B0000, 0x9EA4);
        mode_step_next();
        break;
    }
    func_8004AC18(game_state.screen_brightness);
}

/* Option screen loader: waits for the CD, sets up records 96..159 and the palettes, then fades in. */
void func_80133010(void) {
    s32 i;

    if (game_state.substep == 0) {
        if (cd_read_sync_status() & 1) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 1) {
        actor_clear_upper();
        D_8013D300 = 1;
        for (i = 96; i < 160; i++) {
            ((Actor *)actor_pool)[i].unk_01 = 5;
            ((Actor *)actor_pool)[i].flags = 0x40;
            ((Actor *)actor_pool)[i].quad_flags = 0x01000000;
            ((Actor *)actor_pool)[i].draw_flags = 0x24;
            ((Actor *)actor_pool)[i].frame_table = (u8 *)0x801C32D8;
            ((Actor *)actor_pool)[i].sequence_table = (u8 *)0x801C34E0;
            ((Actor *)actor_pool)[i].tpage_table = (u8 *)0x801C32C8;
            ((Actor *)actor_pool)[i].unk_04 = 8;
            ((Actor *)actor_pool)[i].unk_18 = 0;
            ((Actor *)actor_pool)[i].unk_43 = 0;
            ((Actor *)actor_pool)[i].unk_05 = 0x80;
            ((Actor *)actor_pool)[i].x = -0x200;
            ((Actor *)actor_pool)[i].y = -0x200;
            ((Actor *)actor_pool)[i].brightness = 0x80;
        }
        for (i = 0; i < 4; i++) {
            load_palette(0x801C34F0, (u8)(i + 0x10), 1, 1, i);
        }
        load_palette(0x801B0000, 0x1F, 1, 1, 0);
        for (i = 0; i != 4; i++) {
            load_palette(0x801CBEF0, (u8)(i + 0x1A), 1, 1, i);
        }
        load_tpage_buf_lock(5, 0x801C7CF0, 1);
        func_801328C0();
        ((Actor *)actor_pool)[96].unk_18 = 0;
        ((Actor *)actor_pool)[96].x = -0xA0;
        ((Actor *)actor_pool)[96].y = -0x78;
        ((Actor *)actor_pool)[96].draw_flags |= 0x80;
        ((Actor *)actor_pool)[96].unk_01 = 8;
        ((Actor *)actor_pool)[97].unk_18 = 0xA;
        ((Actor *)actor_pool)[97].x = -0xA0;
        ((Actor *)actor_pool)[97].y = -0x78;
        ((Actor *)actor_pool)[97].draw_flags |= 0x80;
        ((Actor *)actor_pool)[97].unk_01 = 3;
        ((Actor *)actor_pool)[106].unk_18 = 7;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0x24;
        ((Actor *)actor_pool)[106].unk_01 = 1;
        ((Actor *)actor_pool)[159].unk_18 = 7;
        ((Actor *)actor_pool)[159].x = -0xA0;
        ((Actor *)actor_pool)[159].y = -0x78;
        ((Actor *)actor_pool)[159].draw_flags = 0x24;
        ((Actor *)actor_pool)[159].unk_01 = 1;
    }
    if (game_state.substep != 0) {
        game_state.screen_brightness -= 4;
        if (game_state.screen_brightness <= 0x80) {
            game_state.screen_brightness = 0x80;
            if (game_state.substep == 1) {
                game_state.substep = 2;
            }
        }
        if (game_state.substep == 2) {
            mode_step_next();
        }
    }
    func_8004AC18(game_state.screen_brightness);
}

/* FAKE: s32 without an explicit return matches the unprototyped call at func_80132B30. T-24117 */
s32 func_80133330(void) {
    s32 pad; /* FAKE: unused 4-byte local places sp2C at sp+0x2C (0x70 frame); real source unknown. T-24117 */
    Arrs4x8 sp2C;

    menu_check(0, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(0);
    if (game_state.menu_cursor_0 == -1) {
        ((Actor *)actor_pool)[97].draw_flags &= 0xFF7F;
    } else {
        func_801325A0(&sp2C);
        ((Actor *)actor_pool)[97].draw_flags |= 0x80;
        ((Actor *)actor_pool)[97].x = sp2C.a[game_state.menu_cursor_0] + 0x30;
        /* FAKE: '^ 0' prevents uopt from sharing the scaled byte offset with sp2C.a[]; real source unknown. T-24117 */
        ((Actor *)actor_pool)[97].y = sp2C.b[game_state.menu_cursor_0 ^ 0] + 6;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            audio_command(0x501);
            mode_stage_set(1);
            break;
        case 1:
            audio_command(0x501);
            mode_stage_set(2);
            break;
        case 2:
            if (game_state.input_device == INPUT_MOUSE) {
                audio_command(0x501);
                mode_stage_set(3);
            }
            break;
        case 3:
            if (game_state.input_device == INPUT_PAD) {
                audio_command(0x501);
                mode_stage_set(4);
            }
            break;
        case 4:
            audio_command(0x501);
            mode_stage_set(5);
            break;
        case 5:
            audio_command(0x501);
            mode_stage_set(6);
            break;
        case 6:
            audio_command(0x501);
            mode_stage_set(7);
            break;
        case 7:
            audio_command(0x501);
            mode_stage_set(8);
            break;
        }
    }
    if ((D_800B3220 == 1) && ((game_state.pad_held & 0xF) == 0xF)) {
        game_mode_set(GAME_MODE_OPTION_2);
    }
    /* FAKE: 8-bit bitfield view prevents uopt from CSE-ing D_800B3220 across the untaken first branch; real source unknown. T-24117 */
    if ((((OptionByteBf *)&D_800B3220)->x == 1) && ((game_state.pad_held & 0xF0000) == 0xF0000)) {
        game_mode_set(GAME_MODE_OPTION_3);
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        game_state.menu_cursor_0 = 7;
    }
}

/* FAKE: s32 without an explicit return retains the original dead v0 loop initialization; no meaningful return is established. T-9540 */
s32 func_8013357C(void) {
    s32 i;
    Actor *p;

    for (i = 0x60; i < 0xA0; i++) {
        p = (Actor *)actor_pool + i;
        if (p->draw_flags & 0x80) {
            func_8004AE54(p->unk_18, p->x, p->y, 0xF);
        }
    }
}

void func_801335E8(void) {
    game_state.mode_stage_timer += 1;
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_stage) {
    case 0:
        func_80132B30();
        break;
    case 1:
        func_80133C4C();
        break;
    case 2:
        func_8013447C();
        break;
    case 3:
        func_80135450();
        break;
    case 4:
        func_80135DE0();
        break;
    case 5:
        func_8013661C();
        break;
    case 6:
        func_80136BC8();
        break;
    case 7:
        func_8013884C();
        break;
    case 8:
        func_80132A8C();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    func_801326BC();
    if (game_state.screen_brightness != 0) {
        func_80132000();
        func_801320B0();
    }
}

s32 func_80133718(void) {
    s32 i;
    s32 j;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original 0x90 frame and row offsets (T-4070). T-24119 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 1;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        game_state.substep = 1;
        D_80120912[0] = 0x7FFF;
        return;
    }
    D_80120912[0] = D_80120912[0] / 17 * 16;
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0; i < 3; i++) {
            a[i] = 0x10;
            b[i] = i * 0x28 - 0x58;
            c[i] = 0x28;
            d[i] = 0x10;
            j = i * 2 + 0xE;
            /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j; ((Actor *)actor_pool_96)[i + 11].x = a[i]; ((Actor *)actor_pool_96)[i + 11].y = b[i]; ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
        }
        menu_set(1, 3, a, b, c, d);
        switch (get_k_speed()) {
        case 0:
            game_state.menu_cursor_1 = 0;
            break;
        case 1:
            game_state.menu_cursor_1 = 1;
            break;
        case 2:
            game_state.menu_cursor_1 = 2;
            break;
        default:
            k_speed_set(0);
            game_state.menu_cursor_1 = 0;
            break;
        }
        mode_step_next();
    }
}

s32 func_8013394C(void) {
    s32 i;
    s32 j;
    GameState *g;

    /* one table: as1 keeps the loads after the store only when both go through actor_pool (T-9020) */
    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    /* FAKE: pointer view for unk_1094 makes uopt CSE the ILOD into $a2 while keeping move $v0, $a2 in the first switch branch delay slot. T-24082 */
    g = &game_state;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == g->menu_cursor_1) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0xD;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0xE;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (g->menu_cursor_1) {
        case 0:
            k_speed_set(0);
            game_state.options_dirty |= 0x80;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            k_speed_set(1);
            game_state.options_dirty |= 0x80;
            mode_step_next();
            audio_command(0x501);
            break;
        case 2:
            k_speed_set(3);
            game_state.options_dirty |= 0x80;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
        game_state.substep = 2;
    }
}

s32 func_80133AD8(void) {
    s32 i;

    if (game_state.substep == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        mode_stage_set(0);
        mode_step_set(2);
    }
    D_801208FB = 0;
}

s32 func_80133C4C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80133718();
        break;
    case 1:
        func_8013394C();
        break;
    case 2:
        func_80133AD8();
        break;
    }
}

void func_80133CB8(void) {
    GameState *g = &game_state;
    s32 i;
    s32 j;
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    if (g->substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 2;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        g->substep = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24119 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0x50;
                d[i] = 0x10;
                j = i * 2 + 0x16;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((Actor *)actor_pool_96)[i + 11].unk_18 = j; ((Actor *)actor_pool_96)[i + 11].x = a[i]; ((Actor *)actor_pool_96)[i + 11].y = b[i]; ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            switch ((u8)g->icon_mode % 3) {
            case 0:
            default:
                g->menu_cursor_1 = 0;
                break;
            case 1:
                g->menu_cursor_1 = 1;
                break;
            case 2:
                g->menu_cursor_1 = 2;
                break;
            }
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24119 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0((u8)g->icon_count);
            mode_step_next();
        }
    }
    func_801324E8();
}

void func_80133F50(void) {
    s32 pad; /* FAKE: unused 4-byte local reproduces the 0x40 stack frame and stack slot offsets; real source unknown. T-24117 */
    GameState *g;
    s32 i;
    s32 keep;
    s32 old;
    s32 j;

    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    g = &game_state;
    old = g->menu_cursor_1;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == g->menu_cursor_1) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x15;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x16;
        }
    }
    if (old != g->menu_cursor_1) {
        switch (g->menu_cursor_1) {
        case 0:
            keep = (u8)g->icon_mode;
            g->icon_mode = ICON_MODE_TYPE_A;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24117 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(g->icon_count = 0xE);
            g->icon_mode = keep;
            break;
        case 1:
            keep = (u8)g->icon_mode;
            g->icon_mode = ICON_MODE_TYPE_B;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24117 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(g->icon_count = 0xE);
            g->icon_mode = keep;
            break;
        }
    }
    if (g->pad_pressed & PAD_CIRCLE) {
        switch (g->menu_cursor_1) {
        case 0:
            g->icon_mode = 3;
            func_80134210();
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            g->icon_mode = 4;
            func_80134210();
            mode_step_next();
            audio_command(0x501);
            break;
        case 2:
            g->icon_mode = 5;
            func_80134210();
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (g->pad_pressed & PAD_CROSS) {
        func_80134210();
        mode_step_next();
        g->substep = 2;
    }
}

void func_80134210(void) {
    s32 i;

    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] -= 200;
    game_state.icon_count = 0xF;
    func_8006509C();
    if ((u8)game_state.icon_mode % 3U != 2) {
        game_state.icon_count = 0xE;
    }
}

void func_801342A8(void) {
    u8 mode;
    s32 i;
    s32 frame[16]; /* FAKE: observed unused 0x40-byte frame gap, T-24040. */
    Arrs4x8 rows;

    func_8006509C();
    icon_disp_switch(0);
    mode = game_state.substep;
    if (mode == 0) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep += 1;
        }
    } else if (mode == 2) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        func_801325A0(&rows);
        menu_set(0, 8, rows.a, rows.b, rows.c, rows.d);
        mode_stage_set(0);
        mode_step_set(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_8013447C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80133CB8();
        return;
    case 1:
        func_80133F50();
        return;
    case 2:
        func_801342A8();
        return;
    }
}

void func_801344E4(void) {
    s32 k;
    s32 i;
    s32 j;
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 3;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        game_state.substep = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24126 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = -0x78;
                b[i] = i * 0x28 - 0x20;
                c[i] = 0x6C;
                d[i] = 0x10;
                j = i * 2 + 0x1C;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24126 */
                ((Actor *)actor_pool_96)[i + 11].unk_18 = j; ((Actor *)actor_pool_96)[i + 11].x = a[i]; ((Actor *)actor_pool_96)[i + 11].y = b[i]; ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            for (i = 96, j = 0x22, k = -0x28; i < 98; i++, j += 2, k += 0x10) {
                ((Actor *)actor_pool)[i + 14].unk_18 = j; ((Actor *)actor_pool)[i + 14].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 14].x = 0;
                ((Actor *)actor_pool)[i + 14].y = k;
                if (game_state.confirm_button == 1) {
                    ((Actor *)actor_pool)[14].unk_18++;
                } else {
                    ((Actor *)actor_pool)[15].unk_18++;
                }
            }
            for (i = 96, j = 0, k = 0x26; i < 98; i++, j += 0x10, k += 2) {
                ((Actor *)actor_pool)[i + 16].unk_18 = k; ((Actor *)actor_pool)[i + 16].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 16].x = 0;
                ((Actor *)actor_pool)[i + 16].y = j;
                if (*(u8 *)&game_state.click_mode == 0) {
                    ((Actor *)actor_pool)[16].unk_18++;
                } else {
                    ((Actor *)actor_pool)[17].unk_18++;
                }
            }
            for (i = 96, j = 0x2A, k = 0x28; i < 98; i++, k += 0x10, j += 2) {
                ((Actor *)actor_pool)[i + 18].unk_18 = j; ((Actor *)actor_pool)[i + 18].x = 0; ((Actor *)actor_pool)[i + 18].y = k; ((Actor *)actor_pool)[i + 18].draw_flags |= 0x80;
            }
            mode_step_next();
        }
    }
    func_801324E8();
}

void func_80134804(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through actor_pool (T-9020) */
    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x1B;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x1C;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            mode_step_set(3);
            break;
        case 1:
            mode_step_set(6);
            break;
        case 2:
            if (*(u8 *)&game_state.click_mode == 1) {
                mode_step_set(9);
            }
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
        game_state.substep = 2;
    }
}

void func_8013493C(void) {
    s32 i;
    s32 frame[18]; /* FAKE: observed unused 0x48-byte frame area, T-24040. */
    Arrs4x8 rows;

    if (game_state.substep == 0) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 9; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep += 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 9; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        func_801325A0(&rows);
        menu_set(0, 8, rows.a, rows.b, rows.c, rows.d);
        mode_stage_set(0);
        mode_step_set(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_80134B6C(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x60); real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10 - 0x28;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 2, a, b, c, d);
    mode_step_next();
    func_801320C0();
}

void func_80134C00(void) {
    s32 i;
    s32 j;
    s32 sel;

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 14].unk_18 = j + 0x21;
        } else {
            ((Actor *)actor_pool_96)[i + 14].unk_18 = j + 0x22;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            game_state.confirm_button = 1;
            game_state.options_dirty |= 0x10;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            game_state.confirm_button = 0;
            game_state.options_dirty |= 0x10;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void func_80134D38(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x70) and array offsets; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 3, a, b, c, d);
    func_801320C0();
    mode_step_set(1);
}

void func_80134DD0(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x60); real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 2, a, b, c, d);
    mode_step_next();
    func_801320C0();
}

void func_80134E64(void) {
    s32 i;
    s32 j;
    s32 sel;

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 16].unk_18 = j + 0x25;
        } else {
            ((Actor *)actor_pool_96)[i + 16].unk_18 = j + 0x26;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            game_state.click_mode = 0;
            game_state.options_dirty |= 0x40;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            game_state.click_mode = 1;
            game_state.options_dirty |= 0x40;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void func_80134F9C(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x70) and array offsets; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 3, a, b, c, d);
    func_801320C0();
    mode_step_set(1);
}

void func_80135034(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame; real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10 + 0x28;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 2, a, b, c, d);
    /* record 20 of the 0x44-byte table at actor_pool_96: one base symbol keeps the load behind the stores (T-4070) */
    ((Actor *)actor_pool_96)[20].unk_18 = 0x79;
    ((Actor *)actor_pool_96)[20].x = 0x40;
    ((Actor *)actor_pool_96)[20].y = 0x30;
    ((Actor *)actor_pool_96)[20].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[20].unk_43 = 0x1F;
    D_8013D180 = 0;
    mode_step_next();
    func_801320C0();
}

void func_80135110(void) {
    s32 i;
    s32 j;
    s32 sel;

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 18].unk_18 = j + 0x29;
        } else {
            ((Actor *)actor_pool_96)[i + 18].unk_18 = j + 0x2A;
        }
    }
    if (((game_state.confirm_button != 0) && (game_state.mouse_buttons & MOUSE_RIGHT)) || ((game_state.confirm_button == 0) && (game_state.mouse_buttons & MOUSE_LEFT))) {
        switch (sel) {
        case 0:
            game_state.options_dirty |= 0x20;
            *(u8 *)&game_state.double_click_interval = 0x20;
            audio_command(0x501);
            mode_step_next();
            break;
        case 1:
            if (*(u8 *)&D_8013D180 != 0) {
                game_state.options_dirty |= 0x20;
                *(u8 *)&game_state.double_click_interval = 0xFF - *(u8 *)&D_8013D180;
                if (*(u8 *)&game_state.double_click_interval < 0x10U) {
                    *(u8 *)&game_state.double_click_interval = 0x10;
                }
                if (*(u8 *)&game_state.double_click_interval >= 0x41U) {
                    *(u8 *)&game_state.double_click_interval = 0x40;
                }
                audio_command(0x501);
                mode_step_next();
                *(u8 *)&D_8013D180 = 0;
            } else {
                *(u8 *)&D_8013D180 = 0xFF;
            }
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        D_80120BA3 &= 0xFF7F;
        mode_step_next();
    }
    func_801352FC();
    if (*(u8 *)&D_8013D180 != 0) {
        *(u8 *)&D_8013D180 -= 1;
    }
}

void func_801352FC(void) {
    if ((*(u8 *)&game_state.double_click_interval != 0) && ((u32) ((game_state.frame_count / *(u8 *)&game_state.double_click_interval) & 3) < 2U)) {
        if ((u32) ((u32) game_state.frame_count % *(u8 *)&game_state.double_click_interval) < 2U) {
            if (game_state.confirm_button == 0) {
                D_80120BB8 = 0x78;
                return;
            }
            D_80120BB8 = 0x79;
            return;
        }
        D_80120BB8 = 0x77;
    }
}

void func_8013539C(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    menu_set(1, 3, a, b, c, d);
    func_801320C0();
    D_80120BA3 &= 0xFF7F;
    D_80120BE3 = 0;
    mode_step_set(1);
}

s32 func_80135450() {
    switch (game_state.mode_step) {
    case 0:
        func_801344E4();
        return;
    case 1:
        func_80134804();
        return;
    case 2:
        func_8013493C();
        return;
    case 3:
        func_80134B6C();
        return;
    case 4:
        func_80134C00();
        return;
    case 5:
        func_80134D38();
        return;
    case 6:
        func_80134DD0();
        return;
    case 7:
        func_80134E64();
        return;
    case 8:
        func_80134F9C();
        return;
    case 9:
        func_80135034();
        return;
    case 10:
        func_80135110();
        return;
    case 11:
        func_8013539C();
        /* fallthrough */
    default:
        return;
    }
}

void func_8013554C(void) {
    GameState *g = &game_state;
    s32 i;
    u8 t;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    t = g->substep;
    switch (t) {
    case 0:
        ((Actor *)actor_pool)[106].unk_18 = 2;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        g->substep = 1;
        D_80120912[0] = 0x7FFF;
        break;
    case 1:
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            k_sub_reset();
            for (i = 0; i < 4; i++) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0xA0;
                d[i] = 0x10;
                set_kanji_string(a[i] - 4, b[i], 0, (s32)D_8013D350[i], 0);
            }
            menu_set(1, 4, a, b, c, d);
            switch (g->menu_mode & 3) {
            case 0:
                ((Actor *)actor_pool_96)[11].unk_18--;
                g->menu_cursor_1 = 0;
                break;
            case 1:
                ((Actor *)actor_pool_96)[12].unk_18--;
                g->menu_cursor_1 = 1;
                break;
            case 3:
                ((Actor *)actor_pool_96)[13].unk_18--;
                g->menu_cursor_1 = 2;
                break;
            case 2:
                ((Actor *)actor_pool_96)[14].unk_18--;
                g->menu_cursor_1 = 3;
                break;
            }
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　カーソルが、方向キーの上下左右に", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"反応して動きます。", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"（マウスと同等です）", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"メニューアイテムの順番通りにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"その方向のメニューアイテムにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　通常は、マウスライクと同じ動作を", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"します。Ｌ１ボタンを押している間は、", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"メニューＡと同じ動作をします", 0);
            k_sub_disp_start(0x10);
            for (i = 0; i < 12; i++) {
                k_disp_switch(i + 0xC, 0);
            }
            mode_step_next();
        }
        break;
    }
    func_801324E8();
}

void func_801359A4(void) {
    s32 i;
    s32 k;
    GameState *g;

    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    g = &game_state;
    for (i = 0; i < 4; i++) {
        if (i == g->menu_cursor_1) {
            k_disp_switch(i * 3 + 0xC, 1);
            k_disp_switch(i * 3 + 0xD, 1);
            k_disp_switch(i * 3 + 0xE, 1);
            ((TextEntry *)text_list)[i + 8].attribute = 0;
            k = i * 2 + 0x7A;
            ((Actor *)actor_pool_96)[i + 11].unk_18 = k;
        } else {
            k_disp_switch(i * 3 + 0xC, 0);
            k_disp_switch(i * 3 + 0xD, 0);
            k_disp_switch(i * 3 + 0xE, 0);
            ((TextEntry *)text_list)[i + 8].attribute = 1;
            k = i * 2 + 0x7B;
            ((Actor *)actor_pool_96)[i + 11].unk_18 = k;
        }
    }
    prim_gradient_rect(-0x85, 0x3A, 0x100, 0x34, 8, 0x80808, 0x80);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (g->menu_cursor_1) {
        case 0:
            game_state.menu_mode = 4;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            game_state.menu_mode = 5;
            mode_step_next();
            audio_command(0x501);
            break;
        case 2:
            game_state.menu_mode = 7;
            mode_step_next();
            audio_command(0x501);
            break;
        case 3:
            game_state.menu_mode = 6;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        k_sub_reset();
        mode_step_next();
        game_state.substep = 2;
    }
}

void func_80135C0C(void) {
    s32 i;

    if (game_state.substep == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= 0xFF7F;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        mode_stage_set(0);
        mode_step_set(2);
    }
    func_80132384();
    D_801208FB = 0;
    k_sub_reset();
}

void func_80135DE0(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013554C();
        return;
    case 1:
        func_801359A4();
        return;
    case 2:
        func_80135C0C();
        return;
    }
}

s32 func_80135E48(void) {
    GameState *g = &game_state;
    s32 i;
    s32 pad; /* FAKE: unused word reproduces the original frame (0x88) and the row offsets, like the pad of the matched four-array pages (T-4070); real source unknown. T-20910 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (g->substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 4;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        g->substep = 1;
        D_80120912[0] = 0x7FFF;
        return;
    }
    D_80120912[0] = D_80120912[0] / 17 * 16;
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0; i < 5; i++) {
            a[i] = 0x10;
            b[i] = i * 0x18 - 0x18;
            c[i] = 0x30;
            d[i] = 0x10;
            ((Actor *)actor_pool_96)[i + 11].unk_18 = i * 2 + 0xC;
            ((Actor *)actor_pool_96)[i + 11].x = a[i];
            ((Actor *)actor_pool_96)[i + 11].y = b[i];
            ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
        }
        menu_set(1, 5, a, b, c, d);
        switch ((u8)g->cursor_speed % 10) {
        case 1:
        default:
            ((Actor *)actor_pool_96)[11].unk_18++;
            g->menu_cursor_1 = 0;
            break;
        case 2:
            ((Actor *)actor_pool_96)[12].unk_18++;
            g->menu_cursor_1 = 1;
            break;
        case 3:
            ((Actor *)actor_pool_96)[13].unk_18++;
            g->menu_cursor_1 = 1;
            break;
        case 4:
            ((Actor *)actor_pool_96)[14].unk_18++;
            g->menu_cursor_1 = 1;
            break;
        case 6:
            ((Actor *)actor_pool_96)[15].unk_18++;
            g->menu_cursor_1 = 1;
            break;
        }
        mode_step_next();
    }
}

/* FAKE: s32 without an explicit return retains the original dead v0 loop initialization; no meaningful return is established. T-24117 */
s32 func_801361FC(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through actor_pool (T-9020) */
    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 5; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0xB;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0xC;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            game_state.cursor_speed = 1;
            game_state.input_device = INPUT_NONE;
            game_state.options_dirty |= 8;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            game_state.cursor_speed = 2;
            game_state.input_device = INPUT_NONE;
            game_state.options_dirty |= 8;
            mode_step_next();
            audio_command(0x501);
            break;
        case 2:
            game_state.cursor_speed = 3;
            game_state.input_device = INPUT_NONE;
            game_state.options_dirty |= 8;
            mode_step_next();
            audio_command(0x501);
            break;
        case 3:
            game_state.cursor_speed = 4;
            game_state.input_device = INPUT_NONE;
            game_state.options_dirty |= 8;
            mode_step_next();
            audio_command(0x501);
            break;
        case 4:
            game_state.cursor_speed = 6;
            game_state.input_device = INPUT_NONE;
            game_state.options_dirty |= 8;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
        game_state.substep = 2;
    }
}

s32 func_80136434(void) {
    s32 i;

    if (game_state.substep == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        mode_stage_set(0);
        mode_step_set(2);
    }
    D_801208FB = 0;
}

s32 func_8013661C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80135E48();
        break;
    case 1:
        func_801361FC();
        break;
    case 2:
        func_80136434();
        break;
    }
}

void func_80136688(void) {
    GameState *g = &game_state;
    s32 i;
    s32 j;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (g->substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 5;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        g->substep = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24119 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = 0;
                b[i] = i * 0x18 + 0x18;
                c[i] = 0x80;
                d[i] = 0x10;
                j = i * 2 + 0x2E;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((Actor *)actor_pool_96)[i + 11].unk_18 = j; ((Actor *)actor_pool_96)[i + 11].x = a[i]; ((Actor *)actor_pool_96)[i + 11].y = b[i]; ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            switch ((u8)g->voice_mode % 3) {
            case 1:
                ((Actor *)actor_pool_96)[11].unk_18++;
                g->menu_cursor_1 = 0;
                break;
            case 0:
                ((Actor *)actor_pool_96)[12].unk_18++;
                g->menu_cursor_1 = 1;
                break;
            case 2:
            default:
                ((Actor *)actor_pool_96)[13].unk_18++;
                g->menu_cursor_1 = 2;
                break;
            }
            mode_step_next();
        }
    }
    func_801324E8();
}

void func_801368FC(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through actor_pool (T-9020) */
    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x2D;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x2E;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            game_state.voice_mode = 4;
            mode_step_next();
            audio_command(0x501);
            break;
        case 1:
            game_state.voice_mode = 3;
            mode_step_next();
            audio_command(0x501);
            break;
        case 2:
            game_state.voice_mode = 5;
            mode_step_next();
            audio_command(0x501);
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
        game_state.substep = 2;
    }
}

void func_80136A50(void) {
    s32 i;

    if (game_state.substep == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((Actor *)actor_pool)[i + 107].draw_flags &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        mode_stage_set(0);
        mode_step_set(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_80136BC8(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80136688();
        return;
    case 1:
        func_801368FC();
        return;
    case 2:
        func_80136A50();
        return;
    }
}

void func_80136C30(void) {
    s32 i;
    s32 pad[3]; /* FAKE: unused words reproduce the original frame (0x70) and the row offsets (T-4070 pad); real source unknown. T-20910 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[106].unk_18 = 6;
        ((Actor *)actor_pool)[106].x = -0xA0;
        ((Actor *)actor_pool)[106].y = -0x78;
        ((Actor *)actor_pool)[106].draw_flags = 0;
        D_80120912[1] = 0x7FFF;
        D_80120912[0] = 0x7FFF;
        game_state.substep = 1;
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 4; i++) {
                a[i] = -0x40;
                b[i] = i * 0x10 + 0x18;
                c[i] = 0x60;
                d[i] = 0x10;
                /* FAKE: actor stores on one line so as1 schedules lui t6 (unk_03) before li t5, 52 (unk_18) (T-7020). T-24110 */
                ((Actor *)actor_pool_96)[i + 11].unk_18 = i * 2 + 0x34; ((Actor *)actor_pool_96)[i + 11].x = a[i]; ((Actor *)actor_pool_96)[i + 11].y = b[i]; ((Actor *)actor_pool_96)[i + 11].draw_flags |= 0x80;
            }
            if (!(D_800E7D4F & 1) && !(D_800E7D50 & 1)) {
                c[0] = 0;
                ((Actor *)actor_pool_96)[11].draw_flags = 0;
            }
            if (func_80136F3C() == 0) {
                ((Actor *)actor_pool_96)[12].draw_flags = 0;
                c[1] = 0;
                c[2] = 0;
                ((Actor *)actor_pool_96)[13].draw_flags = 0;
            }
            if (D_800E7D37 == 0) {
                c[3] = 0;
                ((Actor *)actor_pool_96)[14].draw_flags = 0;
            }
            menu_set(1, 4, a, b, c, d);
            game_state.menu_cursor_1 = 0;
            mode_step_next();
        }
    }
    func_801324E8();
}

s32 func_80136F3C(void) {
    s32 i;
    s32 count;

    count = 0;
    for (i = 0; i < 14; i++) {
        if (system_data.flags[i] != 0 || game_state.girl_cleared[i] != 0) {
            count++;
        }
    }
    return count;
}

void func_80137040(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through actor_pool (T-9020) */
    ((Actor *)actor_pool)[106].draw_flags = 0x80;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    sel = game_state.menu_cursor_1;
    for (i = 0, j = 0; i < 4; i++, j += 2) {
        if (i == sel) {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x33;
        } else {
            ((Actor *)actor_pool_96)[i + 11].unk_18 = j + 0x34;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (sel) {
        case 0:
            if ((D_800E7D4F & 1) || (D_800E7D50 & 1)) {
                mode_step_set(0xA);
                audio_command(0x501);
            }
            break;
        case 1:
            if (func_80136F3C() != 0) {
                mode_step_set(0x14);
                audio_command(0x501);
            }
            break;
        case 2:
            if (func_80136F3C() != 0) {
                mode_step_set(0x1E);
                audio_command(0x501);
            }
            break;
        case 3:
            if (D_800E7D37 != 0) {
                mode_step_set(0x28);
                audio_command(0x501);
            }
            break;
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
        game_state.substep = 2;
    }
}

void func_801371FC(void) {
    s32 i;

    if (game_state.substep == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 1;
        }
    } else if (game_state.substep == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= 0xFF7F;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            game_state.substep = 1;
        }
    } else {
        mode_stage_set(0);
        mode_step_set(2);
    }
    func_80132384();
    D_801208FB = 0;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 (as in func_801378DC, T-24087). T-24119 */
s32 func_801373C8(void) {
    s32 i;
    s32 j;
    s32 *p;
    s32 z;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (game_state.substep != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121726) {} /* FAKE: empty reads promote D_80121726 and D_80121728 to $t1 and $t3 across the zoom clamp (T-24087). T-24119 */
            if (D_80121728) {}
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 2; i++) {
                a[i] = -0x50;
                b[i] = i * 0x10 + 8;
                c[i] = 0xC4;
                d[i] = 0x10;
                j = i * 2 + 0x3C;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((Actor *)actor_pool_96)[i + 15].unk_18 = j; ((Actor *)actor_pool_96)[i + 15].x = a[i]; ((Actor *)actor_pool_96)[i + 15].y = b[i]; ((Actor *)actor_pool_96)[i + 15].draw_flags |= 0x80;
            }
            ((Actor *)actor_pool_96)[15].draw_flags = 0;
            ((Actor *)actor_pool_96)[16].draw_flags = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    c[0] = 0;
                }
                /* FAKE: word load view reproduces the original lw $t9, D_800E7D50 before masking bit 0. T-24119 */
                if (!(*(s32 *)&D_800E7D50 & 1)) {
                    c[1] = 0;
                }
            }
            menu_set(1, 2, a, b, c, d);
        }
    }
    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[159].unk_18 = 7;
        ((Actor *)actor_pool)[159].x = -0xA0;
        ((Actor *)actor_pool)[159].y = -0x78;
        /* FAKE: folded s16 casts advance ugen's 5-register temp ring back to $t5 without extra reads of unk_110D. T-24119 */
        i = game_state.substep;
        i = (s16)i;
        i = (s16)i;
        i = (s16)i;
        i = (s16)i;
        game_state.substep = i + 1;
        if (0) {}
    } else if (game_state.substep == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            game_state.substep = 2;
        }
    } else {
        mode_step_next();
    }
    ((Actor *)actor_pool)[106].draw_flags = 0;
    ((Actor *)actor_pool)[159].draw_flags = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
    if (0) {}
}

/* FAKE: s32 without an explicit return retains `or $v0, $a2, $zero` in the switch delay slot. T-24117 */
s32 func_801376A8(void) {
    s32 i;
    s32 j;
    GameState *g;

    /* one table: as1 keeps the loads after the stores only when both go through actor_pool (T-9020, T-16930) */
    ((Actor *)actor_pool)[159].draw_flags = 0x80;
    ((Actor *)actor_pool)[159].scale_x = 0x1000;
    ((Actor *)actor_pool)[159].scale_y = 0x1000;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    if ((D_800E7D4F & 1) == 1) {
        ((Actor *)actor_pool)[111].draw_flags = 0x80;
    }
    /* FAKE: 1U matches u8 D_800E7D4F's -cckr unsigned promotion so uopt CSEs constant 1 into $t1. T-24117 */
    if ((D_800E7D50 & 1U) == 1) {
        ((Actor *)actor_pool)[112].draw_flags = 0x80;
    }
    g = &game_state;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == g->menu_cursor_1) {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x3B;
        } else {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x3C;
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (g->menu_cursor_1) {
        case 0:
            if ((D_800E7D4F & 1) == 1) {
                /* FAKE: SystemData view prevents uopt from CSE-ing the byte load with D_800E7D4F across the loop (as in 800674B0.c). T-24117 */
                system_data.unk_027[0x18] &= 0xFFFD;
                audio_command(0x501);
                actor_clear_range(0);
                text_system_reset();
                game_mode_set(GAME_MODE_TT_GAME);
            }
            break;
        case 1:
            if ((D_800E7D50 & 1U) == 1) {
                /* FAKE: SystemData view emits byte load-modify-store without CSE-ing D_800E7D50. T-24117 */
                system_data.unk_027[0x19] &= 0xFFFD;
                audio_command(0x501);
                actor_clear_range(0);
                text_system_reset();
                game_mode_set(GAME_MODE_SHOOTER);
            }
            break;
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void func_80137864(void) {
    s32 i;
    s16 m;

    D_8012170F &= (m = 0xFF7F); /* FAKE: keeps the mask in $a0 for the loop's `and`, as the original; real source unknown. T-9340 */
    for (i = 0; i < 2; i++) {
        ((Actor *)actor_pool_96)[i + 15].draw_flags &= ~0x80;
    }
    mode_step_set(0);
    D_8012170F = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 and puts sh $t3 in the jr delay slot (as in mode_step_set, T-9540/T-11610). T-24087 */
s32 func_801378DC(void) {
    s32 i;
    s32 icon;
    s32 *p;
    s32 z;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    if (game_state.substep != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121728) {} /* FAKE: empty read promotes D_80121728 to $t3 across the zoom clamp as in the original. T-24087 */
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 14; i++) {
                a[i] = (i / 7) * 128 - 0x78;
                b[i] = (i % 7) * 0x12 - 0x48;
                c[i] = 0x80;
                d[i] = 0x10;
                /* FAKE: comma evaluation enters icon and &system_data.flags[i] into uopt before actor_pool_96 so the $t1 increment precedes $v1. T-24087 */
                ((Actor *)actor_pool_96)[i + 15].unk_18 = (icon = i * 2 + 0x40, p = (s32 *)&system_data.flags[i], icon);
                ((Actor *)actor_pool_96)[i + 15].draw_flags = 0;
                ((Actor *)actor_pool_96)[i + 15].x = a[i];
                ((Actor *)actor_pool_96)[i + 15].y = b[i];
                if (system_data.flags[i] == 0 && game_state.girl_cleared[i] == 0) {
                    c[i] = 0;
                }
            }
            menu_set(1, 14, a, b, c, d);
        }
    }
    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[159].unk_18 = 8;
        ((Actor *)actor_pool)[159].x = -0xA0;
        ((Actor *)actor_pool)[159].y = -0x78;
        game_state.substep++;
    } else if (game_state.substep == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[1] = D_80120912[1] / 9 * 8;
        if (D_80120912[1] < 9) {
            game_state.substep = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-0x80, -0x58, 0, (s32)"フリートーク", 0);
        k_sub_disp_start(1);
        mode_step_next();
    }
    ((Actor *)actor_pool)[106].draw_flags = 0;
    ((Actor *)actor_pool)[159].draw_flags = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
}

void func_80137C1C(void) {
    GameState *g = &game_state;
    u32 *e;
    s32 sel;
    s32 i;
    s32 j;

    for (i = 0; i < 14; i++) {
        if (system_data.flags[i] != 0 || g->girl_cleared[i] != 0) {
            ((Actor *)actor_pool_96)[i + 15].draw_flags = 0x80;
        }
    }
    /* one table: as1 keeps the loads after the stores only when both go through actor_pool (T-9020, T-16930) */
    ((Actor *)actor_pool)[159].draw_flags = 0x80;
    ((Actor *)actor_pool)[159].scale_x = 0x1000;
    ((Actor *)actor_pool)[159].scale_y = 0x1000;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    for (i = 0, j = 0; i < 14; i++, j += 2) {
        if (i == g->menu_cursor_1) {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x3F;
        } else {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x40;
        }
    }
    if (g->pad_pressed & PAD_CROSS) {
        audio_command(0x74);
        g->menu_cursor_1 = -1;
        mode_step_set((g->mode_step + 2) & 0xFF);
    } else if (g->pad_pressed & PAD_CIRCLE) {
        sel = g->menu_cursor_1;
        if (system_data.flags[sel] != 0 || g->girl_cleared[sel] != 0) {
            if ((cd_audio_state() & 2) && sel >= 0 && sel < 14) {
                e = D_8013D2C4[sel];
                if (e[0] != 0) {
                    D_801230D0 = 1;
                    D_8013D2FC = 0;
                    cd_audio_request_set(e[1], e[2], sel);
                    audio_command(0x300);
                    mode_step_next();
                }
            }
        } else {
            audio_command(0x74);
        }
    }
    func_801320C0();
}

void func_80137E38(void) {
    u32 **e;
    s32 sel;

    sel = game_state.menu_cursor_1;
    if (D_8013D2FC == 0) {
        if (cd_audio_state() & 4) {
            D_8013D2FC = 1;
        }
    }
    if (D_8013D2FC != 0) {
        e = &D_8013D2C4[sel];
        if (D_801230D0 < **e) {
            if (cd_audio_state() & 2) {
                D_8013D2FC = 0;
                /* Word indexing preserves the original temporary allocation. T-12901 */
                cd_audio_request_set(((s32 *)*e)[D_801230D0 * 2 + 1], ((s32 *)*e)[D_801230D0 * 2 + 2], sel);
                D_801230D0 += 1;
                audio_command(0x300);
            }
        } else if (cd_audio_state() & 2) {
            D_801230D0 = 0;
            mode_step_set((game_state.mode_step - 1) & 0xFF);
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        D_801230D0 = 0;
        audio_command(0x74);
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
    func_801320C0();
}

void func_80137FA0(void) {
    s32 i;

    audio_command(0x74);
    k_sub_reset();
    k_disp_switch(6, 1);
    D_801206D8[0x1037] &= ~0x80;
    D_801206D8[0x377] &= ~0x80;
    D_801206D8[0x3BB] &= ~0x80;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 12; i++) D_801206D8[0x3FF + i * 0x44] &= ~0x80;
    mode_step_set(0);
    D_801206D8[0x1037] = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 and puts sh $t3 in the jr delay slot; pad shifts a..d down by 4 bytes within the 0xE8 frame. T-24087 */
s32 func_80138080(void) {
    s32 i;
    s32 icon;
    s32 *p;
    s32 z;
    s32 pad;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    if (game_state.substep == 1) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121728) {} /* FAKE: empty read promotes D_80121728 to $t3 across the zoom clamp as in the original. T-24087 */
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 14; i++) {
                a[i] = (i / 7) * 128 - 0x78;
                b[i] = (i % 7) * 0x12 - 0x48;
                c[i] = 0x50;
                d[i] = 0x10;
                /* FAKE: comma evaluation enters icon and &system_data.flags[i] into uopt before actor_pool_96 so the $t1 increment precedes $v1. T-24087 */
                ((Actor *)actor_pool_96)[i + 15].unk_18 = (icon = i * 2 + 0x5C, p = (s32 *)&system_data.flags[i], icon);
                ((Actor *)actor_pool_96)[i + 15].draw_flags = 0;
                ((Actor *)actor_pool_96)[i + 15].x = a[i];
                ((Actor *)actor_pool_96)[i + 15].y = b[i];
                if (system_data.flags[i] == 0 && game_state.girl_cleared[i] == 0) {
                    c[i] = 0;
                }
            }
            menu_set(1, 14, a, b, c, d);
        }
    }
    if (game_state.substep == 0) {
        ((Actor *)actor_pool)[159].unk_18 = 8;
        ((Actor *)actor_pool)[159].x = -0xA0;
        ((Actor *)actor_pool)[159].y = -0x78;
        game_state.substep++;
    } else if (game_state.substep == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[1] = D_80120912[1] / 9 * 8;
        if (D_80120912[1] < 9) {
            game_state.substep = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-0x80, -0x58, 0, (s32)"エンディング", 0);
        k_sub_disp_start(1);
        mode_step_next();
    }
    ((Actor *)actor_pool)[106].draw_flags = 0;
    ((Actor *)actor_pool)[159].draw_flags = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
}

void func_801383C4(void) {
    GameState *g = &game_state;
    s32 i;
    s32 j;
    s32 sel;

    for (i = 0; i < 14; i++) {
        if (system_data.flags[i] != 0 || g->girl_cleared[i] != 0) {
            ((Actor *)actor_pool_96)[i + 15].draw_flags = 0x80;
        }
    }
    /* one table: as1 keeps the loads after the stores only when both go through actor_pool (T-9020, T-16930) */
    ((Actor *)actor_pool)[159].draw_flags = 0x80;
    ((Actor *)actor_pool)[159].scale_x = 0x1000;
    ((Actor *)actor_pool)[159].scale_y = 0x1000;
    menu_check(1, ((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
    menu_cursor_update(1);
    func_801320C0();
    for (i = 0, j = 0; i < 14; i++, j += 2) {
        if (i == g->menu_cursor_1) {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x5B;
        } else {
            ((Actor *)actor_pool_96)[i + 15].unk_18 = j + 0x5C;
        }
    }
    if (g->pad_pressed & PAD_CIRCLE) {
        sel = g->menu_cursor_1;
        if (system_data.flags[sel] != 0 || g->girl_cleared[sel] != 0) {
            if (sel >= 0 && sel < 14) {
                g->day_events[0].girl = sel;
                g->day_event_index = 0;
                g->current_girl = sel;
                if (g->current_girl < GIRL_YOSHIO) {
                    actor_clear_range(0);
                    text_system_reset();
                    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
                    audio_command(0x501);
                    mode_step_set(0x21);
                } else {
                    actor_clear_range(0);
                    text_system_reset();
                    audio_command(0x501);
                    mode_step_set(0x23);
                }
            }
        }
    }
    if (g->pad_pressed & PAD_CROSS) {
        g->menu_cursor_1 = -1;
        mode_step_set((g->mode_step + 1) & 0xFF);
    }
}

void func_801385F0(void) {
    s32 i;

    k_disp_switch(6, 1);
    k_sub_reset();
    D_801206D8[0x1037] &= ~0x80;
    D_801206D8[0x377] &= ~0x80;
    D_801206D8[0x3BB] &= ~0x80;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 12; i++) D_801206D8[0x3FF + i * 0x44] &= ~0x80;
    mode_step_set(0);
    D_801206D8[0x1037] = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

void func_801386C8(void) {
    actor_clear_range(0);
    game_state.current_girl = D_800E7D68;
    game_mode_set(GAME_MODE_ALBUM);
    func_801320C0();
}

void func_80138708(void) {
    s32 temp_v0;
    u8 temp;

    if ((u8) game_state.screen_brightness >= 3U) {
        game_state.screen_brightness -= 2;
    } else {
        actor_clear_upper();
        sprite_tables_clear();
    }
    temp_v0 = D_801220E0 - 2;
    temp = (u8) D_801220E0;
    if (temp >= 3U) {
        D_801220E2 = temp_v0;
        D_801220E1 = temp_v0;
        D_801220E0 = temp_v0;
    }
    if (audio_bank_load_poll() == 1) {
        cd_read_sectors(0x11U, 0x801F0000, 0xAE07);
        mode_step_next();
    }
}

void func_801387A8(void) {
    if (cd_read_sync_status() & 1) {
        game_mode_set(GAME_MODE_ENDING);
        mode_stage_set(3);
    }
}

s32 func_801387E4(void) {
    if (game_state.screen_brightness >= 3U) {
        game_state.screen_brightness -= 2;
    } else {
        actor_clear_upper();
        sprite_tables_clear();
        game_mode_set(GAME_MODE_ENDING);
        mode_stage_set(7);
        mode_step_set(0xF);
    }
}

s32 func_8013884C() {
    switch (game_state.mode_step) {
    case 0:
        func_80136C30();
        return;
    case 1:
        func_80137040();
        return;
    case 2:
        func_801371FC();
        return;
    case 10:
        func_801373C8();
        return;
    case 11:
        func_801376A8();
        return;
    case 12:
        func_80137864();
        return;
    case 20:
        func_801378DC();
        return;
    case 21:
        func_80137C1C();
        return;
    case 22:
        func_80137E38();
        return;
    case 23:
        func_80137FA0();
        return;
    case 30:
        func_80138080();
        return;
    case 31:
        func_801383C4();
        return;
    case 32:
        func_801385F0();
        return;
    case 33:
        func_80138708();
        return;
    case 34:
        func_801387A8();
        return;
    case 35:
        func_801387E4();
        return;
    case 40:
        func_801386C8();
        /* fallthrough */
    default:
        return;
    }
}
