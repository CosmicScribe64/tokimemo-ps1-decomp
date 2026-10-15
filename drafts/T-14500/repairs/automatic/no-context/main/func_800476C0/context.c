#include "common.h"
#include "game.h"

typedef struct TextureRecord44 {
    /* 0x00 */ u8 pad[0x34];
    /* 0x34 */ s32 *pages;
    /* 0x38 */ u8 pad38[0x0C];
} TextureRecord44; /* size 0x44; texture-page table view, T-10030 */

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80047560);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6280;
extern s32 D_800E7318;
extern s32 D_800E8C70;
extern s32 D_8011ECA0;
extern s32 D_8011ECD0;
extern u32 D_800E7378;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;
extern u8 D_800E738D;
s32 func_80098680();
s32 printf();
s32 search_tpage();
s32 search_tpage8();
u8 func_8004901C();
/* end bulk_m2c declarations */
void func_800476C0(void) {
    s32 *spEC;
    s32 spE8;
    s16 spE6;
    s16 spE4;
    s16 spE2;
    s16 spE0;
    u8 spDE;
    u8 spDD;
    u8 spDC;
    s16 spDA;
    s16 spD8;
    s8 spD7;
    s8 spD6;
    s16 spD4;
    s16 spD2;
    s16 spD0;
    s16 spCE;
    s16 spCC;
    s32 spC8;
    s32 spC4;
    s32 spC0;
    s32 sp90;
    u16 sp8E;
    u8 sp8D;
    u8 *sp7C;
    s32 *var_s1;
    s16 *var_v1_4;
    s16 temp_a1_2;
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_fp;
    s16 temp_s5;
    s16 temp_s6;
    s16 temp_s7;
    s16 temp_t0;
    s16 var_t8;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 *temp_v1;
    s32 temp_a0_3;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_v0;
    s32 temp_v0_10;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_9;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2;
    s32 var_s3;
    s32 var_t0;
    s32 var_t2;
    s32 var_t9;
    s32 var_v1;
    u16 var_s2;
    u32 temp_v0_4;
    u32 temp_v0_5;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 temp_a0_4;
    u8 temp_a0_5;
    u8 temp_a1;
    u8 temp_a1_3;
    u8 temp_t6;
    u8 temp_v0_6;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 temp_v1_4;
    u8 temp_v1_5;
    u8 var_v0_2;
    u8 *temp_s0;
    u8 *var_v0;

    spEC = &D_800E6280;
    sp8D = func_8004901C();
    search_tpage(0x7FFF, sp90, 0);
    sp90 = 0;
    if (D_800E7318 >= 0) {
        var_s2 = sp8E;
        var_s1 = &D_8011ECD0;
        do {
            temp_a1 = *(u8 *)((unsigned char *)var_s1 + (3));
            if (temp_a1 & 0x80) {
                if ((((temp_a1 & 0x40) != 0x40) || (((u32) D_800E7378 % (u32) (*(u8 *)((unsigned char *)var_s1 + (6)) + 1)) != 0)) && (temp_a0 = *(u8 *)((unsigned char *)var_s1 + (2)), ((temp_a0 & 0x80) == 0))) {
                    if (!(**(s32 **)((unsigned char *)var_s1 + (52)) & 0x80000000) || (temp_v1 = *(s32 **)((unsigned char *)var_s1 + (12)), ((*temp_v1 & 0x80000000) == 0))) {
                        printf("cls%x[m%xs%xs%xf%x] ", sp90, D_800E7388, D_800E7389, (s32) D_800E738A, (s32) D_800E738D);
                        var_v1 = *(s32 *)((unsigned char *)spEC + (4248));
                    } else {
                        var_a2 = temp_a0 & 0x40;
                        if (var_a2 != 0) {
                            var_v0 = *(temp_v1 + (*(s16 *)((unsigned char *)var_s1 + (24)) * 4));
                        } else {
                            temp_t0 = *(s16 *)((unsigned char *)var_s1 + (22));
                            temp_a3 = *(s16 *)((unsigned char *)var_s1 + (24));
                            var_v0 = *(temp_v1 + (*(s16 *)((unsigned char *)(*(s32 *)(*(s32 *)((unsigned char *)var_s1 + (16)) + (temp_t0 * 4)) + (temp_a3 * 2 * 2)) + (2)) * 4));
                            spC0 = (s32) temp_t0;
                            spC4 = (s32) temp_a3;
                        }
                        temp_a1_2 = (*(s32 *)var_v0);
                        temp_s5 = *(s16 *)((unsigned char *)var_v0 + (2));
                        temp_s6 = *(s16 *)((unsigned char *)var_v0 + (4));
                        if (temp_a1_2 != 0) {
                            temp_v1_2 = temp_a1_2 - 1;
                            var_s3 = temp_v1_2;
                            if (temp_v1_2 >= 0) {
                                sp7C = var_v0 + 8;
                                temp_s7 = *(s16 *)((unsigned char *)var_s1 + (38)) + temp_s5;
                                temp_fp = *(s16 *)((unsigned char *)var_s1 + (42)) + temp_s6;
                                do {
                                    temp_a1_3 = *(u8 *)((unsigned char *)var_s1 + (3));
                                    var_t0 = temp_a1_3 & 8;
                                    if (var_t0 != 0) {
                                        var_t2 = var_s3 * 8;
                                        if (var_s3 >= 4) {
                                            var_s3 = 3;
                                            goto block_18;
                                        }
                                    } else {
block_18:
                                        var_t2 = var_s3 * 8;
                                    }
                                    temp_v1_3 = *(s32 *)((unsigned char *)var_s1 + (56));
                                    spC8 = temp_v1_3;
                                    temp_s0 = sp7C + ((var_t2 - var_s3) * 2);
                                    if (!(temp_a1_3 & 1)) {
                                        if (*(s16 *)((unsigned char *)temp_s0 + (8)) & 2) {
                                            spCC = temp_s7;
                                            spE0 = (*(s16 *)((unsigned char *)temp_s0 + (10)) - temp_s5) + (*(s32 *)temp_s0);
                                            spE4 = -0x1000;
                                        } else {
                                            spCC = temp_s7;
                                            spE4 = 0x1000;
                                            spE0 = temp_s5 - (*(s32 *)temp_s0);
                                        }
                                        if (*(s16 *)((unsigned char *)temp_s0 + (8)) & 1) {
                                            spCE = temp_fp;
                                            spE2 = (*(s16 *)((unsigned char *)temp_s0 + (12)) - temp_s6) + *(s16 *)((unsigned char *)temp_s0 + (2));
                                            spE6 = -0x1000;
                                        } else {
                                            spCE = temp_fp;
                                            spE6 = 0x1000;
                                            spE2 = temp_s6 - *(s16 *)((unsigned char *)temp_s0 + (2));
                                        }
                                    } else {
                                        if (*(s16 *)((unsigned char *)temp_s0 + (8)) & 2) {
                                            spCC = temp_s7;
                                            spE0 = (*(s16 *)((unsigned char *)temp_s0 + (10)) - temp_s5) + (*(s32 *)temp_s0);
                                            spE4 = 0x1000;
                                        } else {
                                            spCC = temp_s7;
                                            spE4 = -0x1000;
                                            spE0 = temp_s5 - (*(s32 *)temp_s0);
                                        }
                                        if (*(s16 *)((unsigned char *)temp_s0 + (8)) & 1) {
                                            spCE = temp_fp;
                                            spE2 = (*(s16 *)((unsigned char *)temp_s0 + (12)) - temp_s6) + *(s16 *)((unsigned char *)temp_s0 + (2));
                                            spE6 = 0x1000;
                                        } else {
                                            spCE = temp_fp;
                                            spE6 = 0x1000;
                                            spE2 = temp_s6 - *(s16 *)((unsigned char *)temp_s0 + (2));
                                        }
                                    }
                                    if (temp_a1_3 & 4) {
                                        spCC -= temp_s5;
                                        spCE -= temp_s6;
                                    }
                                    temp_a3_2 = *(s16 *)((unsigned char *)var_s1 + (26));
                                    if ((temp_a3_2 != 0x1000) && (*(s16 *)((unsigned char *)var_s1 + (28)) != 0x1000)) {
                                        spD0 = *(s16 *)((unsigned char *)temp_s0 + (10)) + 1;
                                        spD2 = *(s16 *)((unsigned char *)temp_s0 + (12)) + 1;
                                    } else {
                                        spD0 = *(s16 *)((unsigned char *)temp_s0 + (10));
                                        spD2 = *(s16 *)((unsigned char *)temp_s0 + (12));
                                    }
                                    if ((*(u8 *)((unsigned char *)var_s1 + (4)) & 8) == 8) {
                                        temp_v0 = temp_v1_3 & 0x03000000;
                                        if (temp_v0 != 0) {
                                            var_v1_2 = var_s2 & 0x1F;
                                            if (temp_v0 != 0x01000000) {
                                                var_a0 = (var_s2 & 0x20) * 4;
                                            } else {
                                                var_s2 = search_tpage8(*(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 1, temp_a3_2) & 0xFFFF;
                                                var_a0 = (var_s2 & 0x20) * 4;
                                                var_v1_2 = var_s2 & 0x1F;
                                                goto block_44;
                                            }
                                        } else {
                                            var_s2 = search_tpage8(*(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 2, temp_a3_2) & 0xFFFF;
                                            var_a0 = (var_s2 & 0x20) * 4;
                                            var_v1_2 = var_s2 & 0x1F;
block_44:
                                            var_t0 = *(u8 *)((unsigned char *)var_s1 + (3)) & 8;
                                        }
                                        spD4 = var_v1_2;
                                        temp_t4 = *(s16 *)((unsigned char *)temp_s0 + (4)) & 0xFF;
                                        spD6 = (temp_t4 & 0xF) * 8;
                                        spD7 = ((temp_t4 >> 4) * 8) + var_a0;
                                    } else {
                                        temp_v0_2 = temp_v1_3 & 0x03000000;
                                        if (temp_v0_2 != 0) {
                                            var_v1_3 = var_s2 & 0x1F;
                                            if (temp_v0_2 != 0x01000000) {
                                                var_a0_2 = (var_s2 & 0x20) * 4;
                                            } else {
                                                var_s2 = search_tpage(*(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 1, temp_a3_2) & 0xFFFF;
                                                var_a0_2 = (var_s2 & 0x20) * 4;
                                                var_v1_3 = var_s2 & 0x1F;
                                                goto block_51;
                                            }
                                        } else {
                                            var_s2 = search_tpage(*(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 2, temp_a3_2) & 0xFFFF;
                                            var_a0_2 = (var_s2 & 0x20) * 4;
                                            var_v1_3 = var_s2 & 0x1F;
block_51:
                                            var_t0 = *(u8 *)((unsigned char *)var_s1 + (3)) & 8;
                                        }
                                        spD4 = var_v1_3;
                                        temp_t7 = *(s16 *)((unsigned char *)temp_s0 + (4)) & 0x3F;
                                        spD6 = (temp_t7 & 7) * 0x10;
                                        spD7 = ((temp_t7 >> 3) * 0x10) + var_a0_2;
                                    }
                                    temp_a0_2 = *(u8 *)((unsigned char *)var_s1 + (67));
                                    temp_v0_3 = temp_a0_2 & 0x60;
                                    if (temp_a0_2 != 0) {
                                        spD8 = 0;
                                        switch (temp_v0_3) {
                                        case 0x20:
                                            spD8 = 0x100;
                                        case 0:
                                        case 3:
                                        case 5:
                                        case 6:
                                        case 7:
                                        case 9:
                                        case 10:
                                        case 11:
                                        case 12:
                                        case 13:
                                        case 14:
                                        case 15:
                                        case 16:
                                        case 17:
                                        case 18:
                                        case 19:
                                        case 20:
                                        case 21:
                                        case 22:
                                        case 23:
                                        case 24:
                                        case 25:
                                        case 26:
                                        case 27:
                                        case 28:
                                        case 29:
                                        case 30:
                                        case 31:
                                        case 32:
block_70:
                                            var_t9 = temp_a0_2 & 0x1F;
                                            break;
                                        case 0x40:
                                            spD8 = 0x120;
                                            goto block_70;
                                        default:
                                            temp_v1_4 = *(u8 *)((unsigned char *)var_s1 + (5));
                                            temp_v0_4 = temp_v1_4 & 0x7F;
                                            if (temp_v1_4 & 0x80) {
                                                switch (temp_v0_4) {
                                                default:
                                                    var_t9 = temp_a0_2 & 0x1F;
                                                    if (temp_v0_4 != 0x40) {
                                                        goto block_70;
                                                    }
                                                    break;
                                                case 1:
                                                    spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF0) >> 4) * 0x10;
                                                    goto block_70;
                                                case 2:
                                                    var_t8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xE0) >> 4) * 0x10;
block_69:
                                                    spD8 = var_t8;
                                                    goto block_70;
                                                case 4:
                                                    spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xC0) >> 4) * 0x10;
                                                    goto block_70;
                                                case 8:
                                                    var_t8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0x80) >> 4) * 0x10;
                                                    goto block_69;
                                                }
                                            } else {
                                                goto block_70;
                                            }
                                            break;
                                        }
                                        spDA = var_t9 + 0x1E0;
                                    } else {
                                        temp_v1_5 = *(u8 *)((unsigned char *)var_s1 + (5));
                                        temp_v0_5 = temp_v1_5 & 0x7F;
                                        switch (temp_v0_5) {
                                        case 0x1:
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF0) >> 4) * 0x10;
                                            break;
                                        case 0x2:
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xE0) >> 4) * 0x10;
                                            break;
                                        case 0x4:
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xC0) >> 4) * 0x10;
                                            break;
                                        case 0x8:
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0x80) >> 4) * 0x10;
                                            break;
                                        default:
                                            spD8 = 0;
                                            break;
                                        }
                                        if (temp_v1_5 & 0x80) {
                                            spDA = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF00) >> 8) + 0x1F0;
                                        } else {
                                            spDA = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF00) >> 8) + 0x1E0;
                                        }
                                    }
                                    if (*(u8 *)((unsigned char *)var_s1 + (3)) & 0x20) {
                                        var_v0_2 = sp8D;
                                        spDC = var_v0_2;
                                        spDD = var_v0_2;
                                        goto block_92;
                                    }
                                    temp_v0_6 = *(u8 *)((unsigned char *)var_s1 + (64));
                                    if ((temp_v0_6 != 0) || (*(u8 *)((unsigned char *)var_s1 + (65)) != 0) || (*(u8 *)((unsigned char *)var_s1 + (66)) != 0)) {
                                        spDC = temp_v0_6;
                                        spDD = *(u8 *)((unsigned char *)var_s1 + (65));
                                        spDE = *(u8 *)((unsigned char *)var_s1 + (66));
                                    } else {
                                        var_v0_2 = *(u8 *)((unsigned char *)var_s1 + (7));
                                        spDC = var_v0_2;
                                        spDD = var_v0_2;
block_92:
                                        spDE = var_v0_2;
                                    }
                                    temp_t2 = *(s32 *)((unsigned char *)var_s1 + (32));
                                    spE8 = temp_t2;
                                    if (var_t0 != 0) {
                                        if ((temp_t2 == 0) && (spE4 == 0x1000)) {
                                            if (spE6 != 0x1000) {
                                                goto block_98;
                                            }
                                        } else {
block_98:
                                            spD0 = (u16) spD0 - 1;
                                            spD2 = (u16) spD2 - 1;
                                        }
                                    }
                                    if ((*(s16 *)((unsigned char *)var_s1 + (26)) != 0x1000) || (*(s16 *)((unsigned char *)var_s1 + (28)) != 0x1000)) {
                                        spE4 = *(s16 *)((unsigned char *)var_s1 + (26)) * (spE4 / 4096);
                                        spE6 = *(s16 *)((unsigned char *)var_s1 + (28)) * (spE6 / 4096);
                                    }
                                    temp_v0_7 = *(u8 *)((unsigned char *)var_s1 + (1));
                                    if (temp_v0_7 == 0) {
                                        func_80098680(&spC8, (D_8011ECA0 * 0x14) + &D_800E8C70, 0, *(s16 *)((unsigned char *)var_s1 + (26)));
                                    } else {
                                        func_80098680(&spC8, (D_8011ECA0 * 0x14) + &D_800E8C70, (temp_v0_7 - 1) & 0xFFFF, *(s16 *)((unsigned char *)var_s1 + (26)));
                                    }
                                    var_s3 -= 1;
                                } while (var_s3 >= 0);
                                var_a2 = *(u8 *)((unsigned char *)var_s1 + (2)) & 0x40;
                            }
                        }
                        if ((var_a2 == 0) && ((*(u8 *)((unsigned char *)var_s1 + (2)) & 1) == 1)) {
                            temp_v0_8 = *(u8 *)((unsigned char *)var_s1 + (6));
                            temp_t8_2 = spC0 * 4;
                            if ((temp_v0_8 & 0x80) && (temp_a0_3 = temp_v0_8 & 0x7F, (temp_a0_3 != 0))) {
                                *(s16 *)((unsigned char *)var_s1 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (8)) + temp_a0_3);
                                var_v1_4 = *(s32 *)(*(s32 *)((unsigned char *)var_s1 + (16)) + temp_t8_2);
                            } else if (temp_v0_8 != 0) {
                                if (((u32) D_800E7378 % temp_v0_8) == 0) {
                                    *(s16 *)((unsigned char *)var_s1 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (8)) + 1);
                                }
                                var_v1_4 = *(s32 *)(*(s32 *)((unsigned char *)var_s1 + (16)) + temp_t8_2);
                            } else {
                                var_v1_4 = *(s32 *)(*(s32 *)((unsigned char *)var_s1 + (16)) + temp_t8_2);
                            }
                            if (*(s16 *)((unsigned char *)var_s1 + (8)) >= *(s16 *)((unsigned char *)(var_v1_4 + (spC4 * 2 * 2)) + (4))) {
                                temp_a0_4 = *(u8 *)((unsigned char *)var_s1 + (2));
                                if ((temp_a0_4 & 0x10) == 0x10) {
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) + 1);
                                    *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (temp_a0_4 & ~0x20);
                                } else if ((temp_a0_4 & 0x20) == 0x20) {
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) - 1);
                                    *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                                    if (*(s16 *)((unsigned char *)var_s1 + (24)) <= 0) {
                                        *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (*(u8 *)((unsigned char *)var_s1 + (2)) & 0xFFDE);
                                    }
                                } else {
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) + 1);
                                    *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                                }
                            }
                            if (*(s16 *)((unsigned char *)var_s1 + (24)) >= *var_v1_4) {
                                temp_a0_5 = *(u8 *)((unsigned char *)var_s1 + (2));
                                if ((temp_a0_5 & 2) == 2) {
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = 0;
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (temp_a0_5 & ~3);
                                } else if ((temp_a0_5 & 4) == 4) {
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (temp_a0_5 & 0xFFFA);
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) - 1);
                                } else if ((temp_a0_5 & 8) == 8) {
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = 0;
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (temp_a0_5 & 0xFFFC);
                                    *(u8 *)((unsigned char *)var_s1 + (3)) = (u8) (*(u8 *)((unsigned char *)var_s1 + (3)) & ~0x80);
                                } else if ((temp_a0_5 & 0x10) == 0x10) {
                                    temp_t6 = temp_a0_5 & ~0x10;
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = temp_t6;
                                    *(u8 *)((unsigned char *)var_s1 + (2)) = (u8) (temp_t6 | 0x20);
                                    if (*(u8 *)((unsigned char *)var_s1 + (3)) & 8) {
                                        if (*var_v1_4 >= 3) {
                                            *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) - 3);
                                        } else {
                                            goto block_139;
                                        }
                                    } else if (*var_v1_4 >= 2) {
                                        *(s16 *)((unsigned char *)var_s1 + (24)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (24)) - 1);
                                    } else {
                                        goto block_139;
                                    }
                                } else {
block_139:
                                    *(s16 *)((unsigned char *)var_s1 + (24)) = 0;
                                }
                            }
                        }
                        if (*(u8 *)((unsigned char *)var_s1 + (3)) & 0x10) {
                            temp_v0_9 = *(s32 *)((unsigned char *)var_s1 + (44));
                            if (temp_v0_9 != 0) {
                                *(s32 *)((unsigned char *)var_s1 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s1 + (36)) + temp_v0_9);
                            }
                            temp_v0_10 = *(s32 *)((unsigned char *)var_s1 + (48));
                            if (temp_v0_10 != 0) {
                                *(s32 *)((unsigned char *)var_s1 + (40)) = (s32) (*(s32 *)((unsigned char *)var_s1 + (40)) + temp_v0_10);
                            }
                        }
                        goto block_146;
                    }
                } else {
                    goto block_146;
                }
            } else {
block_146:
                var_v1 = *(s32 *)((unsigned char *)spEC + (4248));
            }
            var_s1 += 0x44;
            temp_t8 = sp90 + 1;
            sp90 = temp_t8;
        } while (var_v1 >= temp_t8);
        sp8E = var_s2;
    }
}

void tpage_buf_clear_all(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
}

void tpage_buf_clear(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
    _sys_default_tpage_set();
}

void _sys_default_tpage_set(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        D_800E6280.unk_1228[i + 11] = 0x77;
        D_800E6280.unk_1228[i + 43] = 0x77;
        D_800E6280.unk_1228[i + 21] = 0x77;
        D_800E6280.unk_1228[i + 53] = 0x77;
    }
    D_800E6280.unk_1228[60] = *(s32 *)D_800C9730;
    D_800E6280.unk_1228[61] = D_800C9734;
    D_800E6280.unk_1228[62] = D_800C9738;
    D_800E6280.unk_1228[63] = D_800C973C;
    D_800E6280.unk_1228[31] = D_800C974C;
    D_800E6280.unk_1228[30] = D_800C9750;
    D_800E6280.unk_1228[29] = D_800C9754;
    D_800E6280.unk_1228[28] = D_800C9758;
    D_800E6280.unk_1228[27] = D_800C5FCC;
    D_800E6280.unk_1228[59] = D_800C5FD0;
}

/* NON_MATCHING: T-12010, 29 differing lines; narrow argument shuffle. */
#ifdef NON_MATCHING
void load_tpage_buf_lock(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
u8 arg2;
{
    u32 idx;
    D_800E6280.unk_1228[arg0] = arg1;
    idx = arg0;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, (u8)(idx & 0x1F), (s32)arg2, 0, 0, 0x80, 0x80);
    } else {
        load_csr_tp(arg1, arg0 & 0x1F, (s32)arg2, 0, 0x80, 0x80, 0x80);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80047550", load_tpage_buf_lock);
#endif

s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1) {
    GameState *var_v0;
    s32 var_v1;

    /* FAKE: shifted cursor keeps the target's base + 0x1228 addressing, T-9580. */
    var_v0 = (GameState *)((u8 *)&D_800E6280 + 0x14);
    for (var_v1 = 5; var_v1 < 0x40; var_v1++) {
        if (var_v0->unk_1228[0] == -1) {
            var_v0->unk_1228[0] = arg0;
            if (var_v1 < 0x20) {
                load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0, 0x80, 0x80);
                return var_v1;
            }
            load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0x80, 0x80, 0x80);
            return var_v1;
        }
        var_v0 = (GameState *)((u8 *)var_v0 + 4);
    }
    return -1;
}

s32 search_tpage_multi(s32 arg0, u8 arg1) {
    s32 j;
    s32 i;

    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE && D_800E6280.unk_1228[i + j * 16] == arg0) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_800E6280.unk_1128[i]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = arg0;
            if (i < 0x20) {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    TextureRecord44 *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.unk_1228[i] == -1) {
                D_800E6280.unk_1128[i] = 0x7FFF;
            } else {
                D_800E6280.unk_1128[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((TextureRecord44 *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 6);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE &&
                D_800E6280.unk_1228[i + j * 16] == record->pages[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == record->pages[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = record->pages[index];
            if (i < 0x20) {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage8(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    TextureRecord44 *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.unk_1228[i] == -1) {
                D_800E6280.unk_1128[i] = 0x7FFF;
            } else {
                D_800E6280.unk_1128[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((TextureRecord44 *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 8);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE &&
                D_800E6280.unk_1228[i + j * 16] == record->pages[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == record->pages[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = record->pages[index];
            if (i < 0x20) {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

void func_80048CF8(void) {
    s32 i;

    if (D_800E6280.unk_F80 & 2) {
        printf("PRINT_TPAGE_BUF\n");
        for (i = 0; i < 0x40; i++) {
            printf("[%d]%08x", i, D_800E6280.unk_1228[i]);
            if ((i & 3) == 3) {
                printf("\r");
            }
        }
    }
}

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_1112 = 1;
    } else {
        D_800E6280.unk_1112 = 0;
    }
}

void func_80048DD0(s32 count) {
    s32 i;

    printf("%d CLASS_CLEAR \n", count);
    if (count != 0) {
        for (i = 0; i < count && i < 160; i++) {
            D_8011ECD0[i * 0x44 + 3] = 0;
        }
    } else {
        for (i = 0; i < 160; i++) {
            D_8011ECD0[i * 0x44 + 3] = 0;
        }
    }
}

void func_80048E78(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
}

void func_80048EB8(s32 arg0) {
    s32 n;
    s32 i;
    s32 a;
    s32 b;
    s32 c;

    a = D_8011ECF6;
    b = D_8011ECFA;
    c = D_8011ECE8;
    if (arg0 == 0) {
        n = 0xA0;
    } else {
        n = (arg0 >= 0xA1) ? 0xA0 : arg0;
    }
    for (i = 1; i < n; i++) {
        func_80048F64(i);
    }
    D_8011ECF6 = a;
    D_8011ECFA = b;
    D_8011ECE8 = c;
}

void func_80048F64(s32 arg0) {
    u8 *p;

    if (arg0 < 0xA0) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0] = 0;
        *(s32 *)(p + 0x38) = 0;
        p[1] = 0;
        p[4] = 0;
        p[5] = 0;
        p[2] = 0;
        p[3] = 0;
        p[6] = 1;
        *(s16 *)(p + 8) = 0;
        *(s32 *)(p + 0xC) = (s32)sprite_frame_table;
        *(s32 *)(p + 0x10) = (s32)D_800C9A14;
        *(s32 *)(p + 0x34) = (s32)D_800C9730;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0x1A) = 0x1000;
        *(s16 *)(p + 0x1C) = 0x1000;
        p[0x43] = 0;
        p[7] = 0x80;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x30) = 0;
        p[0x40] = 0;
        p[0x41] = 0;
        p[0x42] = 0;
    }
}

u8 func_8004901C(void) {
    return D_800E6280.unk_03A;
}

void func_8004902C(void) {
    D_800E6280.unk_03A = 0x80;
    D_800E6280.unk_03B = 0;
}

void func_80049044(void) {
    u8 *p;
    s32 v;

    p = &D_800E6280.unk_03A;
    if ((s8)D_800E6280.unk_03B != 0) {
        if ((s8)D_800E6280.unk_03B > 0) {
            v = *p;
            v += (s8)D_800E6280.unk_03B;
            if (v >= 0x100) {
                v = 0xFF;
            }
            *p = v;
        } else if ((s8)D_800E6280.unk_03B < 0) {
            v = *p;
            v += (s8)D_800E6280.unk_03B;
            if (v < 0) {
                v = 0;
            }
            *p = v;
        }
    }
}

void func_800490B4(u8 arg0) {
    D_800E6280.unk_03B = arg0;
}
