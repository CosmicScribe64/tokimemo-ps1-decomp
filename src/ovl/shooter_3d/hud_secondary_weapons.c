/* hud_secondary_weapons.c: src/ovl/TACO/8013F250.c:4: HUD, secondary weapons and target search; body 11: sp34 = (s16) shooter_time_left / 3600; 12: sp30 = (s32) ((s16) shooter_time_left % 3600) / 60;. Original object 0x8013F250-0x80140DF0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_draw_hud(void) {
    /* FAKE: preserve the observed local homes and frame extent. T-10200. */
    s32 pad[2];
    s32 sp34;
    s32 sp30;
    s32 sp2C;

    sp34 = (s16) shooter_time_left / 3600;
    sp30 = (s32) ((s16) shooter_time_left % 3600) / 60;
    sp2C = (s32) (((s16) shooter_time_left % 60) * 0x3C) / 100;
    shooter_draw_text((s32) "SCORE", -0xE0, -0x64, 0);
    shooter_draw_number(shooter_score + (shooter_stages_cleared * 0xF4240), -0x50, -0x64, 0);
    shooter_draw_text((s32) "TIME", 0x40, -0x64, 0);
    shooter_draw_number(sp34, 0x90, -0x64, 1);
    shooter_draw_number(sp30, 0xB0, -0x64, 2);
    shooter_draw_number(sp2C, 0xD0, -0x64, 3);
    shooter_draw_hud_panels();
    /* This counter uses an unsigned byte load; other TACO users have a signed view. */
    if (*(u8 *)&shooter_bonus_popup_timer != 0) {
        (*(u8 *)&shooter_bonus_popup_timer)--;
        shooter_draw_number(shooter_bonus_popup_value, 0x14, 0x64, 3);
    }
    shooter_play_triangle_voice();
}

void shooter_draw_hud_panels(void) {
    shooter_draw_reticle();
    shooter_draw_bomb_stock();
    shooter_draw_bomb_panel();
    shooter_draw_shield_gauge();
}

void shooter_draw_reticle(void) {
    s32 top[2]; /* FAKE: eight original frame bytes above matrix; real locals unknown. T-24502 */
    MATRIX matrix;
    SVECTOR v;
    s32 pt[4];
    s32 middle[4]; /* FAKE: original 16-byte gap above scalar homes. T-24502 */
    POLY_FT4 *poly;
    s32 y;
    s32 depth;
    s32 flag;
    s32 o;
    s32 j;

    matrix = D_80129C40;
    func_800A1100(&matrix);
    SetRotMatrix(&matrix);
    v.vz = -0x7000;
    /* FAKE: one source line preserves pointer/counter initialization order. T-24502 */
    y = 0; for (o = 0; o != 2; o++) {
        v.vy = shooter_actors->y + y;
        for (j = 0; j < 2; j++) {
            v.vx = shooter_actors->x + j * 0x200 - 0x100;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        y += 0x200;
    }
    poly = GetWorkBase(0x28, draw_buffer);
    func_8009F040(poly);
    SetSemiTrans(poly, 1);
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->tpage = func_8009ECB0(0, 1, 0x340, 0x100);
    poly->clut = func_8009ED74(0x10, 0x1EF);
    /* FAKE: UV macro expansion shares one line to preserve constant registers and store order. T-24502 */
    poly->u0 = 0x81, poly->v0 = 1, poly->u1 = 0x9F, poly->v1 = 1, poly->u2 = 0x81, poly->v2 = 0x1F, poly->u3 = 0x9F, poly->v3 = 0x1F;
    poly->x0 = ((s16 *)pt)[0];
    poly->y0 = ((s16 *)pt)[1];
    poly->x1 = ((s16 *)pt)[2];
    poly->y1 = ((s16 *)pt)[3];
    poly->x2 = ((s16 *)pt)[4];
    poly->y2 = ((s16 *)pt)[5];
    poly->x3 = ((s16 *)pt)[6];
    poly->y3 = ((s16 *)pt)[7];
    AddPrim(draw_ot_tags + (draw_buffer << 0xA) + 4, poly);
    if (game_state.frame_count & 8) {
        poly = GetWorkBase(0x28, draw_buffer);
        func_8009F040(poly);
        SetSemiTrans(poly, 1);
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
        poly->tpage = func_8009ECB0(0, 1, 0x340, 0x100);
        poly->clut = func_8009ED74(0x10, 0x1EF);
        /* Same UV macro expansion shape as above. */
        poly->u0 = 0x81, poly->v0 = 0x21, poly->u1 = 0x9F, poly->v1 = 0x21, poly->u2 = 0x81, poly->v2 = 0x3F, poly->u3 = 0x9F, poly->v3 = 0x3F;
        poly->x0 = ((s16 *)pt)[0];
        poly->y0 = ((s16 *)pt)[1];
        poly->x1 = ((s16 *)pt)[2];
        poly->y1 = ((s16 *)pt)[3];
        poly->x2 = ((s16 *)pt)[4];
        poly->y2 = ((s16 *)pt)[5];
        poly->x3 = ((s16 *)pt)[6];
        poly->y3 = ((s16 *)pt)[7];
        AddPrim(draw_ot_tags + (draw_buffer << 0xA) + 4, poly);
    }
}


void shooter_draw_bomb_stock(void) {
    s16 v;

    v = shooter_actors->argument_x;
    if ((v == 3) || (v == 4)) {
        shooter_draw_bomb_icon(0xB4, 0x60, v);
    }
    v = shooter_actors->argument_y;
    if ((v == 3) || (v == 4)) {
        shooter_draw_bomb_icon(0xC8, 0x60, v);
    }
    v = shooter_actors->argument_z;
    if ((v == 3) || (v == 4)) {
        shooter_draw_bomb_icon(0xDC, 0x60, v);
    }
}

/* FAKE: the UV stores share a source line to preserve constant scheduling.
 * T-9560; original statement layout unknown. */
void shooter_draw_bomb_icon(s32 arg0, s32 arg1, s32 arg2) {
    s32 palette;
    SPRT *sprite;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    sprite->w = 16;
    sprite->h = 16;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] + arg0;
    sprite->y0 = game_state.draw_origin_y[draw_buffer] + arg1;
    sprite->u0 = 0xC0; sprite->v0 = arg2 * 16 - 0x30;
    palette = ((game_state.frame_count >> 1) % 14) * 16;
    if (arg2 == 3) {
        sprite->clut = func_8009ED74(palette, 0x1EA);
    } else {
        sprite->clut = func_8009ED74(palette, 0x1E9);
    }
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    dtd_on_tpage(0x380, 0x100, 1, 0, 0);
}

/* FAKE: the second sprite's UV stores share a source line to preserve constant scheduling.
 * T-19014; original statement layout unknown. */
void shooter_draw_bomb_panel(void) {
    SPRT *sprite;
    u32 dy;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    sprite->w = 0x20;
    sprite->h = 0x20;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    dy = (shooter_actors->mode & 0x40) >> 2;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] + 0xC8;
    sprite->y0 = game_state.draw_origin_y[draw_buffer] + dy + 0x28;
    sprite->u0 = 0xE0;
    sprite->v0 = 0;
    if (shooter_actors->stats[1] < 0x20) {
        sprite->clut = func_8009ED74(0x10, 0x1EF);
    } else {
        sprite->clut = func_8009ED74(0, 0x1EF);
    }
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    sprite->w = 0x20;
    sprite->h = 0x20;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] + 0xC8;
    sprite->y0 = game_state.draw_origin_y[draw_buffer] + 0x48;
    sprite->u0 = 0xE0; sprite->v0 = 0x20;
    if (shooter_actors->stats[1] < 0x20) {
        sprite->clut = func_8009ED74(0x10, 0x1EF);
    } else {
        sprite->clut = func_8009ED74(0, 0x1EF);
    }
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}

/* FAKE: pad keeps the observed frame extent (0x60) and the spill homes of g and b
 * (0x42, 0x41); the original locals there are unknown. Each sprite's UV stores share
 * a source line to preserve constant scheduling, as in shooter_draw_bomb_icon. T-25141. */
void shooter_draw_shield_gauge(void) {
    s32 pad[6];
    SPRT *sprite;
    u8 r;
    u8 g;
    u8 b;
    s32 value;
    u8 state;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    setWH(sprite, 0x20, 0x40);
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0xFF;
    sprite->g0 = 0xFF;
    sprite->b0 = 0xFF;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] - 0xC8;
    sprite->y0 = game_state.draw_origin_y[draw_buffer] + 0x28;
    sprite->u0 = 0x80; sprite->v0 = 0x40;
    sprite->clut = func_8009ED74(0x10, 0x1EF);
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    state = shooter_actors->stats[1];
    if (state > 0xC0) {
        r = 0;
        g = 0;
        b = 1;
    } else if (state > 0x80) {
        r = 0;
        g = 1;
        b = 0;
    } else if (state > 0x40) {
        r = 1;
        g = 1;
        b = 0;
    } else {
        r = 1;
        g = 0;
        b = 0;
    }
    value = (0x1000 - rsin(game_state.frame_count * 240)) / 128 + 0x40;
    r *= value;
    g *= value;
    b *= value;
    prim_gradient_rect(-0xBF, 0x68, 0xE, -shooter_actors->stats[1] >> 2, 2, (r << 16) + (g << 8) + b, 0x81);
    dtd_on_tpage(0, 0, 2, 1, 0);
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    setWH(sprite, 0x20, 0x20);
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] - 0xC8;
    sprite->y0 = ((0xFF - shooter_actors->stats[1]) >> 2) + game_state.draw_origin_y[draw_buffer] + 0x18;
    sprite->u0 = 0xA0; sprite->v0 = 0x60;
    sprite->clut = func_8009ED74(0x20, 0x1EF);
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    setWH(sprite, 0x28, 0x18);
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = game_state.draw_origin_x[draw_buffer] - 0xCE;
    sprite->y0 = game_state.draw_origin_y[draw_buffer] + 0x68;
    sprite->u0 = 0xA0; sprite->v0 = 0x40;
    sprite->clut = func_8009ED74(0x20, 0x1EF);
    AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}

void shooter_update_secondary_shots(void) {
    shooter_update_secondary_shots_of(0);
    shooter_update_secondary_shots_of(1);
}


void shooter_update_secondary_shots_of(s32 ship_index) {
    ShooterParticle *particle;
    ShooterParticle *base;
    s32 target;
    s32 pad; /* FAKE: preserve the original vector homes and frame. T-24502 */
    VECTOR offset;
    VECTOR velocity;
    ShooterActor *actor;
    ShooterActor *player;
    s32 i;

    if (ship_index == 0) {
        player = shooter_actors;
        base = shooter_player_secondary_shots;
    } else {
        base = shooter_wingman_secondary_shots;
        player = shooter_actors + 8;
    }
    i = 0;
    particle = base;
    do {
        particle->age++;
        if (particle->age == 0xFF) {
            particle->flags = 0;
        }
        if (particle->z < -0x7D00) {
            particle->flags = 0;
        }
        if (player->z + 0x400 < particle->z) {
            particle->flags = 0;
        }
        if (particle->flags & 0x80) {
            if ((particle->flags & 1) && particle->age >= 0x11) {
                if (!(game_state.frame_count & 3)) {
                    target = particle->argument;
                    if (target != -1) {
                        actor = shooter_actors + target;
                        if (actor->z < particle->z && (actor->flag_bytes[1] & 0x80) && (actor->stats[3] & 1)) {
                            offset.vx = actor->x - particle->x;
                            offset.vy = actor->y - particle->y;
                            offset.vz = 0;
                            func_800AA460(&offset, &velocity);
                            particle->velocity_x = (particle->velocity_x + velocity.vx / 64) / 2;
                            if (particle->velocity_x >= 0x41) {
                                particle->velocity_x = 0x40;
                            }
                            particle->velocity_y = (particle->velocity_y + velocity.vy / 64) / 2;
                            if (particle->velocity_y >= 0x41) {
                                particle->velocity_x = 0x40;
                            }
                        }
                    } else {
                        shooter_find_target(&target, *(ShooterPosition *)particle);
                        particle->argument = target;
                    }
                }
            } else if (particle->age >= 0x11 && particle->argument == 0) {
                particle->argument = 1;
                particle->age = 0;
                particle->velocity_z = -0x200;
                particle->velocity_x /= 16;
                particle->velocity_y /= 16;
            }
            particle->x += particle->velocity_x;
            particle->y += particle->velocity_y;
            particle->z += particle->velocity_z;
            shooter_draw_secondary_shot(base, i);
        }
        i++;
        particle++;
    } while (i != 16);
}


s32 shooter_fire_secondary_spread(s32 ship_index, s32 fire_flags) {
    /* FAKE: the unused k reserves the original's top frame home (i sits at 0x38, the frame is
     * 0x40). T-19014. */
    s32 k;
    s32 i;
    ShooterActor *actor;
    ShooterParticle *table;
    ShooterParticle *rec;

    if (ship_index == 0) {
        actor = shooter_actors;
        table = shooter_player_secondary_shots;
    } else {
        table = shooter_wingman_secondary_shots;
        actor = &shooter_actors[8];
    }
    if (fire_flags & 0x20) {
        i = 0;
        rec = table;
        for (; i < 16; i++) {
            if (!(rec->flags & 0x80)) {
                break;
            }
            rec++;
        }
        if (i >= 16) {
            return -1;
        }
        if (ship_index == 0) {
            shooter_play_se(1, 0x503);
        }
        rec->flags = 0x92;
        rec->x = actor->x;
        rec->y = actor->y - (i & 1) * 0x60 + 0x40;
        rec->z = actor->z;
        rec->velocity_x = rsin(actor->rotation_z) / 256;
        if ((i & 1) != 0) {
            rec->velocity_y = -func_800A0140(actor->rotation_z) / 256;
        } else {
            rec->velocity_y = func_800A0140(actor->rotation_z) / 256;
        }
        rec->velocity_z = -0x40;
        rec->age = 0;
        rec->argument = 0;
        return 1;
    }
    return 0;
}

s32 shooter_fire_secondary_homing(s32 ship_index, s32 fire_flags) {
    /* FAKE: the unused k and j reserve two of the original's frame homes (dist sits at 0x3C,
     * the frame is 0x48). T-19014. */
    s32 i;
    s32 k;
    s32 dist;
    ShooterActor *actor;
    ShooterParticle *table;
    s32 j;
    ShooterParticle *rec;

    if (ship_index == 0) {
        actor = shooter_actors;
        table = shooter_player_secondary_shots;
    } else {
        table = shooter_wingman_secondary_shots;
        actor = &shooter_actors[8];
    }
    if (fire_flags & 0x20) {
        rec = table;
        for (i = 0; i < 0x140; i += 0x14) {
            if (!(rec->flags & 0x80)) {
                break;
            }
            rec++;
        }
        if (i >= 0x140) {
            return -1;
        }
        if (ship_index == 0) {
            shooter_play_se(1, SHOOTER_SE_FIRE_SECONDARY);
        }
        rec->flags = 0x91;
        rec->x = actor->x;
        rec->y = actor->y + 0x40;
        rec->z = actor->z + 0x40;
        rec->velocity_x = 0;
        rec->velocity_y = 0;
        rec->velocity_z = -0x100;
        rec->age = 0;
        shooter_find_target(&dist, *(ShooterPosition *)rec);
        rec->argument = dist;
        return 1;
    }
    return 0;
}

s32 shooter_count_targets(s32 *target_actor_index_out) {
    /* FAKE: explicit groups and first count placement preserve the unrolled
       schedule; the original source loop is unknown. T-9560. */
    s32 actor_index;
    s32 target_count;
    ShooterActor *target_group;

    target_count = 0;
    *target_actor_index_out = -1;
    actor_index = 0x10;
    do {
        target_group = &shooter_actors[actor_index];
        if ((target_group[0].flag_bytes[1] & 0x80) && (target_group[0].stats[3] & 1) && (target_group[0].stats[2] != 0xE)) {
            if (*target_actor_index_out == -1) {
                *target_actor_index_out = actor_index;
                target_group = &shooter_actors[actor_index];
            }
            target_count += 1;
        }
        if ((target_group[1].flag_bytes[1] & 0x80) && (target_group[1].stats[3] & 1) && (target_group[1].stats[2] != 0xE)) {
            target_count += 1;
            if (*target_actor_index_out == -1) {
                *target_actor_index_out = actor_index + 1;
                target_group = &shooter_actors[actor_index];
            }
        }
        if ((target_group[2].flag_bytes[1] & 0x80) && (target_group[2].stats[3] & 1) && (target_group[2].stats[2] != 0xE)) {
            target_count += 1;
            if (*target_actor_index_out == -1) {
                *target_actor_index_out = actor_index + 2;
                target_group = &shooter_actors[actor_index];
            }
        }
        if ((target_group[3].flag_bytes[1] & 0x80) && (target_group[3].stats[3] & 1) && (target_group[3].stats[2] != 0xE)) {
            target_count += 1;
            if (*target_actor_index_out == -1) {
                *target_actor_index_out = actor_index + 3;
            }
        }
        actor_index += 4;
    } while (actor_index != 0x40);
    return target_count;
}

/* FAKE (T-19014): f1..f4 are unused locals that reserve four of the original's frame homes (dist sits
 * 0x14 below the frame top); the byte arithmetic `index * 0x88 + base` keeps the original's add order
 * (index first); the empty `if (count) {}` splits the live range of count. The real source is unknown.
 * dist is read uninitialised, as in the original (sp+0x24). */
s32 shooter_find_target(s32 *target_actor_index_out, ShooterPosition pos) {
    s32 f1;
    s32 f2;
    s32 f3;
    s32 f4;
    s32 dist;
    s32 count;
    s32 best;
    s32 start;
    s32 i;
    s32 idx;
    s32 j;
    ShooterActor *rec;

    *target_actor_index_out = -1;
    count = 0;
    best = 0x09C40000;
    start = game_state.frame_count % 48;
    for (i = 0; i <= 48; i++) {
        idx = (start + i) % 48;
        j = idx + 16;
        if (((ShooterActor *)((idx + 16) * 0x88 + (u8 *)shooter_actors))->flag_bytes[1] & 0x80) {
            rec = &shooter_actors[j];
            if ((rec->stats[3] & 1) && (((ShooterActor *)(i * 0x88 + (u8 *)shooter_actors))->stats[2] != 0xE)) {
                if (count) {}
                count++;
                if ((rec->z < pos.z) && (dist < best)) {
                    best = dist;
                    *target_actor_index_out = j;
                }
            }
        }
    }
    return count;
}

/* The four scalars before depth plus j and rot reproduce the original's stack slots. T-19014 */
void shooter_draw_secondary_shot(ShooterParticle *arg0, s32 arg1) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (200), as in shooter_draw_horizon_bands; T-19014 */
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    ShooterParticle *rec;
    POLY_FT4 *poly;
    s32 dist;
    s16 side;
    s32 depth;
    s32 flag;
    s32 o;
    s32 j;
    s32 rot;

    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    rec = &arg0[arg1];
    v.vz = rec->z;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = rec->y + rot - 0x10;
        for (j = 0; j < 2; j++) {
            v.vx = rec->x + j * 0x20 - 0x10;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot += 0x20;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= game_state.draw_origin_y[draw_buffer] - 0x200
            && game_state.draw_origin_y[draw_buffer] + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->age >> 4) & 1;
            dist = (D_80122754 - rec->z) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                return;
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            SetSemiTrans(poly, 0);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(1, 1, 0x3C0, 0x100);
            poly->clut = func_8009ED74(0x100, 0x1F8);
            /* FAKE: hand expansion of the PsyQ macro setUVWH(poly, side << 5, 0x60, 0x20, 0x20), which
             * include/libgpu.h lacks, on one line: as1 schedules the eight stores of one line together, and
             * the macro's store order gives the sum its register. T-19014 */
            poly->u0 = side << 5, poly->v0 = 0x60, poly->u1 = (side << 5) + 0x20, poly->v1 = 0x60, poly->u2 = side << 5, poly->v2 = 0x60 + 0x20, poly->u3 = (side << 5) + 0x20, poly->v3 = 0x60 + 0x20;
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
