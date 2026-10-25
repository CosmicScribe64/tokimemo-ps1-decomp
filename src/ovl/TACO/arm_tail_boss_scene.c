/* arm_tail_boss_scene.c: src/ovl/TACO/80148FE0.c:86: boss scene sets arm poses, runs arm/core/tail attack controller and handles defeat; body 89: for (i = 0; i != 0xC; i++) {; 90: shooter_boss2_update_part(shooter_boss2_part_objects[i], i, shooter_boss2_part_offsets, shooter_boss2_part_radius, shooter_boss2_part_hit_points);. Original object 0x80148FE0-0x80149F90 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_stage_boss2(void) {
    if (((u32) game_state.mode_stage_timer < 0x60U) && (game_state.frame_count & 0x10)) {
        shooter_draw_text("2ST BOSS", -0x30, 0, 2);
    }
    switch (game_state.mode_step) {
    case 0:
        shooter_boss2_step_init();
        break;
    case 1:
        shooter_boss2_step_entrance();
        break;
    case 2:
        shooter_boss2_step_fight();
        break;
    case 3:
        shooter_boss2_step_defeat();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_boss2_step_init(void) {
    s32 i;
    s32 *a;
    u8 *b;
    ShooterModelHandle *c;
    s32 *d;

    if (shooter_load_request == 0) {
        shooter_load_request = 0x30;
        shooter_load_texture_page(0x80172000, 0xE);
        shooter_load_texture_page(0x80162000, 0xC);
        shooter_set_dark_lights();
        shooter_init_object_hierarchy(0);
        shooter_link_model_objects(0x80196000);
        shooter_read_model_polygon_ranges(0x80196000);
        shooter_load_texture_page(0x80162000, 8);
        shooter_move_model(0, shooter_actors->x, shooter_actors->y + 0x44C, -0x640, -0xA, 0xB4, 0, 0, 0);
        for (i = 1; i < 0x24; i++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss2_rest_pose_pointers[i - 1].translation, shooter_boss2_rest_pose_pointers[i - 1].rotation, shooter_boss2_rest_pose_flags[i - 1].translation_fixed, shooter_boss2_rest_pose_flags[i - 1].rotation_fixed);
            D_80127080[i].attribute = 0;
        }
        D_80128880[0].x = -0xA;
        D_80128880[0].y = 0xB4;
        D_80128880[0].z = 0;
        shooter_boss2_rest_arm(1, 0);
        shooter_boss2_rest_arm(2, 7);
        shooter_boss2_rest_arm(3, 0xE);
        d = shooter_boss2_tail_poses;
        for (i = 0x1D; i != 0x21; i++) {
            shooter_set_rotation_from_pose(i, *d);
            d += 1;
        }
        shooter_set_rotation_from_pose(0x23, shooter_boss2_core_pose);
        shooter_boss2_reset_parts();
        mode_step_next();
    }
}

void shooter_boss2_step_entrance(void) {
    s32 i;

    shooter_boss2_update_barrage(3);
    shooter_boss2_update_core(6);
    shooter_boss2_update_tail(4);
    shooter_boss2_update_arms();
    for (i = 0; i < 0xC; i++) {
        shooter_boss2_update_part(shooter_boss2_part_objects[i], i, shooter_boss2_part_offsets, shooter_boss2_part_radius, shooter_boss2_part_hit_points);
    }
    if (game_state.mode_step_timer.u % 240 == 0 && rand() % 3 == 0) {
        shooter_play_se(0, 0x500);
    }
    if (game_state.mode_step_timer.u < 0x12C) {
        shooter_move_model(0, 0, -2, -0x28, 0, 0, 0, 0x3C, 0x3C);
    } else {
        mode_step_next();
    }
    game_state.mode_step_timer.u++;
}

void shooter_boss2_step_fight(void) {
    s32 i;

    for (i = 0; i != 0xC; i++) {
        shooter_boss2_update_part(shooter_boss2_part_objects[i], i, shooter_boss2_part_offsets, shooter_boss2_part_radius, shooter_boss2_part_hit_points);
    }
    shooter_boss2_update_damage_lights();
    shooter_boss2_update_ai();
    if (game_state.mode_step_timer.u % 240 == 0) {
        if (rand() % 3 == 0) {
            shooter_play_se(0, 0x500);
        }
    }
    if (shooter_boss_damage_percent(0xC, shooter_boss2_part_hit_points) >= 0x55) {
        /* FAKE: the first byte is tested twice, as in the original asm (T-9330) */
        if (*((u8 *) shooter_actors + 0x905) == 0 && *((u8 *) shooter_actors + 0x98D) == 0 && *((u8 *) shooter_actors + 0x905) == 0) {
            shooter_set_model_clut(0x80196000, 9);
            shooter_stages_cleared += 1;
            mode_step_next();
        }
    }
    game_state.mode_step_timer.u++;
}

void shooter_boss2_step_defeat(void) {
    s32 i;
    u8 shade;

    shooter_move_model(0, 3, 5, -0x14, 0, 0, 0, 0, 0);
    if ((game_state.mode_step_timer.w >= 0xB4) && (game_state.mode_step_timer.w < 0xC7)) {
        shooter_draw_flash_overlay(1, 0xE0, 0xE0, 0xE0);
    } else if (game_state.mode_step_timer.w == 0xC7) {
        shooter_play_se(0, 0x504);
    } else if ((game_state.mode_step_timer.w >= 0xC8) && (game_state.mode_step_timer.w < 0x140)) {
        if ((game_state.mode_step_timer.w % 3) == 0) {
            shooter_draw_flash_overlay(1, 0xF0, 0xF0, 0xF0);
        }
    } else if ((game_state.mode_step_timer.w >= 0x140) && (game_state.mode_step_timer.w < 0x1B8)) {
        shade = (game_state.mode_step_timer.w * 2) - 0x280;
        shooter_draw_flash_overlay(2, shade, shade, shade);
    } else if (game_state.mode_step_timer.w == 0x1B8) {
        i = 1;
        do {
            D_80127080[i++].attribute = 0x80000000;
        } while (i != 36);
        load_csr_tp(0x80162000, 0xC, 1, 0, 0, 0x100, 0x100);
        shooter_load_texture_page(0x80172000, 0xE);
        view_light_0 = 0x14;
        D_80122764 = 0x64;
        D_80122768 = -0x64;
        D_8012276C = 0xB0;
        D_8012276D = 0xB0;
        D_8012276E = 0xB0;
        func_8009AD70(0, &view_light_0);
        view_light_1 = 0x14;
        D_80122774 = -0x64;
        D_80122778 = 0x64;
        D_8012277C = 0x80;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &view_light_1);
        view_light_2 = -0x14;
        D_80122784 = 0x14;
        D_80122788 = -0x64;
        D_8012278C = 0x60;
        D_8012278D = 0x60;
        D_8012278E = 0x60;
        func_8009AD70(2, &view_light_2);
        mode_stage_set(0x1F);
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 33
     * rows off. Keep line with proof=shooter_boss2_step_defeat:33:d7dd7f6c in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-21520 */
    D_800E7384 += 1;
}

/* FAKE: the slot addresses are written as integer sums, not shooter_actors[idx]. uopt merges the two reads of `unk84[1]`
 * (PRE) and copies the first lexical form of `(s32)shooter_actors + idx * size` into the reload at the end of the `if`
 * body; the `unk78` store enters that form base first, the tests and body index first, as in the original. Array
 * and pointer forms reorder the adds or end the merge. Real source unknown. T-22005 */
s32 shooter_boss2_update_part(s32 arg0, s32 arg1, s16 *arg2, u8 *arg3, u8 *arg4) {
    MATRIX mtx;
    ShooterPosition in;
    ShooterPosition out;
    s32 i;
    s32 j;
    s32 idx;
    s32 offX;
    s32 offY;
    ShooterCoordinate *obj;

    idx = arg1 + 0x10;
    offX = arg2[arg1 * 2];
    offY = arg2[arg1 * 2 + 1];
    shooter_actors[arg1 + 16].stats[0] = arg3[arg1];
    mtx = *(MATRIX *)view_base_matrix;
    shooter_rotation_matrix_x((D_80128880[0].x << 0xC) / 360, &mtx);
    shooter_rotation_matrix_z((D_80128880[0].z << 0xC) / 360, &mtx);
    shooter_rotation_matrix_y((D_80128880[0].y << 0xC) / 360, &mtx);
    obj = &D_80127480[arg0];
    in.x = obj->x;
    in.y = obj->y;
    in.z = obj->z;
    ApplyMatrixSV(&mtx, &in, &out);
    shooter_actors[idx].x = D_80127480[0].x + out.x + offX;
    shooter_actors[idx].y = D_80127480[0].y + out.y + offY;
    ((ShooterActor *)((s32)shooter_actors + idx * sizeof(ShooterActor)))->z = D_80127480[0].z + out.z;
    if (((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->stats[1] < 0x15U
        && ((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->auxiliary < ((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->counter) {
        ((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->stats[1] = arg4[arg1 * 2];
        ((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->auxiliary += 1;
        shooter_actors[17].stats[2] = 1;
    }
    if (((ShooterActor *)(idx * sizeof(ShooterActor) + (s32)shooter_actors))->stats[1] == 0) {
        shooter_set_object_clut(arg0, 9);
        if (obj == &D_801274D0) {
            for (i = 4; i != 0x16; i += 3) {
                shooter_set_object_clut(i, 9);
            }
        }
        if (obj == &D_80127520) {
            for (j = 5; j != 0x17; j += 3) {
                shooter_set_object_clut(j, 9);
            }
        }
        if (obj == &D_80127570) {
            for (j = 6; j != 0x18; j += 3) {
                shooter_set_object_clut(j, 9);
            }
        }
    }
}

void shooter_boss2_reset_parts(void) {
    s32 i;

    for (i = 0x10; i < 0x3F; i++) {
        shooter_release_actor_model(i);
    }
    for (i = 0; i < 0xC; i++) {
        shooter_attach_boss_part(shooter_boss2_part_objects[i], i, 0x80196000, shooter_boss2_part_radius, shooter_boss2_part_hit_points, 1);
    }
    *((u8 *) shooter_actors + 0x906) = 0;
    *((u8 *) shooter_actors + 0x883) = 0;
    *((u8 *) shooter_actors + 0x882) = 0;
    *((u8 *) shooter_actors + 0x98E) = 0;
    *((u8 *) shooter_actors + 0x90B) = 0;
    *((u8 *) shooter_actors + 0x90A) = 0;
    *((u8 *) shooter_actors + 0xA16) = 1;
    *((u8 *) shooter_actors + 0x993) = 0;
    *((u8 *) shooter_actors + 0x992) = 0;
    *((u8 *) shooter_actors + 0xA9E) = 1;
    *((u8 *) shooter_actors + 0xA1B) = 0;
    *((u8 *) shooter_actors + 0xA1A) = 0;
    *((u8 *) shooter_actors + 0xB26) = 1;
    *((u8 *) shooter_actors + 0xAA3) = 0;
    *((u8 *) shooter_actors + 0xAA2) = 0;
    *((u8 *) shooter_actors + 0xB2B) = 0;
    *((u8 *) shooter_actors + 0xB2A) = 0;
    *((u8 *) shooter_actors + 0xBB3) = 0;
    *((u8 *) shooter_actors + 0xBB2) = 0;
}

void shooter_boss2_update_damage_lights(void) {
    s32 v;
    u8 b;

    v = shooter_boss_damage_percent(0xC, shooter_boss2_part_hit_points);
    if (*((u8 *) shooter_actors + 0xBB2) < v) {
        view_light_0 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0x80;
        D_8012276E = 0x80;
        func_8009AD70(0, &view_light_0);
        view_light_1 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &view_light_1);
        view_light_2 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0x80;
        D_8012278E = 0x80;
        func_8009AD70(2, &view_light_2);
        b = *((u8 *) shooter_actors + 0xBB3);
        if (b >= 3) {
            *((u8 *) shooter_actors + 0xBB2) = v;
            view_light_0 = 0x14;
            D_80122764 = 0x64;
            D_80122768 = -0x64;
            D_8012276C = 0xB0;
            D_8012276D = 0xB0;
            D_8012276E = 0xB0;
            func_8009AD70(0, &view_light_0);
            view_light_1 = 0x14;
            D_80122774 = -0x64;
            D_80122778 = 0x64;
            D_8012277C = 0x80;
            D_8012277D = 0x80;
            D_8012277E = 0x80;
            func_8009AD70(1, &view_light_1);
            view_light_2 = -0x14;
            D_80122784 = 0x14;
            D_80122788 = -0x64;
            D_8012278C = 0x60;
            D_8012278D = 0x60;
            D_8012278E = 0x60;
            func_8009AD70(2, &view_light_2);
            *((u8 *) shooter_actors + 0xBB3) = 0;
            return;
        }
        *((u8 *) shooter_actors + 0xBB3) = b + 1;
    }
}
