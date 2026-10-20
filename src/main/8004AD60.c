#include "common.h"
#include "game.h"
#include "psx_types.h"

void func_8004AD60(s32 arg0) {
    if ((arg0 & 0xFF) == 0) {
        D_800B3D80 = 0;
    } else {
        D_800B3D80 = D_800B3D80 - 1;
    }
    if ((u32)D_800B3D80 >= 9U) {
        D_800B3D80 = 8;
    }
}

void func_8004ADAC(u8 arg0) {
    D_800B3D80 = arg0;
    if ((u32)arg0 >= 9) {
        D_800B3D80 = 8;
    }
}

u8 func_8004ADD4(void) {
    return D_800B3D80;
}

void func_8004ADE4(void) {
    if (D_800B3220 != 0) {
        LoadSquare(0x2C0, 0x1E0, 0x40, 0x20, (void *)0x801F0020);
    }
}

/* FAKE: func_8004AE54 takes four arguments and the original leaves a0-a2 untouched; passing the
 * (unused) incoming a0-a2 through reproduces that, real source unknown. T-0800 */
void func_8004AE28(s32 a0, s32 a1, s32 a2) {
    if (D_800B3220 != 0) {
        func_8004AE54(a0, a1, a2, 0xF);
    }
}

/* tpx and tpy are declared locals so that the allocator spills the later one (tpy, sp+0x5C) and keeps tpx in
 * $fp, as the original does; as bare expressions it spills the other. T-17013 */
void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3) {
    SPRT_8 *p;
    s32 n;
    s32 v;
    s32 tpx;
    s32 tpy;

    n = 0;
    if (D_800B3D80 >= 4 && D_800B3220 != 0) {
        v = abs(a0);
        /* Both v >= 0 tests (here and at the loop end) are always true, but the original has the
         * bltz/bgez branches, so they stay. T-17013 */
        if (v >= 0) {
            tpx = (a3 & 8) * 4 + 0x100;
            tpy = (a3 & 7) + 0x1F8;
            do {
                p = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
                func_8009F07C(p);
                SetSemiTrans(p, 0);
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->x0 = D_800E6280.unk_014[D_8011ECA0] + a1 - n * 5;
                p->y0 = D_800E6280.unk_018[D_8011ECA0] + a2;
                p->u0 = D_800B3D84[(v % 10) * 2];
                p->v0 = D_800B3D84[(v % 10) * 2 + 1];
                p->clut = func_8009ED74(tpx, tpy);
                AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
                if (v < 10) {
                    n++;
                    break;
                }
                v = v / 10;
                n++;
            } while (v >= 0);
        }
        if (a0 < 0) {
            tpx = (a3 & 8) * 4 + 0x100;
            tpy = (a3 & 7) + 0x1F8;
            p = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F07C(p);
            SetSemiTrans(p, 0);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->x0 = D_800E6280.unk_014[D_8011ECA0] + a1 - n * 5;
            p->y0 = D_800E6280.unk_018[D_8011ECA0] + a2;
            p->u0 = D_800B3D84[0x12] + 8;
            p->v0 = D_800B3D84[0x13];
            p->clut = func_8009ED74(tpx, tpy);
            AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
        }
        dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    }
}

void func_8004B19C(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 n;
    u32 x;
    u8 *e;

    n = 0;
    if (D_800B3D80 >= 4 && D_800B3220 != 0) {
        x = arg0;
        for (;;) {
            p = (u8 *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F07C(p);
            SetSemiTrans(p, 0);
            p[4] = 0x80;
            p[5] = 0x80;
            p[6] = 0x80;
            *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg1 - n * 6;
            e = D_800B3D84 + (x & 0xF) * 2;
            *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg2;
            p[0xC] = e[0];
            p[0xD] = e[1];
            *(s16 *)(p + 0xE) = func_8009ED74(0x100, 0x1FF);
            AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
            if (x < 0x10) {
                dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
                break;
            }
            x = x >> 4;
            n++;
        }
    }
}

void func_8004B338(s32 arg0, s32 arg1, s32 arg2) {
    func_8004B358(arg0, arg1, arg2, 0xF);
}

s32 func_8004B358(s32 a0, s32 a1, s32 a2, s32 a3) {
    SPRT *p;
    s32 x;
    u8 c;
    s32 flag;
    u8 *ptr;
    s32 tpx;
    s32 tpy;
    /* FAKE: unused local pads stack frame to 0x88 and places spilled tpy at sp+0x5C. T-24123 */
    s32 pad;

    if (D_800B3D80 < 4) {
        return 0;
    }
    if (D_800B3220 == 0) {
        return 0;
    }
    ptr = (u8 *)a0;
    x = a1;
    while (*ptr != 0) {
        c = *ptr & 0x1F;
        flag = *ptr & 0x20;
        tpx = (a3 & 8) * 2 + 0x100;
        tpy = (a3 & 7) + 0x1F8;
        p = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(p);
        SetSemiTrans(p, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->x0 = D_800E6280.unk_014[D_8011ECA0] + x;
        p->y0 = D_800E6280.unk_018[D_8011ECA0] + a2;
        if (flag != 0) {
            x += 6;
        } else {
            x += 8;
        }
        if (flag != 0) {
            p->u0 = D_800B3D84[0x14] + c * 8 - 8;
            p->v0 = D_800B3D84[0x15];
        } else {
            p->u0 = D_800B3D84[0x14] + c * 8 - 8;
            p->v0 = D_800B3D84[0x15] - 0x10;
        }
        if (flag != 0) {
            setWH(p, 6, 0xC);
        } else {
            setWH(p, 8, 8);
        }
        p->clut = func_8009ED74(tpx, tpy);
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
        ptr++;
    }
    dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    return 0;
}

/* Two 5-5-5 colours per word (CLUT entries): read as u32 fields, written as u16 fields. */
typedef struct {
    u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 x0 : 1;
    u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 x1 : 1;
} Pix2In;

typedef struct {
    u16 r0 : 5;
    u16 g0 : 5;
    u16 b0 : 5;
    u16 x0 : 1;
    u16 r1 : 5;
    u16 g1 : 5;
    u16 b1 : 5;
    u16 x1 : 1;
} Pix2Out;

void func_8004B590(arg0, arg1)
Pix2In *arg0;
Pix2Out *arg1;
{
    Pix2In *src;
    Pix2Out *dst;
    s32 i;
    s32 lum;
    s32 t;

    src = arg0;
    dst = arg1;
    for (i = 0; i != 0x80; i++) {
        lum = src->r0 * 0x1E + src->g0 * 0x3B + src->b0 * 0xB;
        if (lum != 0 && lum < 0x64) {
            lum = 0x64;
        }
        dst->r0 = t = lum / 100;
        dst->g0 = t;
        dst->b0 = t;
        lum = src->r1 * 0x1E + src->g1 * 0x3B + src->b1 * 0xB;
        if (lum != 0 && lum < 0x64) {
            lum = 0x64;
        }
        dst->r1 = t = lum / 100;
        dst->g1 = t;
        dst->b1 = t;
        src++;
        dst++;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar);

void make_color_bar16(void *arg0, u32 start, u32 end, s32 count, s32 flag) {
    /* T-24501: declaration order preserves the interpolation spill homes. */
    s32 i;
    Pix2Out *p;
    s32 sr;
    s32 er;
    s32 sg;
    s32 eg;
    s32 sb;
    s32 eb;

    sr = (start >> 16) & 255;
    sg = (start >> 8) & 255;
    sb = start & 255;
    er = (end >> 16) & 255;
    eg = (end >> 8) & 255;
    eb = end & 255;
    p = arg0;
    i = 0;
    if ((count / 2) > 0) {
        do {
            p[i].r1 = ((s32)((count - (i * 2 + 1)) * er + (i * 2 + 1) * sr) / count) / 8;
            p[i].r0 = ((s32)((count - i * 2) * er + i * 2 * sr) / count) / 8;
            p[i].g1 = ((s32)((count - (i * 2 + 1)) * eg + (i * 2 + 1) * sg) / count) / 8;
            p[i].g0 = ((s32)((count - i * 2) * eg + i * 2 * sg) / count) / 8;
            p[i].b1 = ((s32)((count - (i * 2 + 1)) * eb + (i * 2 + 1) * sb) / count) / 8;
            p[i].b0 = ((s32)((count - i * 2) * eb + i * 2 * sb) / count) / 8;
            i++;
        } while (i != (count / 2));
    }
    if (flag) {
        for (i = 1; i < (count / 2); i++) {
            p[i].x1 = 1;
            p[i].x0 = 1;
        }
        p[0].x1 = 1;
    } else {
        for (i = 0; i < (count / 2); i++) {
            p[i].x1 = 0;
            p[i].x0 = 0;
        }
    }
    p[0].x0 = 0;
    p[0].r0 = 0;
    p[0].g0 = 0;
    p[0].b0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", col2sepia);
