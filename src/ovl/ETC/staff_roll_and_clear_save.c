/* staff_roll_and_clear_save.c: ; src/ovl/ETC/80143240.c:3-12 defines data used by staff-roll and clear-save stages in this unsplit object. Original object 0x80143240-0x80146400 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s16 staff_roll_model_x = -0x60;
s16 staff_roll_model_y = 0x20;
s16 staff_roll_model_z = -0x300;
SVECTOR staff_roll_model_rotation = { 0, 0, 0, 0 };
s32 staff_roll_line_index = 0;
s32 staff_roll_girl_slot = 0;
s32 staff_roll_variant = 0;
s32 staff_roll_girl_order[14] = { 0, 10, 13, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11 };
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", D_80150188);
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", staff_roll_fade_dir);
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", staff_roll_palette);
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", last_photo_lba);
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", last_photo_all_clear_lba);
INCLUDE_RODATA("asm/ovl/ETC/data/ETC/staff_roll_and_clear_save.data", staff_roll_credit_table);

s32 misc_all_girls_cleared(void) {
    s32 i;
    u8 *p;

    if (D_800E7D11[0x13] == 0 && game_state.girl_cleared[0] == 0) {
        return 0;
    }
    i = 1;
    /* FAKE: `do {` on the line of the pointer init; as1 schedules by source line and only this layout orders the lui/addiu pair like the original. T-8030 */
    p = D_800E7D11; do {
        if (p[0x14] == 0 && game_state.girl_cleared[i] == 0) {
            return 0;
        }
        if (p[0x15] == 0 && game_state.girl_cleared[i + 1] == 0) {
            return 0;
        }
        if (p[0x16] == 0 && game_state.girl_cleared[i + 2] == 0) {
            return 0;
        }
        if (p[0x17] == 0 && game_state.girl_cleared[i + 3] == 0) {
            return 0;
        }
        i += 4;
        p += 4;
    } while (i != 0xD);
    return 1;
}

void staff_roll_start_variant(void) {
    staff_roll_line_index = 0;
    staff_roll_credit_lines_init();
    if (staff_roll_variant == 1) {
        draw2d3d(1, 1);
        title_view_init();
        title_lights_init();
        title_coordinates_init();
        staff_roll_models_init();
        mode_stage_next();
        return;
    }
    audio_bgm_stop(0);
    cd_audio_request_set(0x07000001, 0x02000E0C, 0xF);
    audio_command(0x300);
    staff_roll_pictures_place();
    mode_stage_next();
    mode_step_set(0xFF);
}



void staff_roll_model_matrix_update(void) {
    s32 pad1[2]; /* FAKE: two unused 8-byte locals (above m1 and below scale) give the original frame 0x88, as in album_model_matrix_update; real source unknown. T-9110 */
    MATRIX m1;
    MATRIX m2;
    VECTOR scale;
    s32 pad2[2];

    m1 = *(MATRIX *)view_base_matrix;
    m2 = *(MATRIX *)view_base_matrix;
    RotMatrix(&staff_roll_model_rotation, &m1);
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    ScaleMatrix(&m2, &scale);
    MulMatrix(&m1, &m2);
    m1.t[0] = staff_roll_model_x;
    m1.t[1] = staff_roll_model_y;
    m1.t[2] = staff_roll_model_z;
    *(MATRIX *)D_801227A4 = m1;
    view_coordinates->flag = 0;
}

void staff_roll_models_init(void) {
    s32 model;
    GraphicsCoordinate2 *record;
    u32 *object;

    model = 0x801F9004;
    func_8009B3C0(model);
    model += 4;
    model += 4;
    /* FAKE: initializer/do line grouping reproduces setup order; original line layout unknown. T-24094 */
    object = model_objects; record = view_coordinates; do {
        func_8009B430(model, (u8 *)object, 0);
        object[1] = (u32)record;
        record++;
        object += 4;
        object[-4] = 0x80000000;
    } while (record != D_80122CA0);
    func_8009B430(model, (u8 *)model_objects, 0);
    model_objects[1] = (u32)view_coordinates;
    model_objects[0] = 0;
    staff_roll_model_x = -0x60;
    staff_roll_model_y = 0x20;
    staff_roll_model_z = -0x300;
    staff_roll_model_rotation.vz = 0;
    staff_roll_model_rotation.vx = 0;
    staff_roll_model_rotation.vy = 0;
}

void staff_roll_stage_load(void) {
    switch (game_state.mode_step) {
    case 0:
        staff_roll_load_step();
        break;
    case 1:
        staff_roll_start_variant();
        break;
    }
    staff_roll_pictures_fade();
}
void staff_roll_music_start(void) {
    audio_bgm_stop(0);
    cd_audio_request_set(0x07000001, 0x02000E0C, 0xF);
    audio_command(0x300);
    mode_step_next();
}

void staff_roll_girl_textures_load(s32 arg0) {
    load_tpage_buf_lock(5, 0x801E8000 - (arg0 << 15), 2, arg0);
    load_tpage_buf_lock(6, 0x801EA000 - (arg0 << 15), 2);
    load_tpage_buf_lock(7, 0x801EC000 - (arg0 << 15), 2);
}

void staff_roll_credit_lines_init(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        j = i + 0x60;
        ((Actor *)actor_pool)[i + 0x60].unk_01 = 0;
        ((Actor *)actor_pool)[i + 0x60].flags = 0x40;
        ((Actor *)actor_pool)[i + 0x60].quad_flags = 0;
        ((Actor *)actor_pool)[i + 0x60].draw_flags = 0xA4;
        ((Actor *)actor_pool)[i + 0x60].frame_table = (u8 *)0x801F86EC;
        ((Actor *)actor_pool)[i + 0x60].sequence_table = (u8 *)0x801F897C;
        ((Actor *)actor_pool)[i + 0x60].tpage_table = (u8 *)0x801F86A4;
        ((Actor *)actor_pool)[i + 0x60].unk_04 = 8;
        ((Actor *)actor_pool)[i + 0x60].unk_18 = staff_roll_credit_table[staff_roll_line_index * 2];
        ((Actor *)actor_pool)[i + 0x60].unk_43 = 0xF;
        ((Actor *)actor_pool)[i + 0x60].unk_05 = 0x80;
        ((Actor *)actor_pool)[i + 0x60].x = staff_roll_credit_table[staff_roll_line_index * 2 + 1] - 0xA0;
        ((Actor *)actor_pool)[i + 0x60].y = i * 0x28 + 0x8C;
        ((Actor *)actor_pool)[i + 0x60].dx = 0;
        ((ActorFixed *)actor_pool)[i + 0x60].dy = -1;
        ((Actor *)actor_pool)[i + 0x60].brightness = 0x80;
        if (((Actor *)actor_pool)[i + 0x60].unk_18 >= 0xF1) {
            ((Actor *)actor_pool)[j].draw_flags = 0x34;
        } else {
            ((Actor *)actor_pool)[j].draw_flags = 0xB4;
        }
        staff_roll_line_index += 1;
    }
}

void staff_roll_credit_line_reset(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 j;

    j = arg0 + 0x60;
    p = actor_pool + arg0 * 0x44;
    p[0x1982] = 0x40;
    p[0x1983] = 0xA4;
    p[0x1981] = 0;
    *(s32 *)(p + 0x19B8) = 0;
    *(s32 *)(p + 0x198C) = 0x801F86EC;
    *(s32 *)(p + 0x1990) = 0x801F897C;
    *(s32 *)(p + 0x19B4) = 0x801F86A4;
    p[0x1984] = 8;
    *(s16 *)(p + 0x1998) = arg1;
    p[0x19C3] = 0xF;
    p[0x1985] = 0x80;
    *(s16 *)(p + 0x19A6) = arg2 - 0xA0;
    *(s16 *)(p + 0x19AA) = 0x8C;
    *(s16 *)(p + 0x19AE) = 0;
    *(s16 *)(p + 0x19B2) = -1;
    p[0x1987] = 0x80;
    if (arg1 == 0xA3) {
        ((Actor *)actor_pool)[j].unk_43 = 0xE;
    }
    p = actor_pool + j * 0x44;
    if (*(s16 *)(p + 0x18) >= 0xF1) {
        p[3] = 0x34;
    } else {
        p[3] = 0xB4;
    }
}

/* Written as s32 (K&R implicit int) with no return statement so caller staff_roll_models_step
 * sees a return value in v0 after jal staff_roll_credit_lines_update, matching the original register allocation (T-24102). */
s32 staff_roll_credit_lines_update(void) {
    s32 i;
    s32 j;

    if (staff_roll_line_index < 0x11D) {
        debug_print_hex(staff_roll_line_index, 0x28, 0x28);
        for (i = 0; i < 7; i++) {
            j = i + 0x60;
            ((Actor *)actor_pool)[i + 0x60].y -= 1;
            if (((Actor *)actor_pool)[i + 0x60].y < -0x8C) {
                staff_roll_credit_line_reset(i, staff_roll_credit_table[staff_roll_line_index * 2], staff_roll_credit_table[staff_roll_line_index * 2 + 1]);
                staff_roll_line_index += 1;
            }
            ((Actor *)actor_pool)[j].scale_x = 0x1000;
            ((Actor *)actor_pool)[j].scale_y = 0x1000;
        }
    }
}

void staff_roll_models_step(void) {
    /* FAKE: unused pad locals and address-taken stores to sp30 (0x30(sp)) and sp28 (0x28(sp))
     * reproduce the original stack slots and spill reloads across the calls; real source unknown. T-24102 */
    s32 pad;
    s32 sp30;
    s32 pad2;
    s32 sp28;

    staff_roll_credit_lines_update();
    *(u32 *)&sp30 = (u32)game_state.mode_step_timer.w / 701U;
    if (((u32)game_state.mode_step_timer.w % 701U) == 1) {
        if (staff_roll_girl_slot == 0) {
            staff_roll_girl_textures_load(game_state.current_girl);
        } else if (staff_roll_girl_slot < 0xF) {
            if (game_state.current_girl == staff_roll_girl_order[staff_roll_girl_slot - 1]) {
                staff_roll_girl_slot += 1;
            }
            staff_roll_girl_textures_load(staff_roll_girl_order[staff_roll_girl_slot - 1]);
        } else {
            staff_roll_girl_textures_load(game_state.current_girl);
        }
        staff_roll_girl_slot += 1;
    } else if ((sp30 < 0xF) && (sp30 != 0)) {
        if (((u32)game_state.mode_step_timer.w % 701U) == 0) {
            staff_roll_model_x = -0x60;
            staff_roll_model_y = 0x20;
            staff_roll_model_rotation.vz = 0;
            staff_roll_model_rotation.vx = 0;
            staff_roll_model_rotation.vy = 0;
            staff_roll_model_z = -0x72D8;
        } else if (((u32)game_state.mode_step_timer.w % 701U) < 0x50U) {
            staff_roll_model_z += 0x172;
        }
    }
    if (staff_roll_girl_slot < 0xF) {
        if (((u32)game_state.mode_step_timer.w % 701U) >= 0x26EU) {
            staff_roll_model_z += 0x12C;
            *(s32 *)&sp28 = sp30 * 0x23D;
            staff_roll_model_y += rsin(sp28) / 256;
            staff_roll_model_x += func_800A0140(sp28) / 256;
        }
    }
    if (((u32)game_state.mode_step_timer.w >= 0x259U) && (cd_audio_state() & 2)) {
        mode_step_next();
    }
    debug_print_hex(staff_roll_model_x, 0x14, -0x28);
    debug_print_hex(staff_roll_model_y, 0x14, -0x1E);
    debug_print_hex(staff_roll_model_z, 0x14, -0x14);
    debug_print_hex(game_state.mode_step_timer.w, 0, -0xA);
    debug_print_hex(sp30, 0, 0);
    debug_print_hex(staff_roll_girl_slot, 0, 0xA);
    debug_print_hex(staff_roll_girl_order[staff_roll_girl_slot - 1], 0, 0x14);
    if (game_state.pad_held & PAD_LEFT) {
        staff_roll_model_x += 0x14;
    }
    if (game_state.pad_held & PAD_RIGHT) {
        staff_roll_model_x -= 0x14;
    }
    if (game_state.pad_held & PAD_DOWN) {
        staff_roll_model_y += 0x14;
    }
    if (game_state.pad_held & PAD_UP) {
        staff_roll_model_y -= 0x14;
    }
    if (game_state.pad_held & PAD_L1) {
        staff_roll_model_z -= 0x14;
    }
    if (game_state.pad_held & PAD_L2) {
        staff_roll_model_z += 0x14;
    }
    if (game_state.pad_held & 0xFA) {
        if (game_state.pad_held & PAD_R1) {
            staff_roll_model_rotation.vz += 0xA;
        }
        if (game_state.pad_held & PAD_R2) {
            staff_roll_model_rotation.vz -= 0xA;
        }
        if (game_state.pad_held & PAD_SQUARE) {
            staff_roll_model_rotation.vx += 0xA;
        }
        if (game_state.pad_held & PAD_CIRCLE) {
            staff_roll_model_rotation.vx -= 0xA;
        }
        if (game_state.pad_held & PAD_TRIANGLE) {
            staff_roll_model_rotation.vy += 0xA;
        }
        if (game_state.pad_held & PAD_CROSS) {
            staff_roll_model_rotation.vy -= 0xA;
        }
    } else {
        staff_roll_model_rotation.vz -= 3;
        staff_roll_model_rotation.vx -= 4;
        staff_roll_model_rotation.vy -= 5;
    }
}

void staff_roll_models_leave(void) {
    /* FAKE: GameState pointer plus one chain assignment of the three bytes of a record: the pointer makes uopt keep the
     * counter in a1 and the record base in v1, the chain stores the bytes in the original's order (autotweak gstate and
     * chain, T-20830). T-21520 */
    GameState *g = &game_state;
    s32 i;

    staff_roll_pictures_fade();
    if (g->screen_brightness != 0) {
        for (i = 0; i < 3; i++) {
            ((u8 *)&D_80121818)[i * 0x24 + 0x14] = ((u8 *)&D_80121818)[i * 0x24 + 0x15] = ((u8 *)&D_80121818)[i * 0x24 + 0x16] = g->screen_brightness;
        }
        g->screen_brightness -= 1;
    }
    staff_roll_model_z -= 0xC8;
    if (staff_roll_model_z < -0x7530) {
        view_model_slots_clear();
        if (g->current_girl >= GIRL_YOSHIO) {
            game_mode_set(GAME_MODE_TITLE);
        } else {
            game_mode_set(GAME_MODE_ETC_C3);
        }
    }
}

void staff_roll_stage_run(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        staff_roll_music_start();
        break;
    case 1:
        staff_roll_models_step();
        break;
    case 2:
        staff_roll_models_leave();
        break;
    case 0xFF:
        staff_roll_pictures_step();
        break;
    }
    staff_roll_model_matrix_update();
}
void staff_roll_pictures_place(s32 arg0) {
    s32 x;
    s32 remainder;
    s32 i;
    SpriteQuad *record;

    record = &sprite_quads[2];
    for (i = 2, x = -0xB0; i != 5; i++) {
        record->flags = 0;
        record->x = x;
        remainder = rand() % 112;
        record->y = remainder - 0x78;
        record++;
        x += 0x6C;
        /* FAKE: unsigned SDK width view shares 0x80 with unk_0A, as matched MAIN80084E90. T-24073. */
        *(u16 *)&record[-1].w = 0x80;
        record[-1].h = 0x80;
        record[-1].unk_0C = i + 3;
        record[-1].unk_0E = 0;
        record[-1].unk_0F = 0;
        record[-1].unk_10 = 0;
        record[-1].unk_12 = 0x1F1;
        record[-1].tint_r = 0;
        record[-1].tint_g = 0;
        record[-1].tint_b = 0;
        record[-1].anchor_x = 0;
        record[-1].anchor_y = 0;
        record[-1].unk_1C = 0x1000;
        record[-1].unk_1E = 0x1000;
        record[-1].unk_20 = 0;
    }
    game_state.quad_layer[2] = 0xFF;
    game_state.quad_layer[3] = 0xFE;
    game_state.quad_layer[4] = 0xFF;
    if (arg0 == 0xE) {
        sprite_quads[2].y = -0x18;
        sprite_quads[3].y = -0x64;
        sprite_quads[4].y = -0x28;
    }
}

void staff_roll_picture_brightness_set(s32 arg0, s32 arg1) {
    u8 v;
    u8 *p;

    if (arg1 > 0x80) {
        v = 0x80;
    } else if (arg1 < 0) {
        v = 0;
    } else {
        v = arg1;
    }
    p = (u8 *)sprite_quads + arg0 * 36;
    p[0x16] = v;
    p[0x15] = v;
    p[0x14] = v;
}

/* The frame counter is read through a pointer: that keeps it in one register range ($a3, reloaded
 * after each call group) and staff_roll_girl_slot promoted only in the remainder-1 arm, as in the original
 * (T-25146). */
void staff_roll_pictures_step(void) {
    WordView *fc;
    /* FAKE: the `*(u32 *)&quot` read below keeps quot in its slot 0x30(sp) (the test reloads it
     * instead of using the divide result), and the `*(s32 *)&v` stores keep v in 0x28(sp) without
     * a spill slot of its own; the matched sibling staff_roll_models_step needs the same two tricks for
     * sp30/sp28. Real source unknown. T-25146 */
    u32 quot;
    s32 x;
    s32 v;

    sprite_quads[0].flags = 0x80000000;
    sprite_quads[1].flags = 0x80000000;
    staff_roll_credit_lines_update();
    fc = &game_state.mode_step_timer;
    quot = fc->u / 701U;
    if (fc->u % 701U == 1) {
        if (staff_roll_girl_slot == 0) {
            staff_roll_girl_textures_load(game_state.current_girl);
        } else if (staff_roll_girl_slot < 0xF) {
            if (game_state.current_girl == staff_roll_girl_order[staff_roll_girl_slot - 1]) {
                staff_roll_girl_slot++;
            }
            staff_roll_girl_textures_load(staff_roll_girl_order[staff_roll_girl_slot - 1]);
        } else {
            staff_roll_girl_textures_load(game_state.current_girl);
        }
        staff_roll_girl_slot += 1;
    } else if ((s32)*(u32 *)&quot < 0xF) {
        if (fc->u % 701U == 2) {
            staff_roll_pictures_place(quot);
        } else if (fc->u % 701U < 0xA0) {
            x = fc->u % 701U;
            *(s32 *)&v = x * 3;
            staff_roll_picture_brightness_set(2, v / 4);
            staff_roll_picture_brightness_set(3, (v - 0x3C) / 4);
            staff_roll_picture_brightness_set(4, (v - 0x78) / 4);
        }
    }
    if (staff_roll_girl_slot < 0x10) {
        if (fc->u % 701U >= 0x21E) {
            x = 701 - fc->u % 701U;
            *(s32 *)&v = x * 3;
            staff_roll_picture_brightness_set(2, (v - 0x78) / 4);
            staff_roll_picture_brightness_set(3, (v - 0x3C) / 4);
            staff_roll_picture_brightness_set(4, v / 4);
        }
    }
    if (fc->u >= 0x259 && (cd_audio_state() & 2)) {
        mode_step_set(2);
    }
    debug_print_hex(fc->w, 0, -0xA);
    debug_print_hex(quot, 0, 0);
    debug_print_hex(staff_roll_girl_slot, 0, 0xA);
    debug_print_hex(staff_roll_girl_order[staff_roll_girl_slot - 1], 0, 0x14);
}

s32 clear_save_ask_init(void) {
    switch (game_state.substep) {
    case 0:
        tpage_buf_clear();
        draw2d3d(1, 0);
        k_speed_set(TEXT_SPEED_FAST);
        game_state.cursor_speed = SETTINGS_CURSOR_SPEED_NORMAL;
        actor_clear_range(0);
        actor_draw_enable(1);
        sprite_tables_clear();
        *(s32 *) &game_state.menu_hit[3] = 0x9F;
        hud_cursor_actor_init(0);
        back_clear_switch(1);
        text_system_reset();
        set_kanji_string(-0x90, -0x40, 0xF, "今回のプレイをセーブしますか？", 0);
        set_kanji_string(-0x18, 4, 0xF, misc_card_yes_text, 0);
        set_kanji_string(0x3C, 4, 1, misc_card_confirm_label(), 0);
        set_kanji_string(-0x18, 0x14, 0xF, misc_card_no_text, 0);
        set_kanji_string(0x3C, 0x14, 2, misc_card_cancel_label(), 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        data_save_load_class_init();
        D_80120653 |= 0x20;
        game_state.substep += 1;
        game_state.screen_brightness = 0;
        break;
    case 1:
        if (game_state.screen_brightness++ > 0x80) {
            mode_step_next();
        }
        break;
    }
}

void clear_save_ask_answer(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        text_system_reset();
        set_kanji_string(-0x90, -0x40, 0xF, "メモリーカードをチェックしています", 0);
        k_disp_start(1);
        D_800E8BEE = 0;
        memory_card_start();
        mode_stage_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(2);
    }
}

void clear_save_finish(void) {
    if (game_state.screen_brightness-- < 8U) {
        game_state.screen_brightness = 0;
        memory_card_stop();
        game_mode_set(GAME_MODE_OPENING);
    }
}

void clear_save_card_check_step(void) {
    GameState *gs = &game_state;

    switch (gs->substep) {
    case 0:
        if (memory_card_check_poll() != 0) {
            if (gs->memory_card_result == 1) {
                memory_card_stop();
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xF1) {
                memory_card_stop();
                mode_step_set(0xA0);
            } else if (gs->memory_card_result == 0xFF) {
                memory_card_stop();
                mode_step_set(0xFF);
            }
        } else if ((u32) gs->memory_card_timer >= 0x201U) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 1:
        memory_card_start();
        gs->substep += 1;
        break;
    case 2:
        if (memory_card_scan_poll() == 2) {
            gs->substep += 1;
        } else if ((u32) gs->memory_card_timer >= 0x201U) {
            mode_step_set(0xFF);
        }
        break;
    case 3:
        if ((u8) D_800E8BEE != 0) {
            memory_card_stop();
            memory_card_start();
            gs->substep += 1;
        } else {
            memory_card_stop();
            text_system_reset();
            set_kanji_string(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            k_disp_start(1);
            mode_step_next();
        }
        break;
    case 4:
        if (memory_card_system_read_poll() == 3) {
            memory_card_system_bank_load();
            text_system_reset();
            set_kanji_string(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            k_disp_start(1);
            mode_step_next();
            memory_card_stop();
        } else if (((u32) gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    }
    k_disp_inc();
}

s32 clear_save_stage_prepare(void) {
    switch (game_state.mode_step) {
    case 0:
        game_state.screen_brightness = 0;
        boot_data_load_step();
        break;
    case 1:
        boot_hud_textures_step();
        break;
    case 2:
        clear_save_ask_init();
        break;
    case 3:
        clear_save_ask_answer();
        break;
    }
}

/* Port of the main clone settings_save_load_save_init (T-17300); only the state codes differ. */
void clear_save_slot_menu_init(void) {
    s32 i;
    s32 d;
    SystemData *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    text_system_reset();
    message_window_set_title_hidden("データセーブ");
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
        mode_step_set(2);
        hud_save_slot_info_show(0, 0);
        settings_save_load_save_prompt();
    } else if (i == 0xF) {
        settings_save_load_save_prompt();
        mode_step_set(2);
    } else {
        game_state.save_slot = i;
        mode_step_set(0xC0);
    }
    settings_save_load_save_marks();
}

void clear_save_new_slot_step(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (game_state.memory_card_timer >= 0x201U || game_state.memory_card_result == 0xD0) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            mode_step_set(2);
            hud_save_slot_info_show(0, 0);
            settings_save_load_save_prompt();
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_step_set(0xFF);
        }
        break;
    }
    settings_save_load_save_marks();
}
void clear_save_slot_select(void) {
    s32 pad[3]; /* FAKE: T-24209, oversized local puts slot at sp+0x28 */
    s32 slot;

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    slot = game_state.menu_cursor_1 + 1;
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_1 != -1) {
            if (system_data.slot_status[slot] == 0) {
                /* FAKE: T-24209, byte view store + plain re-read lets uopt
                 * promote unk_1118 to v0 (andi v0; move a0) */
                ((u8 *)&game_state)[0x1118] = slot;
                if (memory_card_file_create(game_state.save_slot) == 1) {
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
                    /* FAKE: T-24209, repeated store through the byte view keeps
                     * unk_1118 promoted, so both arms store v0 before their calls */
                    ((u8 *)&game_state)[0x1118] = slot;
                    D_800E7D36 = game_state.save_slot;
                    hud_save_slot_info_show(0, 0);
                    settings_save_load_save_prompt();
                    game_state.menu_cursor_1 = D_800E7D36 - 1;
                } else {
                    mode_step_set(0xC0);
                }
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(2);
    }
    k_disp_inc();
    settings_save_load_save_marks();
}

void clear_save_write_step(void) {
    GameState *g = &game_state;

    switch (g->substep) {
    case 0:
        k_sub_reset();
        if (g->girl_cleared[g->current_girl & 0xF] < 0x63U) {
            g->girl_cleared[g->current_girl & 0xF] += 1;
        }
        if (system_data.flags[g->current_girl & 0xF] < 0x63U) {
            system_data.flags[g->current_girl & 0xF] += 1;
        }
        set_kanji_string(-0x80, 0x32, 1, (s32) "データセーブ中です", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        g->substep += 1;
        break;
    case 1:
        if (memory_card_save_write_poll() == 0x14) {
            D_800E7D14[g->save_slot] = 1;
            g->substep += 1;
            memory_card_stop();
        } else if (((u32) g->memory_card_timer >= 0x201U) || (g->memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if ((u8) D_800E8BEE != 0) {
            memory_card_start();
            g->memory_card_job = 1;
            D_800E7D36 = g->save_slot;
            g->save_slot = 0;
            memory_card_slot_summary_store();
            g->substep += 1;
        } else {
            mode_stage_set(2);
        }
        break;
    case 3:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            mode_stage_set(2);
        } else if (((u32) g->memory_card_timer >= 0x201U) || (g->memory_card_result == 0xF0)) {
            mode_step_set(0xFF);
        }
        break;
    }
    settings_save_load_save_marks();
}

s32 clear_save_no_card_step(void) {
    /* FAKE: scalar selector view keeps the selector in v1 for every compare. T-11350 */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        memory_card_stop();
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, (s32) "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, (s32) "メモリーカードを差し込み口１に", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "セットし、ボタンを押してください。", 0);
        k_sub_disp_start(2);
        game_state.substep += 1;
        break;
    case 3:
        if (game_state.pad_pressed & 0x860) {
            memory_card_start();
            mode_step_set(0);
        }
        break;
    }
}

/* Written in the shape of the NAME_ENT sibling func_80142028 (T-17300). Like the sibling it is s32
 * with no return statement: the incremented step stays in v0 on the way out. */
s32 clear_save_card_full_step(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_stop();
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
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
        set_kanji_string(-0x80, 0x32, 1, "別のメモリーカードに", 0);
        set_kanji_string(-0x80, 0x42, 1, "セーブしますか？", 0);
        set_kanji_string(-0x18, 4, 0xF, misc_card_yes_text, 0);
        set_kanji_string(0x3C, 4, 1, misc_card_confirm_label(), 0);
        set_kanji_string(-0x18, 0x14, 0xF, misc_card_no_text, 0);
        set_kanji_string(0x3C, 0x14, 2, misc_card_cancel_label(), 0);
        k_sub_disp_start(6);
        game_state.substep++;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            game_state.substep++;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            mode_stage_set(2);
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "別のメモリーカードを", 0);
        set_kanji_string(-0x80, 0x42, 1, "差し込み口１にセットして、", 0);
        set_kanji_string(-0x80, 0x52, 1, "ボタンを押してください。", 0);
        k_sub_disp_start(3);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed != 0) {
            memory_card_start();
            mode_step_set(0);
        }
        break;
    }
}

void clear_save_card_unformatted_step(void) {
    u8 temp;
    temp = game_state.substep;
    switch (temp) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep += 1;
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "初期化していいですか？", 0);
        set_kanji_string(-0x90, -0x12, 0, (s32) misc_card_yes_text, 0);
        set_kanji_string(-0x3C, -0x12, 1, misc_card_confirm_label(), 0);
        set_kanji_string(-0x90, -4, 0, (s32) misc_card_no_text, 0);
        set_kanji_string(-0x3C, -4, 2, misc_card_cancel_label(), 0);
        k_disp_start(5);
        game_state.substep += 1;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (memory_card_format() == 1) {
                game_state.substep = 0x10;
            } else {
                memory_card_start();
                mode_step_set(0);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep += 1;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "別のメモリーカードを", 0);
        set_kanji_string(-0x80, -0x2E, 1, (s32) "差し込み口１", 0);
        set_kanji_string(-0x80, -0x2E, 0xF, (s32) "　　　　　　にセットして、", 0);
        set_kanji_string(-0x80, -0x20, 0xF, (s32) "　ボタンを押してください。", 0);
        set_kanji_string(-0x80, -0x20, 1, misc_card_confirm_label(), 0);
        k_disp_start(5);
        game_state.substep += 1;
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            memory_card_start();
            mode_step_set(0);
        }
        break;
    case 16:
        if (memory_card_file_create(0) == 1) {
            game_state.substep += 1;
        } else {
            memory_card_start();
            mode_step_set(0);
        }
        break;
    case 17:
        D_800E7D36 = 0;
        *D_800E7D14 = 1;
        game_state.save_slot = 0;
        memory_card_start();
        memory_card_slot_summary_store();
        game_state.substep += 1;
        break;
    case 18:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_start();
            mode_step_set(0);
        }
        break;
    case 19:
        memory_card_start();
        mode_step_set(0);
        break;
    }
}

void clear_save_stage_run(void) {
    u8 sel = game_state.mode_step; /* FAKE: shared global, yet the original selector is in $v0; cause unknown (T-5010) */

    switch (sel) {
    case 0x0:
        clear_save_card_check_step();
        break;
    case 0x1:
        clear_save_slot_menu_init();
        break;
    case 0x2:
        clear_save_slot_select();
        break;
    case 0x3:
        clear_save_write_step();
        break;
    case 0xA0:
        clear_save_card_unformatted_step();
        break;
    case 0xC0:
        clear_save_new_slot_step();
        break;
    case 0xF0:
        clear_save_card_full_step();
        break;
    case 0xFF:
        clear_save_no_card_step();
        break;
    }
}

void clear_save_frame_draw(void) {
    if (game_state.mode_stage_timer != 0) {
        hud_screen_frame_draw(0);
    }
}

void clear_save_entry(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        clear_save_stage_prepare();
        break;
    case 1:
        clear_save_stage_run();
        break;
    case 2:
        clear_save_finish();
        break;
    }
    clear_save_frame_draw();
    clear_save_fade_panels_draw();
}
/* FAKE: the extra pointer homes retain the frame, and the intensity assignment
 * inside the call keeps its spill in the jal delay slot. The implicit return
 * preserves the original return-zero early exit and final call value. T-9720. */
s32 clear_save_fade_panels_draw(void)
{
    u8 *brightness;
    u8 *brightness_alias;
    s32 intensity;
    s32 color;
    brightness = &game_state.screen_brightness;
    if ((game_state.mode_stage == 0) && (((u8) game_state.mode_step) < 2U))
    {
        return 0;
    }
    brightness_alias = &(*brightness);
    prim_gradient_rect(-0xA0, -0x57, 0x11B, 0x7D, 2, (((s32) ((intensity = *brightness_alias) * 3)) / 2) * 0x10101, 2);
    dtd_on(2);
    color = (intensity / 3) * 0x40401;
    prim_gradient_rect(-0x85, 0x2E, 0x10A, 0x34, 2, color, 0x81);
    dtd_on_tpage(0, 0, 2, 0, 0);
}

/* FAKE: unused pad[2] places sp5C at 0x5C in the 0x88 frame, and the single-line for loop
 * prevents uopt from unrolling (T-24121). */
void last_photo_load_step(void) {
    s32 i;
    s32 j;
    s32 count;
    s32 sp5C[8];
    s32 pad[2];

    for (i = 0; i < 8; i++) { sp5C[i] = 0x80180000 + i * 0x4000; }
    switch (game_state.substep) {
    case 0:
        if (game_state.current_girl >= GIRL_YOSHIO) {
            game_mode_set(GAME_MODE_ETC_C4);
            return;
        }
        (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0xFF;
        back_clear_switch(1);
        draw2d3d(1, 0);
        sprite_tables_clear();
        actor_clear_upper();
        count = 0;
        j = 0xF;
        if (game_state.pad_held & PAD_START) {
            cd_read_sectors(0x31U, 0x80180000, last_photo_all_clear_lba);
            game_state.substep += 2;
        } else if ((game_state.year == 0x62) && (game_state.month == 3) && (game_state.day == 1)) {
            for (i = 0; i < 0xD; i++) {
                if ((game_state.girl_cleared[i] != 0) || (system_data.flags[i] != 0)) {
                    count += 1;
                } else {
                    j = i;
                }
            }
            if ((j == (game_state.current_girl & 0xF)) && (count >= 0xC)) {
                cd_read_sectors(0x31U, 0x80180000, last_photo_all_clear_lba);
                game_state.substep += 2;
            } else {
                cd_read_sectors(0x21U, 0x80180000, (&last_photo_lba)[game_state.current_girl]);
                game_state.substep += 1;
            }
        } else {
            cd_read_sectors(0x21U, 0x80180000, (&last_photo_lba)[game_state.current_girl]);
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            view_display_set_narrow_or_standard(1);
            prim_bg_slot_setup(sp5C, 0, 4, 2, 2, 0x1FF, -0x80, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            mode_stage_next();
            mode_step_set(0);
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup(sp5C, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80198000, 0x1F, 1, 3, 0);
            mode_stage_next();
            cd_audio_request_set(0x1D000077, 0x1000007B, 0);
            audio_command(0x300);
            mode_step_set(1);
        }
        break;
    }
    prim_flash_gray(0xFF);
}

void last_photo_stage_load(void) {
    if (game_state.mode_step == 0) {
        last_photo_load_step();
    }
}

void last_photo_fade_in_step(void) {
    back_clear_switch(1);
    if (game_state.substep != 0 && (u32)game_state.mode_step_timer.w < 0xFF) {
        prim_flash_gray(0xFF - game_state.mode_step_timer.w);
    }
}

void last_photo_music_step(void) {
    back_clear_switch(1);
    switch (game_state.substep) {
    case 3:
        break;
    case 0:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_audio_state() & 2) {
            game_state.substep += 1;
        }
        break;
    case 2:
        cd_audio_request_set(0x1B000000, 0x01000004, 0xE);
        audio_command(0x300);
        game_state.substep += 1;
        break;
    }
}

void last_photo_stage_show(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        last_photo_fade_in_step();
        break;
    case 1:
        last_photo_music_step();
        break;
    }
    /* (u32): the original compares the s32 counters unsigned (sltiu); T-1321 */
    if ((u32) game_state.mode_stage_timer < 0xFFU) {
        prim_flash_gray(0xFF - game_state.mode_stage_timer);
    }
    if (game_state.pad_pressed & 0x860) {
        mode_stage_next();
    } else if ((u32) game_state.mode_step_timer.w >= 0x709U) {
        mode_stage_next();
    }
}
void last_photo_leave_choose(void) {
    sprite_tables_clear();
    actor_clear_upper();
    func_8009C5E0(0);
    vram_clear_framebuffers();
    if (game_state.options_dirty != 0) {
        mode_step_set(3);
        return;
    }
    if (game_state.pad_prev & 0x860) {
        mode_step_set(1);
        return;
    }
    mode_step_set(2);
}

void last_photo_leave_to_clear_save(void) {
    if (game_state.video_width == 0x100) {
        view_display_set_narrow_or_standard(0);
    }
    game_mode_set(GAME_MODE_ETC_C4);
    func_8009C5E0(1);
}

void last_photo_leave_to_album(void) {
    if (game_state.video_width == 0x100) {
        view_display_set_narrow_or_standard(0);
    }
    game_mode_set(GAME_MODE_ALBUM);
    func_8009C5E0(1);
}

void last_photo_leave_to_title(void) {
    if (game_state.video_width == 0x100) {
        view_display_set_narrow_or_standard(0);
    }
    game_state.current_girl += 0x10;
    game_mode_set(GAME_MODE_TITLE);
    func_8009C5E0(1);
}

s32 last_photo_stage_leave(void) {
    switch (game_state.mode_step) {
    case 0:
        last_photo_leave_choose();
        break;
    case 1:
        last_photo_leave_to_clear_save();
        break;
    case 2:
        last_photo_leave_to_album();
        break;
    case 3:
        last_photo_leave_to_title();
        break;
    }
}

void last_photo_entry(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        last_photo_stage_load();
        break;
    case 1:
        last_photo_stage_show();
        break;
    case 2:
        last_photo_stage_leave();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}