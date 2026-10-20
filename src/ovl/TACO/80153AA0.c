#include "common.h"
#include "ovl/TACO.h"

void func_80153AA0(s32 arg0, s32 arg1, s32 arg2) {
    POLY_G3 *buf[0x28]; /* func_80153B38 fills one POLY_G3 pointer per fan triangle (0xA0 bytes, the original frame). T-19018 */
    s32 x;
    s32 p;

    func_80153B38(buf, &x, arg2, arg0);
    p = func_800490F0(0xC, D_8011ECA0);
    func_8009D294(p, 0, 1, func_8009ECB0(1, arg1, 0, 0), 0);
    func_8009EED0(D_8011ECA0 * 0x400 + 0x10 + D_800E8CA0, p);
    func_80049450(4);
}

/* NON_MATCHING: T-24502, 26 aligned diff lines (12 target words versus 14 built),
 * confined to the color decode; two extra copies. Default remains original asm.
 * Geometry, polygon loop, original 0x1C8 frame and point home 0x78 agree.
 * FAKE: implicit-int fallthrough preserves the original loop/result view. T-24502 */
#ifdef NON_MATCHING
s32 func_80153B38(buf, arg1, col, r)
POLY_G3 **buf;
s32 *arg1;
u32 col;
s32 r;
{
    SVECTOR v[42];
    s32 i;
    u8 cr;
    u8 cg;
    u8 cb;
    u8 dr;
    u8 dg;
    u8 db;
    POLY_G3 **p;

    for (i = 0; i < 42; i++) {
        if (i == 0) {
            v[0].vx = 0x100;
            v[0].vy = 0x78;
            v[0].vz = 0;
        } else {
            v[i].vx = func_800A0140(i * 0x9000 / 0x168) * r / 4096 + 0x100;
            v[i].vy = func_800A0070(i * 0x9000 / 0x168) * r / 4096 + 0x78;
            v[i].vz = 0;
        }
    }
    p = buf;
    for (i = 0; i != 40; i++) {
        *p = (POLY_G3 *)GetWorkBase(sizeof(POLY_G3), D_8011ECA0);
        func_8009F004(*p);
        SetSemiTrans(*p, 1);
        SetShadeTex(*p, 1);
        if (D_8011ECA0 == 0) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy + 0xF0;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy + 0xF0;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy + 0xF0;
        }
        if (D_8011ECA0 == 1) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy;
        }
        cr = (col >> 16) & 0xFF;
        cg = (col >> 8) & 0xFF;
        cb = col & 0xFF;
        dr = (cr & 0xFF) - 0x25;
        dg = (cg & 0xFF) - 0x25;
        db = (cb & 0xFF) - 0x25;
        (*p)->r0 = dr;
        (*p)->g0 = dg;
        (*p)->b0 = db;
        (*p)->r1 = cr;
        (*p)->g1 = cg;
        (*p)->b1 = cb;
        (*p)->r2 = dr;
        (*p)->g2 = dg;
        (*p)->b2 = db;
        AddPrim(D_8011ECA0 * 0x400 + 0x10 + D_800E8CA0, (s32)*p);
        p++;
    }
}

#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80153AA0", func_80153B38);
#endif

void func_80153E78(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    POLY_F4 *poly;
    s32 sprite;

    poly = GetWorkBase(0x18, D_8011ECA0);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 1);
    poly->x0 = 0;
    poly->y0 = D_800E6280.unk_018[D_8011ECA0] - 0x78;
    poly->x1 = 0x200;
    poly->y1 = D_800E6280.unk_018[D_8011ECA0] - 0x78;
    poly->x2 = 0;
    poly->y2 = D_800E6280.unk_018[D_8011ECA0] + 0x78;
    poly->x3 = 0x200;
    poly->y3 = D_800E6280.unk_018[D_8011ECA0] + 0x78;
    poly->r0 = arg1;
    poly->g0 = arg2;
    poly->b0 = arg3;
    func_8009EED0(D_8011ECA0 * 0x400 + 0x14 + D_800E8CA0, poly);
    sprite = func_800490F0(0xC, D_8011ECA0);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, arg0, 0, 0), 0);
    func_8009EED0(D_8011ECA0 * 0x400 + 0x14 + D_800E8CA0, sprite);
    safe_env(5);
}

s32 func_80154014(void) {
    s32 t;
    s32 v;

    D_8015F4F0 = 0x80;
    t = D_80160464;
    if (t < 0x32) {
        func_8013A790(0, 2);
    } else if (t >= 0x32 && t < 0xDC) {
        func_8013A790(0, 3);
    }
    if (t >= 0x32 && t < 0x5A) {
        func_80153AA0(t * 3 - 0x96, 2, 0xA0A0A0);
    }
    if (t >= 0x5A && t < 0x96) {
        func_80153AA0(0x78, 2, 0xA0A0A0);
    }
    if (t >= 0x96 && t < 0xB4) {
        func_80153AA0(0x10E - t, 2, 0xA0A0A0);
    }
    if (t >= 0xB4 && t < 0xD2) {
        func_80153AA0(0x276 - t * 3, 2, 0xA0A0A0);
    }
    if (t < 0x32) {
        v = t * 2;
        func_80153E78(2, v, v, v);
    } else if (t >= 0x32 && t < 0xD2) {
        if ((t - 0x32) % 3 == 0) {
            func_80153E78(1, 0xA0, 0xA0, 0xC0);
        } else {
            func_80153E78(2, 0x64, 0x64, 0x64);
        }
    } else if (t >= 0xD2 && t < 0x104) {
        v = 0x208 - t * 2;
        func_80153E78(2, v, v, v);
    }
    if (t == 0x104) {
        D_80160464 = 0;
        D_80160460 = 0;
    } else {
        D_80160464 += 1;
    }
    return D_80160460;
}
