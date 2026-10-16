void func_8006D7EC(void) {
    s32 sp94;
    s32 sp8C;
    s32 sp88;
    s32 sp7C;
    s32 sp78;
    s32 sp70;
    s32 sp6C;
    s32 sp64;
    u8 sp63;
    u8 sp62;
    u8 sp61;
    s32 *var_s1;
    s32 *var_s1_4;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 var_a0;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s3;
    s32 var_s4;
    s32 var_t8;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    u32 temp_t0;
    u32 temp_t1;
    u32 temp_t6;
    u32 temp_t7;
    u32 temp_v1;
    u32 var_fp;
    u8 temp_a0;
    u8 temp_s3;
    u8 temp_v0_2;
    u8 *temp_s0;
    u8 *temp_v0;
    u8 *var_s1_2;
    u8 *var_s1_3;

    sp64 = 0;
    sp61 = 0;
    sp63 = 0;
    sp62 = 0;
    sp70 = 0;
    sp6C = 0;
    temp_v0 = ((u8 *)&D_800E6280) + ((u32) (D_800E71E8 & 0xF) % 9U);
    *(u8 *)((unsigned char *)temp_v0 + (309)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (309)) + 1);
    if (D_800E66B8 != 0) {
        D_800E66B8 -= 1;
    }
    sp7C = func_8006F524(D_800E6280.unk_F68.w & 0xF) & 0xF;
    var_s1 = &D_800E62B8;
    var_a0 = 0;
    do {
        temp_t5 = *(s32 *)((unsigned char *)var_s1 + (456));
        var_s1 += 0x38;
        if (temp_t5 & 2) {
            var_a0 += 1;
        }
    } while ((u32) var_s1 < (u32) &D_800E64B0);
    D_800B6740 = -1;
    temp_s3 = D_800E62C0;
    temp_v1 = (u8) D_800E6280.unk_0F6.b[0] >> 5;
    var_s4 = 0;
    sp8C = (s32) D_800E62BF;
    sp88 = (s32) D_800E62C4;
    if (temp_v1 != 0) {
        D_800E6280.unk_0F6.b[0] = ((temp_v1 - 1) << 5) | (D_800E6280.unk_0F6.b[0] & 0xFF1F);
    }
    sp78 = var_a0;
    func_8006CF38();
    var_v1 = 0;
    var_fp = (u32) (D_800E62C1 + 1) % 7U;
    var_s3 = temp_s3 + 1;
    if (temp_s3 == sp88) {
        D_800E691C = 0xFE;
        var_v1 = 1;
        var_s4 = 1;
        var_s3 = 1;
    }
    temp_v0_2 = D_800E6280.unk_044[0].unk_20[var_s3];
    if (temp_v0_2 == 0xB) {
        temp_v1_2 = var_v1 + 1;
        D_800E6280.unk_69C[var_v1].unk_00 = 0x41;
        D_800E6280.unk_69C[temp_v1_2].unk_00 = 0x10;
        sp94 = temp_v1_2 + 1;
        func_8006D52C((s32) &sp94, var_s4, var_s3 + 6);
        func_8004284C();
        return;
    }
    var_at = (s32) var_fp < 7;
    if (temp_v0_2 == 8) {
        temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        if ((temp_t6 != 0xB) && ((D_800E6280.unk_F68.w & 0xF) == 4)) {
            if ((temp_t6 == 6) && ((u32) (D_800E6906 & 0xF) >= 8U)) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x25;
                sp94 = var_v1 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3);
                temp_v1_3 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_3].unk_00 = 0x25;
                sp94 = temp_v1_3 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 1);
                temp_v1_4 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_4].unk_00 = 0x25;
                sp94 = temp_v1_4 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 2);
                temp_v1_5 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_5].unk_01 = 7;
                D_800E6280.unk_69C[temp_v1_5].unk_00 = 0x25;
                sp94 = temp_v1_5 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 3);
                temp_v1_6 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_6].unk_01 = 7;
                D_800E6280.unk_69C[temp_v1_6].unk_00 = 0x25;
                sp94 = temp_v1_6 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 4);
                temp_v1_7 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_7].unk_01 = 7;
                D_800E6280.unk_69C[temp_v1_7].unk_00 = 0x25;
                sp94 = temp_v1_7 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 5);
                temp_v1_8 = sp94 + 1;
                D_800E6280.unk_69C[sp94].unk_00 = 0xFB;
                D_800E6280.unk_69C[temp_v1_8].unk_00 = 0x10;
                sp94 = temp_v1_8 + 1;
                func_8006D52C((s32) &sp94, var_s4, var_s3 + 6);
                func_8004284C();
                return;
            }
            var_s1_2 = ((u8 *)&D_800E6280) + (var_v1 * 4);
            var_s0 = 0;
            do {
                *(s8 *)((unsigned char *)var_s1_2 + (1692)) = 0x42;
                D_800E6280.unk_69C[var_v1].unk_01 = 4;
                D_800E6280.unk_69C[var_v1].unk_03 = (u8) sp7C;
                sp94 = var_v1;
                var_s0 += 1;
                D_800E6280.unk_69C[var_v1].unk_02 = func_8006BDA8((s32) var_fp);
                var_s1_2 = ((u8 *)&D_800E6280) + (var_v1 * 4) + 4;
                var_v1 += 1;
            } while (var_s0 != 6);
            *(s8 *)((unsigned char *)var_s1_2 + (1692)) = 0x10;
            sp94 = var_v1 + 1;
            func_8006D52C((s32) &sp94, var_s4, var_s3 + 6);
            func_8004284C();
            return;
        }
        if (temp_t6 == 0xB) {
            D_800E6280.unk_66C[0xB].unk_03 = (((((u32) (D_800E6918 << 5) >> 0x1E) + 1) * 2) & 6) | (D_800E6280.unk_66C[0xB].unk_03 & 0xFFF9);
        }
        var_at = (s32) var_fp < 7;
        goto block_24;
    }
block_24:
    sp94 = var_v1;
    if (var_at != 0) {
        do {
            if (sp88 < var_s3) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFE;
                var_v1 += 1;
                var_s3 = 1;
                var_s4 = 1;
            }
            temp_s0 = ((u8 *)&D_800E6280) + (var_s4 << 6) + var_s3;
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 6) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0xE) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0x14) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0xA) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0x10) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 2) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFD;
                var_v1 += 1;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 4) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x20;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0xC) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x24;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0xD) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x28;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0xF) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x2C;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0x11) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x31;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if (*(u8 *)((unsigned char *)temp_s0 + (100)) == 0x13) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x30;
                var_v1_2 = var_v1 + 1;
                goto block_138;
            }
            if ((*(u8 *)((unsigned char *)temp_s0 + (100)) == 2) && (D_800E62BE == 0x60)) {
                D_800E6676 = 0;
                D_800E6672 = 0x32;
                D_800E6280.unk_69C[var_v1].unk_01 = 0xA;
                D_800E6280.unk_69C[var_v1].unk_00 = 0x48;
                sp94 = var_v1 + 1;
                func_8006D4A0();
            } else if ((*(u8 *)((unsigned char *)temp_s0 + (100)) == 2) && (D_800E62BE == 0x61)) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0x43;
                sp94 = var_v1 + 1;
                func_8006D4A0();
            } else {
                temp_a0 = *(u8 *)((unsigned char *)temp_s0 + (68));
                sp94 = var_v1;
                if (temp_a0 != 1) {
                    if (temp_a0 == 2) {
                        sp94 = var_v1;
                        goto block_61;
                    }
                    if (D_800E62BE == 0x61) {
                        if ((sp8C + var_s4) == 0xD) {
                            if (var_s3 == 3) {
                                sp94 = var_v1;
                                if ((func_8005352C() == 1) && (temp_t0 = (u16) D_800E6280.unk_0F4.h >> 0xC, (temp_t0 != 6)) && ((u32) (*(u16 *)((unsigned char *)(&D_800E6280.unk_66C[temp_t0]) + (2)) & 0xF) >= 0xAU)) {
                                    D_800E6280.unk_69C[sp94].unk_00 = 0x26;
                                    var_v1_2 = sp94 + 1;
                                } else {
                                    temp_t1 = (u16) D_800E6280.unk_0F4.h >> 0xC;
                                    if (temp_t1 < 6U) {
                                        var_t8 = sp94 * 4;
                                        if ((u16) D_800E6280.unk_66C[temp_t1].unk_00 >= 0x17CU) {
                                            D_800E6280.unk_69C[sp94].unk_00 = 0x27;
                                            var_v1_2 = sp94 + 1;
                                        } else {
                                            goto block_75;
                                        }
                                    } else {
                                        var_t8 = sp94 * 4;
block_75:
                                        *(s8 *)((unsigned char *)(((u8 *)&D_800E6280) + var_t8) + (1692)) = 0;
                                        D_800E6280.unk_69C[sp94].unk_01 = D_800E71E8 & 0xF;
                                        D_800E6280.unk_69C[sp94].unk_03 = (u8) sp7C;
                                        if (func_8006BDA8((s32) var_fp) != 0) {
                                            sp6C += 1;
                                        } else {
                                            sp70 += 1;
                                        }
                                        var_v1_2 = sp94 + 1;
                                    }
                                }
                                goto block_138;
                            }
                            goto block_81;
                        }
                        goto block_81;
                    }
block_81:
                    if ((*(u8 *)((unsigned char *)temp_s0 + (100)) == 0x12) && (D_800E62BE == 0x62)) {
                        D_800E6280.unk_69C[var_v1].unk_00 = 0x4A;
                        var_v1_2 = var_v1 + 1;
                        goto block_138;
                    }
                    if ((temp_a0 == 0) || (sp94 = var_v1, (temp_a0 == 3))) {
                        if ((*(u8 *)((unsigned char *)temp_s0 + (100)) == 5) || (D_800E691C == 0x40)) {
                            var_v1_3 = var_v1 + 1;
                            D_800E6280.unk_69C[var_v1].unk_00 = 0x40;
                            goto block_118;
                        }
                        D_800E6280.unk_69C[var_v1].unk_00 = 0;
                        D_800E6280.unk_69C[var_v1].unk_01 = D_800E71E8 & 0xF;
                        D_800E6280.unk_69C[var_v1].unk_03 = (u8) sp7C;
                        sp94 = var_v1;
                        temp_v0_3 = func_8006BDA8((s32) var_fp);
                        if (temp_v0_3 != 0) {
                            sp6C += 1;
                        } else {
                            sp70 += 1;
                        }
                        if ((D_800E71E8 & 0xF) == 7) {
                            var_s1_3 = ((u8 *)&D_800E6280) + (sp94 * 4);
                            D_800E6280.unk_69C[sp94].unk_02 = 0;
                        } else {
                            var_s1_3 = ((u8 *)&D_800E6280) + (sp94 * 4);
                            D_800E6280.unk_69C[sp94].unk_02 = (u8) temp_v0_3;
                        }
                        sp94 += 1;
                        if ((*(u8 *)((unsigned char *)var_s1_3 + (1694)) == 1) && ((sp6C + sp70) == 6) && (*(u8 *)((unsigned char *)temp_s0 + (68)) == 0) && ((rand() & 0xF) < 2)) {
                            D_800E6280.unk_69C[sp94].unk_00 = 0x47;
                            D_800E6280.unk_69C[sp94].unk_01 = 0xC;
                            sp94 += 1;
                        }
                        if (*(u8 *)((unsigned char *)temp_s0 + (68)) == 3) {
                            if ((D_800E62BD == 2) && ((u8) D_800E6835 < (u32) ((u16) D_800E62C2 / 365U)) && (D_800E6678 & 2) && !(get_g_zyotai_h(0xA) & 0x7F) && ((rand() & 0xF) <= 0)) {
                                D_800E6280.unk_69C[sp94].unk_00 = 0x45;
                                D_800E6280.unk_69C[sp94].unk_01 = 0xA;
                                D_800E6280.unk_69C[sp94].unk_02 = 7;
                                sp94 += 1;
                            }
                            if ((D_800E62BD == 1) && (var_fp == 2) && ((var_s4 + D_800E62BF) == 8) && ((rand() % 100) < 0x14) && (sp78 != 0) && ((temp_t7 = (u32) ((s32) *(u16 *)((unsigned char *)(((u8 *)&D_800E6280)) + (244)) << 0xD) >> 0x1F, (temp_t7 == 0)) || ((D_800E62BE == 0x61) && (temp_t7 == 0) && (var_s3 == 0x1C)))) {
                                D_800E6376 |= 4;
                                D_800E6280.unk_69C[sp94].unk_00 = 0x49;
                                var_v1_3 = sp94 + 1;
block_118:
                                sp94 = var_v1_3;
                            }
                        }
                        if (*(u8 *)((unsigned char *)temp_s0 + (68)) == 0) {
                            if (D_800E691C != 0x40) {
                                if ((D_800E62BE != 0x5F) || (D_800E62BF != 4)) {
                                    if (((u16) D_800E62C2 < 0x2D6U) && (sp61 == 0)) {
                                        if (sp78 < 2) {
                                            if (func_8006EB64(&sp63, &sp62) != 0) {
                                                D_800E6280.unk_69C[sp94].unk_00 = 0x48;
                                                D_800E6280.unk_69C[sp94].unk_01 = sp63;
                                                var_v1_4 = sp94 + 1;
                                                D_800E6280.unk_69C[sp94].unk_02 = sp62;
                                                sp61 += 1;
                                                goto block_130;
                                            }
                                        } else if (func_8006EEC4(&sp63, &sp62) != 0) {
                                            D_800E6280.unk_69C[sp94].unk_00 = 0x48;
                                            D_800E6280.unk_69C[sp94].unk_01 = sp63;
                                            var_v1_4 = sp94 + 1;
                                            D_800E6280.unk_69C[sp94].unk_02 = sp62;
                                            sp61 += 1;
block_130:
                                            sp94 = var_v1_4;
                                        }
                                        if (((D_800E71E8 & 0xF) == 4) && (func_8006F218(&sp63, &sp62) != 0)) {
                                            D_800E6280.unk_69C[sp94].unk_00 = 0x48;
                                            D_800E6280.unk_69C[sp94].unk_01 = sp63;
                                            D_800E6280.unk_69C[sp94].unk_02 = sp62;
                                            sp61 += 1;
                                            sp94 += 1;
                                        }
                                    }
                                }
                                if (sp64 < 2) {
                                    if ((rand() & 3) == 3) {
                                        D_800E6280.unk_69C[sp94].unk_00 = 0x44;
                                        var_v1_2 = sp94 + 1;
                                        sp64 += 1;
                                        goto block_138;
                                    }
                                }
                            }
                        }
                    }
                } else {
block_61:
                    if (func_8006EA40() != 0) {
                        D_800E6839 += 1;
                        D_800E6280.unk_69C[sp94].unk_00 = 0x45;
                        D_800E6280.unk_69C[sp94].unk_01 = 0xB;
                        D_800E6280.unk_69C[sp94].unk_02 = 4;
                        var_v1_2 = sp94 + 1;
                    } else {
                        D_800E6280.unk_69C[sp94].unk_00 = 0x10;
                        var_v1_2 = sp94 + 1;
                    }
block_138:
                    sp94 = var_v1_2;
                }
            }
            if ((var_fp == 5) && ((u16) D_800E62C2 >= 0x21U)) {
                if (D_800E691C == 0x40) {
                    if ((rand() & 1) && (D_800E6640 & 2) && ((rand() % 100) < 0x32) && (get_g_zyotai_h(9) < 3U) && ((D_800E638A + D_800E6382 + D_800E6386) >= 0x12D)) {
                        D_800E6280.unk_F68.b[0] = (D_800E6280.unk_F68.b[0] & 0xFFF0) | 3;
                        D_800E6280.unk_69C[sp94].unk_00 = 0x47;
                        D_800E6280.unk_69C[sp94].unk_01 = 9;
                        var_v1_5 = sp94 + 1;
                        goto block_172;
                    }
                } else if ((*(u8 *)((unsigned char *)temp_s0 + (68)) == 0) && ((D_800E71E8 & 0xF) != 7) && (rand() & 3) && (sp6C == 0) && (sp70 == 6)) {
                    if ((sp7C != 0xF) && (sp7C != 0)) {
                        if ((D_800E71E8 & 0xF) == 5) {
                            if (sp7C != 0) {
                                goto block_159;
                            }
                        } else {
block_159:
                            D_800E6280.unk_69C[sp94].unk_00 = 0x47;
                            var_v1_5 = sp94 + 1;
                            D_800E6280.unk_69C[sp94].unk_01 = (u8) sp7C;
                            goto block_172;
                        }
                    } else {
                        temp_t5_2 = D_800E71E8 & 0xF;
                        if (((temp_t5_2 == 0) || (temp_t5_2 == 1) || (temp_t5_2 == 3) || (temp_t5_2 == 2)) && ((rand() % 100) < 7)) {
                            if ((get_g_zyotai_h(9) < 3U) && (D_800E6640 & 2)) {
                                D_800E6280.unk_69C[sp94].unk_00 = 0x47;
                                D_800E6280.unk_69C[sp94].unk_01 = 9;
                                var_v1_5 = sp94 + 1;
                                goto block_172;
                            }
                            goto block_169;
                        }
block_169:
                        if ((sp7C == 0) && ((D_800E71E8 & 0xF) != 5)) {
                            D_800E6280.unk_69C[sp94].unk_00 = 0x47;
                            D_800E6280.unk_69C[sp94].unk_01 = 0;
                            var_v1_5 = sp94 + 1;
block_172:
                            sp94 = var_v1_5;
                        }
                    }
                }
            }
            var_s0_2 = 1;
            if (birth_day_check_days(0, D_800E62BF + var_s4, var_s3) == 1) {
                D_800E6280.unk_69C[sp94].unk_00 = 0x32;
                D_800E6280.unk_69C[sp94].unk_01 = 0;
                sp94 += 1;
            }
            var_s1_4 = &D_800E62B8;
loop_176:
            temp_v0_4 = *(s32 *)((unsigned char *)var_s1_4 + (456));
            if ((temp_v0_4 & 2) && (temp_v0_4 & 0x40) && (birth_day_check_days(var_s0_2, D_800E62BF + var_s4, var_s3) == 1)) {
                D_800E6280.unk_69C[sp94].unk_00 = 0x32;
                D_800E6280.unk_69C[sp94].unk_01 = (u8) var_s0_2;
                sp94 += 1;
            }
            var_s0_2 += 1;
            var_s1_4 += 0x38;
            if (var_s0_2 != 0xB) {
                goto loop_176;
            }
            if ((D_800E6280.unk_0F4.b[1] & 0xF) != 3) {
                var_v1 = sp94;
                if (birth_day_check_days(0xF, D_800E62BF + var_s4, var_s3) == 1) {
                    D_800E6280.unk_69C[sp94].unk_00 = 0x33;
                    sp94 += 1;
                    goto block_184;
                }
            } else {
block_184:
                var_v1 = sp94;
            }
            if (var_fp == 5) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFC;
                var_v1 += 1;
            }
            if (var_fp != 6) {
                D_800E6280.unk_69C[var_v1].unk_00 = 0xFB;
                var_v1 += 1;
            }
            var_fp += 1;
            var_s3 += 1;
        } while ((s32) var_fp < 7);
        sp94 = var_v1;
    }
    func_8004284C();
}
