void func_800476C0(void) {
    GameState *spEC;
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
    void *sp7C;
    DisplayActor44 *var_s1;
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
    u8 *temp_v1;
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
    void *temp_s0;
    void *var_v0;

    spEC = &D_800E6280;
    sp8D = func_8004901C();
    search_tpage(0x7FFF, sp90, 0, M2C_ERROR(/* Read from unset register $a3 */));
    sp90 = 0;
    if (*(s32 *)((unsigned char *)(&D_800E6280) + (4248)) >= 0) {
        var_s2 = sp8E;
        var_s1 = D_8011ECD0;
        do {
            temp_a1 = var_s1->unk_03;
            if (temp_a1 & 0x80) {
                if ((((temp_a1 & 0x40) != 0x40) || (((u32) D_800E6280.unk_10F8 % (u32) (var_s1->unk_06 + 1)) != 0)) && (temp_a0 = var_s1->unk_02, ((temp_a0 & 0x80) == 0))) {
                    if (!(*var_s1->unk_34 & 0x80000000) || (temp_v1 = var_s1->unk_0C, ((*temp_v1 & 0x80000000) == 0))) {
                        printf("cls%x[m%xs%xs%xf%x] ", sp90, D_800E6280.unk0, D_800E6280.unk_1109, (s32) D_800E6280.unk_110A, (s32) D_800E6280.unk_110D);
                        var_v1 = *(s32 *)((unsigned char *)spEC + (4248));
                    } else {
                        var_a2 = temp_a0 & 0x40;
                        if (var_a2 != 0) {
                            var_v0 = *(temp_v1 + (var_s1->unk_18 * 4));
                        } else {
                            temp_t0 = var_s1->unk_16;
                            temp_a3 = var_s1->unk_18;
                            var_v0 = *(temp_v1 + (*(s16 *)((unsigned char *)(*(var_s1->unk_10 + (temp_t0 * 4)) + (temp_a3 * 2 * 2)) + (2)) * 4));
                            spC0 = (s32) temp_t0;
                            spC4 = (s32) temp_a3;
                        }
                        temp_a1_2 = var_v0->unk0;
                        temp_s5 = *(s16 *)((unsigned char *)var_v0 + (2));
                        temp_s6 = *(s16 *)((unsigned char *)var_v0 + (4));
                        if (temp_a1_2 != 0) {
                            temp_v1_2 = temp_a1_2 - 1;
                            var_s3 = temp_v1_2;
                            if (temp_v1_2 >= 0) {
                                sp7C = var_v0 + 8;
                                temp_s7 = var_s1->unk_26 + temp_s5;
                                temp_fp = var_s1->unk_2A + temp_s6;
                                do {
                                    temp_a1_3 = var_s1->unk_03;
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
                                    temp_v1_3 = var_s1->unk_38;
                                    spC8 = temp_v1_3;
                                    temp_s0 = sp7C + ((var_t2 - var_s3) * 2);
                                    if (!(temp_a1_3 & 1)) {
                                        if (*(s16 *)((unsigned char *)temp_s0 + (8)) & 2) {
                                            spCC = temp_s7;
                                            spE0 = (*(s16 *)((unsigned char *)temp_s0 + (10)) - temp_s5) + temp_s0->unk0;
                                            spE4 = -0x1000;
                                        } else {
                                            spCC = temp_s7;
                                            spE4 = 0x1000;
                                            spE0 = temp_s5 - temp_s0->unk0;
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
                                            spE0 = (*(s16 *)((unsigned char *)temp_s0 + (10)) - temp_s5) + temp_s0->unk0;
                                            spE4 = 0x1000;
                                        } else {
                                            spCC = temp_s7;
                                            spE4 = -0x1000;
                                            spE0 = temp_s5 - temp_s0->unk0;
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
                                    temp_a3_2 = var_s1->unk_1A;
                                    if ((temp_a3_2 != 0x1000) && (*(s16 *)((unsigned char *)var_s1 + (28)) != 0x1000)) {
                                        spD0 = *(s16 *)((unsigned char *)temp_s0 + (10)) + 1;
                                        spD2 = *(s16 *)((unsigned char *)temp_s0 + (12)) + 1;
                                    } else {
                                        spD0 = *(s16 *)((unsigned char *)temp_s0 + (10));
                                        spD2 = *(s16 *)((unsigned char *)temp_s0 + (12));
                                    }
                                    if ((var_s1->unk_04 & 8) == 8) {
                                        temp_v0 = temp_v1_3 & 0x03000000;
                                        if (temp_v0 != 0) {
                                            var_v1_2 = var_s2 & 0x1F;
                                            if (temp_v0 != 0x01000000) {
                                                var_a0 = (var_s2 & 0x20) * 4;
                                            } else {
                                                var_s2 = search_tpage8((s32) *(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 1, (s32) temp_a3_2) & 0xFFFF;
                                                var_a0 = (var_s2 & 0x20) * 4;
                                                var_v1_2 = var_s2 & 0x1F;
                                                goto block_44;
                                            }
                                        } else {
                                            var_s2 = search_tpage8((s32) *(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 2, (s32) temp_a3_2) & 0xFFFF;
                                            var_a0 = (var_s2 & 0x20) * 4;
                                            var_v1_2 = var_s2 & 0x1F;
block_44:
                                            var_t0 = var_s1->unk_03 & 8;
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
                                                var_s2 = search_tpage((s32) *(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 1, (s32) temp_a3_2) & 0xFFFF;
                                                var_a0_2 = (var_s2 & 0x20) * 4;
                                                var_v1_3 = var_s2 & 0x1F;
                                                goto block_51;
                                            }
                                        } else {
                                            var_s2 = search_tpage((s32) *(s16 *)((unsigned char *)temp_s0 + (4)), sp90, 2, (s32) temp_a3_2) & 0xFFFF;
                                            var_a0_2 = (var_s2 & 0x20) * 4;
                                            var_v1_3 = var_s2 & 0x1F;
block_51:
                                            var_t0 = var_s1->unk_03 & 8;
                                        }
                                        spD4 = var_v1_3;
                                        temp_t7 = *(s16 *)((unsigned char *)temp_s0 + (4)) & 0x3F;
                                        spD6 = (temp_t7 & 7) * 0x10;
                                        spD7 = ((temp_t7 >> 3) * 0x10) + var_a0_2;
                                    }
                                    temp_a0_2 = var_s1->unk_43;
                                    temp_v0_3 = temp_a0_2 & 0x60;
                                    if (temp_a0_2 != 0) {
                                        spD8 = 0;
                                        switch (temp_v0_3) { /* switch 1; irregular */
                                        case 0x20:  /* switch 1 */
                                            spD8 = 0x100;
                                        case 0:     /* switch 2 */
                                        case 3:     /* switch 2 */
                                        case 5:     /* switch 2 */
                                        case 6:     /* switch 2 */
                                        case 7:     /* switch 2 */
                                        case 9:     /* switch 2 */
                                        case 10:    /* switch 2 */
                                        case 11:    /* switch 2 */
                                        case 12:    /* switch 2 */
                                        case 13:    /* switch 2 */
                                        case 14:    /* switch 2 */
                                        case 15:    /* switch 2 */
                                        case 16:    /* switch 2 */
                                        case 17:    /* switch 2 */
                                        case 18:    /* switch 2 */
                                        case 19:    /* switch 2 */
                                        case 20:    /* switch 2 */
                                        case 21:    /* switch 2 */
                                        case 22:    /* switch 2 */
                                        case 23:    /* switch 2 */
                                        case 24:    /* switch 2 */
                                        case 25:    /* switch 2 */
                                        case 26:    /* switch 2 */
                                        case 27:    /* switch 2 */
                                        case 28:    /* switch 2 */
                                        case 29:    /* switch 2 */
                                        case 30:    /* switch 2 */
                                        case 31:    /* switch 2 */
                                        case 32:    /* switch 2 */
block_70:
                                            var_t9 = temp_a0_2 & 0x1F;
                                            break;
                                        case 0x40:  /* switch 1 */
                                            spD8 = 0x120;
                                            goto block_70;
                                        default:    /* switch 1 */
                                            temp_v1_4 = var_s1->unk_05;
                                            temp_v0_4 = temp_v1_4 & 0x7F;
                                            if (temp_v1_4 & 0x80) {
                                                switch (temp_v0_4) { /* switch 2; irregular */
                                                default: /* switch 2 */
                                                    var_t9 = temp_a0_2 & 0x1F;
                                                    if (temp_v0_4 != 0x40) {
                                                        goto block_70;
                                                    }
                                                    break;
                                                case 1: /* switch 2 */
                                                    spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF0) >> 4) * 0x10;
                                                    goto block_70;
                                                case 2: /* switch 2 */
                                                    var_t8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xE0) >> 4) * 0x10;
block_69:
                                                    spD8 = var_t8;
                                                    goto block_70;
                                                case 4: /* switch 2 */
                                                    spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xC0) >> 4) * 0x10;
                                                    goto block_70;
                                                case 8: /* switch 2 */
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
                                        temp_v1_5 = var_s1->unk_05;
                                        temp_v0_5 = temp_v1_5 & 0x7F;
                                        switch (temp_v0_5) { /* switch 3; irregular */
                                        case 0x1:   /* switch 3 */
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF0) >> 4) * 0x10;
                                            break;
                                        case 0x2:   /* switch 3 */
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xE0) >> 4) * 0x10;
                                            break;
                                        case 0x4:   /* switch 3 */
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xC0) >> 4) * 0x10;
                                            break;
                                        case 0x8:   /* switch 3 */
                                            spD8 = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0x80) >> 4) * 0x10;
                                            break;
                                        default:    /* switch 3 */
                                            spD8 = 0;
                                            break;
                                        }
                                        if (temp_v1_5 & 0x80) {
                                            spDA = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF00) >> 8) + 0x1F0;
                                        } else {
                                            spDA = ((s32) (*(s16 *)((unsigned char *)temp_s0 + (6)) & 0xF00) >> 8) + 0x1E0;
                                        }
                                    }
                                    if (var_s1->unk_03 & 0x20) {
                                        var_v0_2 = sp8D;
                                        spDC = var_v0_2;
                                        spDD = var_v0_2;
                                        goto block_92;
                                    }
                                    temp_v0_6 = var_s1->pad_3C[4];
                                    if ((temp_v0_6 != 0) || (var_s1->pad_3C[5] != 0) || (var_s1->pad_3C[6] != 0)) {
                                        spDC = temp_v0_6;
                                        spDD = var_s1->pad_3C[5];
                                        spDE = var_s1->pad_3C[6];
                                    } else {
                                        var_v0_2 = var_s1->unk_07;
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
                                    if ((var_s1->unk_1A != 0x1000) || (*(s16 *)((unsigned char *)var_s1 + (28)) != 0x1000)) {
                                        spE4 = var_s1->unk_1A * (spE4 / 4096);
                                        spE6 = *(s16 *)((unsigned char *)var_s1 + (28)) * (spE6 / 4096);
                                    }
                                    temp_v0_7 = var_s1->unk_01;
                                    if (temp_v0_7 == 0) {
                                        func_80098680((s32) &spC8, (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), 0);
                                    } else {
                                        func_80098680((s32) &spC8, (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), (temp_v0_7 - 1) & 0xFFFF);
                                    }
                                    var_s3 -= 1;
                                } while (var_s3 >= 0);
                                var_a2 = var_s1->unk_02 & 0x40;
                            }
                        }
                        if ((var_a2 == 0) && ((var_s1->unk_02 & 1) == 1)) {
                            temp_v0_8 = var_s1->unk_06;
                            temp_t8_2 = spC0 * 4;
                            if ((temp_v0_8 & 0x80) && (temp_a0_3 = temp_v0_8 & 0x7F, (temp_a0_3 != 0))) {
                                var_s1->unk_08 += temp_a0_3;
                                var_v1_4 = *(var_s1->unk_10 + temp_t8_2);
                            } else if (temp_v0_8 != 0) {
                                if (((u32) D_800E6280.unk_10F8 % temp_v0_8) == 0) {
                                    var_s1->unk_08 += 1;
                                }
                                var_v1_4 = *(var_s1->unk_10 + temp_t8_2);
                            } else {
                                var_v1_4 = *(var_s1->unk_10 + temp_t8_2);
                            }
                            if (var_s1->unk_08 >= *(s16 *)((unsigned char *)(var_v1_4 + (spC4 * 2 * 2)) + (4))) {
                                temp_a0_4 = var_s1->unk_02;
                                if ((temp_a0_4 & 0x10) == 0x10) {
                                    var_s1->unk_18 += 1;
                                    var_s1->unk_08 = 0;
                                    var_s1->unk_02 = temp_a0_4 & ~0x20;
                                } else if ((temp_a0_4 & 0x20) == 0x20) {
                                    var_s1->unk_18 -= 1;
                                    var_s1->unk_08 = 0;
                                    if (var_s1->unk_18 <= 0) {
                                        var_s1->unk_02 &= 0xFFDE;
                                    }
                                } else {
                                    var_s1->unk_18 += 1;
                                    var_s1->unk_08 = 0;
                                }
                            }
                            if (var_s1->unk_18 >= *var_v1_4) {
                                temp_a0_5 = var_s1->unk_02;
                                if ((temp_a0_5 & 2) == 2) {
                                    var_s1->unk_18 = 0;
                                    var_s1->unk_02 = temp_a0_5 & ~3;
                                } else if ((temp_a0_5 & 4) == 4) {
                                    var_s1->unk_02 = temp_a0_5 & 0xFFFA;
                                    var_s1->unk_18 -= 1;
                                } else if ((temp_a0_5 & 8) == 8) {
                                    var_s1->unk_18 = 0;
                                    var_s1->unk_02 = temp_a0_5 & 0xFFFC;
                                    var_s1->unk_03 &= ~0x80;
                                } else if ((temp_a0_5 & 0x10) == 0x10) {
                                    temp_t6 = temp_a0_5 & ~0x10;
                                    var_s1->unk_02 = temp_t6;
                                    var_s1->unk_02 = temp_t6 | 0x20;
                                    if (var_s1->unk_03 & 8) {
                                        if (*var_v1_4 >= 3) {
                                            var_s1->unk_18 -= 3;
                                        } else {
                                            goto block_139;
                                        }
                                    } else if (*var_v1_4 >= 2) {
                                        var_s1->unk_18 -= 1;
                                    } else {
                                        goto block_139;
                                    }
                                } else {
block_139:
                                    var_s1->unk_18 = 0;
                                }
                            }
                        }
                        if (var_s1->unk_03 & 0x10) {
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
