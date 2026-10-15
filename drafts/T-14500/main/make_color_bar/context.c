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

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004AE54);

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

void make_color_bar(void *arg0, s32 arg1) {
    s16 temp_t1;
    s16 temp_t2_2;
    s16 temp_t4_2;
    s16 temp_t6;
    s16 temp_t8;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s8 temp_t2;
    s8 temp_t2_3;
    s8 temp_t4;
    s8 temp_t4_3;
    s8 temp_t7;
    s8 temp_t7_2;
    s8 temp_t9;
    s8 temp_t9_2;
    u8 *var_t0;
    u8 *var_t0_2;
    u8 *var_t0_3;
    u8 *var_t0_4;
    u8 *var_t0_5;

    var_v1 = 0;
    var_a2 = -1;
    var_t0 = arg0;
    do {
        temp_v0 = var_v1 & 0x1F;
        *(u8 *)((unsigned char *)var_t0 + (2)) = (u8) (temp_v0 | (*(u8 *)((unsigned char *)var_t0 + (2)) & 0xFFE0));
        *(u8 *)((unsigned char *)var_t0 + (66)) = (u8) (temp_v0 | (*(u8 *)((unsigned char *)var_t0 + (66)) & 0xFFE0));
        *(u8 *)((unsigned char *)var_t0 + (130)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (130)) & 0xFFE0);
        *(u8 *)((unsigned char *)var_t0 + (194)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (194)) & 0xFFE0);
        *(u8 *)((unsigned char *)var_t0 + (258)) = (u8) (temp_v0 | (*(u8 *)((unsigned char *)var_t0 + (258)) & 0xFFE0));
        *(u8 *)((unsigned char *)var_t0 + (322)) = (u8) (temp_v0 | (*(u8 *)((unsigned char *)var_t0 + (322)) & 0xFFE0));
        temp_a1 = var_a2 & 0x1F;
        *(u8 *)((unsigned char *)var_t0 + (386)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (386)) & 0xFFE0);
        *(u8 *)((unsigned char *)var_t0 + (450)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (450)) & 0xFFE0);
        (*(s32 *)var_t0) = (u8) (temp_a1 | ((*(s32 *)var_t0) & 0xFFE0));
        *(u8 *)((unsigned char *)var_t0 + (128)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (128)) & 0xFFE0);
        *(u8 *)((unsigned char *)var_t0 + (192)) = (u8) (*(u8 *)((unsigned char *)var_t0 + (192)) & 0xFFE0);
        *(u8 *)((unsigned char *)var_t0 + (64)) = (u8) (temp_a1 | (*(u8 *)((unsigned char *)var_t0 + (64)) & 0xFFE0));
        var_a2 += 2;
        temp_t4 = temp_a1 | (*(u8 *)((unsigned char *)var_t0 + (256)) & 0xFFE0);
        temp_t7 = temp_a1 | (*(u8 *)((unsigned char *)var_t0 + (320)) & 0xFFE0);
        temp_t9 = *(u8 *)((unsigned char *)var_t0 + (384)) & 0xFFE0;
        temp_t2 = *(u8 *)((unsigned char *)var_t0 + (448)) & 0xFFE0;
        var_v1 += 2;
        var_t0 += 4;
        *(s8 *)((unsigned char *)var_t0 + (252)) = temp_t4;
        *(s8 *)((unsigned char *)var_t0 + (316)) = temp_t7;
        *(s8 *)((unsigned char *)var_t0 + (380)) = temp_t9;
        *(s8 *)((unsigned char *)var_t0 + (444)) = temp_t2;
    } while (var_a2 < 0x1F);
    var_v1_2 = 0;
    var_a2_2 = -1;
    var_t0_2 = arg0;
    do {
        temp_v0_2 = (var_v1_2 << 5) & 0x3E0;
        *(u16 *)((unsigned char *)var_t0_2 + (2)) = (u16) (temp_v0_2 | (*(u16 *)((unsigned char *)var_t0_2 + (2)) & 0xFC1F));
        *(u16 *)((unsigned char *)var_t0_2 + (66)) = (u16) (temp_v0_2 | (*(u16 *)((unsigned char *)var_t0_2 + (66)) & 0xFC1F));
        *(u16 *)((unsigned char *)var_t0_2 + (130)) = (u16) (temp_v0_2 | (*(u16 *)((unsigned char *)var_t0_2 + (130)) & 0xFC1F));
        *(u16 *)((unsigned char *)var_t0_2 + (194)) = (u16) (temp_v0_2 | (*(u16 *)((unsigned char *)var_t0_2 + (194)) & 0xFC1F));
        *(u16 *)((unsigned char *)var_t0_2 + (258)) = (u16) (*(u16 *)((unsigned char *)var_t0_2 + (258)) & 0xFC1F);
        *(u16 *)((unsigned char *)var_t0_2 + (450)) = (u16) (*(u16 *)((unsigned char *)var_t0_2 + (450)) & 0xFC1F);
        temp_v0_3 = (var_a2_2 << 5) & 0x3E0;
        *(u16 *)((unsigned char *)var_t0_2 + (386)) = (u16) (*(u16 *)((unsigned char *)var_t0_2 + (386)) & 0xFC1F);
        *(u16 *)((unsigned char *)var_t0_2 + (322)) = (u16) (*(u16 *)((unsigned char *)var_t0_2 + (322)) & 0xFC1F);
        *(u16 *)((unsigned char *)var_t0_2 + (64)) = (u16) (temp_v0_3 | (*(u16 *)((unsigned char *)var_t0_2 + (64)) & 0xFC1F));
        (*(s32 *)var_t0_2) = (u16) (temp_v0_3 | ((*(s32 *)var_t0_2) & 0xFC1F));
        var_a2_2 += 2;
        *(u16 *)((unsigned char *)var_t0_2 + (128)) = (u16) (temp_v0_3 | (*(u16 *)((unsigned char *)var_t0_2 + (128)) & 0xFC1F));
        temp_t2_2 = temp_v0_3 | (*(u16 *)((unsigned char *)var_t0_2 + (192)) & 0xFC1F);
        temp_t4_2 = *(u16 *)((unsigned char *)var_t0_2 + (256)) & 0xFC1F;
        temp_t6 = *(u16 *)((unsigned char *)var_t0_2 + (320)) & 0xFC1F;
        temp_t8 = *(u16 *)((unsigned char *)var_t0_2 + (384)) & 0xFC1F;
        temp_t1 = *(u16 *)((unsigned char *)var_t0_2 + (448)) & 0xFC1F;
        var_v1_2 += 2;
        var_t0_2 += 4;
        *(s16 *)((unsigned char *)var_t0_2 + (188)) = temp_t2_2;
        *(s16 *)((unsigned char *)var_t0_2 + (252)) = temp_t4_2;
        *(s16 *)((unsigned char *)var_t0_2 + (316)) = temp_t6;
        *(s16 *)((unsigned char *)var_t0_2 + (380)) = temp_t8;
        *(s16 *)((unsigned char *)var_t0_2 + (444)) = temp_t1;
    } while (var_a2_2 < 0x1F);
    var_v1_3 = 0;
    var_a2_3 = -1;
    var_t0_3 = arg0;
    do {
        temp_v0_4 = (var_v1_3 * 4) & 0x7C;
        *(u8 *)((unsigned char *)var_t0_3 + (67)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (67)) & 0xFF83);
        *(u8 *)((unsigned char *)var_t0_3 + (3)) = (u8) (temp_v0_4 | (*(u8 *)((unsigned char *)var_t0_3 + (3)) & 0xFF83));
        *(u8 *)((unsigned char *)var_t0_3 + (195)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (195)) & 0xFF83);
        *(u8 *)((unsigned char *)var_t0_3 + (131)) = (u8) (temp_v0_4 | (*(u8 *)((unsigned char *)var_t0_3 + (131)) & 0xFF83));
        *(u8 *)((unsigned char *)var_t0_3 + (323)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (323)) & 0xFF83);
        *(u8 *)((unsigned char *)var_t0_3 + (259)) = (u8) (temp_v0_4 | (*(u8 *)((unsigned char *)var_t0_3 + (259)) & 0xFF83));
        *(u8 *)((unsigned char *)var_t0_3 + (451)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (451)) & 0xFF83);
        temp_a1_2 = (var_a2_3 * 4) & 0x7C;
        *(u8 *)((unsigned char *)var_t0_3 + (387)) = (u8) (temp_v0_4 | (*(u8 *)((unsigned char *)var_t0_3 + (387)) & 0xFF83));
        *(u8 *)((unsigned char *)var_t0_3 + (1)) = (u8) (temp_a1_2 | (*(u8 *)((unsigned char *)var_t0_3 + (1)) & 0xFF83));
        *(u8 *)((unsigned char *)var_t0_3 + (65)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (65)) & 0xFF83);
        *(u8 *)((unsigned char *)var_t0_3 + (193)) = (u8) (*(u8 *)((unsigned char *)var_t0_3 + (193)) & 0xFF83);
        *(u8 *)((unsigned char *)var_t0_3 + (129)) = (u8) (temp_a1_2 | (*(u8 *)((unsigned char *)var_t0_3 + (129)) & 0xFF83));
        var_a2_3 += 2;
        temp_t2_3 = temp_a1_2 | (*(u8 *)((unsigned char *)var_t0_3 + (257)) & 0xFF83);
        temp_t7_2 = temp_a1_2 | (*(u8 *)((unsigned char *)var_t0_3 + (385)) & 0xFF83);
        temp_t4_3 = *(u8 *)((unsigned char *)var_t0_3 + (321)) & 0xFF83;
        temp_t9_2 = *(u8 *)((unsigned char *)var_t0_3 + (449)) & 0xFF83;
        var_v1_3 += 2;
        var_t0_3 += 4;
        *(s8 *)((unsigned char *)var_t0_3 + (253)) = temp_t2_3;
        *(s8 *)((unsigned char *)var_t0_3 + (317)) = temp_t4_3;
        *(s8 *)((unsigned char *)var_t0_3 + (381)) = temp_t7_2;
        *(s8 *)((unsigned char *)var_t0_3 + (445)) = temp_t9_2;
    } while (var_a2_3 < 0x1F);
    var_v0 = 0;
    if (arg1 != 0) {
        var_v0_2 = 0;
        var_t0_4 = arg0;
        do {
            *(u8 *)((unsigned char *)var_t0_4 + (3)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (3)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (1)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (1)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (7)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (7)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (5)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (5)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (11)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (11)) | 0x80);
            var_v0_2 += 4;
            *(u8 *)((unsigned char *)var_t0_4 + (13)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (13)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (15)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (15)) | 0x80);
            *(u8 *)((unsigned char *)var_t0_4 + (9)) = (u8) (*(u8 *)((unsigned char *)var_t0_4 + (9)) | 0x80);
            var_t0_4 += 0x10;
        } while (var_v0_2 != 0x80);
        return;
    }
    var_t0_5 = arg0;
    do {
        *(u8 *)((unsigned char *)var_t0_5 + (7)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (7)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (3)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (3)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (1)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (1)) & 0xFF7F);
        var_v0 += 4;
        *(u8 *)((unsigned char *)var_t0_5 + (11)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (11)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (13)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (13)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (9)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (9)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (15)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (15)) & 0xFF7F);
        *(u8 *)((unsigned char *)var_t0_5 + (5)) = (u8) (*(u8 *)((unsigned char *)var_t0_5 + (5)) & 0xFF7F);
        var_t0_5 += 0x10;
    } while (var_v0 != 0x80);
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar16);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", col2sepia);
