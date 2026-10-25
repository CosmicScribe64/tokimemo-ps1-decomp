/* formation_and_circling_waves.c: src/ovl/TACO/80156A80.c:4: timed stage script, patrol and formation ships, gate pairs, circling enemies, carriers and turrets; body 5: shooter_script_timer += 1; 6: shooter_script_sub_timer += 1;. Original object 0x80156A80-0x80159090 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_update_stage2_script(void) {
    shooter_script_timer += 1;
    shooter_script_sub_timer += 1;
    shooter_update_enemies();
    if (shooter_script_phase < 0xC) {
        switch (shooter_script_counter) {
        case 0:
            if (shooter_script_sub_timer == 1) {
                shooter_spawn_carrier();
            }
            if (shooter_count_enemies_of_kind(0xE) == 0) {
                shooter_next_script_subphase();
            }
            break;
        case 1:
            shooter_spawn_gate_pair();
            shooter_next_script_subphase();
            break;
        case 2:
        case 3:
            if (shooter_count_enemies_of_kind(0xA) == 0) {
                shooter_spawn_gate_pair();
                shooter_next_script_subphase();
            }
            break;
        case 4:
        case 5:
        case 6:
            shooter_next_script_subphase();
            break;
        case 7:
        case 8:
            shooter_next_script_subphase();
            break;
        case 9:
            shooter_next_script_subphase();
            shooter_script_counter = 0;
            break;
        }
    }
    switch (shooter_script_phase) {
    case 0:
    case 6:
        if ((shooter_script_timer >= 0x2B) && (shooter_count_enemies_of_kind(8) == 0)) {
            shooter_next_script_phase();
        }
        switch (shooter_script_timer) {
        case 2:
            shooter_spawn_formation_positive_x();
            break;
        case 10:
            shooter_spawn_formation_positive_x();
            break;
        case 18:
            shooter_spawn_formation_positive_x();
            break;
        case 26:
            shooter_spawn_formation_positive_x();
            break;
        case 34:
            shooter_spawn_formation_positive_x();
            break;
        case 42:
            shooter_spawn_formation_positive_x();
            break;
        }
    default:
        break;
    case 1:
    case 7:
        if ((shooter_script_timer >= 0x2B) && (shooter_count_enemies_of_kind(8) == 0)) {
            shooter_next_script_phase();
        }
        switch (shooter_script_timer) {
        case 2:
            shooter_spawn_formation_negative_x();
            break;
        case 10:
            shooter_spawn_formation_negative_x();
            break;
        case 18:
            shooter_spawn_formation_negative_x();
            break;
        case 26:
            shooter_spawn_formation_negative_x();
            break;
        case 34:
            shooter_spawn_formation_negative_x();
            break;
        case 42:
            shooter_spawn_formation_negative_x();
            break;
        }
        break;
    case 2:
    case 8:
        if ((shooter_script_timer >= 0x15) && (shooter_count_enemies_of_kind(8) == 0)) {
            shooter_next_script_phase();
        }
        if (shooter_script_timer == 5) {
            shooter_spawn_formation_v();
        }
        break;
    case 3:
    case 9:
        if ((shooter_script_timer >= 0x2B) && (shooter_count_enemies_of_kind(8) == 0)) {
            shooter_next_script_phase();
        }
        switch (shooter_script_timer) {
        case 2:
            shooter_spawn_formation_positive_x();
            break;
        case 10:
            shooter_spawn_formation_negative_x();
            break;
        case 18:
            shooter_spawn_formation_positive_x();
            break;
        case 26:
            shooter_spawn_formation_negative_x();
            break;
        case 34:
            shooter_spawn_formation_positive_x();
            break;
        case 42:
            shooter_spawn_formation_negative_x();
            break;
        }
        break;
    case 4:
    case 5:
    case 10:
    case 11:
        if ((shooter_script_timer >= 0x42) && (shooter_count_enemies_of_kind(0xC) == 0)) {
            shooter_next_script_phase();
        }
        switch (shooter_script_timer) {
        case 0x5:
            shooter_spawn_circling_pair();
            break;
        case 0x23:
            shooter_spawn_circling_pair();
            break;
        case 0x41:
            shooter_spawn_circling_pair();
            break;
        }
        break;
    case 12:
        if (shooter_count_enemies_of_kind(0xFF) == 0) {
            shooter_next_script_phase();
        }
        break;
    case 13:
        if (shooter_count_active_enemies() == 0) {
            shooter_reset_enemies();
            mode_step_next();
        }
        break;
    }
}

void shooter_spawn_patroller(void) {
    /* the family's position locals and spill (see shooter_spawn_carrier); the unused ones keep their stack
     * slots. T-9020 */
    ShooterPosition p0;
    ShooterPosition pos;
    ShooterPosition p2;
    s32 r;

    srand(shooter_script_timer);
    r = rand();
    pos.x = (shooter_actors[0].x + (rand() % 1500) + (r % 500)) - 0x3E8;
    r = rand();
    pos.y = (rand() % 1000 + (r % 500)) - 0x5DC;
    pos.z = -0x8000;
    shooter_spawn_enemy(0xB, 0x10, shooter_zero_position_a, pos, shooter_zero_position_b, 6, 0);
}

void shooter_behavior_patroller(s32 arg0) {
    ShooterPosition pos;

    if ((arg0 + shooter_actors)->counter % 60 == 0) {
        pos.x = (arg0 + shooter_actors)->x;
        pos.y = (arg0 + shooter_actors)->y;
        pos.z = (arg0 + shooter_actors)->z;
        shooter_spawn_enemy_bullet(pos, 0x80, 0);
    }
    (arg0 + shooter_actors)->y += rsin((arg0 + shooter_actors)->counter << 6) * 0x14 >> 12;
    (arg0 + shooter_actors)->z += 0x96;
    if ((arg0 + shooter_actors)->z > 0x1000) {
        shooter_remove_enemy(arg0);
    }
}

void shooter_spawn_formation_positive_x(void) {
    ShooterPosition p0;
    ShooterPosition a;
    ShooterPosition b;

    srand(shooter_script_timer + 0x1E240);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = 0x3E8; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.w = 3;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 4, 0);
}

void shooter_spawn_formation_negative_x(void) {
    ShooterPosition p0;
    ShooterPosition a;
    ShooterPosition b;

    srand(shooter_script_timer + 0x1E240);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = -0x3E8; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.w = 1;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 4, 0);
}

void shooter_spawn_formation_v(void) {
    ShooterPosition p0;
    ShooterPosition a;
    ShooterPosition b;

    srand(shooter_script_timer);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = 0x1F4; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.w = 0;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 4, 0x10);
    a.x = 0xFA; a.y = 0x12C; a.z = -0x7830;
    b.w = 1;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 6, 0);
    a.x = 0; a.y = 0x12C; a.z = -0x7060;
    b.w = 2;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 2, 0x50);
    a.x = -0xFA; a.y = 0x12C; a.z = -0x7830;
    b.w = 3;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 6, 0);
    a.x = -0x1F4; a.y = 0x12C; a.z = -0x8000;
    b.w = 4;
    shooter_spawn_enemy(8, 0x12, shooter_zero_position_a, a, b, 4, 0x10);
}

void shooter_behavior_formation_ship(s32 arg0) {
    ShooterFormationVelocities sp50;
    ShooterFormationSpins sp3C;
    ShooterPosition pos;

    sp50 = shooter_formation_velocities;
    sp3C = shooter_formation_spins;
    switch ((arg0 + shooter_actors)->argument_x) {
    case 0:
        (arg0 + shooter_actors)->z += 0x100;
        if ((arg0 + shooter_actors)->z >= -0x1D4B) {
            (arg0 + shooter_actors)->counter = 0;
            (arg0 + shooter_actors)->argument_x += 1;
        }
        break;
    case 1:
        (arg0 + shooter_actors)->x += (s32) (sp50.vectors[(arg0 + shooter_actors)->param].x * (arg0 + shooter_actors)->counter) / 16;
        (arg0 + shooter_actors)->y += (s32) (sp50.vectors[(arg0 + shooter_actors)->param].y * (arg0 + shooter_actors)->counter) / 16;
        (arg0 + shooter_actors)->z += (s32) ((-(arg0 + shooter_actors)->counter << 8) + 0x1000) / 16;
        (arg0 + shooter_actors)->rotation_x += 0x40;
        (arg0 + shooter_actors)->rotation_y += sp3C.spins[(arg0 + shooter_actors)->param];
        if (((s16) (arg0 + shooter_actors)->counter % 20) == 0) {
            pos.x = (arg0 + shooter_actors)->x;
            pos.y = (arg0 + shooter_actors)->y;
            pos.z = (arg0 + shooter_actors)->z;
            shooter_spawn_enemy_bullet(pos, 0x80, 0xC0);
        }
        if ((arg0 + shooter_actors)->rotation_x >= 0x400) {
            (arg0 + shooter_actors)->argument_x += 1;
        }
        break;
    case 2:
        (arg0 + shooter_actors)->x += sp50.vectors[(arg0 + shooter_actors)->param].x;
        (arg0 + shooter_actors)->y += sp50.vectors[(arg0 + shooter_actors)->param].y;
        if (((arg0 + shooter_actors)->y < -0xFA0) || ((arg0 + shooter_actors)->x < -0x1770) || ((arg0 + shooter_actors)->x >= 0x1771)) {
            shooter_remove_enemy(arg0);
        }
        break;
    }
}

void shooter_spawn_gate_pair(void) {
    ShooterPosition p0;
    ShooterPosition pos;
    ShooterPosition b;
    s32 t;
    s32 r;

    srand(shooter_script_timer);
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.w = 0;
    r = rand();
    pos.x = (rand() % 5000 + shooter_actors->x + r % 3000) - 0xFA0;
    pos.y = (rand() % 2) * 0x1F40 - 0xBB8;
    pos.z = -0x2742;
    t = shooter_spawn_enemy(9, 0x13, shooter_zero_position_a, pos, b, 0x40, 0);
    pos.z = -0x2710;
    b.w = t;
    shooter_spawn_enemy(0xA, 0x14, shooter_zero_position_a, pos, b, 0x40, 0x80);
}

void shooter_behavior_gate_b(s32 arg0) {
    switch ((arg0 + shooter_actors)->argument_x) {
    case 0:
        (arg0 + shooter_actors)->x += (shooter_actors[0].x - (arg0 + shooter_actors)->x) / 10;
        (arg0 + shooter_actors)->y += (shooter_actors[0].y - (arg0 + shooter_actors)->y) / 10;
        shooter_actors[arg0].z = -0x2710;
        if ((arg0 + shooter_actors)->counter >= 0x3D) {
            (arg0 + shooter_actors)->argument_x += 1;
            shooter_actors[arg0].counter = 0;
        }
        break;
    case 1:
        if ((arg0 + shooter_actors)->counter >= 0x5B) {
            (arg0 + shooter_actors)->argument_x += 1;
            shooter_actors[arg0].counter = 0;
        }
        break;
    case 2:
        (arg0 + shooter_actors)->rotation_y = 0x800 - ((arg0 + shooter_actors)->counter << 6);
        if ((arg0 + shooter_actors)->rotation_y <= 0) {
            (arg0 + shooter_actors)->argument_x += 1;
            shooter_actors[arg0].counter = 0;
        }
        break;
    case 3:
        (arg0 + shooter_actors)->mode = 0xFD;
        break;
    }
}

void shooter_behavior_gate_a(s32 arg0) {
    switch ((arg0 + shooter_actors)->argument_x) {
    case 0:
        (arg0 + shooter_actors)->x += (shooter_actors[0].x - (arg0 + shooter_actors)->x) / 10;
        (arg0 + shooter_actors)->y += (shooter_actors[0].y - (arg0 + shooter_actors)->y) / 10;
        shooter_actors[arg0].z = -0x2742;
        if ((arg0 + shooter_actors)->counter >= 0x3D) {
            (arg0 + shooter_actors)->argument_x += 1;
            shooter_actors[arg0].counter = 0;
        }
        break;
    case 1:
        if ((arg0 + shooter_actors)->counter >= 0x3D) {
            (arg0 + shooter_actors)->argument_x += 1;
        }
        break;
    case 2:
        (arg0 + shooter_actors)->mode = 0xFE;
        break;
    }
}

void shooter_spawn_circling_pair(void) {
    ShooterPosition p0;
    ShooterPosition p1;
    ShooterPosition b;

    srand(shooter_script_timer);
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    if (shooter_circler_pairing == 0) {
        if ((shooter_actors->x < 0 && shooter_actors->y < -0x2BC) || (shooter_actors->x > 0 && shooter_actors->y >= -0x2BB)) {
            b.w = 0x11;
            shooter_circler_pairing = 1;
            shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
            b.w = 0;
            shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
            return;
        }
        b.w = 1;
        shooter_circler_pairing = 2;
        shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
        b.w = 0x10;
        shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
        return;
    }
    if (shooter_circler_pairing == 1) {
        b.w = 0x11;
        shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
        b.w = 0;
        shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
        return;
    }
    b.w = 1;
    shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
    b.w = 0x10;
    shooter_spawn_enemy(0xC, 0x11, shooter_zero_position_a, shooter_circler_start_position, b, 2, 0x10);
}

/* The mixed spellings (arg0 + shooter_actors)-> and shooter_actors[arg0]. are deliberate: they give the original's addu operand order (T-19015) */
void shooter_behavior_circler(s32 arg0) {
    s32 ang;
    s32 rad;
    ShooterPosition pos;

    switch ((arg0 + shooter_actors)->argument_x) {
    case 0:
        if ((arg0 + shooter_actors)->param & 1) {
            (arg0 + shooter_actors)->x = -0x1770;
        } else {
            (arg0 + shooter_actors)->x = 0x1770;
        }
        if ((arg0 + shooter_actors)->param & 0x10) {
            (arg0 + shooter_actors)->y = -0x898;
        } else {
            (arg0 + shooter_actors)->y = 0x320;
        }
        shooter_actors[arg0].z = 0;
        (arg0 + shooter_actors)->argument_x += 1;
        break;
    case 1:
        if ((arg0 + shooter_actors)->param & 1) {
            (arg0 + shooter_actors)->x = (arg0 + shooter_actors)->counter * 0x1E - 0x1770;
        } else {
            (arg0 + shooter_actors)->x = 0x1770 - (arg0 + shooter_actors)->counter * 0x1E;
        }
        if ((arg0 + shooter_actors)->param & 0x10) {
            (arg0 + shooter_actors)->y = (arg0 + shooter_actors)->counter * 5 - 0x898;
        } else {
            (arg0 + shooter_actors)->y = 0x320 - (arg0 + shooter_actors)->counter * 5;
        }
        (arg0 + shooter_actors)->z = -(arg0 + shooter_actors)->counter * 0x1E;
        if ((arg0 + shooter_actors)->counter >= 0xC9) {
            (arg0 + shooter_actors)->counter = 0;
            (arg0 + shooter_actors)->argument_x += 1;
        }
        break;
    case 2:
        switch ((arg0 + shooter_actors)->param) {
        case 0:
            /* ang is stored before rad: the load delay slot of the unk7A read then takes the rad store, as in the original */
            ang = (arg0 + shooter_actors)->counter * 0x14;
            rad = -0x1F4;
            break;
        case 1:
            ang = -(arg0 + shooter_actors)->counter * 0x14;
            rad = -0x1F4;
            break;
        case 0x10:
            ang = -(arg0 + shooter_actors)->counter * 0x14;
            rad = 0x1F4;
            break;
        case 0x11:
            ang = (arg0 + shooter_actors)->counter * 0x14;
            rad = 0x1F4;
            break;
        }
        shooter_actors[arg0].x = (rsin(ang) * rad) >> 12;
        shooter_actors[arg0].y = ((func_800A0140(ang) * -rad) >> 12) - 0x2BC;
        if (((arg0 + shooter_actors)->counter % 20) == 0) {
            pos.x = (arg0 + shooter_actors)->x;
            pos.y = (arg0 + shooter_actors)->y;
            pos.z = (arg0 + shooter_actors)->z;
            shooter_spawn_enemy_bullet(pos, 0x100, 0xC0);
        }
        if ((arg0 + shooter_actors)->counter >= 0x19A) {
            (arg0 + shooter_actors)->argument_x += 1;
        }
        break;
    case 3:
        shooter_dismiss_circlers();
        shooter_actors[arg0].mode = 0xFE;
        break;
    }
}

void shooter_dismiss_circlers(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        if ((i + shooter_actors)->stats[2] == 0xC) {
            (i + shooter_actors)->mode = 0xFE;
        }
    }
}

void shooter_spawn_carrier(void) {
    /* p0..p2 and r are the locals of this family (shooter_spawn_homing_mines passes all three positions and spills r
     * at the same offsets); the unused ones keep their stack slots. T-9020 */
    ShooterPosition p0;
    ShooterPosition p1;
    ShooterPosition pos;
    s32 r;

    srand(shooter_script_timer);
    pos.y = 0x400;
    pos.x = 0;
    pos.z = 0;
    pos.w = 0;
    shooter_spawn_enemy(0xE, 0x15, shooter_zero_position_a, shooter_circler_start_position, pos, 0x10, 0xE0);
}

void shooter_behavior_carrier(s32 arg0) {
    (arg0 + shooter_actors)->x = 0x1B58 - (arg0 + shooter_actors)->counter * 0x14;
    shooter_actors[arg0].y = -0x6A4;
    shooter_actors[arg0].z = -0x4E20;
    if ((arg0 + shooter_actors)->x < -0x1B58) {
        shooter_remove_enemy(arg0);
    }
    if ((arg0 + shooter_actors)->x == -0x320) {
        shooter_spawn_turret_a((arg0 + shooter_actors)->x, (arg0 + shooter_actors)->y, (arg0 + shooter_actors)->z);
    }
    if ((arg0 + shooter_actors)->x == 0) {
        shooter_spawn_turret_b((arg0 + shooter_actors)->x, (arg0 + shooter_actors)->y, (arg0 + shooter_actors)->z);
    }
    if ((arg0 + shooter_actors)->x == 0x320) {
        shooter_spawn_turret_a((arg0 + shooter_actors)->x, (arg0 + shooter_actors)->y, (arg0 + shooter_actors)->z);
    }
}

void shooter_spawn_turret_a(s16 arg0, s16 arg1, s16 arg2) {
    /* the family's position locals and spill (see shooter_spawn_carrier); the unused ones keep their stack
     * slots. T-9020 */
    ShooterPosition p1;
    ShooterPosition pos;
    s32 r;

    srand(shooter_script_timer);
    pos.x = arg0;
    pos.y = arg1;
    pos.z = arg2;
    shooter_spawn_enemy(0xF, 0x16, shooter_zero_position_a, pos, shooter_zero_position_b, 4, 0xD0);
}

void shooter_spawn_turret_b(s16 arg0, s16 arg1, s16 arg2) {
    /* the family's position locals and spill (see shooter_spawn_carrier); the unused ones keep their stack
     * slots. T-9020 */
    ShooterPosition p1;
    ShooterPosition pos;
    s32 r;

    srand(shooter_script_timer);
    pos.x = arg0;
    pos.y = arg1;
    pos.z = arg2;
    if (!(rand() & 1)) {
        shooter_spawn_enemy(0x10, 0x16, shooter_zero_position_a, pos, shooter_zero_position_b, 8, 0x60);
        return;
    }
    shooter_spawn_enemy(0x10, 0x16, shooter_zero_position_a, pos, shooter_zero_position_b, 8, 0x70);
}

void shooter_behavior_turret(s32 arg0) {
    ShooterPosition pos;

    if ((arg0 + shooter_actors)->counter % 20 == 0) {
        pos.x = (arg0 + shooter_actors)->x;
        pos.y = (arg0 + shooter_actors)->y;
        pos.z = (arg0 + shooter_actors)->z;
        shooter_spawn_enemy_bullet(pos, 0x80, 0x80);
    }
    (arg0 + shooter_actors)->y += 0x20;
    if ((arg0 + shooter_actors)->y > 0x400) {
        (arg0 + shooter_actors)->mode = 0xFE;
    }
    if ((arg0 + shooter_actors)->argument_x == 0) {
        /* FAKE: call and increment on one source line make as1 place the reload of the spilled index as the original does. T-9310 */
        shooter_set_enemy_visibility(0, arg0); (arg0 + shooter_actors)->argument_x += 1;
    } else {
        shooter_set_enemy_visibility(1, arg0);
    }
}
