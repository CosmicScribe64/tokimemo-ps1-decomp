#include "common.h"
#include "ovl/TACO.h"

/* FAKE: pad_a, pad_e and pad_c are unused locals; with div they reproduce the original frame (0x68: i at sp+0x60, vin at 0x4C, vout at 0x3C, h at 0x3A, the strength-reduced D_8015EB30[i] pointer spilled at 0x2C); real locals unknown. T-19028 */
s32 func_8013DB80(TcPos pos, s32 speed, s32 flags) {
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
        if (!(D_8015EB30[i].unk13 & 0x80)) {
            break;
        }
    }
    if (i >= 16) {
        return -1;
    }
    D_8015EB30[i].unk12 = 0;
    D_8015EB30[i].unk13 = 0x80;
    D_8015EB30[i].unk0 = pos.x;
    D_8015EB30[i].unk2 = pos.y;
    D_8015EB30[i].unk4 = pos.z;
    if (flags & 0x80) {
        vin.vx = D_8015EDB4->unk74 - pos.x;
        vin.vy = D_8015EDB4->unk76 - pos.y;
        vin.vz = D_8015EDB4->unk78 - pos.z;
        if (flags & 0x40) {
            vin.vx = vin.vx - (func_800AE0D0() & 0xFF) + 0x7F;
            vin.vy = vin.vy - (func_800AE0D0() & 0xFF) + 0x7F;
            vin.vz = vin.vz - (func_800AE0D0() & 0xFF) + 0x7F;
        }
        func_800AA460(&vin, &vout);
        div = 0x1000 / h + 1;
        D_8015EB30[i].unk8 = vout.vx / div;
        D_8015EB30[i].unkA = vout.vy / div;
        D_8015EB30[i].unkC = vout.vz / div;
    } else {
        D_8015EB30[i].unkC = h;
        D_8015EB30[i].unk8 = 0;
        D_8015EB30[i].unkA = 0;
    }
    D_8015EB30[i].unk11 = flags;
    return i;
}

void func_8013DDE4(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: init and `do` share a source line (as1 schedules by line); the second base is spelled from the
     * end label so that IDO does not share the first loop's address. Same bytes as the original. T-10320 */
    p = D_8015EB30; do {
        p->unk12 = p->unk12 + 1;
        if (p->unk12 == 0xFF) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 = p->unk0 + p->unk8;
            p->unk2 = p->unk2 + p->unkA;
            p->unk4 = p->unk4 + p->unkC;
        }
        p++;
    } while (p < D_8015EC70);
    p = (Tc14 *)((u8 *)D_8015EC70 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            func_8013DEE4(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DEE4);

s32 func_8013E424(pos, kind)
TcPos pos;
s32 kind;
{
    s32 pad3;
    s32 i;
    /* FAKE: 48 unused bytes around i reproduce the original frame (0x60, i at sp+0x58, the strength-reduced
     * D_8015EC70[i] pointer spilled at sp+0x28); real locals unknown. T-19028 */
    Tc14 pad0;
    Tc14 pad1;
    s32 pad2;

    for (i = 0; i < 16; i++) {
        if (!(D_8015EC70[i].unk13 & 0x80)) {
            break;
        }
    }
    if (i >= 16) {
        return -1;
    }
    D_8015EC70[i].unk12 = 0;
    D_8015EC70[i].unk13 = 0x80;
    D_8015EC70[i].unk0 = pos.x;
    D_8015EC70[i].unk2 = pos.y;
    D_8015EC70[i].unk4 = pos.z;
    D_8015EC70[i].unk8 = 0x3F - (func_800AE0D0() & 0x7F);
    D_8015EC70[i].unkA = 0x3F - (func_800AE0D0() & 0x7F);
    D_8015EC70[i].unkC = 0x50;
    D_8015EC70[i].unk11 = kind;
    return i;
}

void func_8013E4FC(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: the loop entry is written on one source line; as1 schedules by line and this orders the
     * end-pointer and base setup as the original does. T-10320 */
    p = D_8015EC70; do { p->unk12 += 1; if (p->unk12 == 0xFF) { p->unk12 = 0;
            p->unk13 += 1;
        }
        if (p->unk13 == 0x85) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 += p->unk8;
            p->unk2 += p->unkA;
            p->unk4 += p->unkC;
            if (p->unk8 > 0) {
                p->unk8 -= 1;
            }
            if (p->unkA > 0) {
                p->unkA -= 1;
            }
            if (p->unk8 < 0) {
                p->unk8 += 1;
            }
            if (p->unkA < 0) {
                p->unkA += 1;
            }
            if (!(p->unk13 & 4)) {
                if (p->unk0 > 0x800) {
                    p->unk0 = 0x800;
                }
                if (p->unk0 < -0x800) {
                    p->unk0 = -0x800;
                }
                if (p->unk2 > 0x80) {
                    p->unk2 = 0x80;
                }
                if (p->unk2 < -0x700) {
                    p->unk2 = -0x700;
                }
                if (p->unk4 > D_8015EDB4->unk78 + 0x10) {
                    if (p->unkC > 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
                if (p->unk4 < -0x1000) {
                    if (p->unkC < 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
            }
        }
        p++;
    } while (p < (Tc14 *)&D_8015EDB0);
    /* FAKE: the base is spelled from the end label so that IDO does not share the first loop's address; same bytes. T-10320 */
    p = (Tc14 *)((u8 *)&D_8015EDB0 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            if (p->unk11 < 2) {
                func_8013E778(i);
            } else {
                func_8013EAA4(i);
            }
        }
        i++;
        p++;
    } while (i != 0x10);
}

void func_8013E778(s32 arg0) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (208), as in func_8013D528; T-19028 */
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
    v = *(SVECTOR *)&D_8015EC70[arg0];
    v.vz -= 0x200;
    kind = D_8015EC70[arg0].unk11;
    sel = (D_8015EC70[arg0].unk12 >> 2) & 7;
    func_800A1100(&m);
    func_800A1070(&m);
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
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - v.vz) / 127;
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
            poly->tpage = func_8009ECB0(1, 1, 0x2C0, 0x100);
            poly->clut = func_8009ED74(0, kind + 0x1E2);
            /* FAKE: hand expansion of a PsyQ setUVWH-style macro on one line, as in func_8013D528: as1 schedules the
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
            func_8009EED0((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013EAA4);
