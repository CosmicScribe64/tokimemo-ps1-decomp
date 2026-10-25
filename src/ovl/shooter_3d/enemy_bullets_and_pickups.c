/* enemy_bullets_and_pickups.c: src/ovl/TACO/8013DB80.c:5: enemy bullet allocation/movement/drawing and collectible pickup updates; body 15: h = speed; 16: if (h == 0) {. Original object 0x8013DB80-0x8013F250 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

/* FAKE: pad_a, pad_e and pad_c are unused locals; with div they reproduce the original frame (0x68: i at sp+0x60, vin at 0x4C, vout at 0x3C, h at 0x3A, the strength-reduced shooter_enemy_bullets[i] pointer spilled at 0x2C); real locals unknown. T-19028 */
s32 shooter_spawn_enemy_bullet(ShooterPosition pos, s32 speed, s32 flags) {
    s32 pad_a;
    s32 i;
    s32 pad_e;
    VECTOR vin;
    VECTOR vout;
    s16 h;
    s32 div;
    s32 pad_c;

    h = speed;
    if (h == 0) {
        h = 0x100;
    }
    for (i = 0; i < 16; i++) {
        if (!(shooter_enemy_bullets[i].flags & 0x80)) {
            break;
        }
    }
    if (i >= 16) {
        return -1;
    }
    shooter_enemy_bullets[i].age = 0;
    shooter_enemy_bullets[i].flags = 0x80;
    shooter_enemy_bullets[i].x = pos.x;
    shooter_enemy_bullets[i].y = pos.y;
    shooter_enemy_bullets[i].z = pos.z;
    if (flags & 0x80) {
        vin.vx = shooter_actors->x - pos.x;
        vin.vy = shooter_actors->y - pos.y;
        vin.vz = shooter_actors->z - pos.z;
        if (flags & 0x40) {
            vin.vx = vin.vx - (rand() & 0xFF) + 0x7F;
            vin.vy = vin.vy - (rand() & 0xFF) + 0x7F;
            vin.vz = vin.vz - (rand() & 0xFF) + 0x7F;
        }
        func_800AA460(&vin, &vout);
        div = 0x1000 / h + 1;
        shooter_enemy_bullets[i].velocity_x = vout.vx / div;
        shooter_enemy_bullets[i].velocity_y = vout.vy / div;
        shooter_enemy_bullets[i].velocity_z = vout.vz / div;
    } else {
        shooter_enemy_bullets[i].velocity_z = h;
        shooter_enemy_bullets[i].velocity_x = 0;
        shooter_enemy_bullets[i].velocity_y = 0;
    }
    shooter_enemy_bullets[i].argument = flags;
    return i;
}

void shooter_update_enemy_bullets(void) {
    ShooterParticle *p;
    s32 i;

    /* FAKE: init and `do` share a source line (as1 schedules by line); the second base is spelled from the
     * end label so that IDO does not share the first loop's address. Same bytes as the original. T-10320 */
    p = shooter_enemy_bullets; do {
        p->age = p->age + 1;
        if (p->age == 0xFF) {
            p->flags = 0;
        }
        if (p->z >= 0x2711) {
            p->flags = 0;
        }
        if (p->flags & 0x80) {
            p->x = p->x + p->velocity_x;
            p->y = p->y + p->velocity_y;
            p->z = p->z + p->velocity_z;
        }
        p++;
    } while (p < shooter_items);
    p = (ShooterParticle *)((u8 *)shooter_items - 0x140);
    i = 0;
    do {
        if (p->flags & 0x80) {
            shooter_draw_enemy_bullet(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}

s32 shooter_draw_enemy_bullet(s32 arg0) {
    /* FAKE: pad is an unused local; it holds the 4-byte slot that puts the original frame at 0xD8 (m at
     * sp+0xB0, kind at 0x80, v at 0x6C) and the colour switch temporary at 0x68. T-25110 */
    s32 pad;
    POLY_FT4 *poly;
    MATRIX m;
    SVECTOR w;
    s32 pt[4];
    s32 dist;
    s32 o;
    s32 depth;
    s32 flag;
    s32 j;
    s32 kind;
    s32 mode;
    s32 c;
    s32 step;
    SVECTOR v;

    v = *(SVECTOR *)&shooter_enemy_bullets[arg0];
    kind = shooter_enemy_bullets[arg0].argument & 0xF;
    m = D_80129C40;
    v.vz -= 0x200;
    func_800A1100(&m);
    SetRotMatrix(&m);
    switch (kind) {
    case 3:
    case 5:
        step = 0x100;
        break;
    case 0:
        step = 0x40;
        break;
    default:
    case 1:
    case 2:
    case 4:
        step = 0x80;
        break;
    }
    mode = kind;
    w.vz = v.vz;
    for (o = 0; o < 2; o++) {
        w.vy = v.vy + o * step - step / 2;
        for (j = 0; j < 2; j++) {
            w.vx = v.vx + j * step - step / 2;
            func_800AB170(&w, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= game_state.draw_origin_y[draw_buffer] - 0x200
            && game_state.draw_origin_y[draw_buffer] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - v.vz) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                return -1;
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            switch (mode) {
            case 5:
                c = 0xFF;
                break;
            case 6:
                c = 0xFF;
                break;
            default:
                c = 0x80;
                break;
            }
            poly->r0 = c;
            poly->g0 = c;
            poly->b0 = c;
            switch (kind) {
            case 3:
            case 5:
            case 6:
                SetSemiTrans(poly, 1);
                break;
            default:
            case 0:
            case 1:
            case 2:
            case 4:
                SetSemiTrans(poly, 0);
                break;
            }
            if (mode != 4) {
                poly->tpage = func_8009ECB0(1, 1, 0x3C0, 0x100);
            } else {
                poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            }
            if (mode != 4) {
                if (mode == 5) {
                    poly->clut = func_8009ED74(0, 0x1EA);
                } else {
                    poly->clut = func_8009ED74(0x100, 0x1F8);
                }
            } else {
                poly->clut = func_8009ED74(0, 0x1E5);
            }
            switch (kind) {
            default:
            case 0:
            case 1:
                poly->u0 = 1, poly->v0 = 0x41, poly->u1 = 0x1F, poly->v1 = 0x41, poly->u2 = 1, poly->v2 = 0x5F, poly->u3 = 0x1F, poly->v3 = 0x5F;
                break;
            case 2:
                poly->u0 = 0x41, poly->v0 = 1, poly->u1 = 0x7F, poly->v1 = 1, poly->u2 = 0x41, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
                break;
            case 3:
                poly->u0 = 1, poly->v0 = 1, poly->u1 = 0x3F, poly->v1 = 1, poly->u2 = 1, poly->v2 = 0x3F, poly->u3 = 0x3F, poly->v3 = 0x3F;
                break;
            case 4:
                poly->u0 = 0x40, poly->v0 = 0x80, poly->u1 = 0x7F, poly->v1 = 0x80, poly->u2 = 0x40, poly->v2 = 0xBF, poly->u3 = 0x7F, poly->v3 = 0xBF;
                break;
            }
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
        }
    }
}

s32 shooter_spawn_item(pos, kind)
ShooterPosition pos;
s32 kind;
{
    s32 pad3;
    s32 i;
    /* FAKE: 48 unused bytes around i reproduce the original frame (0x60, i at sp+0x58, the strength-reduced
     * shooter_items[i] pointer spilled at sp+0x28); real locals unknown. T-19028 */
    ShooterParticle pad0;
    ShooterParticle pad1;
    s32 pad2;

    for (i = 0; i < 16; i++) {
        if (!(shooter_items[i].flags & 0x80)) {
            break;
        }
    }
    if (i >= 16) {
        return -1;
    }
    shooter_items[i].age = 0;
    shooter_items[i].flags = 0x80;
    shooter_items[i].x = pos.x;
    shooter_items[i].y = pos.y;
    shooter_items[i].z = pos.z;
    shooter_items[i].velocity_x = 0x3F - (rand() & 0x7F);
    shooter_items[i].velocity_y = 0x3F - (rand() & 0x7F);
    shooter_items[i].velocity_z = 0x50;
    shooter_items[i].argument = kind;
    return i;
}

void shooter_update_items(void) {
    ShooterParticle *p;
    s32 i;

    /* FAKE: the loop entry is written on one source line; as1 schedules by line and this orders the
     * end-pointer and base setup as the original does. T-10320 */
    p = shooter_items; do { p->age += 1; if (p->age == 0xFF) { p->age = 0;
            p->flags += 1;
        }
        if (p->flags == 0x85) {
            p->flags = 0;
        }
        if (p->z >= 0x2711) {
            p->flags = 0;
        }
        if (p->flags & 0x80) {
            p->x += p->velocity_x;
            p->y += p->velocity_y;
            p->z += p->velocity_z;
            if (p->velocity_x > 0) {
                p->velocity_x -= 1;
            }
            if (p->velocity_y > 0) {
                p->velocity_y -= 1;
            }
            if (p->velocity_x < 0) {
                p->velocity_x += 1;
            }
            if (p->velocity_y < 0) {
                p->velocity_y += 1;
            }
            if (!(p->flags & 4)) {
                if (p->x > 0x800) {
                    p->x = 0x800;
                }
                if (p->x < -0x800) {
                    p->x = -0x800;
                }
                if (p->y > 0x80) {
                    p->y = 0x80;
                }
                if (p->y < -0x700) {
                    p->y = -0x700;
                }
                if (p->z > shooter_actors->z + 0x10) {
                    if (p->velocity_z > 0) {
                        p->velocity_z = p->velocity_z * -1;
                    }
                }
                if (p->z < -0x1000) {
                    if (p->velocity_z < 0) {
                        p->velocity_z = p->velocity_z * -1;
                    }
                }
            }
        }
        p++;
    } while (p < (ShooterParticle *)&shooter_background_flags);
    /* FAKE: the base is spelled from the end label so that IDO does not share the first loop's address; same bytes. T-10320 */
    p = (ShooterParticle *)((u8 *)&shooter_background_flags - 0x140);
    i = 0;
    do {
        if (p->flags & 0x80) {
            if (p->argument < 2) {
                shooter_draw_item_weapon(i);
            } else {
                shooter_draw_item(i);
            }
        }
        i++;
        p++;
    } while (i != 0x10);
}

void shooter_draw_item_weapon(s32 arg0) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (208), as in shooter_draw_hit_spark; T-19028 */
    MATRIX m;
    SVECTOR w;
    s32 pt[4];
    SVECTOR v;
    POLY_FT4 *poly;
    s32 dist;
    s32 depth;
    s32 flag;
    s32 o;
    s32 kind;
    s32 sel;
    s32 j;
    s32 rot;

    m = D_80129C40;
    v = *(SVECTOR *)&shooter_items[arg0];
    v.vz -= 0x200;
    kind = shooter_items[arg0].argument;
    sel = (shooter_items[arg0].age >> 2) & 7;
    func_800A1100(&m);
    SetRotMatrix(&m);
    w.vz = v.vz;
    for (rot = 0, o = 0; o < 2; o++) {
        w.vy = v.vy + rot - 0x40;
        for (j = 0; j < 2; j++) {
            w.vx = v.vx + j * 0x80 - 0x40;
            func_800AB170(&w, &pt[j * 2 + o], &depth, &flag);
        }
        rot += 0x80;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= game_state.draw_origin_y[draw_buffer] - 0x200
            && game_state.draw_origin_y[draw_buffer] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - v.vz) / 127;
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
            poly->tpage = func_8009ECB0(1, 1, 0x2C0, 0x100);
            poly->clut = func_8009ED74(0, kind + 0x1E2);
            /* FAKE: hand expansion of a PsyQ setUVWH-style macro on one line, as in shooter_draw_hit_spark: as1 schedules the
             * eight UV stores of one line together; separate statements change their order. T-19028 */
            poly->u0 = 0x61, poly->v0 = (sel << 5) + 1, poly->u1 = 0x7F, poly->v1 = (sel << 5) + 1, poly->u2 = 0x61, poly->v2 = (sel << 5) + 0x1F, poly->u3 = 0x7F, poly->v3 = (sel << 5) + 0x1F;
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

void shooter_draw_item(s32 arg0) {
    /* FAKE: pad and pad_a are unused; they hold the original frame (0x110: matrix at sp+0xE8, v at 0xC0, pt
     * at 0xB0, pos at 0xA8, depth/flag at 0x94/0x90, kind/phase at 0x88/0x84, height at 0x74, uopt spill
     * homes from 0x6C down). Real locals unknown. T-25110, T-25201 */
    ShooterParticle *particle;
    POLY_FT4 *poly;
    MATRIX matrix;
    s32 pad[8];
    SVECTOR v;
    s32 pt[4];
    ShooterPosition pos;
    s32 dist;
    s32 o;
    s32 j;
    s32 pad_a;
    s32 depth;
    s32 flag;
    s32 half;
    s32 kind;
    u32 phase;
    s32 row;
    s32 size;
    s32 clut;
    s32 height;
    u32 frame;

    matrix = D_80129C40;
    particle = &shooter_items[arg0];
    pos = *(ShooterPosition *)particle;
    pos.z -= 0x200;
    kind = particle->argument & 7;
    phase = particle->age >> 1;
    func_800A1100(&matrix);
    SetRotMatrix(&matrix);
    v.vz = pos.z;
    for (o = 0; o < 2; o++) {
        v.vy = pos.y + o * 0x80 - 0x40;
        for (j = 0; j < 2; j++) {
            v.vx = pos.x + j * 0x80 - 0x40;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= game_state.draw_origin_y[draw_buffer] - 0x200 && game_state.draw_origin_y[draw_buffer] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - pos.z) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            SetSemiTrans(poly, 0);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            if (shooter_item_frame_count[kind] != 0) {
                frame = phase % shooter_item_frame_count[kind];
            } else {
                frame = 0;
            }
            row = shooter_item_clut_row[kind];
            poly->clut = func_8009ED74(frame * 0x10, row + 0x1E0);
            poly->u0 = shooter_item_u[kind] + 1;
            poly->v0 = shooter_item_v[kind] + 1;
            poly->u1 = shooter_item_u[kind] + 0xF;
            poly->v1 = shooter_item_v[kind] + 1;
            poly->u2 = shooter_item_u[kind] + 1;
            poly->v2 = shooter_item_v[kind] + 0xF;
            poly->u3 = shooter_item_u[kind] + 0xF;
            poly->v3 = shooter_item_v[kind] + 0xF;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
            height = (rsin(particle->age * 0x10) << 7) / 4096;
            /* FAKE: half is a constant local that only steers uopt (real source unknown). It is propagated
             * into its uses (its slot stays), so the third grid's o * 0x100 becomes a later item than
             * j * size * 2, which takes the first spill home (sp+0x64), and the vertical temporary gets
             * sp+0x58. T-25201 */
            half = 0x80;
            v.vz = pos.z;
            for (o = 0; o < 2; o++) {
                v.vy = pos.y + o * height * 2 - height;
                for (j = 0; j < 2; j++) {
                    v.vx = pos.x + j * half * 2 - half;
                    func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
                }
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            clut = ((s32)phase % 14) * 0x10;
            poly->clut = func_8009ED74(clut, 0x1E7);
            poly->u0 = 0x61, poly->v0 = 0x21, poly->u1 = 0x7F, poly->v1 = 0x21, poly->u2 = 0x61, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
            size = (func_800A0140(particle->age * 0x10) << 7) / 4096;
            v.vz = pos.z;
            for (o = 0; o < 2; o++) {
                v.vy = pos.y + o * half * 2 - half;
                for (j = 0; j < 2; j++) {
                    v.vx = pos.x + j * size * 2 - size;
                    func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
                }
            }
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(clut, 0x1E7);
            poly->u0 = 0x61, poly->v0 = 0x21, poly->u1 = 0x7F, poly->v1 = 0x21, poly->u2 = 0x61, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((draw_buffer << 10) + dist * 4 + draw_ot_tags, poly);
        }
    }
}
