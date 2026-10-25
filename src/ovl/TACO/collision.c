/* collision.c: src/ovl/TACO/8013B990.c:4: hits, damage, scoring, pickups, bomb damage; body 5: shooter_hit_ships_with_bullets(); 6: shooter_hit_ships_with_bodies();. Original object 0x8013B990-0x8013D1E0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_run_collisions(void) {
    shooter_hit_ships_with_bullets();
    shooter_hit_ships_with_bodies();
    if (game_state.mode_stage == 0xC && shooter_stages_cleared != 0) {
        return;
    }
    if (game_state.mode_stage == 0x16 && shooter_stages_cleared >= 2) {
        return;
    }
    if (game_state.mode_stage == 0x20 && shooter_stages_cleared >= 3) {
        return;
    }
    shooter_hit_enemies_with_shots(shooter_player_primary_shots, 1, 0);
    shooter_hit_enemies_with_shots(shooter_player_secondary_shots, 4, 0);
    shooter_hit_enemies_with_shots(shooter_wingman_primary_shots, 1, 1);
    shooter_hit_enemies_with_shots(shooter_wingman_secondary_shots, 4, 1);
    shooter_score = shooter_score / 10 * 10 + 1;
    if (shooter_score >= SHOOTER_SCORE_MAXIMUM_EXCLUSIVE) {
        shooter_score = SHOOTER_SCORE_MAXIMUM;
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit, so as1 fills the first branch slot of the
 * particle loop from the latch (void moves the kind test there), as in shooter_hit_ships_with_bullets. T-25111. */
s32 shooter_hit_enemies_with_shots(ShooterParticle *arg0, s32 arg1, s32 arg2) {
    /* The order of these scalars sets their frame homes (i third, damage tenth). T-25111. */
    s32 x;
    s32 y;
    s32 i;
    s32 z;
    s32 j;
    s32 radius;
    s32 xdist;
    s32 ydist;
    s32 kind;
    u32 damage;
    s32 width;
    ShooterShotDamage damageTable;
    u32 scores[256];

    damageTable = shooter_shot_damage_by_kind;
    /* FAKE: through the byte view, tools/cvt_pass.py does not see a switch on an unsigned global
     * and leaves global copy propagation on. The y distances then fold into their compares, as in
     * the original (no `move a0,v0`), and the stage is still compared in v1. T-25111. */
    switch (*(u8 *)&game_state.mode_stage) {
    case 0xC:
        for (j = 0; j < 256; j++) scores[j] = 0x190;
        break;
    case 0x16:
        for (j = 0; j < 256; j++) scores[j] = 0xC8;
        break;
    case 0x20:
        for (j = 0; j < 256; j++) scores[j] = 0x320;
        break;
    default:
        for (j = 0; j < 256; j++) scores[j] = shooter_score_by_class[j];
        break;
    }
    shooter_hit_sound_played = 0;
    if (arg0->flags & 0x20) {
        for (i = 0; i != 2; i++) {
            if (game_state.frame_count & 1) {
                x = (i + arg0)->x;
                y = (i + arg0)->y;
                if (arg2 == 0) z = shooter_actors[0].z;
                else z = shooter_actors[8].z;
                for (j = 16; j != 63; j++) {
                    if (((j + shooter_actors)->flag_bytes[1] & 0x80) && ((j + shooter_actors)->stats[3] & 1) && (j + shooter_actors)->stats[1] != 0 && (j + shooter_actors)->stats[0] != 0) {
                        radius = (j + shooter_actors)->stats[0] * 16;
                        xdist = abs(x - (j + shooter_actors)->x);
                        ydist = abs(y - (j + shooter_actors)->y);
                        if (xdist < radius + 0x18 && ydist < radius + 0x18) {
                            if ((j + shooter_actors)->z < z) {
                                if (shooter_hit_sound_played == 0 && arg2 == 0) {
                                    shooter_hit_sound_played = 1;
                                    shooter_play_se(1, 0x507);
                                }
                                if ((j + shooter_actors)->stats[1] != 0) (j + shooter_actors)->stats[1]--;
                                shooter_spawn_hit_spark(x, y, (j + shooter_actors)->z - 0x200);
                                shooter_score += scores[(shooter_actors + j)->stats[2] % 17] / 20;
                                /* continue, not a fall-through: with a plain if, ugen reuses the stored shooter_score (T-25111) */
                                if ((j + shooter_actors)->stats[1] != 0) continue;
                                shooter_score += scores[(shooter_actors + j)->stats[2] % 17];
                            }
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (((i + arg0)->flags & 0x80) && (kind = (i + arg0)->flags & 15) != 0) {
            x = (i + arg0)->x;
            y = (i + arg0)->y;
            z = (i + arg0)->z;
            damage = damageTable.by_kind[kind];
            if (kind == 8) width = 0x60;
            else width = 0x30;
            for (j = 16; j != 63; j++) {
                if (((j + shooter_actors)->flag_bytes[1] & 0x80) && ((j + shooter_actors)->stats[3] & 1) && (j + shooter_actors)->stats[1] != 0 && (j + shooter_actors)->stats[0] != 0) {
                    radius = (j + shooter_actors)->stats[0] * 16;
                    if (abs(z - (j + shooter_actors)->z) < radius + 0x100) {
                        xdist = abs(x - (j + shooter_actors)->x);
                        ydist = abs(y - (j + shooter_actors)->y);
                        if (xdist < radius + width && ydist < radius + width) {
                            if (shooter_hit_sound_played == 0 && arg2 == 0) {
                                shooter_hit_sound_played = 1;
                                shooter_play_se(1, 0x507);
                            }
                            if (damage < (j + shooter_actors)->stats[1]) (j + shooter_actors)->stats[1] -= damage;
                            else (j + shooter_actors)->stats[1] = 0;
                            if ((j + shooter_actors)->stats[1] == 0) (j + shooter_actors)->stats[3] &= 0xF0;
                            (i + arg0)->age = 0;
                            (i + arg0)->flags = 0;
                            shooter_spawn_hit_spark(((j + shooter_actors)->x + x) / 2, ((j + shooter_actors)->y + y) / 2, z - 0x200);
                            shooter_score += scores[(shooter_actors + j)->stats[2] % 17] / 10 * damage;
                            if ((j + shooter_actors)->stats[1] == 0) {
                                shooter_play_se(1, 0x508);
                                shooter_score += scores[(shooter_actors + j)->stats[2] % 17];
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void drops two
 * instructions); the hit in the second loop returns before it. T-19006. */
s32 shooter_hit_ships_with_bullets(void) {
    u32 i;
    s32 a;
    s32 b;
    u8 value;
    u8 value2;
    shooter_hit_sound_played = 0;
    if (!(shooter_actors->mode & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (shooter_enemy_bullets[i].flags & 0x80) {
                a = abs(shooter_enemy_bullets[i].x - shooter_actors->x);
                b = abs(shooter_enemy_bullets[i].y - shooter_actors->y);
                if (abs(shooter_enemy_bullets[i].z - shooter_actors->z) < 0x100 && b < 0x80 && a < 0x80) {
                    shooter_enemy_bullets[i].flags = 0;
                    shooter_play_se(1, 0x506);
                    shooter_actors->mode |= 4;
                    value = shooter_actors->stats[1];
                    if (value >= 5) {
                        shooter_actors->stats[1] = value - 4;
                    } else {
                        shooter_actors->stats[1] = 0;
                    }
                    shooter_actors->counter = 0x10;
                    break;
                }
            }
        }
    }
    if (!(shooter_actors[8].mode & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (shooter_enemy_bullets[i].flags & 0x80) {
                a = abs(shooter_enemy_bullets[i].x - shooter_actors[8].x);
                b = abs(shooter_enemy_bullets[i].y - shooter_actors[8].y);
                if (abs(shooter_enemy_bullets[i].z - shooter_actors[8].z) < 0x100 && b < 0x80 && a < 0x80) {
                    shooter_enemy_bullets[i].flags = 0;
                    value2 = shooter_actors[8].stats[1];
                    if (value2 >= 5) {
                        shooter_actors[8].stats[1] = value2 - 4;
                    } else {
                        shooter_actors[8].stats[1] = 0;
                    }
                    shooter_actors[8].counter = 0x10;
                    return;
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void changes the output),
 * as in shooter_hit_ships_with_bullets. The first loop is entered by a goto instead of an enclosing if: the if form
 * shares the guard's load of shooter_actors with the loop's pointer, the original loads it again. T-19006. */
s32 shooter_hit_ships_with_bodies(void) {
    u32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 limit;
    u8 value;
    u8 value2;
    shooter_hit_sound_played = 0;
    if (shooter_actors->mode & 0x80) {
        goto skip1;
    }
    for (i = 16; i < 63; i++) {
        if (((i + shooter_actors)->flag_bytes[1] & 0xC0) && ((i + shooter_actors)->stats[1] != 0)) {
            a = abs((i + shooter_actors)->x - shooter_actors->x);
            b = abs((i + shooter_actors)->y - shooter_actors->y);
            c = abs((i + shooter_actors)->z - shooter_actors->z);
            limit = (s32)(i + shooter_actors)->stats[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                shooter_play_se(1, 0x506);
                shooter_actors->mode |= 4;
                value = shooter_actors->stats[1];
                if (value >= 0xD) {
                    shooter_actors->stats[1] = value - 0xC;
                } else {
                    shooter_actors->stats[1] = 0;
                }
                shooter_actors->counter = 0x10;
                break;
            }
        }
    }
skip1:
    if (shooter_actors[8].mode & 0x80) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (((i + shooter_actors)->flag_bytes[1] & 0xC0) && ((i + shooter_actors)->stats[1] != 0)) {
            a = abs((i + shooter_actors)->x - shooter_actors[8].x);
            b = abs((i + shooter_actors)->y - shooter_actors[8].y);
            c = abs((i + shooter_actors)->z - shooter_actors[8].z);
            limit = (s32)(i + shooter_actors)->stats[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                value2 = shooter_actors[8].stats[1];
                if (value2 >= 0xD) {
                    shooter_actors[8].stats[1] = value2 - 0xC;
                } else {
                    shooter_actors[8].stats[1] = 0;
                }
                shooter_actors[8].counter = 0x10;
                return;
            }
        }
    }
}

/* FAKE: s32 with no return keeps the second loop's counter init (v0) live at the first loop's exit
 * (void drops the `move v0,zero`), as in shooter_hit_ships_with_bullets. T-25111. */
s32 shooter_collect_items(void) {
    s32 pad[6]; /* FAKE: T-25111, six home words above the locals; the original frame has them, their source is unknown */
    ShooterParticle *p;
    s32 dy;
    s32 dx;
    u8 kind;
    u8 value;
    s32 last_kind;
    s32 v;
    s32 got2;
    u32 i; /* declared between the flags: its home sits between them in the original frame */
    s32 got_bonus;
    s32 got5;
    s32 sound_ok;

    sound_ok = 1;
    last_kind = -7;
    got2 = -7;
    got_bonus = -7;
    got5 = -7;
    for (p = shooter_items; p < shooter_items + 16; p++) {
        if (p->flags & 0x80) {
            dx = abs(p->x - shooter_actors->x);
            dy = abs(p->y - shooter_actors->y);
            if (abs(p->z - shooter_actors->z) < 0x200 && dy < 0x200 && dx < 0x200) {
                shooter_score += 1000;
                kind = p->argument;
                p->flags = 0;
                switch (kind) {
                case 0:
                case 1:
                    last_kind = kind;
                    break;
                case 2:
                    got2 = 1;
                    break;
                case 3:
                case 4:
                    shooter_play_se(1, 0x509);
                    if (shooter_actors->argument_x == 0) {
                        shooter_actors->argument_x = p->argument;
                    } else if (shooter_actors->argument_y == 0) {
                        shooter_actors->argument_y = p->argument;
                    } else {
                        shooter_actors->argument_z = p->argument;
                    }
                    break;
                case 5:
                    got5 = 1;
                    value = shooter_actors->stats[1];
                    if (value < 0xEF) {
                        shooter_actors->stats[1] = value + 0x10;
                    } else {
                        shooter_actors->stats[1] = 0xFF;
                    }
                    break;
                default:
                    shooter_bonus_popup_timer = 0x20;
                    got_bonus = 1;
                    shooter_bonus_popup_value = 1;
                    switch (rand() & 7) {
                    case 0:
                        shooter_bonus_popup_value = 100;
                        break;
                    case 1:
                        shooter_bonus_popup_value = 200;
                        break;
                    case 2:
                        shooter_bonus_popup_value = 400;
                        break;
                    case 3:
                        shooter_bonus_popup_value = 800;
                        break;
                    case 4:
                        shooter_bonus_popup_value = 1600;
                        break;
                    case 5:
                        shooter_bonus_popup_value = 3200;
                        break;
                    case 6:
                        shooter_bonus_popup_value = 6400;
                        break;
                    case 7:
                    default: /* every path assigns shooter_bonus_popup_value, so uopt keeps it in v0 up to the sum */
                        shooter_bonus_popup_value = 57300;
                        break;
                    }
                    shooter_score += shooter_bonus_popup_value;
                    break;
                case 7:
                    last_kind = -1;
                    break;
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (shooter_items[i].flags & 0x80) {
            v = shooter_items[i].argument;
            if (v != 3 && v != 4) {
                dx = abs(shooter_items[i].x - shooter_actors[8].x);
                dy = abs(shooter_items[i].y - shooter_actors[8].y);
                v = abs(shooter_items[i].z - shooter_actors[8].z);
                if (v < 0x200 && dy < 0x200 && dx < 0x200) {
                    v = shooter_items[i].argument;
                    shooter_items[i].flags = 0;
                    switch (v) {
                    case 0:
                    case 1:
                    case 7:
                        shooter_actors[8].weapon = v;
                        break;
                    case 2:
                        v = shooter_actors[8].auxiliary;
                        if (v == -1) {
                            shooter_actors[8].auxiliary = 1;
                            v = shooter_actors[8].auxiliary;
                        }
                        shooter_actors[8].auxiliary = 1 - v;
                        break;
                    case 5:
                        value = shooter_actors[8].stats[1];
                        if (value < 0xEF) {
                            shooter_actors[8].stats[1] = value + 0x10;
                        } else {
                            shooter_actors[8].stats[1] = 0xFF;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (last_kind != shooter_actors->weapon) {
        if (last_kind != -7) {
            shooter_actors->weapon = last_kind;
            switch (last_kind) {
            case -1:
                shooter_clear_beam_flags(0);
                shooter_play_se(1, 0x500);
                sound_ok = 0;
                break;
            case 0:
                shooter_clear_beam_flags(0);
                shooter_play_se(1, 0x514);
                sound_ok = 0;
                break;
            case 1:
                shooter_clear_beam_flags(0);
                shooter_play_se(1, 0x512);
                sound_ok = 0;
                break;
            }
        }
    }
    if (got2 == 1) {
        v = shooter_actors->auxiliary;
        if (v == -1) {
            shooter_actors->auxiliary = 1;
            v = shooter_actors->auxiliary;
        }
        shooter_actors->auxiliary = 1 - v;
        if (sound_ok != 0) {
            switch (shooter_actors->auxiliary) {
            case 0:
                shooter_play_se(1, 0x511);
                sound_ok = 0;
                break;
            case 1:
                shooter_play_se(1, 0x516);
                sound_ok = 0;
                break;
            }
        }
    }
    if (got5 == 1) {
        if (sound_ok != 0) {
            shooter_play_se(1, 0x510);
            sound_ok = 0;
        }
    }
    if (got_bonus == 1) {
        if (sound_ok != 0) {
            shooter_play_se(1, 0x509);
        }
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit (void changes one instruction), as in
 * shooter_bomb_damage_all. The i == 8 test is dead (the loop starts at 16) but the original has it. T-19006. */
s32 shooter_explode_dead_enemies(void) {
    u32 i;
    for (i = 16; i < 64; i++) {
        if (((i + shooter_actors)->stats[1] == 0) && ((i + shooter_actors)->flag_bytes[1] & 0x80)) {
            if (!(game_state.frame_count & 0x1F)) {
                shooter_play_se(1, 0x508);
            }
            shooter_spawn_explosion(*(ShooterPosition *)&(i + shooter_actors)->x, *(ShooterPosition *)&(i + shooter_actors)->argument_x, 0);
            shooter_spawn_explosion(*(ShooterPosition *)&(i + shooter_actors)->x, *(ShooterPosition *)&(i + shooter_actors)->argument_x, 0);
            if (i == 8) {
                (shooter_actors + i)->stats[1] = rand() & 0x3F;
                (shooter_actors + i)->x = rand() % 100;
                (shooter_actors + i)->y = rand() % 100;
            }
        }
    }
}

void shooter_nop(void) {
}

/* FAKE: s32 fall-through and the two pointer-addition orders retain the
 * original result/address registers. T-10010. */
s32 shooter_bomb_damage_all(void) {
    u32 i;
    u8 value;
    for (i = 16; i < 64; i++) {
        if (((i + shooter_actors)->flag_bytes[1] & 0x80) && ((i + shooter_actors)->stats[3] & 1)) {
            value = (i + shooter_actors)->stats[1];
            if (value != 0 && (i + shooter_actors)->stats[0] != 0) {
                if (value >= 3U) {
                    (i + shooter_actors)->stats[1] = value - 2;
                } else {
                    (i + shooter_actors)->stats[1] = 0;
                }
                shooter_spawn_hit_spark((i + shooter_actors)->x, (i + shooter_actors)->y, (i + shooter_actors)->z - 0x200);
                if ((i + shooter_actors)->stats[1] == 0) {
                    shooter_play_se(1, 0x508);
                    shooter_score += shooter_score_by_class[(shooter_actors + i)->stats[2] % 17U];
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit, as in shooter_bomb_damage_all (void changes one
 * instruction). T-19006. */
s32 shooter_bomb_damage_by_distance(void) {
    u32 i;
    s16 x;
    s16 y;
    s32 a;
    s32 b;
    s32 d;
    s32 t;
    s32 u;
    x = shooter_bomb_shots[0].x;
    y = shooter_bomb_shots[0].y;
    for (i = 16; i < 64; i++) {
        if (((i + shooter_actors)->flag_bytes[1] & 0x80) && ((i + shooter_actors)->stats[3] & 1) && ((i + shooter_actors)->stats[1] != 0) && ((i + shooter_actors)->stats[0] != 0)) {
            t = abs(x - (i + shooter_actors)->x);
            u = abs(y - (i + shooter_actors)->y);
            a = 0x1000 - t;
            if (a < 0) {
                a = 0;
            }
            b = 0x1000 - u;
            if (b < 0) {
                b = 0;
            }
            d = (a + b) / 256;
            if (d != 0) {
                shooter_spawn_hit_spark((i + shooter_actors)->x, (i + shooter_actors)->y, (i + shooter_actors)->z - 0x200);
            }
            if ((u32)d < (i + shooter_actors)->stats[1]) {
                (i + shooter_actors)->stats[1] -= d;
            } else {
                (i + shooter_actors)->stats[1] = 0;
            }
            if ((i + shooter_actors)->stats[1] == 0) {
                shooter_play_se(1, 0x508);
                shooter_score += shooter_score_by_class[(shooter_actors + i)->stats[2] % 17U];
            }
        }
    }
}
