/* swarm_and_orbit_waves.c: src/ovl/TACO/80159090.c:4: timed stage script with swarming ships, wall ships and orbiting parts; body 5: shooter_script_timer += 1; 6: shooter_update_enemies();. Original object 0x80159090-0x8015A900 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_update_stage3_script(void) {
    shooter_script_timer += 1;
    shooter_update_enemies();
    if (((s32) shooter_script_timer % 30) == 0) {
        if (shooter_script_phase < 4) {
            if (shooter_count_enemies_of_kind(2) < 0xA) {
                shooter_spawn_swarm_ship();
            }
        } else if ((shooter_script_phase < 6) && (shooter_count_enemies_of_kind(2) < 3)) {
            shooter_spawn_swarm_ship();
        }
    }
    switch (shooter_script_phase) {
    case 0:
        shooter_spawn_wall_ships(shooter_script_phase);
        shooter_next_script_phase();
    default:
        break;
    case 1:
    case 2:
    case 3:
        if (shooter_count_enemies_of_kind(3) == 0) {
            shooter_spawn_wall_ships(shooter_script_phase);
            shooter_next_script_phase();
        }
        break;
    case 4:
        if (shooter_count_enemies_of_kind(3) == 0) {
            shooter_next_script_phase();
        }
        break;
    case 5:
        shooter_next_script_phase();
        break;
    case 6:
        shooter_next_script_phase();
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        if (shooter_count_active_enemies() == 0) {
            shooter_spawn_guardian_ring(shooter_script_phase - 3);
            shooter_next_script_phase();
        }
        break;
    case 11:
        if (shooter_count_enemies_of_kind(0xFF) == 0) {
            shooter_next_script_phase();
        }
        break;
    case 12:
        if (shooter_count_active_enemies() == 0) {
            shooter_reset_enemies();
            mode_step_next();
        }
        break;
    }
}

s32 shooter_spawn_swarm_ship(void) {
    ShooterPosition vel;
    ShooterPosition pos;
    /* FAKE: unused ShooterPosition below the two positions reproduces the frame and their offsets; real locals unknown. T-16560 */
    ShooterPosition pad;
    s32 r;
    u32 kind;

    srand(shooter_script_timer);
    r = rand();
    pos.x = ((rand() % 2500) + (r % 1500)) - 0x7D0;
    r = rand();
    pos.y = ((rand() % 2000) + (r % 1000)) - 0x7D0;
    pos.z = (rand() % 5000) - 0x61A8;
    vel.x = (rand() % 200) - 0x64;
    vel.y = (rand() % 200) - 0x64;
    kind = rand() % 5U;
    switch (kind) {
    case 0:
        shooter_spawn_enemy(2, 0x20, vel, pos, shooter_zero_position_b, 4, 0x10);
        return;
    case 1:
        shooter_spawn_enemy(2, 0x20, vel, pos, shooter_zero_position_b, 4, 0x10);
        return;
    case 2:
        shooter_spawn_enemy(2, 0x20, vel, pos, shooter_zero_position_b, 4, 0);
        return;
    case 3:
        shooter_spawn_enemy(2, 0x20, vel, pos, shooter_zero_position_b, 4, 0);
        return;
    case 4:
        shooter_spawn_enemy(2, 0x20, vel, pos, shooter_zero_position_b, 4, 0);
        return;
    }
}

void shooter_behavior_swarm_ship(s32 actor_index) {
    (actor_index + shooter_actors)->x -= (((((actor_index + shooter_actors)->z << 5) / 25000) * func_800A0140((actor_index + shooter_actors)->counter << 5)) >> 0xC);
    (actor_index + shooter_actors)->y -= (((((actor_index + shooter_actors)->z << 4) / 25000) * rsin((actor_index + shooter_actors)->counter << 7)) >> 0xC);
    if ((actor_index + shooter_actors)->counter >= 0x12D) {
        (actor_index + shooter_actors)->z += 0x100;
        (actor_index + shooter_actors)->x += ((shooter_actors->x + (actor_index + shooter_actors)->argument_x) - (actor_index + shooter_actors)->x) / 10;
        (actor_index + shooter_actors)->y += ((shooter_actors->y + (actor_index + shooter_actors)->argument_y) - (actor_index + shooter_actors)->y) / 10;
    }
    if ((actor_index + shooter_actors)->z >= 0x1001) {
        shooter_remove_enemy(actor_index);
    }
}

s32 shooter_spawn_wall_ships(s32 wave_variant) {
    /* FAKE: unused ShooterPosition above the two positions reproduces the frame (0x60) and their offsets; real locals unknown. T-16561 */
    ShooterPosition pad;
    ShooterPosition sp50;
    ShooterPosition sp48;

    srand(shooter_script_timer);
    sp48.x = 0;
    sp48.z = 0;
    sp50.z = -0x4E20;
    /* FAKE: each `sp48.y = ..; sp50.x = ..;` pair shares a source line; as1 orders the sh stores by line and only this layout gives the original order. T-16561 */
    switch (wave_variant) {
    case 0:
    case 2:
        sp48.y = 0x400; sp50.x = 0x1B58;
        sp50.y = -0x4B0;
        shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0);
        sp50.y = -0xC8;
        shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0);
        sp48.y = 0xC00; sp50.x = -0x1B58;
        sp50.y = -0x2BC;
        if (wave_variant == 0) {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 0xA, 0xE0);
            return;
        }
        shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 0xA, 0);
        return;
    case 1:
        /* fallthrough */
    case 3:
        sp48.y = 0xC00; sp50.x = -0x1B58;
        sp50.y = -0x4B0;
        if (wave_variant == 3) {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0x50);
        } else {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0);
        }
        sp50.y = -0xC8;
        if (wave_variant == 1) {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0x50);
        } else {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0);
        }
        sp48.y = 0x400; sp50.x = 0x1B58;
        sp50.y = -0x2BC;
        if (wave_variant == 3) {
            shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 0xA, 0x80);
            return;
        }
        shooter_spawn_enemy(3, 0x21, shooter_zero_position_a, sp50, sp48, 8, 0);
        return;
    default:
        /* the empty default adds the basic blocks that keep wave_variant out of $s0 (uopt allocation cost); the original reloads it from its home. T-16561 */
        break;
    }
}

void shooter_behavior_wall_ship(s32 actor_index) {
    ShooterPosition pos;

    (actor_index + shooter_actors)->x += -(actor_index + shooter_actors)->x / 50;
    if ((actor_index + shooter_actors)->counter >= 0xB5) {
        (actor_index + shooter_actors)->counter = 0;
    }
    if (((actor_index + shooter_actors)->counter % 10) == 0) {
        if (abs((actor_index + shooter_actors)->x) < 0x3E8) {
            pos.x = (actor_index + shooter_actors)->x;
            pos.y = (actor_index + shooter_actors)->y;
            pos.z = (actor_index + shooter_actors)->z;
            switch ((actor_index + shooter_actors)->y) {
            case -1200:
                if ((actor_index + shooter_actors)->counter < 0x3C) {
                    shooter_spawn_enemy_bullet(pos, 0x180, 0xC0);
                }
                break;
            case -700:
                if ((actor_index + shooter_actors)->counter >= 0x3D && (actor_index + shooter_actors)->counter < 0x78) {
                    shooter_spawn_enemy_bullet(pos, 0x180, 0x82);
                }
                break;
            case -200:
                if ((actor_index + shooter_actors)->counter >= 0x79) {
                    shooter_spawn_enemy_bullet(pos, 0x180, 0xC0);
                }
                break;
            }
        }
    }
}

void shooter_spawn_guardian_ring(s32 enemy_count) {
    /* FAKE: unused leading ShooterPosition local reproduces the frame (see shooter_spawn_patroller); real locals unknown. T-9180 */
    ShooterPosition p0;
    ShooterPosition pos;
    ShooterPosition vel;
    s32 i;
    s32 j;

    srand(shooter_script_timer);
    pos.z = 0x4E20;
    shooter_spawn_enemy(0xD, 0x23, shooter_zero_position_a, pos, shooter_zero_position_b, 0x28, 0xD0);
    i = 0;
    if (enemy_count > 0) {
        j = 0;
        do {
            vel.y = 0x800;
            vel.x = 0;
            vel.z = 0;
            vel.w = j / enemy_count;
            shooter_spawn_enemy(0xD, 0x22, shooter_zero_position_a, pos, vel, 0x14, (rand() % 9) * 0x10);
            i += 1;
            j += 0x1000;
        } while (i != enemy_count);
    }
}

void shooter_behavior_orbiting_orb(s32 actor_index) {
    switch ((actor_index + shooter_actors)->argument_x) {
    case 0:
        shooter_actors[actor_index].x = (rsin((actor_index + shooter_actors)->param) * 0x3E8) >> 0xC;
        shooter_actors[actor_index].y = ((func_800A0140((actor_index + shooter_actors)->param) * 0x3E8) >> 0xC) - 0x2BC;
        shooter_actors[actor_index].z = -0x4E20;
        shooter_actors[actor_index].scale_y = 1;
        (actor_index + shooter_actors)->argument_x += 1;
        break;
    case 1:
        (actor_index + shooter_actors)->scale_y = (actor_index + shooter_actors)->counter << 9;
        (actor_index + shooter_actors)->stats[0] = ((actor_index + shooter_actors)->counter * 0x16) / 32;
        if ((actor_index + shooter_actors)->scale_y >= (actor_index + shooter_actors)->scale_x) {
            (actor_index + shooter_actors)->argument_x += 1;
            shooter_actors[actor_index].counter = 0;
        }
        break;
    case 2:
        shooter_actors[actor_index].x = (rsin((actor_index + shooter_actors)->param) * 0x3E8) >> 0xC;
        shooter_actors[actor_index].y = ((func_800A0140((actor_index + shooter_actors)->param) * 0x3E8) >> 0xC) - 0x2BC;
        (actor_index + shooter_actors)->param += 0x20;
        if ((actor_index + shooter_actors)->counter >= 0x259) {
            (actor_index + shooter_actors)->argument_x += 1;
            shooter_actors[actor_index].counter = 0;
        }
        break;
    case 3:
        (actor_index + shooter_actors)->scale_y = (0x10 - (actor_index + shooter_actors)->counter) << 9;
        (actor_index + shooter_actors)->stats[0] = ((0x10 - (actor_index + shooter_actors)->counter) << 5) / 32;
        if ((actor_index + shooter_actors)->scale_y <= 0) {
            shooter_remove_enemy(actor_index);
        }
        break;
    }
}

void shooter_behavior_guardian_core(s32 actor_index) {
    ShooterPosition pos;

    pos.x = (actor_index + shooter_actors)->x;
    pos.y = (actor_index + shooter_actors)->y;
    pos.z = (actor_index + shooter_actors)->z;
    switch ((actor_index + shooter_actors)->argument_x) {
    case 0:
        (actor_index + shooter_actors)->x = 0;
        shooter_actors[actor_index].y = -0x2BC;
        shooter_actors[actor_index].z = -0x4E20;
        shooter_actors[actor_index].scale_y = 1;
        (actor_index + shooter_actors)->argument_x += 1;
        break;
    case 1:
        (actor_index + shooter_actors)->scale_y = (actor_index + shooter_actors)->counter << 9;
        (actor_index + shooter_actors)->stats[0] = ((actor_index + shooter_actors)->counter * 0x16) / 32;
        if ((actor_index + shooter_actors)->scale_y >= (actor_index + shooter_actors)->scale_x) {
            (actor_index + shooter_actors)->argument_x += 1;
            shooter_actors[actor_index].counter = 0;
        }
        break;
    case 2:
        if (((actor_index + shooter_actors)->counter % 15) == 0) {
            shooter_spawn_enemy_bullet(pos, 0x100, 0xC2);
        }
        if ((actor_index + shooter_actors)->counter >= 0x259) {
            (actor_index + shooter_actors)->argument_x += 1;
            shooter_actors[actor_index].counter = 0;
        }
        break;
    case 3:
        (actor_index + shooter_actors)->scale_y = (0x10 - (actor_index + shooter_actors)->counter) << 9;
        (actor_index + shooter_actors)->stats[0] = ((0x10 - (actor_index + shooter_actors)->counter) << 5) / 32;
        if ((actor_index + shooter_actors)->scale_y <= 0) {
            shooter_remove_enemy(actor_index);
        }
        break;
    }
}
