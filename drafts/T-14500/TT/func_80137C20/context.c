#include "common.h"
#include "ovl/TT.h"

/* Fields used by TT's phase update. T-9720. */
typedef struct TtPhase3AE {
    /* 0x000 */ u8 pad000[0x52];
    /* 0x052 */ u8 frame;
    /* 0x053 */ u8 pad053[0x357];
    /* 0x3AA */ u16 phase;
    /* 0x3AC */ u16 step;
} TtPhase3AE; /* size 0x3AE */

/* T-11000 near miss: frame, spill slots, hoisted constants and stores match; only the saved-register numbering of a, &pt and D_80155C64 is rotated (original s2, s3, s4; here s4, s2, s3), 32 line pairs. */
#ifdef NON_MATCHING
void func_801374C0(u8 *arg0) {
    u8 *a;
    u8 *p;
    s32 j;
    s32 n;
    /* FAKE: unused homes (pa/pm) place pt at sp+0x68 and the spilled q, i at sp+0x54/0x58 in the 152-byte frame. T-11000. */
    s32 pa[7];
    s16 pt[2];
    s32 pm[3];
    s32 i;
    u8 *q;

    a = D_80158A74;
    i = 0;
    q = arg0;
    do {
        pt[0] = *(s16 *)(q + 0x1E);
        pt[1] = *(s16 *)(q + 0x9E);
        if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, (u8 *)pt, D_80155C64) != 0) {
            func_80144094();
            *(u16 *)(a + 4) = 0x60;
            *(s16 *)(a + 10) = 0;
            *(s16 *)(a + 8) = 0;
            *(s16 *)(a + 6) = 0;
            a[15] = 0;
            a[14] = 0;
            a[13] = 0;
        }
        p = D_80158A80;
        for (j = 0, n = 12; j != n; j++, p += 0x54) {
            if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, D_80155C64) != 0) {
                *(u16 *)(p + 4) = 0x80;
                *(s16 *)(p + 10) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 6) = 0;
                p[15] = 0;
                p[14] = 0;
                p[13] = 0;
            }
        }
        i += 4;
        q += 4;
    } while (i != 0x20);
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801374C0);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137654);

void func_80137878(void) {
    u8 *c;
    u8 *b;
    /* FAKE: unused homes retain the observed frame. T-11000. */
    s32 frame_pad[8];
    s16 pt[2];
    u8 *p;
    s32 i;
    s32 n;

    c = D_80158A8C;
    pt[0] = *(s16 *)(c + 0x14) + *(s16 *)(c + 0x254);
    b = D_80158A64;
    pt[1] = *(s16 *)(c + 0x16) + *(s16 *)(c + 0x284);
    c[0xBA] = 0;
    p = D_80158A80;
    for (i = 0, n = 12; i != n; i++, p += 0x54) {
        if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, &D_80155C70) != 0) {
            if (*(u16 *)(c + 0x30E) == 2) {
                b[0x25] = b[0x25] + 1;
                if (b[0x24] != 0) {
                    if (!(b[0x25] & 3)) {
                        if (!(b[0x25] & 7)) {
                            func_801342CC(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        } else {
                            func_80133FB0(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        }
                    }
                }
                func_80133AD0(0x507);
                if (i >= 6) {
                    *(s16 *)(c + 0x30C) -= 1;
                } else if (*(u16 *)(p + 2) == 9) {
                    *(s16 *)(c + 0x30C) -= 4;
                } else {
                    *(s16 *)(c + 0x30C) -= 2;
                }
            } else {
                func_80133AD0(0x506);
            }
            c[0xBA] = 1;
            *(u16 *)(p + 4) = 0x80;
            p[13] = 0;
            p[14] = 0;
            p[15] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 10) = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137A78);

/* bulk_m2c declarations (T-11190) */
extern s32 D_80155C5E;
extern u8 D_80152C4C;
extern u8 D_80152C50;
/* end bulk_m2c declarations */
void func_80137C20(void) {
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a3;
    s32 var_t1;
    s32 var_t1_2;
    s32 var_v1_4;
    s32 var_v1_5;
    u8 *temp_s1;
    u8 *temp_s2;
    u8 *temp_s2_2;
    u8 *var_a0;
    u8 *var_a1;
    u8 *var_a2;
    u8 *var_a3_2;
    u8 *var_a3_3;
    u8 *var_a3_4;
    u8 *var_a3_5;
    u8 *var_t0;
    u8 *var_t0_2;
    u8 *var_t2;
    u8 *var_t2_2;
    u8 *var_t3;
    u8 *var_t3_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;
    u8 *var_v0_5;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;

    temp_s1 = D_80158A8C;
    temp_s2 = D_80158AAC;
    func_80142740();
    func_80142774();
    (*(s32 *)temp_s1) = 1;
    *(s8 *)((unsigned char *)temp_s1 + (834)) = 0;
    *(s32 *)((unsigned char *)temp_s1 + (32)) = 0;
    *(s32 *)((unsigned char *)temp_s1 + (36)) = 0xFFC00000;
    *(s16 *)((unsigned char *)temp_s1 + (20)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (22)) = -0x40;
    *(s32 *)((unsigned char *)temp_s1 + (40)) = 0;
    *(s32 *)((unsigned char *)temp_s1 + (44)) = 0x20000;
    var_a0 = &D_80152C50;
    var_a1 = &D_80152C4C;
    var_v0 = temp_s1;
    var_a2 = temp_s1;
    var_a3 = 0;
    var_v1 = temp_s1;
    do {
        *(u8 **)((unsigned char *)var_v0 + (56)) = D_80151A50;
        *(s8 *)((unsigned char *)var_v0 + (60)) = 0;
        *(u8 *)((unsigned char *)var_v0 + (61)) = (u8) *var_a1;
        *(s8 *)((unsigned char *)var_v0 + (64)) = 0x60;
        *(s8 *)((unsigned char *)var_v0 + (67)) = 0xF;
        *(s8 *)((unsigned char *)var_v0 + (72)) = 0;
        *(u8 *)((unsigned char *)var_v0 + (62)) = (u8) *var_a0;
        *(s8 *)((unsigned char *)var_a2 + (784)) = 1;
        if (var_a3 < 4) {
            *(s16 *)((unsigned char *)var_v1 + (680)) = 0xA;
        } else {
            *(s16 *)((unsigned char *)var_v1 + (680)) = 8;
        }
        var_a0 += 1;
        var_v0 += 0x14;
        var_a1 += 1;
        var_a2 += 1;
        var_a3 += 2;
        var_v1 += 2;
        *(s16 *)((unsigned char *)var_v1 + (582)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (630)) = 0;
    } while ((u32) var_a0 < (u32) D_80152C54);
    *(s16 *)((unsigned char *)temp_s1 + (632)) = -6;
    *(s16 *)((unsigned char *)temp_s1 + (588)) = 0x18;
    *(s16 *)((unsigned char *)temp_s1 + (636)) = 0x34;
    *(s16 *)((unsigned char *)temp_s1 + (590)) = -0x18;
    *(s16 *)((unsigned char *)temp_s1 + (638)) = 0x34;
    *(s16 *)((unsigned char *)temp_s1 + (780)) = 0x60;
    *(s16 *)((unsigned char *)temp_s1 + (782)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (932)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (934)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (936)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (938)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (940)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (868)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (870)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (872)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (874)) = 0;
    *(s16 *)((unsigned char *)temp_s1 + (876)) = 0;
    *(s8 *)((unsigned char *)temp_s1 + (835)) = 0;
    (*(s32 *)temp_s2) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) + 0x1C);
    *(s16 *)((unsigned char *)temp_s2 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (22)) - 0x22);
    *(s16 *)((unsigned char *)temp_s2 + (16)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2 + (20)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2 + (18)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2 + (22)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2 + (24)) = 0x100;
    *(s8 *)((unsigned char *)temp_s2 + (26)) = 8;
    *(s8 *)((unsigned char *)temp_s2 + (27)) = 4;
    *(s32 *)((unsigned char *)temp_s2 + (12)) = 0x380;
    *(s32 *)((unsigned char *)temp_s2 + (8)) = 0x1C0;
    var_t1 = 0;
    var_a3_2 = temp_s2;
    var_t0 = temp_s2;
    var_t2 = temp_s2;
    var_t3 = temp_s2;
    do {
        *(s32 *)((unsigned char *)var_a3_2 + (284)) = 0x380;
        *(s16 *)((unsigned char *)var_t0 + (2460)) = 0xC00;
        *(s16 *)((unsigned char *)var_t0 + (3548)) = 0;
        var_a0_2 = 0;
        var_v0_2 = var_t2;
        var_v1_2 = var_t3;
loop_7:
        *(s32 *)((unsigned char *)var_v0_2 + (412)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_2 + (2524)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_2 + (416)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_2 + (2526)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_2 + (420)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_2 + (2528)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_2 + (424)) = 0x380;
        var_a0_2 += 4;
        *(s16 *)((unsigned char *)var_v1_2 + (2530)) = 0xC00;
        var_v0_2 += 0x10;
        var_v1_2 += 8;
        if (var_a0_2 != 0x10) {
            goto loop_7;
        }
        var_t1 += 4;
        var_a3_2 += 4;
        var_t0 += 2;
        var_t2 += 0x40;
        var_t3 += 0x20;
    } while (var_t1 < 0x80);
    func_801410A0((s32) temp_s2, 0xC00, 0x380, (s32) var_a3_2);
    *(s16 *)((unsigned char *)temp_s2 + (3836)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) - 0x1C);
    temp_s2_2 = temp_s2 + 0xEFC;
    *(s16 *)((unsigned char *)temp_s2_2 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (22)) - 0x22);
    *(s16 *)((unsigned char *)temp_s2_2 + (16)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2_2 + (20)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2_2 + (18)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2_2 + (22)) = 0xC00;
    *(s16 *)((unsigned char *)temp_s2_2 + (24)) = 0x100;
    *(s8 *)((unsigned char *)temp_s2_2 + (26)) = 8;
    *(s8 *)((unsigned char *)temp_s2_2 + (27)) = 4;
    *(s32 *)((unsigned char *)temp_s2_2 + (12)) = 0x380;
    *(s32 *)((unsigned char *)temp_s2_2 + (8)) = 0x1C0;
    var_a3_3 = temp_s2_2;
    var_t0_2 = temp_s2_2;
    var_t2_2 = temp_s2_2;
    var_t3_2 = temp_s2_2;
    var_t1_2 = 0;
    do {
        *(s32 *)((unsigned char *)var_a3_3 + (284)) = 0x380;
        *(s16 *)((unsigned char *)var_t0_2 + (2460)) = 0xC00;
        *(s16 *)((unsigned char *)var_t0_2 + (3548)) = 0;
        var_a0_3 = 0;
        var_v0_3 = var_t2_2;
        var_v1_3 = var_t3_2;
loop_11:
        *(s32 *)((unsigned char *)var_v0_3 + (412)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_3 + (2524)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_3 + (416)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_3 + (2526)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_3 + (420)) = 0x380;
        *(s16 *)((unsigned char *)var_v1_3 + (2528)) = 0xC00;
        *(s32 *)((unsigned char *)var_v0_3 + (424)) = 0x380;
        var_a0_3 += 4;
        *(s16 *)((unsigned char *)var_v1_3 + (2530)) = 0xC00;
        var_v0_3 += 0x10;
        var_v1_3 += 8;
        if (var_a0_3 != 0x10) {
            goto loop_11;
        }
        var_t1_2 += 4;
        var_a3_3 += 4;
        var_t0_2 += 2;
        var_t2_2 += 0x40;
        var_t3_2 += 0x20;
    } while (var_t1_2 < 0x80);
    func_801410A0((s32) temp_s2_2, 0xC00, 0x380, (s32) var_a3_3);
    var_a3_4 = temp_s2_2;
    var_v1_4 = 0;
    var_v0_4 = D_80158AA4 + 0xFA0;
    do {
        (*(s32 *)var_v0_4) = 1;
        *(s8 *)((unsigned char *)var_v0_4 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0_4 + (68)) = 0;
        var_a3_4 += 4;
        *(s16 *)((unsigned char *)var_v0_4 + (20)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) + 0x14);
        var_v0_4 -= 0x64;
        *(s16 *)((unsigned char *)var_v0_4 + (122)) = (s16) ((*(s16 *)((unsigned char *)temp_s1 + (22)) + var_v1_4) - 0x2A);
        var_v1_4 += 8;
        *(s16 *)((unsigned char *)var_v0_4 + (120)) = (s16) (*(s16 *)((unsigned char *)var_a3_4 + (26)) - 8);
        *(s16 *)((unsigned char *)var_v0_4 + (122)) = (s16) (*(s16 *)((unsigned char *)var_a3_4 + (154)) - 8);
        *(s16 *)((unsigned char *)var_v0_4 + (162)) = 0x10;
        *(s16 *)((unsigned char *)var_v0_4 + (160)) = 0x10;
        *(s8 *)((unsigned char *)var_v0_4 + (171)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_4 + (170)) = 0x90;
        *(s8 *)((unsigned char *)var_v0_4 + (158)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_4 + (157)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_4 + (156)) = 0x80;
        *(s16 *)((unsigned char *)var_v0_4 + (164)) = 0x12;
        *(s16 *)((unsigned char *)var_v0_4 + (166)) = 0x3CD9;
        *(s8 *)((unsigned char *)var_v0_4 + (159)) = 0x65;
        *(s16 *)((unsigned char *)var_v0_4 + (116)) = 9;
    } while (var_v1_4 < 0x38);
    var_a3_5 = temp_s2_2 + 4;
    *(s8 *)((unsigned char *)D_80158AA4 + (4800)) = 1;
    *(s8 *)((unsigned char *)D_80158AA4 + (4801)) = 0;
    *(s16 *)((unsigned char *)D_80158AA4 + (4868)) = 0;
    var_v1_5 = 8;
    *(s16 *)((unsigned char *)D_80158AA4 + (4820)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) - 0x24);
    *(s16 *)((unsigned char *)D_80158AA4 + (4822)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (22)) - 0x2A);
    var_v0_5 = D_80158AA4 + 0x125C;
    *(s16 *)((unsigned char *)var_v0_5 + (120)) = (s16) (*(s16 *)((unsigned char *)temp_s2_2 + (30)) - 8);
    *(s16 *)((unsigned char *)var_v0_5 + (116)) = 9;
    *(s16 *)((unsigned char *)var_v0_5 + (122)) = (s16) (*(s16 *)((unsigned char *)temp_s2_2 + (158)) - 8);
    *(s8 *)((unsigned char *)var_v0_5 + (159)) = 0x65;
    *(s16 *)((unsigned char *)var_v0_5 + (166)) = 0x3CD9;
    *(s16 *)((unsigned char *)var_v0_5 + (164)) = 0x12;
    *(s8 *)((unsigned char *)var_v0_5 + (158)) = 0x80;
    *(s8 *)((unsigned char *)var_v0_5 + (157)) = 0x80;
    *(s8 *)((unsigned char *)var_v0_5 + (156)) = 0x80;
    *(s8 *)((unsigned char *)var_v0_5 + (171)) = 0x80;
    *(s8 *)((unsigned char *)var_v0_5 + (170)) = 0x90;
    *(s16 *)((unsigned char *)var_v0_5 + (162)) = 0x10;
    *(s16 *)((unsigned char *)var_v0_5 + (160)) = 0x10;
    do {
        (*(s32 *)var_v0_5) = 1;
        *(s8 *)((unsigned char *)var_v0_5 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0_5 + (68)) = 0;
        var_a3_5 += 8;
        *(s16 *)((unsigned char *)var_v0_5 + (20)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) - 0x24);
        var_v0_5 -= 0xC8;
        *(s16 *)((unsigned char *)var_v0_5 + (222)) = (s16) ((*(s16 *)((unsigned char *)temp_s1 + (22)) + var_v1_5) - 0x2A);
        *(s16 *)((unsigned char *)var_v0_5 + (220)) = (s16) (*(s16 *)((unsigned char *)var_a3_5 + (22)) - 8);
        *(s16 *)((unsigned char *)var_v0_5 + (168)) = 0;
        *(s8 *)((unsigned char *)var_v0_5 + (101)) = 0;
        *(s8 *)((unsigned char *)var_v0_5 + (100)) = 1;
        *(s16 *)((unsigned char *)var_v0_5 + (222)) = (s16) (*(s16 *)((unsigned char *)var_a3_5 + (150)) - 8);
        *(s16 *)((unsigned char *)var_v0_5 + (262)) = 0x10;
        *(s16 *)((unsigned char *)var_v0_5 + (260)) = 0x10;
        *(s8 *)((unsigned char *)var_v0_5 + (271)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (270)) = 0x90;
        *(s8 *)((unsigned char *)var_v0_5 + (258)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (257)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (256)) = 0x80;
        *(s16 *)((unsigned char *)var_v0_5 + (264)) = 0x12;
        *(s16 *)((unsigned char *)var_v0_5 + (266)) = 0x3CD9;
        *(s8 *)((unsigned char *)var_v0_5 + (259)) = 0x65;
        *(s16 *)((unsigned char *)var_v0_5 + (216)) = 9;
        *(s16 *)((unsigned char *)var_v0_5 + (120)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (20)) - 0x24);
        *(s16 *)((unsigned char *)var_v0_5 + (122)) = (s16) ((*(s16 *)((unsigned char *)temp_s1 + (22)) + var_v1_5) - 0x22);
        var_v1_5 += 0x10;
        *(s16 *)((unsigned char *)var_v0_5 + (120)) = (s16) (*(s16 *)((unsigned char *)var_a3_5 + (26)) - 8);
        *(s16 *)((unsigned char *)var_v0_5 + (116)) = 9;
        *(s16 *)((unsigned char *)var_v0_5 + (122)) = (s16) (*(s16 *)((unsigned char *)var_a3_5 + (154)) - 8);
        *(s8 *)((unsigned char *)var_v0_5 + (159)) = 0x65;
        *(s16 *)((unsigned char *)var_v0_5 + (166)) = 0x3CD9;
        *(s16 *)((unsigned char *)var_v0_5 + (164)) = 0x12;
        *(s8 *)((unsigned char *)var_v0_5 + (158)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (157)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (156)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (171)) = 0x80;
        *(s8 *)((unsigned char *)var_v0_5 + (170)) = 0x90;
        *(s16 *)((unsigned char *)var_v0_5 + (162)) = 0x10;
        *(s16 *)((unsigned char *)var_v0_5 + (160)) = 0x10;
    } while (var_v1_5 != 0x38);
    *(s32 **)((unsigned char *)temp_s1 + (568)) = &D_80155C5E;
}

void func_80138178(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
        *(s16 *)(p + 4) = 0x30;
    }
}

void func_801381E8(void) {
    u8 *p = D_80158A8C;

    func_80142AF0(D_80152C54, 0);
    if (p[0x40] & 1) {
        *(u8 **)(p + 0x218) = D_80155C58;
        *(s16 *)(p + 4) = 0x40;
    }
}

void func_80138240(void) {
    u8 *b = D_80158A8C;
    u8 *a = D_80158A74;
    s32 d = *(s16 *)(a + 0x14) - *(s16 *)(b + 0x14);
    s32 ad;
    s16 y;

    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < 2) {
        *(s32 *)(b + 0x20) = *(s16 *)(a + 0x14) << 16;
    } else if (d < 0) {
        *(s32 *)(b + 0x20) -= 0x4000;
    } else {
        *(s32 *)(b + 0x20) += 0x4000;
    }
    y = *(s16 *)(b + 0x22);
    if (y >= 0x51) {
        *(s32 *)(b + 0x20) = 0x500000;
        y = *(s16 *)(b + 0x22);
    } else if (y < -0x50) {
        *(s32 *)(b + 0x20) = 0xFFB00000;
        y = *(s16 *)(b + 0x22);
    }
    *(s16 *)(b + 0x14) = y;
}

/* Operand order of the sums (x + f() / 64, table lookups) sets the temp numbering; p -= 0x64 before the second store of the loop gives the original delay-slot fill; s += 0xEFC reuses s for the second half. T-11520 */
void func_80138300(void) {
    u8 *c;
    u8 *s;
    u8 *a;
    u8 *p;
    s32 i;
    s16 x;
    u16 v;

    c = D_80158A8C;
    s = D_80158AAC;
    a = D_80158A74;
    *(s16 *)s = *(s16 *)(c + 0x14) + 0x1C;
    *(s16 *)(s + 2) = *(s16 *)(c + 0x16) - 0x22;
    if (*(u16 *)(c + 0x36A) < 0x100) {
        *(s16 *)(s + 4) = *(s16 *)(a + 0x14);
        *(s16 *)(s + 6) = *(s16 *)(a + 0x16);
        if (*(u16 *)(c + 0x3A4) != 0) {
            *(u16 *)(c + 0x3A8) = 0x400;
        } else {
            *(u16 *)(c + 0x3A8) = 0x2400;
        }
    } else {
        *(s16 *)(s + 4) = *(s16 *)s + func_800A0140(*(u16 *)(c + 0x3A8)) / 64;
        *(s16 *)(s + 6) = *(s16 *)(s + 2) + func_800A0070(*(u16 *)(c + 0x3A8)) / 64;
        if (*(u16 *)(c + 0x3A4) != 0) {
            *(u16 *)(c + 0x3A8) += 0x30;
            if (*(u16 *)(c + 0x3A8) >= 0x2400) {
                *(u16 *)(c + 0x36A) = 0;
            }
        } else {
            *(u16 *)(c + 0x3A8) -= 0x30;
            if (*(u16 *)(c + 0x3A8) < 0x401) {
                *(u16 *)(c + 0x36A) = 0;
            }
        }
    }
    *(u16 *)(c + 0x36A) += 1;
    func_801410A0(s);
    p = D_80158AA4 + 0xFA0;
    for (i = 0; i < 7; i++) {
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        p -= 0x64;
        *(s16 *)(p + 0x7A) = *(s16 *)(s + 0x9E + i * 4) - 8;
    }
    x = *(s16 *)(c + 0x14);
    *(s16 *)(c + 0x24C) = *(s16 *)(s + 0x3A) - x;
    *(s16 *)(c + 0x27C) = *(s16 *)(s + 0xBA) - *(s16 *)(c + 0x16);
    c[0x66] = D_80152C68[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256] + D_80152C88[(*(u16 *)(c + 0x364) >> 3) & 3];
    c[0x64] = D_80152C78[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256];
    s += 0xEFC;
    *(s16 *)s = x - 0x1C;
    *(s16 *)(s + 2) = *(s16 *)(c + 0x16) - 0x22;
    if (*(u16 *)(c + 0x368) < 0x100) {
        *(s16 *)(s + 4) = *(s16 *)(a + 0x14);
        *(s16 *)(s + 6) = *(s16 *)(a + 0x16);
        if (*(u16 *)(c + 0x3A4) != 0) {
            *(u16 *)(c + 0x3A6) = 0x2400;
        } else {
            *(u16 *)(c + 0x3A6) = 0x400;
        }
    } else {
        *(s16 *)(s + 4) = *(s16 *)s + func_800A0140(*(u16 *)(c + 0x3A6)) / 64;
        *(s16 *)(s + 6) = *(s16 *)(s + 2) + func_800A0070(*(u16 *)(c + 0x3A6)) / 64;
        if (*(u16 *)(c + 0x3A4) != 0) {
            *(u16 *)(c + 0x3A6) -= 0x30;
            if (*(u16 *)(c + 0x3A6) < 0x401) {
                *(u16 *)(c + 0x368) = 0;
                *(u16 *)(c + 0x3A4) += 1;
                *(u16 *)(c + 0x3A4) &= 1;
            }
        } else {
            *(u16 *)(c + 0x3A6) += 0x30;
            if (*(u16 *)(c + 0x3A6) >= 0x2400) {
                *(u16 *)(c + 0x368) = 0;
                *(u16 *)(c + 0x3A4) += 1;
                *(u16 *)(c + 0x3A4) &= 1;
            }
        }
    }
    *(u16 *)(c + 0x368) += 1;
    func_801410A0(s);
    p = D_80158AA4 + 0x12C0;
    for (i = 0; i < 7; i++) {
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        *(s16 *)(p + 0x16) = *(s16 *)(s + 0x9E + i * 4) - 8;
        p -= 0x64;
    }
    *(s16 *)(c + 0x24E) = *(s16 *)(s + 0x3A) - *(s16 *)(c + 0x14);
    v = *(u16 *)(c + 0x364);
    *(s16 *)(c + 0x27E) = *(s16 *)(s + 0xBA) - *(s16 *)(c + 0x16);
    c[0x7A] = D_80152C68[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256] + D_80152C88[(v >> 3) & 3];
    c[0x78] = D_80152C78[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256];
    *(u16 *)(c + 0x364) = v + 1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138808);

void func_80138B90(void) {
    u8 *p;
    s32 i;

    p = D_80158A8C;
    /* IDO unrolls this by two after peeling i = 2; the typed record index folds the second copy into offsets */
    for (i = 2; i < 7; i++) {
        ((TtAnim14 *)(p + 0x38))[i].frames = D_80151A50;
        ((TtAnim14 *)(p + 0x38))[i].unk_04 = 0;
        ((TtAnim14 *)(p + 0x38))[i].unk_05 = 6;
        if (i == 6) {
            ((TtAnim14 *)(p + 0x38))[i].unk_06 = 0x14;
        } else {
            ((TtAnim14 *)(p + 0x38))[i].unk_06 = 0x1F;
        }
        ((TtAnim14 *)(p + 0x38))[i].unk_08 = 0x60;
        ((TtAnim14 *)(p + 0x38))[i].unk_09 = 0;
        ((TtAnim14 *)(p + 0x38))[i].unk_0B = 0xF;
        ((TtAnim14 *)(p + 0x38))[i].unk_10 = 0;
        p[0x310 + i] = 1;
        *(s16 *)(p + 0x2A8 + i * 2) = 9;
        if (i == 6) {
            *(s16 *)(p + 0x248 + i * 2) = 0;
        } else {
            *(s16 *)(p + 0x248 + i * 2) = i * 16 - 0x38;
        }
        *(s16 *)(p + 0x278 + i * 2) = D_80152C40[i - 2];
    }
}

void func_80138D10(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 1:
        func_80142AF0(D_80152CA4, 6);
        break;
    case 2:
        func_80142AF0(D_80152CB0, 6);
        break;
    case 3:
        func_80142AF0(D_80152CC4, 6);
        break;
    }
}

void func_80138D94(void) {
    u8 *base;
    s32 i;

    base = D_80158A8C;
    for (i = 2; i < 6; i++) {
        if (base[i + 0x351] != 0) {
            func_80142AF0(D_80152CD0, i);
        } else {
            func_80142AF0(D_80152CE4, i);
        }
    }
}

/* T-11510 near miss: structure matches; differences are the register numbers of the hoisted constants in the case-0 slot loop (the original ranks 8, the table pointer, 4, then the bound 18; here 18 comes first) and the delay-slot fill of the sltiu before the 0x4B test; about 73 differing instructions (permuter 8 min: no gain). */
void func_80138E28(void) {
    u8 *c;
    u8 *s;
    u8 *p;
    u8 *e;
    s16 *t;
    s16 j;
    s32 i;
    u16 k;

    k = *(u16 *)(D_80158A8C + 6);
    c = D_80158A8C;
    s = D_80158AB0;
    switch (k) {
    case 0:
        *(u16 *)(c + 0x364) = 0;
        *(u16 *)(c + 0x3D4) = 0;
        /* The unsigned bound is a constant of its own; an int 0x12 would share the hoisted register of the 0x12 stored in the 0x4B block and change the register order. T-11830 */
        for (i = 0x10; i != 0x12U; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 4;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            c[0x310 + i] = 0;
            *(s16 *)(c + 0x2A8 + i * 2) = 8;
            *(s16 *)(c + 0x248 + i * 2) = 0;
            *(s16 *)(c + 0x278 + i * 2) = 0;
        }
        *(s16 *)(c + 6) = 0x40;
        return;
    case 0x40:
        t = D_80152CFC;
        e = D_80158A8C;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            func_80133AD0(0x501);
            j = t[*(u16 *)(c + 0x3D4) + 4];
            c[j + 0x310] = 1;
            c[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(c + j * 2 + 0x248) = t[*(u16 *)(c + 0x3D4) + 1];
            *(s16 *)(c + j * 2 + 0x278) = t[*(u16 *)(c + 0x3D4) + 2];
            *(s16 *)(c + j * 2 + 0x2A8) = t[*(u16 *)(c + 0x3D4) + 3];
            *(u16 *)(c + 0x3D4) += 5;
        }
        for (i = 0x10; i < 0x12; i++) {
            func_80142AF0(D_80150950, i);
        }
        if (*(u16 *)(c + 0x364) == 0x4B) {
            c[0x310] = 0;
            func_80138B90();
            c[0x52] = 1;
            p = D_80158AA4 + 0x1900;
            for (i = 0; i != 0x10; i++, p += 0x64) {
                p[0] = 1;
                p[1] = 0;
                *(s16 *)(p + 0x44) = 0;
                p[0x3B] = 0x80;
                *(u8 **)(p + 0x48) = s;
                p[0x4D] = 0;
                p[0x4E] = 1;
                p[0x50] = 0;
                *(s16 *)(p + 0x14) = *(s16 *)(c + 0x14) - 8;
                *(s16 *)(p + 0x16) = *(s16 *)(c + 0x16) - 8;
                p[0x47] = (i & 1) * 0x10 + 0x40;
                p[0x46] = 0xF0;
                p[0x4F] = 0;
                *(s16 *)(p + 0x3C) = 0x10;
                *(s16 *)(p + 0x3E) = 0x10;
                *(s16 *)(p + 0x40) = 0x12;
                *(s16 *)(p + 0x42) = 0x3CD2;
                p[0x4C] = 0;
                *(s16 *)(p + 0x10) = 8;
            }
            *(u16 *)(c + 0x3D4) = 0;
        }
        if (*(u16 *)(c + 0x364) >= 0x4C) {
            p = D_80158AA4 + 0x1900;
            for (i = 0; i < 0x10; i++, p += 0x64) {
                *(s16 *)(p + 0x14) = *(s16 *)(c + 0x14) + ((u32)(func_800A0140(i * 0x100) * *(u16 *)(c + 0x3D4)) >> 12) - 8;
                *(s16 *)(p + 0x16) = *(s16 *)(c + 0x16) + ((u32)(func_800A0070(i * 0x100) * *(u16 *)(c + 0x3D4)) >> 12) - 8;
                p[0x4C] = D_80152D30[(*(u16 *)(c + 0x364) >> 2) & 3];
            }
            *(u16 *)(c + 0x3D4) += 4;
        }
        if (*(u16 *)(c + 0x364) >= 0x88) {
            p = D_80158AA4 + 0x1900;
            for (i = 0; i < 0x10; i++, p += 0x64) {
                p[0] = 0;
            }
            *(s16 *)(c + 4) = 0x52;
            *(s16 *)(c + 6) = 0;
        }
        *(u16 *)(c + 0x364) += 1;
        return;
    }
}

void func_8013923C(void) {
    TtPhase3AE *work;
    u16 step;
    u16 phase;

    work = (TtPhase3AE *)D_80158A8C;
    phase = work->phase += work->step;
    if (phase >= 0x800U) {
        phase = work->phase = 0;
    }
    step = work->step;
    work->frame = (s8) ((phase >> 8) + 1);
    if (step < 0x100U) {
        work->step = (u16) (step + 1);
        return;
    }
    work->step = 0x100U;
}

/* T-11520 near miss: 11 differing instructions, registers only. Writing the case-0 short arrays as *(s16 *)(c + 0x2A8 + i * 2) makes c + i * 2 one range with the 0x40 loops (priority 75.5), so it takes v1 and the slot pointer a0 as in the original. Left: the shared c[0x364] tail value (PRE'd load in the default case, the end of case 0, the 0xDC/0xFD tests and the 0xFD block) is a0 in the original and v0 here (nothing allocated before it holds v0 or v1 in its blocks), and the 0xFD loop preheader orders li v1,0x18 before move v0,a2 (the original fills the bne delay slot with the move). Tried: i = 0 before the 0xDC test, do/while and i < 0x18 forms of the 0xFD loop, (*p)++ tail. */
#ifdef NON_MATCHING
void func_801392A0(void) {
    u8 *c;
    u8 *e;
    s16 *t;
    s16 j;
    s32 i;
    u16 k;
    u16 m;

    k = *(u16 *)(D_80158A8C + 6);
    c = D_80158A8C;
    switch (k) {
    case 0:
        func_80142774();
        *(u16 *)(c + 0x364) = 0;
        *(u16 *)(c + 0x3D4) = 0;
        for (i = 0x10; i < 0x13; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 4;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            c[0x310 + i] = 0;
            *(s16 *)(c + 0x2A8 + i * 2) = 8;
            *(s16 *)(c + 0x248 + i * 2) = 0;
            *(s16 *)(c + 0x278 + i * 2) = 0;
        }
        for (i = 0x13; i != 0x18; i++) {
            c[0x310 + i] = 0;
        }
        *(s16 *)(c + 6) = 0x40;
        *(s16 *)(c + 8) = 0;
        break;
    case 0x40:
        t = D_80152D34;
        e = D_80158A8C;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            func_80133AD0(0x501);
            j = t[*(u16 *)(c + 0x3D4) + 4];
            c[j + 0x310] = 1;
            c[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(c + j * 2 + 0x248) = t[*(u16 *)(c + 0x3D4) + 1];
            *(s16 *)(c + j * 2 + 0x278) = t[*(u16 *)(c + 0x3D4) + 2];
            *(s16 *)(c + j * 2 + 0x2A8) = t[*(u16 *)(c + 0x3D4) + 3];
            *(u16 *)(c + 0x3D4) += 5;
        }
        m = *(u16 *)(c + 8);
        switch (m) {
        case 0:
            if (*(u16 *)(c + 0x364) == 0x78) {
                func_80133AD0(0x505);
                for (i = 0; i < 5; i++) {
                    *(s32 *)(c + 0x3E4 + i * 4) = *(s16 *)(c + 0x27C + i * 2) << 16;
                }
                *(s32 *)(c + 0x3F8) = 0xFFFE0000;
                *(s32 *)(c + 0x3FC) = 0x1000;
                *(s16 *)(c + 0x3D6) = 0;
                *(s16 *)(c + 8) = 0x40;
            }
            break;
        case 0x40:
            for (i = 0; i < 5; i++) {
                *(s16 *)(c + 0x27C + i * 2) = *(s32 *)(c + 0x3E4 + i * 4) >> 16;
                *(s32 *)(c + 0x3E4 + i * 4) += *(s32 *)(c + 0x3F8);
            }
            *(s16 *)(c + 0x24C) -= 1;
            *(s16 *)(c + 0x252) += 1;
            if (*(u16 *)(c + 0x3D6) == 1) {
                *(u16 *)(c + 0x3D6) = 0;
                *(s16 *)(c + 0x24E) -= 1;
                *(s16 *)(c + 0x250) += 1;
            } else {
                *(u16 *)(c + 0x3D6) = 1;
            }
            *(s32 *)(c + 0x3F8) += *(s32 *)(c + 0x3FC);
            if (*(s16 *)(c + 0x284) >= 0xB1) {
                *(u16 *)(c + 8) = 0x50;
            }
            break;
        case 0x50:
            break;
        }
        for (i = 0x10; i < 0x13; i++) {
            func_80142AF0(D_80150950, i);
        }
        if (*(u16 *)(c + 0x364) == 0xDC) {
            c[0x311] = 0;
        }
        if (*(u16 *)(c + 0x364) == 0xFD) {
            for (i = 0; i != 0x18; i++) {
                c[0x310 + i] = 0;
            }
            *(s16 *)(c + 10) = 0;
            *(s16 *)(c + 8) = 0;
            *(s16 *)(c + 6) = 0;
            c[15] = 0;
            c[14] = 0;
            c[13] = 0;
            *(s16 *)(c + 4) = 0xA0;
        }
        break;
    }
    *(u16 *)(c + 0x364) += 1;
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801392A0);
#endif

void func_801396C4(void) {
    func_80138300();
}

void func_801396E4(void) {
    u8 *p = D_80158A8C;
    u8 *q;

    if ((u32)*(u16 *)(p + 0x366) >= 0x18) {
        q = D_80158AAC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        q += 0xEFC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        *(u16 *)(p + 0x366) = 0;
        return;
    }
    *(u16 *)(p + 0x366) += 1;
}

/* Non-prototype declaration for taking the address: the ANSI prototype of 80133970.c (s16/u16 parameters) would change the K&R calls in this file. T-11510 */
extern void func_80134470();

void func_80139770(void) {
    void (*fn)();
    u8 *c;
    s32 i;

    c = D_80158A8C;
    if (*(u16 *)(c + 0x30E) == 1) {
        fn = func_80133FB0;
    } else {
        fn = func_80134470;
    }
    if (*(u16 *)(D_80158A8C + 0x366) >= 0x30) {
        func_80133AD0(0x50B);
        switch (*(u16 *)(c + 0x3A4)) {
        case 0:
            if (c[0x52] == 1) {
                fn(*(s16 *)(c + 0x14), *(s16 *)(c + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    func_80133E30(*(s16 *)(c + 0x14) + D_80152D9C[i], *(s16 *)(c + 0x16) + D_80152DA8[i], 0x20, D_80152DB4[i], 0xA9);
                    ((TtSlot14 *)(c + 0x38))[0x10 + i].unk_08 = 0x80;
                    c[0x320 + i] = 1;
                    ((s16 *)(c + 0x268))[i] = D_80152D9C[i];
                    ((s16 *)(c + 0x298))[i] = D_80152DA8[i];
                }
                *(u16 *)(c + 0x3A4) = 1;
                *(u16 *)(c + 0x366) = 0;
            }
            break;
        case 1:
            if (c[0x52] == 3) {
                fn(*(s16 *)(c + 0x14), *(s16 *)(c + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    func_80133E30(*(s16 *)(c + 0x14) + D_80152DC0[i], *(s16 *)(c + 0x16) + D_80152DCC[i], 0x20, D_80152DD8[i], 0xA9);
                    ((TtSlot14 *)(c + 0x38))[0x10 + i].unk_08 = 0x80;
                    c[0x320 + i] = 1;
                    ((s16 *)(c + 0x268))[i] = D_80152DC0[i];
                    ((s16 *)(c + 0x298))[i] = D_80152DCC[i];
                }
                *(u16 *)(c + 0x3A4) = 2;
                *(u16 *)(c + 0x366) = 0;
            }
            break;
        case 2:
            if (c[0x52] == 5) {
                fn(*(s16 *)(c + 0x14), *(s16 *)(c + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 4; i++) {
                    func_80133E30(*(s16 *)(c + 0x14) + D_80152DE4[i], *(s16 *)(c + 0x16) + D_80152DEC[i], 0x20, D_80152DF4[i], 0xA9);
                    ((TtSlot14 *)(c + 0x38))[0x10 + i].unk_08 = 0x80;
                    c[0x320 + i] = 1;
                    ((s16 *)(c + 0x268))[i] = D_80152DE4[i];
                    ((s16 *)(c + 0x298))[i] = D_80152DEC[i];
                }
                *(u16 *)(c + 0x3A4) = 3;
                *(u16 *)(c + 0x366) = 0;
            }
            break;
        case 3:
            if (c[0x52] == 7) {
                fn(*(s16 *)(c + 0x14), *(s16 *)(c + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    func_80133E30(*(s16 *)(c + 0x14) + D_80152DFC[i], *(s16 *)(c + 0x16) + D_80152E08[i], 0x20, D_80152E14[i], 0xA9);
                    ((TtSlot14 *)(c + 0x38))[0x10 + i].unk_08 = 0x80;
                    c[0x320 + i] = 1;
                    ((s16 *)(c + 0x268))[i] = D_80152DFC[i];
                    ((s16 *)(c + 0x298))[i] = D_80152E08[i];
                }
                *(u16 *)(c + 0x3A4) = 0;
                *(u16 *)(c + 0x366) = 0;
            }
            break;
        }
    } else {
        *(u16 *)(c + 0x366) += 1;
    }
    for (i = 0x10; i != 0x15; i++) {
        func_80142AF0(D_80152D7C, i);
    }
}

/* T-11520 near miss: structure and frame match; 28 differing lines. Spill homes of the promoted a/b pointers are at sp+0x34/0x38 here against 0x38/0x3C in the original (one extra home between them; unused pad locals do not move them), and the hoisted constant 1 takes v0 instead of a2 (case 0x40, 0x52 and 0xA0 stores). The case-0x54 u16 store of 0x80 and the u8 store in case 0xA0 are needed to match the shared li a0,0x80 and sb. */
/* The constant 1 is one hoisted value (a2) over cases 0x40, 0x52, 0x80 and 0xA0 only when every store of it is unsigned (u16/u8), and the case-0x54 u16 store of 0x80 shares the dispatch's li a0,0x80. a and b are declared first for their spill homes at sp+0x3C/0x38. T-11830 */
void func_80139B98(void) {
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *d;
    s32 i;
    s16 v;
    u16 k;

    c = D_80158A8C;
    k = *(u16 *)(c + 4);
    a = D_80158A60;
    b = D_80158A64;
    d = D_80158A74;
    switch (k) {
    case 0:
        func_80142570(a, d);
        if (b[0x22] != 0 && a[0x52] == 0xFF) {
            func_80044750(0x71);
        } else if (*(u16 *)(b + 4) == 0) {
            func_80044750(0xB1);
            func_800451E0(0x8019A000);
            func_80044750(0x12);
            func_80044750(0x23);
            func_80044750(0x2E);
            func_80044750(0x21);
            func_80044750(4);
            func_80044750(0x200);
        }
        func_80137C20();
        *(s16 *)(c + 4) = 0x20;
        break;
    case 0x20:
        func_80138178();
        func_801396C4();
        break;
    case 0x30:
        func_801381E8();
        func_801396C4();
        break;
    case 0x40:
        v = *(s16 *)(D_80158A8C + 0x30C);
        if (v < 0x20) {
            *(u16 *)(c + 6) = 2;
        } else if (v < 0x40) {
            *(u16 *)(c + 6) = 1;
        } else {
            *(u16 *)(c + 6) = 0;
        }
        func_80138240();
        func_801396C4();
        if (*(u16 *)(c + 8) == 0) {
            func_801396E4();
        }
        v = *(s16 *)(c + 0x30C);
        if (v < 0) {
            *(s16 *)(c + 4) = 0x50;
            *(s16 *)(c + 10) = 0;
            *(u16 *)(c + 8) = 0;
            *(s16 *)(c + 6) = 0;
            c[15] = 0;
            c[14] = 0;
            c[13] = 0;
        } else if (v < 0x20) {
            c[0x3E] = 0x3A;
        } else if (v < 0x40) {
            c[0x3E] = 0x36;
        }
        break;
    case 0x50:
        *(s32 *)(c + 0x218) = 0;
        func_80138808();
        break;
    case 0x51:
        func_80138E28();
        break;
    case 0x52:
        *(s16 *)(c + 4) = 0x53;
        *(u16 *)(c + 0x30E) = 1;
        *(s16 *)(c + 0x3A4) = 0;
        *(s16 *)(c + 0x3AA) = 0;
        *(u16 *)(c + 0x3AC) = 1;
        *(s16 *)(c + 0x366) = 0;
        for (i = 0; i < 4; i++) {
            *(s16 *)(c + 0x2D8 + i * 2) = 0x18;
            c[0x353 + i] = 1;
        }
        for (i = 0x10; i != 0x15; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 6;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            c[0x310 + i] = 0;
            *(s16 *)(c + 0x2A8 + i * 2) = 8;
            *(s16 *)(c + 0x248 + i * 2) = 0;
            *(s16 *)(c + 0x278 + i * 2) = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_08 = 0x60;
        }
        break;
    case 0x53:
        func_80138240();
        func_8013923C();
        func_80138D10();
        func_80138D94();
        func_80139770();
        if (c[0x353] == 0 && c[0x354] == 0 && c[0x355] == 0 && c[0x356] == 0) {
            *(s16 *)(c + 4) = 0x54;
            *(s16 *)(c + 0x30E) = 2;
            *(s16 *)(c + 0x30C) = 0x48;
        }
        break;
    case 0x54:
        func_80138240();
        func_8013923C();
        func_80138D10();
        func_80138D94();
        func_80139770();
        if (*(s16 *)(c + 0x30C) < 0) {
            func_80133AD0(0x504);
            *(u16 *)(c + 4) = 0x80;
            *(s16 *)(c + 0x30E) = 3;
            c[0xB8] = 0x80;
        }
        break;
    case 0x80:
        func_80044750(0xC1);
        d[0x52] = 1;
        func_801392A0();
        func_80138D10();
        func_80138D94();
        break;
    case 0xA0:
        func_80044750(0xB1);
        c[0] = 0;
        b[0x12] = 0;
        *(u16 *)((u8 *)D_80158A70 + 0x1E) = 1;
        *(s16 *)(b + 2) = 0xA0;
        break;
    }
    func_80137A78();
}
