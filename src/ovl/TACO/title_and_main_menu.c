/* title_and_main_menu.c: src/ovl/TACO/80134930.c:16 initializes animated title models and lights; :94 runs title presentation; :330 dispatches selected main-menu row to play/settings/rankings/instructions/exit. Original object 0x80134930-0x80135720 (config/objects/TACO.txt). */
#define MAIN_API_OVERRIDE_view_light_0 /* three-light block (3 x 16 bytes) sharing base symbol for as1 load hoisting (T-24107) */
#include "common.h"
#include "ovl/TACO.h"

typedef struct {
    s32 light0_vector[3];
    u8 r0, g0, b0, pad0;
    s32 light1_vector[3];
    u8 r1, g1, b1, pad1;
    s32 light2_vector[3];
    u8 r2, g2, b2, pad2;
} ShooterLightBlock;

extern ShooterLightBlock view_light_0;

void shooter_title_setup(void) {
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    game_state.clear_color_b = 0;
    draw2d3d(1, 1);
    initView();
    shooter_init_object_hierarchy(-0xFA0);
    initModelingData_init(0x8019C800);
    shooter_link_model_objects(0x8019C800);
    shooter_apply_keyframe(1, shooter_title_pose_tables[0], 0);
    D_80127090.attribute = 0;
    D_80128880[1].x = 0x32;
    D_80128880[1].z = 0x32;
    D_80128880[1].y = 0xD2;
    shooter_apply_keyframe(2, shooter_title_pose_tables[1], 0);
    D_801270A0.attribute = 0;
    D_80128880[2].x = 0x4B;
    D_80128880[2].z = -0x32;
    D_80128880[2].y = 0xC3;
    shooter_title_frame = 0;
}

s32 shooter_title_animate(void) {
    s32 i;
    s32 frame;

    frame = shooter_title_frame;
    if (frame < 0x20) {
        if (view_light_0.r0 < 0xC0) {
            /* Chained stores: IDO stores the middle and left targets first and the read one last, the original order. */
            view_light_0.r0 = view_light_0.g0 = view_light_0.b0 = view_light_0.b0 + 6;
            view_light_0.r1 = view_light_0.g1 = view_light_0.b1 = view_light_0.b1 + 1;
            /* FAKE: repeated masked sum makes a uopt CSE range in $v0 while keeping the ugen temporary slot (T-22006, T-24107). */
            view_light_0.g2 = (view_light_0.b2 + 1) & 0xFF;
            view_light_0.r2 = view_light_0.b2 = (view_light_0.b2 + 1) & 0xFF;
            func_8009AD70(0, (s32) &view_light_0);
            func_8009AD70(1, (s32) &view_light_1);
            func_8009AD70(2, (s32) &view_light_2);
        }
        if (frame < 0xF) {
            game_state.clear_color_b += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe(i + 1, shooter_title_pose_tables[i], 7);
        }
    } else if (frame >= 0xF0) {
        return 1;
    }
    shooter_title_frame = shooter_title_frame + 1;
    return 0;
}


void shooter_stage_title(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_title_step_init();
        break;
    case 1:
        shooter_title_step_wait();
        break;
    case 2:
        shooter_title_step_reveal();
        break;
    case 3:
        shooter_title_step_hold();
        break;
    case 4:
        shooter_title_step_advance();
        break;
    case 5:
        shooter_title_step_finish();
        break;
    }
    shooter_title_animate();
    shooter_draw_title_stars();
    if (game_state.mode_stage_timer == 0xA0) {
        shooter_play_se(0, 0x504);
    }
    if (game_state.mode_stage_timer == 0x40) {
        shooter_play_se(0, 0x505);
    }
    if (game_state.mode_stage_timer == 0x80) {
        shooter_play_se(0, 0x506);
    }
    if (game_state.mode_stage_timer == 0xC0) {
        shooter_play_se(0, 0x507);
    }
}

void shooter_title_step_init(void) {
    s32 i;

    shooter_background_flags = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    shooter_menu_cursor = 0;
    game_state.clear_color_b = 0;
    view_model_slots_clear();
    draw2d3d(1, 1);
    actor_draw_enable(1);
    k_reset(0);
    StoreSquare(0, 0x1EF, 0x100, 1, &game_state.scratch_words[0]);
    for (i = 56; i < 72; i++) {
        game_state.scratch_words[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        game_state.scratch_words[128 + i] = 0;
    }
    load_palette(&game_state.scratch_words[0], 0x1F, 1, 1, 0);
    shooter_setup_text_layer();
    shooter_title_setup();
    mode_step_next();
}

void shooter_setup_text_layer(void) {
    actor_clear(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = game_state.ovl_frame_table;
    D_8011ED24 = game_state.ovl_sequence_table;
    D_8011ED48 = game_state.ovl_tpage_table;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED3A = -0xA0;
    D_8011ED3E = -0x64;
}

void shooter_draw_title_stars(void) {
    /* FAKE: unused 4-byte local reserves the top frame word so the $a3 spill sits at 0x3C($sp) in the 0x48 frame (T-24120). */
    s32 pad[1];
    s32 i;
    u32 x;
    u32 y;

    for (i = 0; i < 0x80; i++) {
        if (game_state.scratch_words[128 + i] == 0) {
            x = rand() % 320;
            y = rand() % 240;
            game_state.scratch_words[128 + i] = (x << 16) + y;
            break;
        }
    }
    for (i = 0; i < 0x80; i++) {
        if (game_state.scratch_words[128 + i] != 0) {
            x = (u32)game_state.scratch_words[128 + i] >> 16;
            y = game_state.scratch_words[128 + i] & 0xFFFF;
            shooter_draw_dot(x - 0xA0, y - 0x78, (x * y + game_state.frame_count) & 0xFFFFFF);
        }
    }
}


s32 shooter_title_reveal_step(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 56; i < 72; i++) {
        if (game_state.scratch_words[i] == 0) {
            if ((rand() & 0xFF) < 0x7F && n == 0) {
                game_state.scratch_words[i] = ((s32 *)game_state.ovl_palette_a)[i];
            } else {
                n++;
            }
        }
    }
    load_palette(&game_state.scratch_words[0], 0x1F, 1, 1, 0);
    return n;
}

void shooter_title_step_wait(void) {
    if ((u32)game_state.mode_step_timer.w >= 0xD1U) {
        mode_step_next();
    } else if (game_state.pad_pressed & 0x860) {
        mode_step_set(5);
    }
}

void shooter_title_step_reveal(void) {
    if (shooter_title_reveal_step() == 0) {
        mode_step_next();
    } else if (game_state.pad_pressed & 0x860) {
        mode_step_set(5);
    }
}

void shooter_title_step_hold(void) {
    if ((u32)game_state.mode_step_timer.w < 0xFFU) {
        if (game_state.pad_pressed != 0) {
            mode_step_set(5);
            return;
        }
        prim_flash_gray(0xFF - game_state.mode_step_timer.w);
        return;
    }
    mode_step_next();
}

void shooter_title_step_advance(void) {
    mode_step_next();
}

void shooter_title_step_finish(void) {
    view_model_slots_clear();
    load_palette(game_state.ovl_palette_a, 0x1F, 1, 1, 0);
    mode_stage_set(3);
}

void shooter_stage_menu(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_menu_step_init();
        break;
    case 1:
        shooter_menu_step_select();
        break;
    case 2:
        shooter_menu_step_confirm();
        break;
    }
    shooter_draw_title_stars();
}
void shooter_menu_step_init(void) {
    func_8009C210(0);
    view_model_slots_clear();
    view_display_set_wide_or_standard(0);
    shooter_setup_text_layer();
    D_8011ED1B = 0x40;
    D_8011ED3E = -0x78;
    mode_step_next();
}

void shooter_menu_step_select(void) {
    if (game_state.pad_pressed & 0x5100) {
        if (game_state.pad_pressed & PAD_UP) {
            shooter_menu_cursor -= 1;
        }
        if (game_state.pad_pressed & PAD_DOWN) {
            shooter_menu_cursor += 1;
        }
        if (game_state.pad_pressed & PAD_SELECT) {
            shooter_menu_cursor += 1;
        }
        if (shooter_menu_cursor < 0) {
            shooter_menu_cursor = 4;
        }
        if (shooter_menu_cursor >= 5) {
            shooter_menu_cursor = 0;
        }
        shooter_play_se(0, 0x500);
    }
    if (game_state.pad_pressed & 0x820) {
        shooter_play_se(0, 0x501);
        mode_step_next();
    }
    shooter_menu_draw_item();
    if ((u32) game_state.mode_stage_timer >= 0x385U) {
        mode_stage_set(6);
    }
}

/* The menu index shooter_menu_cursor is an s8 elsewhere (compared < 0); these switches read it as the unsigned byte the original uses. T-16560 */
s32 shooter_menu_draw_item(void) {
    switch (*(u8 *)&shooter_menu_cursor) {
    case 0:
        shooter_draw_text("GAME START", -0x40, 0x48, 2);
        break;
    case 1:
        shooter_draw_text("  OPTION  ", -0x40, 0x48, 3);
        break;
    case 2:
        shooter_draw_text("  SCORES  ", -0x40, 0x48, 5);
        break;
    case 3:
        shooter_draw_text("OPERATIONS", -0x40, 0x48, 6);
        break;
    case 4:
        shooter_draw_text("   EXIT   ", -0x40, 0x48, 0);
        break;
    }
}

/* The switch reads shooter_menu_cursor as an unsigned byte, as shooter_menu_draw_item does. T-16560 */
s32 shooter_menu_step_confirm(void) {
    if ((u32) game_state.mode_step_timer.u >= 0x41U) {
        switch (*(u8 *)&shooter_menu_cursor) {
        case 0:
            mode_stage_set(0xA);
            break;
        case 1:
            mode_stage_set(4);
            break;
        case 2:
            mode_stage_set(5);
            break;
        case 3:
            mode_stage_set(7);
            break;
        case 4:
            mode_stage_set(0x2E);
            break;
        }
    }
    if (game_state.mode_stage != 3) {
        actor_draw_enable(0);
    }
    if (game_state.mode_step_timer.u & 4) {
        shooter_menu_draw_item();
    }
}
