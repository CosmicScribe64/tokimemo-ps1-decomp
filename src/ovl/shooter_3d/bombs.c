/* bombs.c: src/ovl/TACO/80141DE0.c:6: bomb state machine and blast bomb shots; body 15: if (shooter_try_start_bomb(arg0) == 0) {; 16: return 0;. Original object 0x80141DE0-0x80142820 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

/* No return at the end: the paths that leave shooter_update_pulse_bomb() or shooter_update_blast_shots() non-zero return their
 * result in $v0 (implicit int, K&R style). T-19024 */
s32 shooter_update_bomb(s32 fire_flags) {
    u32 idx;
    u32 temp;
    s32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    ShooterParticle *p;

    if (shooter_try_start_bomb(fire_flags) == 0) {
        return 0;
    }
    if (shooter_try_start_bomb(fire_flags) != 4 && shooter_try_start_bomb(fire_flags) != 3) {
        if (shooter_try_start_bomb(fire_flags) == 1) {
            if (!(game_state.frame_count & 3) && shooter_pulse_bomb_timer >= 0x41) {
                shooter_bomb_damage_all();
                shooter_play_se(1, 0x505);
            }
        } else if (!(shooter_bomb_shots[0].age & 3)) {
            /* FAKE: the temporary splits the first definition of idx so that uopt colours idx $v1 and the byte
             * $a0, as in the original; real source unknown. T-19024 */
            temp = shooter_bomb_shots[0].age >> 2;
            idx = temp;
            if (shooter_bomb_shots[0].age > 0x80) {
                shooter_bomb_damage_by_distance();
                shooter_play_se(1, 0x504);
                idx = shooter_bomb_shots[0].age >> 2;
            }
            if ((s32) idx < 0x10) {
                p = &shooter_bomb_shots[idx];
                p->flags = 0x90;
                p->x = shooter_actors->x;
                p->y = shooter_actors->y + 0x10;
                p->z = shooter_actors->z - 0x64;
                p->velocity_x = 0;
                p->velocity_y = 0;
                p->velocity_z = -0x20;
                p->age = 0;
            }
            p = &shooter_bomb_shots[idx];
            if (p->age > 0x10 && shooter_bomb_shots[0].age < 0x80) {
                p->velocity_z = -0x100;
            }
            dx = shooter_actors[0].x - shooter_actors[8].x;
            dy = shooter_actors[0].y - shooter_actors[8].y;
            dz = shooter_actors[0].z - shooter_actors[8].z - 0x200;
            shooter_actors[8].x += dx / 2;
            shooter_actors[8].y += dy / 2;
            shooter_actors[8].z += dz / 2;
        }
    }
    if (shooter_pulse_bomb_active != 0) {
        if (shooter_update_pulse_bomb() == 0) {
            shooter_actors->mode &= 0xFF3F;
            shooter_bomb_damage_all();
            return shooter_consume_bomb();
        }
    } else if (shooter_update_blast_shots() == 0) {
        shooter_actors->mode &= 0xFF3F;
        for (i = 0; i < 16; i++) {
            shooter_bomb_shots[i].flags = 0;
        }
        shooter_bomb_damage_by_distance();
        return shooter_consume_bomb();
    }
}

s16 shooter_consume_bomb() {
    if (shooter_actors->argument_z == 3 || shooter_actors->argument_z == 4) {
        shooter_actors->argument_z = 0;
        return shooter_actors->argument_z;
    }
    if (shooter_actors->argument_y == 3 || shooter_actors->argument_y == 4) {
        shooter_actors->argument_y = 0;
        return shooter_actors->argument_y;
    }
    if (shooter_actors->argument_x == 3 || shooter_actors->argument_x == 4) {
        shooter_actors->argument_x = 0;
        return shooter_actors->argument_x;
    }
    return 0;
}

s16 shooter_peek_bomb() {
    if (shooter_actors->argument_z == 3 || shooter_actors->argument_z == 4) {
        return shooter_actors->argument_z;
    }
    if (shooter_actors->argument_y == 3 || shooter_actors->argument_y == 4) {
        return shooter_actors->argument_y;
    }
    if (shooter_actors->argument_x == 3 || shooter_actors->argument_x == 4) {
        return shooter_actors->argument_x;
    }
    return 0;
}

s32 shooter_try_start_bomb(s32 fire_flags) {
    ShooterParticle *p;

    for (p = shooter_bomb_shots; p < &shooter_bomb_shots[16]; p++) {
        if (p->flags & 0x80) {
            return -1;
        }
    }
    if (shooter_pulse_bomb_active != 0) {
        return 1;
    }
    if (shooter_actors->mode & 0x80) {
        return 0;
    }
    if (fire_flags & 0x40) {
        if (shooter_peek_bomb() == 3) {
            shooter_actors->mode |= 0xC0;
            shooter_clear_player_shots();
            shooter_clear_enemy_bullets();
            shooter_pulse_bomb_active = 1;
            return 3;
        }
        if (shooter_peek_bomb() == 4) {
            shooter_actors->mode |= 0xC0;
            shooter_clear_player_shots();
            shooter_clear_enemy_bullets();
            shooter_bomb_shots[0].flags = 0x90;
            shooter_bomb_shots[0].x = shooter_actors->x;
            shooter_bomb_shots[0].y = shooter_actors->y;
            shooter_bomb_shots[0].z = shooter_actors->z - 0xC8;
            shooter_bomb_shots[0].velocity_x = 0;
            shooter_bomb_shots[0].velocity_y = 0;
            shooter_bomb_shots[0].velocity_z = -8;
            shooter_bomb_shots[0].age = 0;
            shooter_play_se(1, 0x505);
            return 4;
        }
    }
    return 0;
}

s32 shooter_update_blast_shots(void) {
    ShooterParticle *blast_shot;
    s32 blast_shot_index;
    u8 shot_age;

    blast_shot = shooter_bomb_shots;
    blast_shot_index = 0;
loop_1:
    shot_age = blast_shot->age;
    if (shot_age >= 0xFFU) {
        blast_shot->flags = 0;
        return 0;
    }
    if (shot_age >= 0x80U) {
        blast_shot->velocity_z = -0x400;
    }
    if (blast_shot->z < -0x7D00) {
        blast_shot->flags = 0;
    }
    if (blast_shot->flags & 0x80) {
        blast_shot->age++;
        blast_shot->x += blast_shot->velocity_x;
        blast_shot->y += blast_shot->velocity_y;
        blast_shot->z += blast_shot->velocity_z;
        /* A saved byte or extra mask changes IDO's reload here (T-9560). */
        shooter_draw_blast_shot(blast_shot_index, blast_shot->age);
    }
    blast_shot_index += 1;
    blast_shot += 1;
    if (blast_shot_index == 0x10) {
        return 1;
    }
    goto loop_1;
}

void shooter_draw_blast_shot(s32 arg0, s32 arg1) {
    /* FAKE: unused local word reserves the top frame slot so pt/v/m sit at their original offsets (T-19024, T-24120). */
    s32 pad[1];
    s32 y;
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    ShooterParticle *rec;
    POLY_FT4 *poly;
    s32 side;
    s32 o;
    s32 depth;
    s32 flag;
    s32 j;
    s32 step;
    u8 tbl[2];
    s32 k;

    *(u16 *)tbl = shooter_blast_shot_frame_u;
    if (arg1 >= 0x80) {
        step = 0x800;
    } else {
        step = (0x18000 - (arg1 << 9)) / 256;
    }
    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    rec = &shooter_bomb_shots[arg0];
    v.vz = rec->z;
    for (o = 0; o < 2; o++) {
        v.vy = rec->y + o * step - step / 2;
        for (j = 0; j < 2; j++) {
            v.vx = rec->x + j * step - step / 2;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        /* FAKE: indirect load via (u8 *)&draw_buffer + local y keeps uopt from copy-propagating k in BB_cond1 so k gets $a1 directly (T-24120). */
        k = *(s32 *)((u8 *)&draw_buffer);
        y = game_state.draw_origin_y[k];
        if (((s16 *)pt)[3] >= y - 0x200
            && y + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->age >> 2) & 1;
            poly = GetWorkBase(0x28, k);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(0x50, 0x1E7);
            poly->u0 = tbl[side], poly->v0 = 0, poly->u1 = tbl[side] + 0xF, poly->v1 = 0, poly->u2 = tbl[side], poly->v2 = 0xF, poly->u3 = tbl[side] + 0xF, poly->v3 = 0xF;
            poly->x0 = ((s16 *)pt)[0];
            poly->y0 = ((s16 *)pt)[1];
            poly->x1 = ((s16 *)pt)[2];
            poly->y1 = ((s16 *)pt)[3];
            poly->x2 = ((s16 *)pt)[4];
            poly->y2 = ((s16 *)pt)[5];
            poly->x3 = ((s16 *)pt)[6];
            poly->y3 = ((s16 *)pt)[7];
            AddPrim((draw_buffer << 10) + 0x10 + draw_ot_tags, poly);
        }
    }
}
