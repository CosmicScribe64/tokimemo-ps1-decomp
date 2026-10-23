#include "common.h"
#include "ovl/TACO.h"

void func_8013F250(void) {
    /* FAKE: preserve the observed local homes and frame extent. T-10200. */
    s32 pad[2];
    s32 sp34;
    s32 sp30;
    s32 sp2C;

    sp34 = (s16) D_8015EDEC / 3600;
    sp30 = (s32) ((s16) D_8015EDEC % 3600) / 60;
    sp2C = (s32) (((s16) D_8015EDEC % 60) * 0x3C) / 100;
    func_801432F0((s32) "SCORE", -0xE0, -0x64, 0);
    func_801430D0(D_8015EDF0 + (D_8015EDCC * 0xF4240), -0x50, -0x64, 0);
    func_801432F0((s32) "TIME", 0x40, -0x64, 0);
    func_801430D0(sp34, 0x90, -0x64, 1);
    func_801430D0(sp30, 0xB0, -0x64, 2);
    func_801430D0(sp2C, 0xD0, -0x64, 3);
    func_8013F430();
    /* This counter uses an unsigned byte load; other TACO users have a signed view. */
    if (*(u8 *)&D_8015EDD0 != 0) {
        (*(u8 *)&D_8015EDD0)--;
        func_801430D0(D_8015EDD4, 0x14, 0x64, 3);
    }
    func_80133738();
}

void func_8013F430(void) {
    func_8013F468();
    func_8013F7D8();
    func_8013F9C0();
    func_8013FBF0();
}

void func_8013F468(void) {
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
    func_800A1070(&matrix);
    v.vz = -0x7000;
    /* FAKE: one source line preserves pointer/counter initialization order. T-24502 */
    y = 0; for (o = 0; o != 2; o++) {
        v.vy = D_8015EDB4->unk76 + y;
        for (j = 0; j < 2; j++) {
            v.vx = D_8015EDB4->unk74 + j * 0x200 - 0x100;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        y += 0x200;
    }
    poly = GetWorkBase(0x28, D_8011ECA0);
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
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 4, poly);
    if (D_800E6280.unk_10F8 & 8) {
        poly = GetWorkBase(0x28, D_8011ECA0);
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
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 4, poly);
    }
}


void func_8013F7D8(void) {
    s16 v;

    v = D_8015EDB4->unk7C;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xB4, 0x60, v);
    }
    v = D_8015EDB4->unk7E;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xC8, 0x60, v);
    }
    v = D_8015EDB4->unk80;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xDC, 0x60, v);
    }
}

/* FAKE: the UV stores share a source line to preserve constant scheduling.
 * T-9560; original statement layout unknown. */
void func_8013F874(s32 arg0, s32 arg1, s32 arg2) {
    s32 palette;
    SPRT *sprite;

    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 16;
    sprite->h = 16;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = D_800E6280.unk_014[D_8011ECA0] + arg0;
    sprite->y0 = D_800E6280.unk_018[D_8011ECA0] + arg1;
    sprite->u0 = 0xC0; sprite->v0 = arg2 * 16 - 0x30;
    palette = ((D_800E6280.unk_10F8 >> 1) % 14) * 16;
    if (arg2 == 3) {
        sprite->clut = func_8009ED74(palette, 0x1EA);
    } else {
        sprite->clut = func_8009ED74(palette, 0x1E9);
    }
    AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, (s32)sprite);
    dtd_on_tpage(0x380, 0x100, 1, 0, 0);
}

/* FAKE: the second sprite's UV stores share a source line to preserve constant scheduling.
 * T-19014; original statement layout unknown. */
void func_8013F9C0(void) {
    SPRT *sprite;
    u32 dy;

    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 0x20;
    sprite->h = 0x20;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    dy = (D_8015EDB4->unk2 & 0x40) >> 2;
    sprite->x0 = D_800E6280.unk_014[D_8011ECA0] + 0xC8;
    sprite->y0 = D_800E6280.unk_018[D_8011ECA0] + dy + 0x28;
    sprite->u0 = 0xE0;
    sprite->v0 = 0;
    if (D_8015EDB4->unk84[1] < 0x20) {
        sprite->clut = func_8009ED74(0x10, 0x1EF);
    } else {
        sprite->clut = func_8009ED74(0, 0x1EF);
    }
    AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, (s32)sprite);
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 0x20;
    sprite->h = 0x20;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = D_800E6280.unk_014[D_8011ECA0] + 0xC8;
    sprite->y0 = D_800E6280.unk_018[D_8011ECA0] + 0x48;
    sprite->u0 = 0xE0; sprite->v0 = 0x20;
    if (D_8015EDB4->unk84[1] < 0x20) {
        sprite->clut = func_8009ED74(0x10, 0x1EF);
    } else {
        sprite->clut = func_8009ED74(0, 0x1EF);
    }
    AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, (s32)sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_8013FBF0);

void func_80140000(void) {
    func_80140028(0);
    func_80140028(1);
}


void func_80140028(s32 arg0) {
    Tc14 *particle;
    Tc14 *base;
    s32 target;
    s32 pad; /* FAKE: preserve the original vector homes and frame. T-24502 */
    VECTOR offset;
    VECTOR velocity;
    TcActor *actor;
    TcActor *player;
    s32 i;

    if (arg0 == 0) {
        player = D_8015EDB4;
        base = D_8015E4F0;
    } else {
        base = D_8015E630;
        player = D_8015EDB4 + 8;
    }
    i = 0;
    particle = base;
    do {
        particle->unk12++;
        if (particle->unk12 == 0xFF) {
            particle->unk13 = 0;
        }
        if (particle->unk4 < -0x7D00) {
            particle->unk13 = 0;
        }
        if (player->unk78 + 0x400 < particle->unk4) {
            particle->unk13 = 0;
        }
        if (particle->unk13 & 0x80) {
            if ((particle->unk13 & 1) && particle->unk12 >= 0x11) {
                if (!(D_800E6280.unk_10F8 & 3)) {
                    target = particle->unk11;
                    if (target != -1) {
                        actor = D_8015EDB4 + target;
                        if (actor->unk78 < particle->unk4 && (actor->unk0[1] & 0x80) && (actor->unk84[3] & 1)) {
                            offset.vx = actor->unk74 - particle->unk0;
                            offset.vy = actor->unk76 - particle->unk2;
                            offset.vz = 0;
                            func_800AA460(&offset, &velocity);
                            particle->unk8 = (particle->unk8 + velocity.vx / 64) / 2;
                            if (particle->unk8 >= 0x41) {
                                particle->unk8 = 0x40;
                            }
                            particle->unkA = (particle->unkA + velocity.vy / 64) / 2;
                            if (particle->unkA >= 0x41) {
                                particle->unk8 = 0x40;
                            }
                        }
                    } else {
                        func_801407D4(&target, *(TcPos *)particle);
                        particle->unk11 = target;
                    }
                }
            } else if (particle->unk12 >= 0x11 && particle->unk11 == 0) {
                particle->unk11 = 1;
                particle->unk12 = 0;
                particle->unkC = -0x200;
                particle->unk8 /= 16;
                particle->unkA /= 16;
            }
            particle->unk0 += particle->unk8;
            particle->unk2 += particle->unkA;
            particle->unk4 += particle->unkC;
            func_80140ADC(base, i);
        }
        i++;
        particle++;
    } while (i != 16);
}


s32 func_80140364(s32 arg0, s32 arg1) {
    /* FAKE: the unused k reserves the original's top frame home (i sits at 0x38, the frame is
     * 0x40). T-19014. */
    s32 k;
    s32 i;
    TcActor *actor;
    Tc14 *table;
    Tc14 *rec;

    if (arg0 == 0) {
        actor = D_8015EDB4;
        table = D_8015E4F0;
    } else {
        table = D_8015E630;
        actor = &D_8015EDB4[8];
    }
    if (arg1 & 0x20) {
        i = 0;
        rec = table;
        for (; i < 16; i++) {
            if (!(rec->unk13 & 0x80)) {
                break;
            }
            rec++;
        }
        if (i >= 16) {
            return -1;
        }
        if (arg0 == 0) {
            func_801335A0(1, 0x503);
        }
        rec->unk13 = 0x92;
        rec->unk0 = actor->unk74;
        rec->unk2 = actor->unk76 - (i & 1) * 0x60 + 0x40;
        rec->unk4 = actor->unk78;
        rec->unk8 = func_800A0070(actor->unk68) / 256;
        if ((i & 1) != 0) {
            rec->unkA = -func_800A0140(actor->unk68) / 256;
        } else {
            rec->unkA = func_800A0140(actor->unk68) / 256;
        }
        rec->unkC = -0x40;
        rec->unk12 = 0;
        rec->unk11 = 0;
        return 1;
    }
    return 0;
}

s32 func_801404FC(s32 arg0, s32 arg1) {
    /* FAKE: the unused k and j reserve two of the original's frame homes (dist sits at 0x3C,
     * the frame is 0x48). T-19014. */
    s32 i;
    s32 k;
    s32 dist;
    TcActor *actor;
    Tc14 *table;
    s32 j;
    Tc14 *rec;

    if (arg0 == 0) {
        actor = D_8015EDB4;
        table = D_8015E4F0;
    } else {
        table = D_8015E630;
        actor = &D_8015EDB4[8];
    }
    if (arg1 & 0x20) {
        rec = table;
        for (i = 0; i < 0x140; i += 0x14) {
            if (!(rec->unk13 & 0x80)) {
                break;
            }
            rec++;
        }
        if (i >= 0x140) {
            return -1;
        }
        if (arg0 == 0) {
            func_801335A0(1, 0x503);
        }
        rec->unk13 = 0x91;
        rec->unk0 = actor->unk74;
        rec->unk2 = actor->unk76 + 0x40;
        rec->unk4 = actor->unk78 + 0x40;
        rec->unk8 = 0;
        rec->unkA = 0;
        rec->unkC = -0x100;
        rec->unk12 = 0;
        func_801407D4(&dist, *(TcPos *)rec);
        rec->unk11 = dist;
        return 1;
    }
    return 0;
}

s32 func_80140638(s32 *arg0) {
    /* FAKE: explicit groups and first count placement preserve the unrolled
       schedule; the original source loop is unknown. T-9560. */
    s32 var_v0;
    s32 var_v1;
    TcActor *var_a2;

    var_v1 = 0;
    *arg0 = -1;
    var_v0 = 0x10;
    do {
        var_a2 = &D_8015EDB4[var_v0];
        if ((var_a2[0].unk0[1] & 0x80) && (var_a2[0].unk84[3] & 1) && (var_a2[0].unk84[2] != 0xE)) {
            if (*arg0 == -1) {
                *arg0 = var_v0;
                var_a2 = &D_8015EDB4[var_v0];
            }
            var_v1 += 1;
        }
        if ((var_a2[1].unk0[1] & 0x80) && (var_a2[1].unk84[3] & 1) && (var_a2[1].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 1;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[2].unk0[1] & 0x80) && (var_a2[2].unk84[3] & 1) && (var_a2[2].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 2;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[3].unk0[1] & 0x80) && (var_a2[3].unk84[3] & 1) && (var_a2[3].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 3;
            }
        }
        var_v0 += 4;
    } while (var_v0 != 0x40);
    return var_v1;
}

/* FAKE (T-19014): f1..f4 are unused locals that reserve four of the original's frame homes (dist sits
 * 0x14 below the frame top); the byte arithmetic `index * 0x88 + base` keeps the original's add order
 * (index first); the empty `if (count) {}` splits the live range of count. The real source is unknown.
 * dist is read uninitialised, as in the original (sp+0x24). */
s32 func_801407D4(s32 *arg0, TcPos pos) {
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
    TcActor *rec;

    *arg0 = -1;
    count = 0;
    best = 0x09C40000;
    start = D_800E6280.unk_10F8 % 48;
    for (i = 0; i <= 48; i++) {
        idx = (start + i) % 48;
        j = idx + 16;
        if (((TcActor *)((idx + 16) * 0x88 + (u8 *)D_8015EDB4))->unk0[1] & 0x80) {
            rec = &D_8015EDB4[j];
            if ((rec->unk84[3] & 1) && (((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[2] != 0xE)) {
                if (count) {}
                count++;
                if ((rec->unk78 < pos.z) && (dist < best)) {
                    best = dist;
                    *arg0 = j;
                }
            }
        }
    }
    return count;
}

/* The four scalars before depth plus j and rot reproduce the original's stack slots. T-19014 */
void func_80140ADC(Tc14 *arg0, s32 arg1) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (200), as in func_80144430; T-19014 */
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    Tc14 *rec;
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
    func_800A1070(&m);
    rec = &arg0[arg1];
    v.vz = rec->unk4;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = rec->unk2 + rot - 0x10;
        for (j = 0; j < 2; j++) {
            v.vx = rec->unk0 + j * 0x20 - 0x10;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot += 0x20;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->unk12 >> 4) & 1;
            dist = (D_80122754 - rec->unk4) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                return;
            }
            poly = func_800490F0(0x28, D_8011ECA0);
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
            func_8009EED0((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}
