#include "common.h"
#include "ovl/TACO.h"

void func_80140DF0(void) {
    func_80140E18(0);
    func_80140E18(1);
}

void func_80140E18(s32 arg0) {
    Tc14 *base;
    Tc14 *p;
    TcActor *act;
    s32 i;

    if (arg0 == 0) {
        act = D_8015EDB4;
        base = D_8015E270;
    } else {
        base = D_8015E3B0;
        act = &D_8015EDB4[8];
    }
    for (i = 0, p = base; i < 16; i++, p++) {
        p->unk12 += 1;
        if (p->unk12 == 0xFF) {
            p->unk13 = 0;
        }
        if (p->unk4 < -0x7D00) {
            p->unk13 = 0;
        }
        switch (p->unk13 & 0x2F) {
        case 4:
            if (p->unk13 & 0x80) {
                p->unk0 += p->unk8;
                p->unk2 += p->unkA;
                p->unk4 += p->unkC;
                func_801413B8(base, i);
            }
            break;
        case 8:
            if (p->unk13 & 0x80) {
                p->unk0 += p->unk8;
                p->unk2 += p->unkA;
                p->unk4 += p->unkC;
                func_801416E8(base, i);
            }
            break;
        }
    }
    if ((base->unk13 & 0xA0) == 0xA0) {
        base->unk12 += 1;
        if (base->unk12 >= 3U) {
            base[0].unk0 = act->unk74 + 0x40;
            base[0].unk2 = act->unk76 + 0x20;
            base[0].unk4 = act->unk78;
            base[1].unk0 = act->unk74 - 0x40;
            base[1].unk2 = act->unk76 + 0x20;
            base[1].unk4 = act->unk78;
            func_80141B18(base, arg0);
        }
        if (base->unk12 >= 0x81U) {
            base->unk13 = 0;
            base[1].unk13 = 0;
        }
    }
}

#ifdef NON_MATCHING
s32 func_80141050(s32 arg0, s32 arg1) {
    s32 ret;
    s32 i;
    TcActor *act;
    Tc14 *base;
    Tc14 *p;

    if (arg0 == 0) {
        act = D_8015EDB4;
        base = D_8015E270;
    } else {
        base = D_8015E3B0;
        act = D_8015EDB4 + 8;
    }
    ret = 0;
    if (arg1 & 0x20) {
        for (i = 0, p = base; i < 16; i++, p++) {
            if (!(p->unk13 & 0x80)) {
                break;
            }
        }
        if (i >= 16) {
            return -1;
        }
        if (arg0 == 0) {
            func_801335A0(1, 0x500);
        }
        p->unk13 = 0x94;
        p->unk0 = act->unk74 + ((i & 1) << 7) - 0x40;
        p->unk2 = act->unk76 - (arg0 << 7) + 0x80;
        p->unk8 = 0;
        p->unkA = 0;
        p->unkC = -0x200;
        p->unk12 = 0;
        p->unk4 = act->unk78;
        ret = 1;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141050);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141170);

s32 func_80141290(s32 arg0, s32 arg1) {
    Tc14 *p;
    s32 free_count;
    s32 offset;
    TcActor *act;
    Tc14 *base;

    if (arg0 == 0) {
        act = D_8015EDB4;
        base = D_8015E270;
    } else {
        base = D_8015E3B0;
        act = D_8015EDB4 + 8;
    }
    free_count = 0;
    if (arg1 & 0x20) {
        offset = 0;
        p = base;
        do {
            offset += 0x14;
            if (!(p->unk13 & 0x80)) {
                free_count += 1;
            }
            p += 1;
        } while (offset != 0x140);
        if (free_count != 0x10) {
            return -1;
        }
        if (arg0 == 0) {
            func_801335A0(1, 0x501);
        }
        base->unk13 = 0xB0;
        base->unk0 = act->unk74;
        base->unk2 = act->unk76 + 0x20;
        base->unk4 = act->unk78;
        base[1].unk0 = (s16) act->unk74;
        base[1].unk2 = (s16) (act->unk76 + 0x20);
        base[1].unk4 = (s16) act->unk78;
        base->unk8 = 0;
        base->unkA = 0;
        base->unkC = 0;
        base->unk12 = 0;
        return 1;
    }
    return 0;
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-19017 */
void func_801413B8(Tc14 *arg0, s32 arg1) {
    s32 pad[2];
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    Tc14 *rec;
    POLY_FT4 *poly;
    s32 dist;
    s32 side;
    s32 depth;
    s32 flag;
    s32 o;
    s32 j;
    u8 tbl[2];
    s32 rot;
    s32 k;

    *(u16 *)tbl = D_8015F580;
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    rec = &arg0[arg1];
    v.vz = rec->unk4;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = rec->unk2 + rot - 0x28;
        for (j = 0; j < 2; j++) {
            v.vx = rec->unk0 + j * 0x50 - 0x28;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot += 0x50;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->unk12 >> 2) & 1;
            dist = (D_80122754 - rec->unk4) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(0x80, 0x1E4);
            poly->u0 = tbl[side], poly->v0 = 1, poly->u1 = tbl[side] + 0xE, poly->v1 = 1, poly->u2 = tbl[side], poly->v2 = 1 + 0xE, poly->u3 = tbl[side] + 0xE, poly->v3 = 1 + 0xE;
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
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_801413B8);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_801416E8);

/* FAKE: pad, k1, k7, k8, k2, t, k3 and k4 are unused locals; they reproduce the original frame (224 bytes: eight above m,
 * three words below pt, the homes of depth, flag and fade). Real source unknown. T-19017 */
void func_80141B18(Tc14 *arg0, s32 arg1) {
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

    if (arg0->unk12 > 0x80) {
        fade = 0xFF - arg0->unk12;
    } else {
        fade = arg0->unk12;
    }
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    sgn = 1 - arg1;
    j = 0;
    do {
        v.vz = arg0[j].unk4 + (sgn << 11);
        v.vy = arg0[j].unk2 + (sgn << 8);
        v.vx = arg0[j].unk0;
        func_800AB170(&v, &pt[j * 4], &depth, &flag);
        v.vz = -0x7FFF;
        v.vy = arg0[j].unk2 + 0x100;
        v.vx = arg0[j].unk0;
        func_800AB170(&v, &pt[j * 4 + 2], &depth, &flag);
        j++;
    } while (&pt[j * 4 + 2] < &v.vz);
    poly = func_800490F0(0x24, D_8011ECA0);
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
    AddPrim((D_8011ECA0 << 10) + 0x10 + D_800E8CA0, poly);
    dtd_on_tpage(0x340, 0x100, 4, 1, 0);
}
