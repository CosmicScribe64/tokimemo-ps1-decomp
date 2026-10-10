#include "common.h"
#include "ovl/TACO.h"

void func_80153AA0(s32 arg0, s32 arg1, s32 arg2) {
    u8 buf[0xA0]; /* FAKE: size reproduces the original frame (locals 0xA0 bytes); the real type behind func_80153B38's out-buffer is unknown. T-8040 */
    s32 x;
    s32 p;

    func_80153B38(buf, &x, arg2, arg0);
    p = func_800490F0(0xC, D_8011ECA0);
    func_8009D294(p, 0, 1, func_8009ECB0(1, arg1, 0, 0), 0);
    func_8009EED0(D_8011ECA0 * 0x400 + 0x10 + D_800E8CA0, p);
    func_80049450(4);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80153AA0", func_80153B38);

typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 x1;
    /* 0x0E */ s16 y1;
    /* 0x10 */ s16 x2;
    /* 0x12 */ s16 y2;
    /* 0x14 */ s16 x3;
    /* 0x16 */ s16 y3;
} TcPolyF4; /* size 0x18 */

void func_80153E78(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    TcPolyF4 *poly;
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
