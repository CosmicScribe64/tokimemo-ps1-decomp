/* player_control.c: src/ovl/TACO/80132220.c:4: player positions, input, fire handling and player update; body 5: shooter_actors->unk74 = 0; 6: shooter_actors->unk76 = 0;. Original object 0x80132220-0x801335A0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_reset_ship_state(void) {
    shooter_actors->x = 0;
    shooter_actors->y = 0;
    shooter_actors->z = 0x800;
    shooter_actors[8].x = 0;
    shooter_actors[8].y = 0;
    shooter_actors[8].z = 0x800;
    shooter_actors->mode = 0;
    shooter_actors[8].mode = 0;
    shooter_actors->phase = 0;
    shooter_actors->rotation_z = 0;
    shooter_actors[8].rotation_z = 0;
}

void shooter_init_ships(void) {
    shooter_init_actor_model(0x8019C800, 0, shooter_actors);
    shooter_actors->flag_bytes[1] = 0x80;
    shooter_actors->scale_x = 0x400;
    shooter_actors->scale_y = 0x400;
    shooter_actors->scale_z = 0x400;
    shooter_init_actor_model(0x8019C800, 1, &shooter_actors[8]);
    shooter_actors[8].flag_bytes[1] = 0x80;
    shooter_actors[8].scale_x = 0x400;
    shooter_actors[8].scale_y = 0x400;
    shooter_actors[8].scale_z = 0x400;
    shooter_actors->counter = 0;
    shooter_actors[8].counter = 0;
    shooter_actors->weapon = -1;
    shooter_actors[8].weapon = -1;
    shooter_actors->auxiliary = -1;
    shooter_actors[8].auxiliary = -1;
    shooter_actors->argument_x = 4;
    shooter_actors->argument_y = 3;
    shooter_actors->argument_z = 0;
    shooter_actors->stats[1] = 0xFF;
    shooter_actors[8].stats[1] = 0x80;
    shooter_reset_ship_state();
    shooter_bonus_popup_timer = 0;
    shooter_bonus_popup_value = 0;
}

/* An int function with no return statement (K&R): with it the switch operands sit in $v0 as in the original; a returned result variable moves them to $a0. */
s32 shooter_update_player() {
    /* FAKE: pad1..pad4 are unused locals that reserve the original's four extra frame words (m1 at sp+0x30, m2 at sp+0x2C); real source unknown. T-19025 */
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 m1;
    s32 m2;
    s32 pad4;
    ShooterActor *p;
    s32 step;
    u32 r;

    p = shooter_actors;
    step = 0x20;
    if ((game_state.pad_held & 0xA000) && (game_state.pad_held & 0x5000)) {
        step = 0x2D;
    }
    if (game_state.pad_held & PAD_LEFT) {
        if (shooter_actors->x > 0x800) {
            p->x = p->x + step * (0x1000 - p->x) / 2048;
        } else {
            p->x = p->x + step;
        }
    }
    if (game_state.pad_held & PAD_RIGHT) {
        if (p->x < -0x800) {
            p->x = p->x + step * (-0x1000 - p->x) / 2048;
        } else {
            p->x = p->x - step;
        }
    }
    if (game_state.pad_held & PAD_UP) {
        if (p->y < -0x700) {
            p->y = p->y + step * (-0x800 - p->y) / 256;
        } else {
            p->y = p->y - step;
        }
    }
    if (game_state.pad_held & PAD_DOWN) {
        if (p->y > 0x80) {
            p->y = p->y + step * (0x180 - p->y) / 256;
        } else {
            p->y = p->y + step;
        }
    }
    if (p->x < -0x1000) {
        p->x = -0x1000;
    }
    if (p->x > 0x1000) {
        p->x = 0x1000;
    }
    if (p->y < -0x800) {
        p->y = -0x800;
    }
    if (p->y > 0x180) {
        p->y = 0x180;
    }
    switch (shooter_setting_roll_button % 3) {
    case 0:
        m1 = 5;
        m2 = 0xA;
        break;
    case 1:
        m1 = 4;
        m2 = 8;
        break;
    case 2:
        m1 = 1;
        m2 = 2;
        break;
    }
    if (p->phase == 0) {
        if (game_state.pad_pressed & m1) {
            shooter_actors[8].mode += 1;
            p->mode = 0x81;
            p->phase = 0x7F;
        }
        if (game_state.pad_pressed & m2) {
            shooter_actors[8].mode += 1;
            p->mode = 0x82;
            p->phase = 0x7F;
        }
    }
    if (p->mode & 1) {
        r = p->phase;
        p->rotation_z += r;
        p->x += 8;
        if (r < 0xF) {
            if ((p->rotation_z & 0xFFF) >= 0xFE1) {
                p->phase = 0;
                p->rotation_z = 0;
                p->mode = 0;
            }
        } else {
            p->phase = p->phase - 1;
            if (p->phase < 0x40) {
                p->mode &= 0x7F;
            }
        }
    }
    if (p->mode & 2) {
        r = p->phase;
        p->rotation_z -= r;
        p->x -= 8;
        if (r < 0xF) {
            if ((p->rotation_z & 0xFFF) < 0x1F) {
                p->phase = 0;
                p->rotation_z = 0;
                p->mode = 0;
            }
        } else {
            p->phase = p->phase - 1;
            if (p->phase < 0x40) {
                p->mode &= 0x7F;
            }
        }
    }
    if (p->mode & 4) {
        switch (shooter_actors->counter--) {
        case 2:
        case 10:
        case 14:
        case 15:
            shooter_flash_red();
            break;
        case 0:
            shooter_actors->mode &= 0xFB;
            break;
        }
    }
}

void shooter_update_wingman(void) {
    /* FAKE: pad1..pad4 are unused locals that reserve the original's extra frame words (idx at sp+0x70, in/out vectors at sp+0x58/0x48, flags at sp+0x40, dist at sp+0x38); real source unknown. T-19025 */
    ShooterActor *act;
    u32 len;
    s32 pad1;
    s32 idx;
    s32 pad2;
    ShooterActor *q;
    VECTOR in;
    VECTOR out;
    s32 pad3;
    s32 flags;
    s32 pad4;
    s32 dist;

    dist = shooter_count_targets(&idx);
    flags = 0;
    if (dist != 0) {
        if (!(game_state.frame_count & 7)) {
            flags = 0x20;
        } else if (!(game_state.frame_count & 0xF)) {
            flags = 0x20;
        }
    }
    act = &shooter_actors[8];
    shooter_actors[8].flag_bytes[1] = 0x80;
    act->stats[3] = 1;
    act->stats[0] = 2;
    if (act->weapon == 0) {
        shooter_fire_wide_shot(1, flags);
    } else if (act->weapon == 1) {
        shooter_fire_beam(1, flags);
    } else {
        shooter_fire_vulcan(1, flags);
    }
    if (act->auxiliary == 0) {
        shooter_fire_secondary_homing(1, flags);
    } else if (act->auxiliary == 1) {
        shooter_fire_secondary_spread(1, flags);
    }
    if ((u32)dist < 8) {
        in.vx = shooter_actors->x - act->x;
        in.vy = shooter_actors->y - act->y;
        in.vz = 0;
        len = func_800AA460(&in, &out);
        switch (act->mode) {
        default:
            act->mode = 0;
        case 0:
            act->flag_bytes[1] = 0x80;
            act->mode += 1;
        case 1:
            if (len >= 0x10001) {
                act->x += out.vx / 103;
                act->y += out.vy / 103;
            }
            break;
        case 2:
            act->mode += 1;
        case 3:
            if (len >= 0x10001) {
                act->x += out.vy / 103;
                act->y += out.vx / 103;
            }
            break;
        case 4:
            act->flag_bytes[1] = 0x80;
            act->mode += 1;
        case 5:
            in.vx = shooter_actors->x - act->x;
            in.vy = shooter_actors->y - act->y + 0x80;
            in.vz = 0;
            len = func_800AA460(&in, &out);
            if (len >= 0x641) {
                act->x += out.vy / 103;
                act->y += out.vx / 103;
            }
            break;
        case 6:
            act->flag_bytes[1] = 0x90;
            act->argument_x = 3 - (rand() & 7);
            act->argument_y = 3 - (rand() & 7);
            act->mode += 1;
        case 7:
            if (shooter_actors->z + 0x21 < act->z) {
                act->argument_z = -0x20;
            }
            if (act->z < 0x29) {
                act->argument_z = 0x20;
            }
            break;
        case 8:
            act->flag_bytes[1] = 0x90;
            in.vx = 0xF - (rand() & 0x1F);
            in.vy = 0xF - (rand() & 0x1F);
            in.vz = 0xF - (rand() & 0x1F);
            func_800AA460(&in, &out);
            act->argument_x = out.vx / 103;
            act->argument_y = out.vy / 103;
            act->argument_z = out.vz / 103;
            act->mode += 1;
        case 9:
            if (!(rand() & 0x7F)) {
                act->mode = 8;
            }
            break;
        }
    } else {
        q = &shooter_actors[idx];
        in.vx = q->x - act->x;
        in.vy = q->y - act->y;
        in.vz = 0;
        func_800AA460(&in, &out);
        act->x += out.vx / 103;
        act->y += out.vy / 103;
    }
    act->z += act->argument_z;
    if (act->x > 0x1000) {
        act->x = 0x1000;
    }
    if (act->x < -0x1000) {
        act->x = -0x1000;
    }
    if (act->y > 0x180) {
        act->y = 0x180;
    }
    if (act->y < -0x800) {
        act->y = -0x800;
    }
    if (!(rand() & 0x3F)) {
        act->argument_z = 0xF - (rand() & 0x1F);
    }
    if (shooter_actors->z + 0x200 < act->z) {
        act->argument_z = -(rand() & 0xF);
    }
    if (act->z < -0x1000) {
        act->argument_z = rand() & 0xF;
    }
    act->scale_x = 0x400;
    act->scale_y = 0x400;
    act->scale_z = 0x400;
}

void shooter_update_battle(void) {
    /* FAKE: pad1..pad3 are unused locals that reserve the original's three frame words above flags (flags at sp+0x30, actor at sp+0x2C, btn at sp+0x28); real source unknown. flags is left unset on the unk2 & 0x40 path and passed to shooter_update_bomb as is, as in the original. T-19025 */
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 flags;
    ShooterActor *actor;
    /* FAKE: btn copies game_state.pad_pressed in both arms (the first arm reads it after the flag tests) and all six mask tests use it; it stays live in $v0 across the shooter_setting_bomb_button tests so those take $a0 as in the original. Matches, real source unknown. T-19025 */
    s32 btn;

    actor = shooter_actors;
    if (actor->mode & 0x40) {
    } else {
        shooter_update_player();
        shooter_update_wingman();
        shooter_update_secondary_shots();
        shooter_update_primary_shots();
        flags = 0;
        if (shooter_setting_auto_shot != 0) {
            if ((game_state.pad_held & PAD_CIRCLE) && (shooter_setting_shot_button % 3 == 0) && !(game_state.frame_count & 3)) {
                flags = 0x20;
            }
            if ((game_state.pad_held & PAD_CROSS) && (shooter_setting_shot_button % 3 == 1) && !(game_state.frame_count & 3)) {
                flags |= 0x20;
            }
            if ((game_state.pad_held & PAD_SQUARE) && (shooter_setting_shot_button % 3 == 2) && !(game_state.frame_count & 3)) {
                flags |= 0x20;
            }
            btn = game_state.pad_pressed;
        } else {
            btn = game_state.pad_pressed;
            if (btn) {} /* FAKE: empty test of btn; without it the three btn masks that uopt inserts at the end of the first arm come out as 0x20, 0x80, 0x40, the original has 0x80, 0x40, 0x20. T-19025 */
            if ((btn & 0x20) && (shooter_setting_shot_button % 3 == 0)) {
                flags = 0x20;
            }
            if ((btn & 0x40) && (shooter_setting_shot_button % 3 == 1)) {
                flags |= 0x20;
            }
            if ((btn & 0x80) && (shooter_setting_shot_button % 3 == 2)) {
                flags |= 0x20;
            }
        }
        if ((btn & 0x40) && (shooter_setting_bomb_button % 3 == 0)) {
            flags |= 0x40;
        }
        if ((btn & 0x80) && (shooter_setting_bomb_button % 3 == 1)) {
            flags |= 0x40;
        }
        if ((btn & 0x20) && (shooter_setting_bomb_button % 3 == 2)) {
            flags |= 0x40;
        }
        if (actor->weapon == 0) {
            shooter_fire_wide_shot(0, flags);
        } else if (actor->weapon == 1) {
            shooter_fire_beam(0, flags);
        } else {
            shooter_fire_vulcan(0, flags);
        }
        if (actor->auxiliary == SHOOTER_SECONDARY_HOMING) {
            shooter_fire_secondary_homing(0, flags);
        } else if (actor->auxiliary == SHOOTER_SECONDARY_SPREAD) {
            shooter_fire_secondary_spread(0, flags);
        }
    }
    shooter_update_bomb(flags);
    shooter_run_collisions();
    shooter_update_hit_sparks();
    shooter_update_explosions();
    shooter_update_enemy_bullets();
    shooter_collect_items();
    shooter_update_items();
}

void shooter_check_battle_end(void) {
    if (shooter_stages_cleared < 3U) {
        shooter_time_left -= 1;
    }
    if (shooter_time_left == 0) {
        mode_stage_set(0x2D);
    }
    if (game_state.pad_pressed & PAD_START) {
        mode_stage_set(0x2D);
    }
    if (shooter_actors->stats[1] == 0) {
        mode_stage_set(0x2D);
    }
}

void shooter_clear_beam_flags(s32 ship_index) {
    s32 i;

    if (ship_index == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) shooter_player_primary_shots[i].flags &= ~0x20;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) shooter_wingman_primary_shots[i].flags &= ~0x20;
    }
}

void shooter_clear_player_shots(void) {
    s32 i;

    shooter_pulse_bomb_active = 0;
    shooter_pulse_bomb_timer = 0; for (i = 0; i < 16; i++) { /* FAKE: store and loop head on one source line, as1 then schedules the store after the pointer setup */
        shooter_player_primary_shots[i].flags = 0;
        shooter_wingman_primary_shots[i].flags = 0;
        shooter_player_secondary_shots[i].flags = 0;
        shooter_wingman_secondary_shots[i].flags = 0;
        shooter_bomb_shots[i].flags = 0;
    }
}

void shooter_clear_enemy_bullets(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        shooter_enemy_bullets[i].flags = 0;
    }
}
