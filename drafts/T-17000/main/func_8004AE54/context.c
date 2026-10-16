#include "common.h"
#include "game.h"

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

/* bulk_m2c declarations (T-11190) */
extern u8 D_800B3D96;
extern u8 D_800B3D97;
/* end bulk_m2c declarations */
void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 sp5C;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    u8 *temp_v0_3;

    var_s5 = 0;
    if ((u8) D_800B3D80 >= 4U) {
        if (D_800B3220 != 0) {
            temp_v0 = abs();
            var_s1 = temp_v0;
            if (temp_v0 >= 0) {
                sp5C = (a3 & 7) + 0x1F8;
                var_s2 = 0 * 4;
loop_4:
                temp_v0_2 = GetWorkBase(0x10, D_8011ECA0);
                func_8009F07C(temp_v0_2);
                SetSemiTrans(temp_v0_2, 0);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + a1) - var_s2);
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + a2);
                temp_v0_3 = D_800B3D84 + ((var_s1 % 10) * 2);
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) (*(s32 *)temp_v0_3);
                *(u8 *)((unsigned char *)temp_v0_2 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_3 + (1));
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(((a3 & 8) * 4) + 0x100, sp5C);
                AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_2);
                if (var_s1 < 0xA) {
                    var_s5 += 1;
                } else {
                    var_s5 += 1;
                    var_s2 += 5;
                    var_s1 = var_s1 / 10;
                    if (var_s1 >= 0) {
                        goto loop_4;
                    }
                }
            }
            if (a0 < 0) {
                sp5C = (a3 & 7) + 0x1F8;
                temp_v0_4 = GetWorkBase(0x10, D_8011ECA0);
                func_8009F07C(temp_v0_4);
                SetSemiTrans(temp_v0_4, 0);
                *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + a1) - (var_s5 * 5));
                *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + a2);
                *(s8 *)((unsigned char *)temp_v0_4 + (12)) = (s8) (D_800B3D96 + 8);
                *(u8 *)((unsigned char *)temp_v0_4 + (13)) = (u8) D_800B3D97;
                *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74(((a3 & 8) * 4) + 0x100, sp5C);
                AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_4);
            }
            dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
        }
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

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B358);

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

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar16);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", col2sepia);
