/* primary_weapons.c: src/ovl/TACO/80140DF0.c:4: vulcan, wide shot, beam; body 5: shooter_update_primary_shots_of(0); 6: shooter_update_primary_shots_of(1);. Original object 0x80140DF0-0x80141DE0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_update_primary_shots(void) {
    shooter_update_primary_shots_of(0);
    shooter_update_primary_shots_of(1);
}

void shooter_update_primary_shots_of(s32 arg0) {
    ShooterParticle *base;
    ShooterParticle *p;
    ShooterActor *act;
    s32 i;

    if (arg0 == 0) {
        act = shooter_actors;
        base = shooter_player_primary_shots;
    } else {
        base = shooter_wingman_primary_shots;
        act = &shooter_actors[8];
    }
    for (i = 0, p = base; i < 16; i++, p++) {
        p->age += 1;
        if (p->age == 0xFF) {
            p->flags = 0;
        }
        if (p->z < -0x7D00) {
            p->flags = 0;
        }
        switch (p->flags & 0x2F) {
        case 4:
            if (p->flags & 0x80) {
                p->x += p->velocity_x;
                p->y += p->velocity_y;
                p->z += p->velocity_z;
                shooter_draw_vulcan_shot(base, i);
            }
            break;
        case 8:
            if (p->flags & 0x80) {
                p->x += p->velocity_x;
                p->y += p->velocity_y;
                p->z += p->velocity_z;
                shooter_draw_wide_shot(base, i);
            }
            break;
        }
    }
    if ((base->flags & 0xA0) == 0xA0) {
        base->age += 1;
        if (base->age >= 3U) {
            base[0].x = act->x + 0x40;
            base[0].y = act->y + 0x20;
            base[0].z = act->z;
            base[1].x = act->x - 0x40;
            base[1].y = act->y + 0x20;
            base[1].z = act->z;
            shooter_draw_beam(base, arg0);
        }
        if (base->age >= 0x81U) {
            base->flags = 0;
            base[1].flags = 0;
        }
    }
}

s32 shooter_fire_vulcan(s32 arg0, s32 arg1) {
    s32 pad; /* FAKE: unused local; it gives the original 64-byte frame (pointer spill at sp+0x2C). T-19017 */
    s32 i;
    ShooterActor *act;
    ShooterParticle *base;

    if (arg0 == 0) {
        act = shooter_actors;
        base = shooter_player_primary_shots;
    } else {
        base = shooter_wingman_primary_shots;
        act = shooter_actors + 8;
    }
    if (arg1 & 0x20) {
        for (i = 0; i < 16; i++) {
            if (!(base[i].flags & 0x80)) {
                break;
            }
        }
        if (i >= 16) {
            return -1;
        }
        if (arg0 == 0) {
            shooter_play_se(1, 0x500);
        }
        base[i].flags = 0x94;
        base[i].x = act->x + ((i & 1) << 7) - 0x40;
        base[i].y = act->y - (arg0 << 7) + 0x80;
        base[i].z = act->z;
        base[i].velocity_x = 0;
        base[i].velocity_y = 0;
        base[i].velocity_z = -0x200;
        base[i].age = 0;
        return 1;
    }
    return 0;
}

s32 shooter_fire_wide_shot(s32 arg0, s32 arg1) {
    s32 pad; /* FAKE: unused local; it gives the original 64-byte frame, as in shooter_fire_vulcan. T-19017 */
    s32 i;
    ShooterActor *act;
    ShooterParticle *base;

    if (arg0 == 0) {
        act = shooter_actors;
        base = shooter_player_primary_shots;
    } else {
        base = shooter_wingman_primary_shots;
        act = shooter_actors + 8;
    }
    if (arg1 & 0x20) {
        for (i = 0; i < 16; i++) {
            if (!(base[i].flags & 0x80)) {
                break;
            }
        }
        if (i >= 16) {
            return -1;
        }
        if (arg0 == 0) {
            shooter_play_se(1, 0x502);
        }
        base[i].flags = 0x98;
        base[i].x = act->x + ((i & 1) << 7) - 0x40;
        base[i].y = act->y - (arg0 << 6) + 0x40;
        base[i].z = act->z;
        base[i].velocity_x = 0;
        base[i].velocity_y = 0;
        base[i].velocity_z = -0x200;
        base[i].age = 0;
        return 1;
    }
    return 0;
}

s32 shooter_fire_beam(s32 arg0, s32 arg1) {
    ShooterParticle *p;
    s32 free_count;
    s32 offset;
    ShooterActor *act;
    ShooterParticle *base;

    if (arg0 == 0) {
        act = shooter_actors;
        base = shooter_player_primary_shots;
    } else {
        base = shooter_wingman_primary_shots;
        act = shooter_actors + 8;
    }
    free_count = 0;
    if (arg1 & 0x20) {
        offset = 0;
        p = base;
        do {
            offset += 0x14;
            if (!(p->flags & 0x80)) {
                free_count += 1;
            }
            p += 1;
        } while (offset != 0x140);
        if (free_count != 0x10) {
            return -1;
        }
        if (arg0 == 0) {
            shooter_play_se(1, 0x501);
        }
        base->flags = 0xB0;
        base->x = act->x;
        base->y = act->y + 0x20;
        base->z = act->z;
        base[1].x = (s16) act->x;
        base[1].y = (s16) (act->y + 0x20);
        base[1].z = (s16) act->z;
        base->velocity_x = 0;
        base->velocity_y = 0;
        base->velocity_z = 0;
        base->age = 0;
        return 1;
    }
    return 0;
}

/* FAKE: pad and k are unused locals; they reproduce the original frame (208 bytes: eight above m, one word below tbl). Real
 * source unknown, same idiom as shooter_draw_secondary_shot. T-19017 */
void shooter_draw_vulcan_shot(ShooterParticle *arg0, s32 arg1) {
    s32 pad[2];
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    ShooterParticle *rec;
    POLY_FT4 *poly;
    s32 dist;
    s32 side;
    s32 depth;
    s32 flag;
    s32 o;
    s32 j;
    u8 tbl[2];
    s32 k;

    *(u16 *)tbl = shooter_vulcan_frame_u;
    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    rec = &arg0[arg1];
    v.vz = rec->z;
    for (o = 0; o < 2; o++) {
        v.vy = rec->y + o * 0x50 - 0x28;
        for (j = 0; j < 2; j++) {
            v.vx = rec->x + j * 0x50 - 0x28;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= game_state.draw_origin_y[draw_buffer] - 0x200
            && game_state.draw_origin_y[draw_buffer] + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->age >> 2) & 1;
            dist = (D_80122754 - rec->z) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(0x80, 0x1E4);
            /* FAKE: hand expansion of the PsyQ macro setUVWH(poly, tbl[side], 1, 0xE, 0xE) on one line, as in shooter_draw_secondary_shot
             * (as1 schedules the eight stores of one line together). T-19017 */
            poly->u0 = tbl[side], poly->v0 = 1, poly->u1 = tbl[side] + 0xE, poly->v1 = 1, poly->u2 = tbl[side], poly->v2 = 1 + 0xE, poly->u3 = tbl[side] + 0xE, poly->v3 = 1 + 0xE;
            poly->x0 = ((s16 *)pt)[0];
            poly->y0 = ((s16 *)pt)[1];
            poly->x1 = ((s16 *)pt)[2];
            poly->y1 = ((s16 *)pt)[3];
            poly->x2 = ((s16 *)pt)[4];
            poly->y2 = ((s16 *)pt)[5];
            poly->x3 = ((s16 *)pt)[6];
            poly->y3 = ((s16 *)pt)[7];
            AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
        }
    }
}

void shooter_draw_wide_shot(ShooterParticle *arg0, s32 arg1) {
    s32 pad[2]; /* FAKE: pad and k2 are unused; they reproduce the original frame (208 bytes). T-19017 */
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    ShooterParticle *rec;
    POLY_FT4 *poly;
    s32 size;
    s32 o;
    s32 depth;
    s32 flag;
    s32 j;
    s32 dist;
    u8 tbl[2];
    s32 side;
    s32 k2;

    *(u16 *)tbl = shooter_wide_shot_frame_u;
    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    rec = &arg0[arg1];
    dist = (D_80122754 - rec->z) / 127;
    if (dist >= 0x100) {
        dist = 0xFF;
    } else if (dist <= 0) {
        dist = 1;
    }
    size = (dist * 5 * 8 + 0x280) / 16;
    v.vz = rec->z;
    for (o = 0; o < 2; o++) {
        v.vy = rec->y + o * size - size / 2;
        for (j = 0; j < 2; j++) {
            v.vx = rec->x + j * size - size / 2;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        /* The early return (not a nested if) gives the original's separate load of draw_buffer for the GetWorkBase call. T-22005 */
        if (((s16 *)pt)[3] < game_state.draw_origin_y[draw_buffer] - 0x200 || game_state.draw_origin_y[draw_buffer] + 0x200 < ((s16 *)pt)[7]) {
            return;
        }
        side = rec->age;
        poly = GetWorkBase(0x28, draw_buffer);
        func_8009F040(poly);
        SetSemiTrans(poly, 1);
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
        poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
        poly->clut = func_8009ED74((side % 14) << 4, 0x1EB);
        /* FAKE: the four branches are hand expansions of setUVWH(poly, 0x80 + 0x20 * k, 0x20, 0x1E, 0x1E) on one line each
         * (as1 schedules the eight stores of one line together). T-19017 */
        if (dist < 0x20) {
            poly->u0 = 0x80, poly->v0 = 0x20, poly->u1 = 0x80 + 0x1E, poly->v1 = 0x20, poly->u2 = 0x80, poly->v2 = 0x20 + 0x1E, poly->u3 = 0x80 + 0x1E, poly->v3 = 0x20 + 0x1E;
        } else if (dist < 0x60) {
            poly->u0 = 0xA0, poly->v0 = 0x20, poly->u1 = 0xA0 + 0x1E, poly->v1 = 0x20, poly->u2 = 0xA0, poly->v2 = 0x20 + 0x1E, poly->u3 = 0xA0 + 0x1E, poly->v3 = 0x20 + 0x1E;
        } else if (dist < 0xA0) {
            poly->u0 = 0xC0, poly->v0 = 0x20, poly->u1 = 0xC0 + 0x1E, poly->v1 = 0x20, poly->u2 = 0xC0, poly->v2 = 0x20 + 0x1E, poly->u3 = 0xC0 + 0x1E, poly->v3 = 0x20 + 0x1E;
        } else {
            poly->u0 = 0xE0, poly->v0 = 0x20, poly->u1 = 0xE0 + 0x1E, poly->v1 = 0x20, poly->u2 = 0xE0, poly->v2 = 0x20 + 0x1E, poly->u3 = 0xE0 + 0x1E, poly->v3 = 0x20 + 0x1E;
        }
        poly->x0 = ((s16 *)pt)[0];
        poly->y0 = ((s16 *)pt)[1];
        poly->x1 = ((s16 *)pt)[2];
        poly->y1 = ((s16 *)pt)[3];
        poly->x2 = ((s16 *)pt)[4];
        poly->y2 = ((s16 *)pt)[5];
        poly->x3 = ((s16 *)pt)[6];
        poly->y3 = ((s16 *)pt)[7];
        AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
    }
}

/* FAKE: pad, k1, k7, k8, k2, t, k3 and k4 are unused locals; they reproduce the original frame (224 bytes: eight above m,
 * three words below pt, the homes of depth, flag and fade). Real source unknown. T-19017 */
void shooter_draw_beam(ShooterParticle *arg0, s32 arg1) {
    s32 pad[2];
    MATRIX m;
    SVECTOR v;
    s16 pt[8];
    u32 j;
    POLY_G4 *poly;
    s32 k1;
    s32 k7;
    s32 k8;
    s32 sgn;
    s32 i;
    s32 depth;
    s32 flag;
    s32 k2;
    s16 t;
    u8 fade;
    s32 k3;
    s32 k4;

    if (arg0->age > 0x80) {
        fade = 0xFF - arg0->age;
    } else {
        fade = arg0->age;
    }
    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    sgn = 1 - arg1;
    j = 0;
    /* The loop test is written on the address expression: it gives the original's sltu against &v.vz (bne with a counter
     * compare); see wiki/matching-notes.md, T-19017. */
    do {
        v.vz = arg0[j].z + (sgn << 11);
        v.vy = arg0[j].y + (sgn << 8);
        v.vx = arg0[j].x;
        func_800AB170(&v, &pt[j * 4], &depth, &flag);
        v.vz = -0x7FFF;
        v.vy = arg0[j].y + 0x100;
        v.vx = arg0[j].x;
        func_800AB170(&v, &pt[j * 4 + 2], &depth, &flag);
        j++;
    } while (&pt[j * 4 + 2] < &v.vz);
    poly = GetWorkBase(0x24, draw_buffer);
    func_8009F054(poly);
    SetSemiTrans(poly, 1);
    poly->g0 = 0x10;
    poly->r0 = fade + 0x20;
    poly->b0 = 0xC0;
    poly->r1 = fade + 0x20;
    poly->g1 = fade + 0x10;
    poly->b1 = 0xC0;
    poly->g2 = fade + 0x10;
    poly->r2 = 0x20;
    poly->b2 = 0xC0;
    poly->r3 = fade + 0x20;
    poly->g3 = fade + 0x10;
    poly->b3 = 0xC0;
    for (i = 0; i < 2; i++) {
        if (pt[i * 2] == pt[i * 2 + 4]) {
            pt[i * 2 + 4] = pt[i * 2 + 4] + 1;
        }
    }
    poly->x0 = pt[2];
    poly->y0 = pt[3];
    poly->x1 = pt[0];
    poly->y1 = pt[1];
    poly->x2 = pt[6];
    poly->y2 = pt[7];
    poly->x3 = pt[4];
    poly->y3 = pt[5];
    AddPrim((draw_buffer << 10) + 0x10 + draw_ot_tags, poly);
    dtd_on_tpage(0x340, 0x100, 4, 1, 0);
}
