#include "common.h"
#include "ovl/TT.h"

/* NON_MATCHING: T-11000, register-only (~60 lines, identical instruction stream incl. 4x unroll): original skips v1/a0, i in a1, 0x40/0x70/0x20 in a2/a3/t0; IDO uses v1, a0, a1, a2 (phantom a0, T-0018) */
#ifdef NON_MATCHING
void func_80142570(void) {
    u8 *p;
    s32 i;

    func_80133AD0(0x60C);
    p = D_80158A98;
    i = 0;
    do {
        i++;
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    } while (i < 0x40);
    p = D_80158A9C;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142570);
#endif

void func_801426F0(void) {
    s32 i;
    u8 *p = D_80158A8C;
    u8 *r = D_80158AA4 + 0xC80;

    p[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

u8 func_801427A8(void) {
    u8 *sp78;
    u8 *sp70;
    s32 sp68;
    u8 sp5F;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_t9;
    s32 var_s0;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_v1;
    u8 *temp_s5;
    u8 *temp_s6;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 *var_s4;
    u8 *var_s4_2;
    u8 *var_v0;
    u8 temp_v0;

    temp_s6 = D_80158A64;
    temp_s5 = D_80158A8C;
    var_s1 = D_80158A80;
    sp5F = 0;
    sp68 = 0;
    sp78 = D_80158A6C;
    sp70 = D_80158A74;
    do {
        if ((*(s32 *)var_s1) != 0) {
            var_s3 = 0;
            if (*(u16 *)((unsigned char *)var_s1 + (4)) == 0x40) {
                var_s4 = temp_s5;
                do {
                    temp_a3 = *(s32 *)((unsigned char *)var_s4 + (536));
                    if ((temp_a3 != 0) && (func_80145508((s32) (var_s1 + 0x14), (s32) &D_80155B36, (s32) (temp_s5 + 0x14), temp_a3) != 0)) {
                        *(u8 *)((unsigned char *)temp_s6 + (37)) = (u8) (*(u8 *)((unsigned char *)temp_s6 + (37)) + 1);
                        sp5F = 1;
                        var_v1 = 0;
                        if (*(u8 *)((unsigned char *)temp_s6 + (36)) != 0) {
                            temp_v0 = *(u8 *)((unsigned char *)temp_s6 + (37));
                            var_v1 = 0;
                            if (!(temp_v0 & 3)) {
                                if ((u8) *(u8 *)((unsigned char *)sp78 + (11)) < 2U) {
                                    if (!(temp_v0 & 7)) {
                                        func_801342CC((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x100, 0x20, 0xA0);
                                    } else {
                                        func_80133FB0((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x100, 0x20, 0xA0);
                                    }
                                } else {
                                    func_80133D14((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x20, 0xA0);
                                }
                                var_v1 = 0;
                            }
                        }
                        var_v0 = temp_s5;
                        do {
                            var_v1 += 4;
                            *(s8 *)((unsigned char *)var_v0 + (86)) = 1;
                            *(s8 *)((unsigned char *)var_v0 + (106)) = 1;
                            *(s8 *)((unsigned char *)var_v0 + (126)) = 1;
                            var_v0 += 0x50;
                            *(s8 *)((unsigned char *)var_v0 + (-14)) = 1;
                        } while (var_v1 != 0x18);
                        if (sp68 >= 6) {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 1);
                        } else if (*(u16 *)((unsigned char *)var_s1 + (2)) == 9) {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 4);
                        } else {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 2);
                        }
                        *(u16 *)((unsigned char *)var_s1 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (13)) = 0;
                    }
                    var_s3 += 4;
                    var_s4 += 4;
                } while (var_s3 < 0x10);
                var_s3_2 = 0x10;
                var_s4_2 = temp_s5 + 0x10;
                do {
                    temp_a3_2 = *(s32 *)((unsigned char *)var_s4_2 + (536));
                    if ((temp_a3_2 != 0) && (func_80145508((s32) (var_s1 + 0x14), (s32) &D_80155B36, (s32) (temp_s5 + 0x14), temp_a3_2) != 0)) {
                        *(u16 *)((unsigned char *)var_s1 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (13)) = 0;
                    }
                    var_s3_2 += 4;
                    var_s4_2 += 4;
                } while (var_s3_2 != 0x20);
            }
        }
        var_s1 += 0x54;
        temp_t9 = sp68 + 1;
        sp68 = temp_t9;
    } while (temp_t9 < 0xC);
    sp68 = 4;
    var_s0 = 0x10;
    var_s1_2 = temp_s5 + 0x10;
    do {
        temp_a3_3 = *(s32 *)((unsigned char *)var_s1_2 + (536));
        if ((temp_a3_3 != 0) && (*(u8 *)((unsigned char *)sp70 + (82)) == 0) && ((*(s32 *)sp70) != 0) && (func_80145508((s32) (sp70 + 0x14), (s32) D_80155B30, (s32) (temp_s5 + 0x14), temp_a3_3) != 0)) {
            func_80144094();
            *(s16 *)((unsigned char *)sp70 + (4)) = 0x60;
            *(s16 *)((unsigned char *)sp70 + (10)) = 0;
            *(s16 *)((unsigned char *)sp70 + (8)) = 0;
            *(s16 *)((unsigned char *)sp70 + (6)) = 0;
            *(s8 *)((unsigned char *)sp70 + (15)) = 0;
            *(s8 *)((unsigned char *)sp70 + (14)) = 0;
            *(s8 *)((unsigned char *)sp70 + (13)) = 0;
        }
        var_s0 += 4;
        var_s1_2 += 4;
    } while (var_s0 != 0x30);
    return sp5F;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142AF0);

void func_80142C70(void) {
    s32 i;
    u8 *base;

    base = D_80158A8C;
    if (base[0] != 0) {
        for (i = 0; i != 0x18; i++) {
            if (base[i + 0x310] != 0) {
                *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22) + *(s16 *)(base + 0x248 + i * 2);
                *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26) + *(s16 *)(base + 0x278 + i * 2);
                func_80147A18(base + 0x14, base + 0x38 + i * 0x14, *(u16 *)(base + 0x2A8 + i * 2));
            }
        }
        *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22);
        *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26);
    }
}
