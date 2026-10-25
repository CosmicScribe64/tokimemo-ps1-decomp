/* opening_and_sky_gradient.c: src/ovl/TACO/801447D0.c:4: opening fly-in and close-up sequences plus scrolling stars and rotating sky gradient quads; body 5: switch (game_state.mode_step) {; 7: shooter_opening_step_init();. Original object 0x801447D0-0x80146D60 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_stage_opening(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_opening_step_init();
        break;
    case 1:
        shooter_opening_step_fly_in();
        break;
    case 2:
        shooter_opening_step_first_close_up();
        break;
    case 3:
        shooter_opening_step_second_close_up();
        break;
    case 4:
        shooter_opening_step_end();
        break;
    }
    if (game_state.mode_step != 5) {
        if (game_state.pad_pressed & 0x860) {
            mode_step_set(4);
        }
    }
}

void shooter_opening_step_init(void) {
    (&game_state.clear_color_r)[0] = (&game_state.clear_color_r)[1] = 8;
    game_state.clear_color_b = 0x10;
    draw2d3d(1, 1);
    initView();
    shooter_set_dark_lights();
    shooter_init_object_hierarchy(-0xFA0);
    shooter_link_model_objects(0x8019C800);
    shooter_init_scrolling_stars((u8 *)game_state.scratch_words);
    shooter_init_scrolling_stars(game_state.scratch_buffer);
    func_8009C674(0);
    shooter_select_sky_palette(7);
    shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0);
    D_80127090.attribute = 0;
    D_80128880[1].x = -0x5A;
    D_80128880[1].z = 0;
    D_80128880[1].y = 0x5A;
    shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0);
    D_801270A0.attribute = 0;
    D_80128880[2].x = -0x5A;
    D_80128880[2].z = 0;
    D_80128880[2].y = -0x5A;
    mode_step_next();
}

void shooter_opening_step_end(void) {
    D_80127090.attribute = 0x80000000;
    D_801270A0.attribute = 0x80000000;
    draw2d3d(1, 0);
    mode_stage_set(2);
}

void shooter_draw_scrolling_stars(s32 *arg0, s32 arg1, s32 arg2) {
    /* FAKE: reserves the 0x130 bytes of frame the original has above its packet table; real locals unknown. T-19002 */
    u8 pad[0x130];
    POLY_F4 *pkt[25];
    s32 i;
    s32 x;
    s32 y;
    s32 fade;

    for (i = 0; i != 25; i++) {
        pkt[i] = (POLY_F4 *)GetWorkBase(0x18, draw_buffer);
        func_8009F02C(pkt[i]);
        SetSemiTrans(pkt[i], 1);
        SetShadeTex(pkt[i], 0);
        arg0[i * 2] += arg1;
        arg0[i * 2 + 1] += arg2;
        x = arg0[i * 2];
        y = arg0[i * 2 + 1] + game_state.screen_center_y[draw_buffer];
        if ((x < 0) || (x >= 0x141)) {
            if (arg1 > 0) {
                arg0[i * 2] = 0;
            } else {
                arg0[i * 2] = 0x140;
            }
            arg0[i * 2 + 1] = rand() % 240 - 0x78;
        } else if ((y < 0) || (y >= 0x1E1)) {
            arg0[i * 2] = rand() % 320;
            if (arg2 > 0) {
                arg0[i * 2 + 1] = -0x78;
            } else {
                arg0[i * 2 + 1] = 0x78;
            }
        }
        pkt[i]->x0 = x;
        pkt[i]->y0 = y;
        pkt[i]->x1 = x + 1;
        pkt[i]->y1 = y;
        pkt[i]->x2 = x;
        pkt[i]->y2 = y + 1;
        pkt[i]->x3 = x + 1;
        pkt[i]->y3 = y + 1;
        if ((rand() % 5) >= 3) {
            pkt[i]->r0 = 0x80;
            pkt[i]->g0 = 0x80;
            pkt[i]->b0 = 0x80;
        } else {
            pkt[i]->r0 = 0xFF;
            pkt[i]->g0 = 0xFF;
            pkt[i]->b0 = 0xFF;
        }
        if ((rand() % 3) > 0) {
            AddPrim((draw_buffer << 10) + 0x190 + D_800E8CA0, (s32)pkt[i]);
        }
    }
    fade = GetWorkBase(0xC, draw_buffer);
    func_8009D294(fade, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    AddPrim((draw_buffer << 10) + D_800E8CA0, fade);
    safe_env(0);
}

void shooter_init_scrolling_stars(u8 *arg0) {
    s32 i;

    for (i = 0; i != 0x19; i++) {
        *(s32 *)(arg0 + i * 8) = rand() % 320;
        *(s32 *)(arg0 + i * 8 + 4) = rand() % 240 - 0x78;
    }
}

void shooter_select_sky_palette(s32 arg0) {
    RECT r;

    if (arg0 < 0) {
        arg0 = 0;
    } else if (arg0 >= 8) {
        arg0 = 7;
    }
    /* FAKE: the assignment pairs share a source line each; as1 orders the stores by line (T-3330), T-6050 */
    r.x = arg0 * 8; r.y = 0x1F1;
    r.w = 8; r.h = 1;
    func_8009C93C(&r, 0xF8, 0x1F0);
}

void shooter_reset_model_slot(s32 arg0) {
    ShooterCoordinate *o;
    ShooterPosition *p;

    o = &D_80127480[arg0];
    p = &D_80128880[arg0];
    o->x = 0;
    o->y = 0;
    o->z = 0;
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

void shooter_draw_sky_gradient(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, s32 arg6) {
    /* FAKE: pad and local order preserve the original 0x90 frame and byte homes. T-24502 */
    s32 pad[1];
    POLY_G4 *polys[3];
    POLY_G4 **end;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 r0;
    u8 g0;
    u8 b0;
    s32 i;
    POLY_G4 **p;

    /* FAKE: grouping preserves the original loop bound and initialization scheduling. T-24502 */
    p = polys; i = 0; do {
        *p = (POLY_G4 *)GetWorkBase(sizeof(POLY_G4), draw_buffer);
        func_8009F054((s32) *p);
        SetSemiTrans(*p, 1);
        SetShadeTex(*p, 1);
        switch (arg5) {
        case 0:
            b2 = 0xFF;
            g2 = 0xFF;
            r2 = 0xFF;
            g0 = 0x20;
            r0 = 0x20;
            b0 = 0x50;
            if ((u8) arg4 < 0xF8U) {
                g2 = r2 = arg4 + 8;
            }
            if ((u8) arg4 < 0xF0U) {
                b2 = ((u8) arg4 + 0x10);
            }
            if ((u8) arg4 < 0x18U) {
                g0 = r0 = arg4 + 8;
            }
            if (arg4 < 0x40) {
                b0 = ((u8) arg4 + 0x10);
            }
            break;
        case 1:
            g0 = 8;
            r0 = 8;
            g2 = 8;
            r2 = 8;
            b0 = 0x10;
            b2 = 0x10;
            if ((u8) arg4 < 0xF8U) {
                g2 = r2 = 0xFF - arg4;
            }
            if ((u8) arg4 < 0xF0U) {
                b2 = (0xFF - (u8) arg4);
            }
            if ((u8) arg4 < 0x18U) {
                g0 = r0 = 0x20 - arg4;
            }
            if ((u8) arg4 < 0x40U) {
                b0 = (0x50 - (u8) arg4);
            }
            break;
        case 2:
            b2 = 0xFF;
            g2 = 0xFF;
            r2 = 0xFF;
            g0 = 0x20;
            r0 = 0x20;
            b0 = 0x50;
            break;
        }
        switch (i) {
        case 0:
            (*p)->x0 = (s16) arg0;
            (*p)->y0 = (s16) (game_state.screen_center_y[draw_buffer] + arg1);
            (*p)->x1 = (arg0 + 0x140);
            (*p)->y1 = (s16) (game_state.screen_center_y[draw_buffer] + arg1);
            (*p)->x2 = (s16) arg0;
            (*p)->y2 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3));
            (*p)->x3 = (arg0 + 0x140);
            (*p)->y3 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3));
            (*p)->r0 = (u8) r0;
            (*p)->g0 = g0;
            (*p)->b0 = (u8) b0;
            (*p)->r1 = (u8) r0;
            (*p)->g1 = g0;
            (*p)->b1 = (u8) b0;
            (*p)->r2 = r2;
            (*p)->g2 = g2;
            (*p)->b2 = b2;
            (*p)->r3 = r2;
            (*p)->g3 = g2;
            (*p)->b3 = b2;
            break;
        case 1:
            (*p)->x0 = (s16) arg0;
            (*p)->y0 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2));
            (*p)->x1 = (arg0 + 0x140);
            (*p)->y1 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2));
            (*p)->x2 = (s16) arg0;
            (*p)->y2 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2) + (arg2 / 3));
            (*p)->x3 = (arg0 + 0x140);
            (*p)->y3 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2) + (arg2 / 3));
            (*p)->r0 = r2;
            (*p)->g0 = g2;
            (*p)->b0 = b2;
            (*p)->r1 = r2;
            (*p)->g1 = g2;
            (*p)->b1 = b2;
            (*p)->r2 = (u8) r0;
            (*p)->g2 = g0;
            (*p)->b2 = (u8) b0;
            (*p)->r3 = (u8) r0;
            (*p)->g3 = g0;
            (*p)->b3 = (u8) b0;
            break;
        case 2:
            (*p)->x0 = (s16) arg0;
            (*p)->y0 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3));
            (*p)->x1 = (arg0 + 0x140);
            (*p)->y1 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3));
            (*p)->x2 = (s16) arg0;
            (*p)->y2 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2));
            (*p)->x3 = (arg0 + 0x140);
            (*p)->y3 = (s16) (game_state.screen_center_y[draw_buffer] + arg1 + (arg2 / 3) + (arg2 / 2));
            (*p)->r0 = r2;
            (*p)->g0 = g2;
            (*p)->b0 = b2;
            (*p)->r1 = r2;
            (*p)->g1 = g2;
            (*p)->b1 = b2;
            (*p)->r2 = r2;
            (*p)->g2 = g2;
            (*p)->b2 = b2;
            (*p)->r3 = r2;
            (*p)->g3 = g2;
            (*p)->b3 = b2;
            break;
        }
        i += 1;
        p += 1;
    } while (i < 3);
    if (arg6 > 0) {
        shooter_rotate_gradient_quads((s32) polys, arg6, arg2);
    }
    /* FAKE: grouping keeps the saved offset/base initialization before the pointer loop. T-24502 */
    i = arg3 * 4; p = polys; end = polys + 3; do {
        AddPrim(D_800E8CA0 + (draw_buffer << 0xA) + i, *p);
        p += 1;
    } while (p != end);
    p = (POLY_G4 **)GetWorkBase(0xC, draw_buffer);
    func_8009D294((s32) p, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    AddPrim(D_800E8CA0 + (draw_buffer << 0xA) + i, (s32) p);
    safe_env(arg3);
}


void shooter_draw_sky_gradient_phase(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if ((arg4 >= 0) && (arg4 < 0x1F)) {
        shooter_draw_sky_gradient(arg0, arg1, arg2, arg3, (arg4 & 0xFF) * 8, 0, arg5);
        return;
    }
    if ((arg4 >= 0x1F) && (arg4 < 0x23)) {
        shooter_draw_sky_gradient(arg0, arg1, arg2, arg3, 0, 2, arg5);
        return;
    }
    if ((arg4 >= 0x23) && (arg4 < 0x61)) {
        shooter_draw_sky_gradient(arg0, arg1, arg2, arg3, ((arg4 - 0x23) & 0xFF) * 4, 1, arg5);
    }
}

s32 shooter_rotate_gradient_quads(POLY_G4 **arg0, s32 arg1, s32 arg2) {
    s32 angle;
    SVECTOR input;
    SVECTOR output;
    MATRIX matrix;
    s32 x;
    s32 y;

    x = arg0[2]->x0;
    y = arg0[2]->y0;
    input.vx = 0;
    input.vy = 0;
    input.vz = 0;
    matrix = *(MATRIX *)D_80125C60;
    angle = (arg1 << 12) / 360;
    matrix.t[0] = 0;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    shooter_rotation_matrix_z(angle, &matrix);
    ApplyMatrixSV(&matrix, &input, &output);
    arg0[2]->x0 = output.vx + x;
    arg0[2]->y0 = output.vy + y;
    input.vx = arg0[2]->x1 - x;
    input.vy = arg0[2]->y1 - y;
    input.vz = 0;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    matrix.t[0] = input.vx;
    shooter_rotation_matrix_z(angle, &matrix);
    ApplyMatrixSV(&matrix, &input, &output);
    arg0[2]->x1 = output.vx + x;
    arg0[2]->y1 = output.vy + y;
    input.vx = arg0[2]->x2 - x;
    input.vy = arg0[2]->y2 - y;
    input.vz = 0;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    matrix.t[0] = input.vx;
    shooter_rotation_matrix_z(angle, &matrix);
    ApplyMatrixSV(&matrix, &input, &output);
    arg0[2]->x2 = output.vx + x;
    arg0[2]->y2 = output.vy + y;
    input.vx = arg0[2]->x3 - x;
    input.vy = arg0[2]->y3 - y;
    input.vz = 0;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    matrix.t[0] = input.vx;
    shooter_rotation_matrix_z(angle, &matrix);
    ApplyMatrixSV(&matrix, &input, &output);
    arg0[2]->x3 = output.vx + x;
    arg0[2]->y3 = output.vy + y;
    arg0[0]->x0 = arg0[2]->x0 + rsin(angle) * arg2 / 3 / 4096;
    arg0[0]->y0 = arg0[2]->y0 - func_800A0140(angle) * arg2 / 3 / 4096;
    arg0[0]->x1 = arg0[2]->x1 + rsin(angle) * arg2 / 3 / 4096;
    arg0[0]->y1 = arg0[2]->y1 - func_800A0140(angle) * arg2 / 3 / 4096;
    arg0[0]->x2 = arg0[2]->x0;
    arg0[0]->y2 = arg0[2]->y0;
    arg0[0]->x3 = arg0[2]->x1;
    arg0[0]->y3 = arg0[2]->y1;
    arg0[1]->x0 = arg0[2]->x2;
    arg0[1]->y0 = arg0[2]->y2;
    arg0[1]->x1 = arg0[2]->x3;
    arg0[1]->y1 = arg0[2]->y3;
    arg0[1]->x2 = arg0[2]->x2 - rsin(angle) * arg2 / 3 / 4096;
    arg0[1]->y2 = arg0[2]->y2 + func_800A0140(angle) * arg2 / 3 / 4096;
    arg0[1]->x3 = arg0[2]->x3 - rsin(angle) * arg2 / 3 / 4096;
    arg0[1]->y3 = arg0[2]->y3 + func_800A0140(angle) * arg2 / 3 / 4096;
}

s32 shooter_opening_step_fly_in(void) {
    s32 t;
    s32 i;

    D_8012274C = 0;
    D_80122750 = D_80127480[1].y;
    t = game_state.mode_step_timer.w;
    D_80122754 = -0x7D0;
    func_80099540(&D_80122740);
    switch (game_state.substep) {
    case 0:
        shooter_draw_scrolling_stars(game_state.scratch_words, 0, 0xA);
        shooter_draw_scrolling_stars((s32 *)game_state.scratch_buffer, 0, 8);
        if (t < 0x1E) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 1);
            }
        } else if ((t >= 0x1E) && (t < 0x5A)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 2);
            }
        } else if ((t >= 0x5A) && (t < 0x96)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 3);
            }
        } else if ((t >= 0x96) && (t < 0xB4)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 4);
            }
        } else if ((t >= 0xB4) && (t < 0xD2)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 5);
            }
        } else if ((t >= 0xD2) && (t < 0x10E)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 6);
            }
        } else if ((t >= 0x10E) && (t < 0x14A)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 7);
            }
        } else if ((t >= 0x14A) && (t < 0x168)) {
            for (i = 0; i != 2; i++) {
                shooter_apply_keyframe(i + 1, shooter_opening_pose_tables[i], 8);
            }
        } else if (t == 0x168) {
            game_state.mode_step_timer.w = 0;
            game_state.substep += 1;
        }
        if (t == 0x1E) {
            shooter_play_se(0, 0x505);
        }
        if (t == 0xAA) {
            shooter_play_se(0, 0x507);
        }
        if (t == 0x12C) {
            shooter_play_se(0, 0x506);
        }
        break;
    case 1:
        if (t == 1) {
            D_80127080[1].attribute = 0x80000000;
            D_80127080[2].attribute = 0x80000000;
        } else if (t >= 0x3C) {
            mode_step_next();
        }
        break;
    }
    game_state.mode_step_timer.w += 1;
}

s32 shooter_opening_step_first_close_up(void) {
    s32 t;

    t = game_state.mode_step_timer.w;
    if (t == 1) {
        shooter_reset_model_slot(1);
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 9);
        D_80127090.attribute = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((t >= 2) && (t < 0x32)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0xA);
    } else if ((t >= 0x32) && (t < 0x64)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0xB);
    } else if ((t >= 0x64) && (t < 0x78)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0xC);
    } else if ((t >= 0x78) && (t < 0x91)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0xD);
    } else if ((t >= 0x91) && (t < 0xB4)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0xE);
    } else if ((t >= 0xB4) && (t < 0xC8)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x10);
    } else if ((t >= 0xC8) && (t < 0xEB)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x11);
    } else if ((t >= 0xEB) && (t < 0x11D)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x12);
    } else if ((t >= 0x11D) && (t < 0x14F)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x13);
    } else if ((t >= 0x14F) && (t < 0x1C2)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x14);
    } else if ((t >= 0x1CC) && (t < 0x1EA)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x15);
    } else if ((t >= 0x1F4) && (t < 0x208)) {
        shooter_apply_keyframe(1, shooter_opening_pose_tables[0], 0x16);
    }
    if ((t >= 0x64) && (t < 0xBE)) {
        shooter_draw_sky_gradient_phase(0, -0x41, 0x32, 0x11, t - 0x55, 0);
    }
    if ((t >= 0xB4) && (t < 0x118)) {
        shooter_draw_sky_gradient_phase(0, -0x1E, 0x1E, 0x4B, t - 0xAA, 0);
    }
    if ((t >= 0xAA) && (t < 0x10E)) {
        shooter_draw_sky_gradient_phase(0, 0x32, 0x3C, 0xA, t - 0xAA, 0);
    }
    if ((t >= 0xBE) && (t < 0x122)) {
        shooter_draw_sky_gradient_phase(0, -0x50, 0xA, 0x50, t - 0xBE, 0);
    }
    if ((t >= 0xEB) && (t < 0x14F)) {
        shooter_draw_sky_gradient_phase(0, -0x3C, 0x19, 0x14, t - 0xE1, 0);
    }
    if ((t >= 0x10E) && (t < 0x168)) {
        shooter_draw_sky_gradient_phase(0, -0x17, 0x1C, 0x1E, t - 0xFA, 0);
    }
    if ((t >= 0x11D) && (t < 0x181)) {
        shooter_draw_sky_gradient_phase(0, 0x14, 0x23, 0x28, t - 0x113, 0);
    }
    if ((t >= 0x136) && (t < 0x19A)) {
        shooter_draw_sky_gradient_phase(0, 0x3C, 0x2D, 0x1E, t - 0x12C, 0);
    }
    if ((t >= 0x154) && (t < 0x1B8)) {
        shooter_draw_sky_gradient_phase(0, -0x50, 0x3C, 0x14, t - 0x140, 0);
    }
    if ((t >= 0x168) && (t < 0x1CC)) {
        shooter_draw_sky_gradient_phase(0, 0, 0x14, 0x1E, t - 0x168, 0);
    }
    if ((t >= 0x186) && (t < 0x1EA)) {
        shooter_draw_sky_gradient_phase(0, 0x28, 0x32, 0x1E, t - 0x186, 0);
    }
    if ((t >= 0x1AE) && (t < 0x212)) {
        shooter_draw_sky_gradient_phase(0, -0x5A, 0x14, 0x1E, t - 0x1AE, 0);
    }
    if ((t >= 0x1CC) && (t < 0x230)) {
        shooter_draw_sky_gradient_phase(0, -0x28, 0x28, 0x1E, t - 0x1CC, 0);
    }
    if ((t >= 0x1E0) && (t < 0x244)) {
        shooter_draw_sky_gradient_phase(0, 0x32, 0x3C, 5, t - 0x1E0, 0);
    }
    if (t == 0xA) {
        shooter_play_se(0, 0x505);
    }
    if (t == 0x64) {
        shooter_play_se(0, 0x506);
    }
    if (t == 0xDC) {
        shooter_play_se(0, 0x506);
    }
    if (t == 0x190) {
        shooter_play_se(0, 0x507);
    }
    if (t == 0x1E0) {
        shooter_play_se(0, 0x505);
    }
    if (t < 0x244) {
        shooter_draw_scrolling_stars(game_state.scratch_words, -0xA, 0);
        shooter_draw_scrolling_stars((s32 *)game_state.scratch_buffer, -8, 0);
    }
    if (t == 0x280) {
        D_80127090.attribute = 0x80000000;
        mode_step_next();
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 165
     * rows off. Keep line with proof=shooter_opening_step_first_close_up:165:bb8762f0 in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-21520 */
    D_800E7384 += 1;
}

s32 shooter_opening_step_second_close_up(void) {
    s32 t;
    s32 x;

    t = game_state.mode_step_timer.w;
    if (t == 1) {
        shooter_reset_model_slot(2);
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 9);
        D_80127080[2].attribute = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((t >= 2) && (t < 0x64)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xA);
    } else if ((t >= 0x65) && (t < 0x78)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xB);
    } else if ((t >= 0x79) && (t < 0x8C)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xC);
    } else if ((t >= 0x8D) && (t < 0xA0)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xD);
    } else if ((t >= 0xA1) && (t < 0xB4)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xE);
    } else if ((t >= 0xB5) && (t < 0xC8)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0xF);
    } else if ((t >= 0xC9) && (t < 0xDC)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x10);
    } else if ((t >= 0xDD) && (t < 0xFA)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x11);
    } else if ((t >= 0xFB) && (t < 0x122)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x12);
    } else if ((t >= 0x123) && (t < 0x15E)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x13);
    } else if ((t >= 0x15F) && (t < 0x186)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x14);
    } else if ((t >= 0x187) && (t < 0x1A4)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x15);
    } else if ((t >= 0x1A5) && (t < 0x1F4)) {
        shooter_apply_keyframe(2, shooter_opening_pose_tables[1], 0x16);
    }
    if ((t >= 0x32) && (t < 0x96)) {
        shooter_draw_sky_gradient_phase(0x14, -0xAA, 0x1E, 0x14, t - 0x1E, 0x3C);
    }
    if ((t >= 0x69) && (t < 0xC8)) {
        shooter_draw_sky_gradient_phase(0xAA, -0xAA, 0x50, 8, t - 0x50, 0x3C);
    }
    if ((t >= 0x78) && (t < 0xDC)) {
        shooter_draw_sky_gradient_phase(-0x1E, -0xAA, 0x50, 0xA, t - 0x78, 0x3C);
    }
    if ((t >= 0xB4) && (t < 0x118)) {
        shooter_draw_sky_gradient_phase(0xC8, -0xAA, 0x32, 0xF, t - 0xAA, 0x3C);
    }
    if ((t >= 0xDC) && (t < 0x140)) {
        shooter_draw_sky_gradient_phase(0x5A, -0xAA, 0x3C, 0xA, t - 0xD2, 0x3C);
    }
    if ((t >= 0x122) && (t < 0x136)) {
        shooter_draw_sky_gradient_phase(0xF, -0x96, 0x2D, 0xA, t - 0x118, 0x3C);
    }
    if ((t >= 0x136) && (t < 0x186)) {
        shooter_draw_sky_gradient_phase((t * 2) - 0x25D, -0x96, 0x2D, 0xA, t - 0x118, (t - 0x136) / 3 + 0x3C);
    }
    if ((t >= 0x190) && (t < 0x1F4)) {
        shooter_draw_sky_gradient_phase(0x15E, -0x96, 0x50, 0xA, t - 0x186, 0x78);
    }
    if ((t >= 0x1A4) && (t < 0x1F4)) {
        shooter_draw_sky_gradient_phase(0xDC, -0x96, 0x3C, 0x14, t - 0x19A, 0x78);
    }
    if ((t >= 0x1B8) && (t < 0x21C)) {
        shooter_draw_sky_gradient_phase(0x96, -0x96, 0x28, 0x14, t - 0x1AE, 0x78);
    }
    if (t == 0x64) {
        shooter_play_se(0, 0x507);
    }
    if (t == 0xE6) {
        shooter_play_se(0, 0x505);
    }
    if (t == 0x12C) {
        shooter_play_se(0, 0x506);
    }
    if (t == 0x190) {
        shooter_play_se(0, 0x505);
    }
    if (t < 0x12C) {
        shooter_draw_scrolling_stars(game_state.scratch_words, -5, -0xA);
        shooter_draw_scrolling_stars((s32 *)game_state.scratch_buffer, -5, -8);
    } else if ((t >= 0x12C) && (t < 0x190)) {
        x = (t - 0x12C) / 10 - 5;
        shooter_draw_scrolling_stars(game_state.scratch_words, x, -0xA);
        shooter_draw_scrolling_stars((s32 *)game_state.scratch_buffer, x, -8);
    } else if (t >= 0x190) {
        shooter_draw_scrolling_stars(game_state.scratch_words, 5, -0xA);
        shooter_draw_scrolling_stars((s32 *)game_state.scratch_buffer, 5, -8);
    }
    if (t == 0x226) {
        mode_step_set(4);
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 156
     * rows off. Keep line with proof=shooter_opening_step_second_close_up:156:5e8407a9 in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-24129 */
    D_800E7384 += 1;
}
