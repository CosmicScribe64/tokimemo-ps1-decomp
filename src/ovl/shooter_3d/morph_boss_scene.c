/* morph_boss_scene.c: src/ovl/TACO/801480B0.c:69: boss scene loads multipart model, runs morphing attack controller, handles defeat and damage-light flashes; body 72: shooter_update_stars(1, 1); 73: for (i = 0; i < 0xB; i++) {. Original object 0x801480B0-0x80148FE0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_boss1(void) {
    if ((u32)game_state.mode_stage_timer < 0x60 && (game_state.frame_count & 0x10)) {
        shooter_draw_text((s32)"1ST BOSS", -0x30, 0, 2);
    }
    switch (game_state.mode_step) {
    case 0:
        shooter_boss1_step_init();
        break;
    case 1:
        shooter_boss1_step_entrance();
        break;
    case 2:
        shooter_boss1_step_fight();
        break;
    case 3:
        shooter_boss1_step_defeat();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_boss1_step_init(void) {
    s32 i;

    if (shooter_load_request == 0) {
        shooter_load_request = SHOOTER_LOAD_STAGE2;
        shooter_load_texture_page(0x80162000, 0xE);
        shooter_load_texture_page(0x80172000, 0xC);
        shooter_boss1_set_lights();
        shooter_init_object_hierarchy(0);
        shooter_link_model_objects(0x8018D000);
        /* pointer argument: as an int constant it is shared with the shooter_load_texture_page argument in a callee-saved register */
        load_csr_tp((void *)0x80162000, 8, 1, 0, 0, 0x100, 0x100);
        shooter_move_model(0, shooter_actors->x - 0xBB8, shooter_actors->y + 0x3E8, -0x109A0, 0, 0xBE, 0, 0, 0);
        for (i = 1; i < 0x1F; i++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss1_rest_pose_pointers[i - 1].translation, shooter_boss1_rest_pose_pointers[i - 1].rotation, shooter_boss1_rest_pose_flags[i - 1].translation_fixed, shooter_boss1_rest_pose_flags[i - 1].rotation_fixed);
            D_80127080[i].attribute = 0;
        }
        D_80128880[0].x = 0;
        D_80128880[0].y = 0xBE;
        D_80128880[0].z = 0;
        shooter_boss1_reset_parts(0);
        mode_step_next();
    }
}

void shooter_boss1_step_entrance(void) {
    s32 i;
    u32 t;

    shooter_update_stars(1, 1);
    t = game_state.mode_step_timer.u;
    if (t < 0x47E) {
        game_state.mode_step_timer.u = t; /* FAKE: the original writes the counter back before the call (store in the jal delay slot), source unknown. T-9370 */
        shooter_boss1_entrance_script(t);
    } else {
        shooter_boss1_reset_parts(1);
        for (i = 1; i < 0x1F; i++) D_80127080[i].attribute = 0;
        mode_step_next();
    }
    game_state.mode_step_timer.u += 1;
}

void shooter_boss1_step_fight(void) {
    s32 i;

    shooter_update_stars(1, 1);
    for (i = 0; i < 0xB; i++) {
        shooter_boss1_update_part(shooter_boss1_part_objects[i], i, shooter_boss1_part_offsets, shooter_boss1_part_radius, shooter_boss1_part_hit_points);
    }
    shooter_boss1_update_damage_lights();
    shooter_boss1_update_ai(game_state.mode_step_timer.w);
    if (shooter_boss_damage_percent(0xB, shooter_boss1_part_hit_points) >= 0x50) {
        D_80127090.attribute = 0x40000000;
        for (i = 3; i < 0x1F; i++) {
            D_80127080[i].attribute = 0x40000000;
            /* FAKE: loop-invariant store; uopt hoists it behind the loop address setup, which places it as the original does. Real source unknown. T-10320 */
            D_801270A0.attribute = 0x40000000;
        }
        shooter_play_se(0, 0x505);
        shooter_stages_cleared += 1;
        mode_step_next();
    }
    game_state.mode_step_timer.w += 1;
}

/* s32 with no return (v0 live at the exit): with void the first spill of the counter is `sw v0` instead of the original's
 * `sw t0`. T-22005 */
s32 shooter_boss1_step_defeat() {
    s32 pad[2]; /* FAKE: reserves the 8 bytes of frame the original has below the spill slot; real local unknown. T-22005 */
    s32 t;
    u8 a;
    u8 b;
    s32 i;

    t = game_state.mode_step_timer.w;
    if (t < 0x78) {
        if (t % 60 == 0) {
            shooter_play_se(0, 0x508);
        }
        shooter_boss_death_explosions(0);
    }
    if (t == 0x64) {
        for (i = 1; i < 0x1F; i++) D_80127080[i].attribute = 0x80000000;
    }
    if (t < 0x78) {
        shooter_draw_flash_overlay(1, 0xE0, 0xE0, 0xE0);
    } else if (t >= 0x78 && t < 0xB0) {
        a = 0x2C0 - t * 4;
        shooter_draw_flash_overlay(1, a, a, a);
    } else if (t >= 0xB0 && t < 0xF0) {
        b = t * 4 - 0x2C0;
        shooter_draw_flash_overlay(2, b, b, b);
    } else if (t == 0xF0) {
        shooter_load_texture_page(0x80162000, 0xE);
        shooter_load_texture_page(0x80172000, 0xC);
        mode_stage_set(0x15);
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment). Keep line with proof=shooter_boss1_step_defeat:32:cda52ed3 in config/migrate_globals.txt,
     * wiki/keep-lines.md. T-22005 */
    D_800E7384 += 1;
}

void shooter_boss1_set_lights(void) {
    view_light_0 = -0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x1E;
    D_8012276C = 0xE0;
    D_8012276D = 0xE0;
    D_8012276E = 0xE0;
    func_8009AD70(0, &view_light_0);
    view_light_1 = 0x28;
    D_80122774 = 0x32;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &view_light_1);
    view_light_2 = 0xA;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x30;
    D_8012278D = 0x30;
    D_8012278E = 0x30;
    func_8009AD70(2, &view_light_2);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

/* arg2: 8-byte position records of four s16; arg3, arg4: byte pairs, all indexed by arg1. The s16 index `arg1 * 4 + k` is a two-step scaling (the original's extra ugen temporary, T-19013); the index-first sums for arg3 and arg4 set the operand order. */
void shooter_boss1_update_part(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4) {
    u8 st;
    MATRIX m;
    SVECTOR vec;
    SVECTOR rot;
    s32 sel;
    s32 px;
    s32 py;

    sel = arg1 + 0x10;
    if (shooter_actors[17].mode == 1) {
        px = ((s16 *)arg2)[arg1 * 4];
        py = ((s16 *)arg2)[arg1 * 4 + 1];
        shooter_actors[sel].stats[0] = *(arg1 * 2 + (u8 *)arg3);
        if (arg0 == 0x1C && shooter_actors[16].stats[2] == 1) {
            shooter_actors[sel].stats[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        }
        if ((arg0 == 0x1C || arg0 == 0x1B || arg0 == 0x15) && shooter_actors[21].stats[2] == 1) {
            shooter_actors[sel].stats[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        }
    } else if (shooter_actors[17].mode == 0x1D) {
        px = ((s16 *)arg2)[arg1 * 4 + 2];
        py = ((s16 *)arg2)[arg1 * 4 + 3];
        shooter_actors[sel].stats[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        if ((arg0 == 0x1B || arg0 == 0x1C) && shooter_actors[22].stats[2] != 0 && shooter_actors[23].stats[2] == 0) {
            shooter_actors[sel].stats[0] = 0;
        }
    }
    m = *(MATRIX *)view_base_matrix;
    shooter_rotation_matrix_x((D_80128880[0].x << 12) / 360, &m);
    shooter_rotation_matrix_z((D_80128880[0].z << 12) / 360, &m);
    shooter_rotation_matrix_y((D_80128880[0].y << 12) / 360, &m);
    vec.vx = D_80127480[arg0].x;
    vec.vy = D_80127480[arg0].y;
    vec.vz = D_80127480[arg0].z;
    ApplyMatrixSV(&m, &vec, &rot);
    shooter_actors[sel].x = D_80127480[0].x + rot.vx + px;
    shooter_actors[sel].y = D_80127480[0].y + rot.vy + py;
    shooter_actors[sel].z = D_80127480[0].z + rot.vz;
    st = (sel + shooter_actors)->stats[1];
    if (st < 0x15U && (sel + shooter_actors)->auxiliary < (sel + shooter_actors)->counter) {
        (sel + shooter_actors)->stats[1] = *(arg1 * 2 + (u8 *)arg4);
        (sel + shooter_actors)->auxiliary += 1;
        st = shooter_actors[sel].stats[1];
    }
    if (st == 0) {
        if ((game_state.mode_step_timer.w & 1) == 0) {
            D_80127080[arg0].attribute = 0x40000000;
        } else {
            D_80127080[arg0].attribute = 0x80000000;
        }
    }
}

void shooter_boss1_reset_parts(s32 attach_parts) {
    s32 index;

    index = 0x10;
    do {
        shooter_release_actor_model(index);
        index += 1;
    } while (index < 0x3F);
    index = 0;
    if (attach_parts > 0) {
        do {
            shooter_attach_boss_part(shooter_boss1_part_objects[index], index, 0x8018D000, shooter_boss1_part_radius, shooter_boss1_part_hit_points, 2);
            index += 1;
        } while (index != 0xB);
    }
    shooter_actors[16].stats[2] = 0;
    shooter_actors[16].phase = 0;
    shooter_actors[16].mode = 0;
    shooter_actors[17].stats[2] = 0;
    shooter_actors[17].phase = 0;
    shooter_actors[17].mode = 1;
    shooter_actors[18].stats[2] = 0;
    shooter_actors[18].phase = 0;
    shooter_actors[18].mode = 0;
    shooter_actors[19].stats[2] = 0;
    shooter_actors[19].phase = 0;
    shooter_actors[19].mode = 0;
    shooter_actors[20].stats[2] = 0;
    shooter_actors[20].phase = 0;
    shooter_actors[20].mode = 0;
    shooter_actors[21].stats[2] = 0;
    shooter_actors[21].phase = 0;
    shooter_actors[21].mode = 0;
    shooter_actors[22].stats[2] = 0;
    shooter_actors[22].phase = 0;
    shooter_actors[22].mode = 0;
    shooter_actors[23].stats[2] = 0;
    shooter_actors[24].stats[2] = 0;
    shooter_actors[24].phase = 0;
    shooter_actors[24].mode = 0;
    shooter_actors[25].stats[2] = 0;
    shooter_actors[25].phase = 0;
    shooter_actors[25].mode = 0;
}

void shooter_boss1_update_damage_lights(void) {
    u32 v;
    u8 t;

    v = shooter_boss_damage_percent(0xB, shooter_boss1_part_hit_points);
    if (shooter_actors[20].mode < v) {
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
        t = shooter_actors[20].phase;
        if (t >= 3U) {
            shooter_actors[20].mode = v;
            view_light_0 = -0x64;
            D_80122764 = 0x1E;
            D_80122768 = -0x1E;
            D_8012276C = 0xE0;
            D_8012276D = 0xE0;
            D_8012276E = 0xE0;
            func_8009AD70(0, &view_light_0);
            view_light_1 = 0x28;
            D_80122774 = 0x32;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &view_light_1);
            view_light_2 = 0xA;
            D_80122784 = -0x14;
            D_80122788 = -0x64;
            D_8012278C = 0x30;
            D_8012278D = 0x30;
            D_8012278E = 0x30;
            func_8009AD70(2, &view_light_2);
            shooter_actors[20].phase = 0;
        } else {
            shooter_actors[20].phase = t + 1;
        }
    }
}
