/* enemy_dispatch.c: src/ovl/TACO/8015A900.c:4: behavior switch and the generic leave behaviors; body 5: shooter_script_timer = 0; 6: shooter_script_phase += 1;. Original object 0x8015A900-0x8015ABF0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_next_script_phase(void) {
    shooter_script_timer = 0;
    shooter_script_phase += 1;
    shooter_circler_pairing = 0;
}

void shooter_next_script_subphase(void) {
    shooter_script_sub_timer = 0;
    shooter_script_counter += 1;
}

void shooter_run_enemy_behavior(u8 arg0, s32 arg1) {
    switch (arg0) {
    case 1:
        shooter_behavior_debris(arg1);
        return;
    case 2:
        shooter_behavior_swooper(arg1);
        return;
    case 3:
        shooter_behavior_spiral_in(arg1);
        return;
    case 4:
        shooter_behavior_spiral_out(arg1);
        return;
    case 5:
        shooter_behavior_turret_sweep(arg1);
        return;
    case 6:
        shooter_behavior_stage1_mid_boss(arg1);
        return;
    case 7:
        shooter_behavior_homing_mine(arg1);
        return;
    case 0x10:
        shooter_behavior_patroller(arg1);
        return;
    case 0x13:
        shooter_behavior_gate_a(arg1);
        return;
    case 0x14:
        shooter_behavior_gate_b(arg1);
        return;
    case 0x12:
        shooter_behavior_formation_ship(arg1);
        return;
    case 0x11:
        shooter_behavior_circler(arg1);
        return;
    case 0x15:
        shooter_behavior_carrier(arg1);
        return;
    case 0x16:
        shooter_behavior_turret(arg1);
        return;
    case 0x20:
        shooter_behavior_swarm_ship(arg1);
        return;
    case 0x21:
        shooter_behavior_wall_ship(arg1);
        return;
    case 0x22:
        shooter_behavior_orbiting_orb(arg1);
        return;
    case 0x23:
        shooter_behavior_guardian_core(arg1);
        return;
    case 0xFD:
        shooter_behavior_leave_back(arg1);
        return;
    case 0xFE:
        shooter_behavior_leave_forward(arg1);
        return;
    case 0xFF:
        shooter_behavior_dying(arg1);
        return;
    }
}

void shooter_behavior_leave_forward(s32 arg0) {
    (arg0 + shooter_actors)->z += 0x100;
    if ((arg0 + shooter_actors)->z > 0x1000) {
        shooter_remove_enemy();
    }
}

void shooter_behavior_leave_back(s32 arg0) {
    (arg0 + shooter_actors)->z -= 0xFA;
    if ((arg0 + shooter_actors)->z < -0x7530) {
        shooter_remove_enemy();
    }
}
