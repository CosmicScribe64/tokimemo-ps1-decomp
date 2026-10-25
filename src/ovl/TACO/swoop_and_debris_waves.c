/* swoop_and_debris_waves.c: src/ovl/TACO/80154960.c:4: timed stage script, debris, swooping and spiral waves, sweep turret and homing mine behaviors; body 9: shooter_script_timer += 1; 10: shooter_update_enemies();. Original object 0x80154960-0x80156A80 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_update_stage1_script(void) {
    /* FAKE: reserves the word above poly's spill slot (poly sits at 0x30 in the original); real local unknown. T-16564 */
    u8 pad[4];
    POLY_F4 *poly;

    shooter_script_timer += 1;
    shooter_update_enemies();
    switch (shooter_script_phase) {
    case 0:
        if (shooter_script_timer < 0x834) {
            if ((shooter_script_timer % 7) == 0) {
                shooter_spawn_debris();
            }
        }
        if (shooter_script_timer < 0x4B0) {
            if ((shooter_script_timer % 50) == 0) {
                shooter_spawn_homing_mines(0);
            }
        }
        if (shooter_script_timer >= 0x259) {
            if ((shooter_script_timer < 0x708) && (shooter_count_enemies_of_kind(4) == 0)) {
                shooter_spawn_swoop_wave();
            }
        }
        if (shooter_script_timer >= 0x709) {
            shooter_next_script_phase();
        }
        break;
    case 1:
        shooter_stage1_mid_boss_slot = shooter_spawn_stage1_mid_boss();
        shooter_next_script_phase();
        break;
    case 2:
        if (shooter_script_timer < 0x258) {
            if ((shooter_script_timer % 7) == 0) {
                shooter_spawn_debris();
            }
        }
        if ((shooter_script_timer < 0x12C) && (shooter_count_enemies_of_kind(4) == 0)) {
            shooter_spawn_swoop_wave();
        }
        if (shooter_script_timer >= 0x1E1) {
            if ((shooter_script_timer % 110) == 0) {
                shooter_spawn_homing_mines(1);
            }
        }
        if (shooter_script_timer >= 0x12D) {
            if ((shooter_script_timer % 120) == 0) {
                shooter_spawn_spiral_pair();
            }
        }
        if (shooter_count_enemies_of_kind(6) == 0) {
            shooter_next_script_phase();
        }
        if ((shooter_stage1_mid_boss_slot + shooter_actors)->phase == 2) {
            shooter_next_script_phase();
        }
        break;
    case 3:
        shooter_zero_enemy_hit_points();
        shooter_next_script_phase();
        break;
    case 4:
        poly = GetWorkBase(0x18, draw_buffer);
        func_8009F02C(poly);
        SetSemiTrans(poly, 1);
        poly->r0 = shooter_script_timer;
        poly->g0 = shooter_script_timer;
        poly->b0 = shooter_script_timer;
        poly->x0 = game_state.screen_center_x[draw_buffer] - 0x100;
        poly->y0 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly->x1 = game_state.screen_center_x[draw_buffer] + 0x100;
        poly->y1 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly->x2 = game_state.screen_center_x[draw_buffer] - 0x100;
        poly->y2 = game_state.screen_center_y[draw_buffer] + 0x78;
        poly->x3 = game_state.screen_center_x[draw_buffer] + 0x100;
        poly->y3 = game_state.screen_center_y[draw_buffer] + 0x78;
        AddPrim((draw_buffer << 0xA) + D_800E8CA0, poly);
        dtd_on_tpage(0, 0, 0, 1, 0);
        shooter_script_timer += 2;
        if (shooter_script_timer >= 0x80) {
            shooter_next_script_phase();
        }
        break;
    case 5:
        poly = GetWorkBase(0x18, draw_buffer);
        func_8009F02C(poly);
        SetSemiTrans(poly, 1);
        poly->r0 = 0x80 - shooter_script_timer;
        poly->g0 = 0x80 - shooter_script_timer;
        poly->b0 = 0x80 - shooter_script_timer;
        poly->x0 = game_state.screen_center_x[draw_buffer] - 0x100;
        poly->y0 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly->x1 = game_state.screen_center_x[draw_buffer] + 0x100;
        poly->y1 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly->x2 = game_state.screen_center_x[draw_buffer] - 0x100;
        poly->y2 = game_state.screen_center_y[draw_buffer] + 0x78;
        poly->x3 = game_state.screen_center_x[draw_buffer] + 0x100;
        poly->y3 = game_state.screen_center_y[draw_buffer] + 0x78;
        AddPrim((draw_buffer << 0xA) + D_800E8CA0, poly);
        dtd_on_tpage(0, 0, 0, 1, 0);
        shooter_script_timer += 3;
        if (shooter_script_timer >= 0x80) {
            shooter_reset_enemies();
            mode_step_next();
        }
        break;
    }
}

void shooter_spawn_debris(void) {
    /* FAKE: unused leading ShooterPosition local reproduces the frame (see shooter_spawn_patroller); real locals unknown. T-9180 */
    ShooterPosition p0;
    ShooterPosition pos;
    ShooterPosition vel;
    s32 id;
    s32 r;

    srand(shooter_script_timer + 0x1E240);
    r = rand();
    pos.x = (rand() % 5000 + shooter_actors->x + r % 3000) - 0xFA0;
    r = rand();
    pos.y = (rand() % 2500 + shooter_actors->y + r % 1500) - 0x7D0;
    pos.z = -0x8000;
    vel.x = rand() % 4096;
    vel.y = rand() % 4096;
    vel.z = rand() % 4096;
    vel.w = rand() % 7;
    vel.x = rand() % 3;
    if (rand() % 3 == 0) {
        id = shooter_spawn_enemy(0, 1, vel, pos, vel, vel.x * 4 + 4, 0x10);
    } else {
        id = shooter_spawn_enemy(0, 1, vel, pos, vel, vel.x * 4 + 4, 0);
    }
    shooter_actors[id].stats[0] = vel.x + 3;
}

void shooter_behavior_debris(s32 arg0) {
    s16 t;

    (arg0 + shooter_actors)->z += 0x100;
    t = ((arg0 + shooter_actors)->argument_x << 9) + 0x200;
    (arg0 + shooter_actors)->scale_z = t;
    shooter_actors[arg0].scale_y = t;
    shooter_actors[arg0].scale_x = t;
    switch ((arg0 + shooter_actors)->param) {
    case 0:
        (arg0 + shooter_actors)->rotation_x += 0x32;
        break;
    case 1:
        (arg0 + shooter_actors)->rotation_y += 0x32;
        break;
    case 2:
        (arg0 + shooter_actors)->rotation_z += 0x32;
        break;
    case 3:
        (arg0 + shooter_actors)->rotation_x += 0x63;
        break;
    case 4:
        (arg0 + shooter_actors)->rotation_y += 0x63;
        break;
    case 5:
        (arg0 + shooter_actors)->rotation_z += 0x63;
        break;
    }
    if ((arg0 + shooter_actors)->z >= 0x1001) {
        shooter_remove_enemy(arg0);
    }
}

void shooter_spawn_swoop_wave(void) {
    ShooterPosition sp50;
    /* FAKE: unused ShooterPosition between the two reproduces the original frame (see shooter_spawn_patroller); real local unknown. T-16564 */
    ShooterPosition sp48;
    ShooterPosition sp40;

    srand(shooter_script_timer);
    sp50.x = shooter_actors->x;
    sp50.y = (shooter_actors->x > 0) ? 1 : -1;
    sp50.z = -0x2BC;
    sp40.w = 0;
    if ((shooter_script_counter % 3) == 0) {
        shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0xE0);
    } else {
        shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0x50);
    }
    /* FAKE: both stores on one line so as1 orders them z, unk6 like the original (T-11690 idiom); real source unknown. T-16564 */
    sp40.w = 0x14; sp50.z = -0x384;
    shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0);
    sp40.w = 0x14; sp50.z = -0x1F4;
    shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0);
    sp40.w = 0x28; sp50.z = -0x44C;
    shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0);
    sp40.w = 0x28; sp50.z = -0x12C;
    shooter_spawn_enemy(4, 2, sp50, shooter_zero_position_a, sp40, 6, 0);
    shooter_script_counter += 1;
}

/* FAKE: the operand order of the actor address (idx + base or base + idx) differs between the statements of the original; the mixed spellings reproduce it, real source unknown. T-16564 */
void shooter_behavior_swooper(s32 arg0) {
    s32 t;

    t = ((arg0 + shooter_actors)->counter - (arg0 + shooter_actors)->param) * 7;
    (shooter_actors + arg0)->z = (rsin(t) * -0x3A98) >> 0xC;
    (shooter_actors + arg0)->x = (((func_800A0140(t) << 0xB) >> 0xC) * (shooter_actors + arg0)->argument_y) + (shooter_actors + arg0)->argument_x;
    (arg0 + shooter_actors)->y = (arg0 + shooter_actors)->argument_z;
    (shooter_actors + arg0)->rotation_x = 0;
    if ((arg0 + shooter_actors)->argument_y == 1) {
        (arg0 + shooter_actors)->rotation_y = t;
        (shooter_actors + arg0)->rotation_z = 0xC00;
    } else {
        (arg0 + shooter_actors)->rotation_y = 0x1000 - t;
        (shooter_actors + arg0)->rotation_z = 0x400;
    }
    if (t >= 0xA01) {
        shooter_remove_enemy(arg0);
    }
}

void shooter_spawn_spiral_pair(void) {
    /* FAKE: unused ShooterPosition local reproduces the frame and the offsets of pos and vel (see shooter_spawn_patroller); real locals unknown. T-9180 */
    ShooterPosition pos;
    ShooterPosition p1;
    ShooterPosition vel;
    s32 id;
    s32 r;

    srand(shooter_script_timer);
    r = rand();
    pos.x = (rand() % 250 + shooter_actors->x + r % 150) - 0xC8;
    r = rand();
    pos.y = (rand() % 135 + shooter_actors->y + r % 75) - 0x64;
    pos.z = -0x1D4C;
    vel.y = 0x800;
    vel.z = 0;
    vel.x = 0;
    if (shooter_script_counter % 3 == 0) {
        id = shooter_spawn_enemy(5, 3, pos, shooter_zero_position_a, vel, 4, 0x80);
    } else {
        id = shooter_spawn_enemy(5, 3, pos, shooter_zero_position_a, vel, 4, 0x10);
    }
    if (id != -1) {
        shooter_actors[id].stats[0] = 0;
    }
    id = shooter_spawn_enemy(5, 4, pos, shooter_zero_position_a, vel, 2, 0);
    if (id != -1) {
        shooter_actors[id].stats[0] = 0;
    }
    shooter_script_counter += 1;
}

void shooter_behavior_spiral_in(s32 arg0) {
    ShooterActor *actor;
    s32 trig;

    /* FAKE: byte-stride pointer calculations and xor-zero keep the original
     * addition and multiplication operand order. T-10010. */
    trig = rsin((arg0 + shooter_actors)->counter * 0x3C);
    actor = (ShooterActor *)((u8 *)shooter_actors + arg0 * sizeof(ShooterActor));
    actor->x = ((s32) ((trig ^ 0) * (0x3E8 - (actor->counter * 0xA))) >> 0xD) + actor->argument_x;
    trig = func_800A0140((arg0 + shooter_actors)->counter * 0x3C);
    actor = (ShooterActor *)((u8 *)shooter_actors + arg0 * sizeof(ShooterActor));
    actor->y = ((s32) ((trig ^ 0) * (0x3E8 - (actor->counter * 0xA))) >> 0xD) + actor->argument_y;
    actor = arg0 + shooter_actors;
    actor->z = actor->argument_z;
    if (!((arg0 + shooter_actors)->counter & 1)) {
        shooter_set_enemy_visibility(0, arg0);
    } else {
        shooter_set_enemy_visibility(1, arg0);
    }
    actor = arg0 + shooter_actors;
    if (actor->counter >= 0x65) {
        actor->stats[0] = shooter_enemy_radius_5;
        shooter_actors[arg0].mode = 5;
        shooter_actors[arg0].counter = 0;
        shooter_actors[arg0].argument_y = 0;
    }
}

void shooter_behavior_spiral_out(s32 arg0) {
    (arg0 + shooter_actors)->x = ((rsin((arg0 + shooter_actors)->counter * 0x3C + 0x800) * (0x3E8 - (shooter_actors + arg0)->counter * 0xA)) >> 0xD) + (shooter_actors + arg0)->argument_x;
    (arg0 + shooter_actors)->y = ((func_800A0140((arg0 + shooter_actors)->counter * 0x3C + 0x800) * (0x3E8 - (shooter_actors + arg0)->counter * 0xA)) >> 0xD) + (shooter_actors + arg0)->argument_y;
    (arg0 + shooter_actors)->z = (arg0 + shooter_actors)->argument_z;
    if ((arg0 + shooter_actors)->counter >= 0x65) {
        shooter_remove_enemy(arg0);
    }
    if (((arg0 + shooter_actors)->counter & 1) == 0) {
        shooter_set_enemy_visibility(1, arg0);
    } else {
        shooter_set_enemy_visibility(0, arg0);
    }
}

void shooter_behavior_turret_sweep(s32 arg0) {
    ShooterPosition pos;

    switch ((arg0 + shooter_actors)->argument_y) {
    case 0:
        if (((arg0 + shooter_actors)->counter % 7) == 0) {
            pos.x = (arg0 + shooter_actors)->x;
            pos.y = (arg0 + shooter_actors)->y;
            pos.z = (arg0 + shooter_actors)->z;
            shooter_spawn_enemy_bullet(pos, 0x100, 0xC0);
        }
        if ((arg0 + shooter_actors)->counter == 0x3C) {
            if (shooter_actors->x < (arg0 + shooter_actors)->argument_x) {
                (arg0 + shooter_actors)->argument_x = 1;
            } else {
                (arg0 + shooter_actors)->argument_x = -1;
            }
            shooter_actors[arg0].counter = 0;
            (arg0 + shooter_actors)->argument_y += 1;
        }
        break;
    case 1:
        (arg0 + shooter_actors)->rotation_y += (arg0 + shooter_actors)->argument_x * 0x32;
        if ((arg0 + shooter_actors)->counter >= 0x29) {
            (arg0 + shooter_actors)->argument_y += 1;
        }
        break;
    case 2:
        (arg0 + shooter_actors)->z -= 0x320;
        if ((arg0 + shooter_actors)->z < -0x7530) {
            shooter_remove_enemy(arg0);
        }
        break;
    }
}

/* No return value on purpose: the caller reads v0, the index shooter_spawn_enemy returns (implicit-int style, see shooter_help_step_init). T-16564 */
s32 shooter_spawn_stage1_mid_boss(void) {
    ShooterPosition p0;
    ShooterPosition p2;
    ShooterPosition pos;

    srand(shooter_script_timer);
    /* FAKE: x and y on one line so as1 orders their stores y, x like the original (decomp-permuter); real source unknown. T-9180 */
    pos.x = -0x100; pos.y = 0x800;
    pos.z = 0;
    shooter_spawn_enemy(6, 6, shooter_zero_position_a, shooter_zero_position_a, pos, 0xFF, 0xF0);
}

s32 shooter_behavior_stage1_mid_boss(s32 arg0) {
    /* FAKE: mixed pointer-add spelling and byte-stride view retain the target's addu operand order. T-9750. */
    s32 state;
    s16 value;

    state = (shooter_actors + arg0)->argument_x;
    switch (state) {
    case 0:
        (shooter_actors + arg0)->z = -0x8000;
        (shooter_actors + arg0)->x = 0;
        (shooter_actors + arg0)->y = -0x1F4;
        (shooter_actors + arg0)->scale_z = 1;
        value = ((ShooterActor *)((u8 *)shooter_actors + arg0 * 0x88))->scale_z;
        (shooter_actors + arg0)->scale_y = value;
        (shooter_actors + arg0)->scale_x = value;
        (shooter_actors + arg0)->stats[0] = 0;
        (arg0 + shooter_actors)->argument_x++;
        break;
    case 1:
        value = (shooter_actors + arg0)->counter * 0x14;
        (shooter_actors + arg0)->scale_z = value;
        (shooter_actors + arg0)->scale_y = value;
        (shooter_actors + arg0)->scale_x = value;
        if ((shooter_actors + arg0)->counter >= 0x1F5) {
            (shooter_actors + arg0)->argument_x++;
        }
        break;
    case 2:
        (shooter_actors + arg0)->z += 0x32;
        if ((shooter_actors + arg0)->z >= -0x61A7) {
            (shooter_actors + arg0)->argument_x++;
        }
        break;
    case 3:
        (shooter_actors + arg0)->stats[0] = 0x20;
        break;
    }
}

void shooter_spawn_homing_mines(s32 arg0) {
    ShooterPosition sp58;
    ShooterPosition sp50;
    ShooterPosition sp48;
    /* FAKE: reserves the unused word under the position locals (the spill temporary sits one word lower); real local unknown. T-16564 */
    u8 pad[4];

    srand(shooter_script_timer);
    if (arg0 == 0) {
        sp50.x = ((rand() % 2000) + ((rand() % 4000) + shooter_actors->x)) - 0xBB8;
        sp50.y = ((rand() % 1500) + ((rand() % 2500) + shooter_actors->y)) - 0x7D0;
        sp50.z = (rand() % 20000) + 0x8000;
        sp58.x = (rand() % 30) + 0x28;
        sp48.y = 0x800;
        sp48.x = 0;
        sp48.z = 0;
        sp58.y = rand() % 2;
        if (shooter_script_timer < 0xF0) {
            shooter_spawn_enemy(7, 7, sp58, sp50, sp48, 5, 0x50);
            return;
        }
        shooter_spawn_enemy(7, 7, sp58, sp50, sp48, 5, 0);
        return;
    }
    sp50.y = -0x384;
    sp50.z = -0x6590;
    sp48.y = 0x800;
    sp48.x = 0;
    sp48.z = 0;
    sp58.x = (rand() % 20) + 0x1E;
    sp50.x = 0;
    sp58.y = rand() % 2;
    shooter_actors[shooter_spawn_enemy(7, 7, sp58, sp50, sp48, 0x14, 0)].stats[0] = 4;
    sp58.x = (rand() % 20) + 0x1E;
    sp50.x = -0x3E8;
    sp58.y = rand() % 2;
    shooter_actors[shooter_spawn_enemy(7, 7, sp58, sp50, sp48, 0x14, 0)].stats[0] = 4;
    sp58.x = (rand() % 20) + 0x1E;
    sp50.x = 0x3E8;
    sp58.y = rand() % 2;
    shooter_actors[shooter_spawn_enemy(7, 7, sp58, sp50, sp48, 0x14, 0)].stats[0] = 4;
}

void shooter_behavior_homing_mine(s32 arg0) {
    (arg0 + shooter_actors)->z += 0xFA;
    (arg0 + shooter_actors)->x += (shooter_actors->x - (arg0 + shooter_actors)->x) / (arg0 + shooter_actors)->argument_x;
    (arg0 + shooter_actors)->y += (shooter_actors->y - (arg0 + shooter_actors)->y) / (arg0 + shooter_actors)->argument_x;
    if ((arg0 + shooter_actors)->argument_y == 0) {
        (arg0 + shooter_actors)->rotation_z += 0xC8;
    } else {
        (arg0 + shooter_actors)->rotation_z -= 0xC8;
    }
    if ((arg0 + shooter_actors)->z >= 0x7D1) {
        shooter_remove_enemy(arg0);
    }
}
