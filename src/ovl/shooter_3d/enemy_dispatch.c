/* enemy_dispatch.c: src/ovl/TACO/8015A900.c:4: behavior switch and the generic leave behaviors; body 5: shooter_script_timer = 0; 6: shooter_script_phase += 1;. Original object 0x8015A900-0x8015ABF0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_next_script_phase(void) {
    shooter_script_timer = 0;
    shooter_script_phase += 1;
    shooter_circler_pairing = 0;
}

void shooter_next_script_subphase(void) {
    shooter_script_sub_timer = 0;
    shooter_script_counter += 1;
}

void shooter_run_enemy_behavior(u8 behavior_id, s32 actor_index) {
    switch (behavior_id) {
    case 1:
        shooter_behavior_debris(actor_index);
        return;
    case 2:
        shooter_behavior_swooper(actor_index);
        return;
    case 3:
        shooter_behavior_spiral_in(actor_index);
        return;
    case 4:
        shooter_behavior_spiral_out(actor_index);
        return;
    case 5:
        shooter_behavior_turret_sweep(actor_index);
        return;
    case 6:
        shooter_behavior_stage1_mid_boss(actor_index);
        return;
    case 7:
        shooter_behavior_homing_mine(actor_index);
        return;
    case 0x10:
        shooter_behavior_patroller(actor_index);
        return;
    case 0x13:
        shooter_behavior_gate_a(actor_index);
        return;
    case 0x14:
        shooter_behavior_gate_b(actor_index);
        return;
    case 0x12:
        shooter_behavior_formation_ship(actor_index);
        return;
    case 0x11:
        shooter_behavior_circler(actor_index);
        return;
    case 0x15:
        shooter_behavior_carrier(actor_index);
        return;
    case 0x16:
        shooter_behavior_turret(actor_index);
        return;
    case 0x20:
        shooter_behavior_swarm_ship(actor_index);
        return;
    case 0x21:
        shooter_behavior_wall_ship(actor_index);
        return;
    case 0x22:
        shooter_behavior_orbiting_orb(actor_index);
        return;
    case 0x23:
        shooter_behavior_guardian_core(actor_index);
        return;
    case 0xFD:
        shooter_behavior_leave_back(actor_index);
        return;
    case 0xFE:
        shooter_behavior_leave_forward(actor_index);
        return;
    case 0xFF:
        shooter_behavior_dying(actor_index);
        return;
    }
}

void shooter_behavior_leave_forward(s32 actor_index) {
    (actor_index + shooter_actors)->z += 0x100;
    if ((actor_index + shooter_actors)->z > 0x1000) {
        shooter_remove_enemy();
    }
}

void shooter_behavior_leave_back(s32 actor_index) {
    (actor_index + shooter_actors)->z -= 0xFA;
    if ((actor_index + shooter_actors)->z < -0x7530) {
        shooter_remove_enemy();
    }
}
