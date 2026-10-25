/* enemy_pool_and_keyframes.c: src/ovl/TACO/8015ABF0.c:5 resets enemy/model coordinate pools; :27 allocates/spawns; :113 and :137 set poses; :185 updates active records; :260 and :307 animate keyframes/poses; :359 and :465 remove/handle death; :542 and :555 count availability and active enemy kinds. Original object 0x8015ABF0-0x8015D410 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"


void shooter_reset_enemies(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        shooter_coordinate_owner[i] = 0;
        D_80127080[i].attribute = 0x80000000;
    }
    for (i = 16; i != 63; i++) {
        shooter_release_actor_model(i);
        shooter_actors[i].model.attribute = 0x80000000;
        shooter_actors[i].stats[2] = 1;
    }
    shooter_script_timer = 0;
    shooter_script_phase = 0;
    shooter_script_sub_timer = 0;
    shooter_script_counter = 0;
    shooter_circler_pairing = 0;
}

/* The two table walks are index loops over shooter_coordinate_owner (uopt shares one strength-reduced pointer between them, which
 * is why n, i and that pointer get s2, s0 and s1); the count loop compares the element address against the table
 * end. HYPOTHESIS: base is a model data address (+4, then +8 more for the link call). T-19026 */
s32 shooter_spawn_enemy(s32 a, s32 b, ShooterPosition c, ShooterPosition d, ShooterPosition e, s32 f, s32 g) {
    s32 i;
    s32 base;
    s32 id;
    u32 n;
    s32 *pair;

    id = 0;
    n = 0;
    for (i = 16; i < 63; i++) {
        if (!((i + shooter_actors)->flag_bytes[1] & 0x80)) {
            id = i;
            break;
        }
    }
    if (id == 0) {
        return -1;
    }
    for (i = 0; &shooter_coordinate_owner[i] < &shooter_coordinate_owner[0x40]; i++) {
        if (shooter_coordinate_owner[i] == 0) {
            n++;
        }
    }
    if (n < shooter_enemy_joint_count[a]) {
        return -1;
    }
    base = shooter_enemy_model_base[a];
    base += 4;
    func_8009B3C0(base);
    base += 8;
    shooter_release_actor_model(id);
    /* FAKE: the empty test adds one basic block to the live range of a, which lowers its priority below base's, so base
     * gets $fp and a stays in its stack home as in the original; no code. T-19026 */
    if (b) {}
    func_80099E30(0, &(id + shooter_actors)->coordinate);
    if (a == 0xA) {
        func_8009B430(base, (u8 *)&(id + shooter_actors)->model, 1);
    } else {
        func_8009B430(base, (u8 *)&(id + shooter_actors)->model, 0);
    }
    n = shooter_enemy_joint_count[a];
    for (i = 0; n != 0; i++) {
        if (shooter_coordinate_owner[i] == 0) {
            func_80099E30((s32)&(id + shooter_actors)->coordinate, &D_80127480[i]);
            shooter_coordinate_owner[i] = id;
            if (a == 0xF || a == 0x10) {
                pair = (shooter_enemy_joint_count[a] - n) * 2 + shooter_enemy_joint_poses[a];
                shooter_set_pose_from_table(i, pair[0], pair[1], 0);
            } else {
                shooter_apply_initial_pose(i, shooter_enemy_joint_poses[a][shooter_enemy_joint_count[a] - n]);
            }
            func_8009B430(base, (u8 *)&D_80127080[i], shooter_enemy_joint_count[a] - n);
            n--;
            D_80127080[i].links[0] = (s32)&D_80127480[i];
            D_80127080[i].attribute = 0x80000000;
        }
    }
    (shooter_actors + id)->model.attribute = 0x80000000;
    (shooter_actors + id)->mode = b;
    (shooter_actors + id)->counter = 0x7FFF;
    (shooter_actors + id)->stats[2] = a;
    (shooter_actors + id)->phase = 1;
    (shooter_actors + id)->auxiliary = 0;
    (shooter_actors + id)->weapon = 0;
    (shooter_actors + id)->argument_x = c.x;
    (shooter_actors + id)->argument_y = c.y;
    (shooter_actors + id)->argument_z = c.z;
    (shooter_actors + id)->flag_bytes[1] = 0x80;
    (shooter_actors + id)->x = d.x;
    (shooter_actors + id)->y = d.y;
    (shooter_actors + id)->z = d.z;
    (shooter_actors + id)->rotation_x = e.x;
    (shooter_actors + id)->rotation_y = e.y;
    (shooter_actors + id)->rotation_z = e.z;
    (shooter_actors + id)->param = e.w;
    (shooter_actors + id)->scale_x = shooter_enemy_scale[a];
    (shooter_actors + id)->scale_y = shooter_enemy_scale[a];
    (shooter_actors + id)->scale_z = shooter_enemy_scale[a];
    (shooter_actors + id)->stats[1] = f;
    (shooter_actors + id)->stats[0] = shooter_enemy_radius[a];
    (shooter_actors + id)->stats[3] = g | 1;
    (id + shooter_actors)->model.links[0] = (s32)&(id + shooter_actors)->coordinate;
    return id;
}

/* Declare i, j before matrix so matrix sits at the original stack offset (T-16563). */
void shooter_set_pose_from_table(object_index, translation_table, rotation_table, frame_index)
    s32 object_index;
    s16 *translation_table;
    s16 *rotation_table;
    s32 frame_index;
{
    s32 i;
    s32 j;
    MATRIX matrix;
    ShooterCoordinateView *obj;

    rotation_table += frame_index * 9;
    translation_table += frame_index * 3;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            matrix.m[i][j] = *rotation_table++;
        }
        matrix.t[i] = *translation_table++;
    }
    obj = (ShooterCoordinateView *)&D_80127480[object_index];
    obj->local = matrix;
    obj->flag = 0;
}

void shooter_apply_initial_pose(s32 object_index, s16 *pose_record) {
    s32 pad[7]; /* FAKE: unused local reproduces the original frame (0x60) and spill offsets; real source unknown. T-6070 */
    ShooterCoordinate *o;
    ShooterPosition *pos;
    u8 *q;

    o = &D_80127480[object_index];
    o->x += pose_record[0];
    o->y += pose_record[1];
    o->z += pose_record[2];
    pos = &D_80128880[object_index];
    pos->x = pose_record[3];
    pos->z = pose_record[4];
    q = (u8 *)o->rotation;
    pos->y = pose_record[5];
    shooter_rotation_matrix_x((pos->x << 12) / 360, q);
    shooter_rotation_matrix_z((pos->z << 12) / 360, q);
    shooter_rotation_matrix_y((pos->y << 12) / 360, q);
    *(s32 *)o = 0;
}

void shooter_apply_pose_key(s32 object_index, s16 *pose_frames, s32 frame_index) {
    /* The pointer precedes the payload to preserve its local layout. T-9520. */
    ShooterCoordinateView *obj;
    MATRIX matrix;
    ShooterPosition *pos;

    pose_frames += frame_index * 6;
    matrix = *(MATRIX *)view_base_matrix;
    obj = (ShooterCoordinateView *)&D_80127480[object_index];
    matrix.t[0] = obj->local.t[0] + pose_frames[0];
    matrix.t[1] = obj->local.t[1] + pose_frames[1];
    matrix.t[2] = obj->local.t[2] + pose_frames[2];
    pos = &D_80128880[object_index];
    pos->x += pose_frames[3];
    pose_frames += 4;
    shooter_rotation_matrix_x((pos->x << 12) / 360, &matrix);
    pos->z += *pose_frames;
    pose_frames++;
    shooter_rotation_matrix_z((pos->z << 12) / 360, &matrix);
    pos->y += *pose_frames;
    shooter_rotation_matrix_y((pos->y << 12) / 360, &matrix);
    obj->local = matrix;
    obj->flag = 0;
}

/* FAKE: byte-offset actor views preserve the original alias/reload behavior;
 * direct ShooterActor fields differ by 30 listing rows. T-12070. */
void shooter_update_enemies(void) {
    s32 i;
    ShooterPosition position;
    for (i = 0x10; i != 0x3F; i++) {
        if (*(u8 *)((u8 *)&shooter_actors[i] + 1) & 0x80) {
            if (*(s16 *)((u8 *)&shooter_actors[i] + 122) == 0x7FFF) {
                shooter_set_enemy_visibility(1, i);
                *(s16 *)((u8 *)&shooter_actors[i] + 122) = 0;
            }
            if ((*(u8 *)((u8 *)&shooter_actors[i] + 133) == 0) && (*(u8 *)((u8 *)&shooter_actors[i] + 2) != 0xFF)) {
                *(s16 *)((u8 *)&shooter_actors[i] + 122) = 0;
                *(u8 *)((u8 *)&shooter_actors[i] + 3) = 2;
                *(u8 *)((u8 *)&shooter_actors[i] + 2) = 0xFF;
                position.x = *(s16 *)((u8 *)&shooter_actors[i] + 116);
                position.y = *(s16 *)((u8 *)&shooter_actors[i] + 118);
                position.z = *(s16 *)((u8 *)&shooter_actors[i] + 120);
                switch (*(u8 *)((u8 *)&shooter_actors[i] + 135) & 0xF0) {
                case 0x10:
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    break;
                case 0x20:
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_VULCAN);
                    break;
                case 0x30:
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_WIDE);
                    break;
                case 0x40:
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_BEAM);
                    break;
                case 0x50:
                    shooter_spawn_item(position, SHOOTER_ITEM_SECONDARY_TOGGLE);
                    break;
                case 0x60:
                    shooter_spawn_item(position, SHOOTER_ITEM_BOMB_PULSE);
                    break;
                case 0x70:
                    shooter_spawn_item(position, SHOOTER_ITEM_BOMB_BLAST);
                    break;
                case 0x80:
                    shooter_spawn_item(position, SHOOTER_ITEM_HEAL);
                    break;
                case 0xD0:
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    break;
                case 0xE0:
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_WIDE);
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_BEAM);
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_VULCAN);
                    break;
                case 0xF0:
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_BONUS);
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_VULCAN);
                    shooter_spawn_item(position, SHOOTER_ITEM_SECONDARY_TOGGLE);
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_WIDE);
                    shooter_spawn_item(position, SHOOTER_ITEM_WEAPON_BEAM);
                    shooter_spawn_item(position, SHOOTER_ITEM_HEAL);
                    shooter_spawn_item(position, SHOOTER_ITEM_BOMB_PULSE);
                    shooter_spawn_item(position, SHOOTER_ITEM_BOMB_BLAST);
                    break;
                }
            }
            shooter_run_enemy_behavior((s32) *(u8 *)((u8 *)&shooter_actors[i] + 2), i);
            shooter_animate_enemy(i);
            *(s16 *)((u8 *)&shooter_actors[i] + 122) = (s16) (*(s16 *)((u8 *)&shooter_actors[i] + 122) + 1);
        }
    }
}

/* FAKE: the actor is addressed through a byte-offset view in the loop: one pointer then serves the field loads and
 * &unk14, the array form builds a second one. Real source unknown. T-16563 */
void shooter_animate_enemy_keys(s32 actor_index, s16 *keyframe_script) {
    s32 i;
    s32 pose_frame_index;
    s16 frame;
    s32 phase;
    s32 j;
    s32 n;
    s32 last;
    u8 *id;

    frame = shooter_actors[actor_index].auxiliary;
    if (frame >= keyframe_script[3]) {
        shooter_actors[actor_index].auxiliary = 0;
        frame = shooter_actors[actor_index].auxiliary;
    }
    phase = frame % keyframe_script[2];
    n = 0;
    for (i = 0;; i++) {
        if (phase < keyframe_script[4 + i * 2]) {
            if (i) {} /* FAKE: the empty test splits the live range of i at the found key, so i and j * 4 get s1 and s0 as in the original. T-16563 */
            pose_frame_index = keyframe_script[5 + i * 2];
            break;
        }
    }
    id = shooter_coordinate_owner;
    j = keyframe_script[0];
    last = keyframe_script[1];
    for (i = 0; i != 0x40; i++, id++) {
        if (actor_index == *id) {
            if (n >= j) {
                if (((ShooterActor *)((u8 *)shooter_actors + actor_index * 0x88))->auxiliary % keyframe_script[2] == 0) {
                    func_80099E30((s32)&((ShooterActor *)((u8 *)shooter_actors + actor_index * 0x88))->coordinate, (i + D_80127480));
                    shooter_apply_initial_pose(i, shooter_enemy_joint_poses[shooter_actors[actor_index].stats[2]][j]);
                } else {
                    shooter_apply_pose_key(i, shooter_enemy_joint_poses[((ShooterActor *)((u8 *)shooter_actors + actor_index * 0x88))->stats[2]][j], pose_frame_index);
                }
                j++;
            }
            n++;
        }
        if (last < j) {
            break;
        }
    }
}

/* No local actor pointer (see shooter_set_enemy_visibility); `n = 0` after the first test lands in its branch delay slot as in the original. T-16563 */
void shooter_animate_enemy_poses(s32 actor_index) {
    u32 n;
    s32 i;
    s32 *rec;

    if ((actor_index + shooter_actors)->auxiliary >= 0xF) {
        (actor_index + shooter_actors)->auxiliary = 0;
    }
    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (actor_index == shooter_coordinate_owner[i]) {
            rec = shooter_enemy_joint_poses[(actor_index + shooter_actors)->stats[2]] + n * 2;
            shooter_set_pose_from_table(i, rec[0], rec[1], (actor_index + shooter_actors)->auxiliary);
            n++;
        }
        if (n >= shooter_enemy_joint_count[(actor_index + shooter_actors)->stats[2]]) {
            break;
        }
    }
}

/* The actor is addressed as (arg1 + shooter_actors) in every use, no local: uopt then keeps it in $v0 and the index multiply comes first (T-16563). */
void shooter_set_enemy_visibility(s32 visibility_mode, s32 actor_index) {
    s32 i;

    if ((actor_index + shooter_actors)->flag_bytes[1] & 0x80) {
        if (visibility_mode == 1) {
            if ((actor_index + shooter_actors)->stats[2] < 0xB) {
                (actor_index + shooter_actors)->model.attribute = 0;
                return;
            }
            (actor_index + shooter_actors)->model.attribute = 0x80000000;
            for (i = 0; i < 0x40; i++) {
                if (actor_index == shooter_coordinate_owner[i]) {
                    D_80127080[i].attribute = 0;
                }
            }
            return;
        }
        if ((actor_index + shooter_actors)->stats[2] < 9) {
            (actor_index + shooter_actors)->model.attribute = 0x80000000;
            return;
        }
        (actor_index + shooter_actors)->model.attribute = 0x80000000;
        for (i = 0; i < 0x40; i++) {
            if (actor_index == shooter_coordinate_owner[i]) {
                D_80127080[i].attribute = 0x80000000;
            }
        }
    }
}

void shooter_remove_enemy(s32 actor_index) {
    s32 i;

    shooter_set_enemy_visibility(0, actor_index);
    shooter_actors[actor_index].flag_bytes[1] = 0;
    shooter_actors[actor_index].stats[2] = 0;
    for (i = 0; i < 0x40; i++) {
        if (actor_index == shooter_coordinate_owner[i]) {
            shooter_coordinate_owner[i] = 0;
        }
    }
}

/* FAKE: byte-offset views preserve actor reloads; explicit case 0 preserves
 * the original table range. Keep the case-13 call/load on one source line
 * for the original load scheduling. T-12070. */
void shooter_animate_enemy(s32 actor_index) {
    u8 enemy_phase;
    if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 3)) != 0) {
        switch (*(u8 *)((u8 *)&shooter_actors[actor_index] + 134)) {
        case 11:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_a);
                shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_b);
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) += 1;
            }
        case 0:
        default:
block_28:
            break;
        case 12:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 3)) == 1) {
                    shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_c);
                } else {
                    shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_c);
                }
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) = (s16) ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) + 1);
                goto block_28;
            }
            break;
        case 13:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                enemy_phase = (*(u8 *)((u8 *)&shooter_actors[actor_index] + 3));
                if (enemy_phase == 1) {
                    shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_d);                     enemy_phase = (*(u8 *)((u8 *)&shooter_actors[actor_index] + 3));
                }
                if (enemy_phase == 2) {
                    shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_d);                 }
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) += 1;
                goto block_28;
            }
            break;
        case 14:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 3)) == 1) {
                    shooter_animate_enemy_keys(actor_index, shooter_enemy_keys_e);
                }
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) += 1;
                goto block_28;
            }
            break;
        case 15:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 3)) == 1) {
                    shooter_animate_enemy_poses(actor_index);
                }
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) += 1;
                goto block_28;
            }
            break;
        case 16:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) += 1;
            if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) >= 6) {
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 114)) = 0;
                if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 3)) == 1) {
                    shooter_animate_enemy_poses(actor_index);
                }
                (*(s16 *)((u8 *)&shooter_actors[actor_index] + 130)) += 1;
                goto block_28;
            }
            break;
        }
    }
}

void shooter_zero_enemy_hit_points(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        shooter_actors[i].stats[1] = 0;
    }
}

/* FAKE: byte-offset actor views retain the original reload/alias behavior.
 * The switch intentionally reads the selector as u16. T-12070. */
void shooter_behavior_dying(s32 actor_index) {
    ShooterPosition position;
    s16 counter;

    if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 122)) == 0) {
        if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 134)) != 6) {
            srand(shooter_script_timer + actor_index);
            (*(u8 *)((u8 *)&shooter_actors[actor_index] + 1)) = 0x90;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 124)) = (rand() % 40) - 0x14;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 126)) = (rand() % 10) + 0xA;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 128)) = (rand() % 10) + 0xA;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 106)) = (s16) (rand() % 6);
            shooter_spawn_explosion(*(ShooterPosition *)&(*(s16 *)((u8 *)&shooter_actors[actor_index] + 116)), *(ShooterPosition *)&(*(s16 *)((u8 *)&shooter_actors[actor_index] + 124)), 0);

        } else {
            srand(shooter_script_timer + actor_index);
            (*(u8 *)((u8 *)&shooter_actors[actor_index] + 1)) = 0x90;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 124)) = (rand() % 10) - 5;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 126)) = (rand() % 2) + 2;
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 128)) = (rand() % 2) + 2;
            shooter_spawn_explosion(*(ShooterPosition *)&(*(s16 *)((u8 *)&shooter_actors[actor_index] + 116)), *(ShooterPosition *)&(*(s16 *)((u8 *)&shooter_actors[actor_index] + 124)), 0);

        }
    }
    if ((*(u8 *)((u8 *)&shooter_actors[actor_index] + 134)) != 6) {
        switch (*(u16 *)((u8 *)&shooter_actors[actor_index] + 106)) {
        case 0:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 100)) += 0x10;

            break;
        case 1:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 102)) += 0x10;

            break;
        case 2:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 104)) += 0x10;

            break;
        case 3:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 100)) -= 0x10;

            break;
        case 4:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 102)) -= 0x10;

            break;
        case 5:
            (*(s16 *)((u8 *)&shooter_actors[actor_index] + 104)) -= 0x10;

            break;
        }
        counter = (*(s16 *)((u8 *)&shooter_actors[actor_index] + 122));
    } else {
        if (((s16) (*(s16 *)((u8 *)&shooter_actors[actor_index] + 122)) % 3) == 0) {
            srand(shooter_script_timer + actor_index);
            position.x = ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 116)) + (rand() % 500)) - 0xFA;

            position.y = (*(s16 *)((u8 *)&shooter_actors[actor_index] + 118)) + (rand() % 250);
            position.z = (*(s16 *)((u8 *)&shooter_actors[actor_index] + 120));
            shooter_spawn_explosion(position, *(ShooterPosition *)&(*(s16 *)((u8 *)&shooter_actors[actor_index] + 124)), 0);

        }
        (*(s16 *)((u8 *)&shooter_actors[actor_index] + 100)) += 2;

        (*(s16 *)((u8 *)&shooter_actors[actor_index] + 104)) += 2;
        counter = (*(s16 *)((u8 *)&shooter_actors[actor_index] + 122));
    }
    if (!(counter & 1)) {
        shooter_set_enemy_visibility(1, actor_index);
    } else {
        shooter_set_enemy_visibility(0, actor_index);
    }
    if ((*(s16 *)((u8 *)&shooter_actors[actor_index] + 122)) >= 0x3D) {
        shooter_remove_enemy(actor_index);
    }
}

s32 shooter_count_free_coordinates(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (shooter_coordinate_owner[i] == 0) {
            n += 1;
        }
    }
    return n;
}

s32 shooter_count_enemies_of_kind(actor_kind)
s32 actor_kind;
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 16; i != 63; i++) {
        if (((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[1] != 0) {
            if (!(((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->flag_bytes[1] & 0x80)) {
                ((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[1] = 0;
            }
        }
        if (actor_kind != 0xFF) {
            if (actor_kind == ((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[2] && ((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[1] != 0) {
                n++;
            }
        } else if (((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[1] != 0) {
            n++;
        }
    }
    return n;
}

/* Index first, as in shooter_set_enemy_visibility: the global then gets the later colour. T-16563 */
s32 shooter_count_active_enemies(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0x10; i != 0x3F; i++) {
        if ((i + shooter_actors)->flag_bytes[1] & 0x80) {
            n++;
        }
    }
    return n;
}
