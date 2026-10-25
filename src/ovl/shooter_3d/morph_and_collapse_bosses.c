/* morph_and_collapse_bosses.c: src/ovl/TACO/80149F90.c:323: combined object: morphing boss attack groups and descending final boss collapse/part attacks; body 326: shooter_boss1_run_attack_mode(); 327: t = shooter_actors[16].unk84[2];. Original object 0x80149F90-0x80151C30 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_boss3(void) {
    if (((u32) game_state.mode_stage_timer < 0x60U) && (game_state.frame_count & 0x10)) {
        shooter_draw_text("3ST BOSS", -0x30, 0, 2);
    }
    switch (game_state.mode_step) {
    case 0:
        shooter_boss3_step_init();
        break;
    case 1:
        shooter_boss3_step_descend();
        break;
    case 2:
        shooter_boss3_step_fight();
        break;
    case 3:
        shooter_boss3_step_defeat();
        break;
    case 4:
        shooter_boss3_step_collapse();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_boss3_step_init(void) {
    s32 i;
    s32 j;

    if (shooter_load_request == 0) {
        shooter_boss3_set_lights();
        shooter_init_object_hierarchy(0);
        shooter_link_model_objects(0x801A7000);
        shooter_read_model_polygon_ranges(0x801A7000);
        shooter_load_texture_page(0x80162000, 8);
        shooter_load_texture_page(0x80172000, 0xA);
        shooter_move_model(0, shooter_actors->x, shooter_actors->y - 0xBB8, -0x3A98, 0x1E, 0xB4, 0, 0, 0);
        for (i = 1, j = 0; i < 0x22; i++, j++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss3_rest_pose_pointers[j * 2 - 2], shooter_boss3_rest_pose_pointers[j * 2 - 1], shooter_boss3_rest_pose_flags[j * 2 - 2], shooter_boss3_rest_pose_flags[j * 2 - 1]);
            (&D_80127090)[j].attribute = 0; /* FAKE: strides from D_80127090, not D_80127080[]; the aggregate is unknown. T-9320 */
        }
        D_80128880[0].x = 0x1E;
        D_80128880[0].y = 0xB4;
        D_80128880[0].z = 0;
        for (i = 0x10; i != 0x3F; i++) {
            shooter_release_actor_model(i);
        }
        mode_step_next();
    }
}

void shooter_boss3_step_descend(void) {
    shooter_boss3_descend_motion();
    shooter_boss3_fire_pattern();
    if (((u32)game_state.mode_step_timer.w % 180U) == 0) {
        shooter_play_se(0, 0x500);
    }
    game_state.mode_step_timer.w += 1;
}

void shooter_boss3_step_fight(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        shooter_boss3_update_part(shooter_boss3_part_objects[i], i, shooter_boss3_part_offsets, shooter_boss3_part_radius, shooter_boss3_part_hit_points, game_state.mode_step_timer.w);
    }
    shooter_boss3_update_damage_lights();
    shooter_boss3_update_ai();
    if (game_state.mode_step_timer.w % 180U == 0) {
        shooter_play_se(0, 0x500);
    }
    shooter_boss3_fire_pattern();
    if (shooter_boss_damage_percent(0xB, shooter_boss3_part_hit_points) >= 0x55 && shooter_actors[16].stats[1] == 0 && shooter_actors[19].stats[1] == 0 && shooter_actors[20].stats[1] == 0) {
        shooter_stages_cleared += 1;
        game_state.mode_step_timer.w = 0;
        mode_step_next();
    }
    game_state.mode_step_timer.w += 1;
}

void shooter_boss3_step_defeat(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        shooter_boss3_update_part(shooter_boss3_part_objects[i], i, shooter_boss3_part_offsets, shooter_boss3_part_radius, shooter_boss3_part_hit_points, 0);
    }
    shooter_boss3_defeat_motion();
}

void shooter_boss3_step_collapse(void) {
    shooter_boss3_update_part(shooter_boss3_part_objects_1, 1, shooter_boss3_part_offsets, shooter_boss3_part_radius, shooter_boss3_part_hit_points, 1);
    shooter_boss3_collapse_motion();
}

/* s32 with no return fixes the lui order of the second 0x40000000 branch (T-19003, T-22025). */
s32 shooter_boss3_update_part(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5) {
    MATRIX m;
    SVECTOR vec;
    SVECTOR rot;
    s32 sel;
    s32 px;
    s32 py;
    ShooterCoordinate *o;

    sel = arg1 + 0x10;
    px = ((s16 *)arg2)[arg1 * 2];
    py = ((s16 *)arg2)[arg1 * 2 + 1];
    shooter_actors[arg1 + 16].stats[0] = ((u8 *)arg3)[arg1];
    m = *(MATRIX *)view_base_matrix;
    shooter_rotation_matrix_x((D_80128880[0].x << 12) / 360, &m);
    shooter_rotation_matrix_z((D_80128880[0].z << 12) / 360, &m);
    shooter_rotation_matrix_y((D_80128880[0].y << 12) / 360, &m);
    o = &D_80127480[arg0];
    vec.vx = o->x;
    vec.vy = o->y;
    vec.vz = o->z;
    ApplyMatrixSV(&m, &vec, &rot);
    /* FAKE: byte-pointer addition seeds uopt's Uadd ichain with shooter_actors first for the partial-redundancy reload at the end of the if body (T-24120). */
    ((ShooterActor *)((u8 *)shooter_actors + sel * 0x88))->x = D_80127480[0].x + rot.vx + px;
    shooter_actors[sel].y = D_80127480[0].y + rot.vy + py;
    shooter_actors[sel].z = D_80127480[0].z + rot.vz;
    if ((sel + shooter_actors)->stats[1] < 0x15U && (sel + shooter_actors)->auxiliary < (sel + shooter_actors)->counter) {
        (sel + shooter_actors)->stats[1] = *(arg1 * 2 + (u8 *)arg4);
        (sel + shooter_actors)->auxiliary += 1;
    }
    if ((sel + shooter_actors)->stats[1] == 0) {
        if (arg0 != 0x19) {
            shooter_boss3_set_part_visibility(arg0, arg5);
        }
        if (o == (ShooterCoordinate *)D_80127610) {
            D_80127080[1].attribute = 0x40000000;
            D_80127080[2].attribute = 0x40000000;
            D_80127080[3].attribute = 0x40000000;
            D_80127080[4].attribute = 0x40000000;
        } else if (o == (ShooterCoordinate *)D_801277F0) {
            D_80127080[7].attribute = 0x40000000;
            D_80127080[8].attribute = 0x40000000;
            D_80127080[9].attribute = 0x40000000;
            D_80127080[10].attribute = 0x40000000;
        }
    }
}

void shooter_boss3_reset_parts(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        shooter_attach_boss_part(shooter_boss3_part_objects[i], i, 0x801A7000, shooter_boss3_part_radius, shooter_boss3_part_hit_points, 1);
    }
    shooter_actors[16].phase = 0;
    shooter_actors[16].mode = 0;
    shooter_actors[17].phase = 0;
    shooter_actors[17].mode = 0;
    shooter_actors[18].stats[2] = 0;
    shooter_actors[18].phase = 0;
    shooter_actors[18].mode = 0;
    shooter_actors[19].stats[2] = 0;
    shooter_actors[19].phase = 0;
    shooter_actors[19].mode = 0;
    shooter_actors[20].stats[2] = 0;
    shooter_actors[20].mode = 0;
    shooter_actors[21].phase = 0;
    shooter_actors[21].mode = 0;
}

void shooter_boss3_update_damage_lights(void) {
    u32 v;
    u8 t;

    v = shooter_boss_damage_percent(0xB, shooter_boss3_part_hit_points);
    if (shooter_actors[21].mode < v) {
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
        t = shooter_actors[21].phase;
        if (t >= 3U) {
            shooter_actors[21].mode = v;
            view_light_0 = 0x64;
            D_80122764 = 0x32;
            D_80122768 = -0x64;
            D_8012276C = 0x70;
            D_8012276D = 0x70;
            D_8012276E = 0x80;
            func_8009AD70(0, &view_light_0);
            view_light_1 = 0;
            D_80122774 = -0x64;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &view_light_1);
            view_light_2 = -0x64;
            D_80122784 = 0x32;
            D_80122788 = -0x64;
            D_8012278C = 0x80;
            D_8012278D = 0x80;
            D_8012278E = 0x90;
            func_8009AD70(2, &view_light_2);
            shooter_actors[21].phase = 0;
        } else {
            shooter_actors[21].phase = t + 1;
        }
    }
}

/* T-24705: without the empty test the store moves to the right place (after sw ra) but the forwarded `lw` of the home spends one ugen temporary, so every temporary is one high (6 lines); `if (p){}`, `if (n){}`, `if (i){}`, `if (p == 0){}`, `arg1 |= 0;`, `arg1 = arg1 ^ 0;`, `arg1 = arg1 * 1;`, `arg1 & -1`, a `k = arg1` local, an absolute-address view and the for/while forms all stay at 5 or 6. */
void shooter_boss1_entrance_script(s32 entrance_elapsed_frames) {
    s32 i;
    s32 j;
    s32 random;

    if (entrance_elapsed_frames == 0xA) {
        shooter_play_se(0, 0x500);
    }
    if (entrance_elapsed_frames < 0x64) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0, 0x32, 0x64);
    } else if (entrance_elapsed_frames == 0xC7) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 1, 0x28, 0x28);
        /* FAKE: this store chain consumes the original ugen slot; plain stores shift later temporaries. T-24502 */
        D_80128880[0].x = D_80128880[0].z = D_80128880[0].y = 0;
        shooter_play_se(0, 0x500);
    } else if ((entrance_elapsed_frames >= 0xC8) && (entrance_elapsed_frames < 0xF0)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 2, 0x32, 0x64);
    } else if ((entrance_elapsed_frames >= 0xF0) && (entrance_elapsed_frames < 0x118)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 3, 0x32, 0x64);
    } else if ((entrance_elapsed_frames >= 0x118) && (entrance_elapsed_frames < 0x140)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 4, 0x32, 0x64);
    } else if ((entrance_elapsed_frames >= 0x140) && (entrance_elapsed_frames < 0x154)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 5, 0x32, 0x5A);
    } else if ((entrance_elapsed_frames >= 0x154) && (entrance_elapsed_frames < 0x172)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 6, 0x32, 0x50);
    } else if ((entrance_elapsed_frames >= 0x172) && (entrance_elapsed_frames < 0x190)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 7, 0x32, 0x46);
    } else if ((entrance_elapsed_frames >= 0x190) && (entrance_elapsed_frames < 0x1A4)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 8, 0x32, 0x3C);
    } else if ((entrance_elapsed_frames >= 0x1A4) && (entrance_elapsed_frames < 0x1B8)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 9, 0x32, 0x32);
    } else if ((entrance_elapsed_frames >= 0x1B8) && (entrance_elapsed_frames < 0x1C2)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xA, 0x1E, 0x1E);
    } else if ((entrance_elapsed_frames >= 0x1C2) && (entrance_elapsed_frames < 0x1D6)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xB, 0x14, 0x14);
    } else if ((entrance_elapsed_frames >= 0x1D6) && (entrance_elapsed_frames < 0x1F4)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xC, 0xA, 0xA);
    } else if (entrance_elapsed_frames == 0x1F4) {
        for (i = 1, j = 0; i != 0x1F; i++, j++) {
            shooter_set_pose_from_tables(i, 1, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
        }
        shooter_actors[20].stats[2] = 1U;
        shooter_boss1_rest_group_pair(0xD, 0);
        shooter_boss1_rest_group_pair(9, 3);
    } else if ((entrance_elapsed_frames >= 0x1F4) && (entrance_elapsed_frames < 0x203)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xD, 5, 5);
    } else if ((entrance_elapsed_frames >= 0x203) && (entrance_elapsed_frames < 0x22B)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xE, 0xA, 0xA);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_boss1_update_group_pair_short(&shooter_actors[19].phase, &shooter_actors[19].mode, 0xD, 0, 3U);
            shooter_boss1_update_group_pair_short(&shooter_actors[18].phase, &shooter_actors[18].mode, 9, 3, 3U);
        }
    } else if ((entrance_elapsed_frames >= 0x22B) && (entrance_elapsed_frames < 0x23A)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xF, 5, 0x64);
    } else if ((entrance_elapsed_frames >= 0x23A) && (entrance_elapsed_frames < 0x267)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0xD, 5, 0x64);
    } else if ((entrance_elapsed_frames >= 0x267) && (entrance_elapsed_frames < 0x289)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0xC8);
    } else if (entrance_elapsed_frames == 0x289) {
        /* FAKE: T-25043, keep the helper result live in uopt through the flag
         * stores so the shared constant takes v1. Ugen removes the unsigned
         * always-false test and its store. */
        for (i = 1, j = 0; i != 0x1F; i++, j++) {
            random = shooter_set_pose_from_tables(i, 1, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
        }
        shooter_actors[17].mode = 1;
        shooter_actors[17].stats[2] = 1U;
        if ((u32)random < 0) {
            shooter_actors[17].stats[2] = 1U;
        }
    } else if ((entrance_elapsed_frames >= 0x28A) && (entrance_elapsed_frames < 0x32A)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0xC8);
        if (shooter_actors[17].stats[2] == 1) {
            shooter_boss1_morph_to_form2(2);
        }
    } else if ((entrance_elapsed_frames >= 0x32A) && (entrance_elapsed_frames < 0x3B6)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        random = rand();
        shooter_fire_from_joint(0, 0x1B, (s16) ((random & 0x3FF) - 0x200), (s16) ((rand() & 0x3FF) - 0x200), 0x7D0, 0x190, 0x81);
    } else if (entrance_elapsed_frames == 0x3B6) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        for (i = 1, j = 0; i != 0x1F; i++, j++) {
            shooter_set_pose_from_tables(i, 0x1D, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
        }
        shooter_actors[17].stats[2] = 2U;
    } else if ((entrance_elapsed_frames >= 0x3B7) && (entrance_elapsed_frames < 0x47E)) {
        shooter_apply_keyframe_tween(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        if (shooter_actors[17].stats[2] == 2) {
            shooter_boss1_morph_to_form1(2);
        }
    }
}


void shooter_boss1_update_ai(s32 fight_elapsed_frames) {
    u8 t;

    shooter_boss1_run_attack_mode();
    t = shooter_actors[16].stats[2];
    if (t == 0 || t == 2 || t == 3 || t == 4) {
        shooter_boss_damage_percent(0xB, shooter_boss1_part_hit_points);
        if (shooter_actors[17].mode == 1 && fight_elapsed_frames >= 0x259) {
            shooter_actors[17].stats[2] = 1;
        }
    }
    if (shooter_actors[17].stats[2] == 1) {
        shooter_boss1_morph_to_form2(2);
    }
    if (shooter_actors[17].stats[2] == 2) {
        shooter_boss1_morph_to_form1(2);
    }
}

void shooter_boss1_morph_to_form2(s32 phase_threshold) {
    s32 i;

    if ((u32)phase_threshold < shooter_actors[17].phase) {
        if (shooter_actors[17].mode == 1) {
            shooter_play_se(0, 0x501);
        }
        for (i = 1; i < 0x1F; i++) {
            shooter_set_pose_from_tables(i, shooter_actors[17].mode, shooter_boss1_morph_pose_pointers[i * 2 - 2], shooter_boss1_morph_pose_pointers[i * 2 - 1], shooter_boss1_morph_pose_flags[i * 2 - 2], shooter_boss1_morph_pose_flags[i * 2 - 1]);
        }
        shooter_actors[17].mode += 1;
        shooter_actors[17].phase = 0;
    }
    if (shooter_actors[17].mode >= 0x1EU) {
        shooter_actors[17].stats[2] = 0;
        shooter_actors[17].phase = shooter_actors[17].stats[2];
        shooter_actors[17].mode = 0x1D;
        return;
    }
    shooter_actors[17].phase += 1;
}

void shooter_boss1_morph_to_form1(s32 phase_threshold) {
    s32 i;

    if ((u32)phase_threshold < shooter_actors[17].phase) {
        if (shooter_actors[17].mode == 0x1D) {
            shooter_play_se(0, 0x501);
        }
        for (i = 1; i < 0x1F; i++) {
            shooter_set_pose_from_tables(i, shooter_actors[17].mode, shooter_boss1_morph_pose_pointers[i * 2 - 2], shooter_boss1_morph_pose_pointers[i * 2 - 1], shooter_boss1_morph_pose_flags[i * 2 - 2], shooter_boss1_morph_pose_flags[i * 2 - 1]);
        }
        shooter_actors[17].mode -= 1;
        shooter_actors[17].phase = 0;
    }
    if (shooter_actors[17].mode == 0) {
        shooter_actors[17].stats[2] = 0;
        shooter_actors[17].phase = shooter_actors[17].stats[2];
        shooter_actors[17].mode = 1;
        return;
    }
    shooter_actors[17].phase += 1;
}

void shooter_boss1_update_group_pair(u8 *phase_counter, u8 *pose_mode, s32 object_index_base, s32 pose_table_index, s32 frame_threshold) {
    s32 i;
    u8 t;

    t = *pose_mode;
    if (t < 0xC || t >= 0x16) {
        if ((u32)frame_threshold < *phase_counter) {
            shooter_boss1_pose_group_pair(object_index_base, pose_table_index, t);
            *pose_mode += 1;
            *phase_counter = 0;
            t = *pose_mode;
        }
    } else if (t >= 0xC && t < 0x16) {
        if (*phase_counter != 0) {
            shooter_boss1_pose_group_pair(object_index_base, pose_table_index, t);
            *pose_mode += 1;
            *phase_counter = 0;
            t = *pose_mode ^ 0; /* FAKE: the ^ 0 burns the $t3 temporary the original skips (permuter); real source unknown. T-9320 */
        }
    }
    if (t >= 0x32) {
        *pose_mode = 0;
        *phase_counter = 0;
        if (object_index_base == 9) {
            shooter_actors[18].stats[2] = 0;
        } else {
            shooter_actors[19].stats[2] = 0;
        }
        for (i = object_index_base; i < object_index_base + 3; i++) {
            shooter_set_pose_from_tables(i, 1, shooter_boss1_morph_pose_pointers[i * 2 - 2], shooter_boss1_morph_pose_pointers[i * 2 - 1], shooter_boss1_morph_pose_flags[i * 2 - 2], shooter_boss1_morph_pose_flags[i * 2 - 1]);
        }
    } else {
        *phase_counter += 1;
    }
}

void shooter_boss1_update_group_pair_short(u8 *phase_counter, u8 *pose_mode, s32 object_index_base, s32 pose_table_index, u32 frame_threshold) {
    s32 i;
    u8 t;

    if (frame_threshold < *phase_counter) {
        shooter_boss1_pose_group_pair(object_index_base, pose_table_index, *pose_mode);
        *pose_mode += 1;
        *phase_counter = 0;
    }
    if (*pose_mode >= 8U) {
        shooter_actors[20].stats[2] = 0;
        t = shooter_actors[20].stats[2];
        *pose_mode = t;
        *phase_counter = t;
        for (i = object_index_base; i < object_index_base + 3; i++) {
            shooter_set_pose_from_tables(i, 1, shooter_boss1_morph_pose_pointers[i * 2 - 2], shooter_boss1_morph_pose_pointers[i * 2 - 1], shooter_boss1_morph_pose_flags[i * 2 - 2], shooter_boss1_morph_pose_flags[i * 2 - 1]);
        }
    } else {
        *phase_counter += 1;
    }
}

/* s32 with no return (implicit int in the original), as shooter_boss1_form2_group_d_motion: v0 is live at the exit, so as1 keeps
 * li v0,0x40 out of the beqz delay slot and takes li v1,0x800 there, as in the original. T-20620 */
s32 shooter_boss1_pose_group_pair(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 n;

    if (arg2 < 4) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 1, 0, 0);
        }
    } else if (arg2 >= 4 && arg2 < 8) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 2, 0, 0);
        }
    } else if (arg2 >= 0xC && arg2 < 0x16) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 3, 0, 0);
        }
    } else if (arg2 >= 0x18 && arg2 < 0x1C) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 1, 0, 0);
        }
    } else if (arg2 >= 0x20 && arg2 < 0x2A) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 4, 0, 0);
        }
    } else if (arg2 >= 0x2E && arg2 < 0x32) {
        for (i = 0, n = arg0; i != 3; i++, n++, arg1++) {
            shooter_apply_keyframe_tween(n, (s16 *)shooter_boss1_group_pair_poses[arg1], 2, 0, 0);
        }
    }
    if (arg2 >= 0x14 && arg2 < 0x1E) {
        shooter_actors[18].flag_bytes[1] = 0x40;
        shooter_actors[18].z = 0x800;
        shooter_actors[19].flag_bytes[1] = 0x40;
        shooter_actors[19].z = 0x800;
        shooter_actors[19].stats[0] = 0x14;
        shooter_actors[20].flag_bytes[1] = 0x40;
        shooter_actors[20].z = 0x800;
        shooter_actors[20].stats[0] = 0x14;
        shooter_actors[21].flag_bytes[1] = 0x40;
        shooter_actors[21].z = 0x800;
        shooter_actors[21].stats[0] = 0x14;
        shooter_actors[23].flag_bytes[1] = 0x40;
        shooter_actors[23].z = 0x800;
        shooter_actors[23].stats[0] = 0x14;
        shooter_actors[24].flag_bytes[1] = 0x40;
        shooter_actors[24].z = 0x800;
        shooter_actors[24].stats[0] = 0x14;
        shooter_actors[25].flag_bytes[1] = 0x40;
        shooter_actors[25].z = 0x800;
        shooter_actors[25].stats[0] = 0x14;
        return;
    }
    if (arg2 == 0x1E) {
        shooter_actors[18].flag_bytes[1] = 0x80;
        shooter_actors[19].flag_bytes[1] = 0x80;
        shooter_actors[20].flag_bytes[1] = 0x80;
        shooter_actors[21].flag_bytes[1] = 0x80;
        shooter_actors[23].flag_bytes[1] = 0x80;
        shooter_actors[24].flag_bytes[1] = 0x80;
        shooter_actors[25].flag_bytes[1] = 0x80;
    }
}

void shooter_boss1_rest_group_pair(s32 first_model_slot, s32 pose_group_index) {
    s32 i;
    s32 n;
    s32 *p;

    /* FAKE: init statements on one source line; as1 schedules by source line and only this layout orders the prologue like the original. T-8030 */
    i = 0; n = first_model_slot; p = &shooter_boss1_group_pair_poses[pose_group_index];
    for (; i < 3; ) {
        shooter_set_rotation_from_pose(n, *p);
        i++;
        n++;
        p++;
    }
}

void shooter_boss1_update_group_c(s32 phase_threshold) {
    s32 i;
    u8 t;

    if ((u32)phase_threshold < shooter_actors[21].phase) {
        shooter_boss1_pose_group_c(shooter_actors[21].mode);
        shooter_actors[21].mode += 1;
        shooter_actors[21].phase = 0;
    }
    if (shooter_actors[21].mode >= 0x24U) {
        shooter_actors[21].stats[2] = 0;
        t = shooter_actors[21].stats[2];
        shooter_actors[21].mode = t;
        shooter_actors[21].phase = t;
        for (i = 1; i < 4; i++) {
            shooter_set_pose_from_tables(i, 1, shooter_boss1_morph_pose_pointers[i * 2 - 2], shooter_boss1_morph_pose_pointers[i * 2 - 1], shooter_boss1_morph_pose_flags[i * 2 - 2], shooter_boss1_morph_pose_flags[i * 2 - 1]);
        }
        return;
    }
    shooter_actors[21].phase += 1;
}

void shooter_boss1_pose_group_c(s32 arg0) {
    s32 i;

    if (arg0 < 8) {
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_c_poses[i], 1, 0, 0);
        }
    } else if (arg0 >= 0x1C && arg0 < 0x24) {
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_c_poses[i], 2, 0, 0);
        }
    }
}

void shooter_boss1_rest_group_c(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        shooter_set_rotation_from_pose(i + 1, shooter_boss1_group_c_poses[i]);
    }
}

void shooter_boss1_update_group_d(u32 phase_threshold) {
    s32 i;
    s32 j;
    u8 t;

    t = shooter_actors[22].mode;
    if (t < 0xA) {
        if (phase_threshold < shooter_actors[22].phase) {
            if (shooter_actors[23].stats[2] == 0) {
                shooter_boss1_pose_group_d_form1(t);
                t = shooter_actors[22].mode;
            } else if (shooter_actors[23].stats[2] != 0) {
                shooter_boss1_pose_group_d_form2(t);
                t = shooter_actors[22].mode;
            }
            shooter_actors[22].mode = t + 1;
            shooter_actors[22].phase = 0;
            t = shooter_actors[22].mode;
        }
    }
    if (t >= 0xA) {
        if (shooter_actors[23].stats[2] != 0) {
            shooter_actors[23].stats[2] = 0;
            shooter_actors[22].phase = shooter_actors[22].mode = shooter_actors[22].stats[2] = shooter_actors[23].stats[2];
            for (i = 1, j = 0; i != 0x1F; i++, j++) {
                shooter_set_pose_from_tables(i, 0x1D, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
            }
        }
    } else {
        shooter_actors[22].phase += 1;
    }
}

void shooter_boss1_pose_group_d_form1(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_d_poses_a[i], 1, 0, 0);
        }
        for (i = 0; i < 9; i++) {
            shooter_apply_keyframe_tween(i + 0x13, shooter_boss1_group_d_poses_b[i], 1, 0, 0);
        }
        for (i = 0; i < 10; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss1_group_d_poses_c[i], 1, 0, 0);
        }
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_d_poses_d[i], 1, 0, 0);
        }
    }
}

void shooter_boss1_pose_group_d_form2(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_d_poses_a[i], 2, 0, 0);
        }
        for (i = 0; i < 9; i++) {
            shooter_apply_keyframe_tween(i + 0x13, shooter_boss1_group_d_poses_b[i], 2, 0, 0);
        }
        for (i = 0; i < 10; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss1_group_d_poses_c[i], 2, 0, 0);
        }
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_d_poses_d[i], 2, 0, 0);
        }
    }
}

void shooter_boss1_rest_group_d(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        shooter_set_rotation_from_pose(i + 1, shooter_boss1_group_d_poses_a[i]);
    }
    for (i = 0; i < 9; i++) {
        shooter_set_rotation_from_pose(i + 0x13, shooter_boss1_group_d_poses_b[i]);
    }
    for (i = 0; i < 10; i++) {
        shooter_set_rotation_from_pose(i + 7, shooter_boss1_group_d_poses_c[i]);
    }
    for (i = 0; i < 2; i++) {
        shooter_set_rotation_from_pose(i + 0x1D, shooter_boss1_group_d_poses_d[i]);
    }
}

void shooter_boss1_update_group_e(s32 phase_threshold) {
    s32 i;
    s32 j;
    u8 t;

    t = shooter_actors[24].mode;
    if (t < 0x10 || t >= 0x12) {
        if ((u32)phase_threshold < shooter_actors[24].phase) {
            shooter_boss1_pose_group_e(t);
            shooter_actors[24].mode += 1;
            shooter_actors[24].phase = 0;
            t = shooter_actors[24].mode;
        }
    } else if (t >= 0x10 && t < 0x12) {
        if (shooter_actors[24].phase != 0) {
            shooter_boss1_pose_group_e(t);
            shooter_actors[24].mode += 1;
            shooter_actors[24].phase = 0;
            t = shooter_actors[24].mode;
        }
    }
    if (t >= 0x2A) {
        shooter_actors[24].phase = shooter_actors[24].mode = shooter_actors[24].stats[2] = 0;
        for (i = 1, j = 0; i != 0x1F; i++, j++) {
            shooter_set_pose_from_tables(i, 0x1D, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
        }
    } else {
        shooter_actors[24].phase += 1;
    }
}

void shooter_boss1_pose_group_e(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        shooter_apply_keyframe_tween(0x12, shooter_boss1_group_e_pose_a, 1, 0, 0);
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_e_poses_b[i], 1, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_e_pose_c, 1, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 4, shooter_boss1_group_e_poses_d[i], 1, 0, 0);
        }
    } else if (arg0 >= 0x10 && arg0 < 0x12) {
        shooter_apply_keyframe_tween(0x12, shooter_boss1_group_e_pose_a, 2, 0, 0);
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_e_poses_b[i], 2, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_e_pose_c, 2, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 4, shooter_boss1_group_e_poses_d[i], 2, 0, 0);
        }
    } else if (arg0 >= 0x12 && arg0 < 0x1C) {
        shooter_apply_keyframe_tween(0x12, shooter_boss1_group_e_pose_a, 3, 0, 0);
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_e_poses_b[i], 3, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_e_pose_c, 3, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 4, shooter_boss1_group_e_poses_d[i], 3, 0, 0);
        }
    } else if (arg0 >= 0x20 && arg0 < 0x2A) {
        shooter_apply_keyframe_tween(0x12, shooter_boss1_group_e_pose_a, 4, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_e_poses_b[i], 4, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_e_pose_c, 4, 0, 0);
    }
    if (arg0 >= 0x10 && arg0 < 0x1C) {
        shooter_fire_from_joint(0, 0x12, 0, 0, 0x1388, 0x320, 0x84);
    }
    if (arg0 == 1) {
        shooter_play_se(0, 0x502);
        return;
    }
    if (arg0 == 0x10) {
        shooter_play_se(0, 0x503);
    }
}

void shooter_boss1_rest_group_e(void) {
    s32 i;

    shooter_set_rotation_from_pose(0x12, shooter_boss1_group_e_pose_a);
    for (i = 0; i < 2; i++) {
        shooter_set_rotation_from_pose(i + 0x19, shooter_boss1_group_e_poses_b[i]);
    }
    shooter_set_rotation_from_pose(8, shooter_boss1_group_e_pose_c);
    for (i = 0; i < 2; i++) {
        shooter_set_rotation_from_pose(i + 4, shooter_boss1_group_e_poses_d[i]);
    }
}

void shooter_boss1_update_group_f(s32 phase_threshold) {
    s32 i;
    s32 j;
    u8 t;

    t = shooter_actors[25].mode;
    if (t < 0x1E || t >= 0x28) {
        if ((u32)phase_threshold < shooter_actors[25].phase) {
            shooter_boss1_pose_group_f(t);
            shooter_actors[25].mode += 1;
            shooter_actors[25].phase = 0;
            t = shooter_actors[25].mode;
        }
    } else if ((t >= 0x1E) & (t < 0x28)) {
        shooter_boss1_pose_group_f(t);
        shooter_actors[25].mode += 1;
        shooter_actors[25].phase = 0;
        t = shooter_actors[25].mode;
    }
    if (t >= 0x4A) {
        shooter_actors[25].stats[2] = 0;
        shooter_actors[25].phase = shooter_actors[25].mode = shooter_actors[25].stats[2];
        for (i = 1, j = 0; i != 0x1F; i++, j++) {
            shooter_set_pose_from_tables(i, 0x1D, D_8015FEF8[j * 2 - 2], D_8015FEF8[j * 2 - 1], D_8015FFE2[j * 2 - 2], D_8015FFE2[j * 2 - 1]);
        }
        return;
    }
    shooter_actors[25].phase += 1;
}

s32 shooter_boss1_pose_group_f(s32 arg0) {
    MATRIX matrix;
    SVECTOR input;
    SVECTOR output;
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_f_poses_a[i], 1, 0, 0);
        }
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_f_poses_b[i], 1, 0, 0);
        }
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_f_poses_c[i], 1, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 1, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 0x15, shooter_boss1_group_f_poses_e[i], 1, 0, 0);
        }
    } else if (arg0 >= 0xE && arg0 < 0x18) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_f_poses_a[i], 2, 0, 0);
        }
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(i + 0x19, shooter_boss1_group_f_poses_b[i], 2, 0, 0);
        }
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_f_poses_c[i], 2, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 2, 0, 0);
    } else if (arg0 >= 0x1E && arg0 < 0x28) {
        shooter_apply_keyframe_tween(0x1A, shooter_boss1_group_f_poses_b[1], 3, 0, 0);
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 3, 0, 0);
    } else if (arg0 >= 0x2C && arg0 < 0x36) {
        shooter_apply_keyframe_tween(0x1A, shooter_boss1_group_f_poses_b[1], 4, 0, 0);
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 4, 0, 0);
    } else if (arg0 >= 0x36 && arg0 < 0x40) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_f_poses_a[i], 3, 0, 0);
        }
        shooter_apply_keyframe_tween(0x19, shooter_boss1_group_f_poses_b[0], 3, 0, 0);
        shooter_apply_keyframe_tween(0x1A, shooter_boss1_group_f_poses_b[1], 5, 0, 0);
        shooter_apply_keyframe_tween(0x1B, shooter_boss1_group_f_poses_b[2], 3, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_f_poses_c[i], 3, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 5, 0, 0);
    } else if (arg0 >= 0x40 && arg0 < 0x4A) {
        for (i = 0; i < 5; i++) {
            shooter_apply_keyframe_tween(i + 1, shooter_boss1_group_f_poses_a[i], 4, 0, 0);
        }
        shooter_apply_keyframe_tween(0x19, shooter_boss1_group_f_poses_b[0], 4, 0, 0);
        shooter_apply_keyframe_tween(0x1A, shooter_boss1_group_f_poses_b[1], 6, 0, 0);
        shooter_apply_keyframe_tween(0x1B, shooter_boss1_group_f_poses_b[2], 4, 0, 0);
        for (i = 0; i < 2; i++) {
            shooter_apply_keyframe_tween(i + 0x1D, shooter_boss1_group_f_poses_c[i], 4, 0, 0);
        }
        shooter_apply_keyframe_tween(8, shooter_boss1_group_f_pose_d, 6, 0, 0);
        for (i = 0; i != 2; i++) {
            shooter_apply_keyframe_tween(i + 0x15, shooter_boss1_group_f_poses_e[i], 2, 0, 0);
        }
    }
    if (arg0 >= 0x28 && arg0 < 0x2D) {
        matrix = *(MATRIX *)view_base_matrix;
        shooter_rotation_matrix_x((D_80128880[0].x << 12) / 360, &matrix);
        shooter_rotation_matrix_z((D_80128880[0].z << 12) / 360, &matrix);
        shooter_rotation_matrix_y((D_80128880[0].y << 12) / 360, &matrix);
        input.vx = D_80127480[8].x;
        input.vy = D_80127480[8].y;
        input.vz = D_80127480[8].z;
        ApplyMatrixSV(&matrix, &input, &output);
        shooter_actors[20].flag_bytes[1] = 0x40;
        shooter_actors[20].x = D_80127480[0].x + output.vx;
        shooter_actors[20].y = D_80127480[0].y + output.vy;
        shooter_actors[20].z = 0x800;
        shooter_actors[20].stats[0] = 0x28;
        return;
    }
    if (arg0 == 0x2D) {
        shooter_actors[20].flag_bytes[1] = 0x80;
    }
}


void shooter_boss1_rest_group_f(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        shooter_set_rotation_from_pose(i + 1, shooter_boss1_group_f_poses_a[i]);
    }
    for (i = 0; i < 3; i++) {
        shooter_set_rotation_from_pose(i + 0x19, shooter_boss1_group_f_poses_b[i]);
    }
    for (i = 0; i < 2; i++) {
        shooter_set_rotation_from_pose(i + 0x1D, shooter_boss1_group_f_poses_c[i]);
    }
    shooter_set_rotation_from_pose(8, shooter_boss1_group_f_pose_d);
    for (i = 0; i < 2; i++) {
        shooter_set_rotation_from_pose(i + 0x15, shooter_boss1_group_f_poses_e[i]);
    }
}

void shooter_boss1_run_attack_mode(void) {
    switch (shooter_actors[16].stats[2]) {
    case 0:
        shooter_boss1_mode_sway(1);
        break;
    case 1:
        shooter_boss1_mode_group_pair();
        break;
    case 2:
        shooter_boss1_mode_scatter();
        break;
    case 3:
        shooter_boss1_mode_lunge();
        break;
    case 4:
        shooter_boss1_mode_sway(-1);
        break;
    case 5:
        shooter_boss1_mode_form2_group_d();
        break;
    case 6:
        shooter_boss1_mode_form2_group_e();
        break;
    case 7:
        shooter_boss1_mode_rain();
        break;
    case 8:
        shooter_boss1_mode_form2_group_f();
        break;
    }
}

void shooter_boss1_mode_sway(s32 sway_step) {
    if (shooter_actors[16].phase >= 3) {
        shooter_boss1_sway_motion(shooter_actors[16].mode, sway_step);
        shooter_actors[16].mode += 1;
        shooter_actors[16].phase = 0;
        if (!(shooter_actors[16].mode & 1)) {
            shooter_fire_from_joint(0, 0x1B, 0x1F4, (rand() & 0xFF) - 0x80, 0x7D0, 0x12C, 0);
            shooter_fire_from_joint(0, 0x1B, -0x1F4, (rand() & 0xFF) - 0x80, 0x7D0, 0x12C, 0);
        }
    }
    if (shooter_actors[16].mode >= 0x3C || rand() % 200 == 0) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
    } else {
        shooter_actors[16].phase += 1;
    }
}

void shooter_boss1_sway_motion(s32 sway_phase, s32 sway_step) {
    if (sway_phase < 0x5) {
        shooter_move_model(0, sway_step * 0x96, 0, 0, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x5 && sway_phase < 0xA) {
        shooter_move_model(0, sway_step * 0x78, -0x1E, -0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0xA && sway_phase < 0xF) {
        shooter_move_model(0, sway_step * 0x50, -0x1E, -0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0xF && sway_phase < 0x14) {
        shooter_move_model(0, sway_step * -0x50, -0x1E, -0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x14 && sway_phase < 0x19) {
        shooter_move_model(0, sway_step * -0x78, -0x1E, -0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x19 && sway_phase < 0x23) {
        shooter_move_model(0, sway_step * -0x96, 0, 0, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x23 && sway_phase < 0x28) {
        shooter_move_model(0, sway_step * -0x78, 0x1E, 0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x28 && sway_phase < 0x2D) {
        shooter_move_model(0, sway_step * -0x50, 0x1E, 0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x2D && sway_phase < 0x32) {
        shooter_move_model(0, sway_step * 0x50, 0x1E, 0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x32 && sway_phase < 0x37) {
        shooter_move_model(0, sway_step * 0x78, 0x1E, 0xC8, 0, 0, 0, 0x32, 0x32);
    } else if (sway_phase >= 0x37 && sway_phase < 0x3C) {
        shooter_move_model(0, sway_step * 0x96, 0, 0, 0, 0, 0, 0x32, 0x32);
    }
}

void shooter_boss1_mode_group_pair(void) {
    u8 v;

    v = shooter_actors[16].mode;
    if (v < 0x19 || v >= 0x7D) {
        if (shooter_actors[16].phase >= 2) {
            shooter_boss1_group_pair_motion(v);
            shooter_actors[16].mode += 1;
            shooter_actors[16].phase = 0;
            v = shooter_actors[16].mode;
        }
    } else if (v >= 0x19 && v < 0x7D) {
        shooter_boss1_group_pair_motion(v);
        shooter_actors[16].mode += 1;
        shooter_actors[16].phase = 0;
        v = shooter_actors[16].mode;
    }
    if (v >= 0x96 || shooter_actors[17].stats[2] == 1) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
    } else {
        shooter_actors[16].phase += 1;
    }
}

void shooter_boss1_group_pair_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0xF) {
        shooter_move_model(0, 0, -0x1E, -0x1E, -1, 0, 0, 0x32, 0x32);
    } else if (elapsed_frames >= 0x10 && elapsed_frames < 0x19) {
        shooter_move_model(0, 0, -0x1E, -0x1E, 0, 0, 0, 0x32, 0x32);
    } else if (elapsed_frames == 0x19) {
        if (shooter_actors[18].stats[2] == 0) {
            shooter_actors[18].stats[2] = 1;
            shooter_boss1_rest_group_pair(9, 3);
        }
        if (shooter_actors[19].stats[2] == 0) {
            shooter_actors[19].stats[2] = 1;
            shooter_boss1_rest_group_pair(0xD, 0);
        }
    } else if (elapsed_frames >= 0x1A && elapsed_frames < 0x7D) {
        if (shooter_actors[18].stats[2] != 0 && shooter_actors[17].mode == 1) {
            shooter_boss1_update_group_pair(&shooter_actors[18].phase, &shooter_actors[18].mode, 9, 3, 1);
        }
        if (shooter_actors[19].stats[2] != 0) {
            if (shooter_actors[17].mode == 1) {
                shooter_boss1_update_group_pair(&shooter_actors[19].phase, &shooter_actors[19].mode, 0xD, 0, 1);
            }
        }
    } else if (elapsed_frames >= 0x7D && elapsed_frames < 0x87) {
        shooter_move_model(0, 0, 0x1E, 0x1E, 0, 0, 0, 0x32, 0x32);
    } else if (elapsed_frames >= 0x87 && elapsed_frames < 0x96) {
        shooter_move_model(0, 0, 0x1E, 0x1E, 1, 0, 0, 0x32, 0x32);
    }
}

void shooter_boss1_mode_scatter(void) {
    if (shooter_actors[16].phase >= 3U) {
        shooter_boss1_scatter_motion(shooter_actors[16].mode);
        shooter_actors[16].mode += 1;
        shooter_actors[16].phase = 0;
        if (!(shooter_actors[16].mode & 1)) {
            if (shooter_actors[17].mode == 1) {
                shooter_fire_from_joint(0, 0x15, (rand() & 0x1FF) + 0xF4, (rand() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
                shooter_fire_from_joint(0, 0x15, (rand() & 0x1FF) - 0x2F4, (rand() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
            } else if (shooter_boss_damage_percent(0xB, shooter_boss1_part_hit_points) < 0x32) {
                shooter_fire_from_joint(0, 0x15, (rand() & 0x1FF) + 0x2C, (rand() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
                shooter_fire_from_joint(0, 0x15, (rand() & 0x1FF) - 0x22C, (rand() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
            } else {
                shooter_fire_from_joint(0, 0x1B, (rand() & 0x3FF) - 0x200, (rand() & 0x3FF) - 0x200, 0x7D0, 0x190, 0x81);
            }
        }
    }
    if (shooter_actors[16].mode >= 0x14U) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
        return;
    }
    shooter_actors[16].phase += 1;
}

void shooter_boss1_scatter_motion(s32 elapsed_frames) {
    s32 t;

    t = (-0x41A0 - D_801274A0) / 15;
    if (elapsed_frames < 5) {
        shooter_move_model(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((elapsed_frames >= 5) && (elapsed_frames < 0xF)) {
        shooter_move_model(0, -0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((elapsed_frames >= 0xF) && (elapsed_frames < 0x14)) {
        shooter_move_model(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
}

void shooter_boss1_mode_lunge(void) {
    u8 v;

    v = shooter_actors[16].mode;
    if (v < 0x14 || v >= 0x32) {
        if (shooter_actors[16].phase >= 3) {
            shooter_boss1_lunge_motion(v);
            shooter_actors[16].mode += 1;
            shooter_actors[16].phase = 0;
            v = shooter_actors[16].mode;
        }
    } else if (v >= 0x14 && v < 0x32) {
        if (shooter_actors[16].phase != 0) {
            shooter_boss1_lunge_motion(v);
            shooter_actors[16].mode += 1;
            shooter_actors[16].phase = 0;
            v = shooter_actors[16].mode;
        }
    }
    if (v >= 0x50) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
    } else {
        shooter_actors[16].phase += 1;
    }
}

s32 shooter_boss1_lunge_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0x14) {
        shooter_move_model(0, 0, 0, -0x12C, 0, 0, 0, 0x32, 0x32);
    } else if (elapsed_frames >= 0x14 && elapsed_frames < 0x2F) {
        shooter_move_model(0, 0, 0, 0x320, 0, 0, 0, 0x14, 0x1E);
    } else if (elapsed_frames >= 0x32 && elapsed_frames < 0x50) {
        shooter_move_model(0, 0, 0, -0x208, 0, 0, 0, 0x32, 0x32);
    }
    if (elapsed_frames >= 0x2D && elapsed_frames < 0x37) {
        shooter_actors[17].flag_bytes[1] = 0x40;
        shooter_actors[17].z = 0x800;
        shooter_actors[17].stats[0] = 0x14;
        shooter_actors[18].flag_bytes[1] = 0x40;
        shooter_actors[18].z = 0x800;
        shooter_actors[18].stats[0] = 0x14;
        shooter_actors[19].flag_bytes[1] = 0x40;
        shooter_actors[19].z = 0x800;
        shooter_actors[19].stats[0] = 0x14;
        shooter_actors[20].flag_bytes[1] = 0x40;
        shooter_actors[20].z = 0x800;
        shooter_actors[20].stats[0] = 0x14;
        shooter_actors[21].flag_bytes[1] = 0x40;
        shooter_actors[21].z = 0x800;
        shooter_actors[21].stats[0] = 0x14;
        shooter_actors[22].flag_bytes[1] = 0x40;
        shooter_actors[22].z = 0x800;
        shooter_actors[22].stats[0] = 0x14;
        shooter_actors[23].flag_bytes[1] = 0x40;
        shooter_actors[23].z = 0x800;
        shooter_actors[23].stats[0] = 0x14;
        shooter_actors[24].flag_bytes[1] = 0x40;
        shooter_actors[24].z = 0x800;
        shooter_actors[24].stats[0] = 0x14;
        shooter_actors[25].flag_bytes[1] = 0x40;
        shooter_actors[25].z = 0x800;
        shooter_actors[25].stats[0] = 0x14;
        shooter_actors[26].flag_bytes[1] = 0x40;
        shooter_actors[26].z = 0x800;
        shooter_actors[26].stats[0] = 0x14;
    }
    if (elapsed_frames == 0x37) {
        shooter_actors[16].flag_bytes[1] = 0x80;
        shooter_actors[17].flag_bytes[1] = 0x80;
        shooter_actors[18].flag_bytes[1] = 0x80;
        shooter_actors[19].flag_bytes[1] = 0x80;
        shooter_actors[20].flag_bytes[1] = 0x80;
        shooter_actors[21].flag_bytes[1] = 0x80;
        shooter_actors[22].flag_bytes[1] = 0x80;
        shooter_actors[23].flag_bytes[1] = 0x80;
        shooter_actors[24].flag_bytes[1] = 0x80;
        shooter_actors[25].flag_bytes[1] = 0x80;
        shooter_actors[26].flag_bytes[1] = 0x80;
    }
}

void shooter_boss1_mode_form2_group_d(void) {
    u8 v;

    v = shooter_actors[16].mode;
    if (v < 0x28 || v >= 0x46) {
        if (shooter_actors[16].phase >= 3) {
            shooter_boss1_form2_group_d_motion(v);
            shooter_actors[16].mode += 1;
            shooter_actors[16].phase = 0;
            v = shooter_actors[16].mode;
        }
    } else if (v >= 0x28 && v < 0x46) {
        shooter_boss1_form2_group_d_motion(v);
        shooter_actors[16].mode += 1;
        shooter_actors[16].phase = 0;
        v = shooter_actors[16].mode;
    }
    if (v >= 0x79 || shooter_actors[17].stats[2] == 1) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
    } else {
        shooter_actors[16].phase += 1;
    }
}

/* s32 with no return (implicit int in the original): v0 is live at the exit, so as1 cannot put li v0,0x40 or li v0,0x80 in the branch delay slots and the constants stay after the first load, as in the original. T-19003 */
s32 shooter_boss1_form2_group_d_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0x13) {
        shooter_move_model(0, 0, 0, -0x190, 0, 0, 0, 0x32, 0x32);
    }
    if (elapsed_frames == 0x13) {
        shooter_actors[22].stats[2] = 1;
        shooter_boss1_rest_group_d();
    }
    if (elapsed_frames >= 0x14 && elapsed_frames < 0x27) {
        if (shooter_actors[22].stats[2] != 0) {
            shooter_boss1_update_group_d(0);
        }
    }
    if (elapsed_frames == 0x27) {
        view_light_0 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0xFF;
        D_8012276E = 0xFF;
        func_8009AD70(0, &view_light_0);
        view_light_1 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0xFF;
        D_8012277E = 0xFF;
        func_8009AD70(1, &view_light_1);
        view_light_2 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0xFF;
        D_8012278E = 0xFF;
        func_8009AD70(2, &view_light_2);
    }
    if (elapsed_frames >= 0x28 && elapsed_frames < 0x43) {
        shooter_move_model(0, 0, 0, 0x320, 0, 0, 0, 0x14, 0x1E);
    }
    if (elapsed_frames >= 0x46 && elapsed_frames < 0x64) {
        shooter_move_model(0, 0, 0, -0x1E0, 0, 0, 0, 0x32, 0x32);
    }
    if (elapsed_frames == 0x64) {
        shooter_actors[23].stats[2] = 1;
        shooter_actors[22].mode = 0;
        shooter_actors[22].phase = shooter_actors[22].mode;
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
    }
    if (elapsed_frames >= 0x65 && elapsed_frames < 0x79) {
        if (shooter_actors[22].stats[2] != 0) {
            shooter_boss1_update_group_d(0);
        }
    }
    if (elapsed_frames >= 0x42 && elapsed_frames < 0x48) {
        shooter_actors[16].flag_bytes[1] = 0x40;
        shooter_actors[16].z = 0x800;
        shooter_actors[16].stats[0] = 0xA;
        shooter_actors[17].flag_bytes[1] = 0x40;
        shooter_actors[17].z = 0x800;
        shooter_actors[17].stats[0] = 0x1E;
        shooter_actors[18].flag_bytes[1] = 0x40;
        shooter_actors[18].z = 0x800;
        shooter_actors[18].stats[0] = 0x1E;
    } else if (elapsed_frames == 0x48) {
        shooter_actors[16].flag_bytes[1] = 0x80;
        shooter_actors[17].flag_bytes[1] = 0x80;
        shooter_actors[18].flag_bytes[1] = 0x80;
    }
}

void shooter_boss1_mode_form2_group_e(void) {
    if (shooter_actors[16].phase >= 3U) {
        shooter_boss1_form2_group_e_motion(shooter_actors[16].mode);
        shooter_actors[16].mode = shooter_actors[16].mode + 1;
        shooter_actors[16].phase = 0;
    }
    if ((shooter_actors[16].mode >= 0x50U) || (shooter_actors[17].stats[2] == 1)) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
        return;
    }
    shooter_actors[16].phase = shooter_actors[16].phase + 1;
}

void shooter_boss1_form2_group_e_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0xE) {
        shooter_move_model(0, 0, -0x14, -0x1E, 0, 0, 0, 0x32, 0x1F4);
    } else if (elapsed_frames == 0xE) {
        shooter_actors[24].stats[2] = 1;
        shooter_boss1_rest_group_e();
    } else if ((elapsed_frames >= 0xF) && (elapsed_frames < 0x41)) {
        if (shooter_actors[24].stats[2] == 1) {
            shooter_boss1_update_group_e(0);
        }
        shooter_move_model(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((elapsed_frames >= 0x41) && (elapsed_frames < 0x50)) {
        shooter_move_model(0, 0, 0xF, 0x1E, 0, 0, 0, 0x32, 0x1F4);
    }
}

void shooter_boss1_mode_rain(void) {
    s32 pad[3]; /* FAKE: unused local; reproduces the 12 bytes of frame the original has above the spilled rand() result. T-8030 */

    if (shooter_actors[16].phase >= 3) {
        shooter_boss1_rain_group_c(shooter_actors[16].mode);
        shooter_actors[16].mode += 1;
        shooter_actors[16].phase = 0;
        shooter_fire_from_joint(0, 0x1B, (rand() & 0x3FF) - 0x138, (rand() & 0x1FF) - 0x200, 0x7D0, 0xFA, 0x40);
        shooter_fire_from_joint(0, 0x1B, (rand() & 0x3FF) - 0x2C8, (rand() & 0x1FF) - 0x200, 0x7D0, 0xFA, 0x40);
    }
    if (shooter_actors[16].mode >= 0x32 || shooter_actors[17].stats[2] == 1) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
    } else {
        shooter_actors[16].phase += 1;
    }
}

void shooter_boss1_rain_group_c(s32 elapsed_ticks) {
    if (elapsed_ticks == 1) {
        shooter_actors[21].stats[2] = 1;
        shooter_boss1_rest_group_c();
    } else if ((elapsed_ticks >= 2) && (elapsed_ticks < 0x32)) {
        if (shooter_actors[21].stats[2] == 1) {
            shooter_boss1_update_group_c(0);
        }
    }
}

void shooter_boss1_mode_form2_group_f(void) {
    if (shooter_actors[16].phase >= 3U) {
        shooter_boss1_form2_group_f_motion(shooter_actors[16].mode);
        shooter_actors[16].mode = shooter_actors[16].mode + 1;
        shooter_actors[16].phase = 0;
    }
    if ((shooter_actors[16].mode >= 0x73U) || (shooter_actors[17].stats[2] == 1)) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        shooter_boss1_pick_attack_mode();
        return;
    }
    shooter_actors[16].phase = shooter_actors[16].phase + 1;
}

void shooter_boss1_form2_group_f_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0xE) {
        shooter_move_model(0, 0, -0x14, -0x1E, -1, 0, 0, 0x32, 0x1F4);
    } else if (elapsed_frames == 0xE) {
        shooter_actors[25].stats[2] = 1;
        shooter_boss1_rest_group_f();
    } else if ((elapsed_frames >= 0xF) && (elapsed_frames < 0x64)) {
        if (shooter_actors[25].stats[2] == 1) {
            shooter_boss1_update_group_f(0);
        }
        shooter_move_model(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((elapsed_frames >= 0x64) && (elapsed_frames < 0x72)) {
        shooter_move_model(0, 0, 0xF, 0x1E, 1, 0, 0, 0x32, 0x1F4);
    }
}

void shooter_boss1_pick_attack_mode(void) {
    s32 r;
    s32 h;
    u8 t;

    r = rand();
    t = shooter_actors[17].mode;
    /* FAKE: `t ^ 0` only swaps the operands of the first bne (const reg first); found by the permuter, real source unknown. T-8030 */
    if ((t ^ 0) == 1) {
        h = r % 100;
        if (h < 0x14) {
            shooter_actors[16].stats[2] = 0;
        } else if (h >= 0x28 && h < 0x37) {
            if (shooter_actors[21].stats[1] != 0 || shooter_actors[25].stats[1] != 0) {
                shooter_actors[16].stats[2] = 1;
            } else {
                shooter_actors[16].stats[2] = 7;
            }
        } else if (h >= 0x37 && h < 0x46) {
            shooter_actors[16].stats[2] = 2;
        } else if (h >= 0x46 && h < 0x5A) {
            shooter_actors[16].stats[2] = 3;
        } else if (h >= 0x14 && h < 0x28) {
            shooter_actors[16].stats[2] = 4;
        } else if (h >= 0x5A) {
            shooter_actors[16].stats[2] = 7;
        }
    } else if (t == 0x1D) {
        h = r % 100;
        if (h >= 0 && h < 0x28) {
            shooter_actors[16].stats[2] = 2;
        } else if (h >= 0x28 && h < 0x46) {
            shooter_actors[16].stats[2] = 5;
        } else if (h >= 0x46 && h < 0x5F) {
            shooter_actors[16].stats[2] = 6;
        } else if (h >= 0x5F) {
            shooter_actors[16].stats[2] = 8;
        }
    }
}

void shooter_boss3_descend_motion(void) {
    shooter_boss3_update_sway(5);
    shooter_boss3_pose_idle(7);
    shooter_move_model(0, 0, 1, 0, 0, 0, 0, 0x32, 0x1F4);
    if (abs(D_8012749C - shooter_actors->y) < 0x64) {
        shooter_boss3_reset_parts();
        mode_step_next();
    }
}

void shooter_boss3_update_ai(void) {
    s32 i;
    s32 j;
    s32 r;

    shooter_boss3_update_sway(5);
    shooter_boss3_update_damage_stage();
    if (shooter_actors[20].mode < 0x4B && shooter_actors[18].stats[2] == 0 && shooter_actors[19].stats[1] != 0) {
        if (rand() % 200 == 0) {
            shooter_actors[18].stats[2] = 1;
            for (i = 1, j = 0; i != 7; i++, j++) {
                shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers[j * 2 - 2], shooter_boss3_idle_pose_pointers[j * 2 - 1], shooter_boss3_idle_pose_flags[j * 2 - 2], shooter_boss3_idle_pose_flags[j * 2 - 1]);
            }
            shooter_set_pose_from_tables(0x1A, 0, shooter_boss3_idle_pose_pointers_joint_26, shooter_boss3_idle_pose_pointers_joint_26_rotation, shooter_boss3_idle_pose_flags_joint_26, shooter_boss3_idle_pose_flags_joint_26_rotation);
            shooter_boss3_rest_group_attack(1, 0);
        }
    }
    if (shooter_actors[20].mode < 0x4B && shooter_actors[19].stats[2] == 0 && shooter_actors[20].stats[1] != 0) {
        r = rand() % 200;
        if (r == 0) {
            shooter_actors[19].stats[2] = 1;
            for (i = 7, j = 0; i != 0xD; i++, j++) {
                shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 1]);
            }
            shooter_set_pose_from_tables(0x1B, 0, shooter_boss3_idle_pose_pointers_joint_27, shooter_boss3_idle_pose_pointers_joint_27_rotation, shooter_boss3_idle_pose_flags_joint_27, shooter_boss3_idle_pose_flags_joint_27_rotation);
            shooter_boss3_rest_group_attack(7, 7);
        } else if (r == 1) {
            shooter_actors[19].stats[2] = 2;
            for (i = 7, j = 0; i != 0xD; i++, j++) {
                shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 1]);
            }
            shooter_set_pose_from_tables(0x1B, 0, shooter_boss3_idle_pose_pointers_joint_27, shooter_boss3_idle_pose_pointers_joint_27_rotation, shooter_boss3_idle_pose_flags_joint_27, shooter_boss3_idle_pose_flags_joint_27_rotation);
            shooter_boss3_rest_group_attack(7, 7);
        }
    }
    if (shooter_actors[18].stats[2] != 0) {
        shooter_boss3_update_group_attack_a();
    }
    j = shooter_actors[19].stats[2];
    if (j == 1) {
        shooter_boss3_update_group_attack_b();
    }
    if (shooter_actors[19].stats[2] == 2) {
        shooter_boss3_update_group_attack_c();
    }
}

void shooter_boss3_defeat_motion(void) {
    if (game_state.mode_step_timer.u < 0x64U) {
        shooter_boss3_pose_idle(1);
        shooter_boss3_update_sway(5U);
        shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
    } else if (game_state.mode_step_timer.u >= 0x64U && game_state.mode_step_timer.u < 0x1F4U) {
        shooter_boss3_update_sway(5U);
        shooter_boss3_pose_idle((game_state.mode_step_timer.u - 0x3C) / 40U);
    } else if (game_state.mode_step_timer.u >= 0x226U) {
        if (game_state.clear_color_r < 0xFFU) {
            /* FAKE: repeated masked sum makes a uopt CSE range in $v0 while keeping the ugen temporary slot (T-22006, T-24120). */
            game_state.clear_color_g = (game_state.clear_color_b + 1) & 0xFF;
            game_state.clear_color_r = game_state.clear_color_b = (game_state.clear_color_b + 1) & 0xFF;
            shooter_fade_white(game_state.mode_step_timer.u - 0x226);
        } else {
            view_light_0 = 0x64;
            D_80122764 = 0x32;
            D_80122768 = -0x64;
            D_8012276C = 0x70;
            D_8012276D = 0x70;
            D_8012276E = 0x80;
            func_8009AD70(0, &view_light_0);
            view_light_1 = 0;
            D_80122774 = -0x64;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &view_light_1);
            view_light_2 = -0x64;
            D_80122784 = 0x32;
            D_80122788 = -0x64;
            D_8012278C = 0x80;
            D_8012278D = 0x80;
            D_8012278E = 0x90;
            func_8009AD70(2, &view_light_2);
            game_state.mode_step_timer.u = 0;
            mode_step_next();
        }
    }
    if (game_state.mode_step_timer.u == 0x258) {
        shooter_play_se(0, 0x502);
    }
    game_state.mode_step_timer.u += 1;
}

/* FAKE: s32 return view keeps v0 live at the exit, so uopt keeps the masked chain value (andi $v0,$t9,0xFF); callers ignore the result. T-24136 */
s32 shooter_boss3_collapse_motion(void) {
    s32 i;

    if (game_state.mode_step_timer.u == 1) {
        for (i = 0; i < 0x21; i++) {
            shooter_actors[i + 27].z = (rand() % 8) + 5;
        }
    }
    for (i = 0; i < 0x21; i++) {
        D_80127480[i + 1].y += shooter_actors[i + 27].z;
    }
    if (game_state.clear_color_r != 0) {
        /* FAKE: element chain over the selector bytes stores 037, 036, then the raw decrement to 038 in $t9 (T-24213). */
        (&game_state.clear_color_r)[0] = (&game_state.clear_color_r)[1] = --(&game_state.clear_color_r)[2];
    }
    if (game_state.mode_step_timer.u < 0x191U) {
        shooter_boss_death_explosions(0x11);
    }
    if (game_state.mode_step_timer.u >= 0x258U) {
        shooter_stages_cleared += 1;
        mode_stage_set(0x2D);
        return;
    }
    game_state.mode_step_timer.u += 1;
}

void shooter_boss3_update_damage_stage(void) {
    if ((shooter_boss_damage_percent(0xB, shooter_boss3_part_hit_points) != shooter_actors[20].mode) != 0) { /* FAKE: the redundant != 0 burns the $t7 temporary the original skips (permuter); real source unknown. T-9320 */
        shooter_actors[20].mode = shooter_boss_damage_percent(0xB, shooter_boss3_part_hit_points);
        shooter_actors[20].stats[2] = 1U;
    }
    if (shooter_actors[20].mode < 0x20) {
        shooter_boss3_pose_idle(6);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
            shooter_actors[20].stats[2] = 0;
        }
    } else if (shooter_actors[20].mode >= 0x20 && shooter_actors[20].mode < 0x30) {
        shooter_boss3_pose_idle(5);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
            shooter_actors[20].stats[2] = 0;
        }
    } else if (shooter_actors[20].mode >= 0x30 && shooter_actors[20].mode < 0x40) {
        shooter_boss3_pose_idle(4);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
            shooter_actors[20].stats[2] = 0;
        }
    } else if (shooter_actors[20].mode >= 0x40 && shooter_actors[20].mode < 0x48) {
        shooter_boss3_pose_idle(3);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
            shooter_actors[20].stats[2] = 0;
        }
    } else if (shooter_actors[20].mode >= 0x48 && shooter_actors[20].mode < 0x64) {
        shooter_boss3_pose_idle(2);
        if (shooter_actors[20].stats[2] != 0) {
            shooter_flicker_model_clut(0x801A7000, 0xA, 0x100, 0x1EF);
            shooter_actors[20].stats[2] = 0;
        }
    } else {
        game_state.mode_step_timer.u = 0;
        mode_step_next();
    }
}

void shooter_boss3_pose_idle(s32 arg0) {
    s32 i;
    s32 j;

    if ((u32)arg0 < shooter_actors[17].phase) {
        if (shooter_actors[18].stats[2] == 1 || shooter_actors[19].stats[2] == 1 || shooter_actors[19].stats[2] == 2) {
            for (i = 0xD, j = 0; i < 0x1A; i++, j++) {
                shooter_set_pose_from_tables(i, shooter_actors[17].mode, shooter_boss3_idle_pose_pointers_joint_13[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_13[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_13[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_13[j * 2 - 1]);
            }
            for (i = 0x1C, j = 0; i != 0x22; i++, j++) {
                shooter_set_pose_from_tables(i, shooter_actors[17].mode, shooter_boss3_idle_pose_pointers_joint_28[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_28[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_28[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_28[j * 2 - 1]);
            }
        } else {
            for (i = 1, j = 0; i != 0x22; i++, j++) {
                shooter_set_pose_from_tables(i, shooter_actors[17].mode, shooter_boss3_idle_pose_pointers[j * 2 - 2], shooter_boss3_idle_pose_pointers[j * 2 - 1], shooter_boss3_idle_pose_flags[j * 2 - 2], shooter_boss3_idle_pose_flags[j * 2 - 1]);
            }
        }
        shooter_actors[17].mode += 1;
        shooter_actors[17].phase = 0;
    }
    if (shooter_actors[17].mode >= 0x30U) {
        shooter_actors[17].phase = shooter_actors[17].mode = 0;
        return;
    }
    shooter_actors[17].phase += 1;
}

void shooter_boss3_update_group_attack_a(void) {
    s32 i;
    s32 j;
    u8 t;

    t = shooter_actors[18].mode;
    if (t < 0x1C || t >= 0x1E) {
        if (shooter_actors[18].phase >= 4U) {
            shooter_boss3_pose_group_attack_a(t);
            shooter_actors[18].mode += 1;
            shooter_actors[18].phase = 0;
            t = shooter_actors[18].mode;
        }
    } else if ((t >= 0x1C && t < 0x1E) || (t >= 0x23 && t < 0x25) || (t >= 0x2A && t < 0x2C)) {
        if (shooter_actors[18].phase >= 2U) {
            shooter_boss3_pose_group_attack_a(t);
            shooter_actors[18].mode += 1;
            shooter_actors[18].phase = 0;
            t = shooter_actors[18].mode;
        }
    }
    if (t >= 0x4D) {
        shooter_actors[18].stats[2] = 0;
        shooter_actors[18].phase = shooter_actors[18].mode = shooter_actors[18].stats[2];
        for (i = 1, j = 0; i != 7; i++, j++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers[j * 2 - 2], shooter_boss3_idle_pose_pointers[j * 2 - 1], shooter_boss3_idle_pose_flags[j * 2 - 2], shooter_boss3_idle_pose_flags[j * 2 - 1]);
        }
        shooter_set_pose_from_tables(0x1B, 0, shooter_boss3_idle_pose_pointers_joint_27, shooter_boss3_idle_pose_pointers_joint_27_rotation, shooter_boss3_idle_pose_flags_joint_27, shooter_boss3_idle_pose_flags_joint_27_rotation);
        return;
    }
    shooter_actors[18].phase += 1;
}

s32 shooter_boss3_pose_group_attack_a(s32 arg0) {
    s32 i;
    s32 random;
    s32 *tail;
    /* FAKE: group loop initializers on the do line to preserve scheduling. T-24502 */
    if (arg0 < 0xA) {
        i = 0; do {
            shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 1, 0, 0);
            i++;
        } while (i != 6);
        shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 1, 0, 0);
        return;
    }
    if (arg0 >= 0xE) {
        i = 0;
        if (arg0 < 0x18) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 2, 0, 0);
                i++;
            } while (i != 4);
            return;
        }
    }
    if (arg0 == 0x1B) {
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        return;
    }
    if (arg0 >= 0x1C) {
        i = 0;
        if (arg0 < 0x1E) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 3, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 2, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x1E) {
        i = 0;
        if (arg0 < 0x22) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 4, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 3, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 3, 0, 0);
            return;
        }
    }
    if (arg0 == 0x22) {
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x12C, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x1F4, 0xC1);
        return;
    }
    if (arg0 >= 0x23) {
        i = 0;
        if (arg0 < 0x25) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 3, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 2, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x25) {
        i = 0;
        if (arg0 < 0x29) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 4, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 3, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 3, 0, 0);
            return;
        }
    }
    if (arg0 == 0x29) {
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        random = rand();
        shooter_fire_from_joint(0, 5, (s16) ((random & 0x7F) - 0x40), (s16) ((rand() & 0x3F) - 0x20), 0x3E8, 0x12C, 0xC1);
        return;
    }
    if (arg0 >= 0x2A) {
        i = 0;
        if (arg0 < 0x2C) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 3, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 2, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x2C) {
        i = 0;
        if (arg0 < 0x31) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 4, 0, 0);
                i++;
            } while (i < 4);
            tail = &shooter_boss3_rest_rotation_references[4]; i = 0; do {
                shooter_apply_keyframe_tween(i + 5, (s16 *)tail[i], 3, 0, 0);
                i += 1;
            } while (i != 2);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 3, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x35) {
        i = 0;
        if (arg0 < 0x3F) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 5, 0, 0);
                i++;
            } while (i != 4);
            shooter_apply_keyframe_tween(6, (s16 *)shooter_boss3_rest_rotation_references[5], 4, 0, 0);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 4, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x43) {
        i = 0;
        if (arg0 < 0x4D) {
            do {
                shooter_apply_keyframe_tween(i + 1, (s16 *)shooter_boss3_rest_rotation_references[i], 6, 0, 0);
                i++;
            } while (i != 4);
            shooter_apply_keyframe_tween(5, (s16 *)shooter_boss3_rest_rotation_references[4], 4, 0, 0);
            shooter_apply_keyframe_tween(6, (s16 *)shooter_boss3_rest_rotation_references[5], 5, 0, 0);
            shooter_apply_keyframe_tween(0x1A, (s16 *) shooter_boss3_rest_rotation_references_6, 5, 0, 0);
        }
    }
}


void shooter_boss3_update_group_attack_b(void) {
    s32 i;
    s32 j;
    u8 t;

    t = shooter_actors[19].mode;
    if (t < 0x1C || t >= 0x34) {
        if (shooter_actors[19].phase >= 4U) {
            shooter_boss3_pose_group_attack_b(t);
            shooter_actors[19].mode += 1;
            shooter_actors[19].phase = 0;
            t = shooter_actors[19].mode;
        }
    } else if (t >= 0x1C && t < 0x26) {
        if (shooter_actors[19].phase != 0) {
            shooter_boss3_pose_group_attack_b(t);
            shooter_actors[19].mode += 1;
            shooter_actors[19].phase = 0;
            t = shooter_actors[19].mode;
        }
    } else if (t >= 0x26 && t < 0x34) {
        if (shooter_actors[19].phase >= 3U) {
            shooter_boss3_pose_group_attack_b(t);
            shooter_actors[19].mode += 1;
            shooter_actors[19].phase = 0;
            t = shooter_actors[19].mode;
        }
    }
    if (t >= 0x5A) {
        shooter_actors[19].stats[2] = 0;
        shooter_actors[19].phase = shooter_actors[19].mode = shooter_actors[19].stats[2];
        for (i = 7, j = 0; i != 0xD; i++, j++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 1]);
        }
        shooter_set_pose_from_tables(0x1B, 0, shooter_boss3_idle_pose_pointers_joint_27, shooter_boss3_idle_pose_pointers_joint_27_rotation, shooter_boss3_idle_pose_flags_joint_27, shooter_boss3_idle_pose_flags_joint_27_rotation);
        return;
    }
    shooter_actors[19].phase += 1;
}

void shooter_boss3_pose_group_attack_b(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 1, 0, 0);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 1, 0, 0);
    } else if (arg0 >= 0xE && arg0 < 0x18) {
        for (i = 0; i != 4; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 2, 0, 0);
        }
    } else if (arg0 >= 0x1C && arg0 < 0x26) {
        for (i = 0; i != 5; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 3, 0, 0);
            shooter_set_object_flat_color(i + 7, 0x17);
        }
    } else if (arg0 >= 0x2A && arg0 < 0x34) {
        for (i = 0; i != 5; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 4, 0, 0);
            shooter_set_object_flat_color(i + 7, -0x17);
        }
    } else if (arg0 >= 0x38 && arg0 < 0x42) {
        for (i = 0; i != 4; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 5, 0, 0);
        }
    } else if (arg0 >= 0x46 && arg0 < 0x50) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 6, 0, 0);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 6, 0, 0);
    }
    if (arg0 >= 0x24 && arg0 < 0x2C) {
        shooter_actors[20].flag_bytes[1] = 0x40;
        shooter_actors[20].z = 0x800;
        shooter_actors[20].stats[0] = 0x1E;
        return;
    }
    if (arg0 == 0x2C) {
        shooter_actors[20].flag_bytes[1] = 0x80;
    }
}

void shooter_boss3_update_group_attack_c(void) {
    s32 i;
    s32 j;

    if (shooter_actors[19].phase >= 5U) {
        shooter_boss3_pose_group_attack_c(shooter_actors[19].mode);
        shooter_actors[19].mode += 1;
        shooter_actors[19].phase = 0;
    }
    if (shooter_actors[19].mode >= 0x64U) {
        shooter_actors[19].stats[2] = 0;
        shooter_actors[19].phase = shooter_actors[19].mode = shooter_actors[19].stats[2];
        for (i = 7, j = 0; i < 0xD; i++, j++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 2], shooter_boss3_idle_pose_pointers_joint_7[j * 2 - 1], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 2], shooter_boss3_idle_pose_flags_joint_7[j * 2 - 1]);
        }
        shooter_set_pose_from_tables(0x1B, 0, shooter_boss3_idle_pose_pointers_joint_27, shooter_boss3_idle_pose_pointers_joint_27_rotation, shooter_boss3_idle_pose_flags_joint_27, shooter_boss3_idle_pose_flags_joint_27_rotation);
        return;
    }
    shooter_actors[19].phase += 1;
}

void shooter_boss3_pose_group_attack_c(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 1, 0, 0);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 1, 0, 0);
    } else if (arg0 >= 0xE && arg0 < 0x18) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 2, 0, 0);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 2, 0, 0);
    } else if (arg0 >= 0x1C && arg0 < 0x3C) {
        for (i = 0; i != 6; i++) {
            shooter_set_object_flat_color(i + 7, 7);
        }
        shooter_set_object_flat_color(0x1B, 7);
    } else if (arg0 >= 0x50 && arg0 < 0x5A) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 5, 0, 0);
            shooter_set_object_flat_color(i + 7, -0x17);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 5, 0, 0);
        shooter_set_object_flat_color(0x1B, -0x17);
    } else if (arg0 >= 0x5A && arg0 < 0x64) {
        for (i = 0; i != 6; i++) {
            shooter_apply_keyframe_tween(i + 7, shooter_boss3_group_attack_b_poses[i], 6, 0, 0);
        }
        shooter_apply_keyframe_tween(0x1B, shooter_boss3_rest_rotation_references_13, 6, 0, 0);
    }
    if (arg0 >= 0x32 && arg0 < 0x46) {
        shooter_fire_from_joint(0, 0xB, 0, 0, 0x3E8, 0x1F4, 0x86);
    }
}

/* T-24705: the 0x1 web (bit 11, priority 3) is coloured before the compiler-made `S31+2` web (bit 8, priority 1) and takes the free $v0. `1`/`1U`/`0x1`/`(u8)1`/`1 & 0xFF`/`1 << 0`/a char literal, byte-view stores, the compare operand order and a two-target chain store keep all 7 rows (the chain store changes 174 lines). */
void shooter_boss3_rest_group_attack(s32 first_model_slot, s32 rest_group_index)
{
  s32 i;
  s32 n;
  s32 *p;
  /* FAKE: T-25047, i is always zero but folds after uopt. Alternating + and |
   * preserves nine intermediate ugen temporaries, wrapping its ring to correct
   * the extra slot spent on rest_group_index's home reload. As1 removes these identities.
   * Keep the setup on one line for instruction scheduling. */
  i = (u32)first_model_slot < 0; n = ((((((((((first_model_slot + i) | i) + i) | i) + i) | i) + i) | i) + i) | i); p = shooter_boss3_rest_rotation_references + rest_group_index;
  do
  {
    shooter_set_rotation_from_pose(n, *p);
    i += 1;
    n += 1;
    p += 1;
  }
  while (i != 6);
  if (rest_group_index != 0)
  {
    shooter_set_rotation_from_pose(0x1B, shooter_boss3_rest_rotation_references_13);
    return;
  }
  shooter_set_rotation_from_pose(0x1A, shooter_boss3_rest_rotation_references_6);
}

void shooter_boss3_update_sway(u32 phase_threshold) {
    if (phase_threshold < shooter_actors[16].phase) {
        shooter_boss3_sway_motion(shooter_actors[16].mode);
        shooter_actors[16].mode = shooter_actors[16].mode + 1;
        shooter_actors[16].phase = 0;
    }
    if (shooter_actors[16].mode >= 0x7CU) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        return;
    }
    shooter_actors[16].phase = shooter_actors[16].phase + 1;
}

void shooter_boss3_sway_motion(s32 elapsed_frames) {
    if (elapsed_frames < 0xA) {
        shooter_move_model(0, 0x12, 0, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0xA && elapsed_frames < 0x14) {
        shooter_move_model(0, 0x12, -0xC, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x14 && elapsed_frames < 0x1E) {
        shooter_move_model(0, 0x12, -0xD, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x20 && elapsed_frames < 0x2A) {
        shooter_move_model(0, -0x12, 0x14, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x2A && elapsed_frames < 0x34) {
        shooter_move_model(0, -0x12, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x34 && elapsed_frames < 0x48) {
        shooter_move_model(0, -0x12, 0, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x48 && elapsed_frames < 0x52) {
        shooter_move_model(0, -0x12, -0xC, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x52 && elapsed_frames < 0x5C) {
        shooter_move_model(0, -0x12, -0xD, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x5E && elapsed_frames < 0x68) {
        shooter_move_model(0, 0x12, 0x14, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x68 && elapsed_frames < 0x72) {
        shooter_move_model(0, 0x12, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
    } else if (elapsed_frames >= 0x72 && elapsed_frames < 0x7C) {
        shooter_move_model(0, 0x12, 0, 0, 0, 0, 0, 0xC8, 0xC8);
    }
}

void shooter_boss3_set_part_visibility(s32 arg0, s32 arg1) {
    if (!(arg1 & 1)) {
        D_80127080[arg0].attribute = 0x40000000;
        return;
    }
    D_80127080[arg0].attribute = 0x80000000;
}

void shooter_boss3_set_lights(void) {
    view_light_0 = 0x64;
    D_80122764 = 0x32;
    D_80122768 = -0x64;
    D_8012276C = 0x70;
    D_8012276D = 0x70;
    D_8012276E = 0x80;
    func_8009AD70(0, &view_light_0);
    view_light_1 = 0;
    D_80122774 = -0x64;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &view_light_1);
    view_light_2 = -0x64;
    D_80122784 = 0x32;
    D_80122788 = -0x64;
    D_8012278C = 0x80;
    D_8012278D = 0x80;
    D_8012278E = 0x90;
    func_8009AD70(2, &view_light_2);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

void shooter_fade_white(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    prim_gradient_rect(-0x100, -0x78, 0x100, 0xF0, 1, color, 0x81);
    prim_gradient_rect(0, -0x78, 0x100, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

void shooter_boss3_fire_pattern(void) {
    u32 m;
    s32 r;

    if (shooter_actors[16].stats[1] != 0) {
        m = shooter_actors[16].mode % 10;
        if (m == 0) {
            shooter_fire_from_joint(0, 0x19, 0, 0, 0x3E8, 0x96, 0xC5);
            shooter_play_se(0, 0x501);
            m = shooter_actors[16].mode % 10;
        }
        if (m == 5) {
            shooter_fire_from_joint(0, 0x19, 0, 0, 0x3E8, 0x96, 0xC5);
        }
    } else if (shooter_actors[17].stats[1] != 0) {
        if (shooter_actors[16].mode % 5 == 1) {
            shooter_fire_from_joint(0, 0x19, 0, -0x2BC, 0x3E8, 0x12C, 0xC4);
        }
    } else if (shooter_actors[18].stats[1] != 0) {
        if ((shooter_actors[16].mode & 3) == 1) {
            shooter_fire_from_joint(0, 0x19, 0, -0x5DC, 0x3E8, 0x190, 0x84);
        }
    } else if (shooter_actors[16].mode % 3 == 1) {
        r = rand();
        shooter_fire_from_joint(0, 0x19, (r & 0x1FF) - 0x100, (rand() & 0x1FF) - 0x100, 0x3E8, 0x190, 0x84);
    }
    if ((shooter_actors[19].stats[1] == 0 || shooter_actors[20].stats[1] == 0) && !(shooter_actors[16].mode & 1)) {
        if (shooter_actors[21].stats[1] != 0) {
            shooter_fire_from_joint(0, 0xD, 0, 0, 0x3E8, 0x190, 0x81);
        }
        if (shooter_actors[24].stats[1] != 0) {
            shooter_fire_from_joint(0, 0x13, 0, 0, 0x3E8, 0x190, 0x81);
        }
        if (shooter_actors[22].stats[1] != 0) {
            shooter_fire_from_joint(0, 0xF, 0, 0, 0x3E8, 0x190, 0x81);
        }
        if (shooter_actors[25].stats[1] != 0) {
            shooter_fire_from_joint(0, 0x15, 0, 0, 0x3E8, 0x190, 0x81);
        }
        if (shooter_actors[23].stats[1] != 0) {
            shooter_fire_from_joint(0, 0x11, 0, 0, 0x3E8, 0x190, 0x81);
        }
        if (shooter_actors[26].stats[1] != 0) {
            shooter_fire_from_joint(0, 0x17, 0, 0, 0x3E8, 0x190, 0x81);
        }
    }
}
