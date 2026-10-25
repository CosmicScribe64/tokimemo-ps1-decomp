/* arm_tail_boss_attacks.c: src/ovl/TACO/80151C30.c:4: articulated arms, core, tail, barrages and idle sway attack controllers; body 8: if (shooter_actors[16].unk84[2] == 0) {; 9: shooter_boss2_update_barrage(5);. Original object 0x80151C30-0x80153AA0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_boss2_update_ai(void) {
    s32 pad[4]; /* FAKE: unused slots above r, the original frame is 16 bytes larger (T-3330 idiom, real source unknown). T-6010 */
    s32 r;

    if (shooter_actors[16].stats[2] == 0) {
        shooter_boss2_update_barrage(5);
    }
    shooter_boss2_update_core(6);
    shooter_boss2_update_tail(4);
    shooter_boss2_update_arms();
    shooter_boss2_update_idle_sway();
    if (shooter_actors[16].stats[1] == 0 && shooter_actors[17].stats[1] == 0 && shooter_actors[18].stats[1] == 0) {
        r = rand();
        shooter_fire_from_joint(0, 0x1C, (s16)((r & 0x1FF) - 0x100), (s16)((rand() & 0x1FF) - 0x100), 0x3E8, 0x190, 0xC4);
    }
}

s32 shooter_boss2_update_barrage(u32 phase_threshold) {
    s32 i;
    s32 r;

    if (phase_threshold < shooter_actors[16].phase) {
        for (i = 0x16; i != 0x1C; i++) {
            shooter_set_pose_from_tables(i, shooter_actors[16].mode, shooter_boss2_part_pose_pointers[i - 1].translation, shooter_boss2_part_pose_pointers[i - 1].rotation, shooter_boss2_part_pose_flags[i - 1].translation_fixed, shooter_boss2_part_pose_flags[i - 1].rotation_fixed);
        }
        if (shooter_actors[16].mode % 10 == 0) {
            shooter_play_se(0, 0x502);
        }
        shooter_actors[16].mode = shooter_actors[16].mode + 1;
        shooter_actors[16].phase = 0;
        if (shooter_actors[16].mode % 3 == 0) {
            r = rand();
            shooter_fire_from_joint(0, 0x16, (s16)((r & 0x1FF) - 0x100), (s16)((rand() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
            r = rand();
            shooter_fire_from_joint(0, 0x17, (s16)((r & 0x1FF) - 0x100), (s16)((rand() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
        }
    }
    if (shooter_actors[16].mode >= 0x1E) {
        shooter_actors[16].mode = 0;
        shooter_actors[16].phase = shooter_actors[16].mode;
        for (i = 0x16; i != 0x1C; i++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss2_part_pose_pointers[i - 1].translation, shooter_boss2_part_pose_pointers[i - 1].rotation, shooter_boss2_part_pose_flags[i - 1].translation_fixed, shooter_boss2_part_pose_flags[i - 1].rotation_fixed);
        }
        if (shooter_actors[17].stats[2] == 1) {
            shooter_actors[16].stats[2] = 1;
        }
    } else {
        shooter_actors[16].phase = shooter_actors[16].phase + 1;
    }
}

void shooter_boss2_update_arm_attack_a(u8 *phase_counter, u8 *pose_mode, s32 arm_index, s32 pose_table_index, s32 frame_threshold) {
    s32 pad[15]; /* FAKE: unused slots above idx, the original frame is 56 bytes larger (T-3330 idiom, real source unknown). T-9370 */
    s32 idx;

    if ((u32)frame_threshold < *phase_counter) {
        shooter_boss2_pose_arm_a(arm_index, pose_table_index, *pose_mode);
        *pose_mode += 1;
        *phase_counter = 0;
    }
    if (*pose_mode >= 0x1E && *pose_mode < 0x21) {
        if (arm_index == 1) {
            idx = 0x10;
        } else if (arm_index == 2) {
            idx = 0x11;
        } else if (arm_index == 3) {
            idx = 0x12;
        }
        if ((shooter_actors + idx)->stats[1] != 0) {
            shooter_fire_from_joint(0, arm_index, 0, 0x64, 0x3E8, 0xC8, 0x83);
            if (*phase_counter == 0 && *pose_mode == 0x1E) {
                shooter_play_se(0, 0x501);
            }
        }
    }
    if (*pose_mode >= 0x28) {
        *phase_counter = *pose_mode = 0;
        shooter_boss2_pick_arm_attack(arm_index);
        shooter_boss2_rest_arm(arm_index, pose_table_index);
    } else {
        *phase_counter += 1;
    }
}

/* The table is indexed (shooter_boss2_arm_poses[arg1]), not walked with a local pointer: uopt then knows the
 * loads do not alias the stack, and as1 leaves the stack-argument store for the jal delay slot
 * (T-9210). */
void shooter_boss2_pose_arm_a(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;

    if (arg2 < 0x14) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            shooter_apply_keyframe_tween(j, shooter_boss2_arm_poses[arg1], 1, 0, 0);
        }
    } else if (arg2 >= 0x14 && arg2 < 0x28) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            shooter_apply_keyframe_tween(j, shooter_boss2_arm_poses[arg1], 2, 0, 0);
        }
    }
}

/* FAKE: the pair tables are read through word and byte casts at i * 2 - 2, not as TcPair records at i - 1: both pointer
 * walks then start from one shared product. Real source unknown. T-16563 */
void shooter_boss2_rest_arm(s32 first_arm_part_index, s32 arm_pose_index) {
    s32 i;

    for (i = first_arm_part_index; i < first_arm_part_index + 0x13; i += 3) {
        shooter_set_pose_from_tables(i, 0, ((s32 *)shooter_boss2_part_pose_pointers)[i * 2 - 2], ((s32 *)shooter_boss2_part_pose_pointers)[i * 2 - 1], ((u8 *)shooter_boss2_part_pose_flags)[i * 2 - 2], ((u8 *)shooter_boss2_part_pose_flags)[i * 2 - 1]);
        shooter_set_rotation_from_pose(i, shooter_boss2_arm_poses[arm_pose_index]);
        arm_pose_index++;
    }
}

void shooter_boss2_update_arm_attack_c(u8 *phase_counter, u8 *pose_mode, s32 arm_index, s32 pose_table_index) {
    u8 attack_frame;

    attack_frame = *pose_mode;
    if ((attack_frame < 0x1EU) || (attack_frame >= 0x23U)) {
        if (*phase_counter >= 3U) {
            shooter_boss2_pose_arm_c(arm_index, pose_table_index, (s32) attack_frame);
            *pose_mode += 1;
            *phase_counter = 0;
            attack_frame = *pose_mode;
        }
    } else if (attack_frame < 0x23U) {
        if (attack_frame >= 0x1EU) {
            if (*phase_counter != 0) {
                shooter_boss2_pose_arm_c(arm_index, pose_table_index, (s32) attack_frame);
                *pose_mode += 1;
                *phase_counter = 0;
                attack_frame = *pose_mode;
            }
        }
    }
    if (attack_frame >= 0x2D) {
        *pose_mode = 0;
        *phase_counter = 0;
        shooter_boss2_pick_arm_attack(arm_index);
        shooter_boss2_rest_arm(arm_index, pose_table_index);
        return;
    }
    /* FAKE: redundant byte mask reserves the original increment temporary;
     * real source unknown. T-9760 */
    *phase_counter = (u8)((*phase_counter & 0xFF) + 1);
}

void shooter_boss2_pose_arm_c(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 pad[15]; /* FAKE: unused slots above k; the original frame is 0x40 larger and k sits at sp+0x4C (real source unknown). T-16563 */
    s32 k;
    s32 pad2; /* FAKE: one more unused slot below k, T-16563 */

    if (arg2 < 8) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 3, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 8 && arg2 < 0x10) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 4, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 0x10 && arg2 < 0x1A) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 5, 0, 0);
            arg1++;
        }
    } else if (arg2 == 0x1D) {
        shooter_play_se(0, 0x501);
    } else if (arg2 >= 0x1E && arg2 < 0x23) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 6, 0, 0);
            arg1++;
        }
        if (arg0 == 1) {
            k = 0x10;
        } else if (arg0 == 2) {
            k = 0x11;
        } else if (arg0 == 3) {
            k = 0x12;
        }
        if ((k + shooter_actors)->stats[1] != 0) {
            shooter_fire_from_joint(0, arg0, 0, 0x64, 0x3E8, 0xC8, 0x83);
        }
    } else if (arg2 >= 0x23 && arg2 < 0x2D) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 7, 0, 0);
            arg1++;
        }
    }
}

void shooter_boss2_update_core(u32 phase_threshold) {
    if (phase_threshold < shooter_actors[17].phase) {
        shooter_boss2_pose_core((s32) shooter_actors[17].mode);
        shooter_actors[17].mode = shooter_actors[17].mode + 1;
        shooter_actors[17].phase = 0U;
    }
    if (shooter_actors[17].mode >= 0x14U) {
        shooter_actors[17].mode = 0U;
        shooter_actors[17].phase = shooter_actors[17].mode;
        shooter_set_pose_from_tables(0x23, 0, shooter_boss2_core_pose_pointers, shooter_boss2_core_pose_pointers_rotation, shooter_boss2_core_pose_flags, shooter_boss2_core_pose_flags_rotation);
        shooter_set_rotation_from_pose(0x23, (s32) shooter_boss2_core_tail_poses[0]);
        return;
    }
    shooter_actors[17].phase = shooter_actors[17].phase + 1;
}

void shooter_boss2_pose_core(s32 arg0) {
    if (arg0 < 5) {
        shooter_apply_keyframe_tween(0x23, shooter_boss2_core_tail_poses[0], 1, 0, 0);
        return;
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        shooter_apply_keyframe_tween(0x23, shooter_boss2_core_tail_poses[0], 2, 0, 0);
        return;
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        shooter_apply_keyframe_tween(0x23, shooter_boss2_core_tail_poses[0], 1, 0, 0);
    }
}

s32 shooter_boss2_update_tail(u32 phase_threshold) {
    s32 i;

    if (phase_threshold < shooter_actors[21].phase) {
        shooter_boss2_pose_tail(0x1D, shooter_actors[21].mode);
        shooter_actors[21].mode = shooter_actors[21].mode + 1;
        shooter_actors[21].phase = 0;
    }
    if (shooter_actors[21].mode >= 0x46) {
        shooter_actors[21].mode = 0;
        shooter_actors[21].phase = shooter_actors[21].mode;
        i = 0x1D;
        for (; i != 0x21; i++) {
            shooter_set_pose_from_tables(i, 0, shooter_boss2_part_pose_pointers[i - 1].translation, shooter_boss2_part_pose_pointers[i - 1].rotation, shooter_boss2_part_pose_flags[i - 1].translation_fixed, shooter_boss2_part_pose_flags[i - 1].rotation_fixed);
            /* FAKE: pointer table read from the base shooter_boss2_tail_rest_poses (see TACO.h); shooter_boss2_core_tail_poses[i - 0x1C] gives another offset. T-16563 */
            shooter_set_rotation_from_pose(i, shooter_boss2_tail_rest_poses[i]);
        }
    } else {
        shooter_actors[21].phase = shooter_actors[21].phase + 1;
    }
}

void shooter_boss2_pose_tail(s32 arg0, s32 arg1) {
    s32 i;

    if (arg1 < 8) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 1, 0, 0);
        }
    } else if (arg1 >= 8 && arg1 < 0x10) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 2, 0, 0);
        }
    } else if (arg1 >= 0x10 && arg1 < 0x18) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 3, 0, 0);
        }
    } else if (arg1 >= 0x18 && arg1 < 0x20) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 4, 0, 0);
        }
    } else if (arg1 >= 0x20 && arg1 < 0x28) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 5, 0, 0);
        }
    } else if (arg1 >= 0x28 && arg1 < 0x2E) {
        for (i = 0; i < 4; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 6, 0, 0);
        }
    } else if (arg1 >= 0x2E && arg1 < 0x36) {
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 7, 0, 0);
        }
    } else if (arg1 >= 0x36 && arg1 < 0x3E) {
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 8, 0, 0);
        }
        shooter_apply_keyframe_tween(0x20, shooter_boss2_core_tail_poses[4], 7, 0, 0);
    } else if (arg1 >= 0x3E && arg1 < 0x46) {
        for (i = 0; i < 3; i++) {
            shooter_apply_keyframe_tween(arg0 + i, shooter_boss2_core_tail_poses[i + 1], 9, 0, 0);
        }
        shooter_apply_keyframe_tween(0x20, shooter_boss2_core_tail_poses[4], 8, 0, 0);
    }
}

void shooter_boss2_update_all_arms_attack(s32 attack_frame_limit) {
    if ((u32) attack_frame_limit < shooter_actors[18].phase) {
        shooter_boss2_pose_all_arms(1, 0, shooter_actors[18].mode);
        shooter_boss2_pose_all_arms(2, 7, shooter_actors[18].mode);
        shooter_boss2_pose_all_arms(3, 0xE, shooter_actors[18].mode);
        shooter_actors[18].mode = shooter_actors[18].mode + 1;
        shooter_actors[18].phase = 0U;
    }
    if (shooter_actors[18].mode == 0x50) {
        shooter_actors[16].stats[2] = 0;
    }
    if (shooter_actors[18].mode >= 0x6EU) {
        shooter_actors[18].mode = 0U;
        shooter_actors[18].phase = shooter_actors[18].mode;
        shooter_actors[17].stats[2] = 0;
        shooter_boss2_pick_arm_attack(1);
        shooter_boss2_pick_arm_attack(2);
        shooter_boss2_pick_arm_attack(3);
        shooter_boss2_rest_arm(1, 0);
        shooter_boss2_rest_arm(2, 7);
        shooter_boss2_rest_arm(3, 0xE);
        return;
    }
    shooter_actors[18].phase = shooter_actors[18].phase + 1;
}

/* arg1 walks the table (arg1++ after each call): it keeps the pointer walk and the 3-step counter apart, T-16563. */
void shooter_boss2_pose_all_arms(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 r;

    if (arg2 < 10) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 8, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 10 && arg2 < 20) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 9, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 20 && arg2 < 30) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 10, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 30 && arg2 < 80) {
        r = rand();
        shooter_fire_from_joint(0, 0x1C, (s16)((r & 0x3FF) - 0x200), (s16)((rand() & 0x3FF) - 0x200), 0x3E8, 0x1F4, 0x86);
    } else if (arg2 >= 80 && arg2 < 90) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 11, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 90 && arg2 < 100) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 12, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 100 && arg2 < 110) {
        for (i = 0; i < 7; i++) {
            shooter_apply_keyframe_tween(arg0 + i * 3, shooter_boss2_arm_poses[arg1], 13, 0, 0);
            arg1++;
        }
    }
}

void shooter_boss2_update_arm_attack_b(u8 *phase_counter, u8 *pose_mode, s32 arm_index, s32 pose_table_index, s32 frame_threshold) {
    s32 pad[15]; /* FAKE: unused slots above idx, the original frame is 64 bytes larger (T-3330 idiom, real source unknown). T-9370 */
    s32 idx;
    s32 gap[1]; /* FAKE: unused slot between idx and r, same idiom. T-9370 */
    s32 r;

    if ((u32)frame_threshold < *phase_counter) {
        shooter_boss2_pose_arm_b(arm_index, pose_table_index, *pose_mode);
        *pose_mode += 1;
        *phase_counter = 0;
        if (arm_index == 1) {
            idx = 0x10;
        } else if (arm_index == 2) {
            idx = 0x11;
        } else if (arm_index == 3) {
            idx = 0x12;
        }
        if ((shooter_actors + idx)->stats[1] != 0 && !(*pose_mode & 1)) {
            r = rand();
            shooter_fire_from_joint(0, arm_index, (s16)((r & 0x1FF) - 0x100), (s16)((rand() & 0x1FF) + 0xF4), 0x3E8, 0x190, 0x82);
        }
    }
    if (*pose_mode >= 0x14) {
        *phase_counter = *pose_mode = 0;
        shooter_boss2_pick_arm_attack(arm_index);
        shooter_boss2_rest_arm(arm_index, pose_table_index);
    } else {
        *phase_counter += 1;
    }
}

void shooter_boss2_pose_arm_b(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;

    if (arg2 < 0x0A) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            shooter_apply_keyframe_tween(j, shooter_boss2_arm_poses[arg1], 14, 0, 0);
        }
    } else if (arg2 >= 0x0A && arg2 < 0x14) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            shooter_apply_keyframe_tween(j, shooter_boss2_arm_poses[arg1], 15, 0, 0);
        }
    }
}

void shooter_boss2_update_arms(void) {
    u8 temp;

    temp = shooter_actors[18].stats[2];
    if (temp == 1) {
        shooter_boss2_update_arm_attack_a(&shooter_actors[18].phase, &shooter_actors[18].mode, 1, 0, 4);
    } else if (temp == 2) {
        shooter_boss2_update_arm_attack_a(&shooter_actors[18].phase, &shooter_actors[18].mode, 1, 0, 2);
    } else if (temp == 3) {
        shooter_boss2_update_arm_attack_c(&shooter_actors[18].phase, &shooter_actors[18].mode, 1, 0);
    }
    temp = shooter_actors[19].stats[2];
    if (temp == 1) {
        shooter_boss2_update_arm_attack_a(&shooter_actors[19].phase, &shooter_actors[19].mode, 2, 7, 3);
    } else if (temp == 2) {
        shooter_boss2_update_arm_attack_b(&shooter_actors[19].phase, &shooter_actors[19].mode, 2, 7, 4);
    } else if (temp == 3) {
        shooter_boss2_update_arm_attack_c(&shooter_actors[19].phase, &shooter_actors[19].mode, 2, 7);
    }
    temp = shooter_actors[20].stats[2];
    if (temp == 1) {
        shooter_boss2_update_arm_attack_a(&shooter_actors[20].phase, &shooter_actors[20].mode, 3, 14, 2);
    } else if (temp == 2) {
        shooter_boss2_update_arm_attack_b(&shooter_actors[20].phase, &shooter_actors[20].mode, 3, 14, 3);
    } else if (temp == 3) {
        shooter_boss2_update_arm_attack_c(&shooter_actors[20].phase, &shooter_actors[20].mode, 3, 14);
    }
    if (shooter_actors[17].stats[2] == 1 && shooter_actors[19].stats[2] == 0 && shooter_actors[18].stats[2] == 0 && shooter_actors[20].stats[2] == 0) {
        shooter_boss2_update_all_arms_attack(4);
    }
}

void shooter_boss2_pick_arm_attack(s32 arm_index) {
    s32 r;
    s32 m;

    r = rand();
    if (arm_index == 1) {
        if (shooter_actors[17].stats[2] != 0) {
            shooter_actors[18].stats[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            shooter_actors[18].stats[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            shooter_actors[18].stats[2] = 2;
            return;
        }
        if (m >= 0x46) {
            shooter_actors[18].stats[2] = 3;
        }
    } else if (arm_index == 2) {
        if (shooter_actors[17].stats[2] != 0) {
            shooter_actors[19].stats[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            shooter_actors[19].stats[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            shooter_actors[19].stats[2] = 2;
            return;
        }
        if (m >= 0x46) {
            shooter_actors[19].stats[2] = 3;
        }
    } else if (arm_index == 3) {
        if (shooter_actors[17].stats[2] != 0) {
            shooter_actors[20].stats[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            shooter_actors[20].stats[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            shooter_actors[20].stats[2] = 2;
            return;
        }
        if (m >= 0x46) {
            shooter_actors[20].stats[2] = 3;
        }
    }
}

void shooter_boss2_update_idle_sway(u32 sway_frame_interval) {
    if (sway_frame_interval < shooter_actors[23].phase) {
        shooter_boss3_sway_motion(shooter_actors[23].mode);
        shooter_actors[23].mode = shooter_actors[23].mode + 1;
        shooter_actors[23].phase = 0;
    }
    if (shooter_actors[23].mode >= 0x7CU) {
        shooter_actors[23].mode = 0;
        shooter_actors[23].phase = shooter_actors[23].mode;
        return;
    }
    shooter_actors[23].phase = shooter_actors[23].phase + 1;
}

void shooter_boss2_sway_motion_alt(s32 elapsed_frames) {
    if (elapsed_frames < 0xA) {
        shooter_move_model(0, 8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0xA) && (elapsed_frames < 0x14)) {
        shooter_move_model(0, 8, -1, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x14) && (elapsed_frames < 0x1E)) {
        shooter_move_model(0, 8, -2, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x20) && (elapsed_frames < 0x2A)) {
        shooter_move_model(0, -8, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x2A) && (elapsed_frames < 0x34)) {
        shooter_move_model(0, -8, 0xA, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x34) && (elapsed_frames < 0x48)) {
        shooter_move_model(0, -8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x48) && (elapsed_frames < 0x52)) {
        shooter_move_model(0, -8, -1, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x52) && (elapsed_frames < 0x5C)) {
        shooter_move_model(0, -8, -2, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x5E) && (elapsed_frames < 0x68)) {
        shooter_move_model(0, 8, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x68) && (elapsed_frames < 0x72)) {
        shooter_move_model(0, 8, 0xA, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((elapsed_frames >= 0x72) && (elapsed_frames < 0x7C)) {
        shooter_move_model(0, 8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
    }
}
