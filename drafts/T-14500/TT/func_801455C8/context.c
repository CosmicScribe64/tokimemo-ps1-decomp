#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80144D70", func_80144D70);

/* T-11800: matched. T-10820 dead compare locals reserve a0 in both loops; d2, assigned before the first loop and tested by an empty if inside it (FAKE), also reserves a1 there, so the hoisted 0x40/0x70 take a2/a3 as in the original. */
void func_80145370(void) {
    s32 i;
    u8 *p;
    int d1;
    int d2;

    i = 0;
    p = D_80158A98;
    d2 = (int)D_80158AA0; /* FAKE: with the empty if (d2) {} below, d2 lives through the first loop and reserves a1, so the hoisted 0x70 takes a3 as in the original; no code is emitted. T-11800 */
    for (; i < 0x40; i++) {
        /* FAKE: dead compare locals with an else branch (decomp-permuter); they only reserve registers so that the hoisted constants sit in the original's registers. T-10820 */
        if (p[0] != 0 && (d1 = (*(u16 *)(p + 4) == 0x40))) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            if (d2) { /* FAKE: see d2 above. T-11800 */
            }
            p[0xE] = 0;
            p[0xD] = 0;
        } else {
            d1 = 0;
        }
        p += 0x78;
    }
    i = 0;
    p = D_80158AA0;
    for (; i < 0x80; i++) {
        if (p[0] != 0 && (d2 = (*(u16 *)(p + 4) == 0x40))) {
            p[0] = 0;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = (*(u16 *)(p + 4) == 0x40) * 0;
            p[0xD] = 0;
        } else {
            d2 = 0;
        }
        p += 0x68;
    }
}

void func_801454D8(u8 *arg0) {
    D_80158A74[0x55] = 0xA;
    func_8013BE9C(arg0 + 0x14);
}

s32 func_80145508(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    s16 x1 = *(s16 *)arg1 + *(s16 *)arg0;
    s16 y1 = *(s16 *)(arg1 + 2) + *(s16 *)(arg0 + 2);
    s16 x2 = *(s16 *)arg3 + *(s16 *)arg2;
    s16 y2 = *(s16 *)(arg3 + 2) + *(s16 *)(arg2 + 2);
    s32 dx = x1 - x2;
    s32 dy;
    s32 a;

    if (dx < 0) {
        a = -dx;
    } else {
        a = dx;
    }
    dy = y1 - y2;
    if ((u32)a < (u32)(arg3[4] + arg1[4])) {
        if (dy < 0) {
            a = -dy;
        } else {
            a = dy;
        }
        if ((u32)a < (u32)(arg3[5] + arg1[5])) {
            return 1;
        }
    }
    return 0;
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80155B3C;
extern s32 D_80155B48;
/* end bulk_m2c declarations */
void func_801455C8(void) {
    u8 *sp90;
    s8 sp75;
    s8 sp74;
    s16 sp72;
    s16 sp70;
    s32 sp6C;
    s32 temp_t1;
    s32 temp_t4;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t9;
    s32 temp_v0_5;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_s3_3;
    s32 var_s3_4;
    s32 var_s3_5;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_6;
    u16 temp_v1;
    u16 var_a1;
    u16 var_a1_2;
    u8 *temp_s2;
    u8 *temp_s5;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *var_s0_3;
    u8 *var_s0_4;
    u8 *var_s0_5;
    u8 *var_s0_6;
    u8 *var_s0_7;
    u8 *var_s0_8;
    u8 *var_s0_9;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 temp_t4_2;
    u8 temp_t4_3;

    temp_s5 = D_80158A64;
    var_s0 = D_80158A98;
    sp6C = 0;
    sp90 = D_80158A74;
loop_1:
    if (((*(s32 *)var_s0) != 0) && (*(u16 *)((unsigned char *)var_s0 + (4)) == 0x40) && ((*(u16 *)((unsigned char *)var_s0 + (2)) != 0x47) || ((u16) *(u16 *)((unsigned char *)var_s0 + (6)) >= 2U)) && (func_80145508((s32) (sp90 + 0x14), (s32) D_80155B30, (s32) (var_s0 + 0x14), (s32) &D_80155B3C) != 0)) {
        if ((u16) *(u16 *)((unsigned char *)sp90 + (4)) < 0x60U) {
            func_8014C580(var_s0);
            *(u16 *)((unsigned char *)var_s0 + (4)) = 0x71U;
            *(s16 *)((unsigned char *)var_s0 + (10)) = 0;
            *(s16 *)((unsigned char *)var_s0 + (8)) = 0;
            *(u16 *)((unsigned char *)var_s0 + (6)) = 0U;
            *(s8 *)((unsigned char *)var_s0 + (15)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (14)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (13)) = 0;
        }
        if (*(u8 *)((unsigned char *)sp90 + (82)) == 0) {
            sp6C = 0;
            func_80144094();
            *(u16 *)((unsigned char *)sp90 + (4)) = 0x60U;
        } else {
            goto block_11;
        }
    } else {
block_11:
        var_s0 += 0x78;
        temp_t9 = sp6C + 1;
        sp6C = temp_t9;
        if (temp_t9 >= 0x40) {
            sp6C = 0;
        } else {
            goto loop_1;
        }
    }
    var_s0_2 = D_80158AA0;
loop_14:
    if (((*(s32 *)var_s0_2) != 0) && ((temp_v0 = *(u16 *)((unsigned char *)var_s0_2 + (4)), (temp_v0 == 0x40)) || (temp_v0 == 0x50))) {
        if (*(u16 *)((unsigned char *)var_s0_2 + (2)) == 0xAD) {
            switch (temp_v0) {
            case 0x40:
                sp70 = 0;
                sp74 = 2;
                sp72 = (s16) ((u16) *(u16 *)((unsigned char *)var_s0_2 + (98)) >> 1);
                sp75 = (s8) ((u16) *(u16 *)((unsigned char *)var_s0_2 + (98)) >> 1);
                break;
            case 0x50:
                sp70 = 0;
                sp72 = 0x10;
                sp74 = 2;
                sp75 = 0x10;
                break;
            }
            *(s16 **)((unsigned char *)var_s0_2 + (76)) = &sp70;
        }
        if (func_80145508((s32) (sp90 + 0x14), (s32) D_80155B30, (s32) (var_s0_2 + 0x14), (s32) *(s16 **)((unsigned char *)var_s0_2 + (76))) != 0) {
            if ((u16) *(u16 *)((unsigned char *)sp90 + (4)) < 0x60U) {
                (*(s32 *)var_s0_2) = 0;
            }
            if (*(u8 *)((unsigned char *)sp90 + (82)) == 0) {
                sp6C = 0;
                func_80144094();
                var_a1 = 0x60 & 0xFFFF;
                *(u16 *)((unsigned char *)sp90 + (4)) = 0x60U;
                *(s16 *)((unsigned char *)sp90 + (10)) = 0;
                *(s16 *)((unsigned char *)sp90 + (8)) = 0;
                *(s16 *)((unsigned char *)sp90 + (6)) = 0;
                *(s8 *)((unsigned char *)sp90 + (15)) = 0;
                *(s8 *)((unsigned char *)sp90 + (14)) = 0;
                *(s8 *)((unsigned char *)sp90 + (13)) = 0;
            } else {
                goto block_30;
            }
        } else {
            goto block_30;
        }
    } else {
block_30:
        var_s0_2 += 0x68;
        temp_t7 = sp6C + 1;
        sp6C = temp_t7;
        if (temp_t7 >= 0x80) {
            sp6C = 0;
            var_a1 = *(u16 *)((unsigned char *)sp90 + (4));
        } else {
            goto loop_14;
        }
    }
    if ((var_a1 == 0x30) || (var_a1 == 0x40)) {
        var_s0_3 = D_80158A90;
        do {
            if (*var_s0_3 != 0) {
                if (func_80145508((s32) (sp90 + 0x14), (s32) D_80155B30, (s32) (var_s0_3 + 0x14), (s32) &D_80155B48) != 0) {
                    func_80144A04((s32) var_s0_3);
                }
            }
            temp_t1 = sp6C + 1;
            sp6C = temp_t1;
            var_s0_3 += 0x6C;
        } while (temp_t1 != 0x20);
        sp6C = 0;
    }
    var_s0_4 = D_80158A9C;
    do {
        if ((*(s32 *)var_s0_4) != 0) {
            if (*(u16 *)((unsigned char *)var_s0_4 + (4)) == 0x7A) {
                if (func_80145508((s32) (sp90 + 0x14), (s32) D_80155B30, (s32) (var_s0_4 + 0x14), (s32) &D_80155B48) != 0) {
                    temp_v0_2 = *(u16 *)((unsigned char *)var_s0_4 + (2));
                    if (temp_v0_2 == 0x6E) {
                        func_80133AD0(0x60F);
                        (*(s32 *)var_s0_4) = 0;
                        func_80145370();
                    } else if (temp_v0_2 == 0x6F) {
                        (*(s32 *)var_s0_4) = 0;
                        func_801454D8((s32) var_s0_4);
                    }
                }
            }
        }
        var_s0_4 += 0x78;
        temp_t5 = sp6C + 1;
        sp6C = temp_t5;
    } while (temp_t5 < 0x20);
    if (*(u8 *)((unsigned char *)sp90 + (83)) != 0) {
        var_s1 = D_80158A78;
        sp6C = 0;
        do {
            if (*var_s1 != 0) {
                var_s0_5 = D_80158A98;
                var_s3 = 0;
                do {
                    if (((*(s32 *)var_s0_5) != 0) && (*(u16 *)((unsigned char *)var_s0_5 + (4)) == 0x40) && ((*(u16 *)((unsigned char *)var_s0_5 + (2)) != 0x47) || ((u16) *(u16 *)((unsigned char *)var_s0_5 + (6)) >= 2U)) && (func_80145508((s32) (var_s1 + 0x14), (s32) &D_80155B36, (s32) (var_s0_5 + 0x14), (s32) &D_80155B3C) != 0)) {
                        func_8014C580(var_s0_5);
                        *(u16 *)((unsigned char *)var_s0_5 + (4)) = 0x71U;
                        *(s16 *)((unsigned char *)var_s0_5 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s0_5 + (8)) = 0;
                        *(u16 *)((unsigned char *)var_s0_5 + (6)) = 0U;
                        *(s8 *)((unsigned char *)var_s0_5 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s0_5 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s0_5 + (13)) = 0;
                    }
                    var_s3 += 1;
                    var_s0_5 += 0x78;
                } while (var_s3 < 0x40);
                var_s0_6 = D_80158AA0;
                var_s3_2 = 0;
                do {
                    if (((*(s32 *)var_s0_6) != 0) && ((temp_v0_3 = *(u16 *)((unsigned char *)var_s0_6 + (4)), (temp_v0_3 == 0x40)) || (temp_v0_3 == 0x50))) {
                        temp_v0_4 = *(u16 *)((unsigned char *)var_s0_6 + (2));
                        if ((temp_v0_4 != 0xAB) && (temp_v0_4 != 0xAD) && (func_80145508((s32) (var_s1 + 0x14), (s32) D_80155B30, (s32) (var_s0_6 + 0x14), *(s32 *)((unsigned char *)var_s0_6 + (76))) != 0)) {
                            (*(s32 *)var_s0_6) = 0;
                        }
                    }
                    var_s3_2 += 1;
                    var_s0_6 += 0x68;
                } while (var_s3_2 != 0x80);
            }
            temp_t6 = sp6C + 1;
            sp6C = temp_t6;
            var_s1 += 0xA4;
        } while (temp_t6 != 3);
    }
    var_s1_2 = D_80158A80;
    sp6C = 0;
    do {
        if (((*(s32 *)var_s1_2) != 0) && (*(u16 *)((unsigned char *)var_s1_2 + (4)) == 0x40)) {
            var_s0_7 = D_80158A98;
            var_s3_3 = 0;
            do {
                if (((*(s32 *)var_s0_7) != 0) && (*(u16 *)((unsigned char *)var_s0_7 + (4)) == 0x40) && ((*(u16 *)((unsigned char *)var_s0_7 + (2)) != 0x47) || ((u16) *(u16 *)((unsigned char *)var_s0_7 + (6)) >= 2U)) && (func_80145508((s32) (var_s1_2 + 0x14), (s32) &D_80155B36, (s32) (var_s0_7 + 0x14), (s32) &D_80155B3C) != 0)) {
                    temp_t4_2 = *(u8 *)((unsigned char *)temp_s5 + (37)) + 1;
                    *(u8 *)((unsigned char *)temp_s5 + (37)) = temp_t4_2;
                    if (*(u8 *)((unsigned char *)temp_s5 + (36)) != 0) {
                        temp_v0_5 = temp_t4_2 & 0xFF;
                        if (temp_v0_5 & 1) {
                            if (temp_v0_5 & 2) {
                                func_801342CC((s32) *(s16 *)((unsigned char *)var_s0_7 + (20)), (s32) *(s16 *)((unsigned char *)var_s0_7 + (22)), 0x100, 0x20, 0xA0);
                            } else {
                                func_80133FB0((s32) *(s16 *)((unsigned char *)var_s0_7 + (20)), (s32) *(s16 *)((unsigned char *)var_s0_7 + (22)), 0x100, 0x20, 0xA0);
                            }
                        }
                    }
                    *(u16 *)((unsigned char *)var_s0_7 + (4)) = 0x70U;
                    *(s16 *)((unsigned char *)var_s0_7 + (10)) = 0;
                    *(s16 *)((unsigned char *)var_s0_7 + (8)) = 0;
                    *(u16 *)((unsigned char *)var_s0_7 + (6)) = 0U;
                    *(s8 *)((unsigned char *)var_s0_7 + (15)) = 0;
                    *(s8 *)((unsigned char *)var_s0_7 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_s0_7 + (13)) = 0;
                    *(u16 *)((unsigned char *)var_s1_2 + (4)) = 0x80U;
                    *(s8 *)((unsigned char *)var_s1_2 + (13)) = 0;
                    *(s8 *)((unsigned char *)var_s1_2 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_s1_2 + (15)) = 0;
                    *(s16 *)((unsigned char *)var_s1_2 + (6)) = 0;
                    *(s16 *)((unsigned char *)var_s1_2 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_s1_2 + (10)) = 0;
                }
                var_s3_3 += 1;
                var_s0_7 += 0x78;
            } while (var_s3_3 < 0x40);
            var_s0_8 = D_80158AA0;
            var_s3_4 = 0;
loop_90:
            if (((*(s32 *)var_s0_8) != 0) && ((temp_v0_6 = *(u16 *)((unsigned char *)var_s0_8 + (4)), (temp_v0_6 == 0x40)) || (temp_v0_6 == 0x50))) {
                temp_v1 = *(u16 *)((unsigned char *)var_s0_8 + (2));
                switch (temp_v1) {
                case 0xA2:
                case 0xA9:
                case 0xAA:
                    var_s3_4 += 1;
                    if (func_80145508((s32) (var_s1_2 + 0x14), (s32) &D_80155B36, (s32) (var_s0_8 + 0x14), *(s32 *)((unsigned char *)var_s0_8 + (76))) != 0) {
                        func_80133AD0(0x509);
                        (*(s32 *)var_s0_8) = 0;
                        *(u16 *)((unsigned char *)var_s1_2 + (4)) = 0x80U;
                        *(s8 *)((unsigned char *)var_s1_2 + (13)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (15)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (6)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (10)) = 0;
                        var_a1_2 = *(u16 *)((unsigned char *)sp90 + (4));
                    } else {
                        goto block_106;
                    }
                    break;
                case 0xAC:
                    temp_s2 = var_s0_8 + 0x14;
                    if (func_80145508((s32) (var_s1_2 + 0x14), (s32) &D_80155B36, (s32) temp_s2, *(s32 *)((unsigned char *)var_s0_8 + (76))) != 0) {
                        temp_t4_3 = *(u8 *)((unsigned char *)temp_s5 + (37)) + 1;
                        *(u8 *)((unsigned char *)temp_s5 + (37)) = temp_t4_3;
                        if ((*(u8 *)((unsigned char *)temp_s5 + (36)) != 0) && (temp_t4_3 & 1)) {
                            func_80133D14((s32) *(s16 *)((unsigned char *)var_s0_8 + (20)), (s32) *(s16 *)((unsigned char *)var_s0_8 + (22)), 0x20, 0xA0);
                        }
                        (*(s32 *)var_s0_8) = 0;
                        func_8014C5F8((s32) temp_s2);
                        *(u16 *)((unsigned char *)var_s1_2 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1_2 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (13)) = 0;
                    }
                    goto block_105;
                }
            } else {
block_105:
                var_s3_4 += 1;
block_106:
                var_s0_8 += 0x68;
                if (var_s3_4 >= 0x80) {
                    var_a1_2 = *(u16 *)((unsigned char *)sp90 + (4));
                } else {
                    goto loop_90;
                }
            }
            if ((var_a1_2 == 0x30) || (var_a1_2 == 0x40)) {
                var_s0_9 = D_80158A90;
                var_s3_5 = 0;
                do {
                    if (((*(s32 *)var_s0_9) != 0) && ((u16) *(u16 *)((unsigned char *)var_s0_9 + (4)) < 0x90U) && (func_80145508((s32) (var_s1_2 + 0x14), (s32) &D_80155B36, (s32) (var_s0_9 + 0x14), (s32) &D_80155B48) != 0)) {
                        func_8013C41C((s32) var_s0_9, *(s16 *)((unsigned char *)var_s1_2 + (20)) - *(s16 *)((unsigned char *)var_s0_9 + (20)));
                        *(u16 *)((unsigned char *)var_s1_2 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1_2 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1_2 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1_2 + (13)) = 0;
                    }
                    var_s3_5 += 1;
                    var_s0_9 += 0x6C;
                } while (var_s3_5 != 0x20);
            }
        }
        temp_t4 = sp6C + 1;
        sp6C = temp_t4;
        var_s1_2 += 0x54;
    } while (temp_t4 != 0xC);
}
