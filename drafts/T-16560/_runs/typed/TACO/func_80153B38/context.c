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

void func_80153B38(void **arg0, s32 arg1, u32 arg2, s32 arg3) {
    s16 sp7C;
    s16 sp7A;
    s16 sp78;
    s32 sp5C;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t1;
    s32 var_s2;
    s32 var_s2_2;
    s8 temp_fp;
    s8 temp_s3;
    s8 temp_s4;
    s8 temp_s5;
    s8 temp_s7;
    void **var_s1;
    u8 *temp_s0;
    u8 *temp_s0_2;
    u8 *temp_s0_3;
    void *temp_v0;

    var_s2 = 0;
    do {
        if (var_s2 == 0) {
            sp78 = 0x100;
            sp7A = 0x78;
            sp7C = 0;
        } else {
            temp_s0 = (var_s2 * 8) + &sp78;
            temp_lo = (s32) (var_s2 * 0x9000) / 360;
            (*(s32 *)temp_s0) = (s16) (((s32) (func_800A0140(temp_lo) * arg3) / 4096) + 0x100);
            temp_lo_2 = func_800A0070(temp_lo) * arg3;
            *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = (s16) ((temp_lo_2 / 4096) + 0x78);
        }
        var_s2 += 1;
    } while (var_s2 < 0x2A);
    temp_t1 = arg2 & 0xFF;
    temp_fp = (arg2 >> 8) & 0xFF;
    temp_s7 = (arg2 >> 0x10) & 0xFF;
    var_s1 = arg0;
    temp_s5 = (temp_t1 & 0xFF) - 0x25;
    temp_s4 = (temp_fp & 0xFF) - 0x25;
    temp_s3 = (temp_s7 & 0xFF) - 0x25;
    sp5C = temp_t1;
    var_s2_2 = 0;
    do {
        temp_v0 = GetWorkBase(0x1C, D_8011ECA0);
        *var_s1 = temp_v0;
        func_8009F004((s32) temp_v0);
        SetSemiTrans(*var_s1, 1);
        SetShadeTex(*var_s1, 1);
        temp_s0_2 = &sp78 + (var_s2_2 * 8);
        if (D_8011ECA0 == 0) {
            *(s16 *)((unsigned char *)(*var_s1) + (8)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (8));
            *(s16 *)((unsigned char *)(*var_s1) + (10)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (10)) + 0xF0);
            *(s16 *)((unsigned char *)(*var_s1) + (16)) = sp78;
            *(s16 *)((unsigned char *)(*var_s1) + (18)) = (s16) (sp7A + 0xF0);
            *(s16 *)((unsigned char *)(*var_s1) + (24)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (16));
            *(s16 *)((unsigned char *)(*var_s1) + (26)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (18)) + 0xF0);
        }
        if (D_8011ECA0 == 1) {
            temp_s0_3 = &sp78 + (var_s2_2 * 8);
            *(s16 *)((unsigned char *)(*var_s1) + (8)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (8));
            *(s16 *)((unsigned char *)(*var_s1) + (10)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (10));
            *(s16 *)((unsigned char *)(*var_s1) + (16)) = sp78;
            *(s16 *)((unsigned char *)(*var_s1) + (18)) = sp7A;
            *(s16 *)((unsigned char *)(*var_s1) + (24)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (16));
            *(s16 *)((unsigned char *)(*var_s1) + (26)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (18));
        }
        *(s8 *)((unsigned char *)(*var_s1) + (4)) = temp_s3;
        *(s8 *)((unsigned char *)(*var_s1) + (5)) = temp_s4;
        *(s8 *)((unsigned char *)(*var_s1) + (6)) = temp_s5;
        *(s8 *)((unsigned char *)(*var_s1) + (12)) = temp_s7;
        *(s8 *)((unsigned char *)(*var_s1) + (13)) = temp_fp;
        *(s8 *)((unsigned char *)(*var_s1) + (14)) = (s8) sp5C;
        *(s8 *)((unsigned char *)(*var_s1) + (20)) = temp_s3;
        *(s8 *)((unsigned char *)(*var_s1) + (21)) = temp_s4;
        *(s8 *)((unsigned char *)(*var_s1) + (22)) = temp_s5;
        AddPrim((D_8011ECA0 << 0xA) + 0x10 + D_800E8CA0, *var_s1);
        var_s2_2 += 1;
        var_s1 += 1;
    } while (var_s2_2 != 0x28);
}

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
