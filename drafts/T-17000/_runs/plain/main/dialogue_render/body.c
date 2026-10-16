void dialogue_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7) {
    s16 sp2F2;
    s16 sp2E6;
    u8 sp2A4;
    u8 sp28C;
    u8 sp8C;
    u8 sp84;
    u8 sp80;
    u8 sp7C;
    u8 sp78;
    u8 sp74;
    u8 sp70;
    s32 sp68;
    s32 sp64;
    u8 *sp60;
    s16 temp_v0;
    s16 temp_v0_5;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s16 var_a0;
    s16 var_s0;
    s16 var_s0_10;
    s16 var_s0_11;
    s16 var_s0_12;
    s16 var_s0_13;
    s16 var_s0_14;
    s16 var_s0_15;
    s16 var_s0_16;
    s16 var_s0_17;
    s16 var_s0_18;
    s16 var_s0_19;
    s16 var_s0_20;
    s16 var_s0_21;
    s16 var_s0_22;
    s16 var_s0_23;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s16 var_s0_6;
    s16 var_s0_7;
    s16 var_s0_8;
    s16 var_s0_9;
    s16 var_s3;
    s16 var_s4;
    s16 var_s5;
    s16 var_s5_2;
    s32 var_t8_2;
    s32 var_t9_2;
    u8 *temp_a1;
    u8 *temp_a1_2;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_t6;
    u8 *temp_t6_2;
    u8 *temp_t6_4;
    u8 *temp_t6_5;
    u8 *temp_t6_6;
    u8 *temp_t6_7;
    u8 *temp_t6_8;
    u8 *temp_t7;
    u8 *temp_t7_2;
    u8 *temp_t7_4;
    u8 *temp_t7_5;
    u8 *temp_t8;
    u8 *temp_t8_2;
    u8 *temp_t8_4;
    u8 *temp_t8_5;
    u8 *temp_t8_7;
    u8 *temp_t9_2;
    u8 *temp_t9_3;
    u8 *temp_t9_5;
    u8 *temp_t9_6;
    u8 *temp_t9_7;
    u8 *temp_t9_8;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *var_t6;
    u8 *var_t6_2;
    u8 *var_t6_3;
    u8 *var_t7;
    u8 *var_t7_2;
    u8 *var_t7_3;
    u8 *var_t7_4;
    u8 *var_t8;
    u8 *var_t8_3;
    u8 *var_t9;
    u8 *var_t9_3;
    u8 *var_t9_4;
    u8 temp_t6_3;
    u8 temp_t7_3;
    u8 temp_t7_6;
    u8 temp_t7_7;
    u8 temp_t8_3;
    u8 temp_t8_6;
    u8 temp_t8_8;
    u8 temp_t8_9;
    u8 temp_t9;
    u8 temp_t9_4;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_6;

    *(? *)((unsigned char *)(&sp80) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_800B2228) + (2)));
    *(? *)((unsigned char *)(&sp7C) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_800B222C) + (2)));
    *(? *)((unsigned char *)(&sp78) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_800B2230) + (2)));
    *(? *)((unsigned char *)(&sp74) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_800B2234) + (2)));
    *(? *)((unsigned char *)(&sp70) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_800B2238) + (2)));
    if (*arg3 == 0) {
        return;
    }
    strcpy(&sp84, "（");
    strcpy(&sp8C, (u8 *) arg3);
    D_80122D1C = 1;
    temp_v0 = strlen(&sp8C);
    var_a0 = 0;
    if (temp_v0 > 0) {
        do {
            temp_v1 = &(&sp8C)[var_a0];
            temp_v0_2 = temp_v1->unk0;
            if ((temp_v0_2 == 0x6D) || (temp_v0_2 == 0x4D) || (temp_v0_2 == 0x41) || (temp_v0_2 == 0x57) || (temp_v0_2 == 0x4F) || (temp_v0_2 == 0x46) || (temp_v0_2 == 0x59) || (temp_v0_2 == 0x53) || (temp_v0_2 == 0x50) || (temp_v0_2 == 0x70) || (temp_v0_2 == 0x47) || (temp_v0_2 == 0x20) || (temp_v0_2 == 0x71)) {
                var_a0 -= 1;
            } else if (((temp_v0_2 != sp80) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp81)) && ((temp_v0_2 != sp7C) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp7D)) && ((temp_v0_2 != sp78) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp79)) && ((temp_v0_2 != sp74) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp75)) && ((temp_v0_2 != sp70) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp71))) {
                D_80122D1C = 0;
            }
            var_a0 += 2;
        } while (var_a0 < temp_v0);
    }
    var_s0 = 0;
    sp2E6 = 0;
    var_s3 = 0;
    var_s4 = 0;
    sp2F2 = 0;
loop_32:
    temp_v1_2 = &(&sp8C)[sp2F2];
    temp_v0_3 = *temp_v1_2;
    if (temp_v0_3 == 0x6D) {
block_43:
        sp2F2 -= 1;
block_44:
block_45:
        sp2F2 += 2;
        goto loop_32;
    }
    if (temp_v0_3 == 0x4D) {
        goto block_43;
    }
    if (temp_v0_3 == 0x41) {
        goto block_43;
    }
    if (temp_v0_3 == 0x57) {
        goto block_43;
    }
    if (temp_v0_3 == 0x4F) {
        goto block_43;
    }
    if (temp_v0_3 == 0x46) {
        goto block_43;
    }
    if (temp_v0_3 == 0x59) {
        goto block_43;
    }
    if (temp_v0_3 == 0x53) {
        goto block_43;
    }
    if (temp_v0_3 == 0x50) {
        goto block_43;
    }
    if (temp_v0_3 == 0x70) {
        goto block_43;
    }
    if (temp_v0_3 == 0x47) {
        goto block_43;
    }
    if (temp_v0_3 == 0x20) {
block_49:
        *temp_v1_2 = 0;
        if ((u8) arg7 != 0) {
            set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) ((s16) arg1 + sp2E6), (u8) arg2, (u8 *) arg6, arg4);
        }
        temp_a1 = &(&sp8C)[var_s0];
        sp64 = (s32) (s16) ((s16) arg1 + sp2E6);
        sp60 = temp_a1;
        strncpy((s32) &sp2A4, (s32) temp_a1, 0x40);
        temp_v1_3 = sp2F2 - var_s0;
        var_s5 = temp_v1_3;
        if (temp_v1_3 > 0) {
            do {
                temp_s1 = &(&sp2A4)[var_s3];
                temp_v0_4 = *temp_s1;
                var_s0_2 = 0;
                if (temp_v0_4 == 0x50) {
                    var_t7 = D_800CA17C;
                    if (strlen(D_800CA17C) > 0) {
                        do {
                            temp_t6 = &temp_s1[var_s0_2];
                            var_s0_2 += 1;
                            *temp_t6 = *var_t7;
                            var_t7 = &D_800CA17C[var_s0_2];
                        } while (var_s0_2 < strlen(D_800CA17C));
                    }
                    strncpy((s32) &temp_s1[var_s0_2], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_2) + 0x40);
                    var_s5 = (var_s5 + strlen(D_800CA17C)) - 1;
                    var_s3 = (var_s3 + strlen(D_800CA17C)) - 2;
                    var_s4 -= 1;
                } else {
                    var_s0_3 = 0;
                    if (temp_v0_4 == 0x53) {
                        var_t9 = D_800CA23C;
                        if (strlen(D_800CA23C) > 0) {
                            do {
                                temp_t8 = &temp_s1[var_s0_3];
                                var_s0_3 += 1;
                                *temp_t8 = *var_t9;
                                var_t9 = &D_800CA23C[var_s0_3];
                            } while (var_s0_3 < strlen(D_800CA23C));
                        }
                        strncpy((s32) &temp_s1[var_s0_3], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_3) + 0x40);
                        var_s5 = (var_s5 + strlen(D_800CA23C)) - 1;
                        var_s3 = (var_s3 + strlen(D_800CA23C)) - 2;
                        var_s4 -= 1;
                    } else {
                        var_s0_4 = 0;
                        if (temp_v0_4 == 0x47) {
                            if (strlen(girl_name_buf) > 0) {
                                do {
                                    temp_t7 = &temp_s1[var_s0_4];
                                    temp_t9 = girl_name_buf[var_s0_4];
                                    var_s0_4 += 1;
                                    *temp_t7 = temp_t9;
                                } while (var_s0_4 < strlen(girl_name_buf));
                            }
                            strncpy((s32) &temp_s1[var_s0_4], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_4) + 0x40);
                            var_s5 = (var_s5 + strlen(girl_name_buf)) - 1;
                            var_t9_2 = (var_s4 - 1) << 0x10;
                            var_s3 = (var_s3 + strlen(girl_name_buf)) - 2;
                            goto block_96;
                        }
                        var_s0_5 = 0;
                        if (temp_v0_4 == 0x70) {
                            var_t8 = D_800CA16C;
                            if (strlen(D_800CA16C) > 0) {
                                do {
                                    temp_t9_2 = &temp_s1[var_s0_5];
                                    var_s0_5 += 1;
                                    *temp_t9_2 = *var_t8;
                                    var_t8 = &D_800CA16C[var_s0_5];
                                } while (var_s0_5 < strlen(D_800CA16C));
                            }
                            strncpy((s32) &temp_s1[var_s0_5], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_5) + 0x40);
                            var_s5 = (var_s5 + strlen(D_800CA16C)) - 1;
                            var_s3 = (var_s3 + strlen(D_800CA16C)) - 2;
                            var_s4 -= 1;
                        } else if (temp_v0_4 == 0x59) {
                            var_s0_6 = 0;
                            strcpy(&sp28C, "好雄");
                            var_t7_2 = &sp28C;
                            if (strlen(&sp28C) > 0) {
                                do {
                                    temp_t6_2 = &temp_s1[var_s0_6];
                                    var_s0_6 += 1;
                                    *temp_t6_2 = *var_t7_2;
                                    var_t7_2 = &(&sp28C)[var_s0_6];
                                } while (var_s0_6 < strlen(&sp28C));
                            }
                            strncpy((s32) &temp_s1[var_s0_6], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_6) + 0x40);
                            var_s5 = (var_s5 + strlen(&sp28C)) - 1;
                            var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                            var_s4 -= 1;
                        } else if (temp_v0_4 == 0x46) {
                            var_s0_7 = 0;
                            sprintf((s32) &sp28C, (s32) "%s%s", (s32) D_800CA16C, (s32) D_800CA174);
                            var_t9_3 = &sp28C;
                            if (strlen(&sp28C) > 0) {
                                do {
                                    temp_t8_2 = &temp_s1[var_s0_7];
                                    var_s0_7 += 1;
                                    *temp_t8_2 = *var_t9_3;
                                    var_t9_3 = &(&sp28C)[var_s0_7];
                                } while (var_s0_7 < strlen(&sp28C));
                            }
                            strncpy((s32) &temp_s1[var_s0_7], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_7) + 0x40);
                            var_s5 = (var_s5 + strlen(&sp28C)) - 1;
                            var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                            var_s4 -= 1;
                        } else {
                            if (temp_v0_4 == 0x6D) {
                                var_s0_8 = 0;
                                strcpy(&sp28C, D_800CA174);
                                var_t6 = &sp28C;
                                if (strlen(&sp28C) > 0) {
                                    do {
                                        temp_t7_2 = &temp_s1[var_s0_8];
                                        var_s0_8 += 1;
                                        *temp_t7_2 = *var_t6;
                                        var_t6 = &(&sp28C)[var_s0_8];
                                    } while (var_s0_8 < strlen(&sp28C));
                                }
                                strncpy((s32) &temp_s1[var_s0_8], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_8) + 0x40);
                                var_s5 = (var_s5 + strlen(&sp28C)) - 1;
                                var_t9_2 = (var_s4 - 1) << 0x10;
                                var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                                goto block_96;
                            }
                            var_s0_9 = 0;
                            if (temp_v0_4 == 0x4F) {
                                if (strlen(&D_800CA248) > 0) {
                                    do {
                                        temp_t9_3 = &temp_s1[var_s0_9];
                                        temp_t6_3 = (&D_800CA248)[var_s0_9];
                                        var_s0_9 += 1;
                                        *temp_t9_3 = temp_t6_3;
                                    } while (var_s0_9 < strlen(&D_800CA248));
                                }
                                strncpy((s32) &temp_s1[var_s0_9], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_9) + 0x40);
                                var_s5 = (var_s5 + strlen(&D_800CA248)) - 1;
                                var_s3 = (var_s3 + strlen(&D_800CA248)) - 2;
                                var_s4 -= 1;
                            } else {
                                var_s0_10 = 0;
                                if (temp_v0_4 == 0x57) {
                                    if (strlen(D_800CA250) > 0) {
                                        do {
                                            temp_t6_4 = &temp_s1[var_s0_10];
                                            temp_t8_3 = D_800CA250[var_s0_10];
                                            var_s0_10 += 1;
                                            *temp_t6_4 = temp_t8_3;
                                        } while (var_s0_10 < strlen(D_800CA250));
                                    }
                                    strncpy((s32) &temp_s1[var_s0_10], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_10) + 0x40);
                                    var_s5 = (var_s5 + strlen(D_800CA250)) - 1;
                                    var_s3 = (var_s3 + strlen(D_800CA250)) - 2;
                                    var_s4 -= 1;
                                } else {
                                    var_s0_11 = 0;
                                    if (temp_v0_4 == 0x41) {
                                        if (strlen(D_800CA25C) > 0) {
                                            do {
                                                temp_t8_4 = &temp_s1[var_s0_11];
                                                temp_t7_3 = D_800CA25C[var_s0_11];
                                                var_s0_11 += 1;
                                                *temp_t8_4 = temp_t7_3;
                                            } while (var_s0_11 < strlen(D_800CA25C));
                                        }
                                        strncpy((s32) &temp_s1[var_s0_11], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_11) + 0x40);
                                        var_s5 = (var_s5 + strlen(D_800CA25C)) - 1;
                                        var_s3 = (var_s3 + strlen(D_800CA25C)) - 2;
                                        var_s4 -= 1;
                                    } else {
                                        var_s0_12 = 0;
                                        if (temp_v0_4 == 0x4D) {
                                            if (strlen(&D_800CA280) > 0) {
                                                do {
                                                    temp_t7_4 = &temp_s1[var_s0_12];
                                                    temp_t9_4 = (&D_800CA280)[var_s0_12];
                                                    var_s0_12 += 1;
                                                    *temp_t7_4 = temp_t9_4;
                                                } while (var_s0_12 < strlen(&D_800CA280));
                                            }
                                            strncpy((s32) &temp_s1[var_s0_12], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_12) + 0x40);
                                            var_s5 = (var_s5 + strlen(&D_800CA280)) - 1;
                                            var_t9_2 = (var_s4 - 1) << 0x10;
                                            var_s3 = (var_s3 + strlen(&D_800CA280)) - 2;
block_96:
                                            var_s4 = (s16) (var_t9_2 >> 0x10);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                var_s3 += 2;
                var_s4 += 2;
            } while (var_s3 < var_s5);
            var_s3 = 0;
            var_s4 = 0;
        }
        if ((u8) arg7 != 0) {
            strcat(&sp2A4, (u8 *) arg5);
        }
        if ((sp2A4 == sp84) && (sp2A5 == sp85)) {
            set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) sp64, (u8) arg2, &sp2A4, arg4);
        } else {
            set_kanji_string((s16) arg0, (s16) sp64, (u8) arg2, &sp2A4, arg4);
        }
        sp2E6 += 0x10;
        var_s0 = sp2F2 + 1;
        sp2F2 -= 1;
        goto block_44;
    }
    if (temp_v0_3 == 0x71) {
        goto block_49;
    }
    if (temp_v0_3 != 0) {
        goto block_45;
    }
    if ((u8) arg7 != 0) {
        set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) ((s16) arg1 + sp2E6), (u8) arg2, (u8 *) arg6, arg4);
    }
    temp_v0_5 = (s16) arg1 + sp2E6;
    temp_a1_2 = &(&sp8C)[var_s0];
    sp64 = (s32) temp_v0_5;
    sp60 = temp_a1_2;
    sp68 = (s32) temp_v0_5;
    strncpy((s32) &sp2A4, (s32) temp_a1_2, 0x40);
    temp_v1_4 = sp2F2 - var_s0;
    var_s5_2 = temp_v1_4;
    if (temp_v1_4 > 0) {
        do {
            temp_s1_2 = &(&sp2A4)[var_s3];
            temp_v0_6 = *temp_s1_2;
            var_s0_13 = 0;
            if (temp_v0_6 == 0x50) {
                var_t7_3 = D_800CA17C;
                if (strlen(D_800CA17C) > 0) {
                    do {
                        temp_t8_5 = &temp_s1_2[var_s0_13];
                        var_s0_13 += 1;
                        *temp_t8_5 = *var_t7_3;
                        var_t7_3 = &D_800CA17C[var_s0_13];
                    } while (var_s0_13 < strlen(D_800CA17C));
                }
                strncpy((s32) &temp_s1_2[var_s0_13], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_13) + 0x40);
                var_s5_2 = (var_s5_2 + strlen(D_800CA17C)) - 1;
                var_s3 = (var_s3 + strlen(D_800CA17C)) - 2;
                var_s4 -= 1;
            } else {
                var_s0_14 = 0;
                if (temp_v0_6 == 0x53) {
                    var_t6_2 = D_800CA23C;
                    if (strlen(D_800CA23C) > 0) {
                        do {
                            temp_t9_5 = &temp_s1_2[var_s0_14];
                            var_s0_14 += 1;
                            *temp_t9_5 = *var_t6_2;
                            var_t6_2 = &D_800CA23C[var_s0_14];
                        } while (var_s0_14 < strlen(D_800CA23C));
                    }
                    strncpy((s32) &temp_s1_2[var_s0_14], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_14) + 0x40);
                    var_s5_2 = (var_s5_2 + strlen(D_800CA23C)) - 1;
                    var_s3 = (var_s3 + strlen(D_800CA23C)) - 2;
                    var_s4 -= 1;
                } else {
                    var_s0_15 = 0;
                    if (temp_v0_6 == 0x47) {
                        if (strlen(girl_name_buf) > 0) {
                            do {
                                temp_t6_5 = &temp_s1_2[var_s0_15];
                                temp_t8_6 = girl_name_buf[var_s0_15];
                                var_s0_15 += 1;
                                *temp_t6_5 = temp_t8_6;
                            } while (var_s0_15 < strlen(girl_name_buf));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_15], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_15) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(girl_name_buf)) - 1;
                        var_t8_2 = (var_s4 - 1) << 0x10;
                        var_s3 = (var_s3 + strlen(girl_name_buf)) - 2;
                        goto block_154;
                    }
                    var_s0_16 = 0;
                    if (temp_v0_6 == 0x70) {
                        var_t7_4 = D_800CA16C;
                        if (strlen(D_800CA16C) > 0) {
                            do {
                                temp_t8_7 = &temp_s1_2[var_s0_16];
                                var_s0_16 += 1;
                                *temp_t8_7 = *var_t7_4;
                                var_t7_4 = &D_800CA16C[var_s0_16];
                            } while (var_s0_16 < strlen(D_800CA16C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_16], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_16) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(D_800CA16C)) - 1;
                        var_s3 = (var_s3 + strlen(D_800CA16C)) - 2;
                        var_s4 -= 1;
                    } else if (temp_v0_6 == 0x59) {
                        var_s0_17 = 0;
                        strcpy(&sp28C, "好雄");
                        var_t6_3 = &sp28C;
                        if (strlen(&sp28C) > 0) {
                            do {
                                temp_t9_6 = &temp_s1_2[var_s0_17];
                                var_s0_17 += 1;
                                *temp_t9_6 = *var_t6_3;
                                var_t6_3 = &(&sp28C)[var_s0_17];
                            } while (var_s0_17 < strlen(&sp28C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_17], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_17) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(&sp28C)) - 1;
                        var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                        var_s4 -= 1;
                    } else if (temp_v0_6 == 0x46) {
                        var_s0_18 = 0;
                        sprintf((s32) &sp28C, (s32) "%s%s", (s32) D_800CA16C, (s32) D_800CA174);
                        var_t8_3 = &sp28C;
                        if (strlen(&sp28C) > 0) {
                            do {
                                temp_t7_5 = &temp_s1_2[var_s0_18];
                                var_s0_18 += 1;
                                *temp_t7_5 = *var_t8_3;
                                var_t8_3 = &(&sp28C)[var_s0_18];
                            } while (var_s0_18 < strlen(&sp28C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_18], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_18) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(&sp28C)) - 1;
                        var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                        var_s4 -= 1;
                    } else {
                        if (temp_v0_6 == 0x6D) {
                            var_s0_19 = 0;
                            strcpy(&sp28C, D_800CA174);
                            var_t9_4 = &sp28C;
                            if (strlen(&sp28C) > 0) {
                                do {
                                    temp_t6_6 = &temp_s1_2[var_s0_19];
                                    var_s0_19 += 1;
                                    *temp_t6_6 = *var_t9_4;
                                    var_t9_4 = &(&sp28C)[var_s0_19];
                                } while (var_s0_19 < strlen(&sp28C));
                            }
                            strncpy((s32) &temp_s1_2[var_s0_19], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_19) + 0x40);
                            var_s5_2 = (var_s5_2 + strlen(&sp28C)) - 1;
                            var_t8_2 = (var_s4 - 1) << 0x10;
                            var_s3 = (var_s3 + strlen(&sp28C)) - 2;
                            goto block_154;
                        }
                        var_s0_20 = 0;
                        if (temp_v0_6 == 0x4F) {
                            if (strlen(&D_800CA248) > 0) {
                                do {
                                    temp_t9_7 = &temp_s1_2[var_s0_20];
                                    temp_t7_6 = (&D_800CA248)[var_s0_20];
                                    var_s0_20 += 1;
                                    *temp_t9_7 = temp_t7_6;
                                } while (var_s0_20 < strlen(&D_800CA248));
                            }
                            strncpy((s32) &temp_s1_2[var_s0_20], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_20) + 0x40);
                            var_s5_2 = (var_s5_2 + strlen(&D_800CA248)) - 1;
                            var_s3 = (var_s3 + strlen(&D_800CA248)) - 2;
                            var_s4 -= 1;
                        } else {
                            var_s0_21 = 0;
                            if (temp_v0_6 == 0x57) {
                                if (strlen(D_800CA250) > 0) {
                                    do {
                                        temp_t6_7 = &temp_s1_2[var_s0_21];
                                        temp_t8_8 = D_800CA250[var_s0_21];
                                        var_s0_21 += 1;
                                        *temp_t6_7 = temp_t8_8;
                                    } while (var_s0_21 < strlen(D_800CA250));
                                }
                                strncpy((s32) &temp_s1_2[var_s0_21], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_21) + 0x40);
                                var_s5_2 = (var_s5_2 + strlen(D_800CA250)) - 1;
                                var_t8_2 = (var_s4 - 1) << 0x10;
                                var_s3 = (var_s3 + strlen(D_800CA250)) - 2;
                                goto block_154;
                            }
                            var_s0_22 = 0;
                            if (temp_v0_6 == 0x41) {
                                if (strlen(D_800CA25C) > 0) {
                                    do {
                                        temp_t9_8 = &temp_s1_2[var_s0_22];
                                        temp_t7_7 = D_800CA25C[var_s0_22];
                                        var_s0_22 += 1;
                                        *temp_t9_8 = temp_t7_7;
                                    } while (var_s0_22 < strlen(D_800CA25C));
                                }
                                strncpy((s32) &temp_s1_2[var_s0_22], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_22) + 0x40);
                                var_s5_2 = (var_s5_2 + strlen(D_800CA25C)) - 1;
                                var_s3 = (var_s3 + strlen(D_800CA25C)) - 2;
                                var_s4 -= 1;
                            } else {
                                var_s0_23 = 0;
                                if (temp_v0_6 == 0x4D) {
                                    if (strlen(&D_800CA280) > 0) {
                                        do {
                                            temp_t6_8 = &temp_s1_2[var_s0_23];
                                            temp_t8_9 = (&D_800CA280)[var_s0_23];
                                            var_s0_23 += 1;
                                            *temp_t6_8 = temp_t8_9;
                                        } while (var_s0_23 < strlen(&D_800CA280));
                                    }
                                    strncpy((s32) &temp_s1_2[var_s0_23], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_23) + 0x40);
                                    var_s5_2 = (var_s5_2 + strlen(&D_800CA280)) - 1;
                                    var_t8_2 = (var_s4 - 1) << 0x10;
                                    var_s3 = (var_s3 + strlen(&D_800CA280)) - 2;
block_154:
                                    var_s4 = (s16) (var_t8_2 >> 0x10);
                                }
                            }
                        }
                    }
                }
            }
            var_s3 += 2;
            var_s4 += 2;
        } while (var_s3 < var_s5_2);
    }
    if (*arg5 != 0) {
        strcat(&sp2A4, (u8 *) arg5);
    }
    D_800CA2C0 = sp68 + 0x10;
    if ((sp2A4 == sp84) && (sp2A5 == sp85)) {
        set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) sp64, (u8) arg2, &sp2A4, arg4);
    } else {
        set_kanji_string((s16) arg0, (s16) sp64, (u8) arg2, &sp2A4, arg4);
    }
}
