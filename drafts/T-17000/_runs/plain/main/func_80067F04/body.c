void func_80067F04(void) {
    GameState *sp7C;
    s32 sp68;
    u8 *sp50;
    GameState *var_s2;
    GameState *var_s2_2;
    GameState *var_s2_3;
    GameState *var_s2_4;
    GameState *var_s2_5;
    s32 temp_s1;
    s32 temp_s1_10;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s1_4;
    s32 temp_s1_5;
    s32 temp_s1_6;
    s32 temp_s1_7;
    s32 temp_s1_8;
    s32 temp_s1_9;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s3_2;
    s32 temp_t3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_s4;
    s32 var_s6;
    s32 var_t1;
    s32 var_t3;
    s32 var_t5;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t4;
    u32 temp_t6;
    u8 *var_v0;
    u8 temp_s5;
    u8 temp_t4_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 var_v1;
    u8 var_v1_2;
    u8 *temp_s0;
    u8 *temp_s0_10;
    u8 *temp_s0_11;
    u8 *temp_s0_12;
    u8 *temp_s0_13;
    u8 *temp_s0_14;
    u8 *temp_s0_15;
    u8 *temp_s0_2;
    u8 *temp_s0_3;
    u8 *temp_s0_4;
    u8 *temp_s0_5;
    u8 *temp_s0_6;
    u8 *temp_s0_7;
    u8 *temp_s0_8;
    u8 *temp_s0_9;

    sp7C = &D_800E6280;
    var_v0 = D_8011ECD0;
    var_s1 = 0;
    do {
        sp50 = var_v0;
        func_80048F64(var_s1 + 0x61);
        *(s8 *)((unsigned char *)var_v0 + (6597)) = 3;
        *(s8 *)((unsigned char *)var_v0 + (6598)) = 0x40;
        *(s32 *)((unsigned char *)var_v0 + (6652)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (6599)) = 0x8C;
        *(u8 **)((unsigned char *)var_v0 + (6608)) = sprite_frame_table;
        *(u8 **)((unsigned char *)var_v0 + (6612)) = D_800C9A14;
        *(u8 **)((unsigned char *)var_v0 + (6648)) = D_800C9730;
        var_s1 += 1;
        var_v0 += 0x44;
        *(s8 *)((unsigned char *)var_v0 + (6532)) = 8;
        *(s8 *)((unsigned char *)var_v0 + (6533)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (6534)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (6552)) = 0x62;
        *(s16 *)((unsigned char *)var_v0 + (6566)) = 0x100;
        *(s16 *)((unsigned char *)var_v0 + (6570)) = 0x100;
        *(s8 *)((unsigned char *)var_v0 + (6535)) = 0x80;
        *(s16 *)((unsigned char *)var_v0 + (6548)) = (s16) D_800C9A54;
    } while (var_s1 < 0x20);
    temp_t4 = (u32) (D_800E6378 << 0x17) >> 0x1B;
    temp_s3 = D_800E6378 & 0xF;
    var_s6 = 0;
    var_s4 = 0;
    if (temp_s3 == D_800E62BF) {
        temp_s1 = temp_t4 - 1;
        D_801206AC = 0x6E;
        D_801206BA = func_80068898(0, temp_s1);
        D_801206BE = func_800688F0(0, temp_s1);
        var_s4 = 1;
    }
    if (temp_s3 == (D_800E62BF + 1)) {
        temp_s1_2 = temp_t4 - 1;
        temp_s0 = D_8011ECD0 + (var_s4 * 0x44);
        *(s16 *)((unsigned char *)temp_s0 + (6620)) = 0x6E;
        *(s16 *)((unsigned char *)temp_s0 + (6634)) = func_80068898(1, temp_s1_2);
        *(s16 *)((unsigned char *)temp_s0 + (6638)) = func_800688F0(1, temp_s1_2);
        var_s4 += 1;
    }
    var_s2 = &D_800E6280;
    do {
        temp_v0 = var_s2->unk_1BC[0].unk_10.w;
        temp_t1 = (u32) (temp_v0 << 0x17) >> 0x1B;
        temp_s3_2 = temp_v0 & 0xF;
        if (var_s2->unk_1BC[0].unk_0C.w & 0x40) {
            if (temp_s3_2 == D_800E62BF) {
                temp_s1_3 = temp_t1 - 1;
                temp_s0_2 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_2 + (6620)) = 0x6D;
                *(s16 *)((unsigned char *)temp_s0_2 + (6634)) = func_80068898(0, temp_s1_3);
                *(s16 *)((unsigned char *)temp_s0_2 + (6638)) = func_800688F0(0, temp_s1_3);
                var_s4 += 1;
            }
            temp_v0_2 = D_800E62BF + 1;
            if ((temp_s3_2 == temp_v0_2) || ((temp_s3_2 + 0xC) == temp_v0_2)) {
                temp_s1_4 = temp_t1 - 1;
                temp_s0_3 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_3 + (6620)) = 0x6D;
                *(s16 *)((unsigned char *)temp_s0_3 + (6634)) = func_80068898(1, temp_s1_4);
                *(s16 *)((unsigned char *)temp_s0_3 + (6638)) = func_800688F0(1, temp_s1_4);
                var_s4 += 1;
            }
        }
        var_s2 += 0x38;
    } while ((u32) var_s2 < (u32) &D_800E64E8);
    var_s2_2 = &D_800E6280;
    do {
        temp_t2 = (u16) var_s2_2->unk_54C[0].h[0] >> 0xC;
        temp_t3 = var_s2_2->unk_54C[0].h[1] & 0x1F;
        if (var_s2_2->unk_54C[0].w & 0x20000000) {
            if (temp_t2 == D_800E62BF) {
                temp_s1_5 = temp_t3 - 1;
                temp_s0_4 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_4 + (6620)) = 0x62;
                *(s16 *)((unsigned char *)temp_s0_4 + (6634)) = func_80068898(0, temp_s1_5);
                *(s16 *)((unsigned char *)temp_s0_4 + (6638)) = func_800688F0(0, temp_s1_5);
                var_s4 += 1;
            }
            if (temp_t2 == (((u8) D_800E62BF % 12U) + 1)) {
                temp_s1_6 = temp_t3 - 1;
                temp_s0_5 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_5 + (6620)) = 0x62;
                *(s16 *)((unsigned char *)temp_s0_5 + (6634)) = func_80068898(1, temp_s1_6);
                *(s16 *)((unsigned char *)temp_s0_5 + (6638)) = func_800688F0(1, temp_s1_6);
                var_s4 += 1;
            }
        }
        var_s2_2 += 4;
    } while ((u32) var_s2_2 < (u32) &D_800E62A0);
    temp_t6 = (u16) D_800E6374 >> 0xC;
    if ((temp_t6 >= 6U) && (temp_t6 < 0xBU) && ((temp_s1_7 = (0x15 - D_800E62E4) - 1, temp_s2 = 0x15 - D_800E6324, temp_s0_6 = D_8011ECD0 + (var_s4 * 0x44), *(s16 *)((unsigned char *)temp_s0_6 + (6620)) = 0x6F, *(s16 *)((unsigned char *)temp_s0_6 + (6634)) = func_80068898(0, temp_s1_7), *(s16 *)((unsigned char *)temp_s0_6 + (6638)) = func_800688F0(0, temp_s1_7), var_s4 += 1, temp_s0_7 = temp_s0_6 + 0x44, (D_800E62BE != 0x62)) || (D_800E62BF != 2))) {
        temp_s1_8 = temp_s2 - 1;
        *(s16 *)((unsigned char *)temp_s0_7 + (6620)) = 0x6F;
        *(s16 *)((unsigned char *)temp_s0_7 + (6634)) = func_80068898(1, temp_s1_8);
        *(s16 *)((unsigned char *)temp_s0_7 + (6638)) = func_800688F0(1, temp_s1_8);
        var_s4 += 1;
    }
    var_s1_2 = 0;
    if (D_800E62C4 != 0) {
        var_s3 = sp68;
        var_s2_3 = &D_800E6280;
loop_28:
        temp_v0_3 = var_s2_3->unk_044[0].unk_20[1];
        if ((temp_v0_3 != 0) && (var_v1 = D_800B5C10[temp_v0_3], (var_v1 != 0xFF))) {
            if (var_v1 == 0x6F) {
                if (D_800E62BF == 8) {
                    if ((u32) ((u16) D_800E6374 >> 0xC) < 0xBU) {
                        temp_s0_8 = D_8011ECD0 + (var_s4 * 0x44);
                        *(s16 *)((unsigned char *)temp_s0_8 + (6620)) = (s16) var_v1;
                        *(s16 *)((unsigned char *)temp_s0_8 + (6634)) = func_80068898(0, var_s1_2);
                        *(s16 *)((unsigned char *)temp_s0_8 + (6638)) = func_800688F0(0, var_s1_2);
                        var_v1 = D_800B5C10[var_s2_3->unk_044[0].unk_20[1]];
                        goto block_35;
                    }
                }
            } else {
                temp_s0_9 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_9 + (6620)) = (s16) var_v1;
                *(s16 *)((unsigned char *)temp_s0_9 + (6634)) = func_80068898(0, var_s1_2);
                *(s16 *)((unsigned char *)temp_s0_9 + (6638)) = func_800688F0(0, var_s1_2);
                var_v1 = D_800B5C10[var_s2_3->unk_044[0].unk_20[1]];
block_35:
                var_s4 += 1;
            }
            if (var_v1 == 0x66) {
                if (D_800E62BF != 2) {
                    var_s3 = (s32) var_v1;
                    var_s6 = 6;
                    goto block_52;
                }
                goto block_41;
            }
block_41:
            if (var_v1 == 0x6C) {
                var_s3 = (s32) var_v1;
                var_s6 = 6;
                goto block_52;
            }
            if (var_v1 == 0x67) {
                var_s3 = (s32) var_v1;
                var_s6 = 7;
                goto block_52;
            }
            if ((var_v1 == 0x6F) && (D_800E62BF == 8)) {
                var_t5 = var_s4 + 0x61;
                if ((u32) ((u16) D_800E6374 >> 0xC) < 0xBU) {
                    var_s3 = (s32) var_v1;
                    var_s6 = 6;
                    goto block_52;
                }
            } else {
                goto block_52;
            }
        } else {
            var_t5 = var_s4 + 0x61;
            if (var_s6 > 0) {
                temp_s0_10 = D_8011ECD0 + (var_s4 * 0x44);
                *(s16 *)((unsigned char *)temp_s0_10 + (6620)) = (s16) var_s3;
                *(s16 *)((unsigned char *)temp_s0_10 + (6634)) = func_80068898(0, var_s1_2);
                *(s16 *)((unsigned char *)temp_s0_10 + (6638)) = func_800688F0(0, var_s1_2);
                var_s4 += 1;
block_52:
                var_t5 = var_s4 + 0x61;
            }
        }
        var_s6 -= 1;
        if (var_t5 < 0xA0) {
            var_s1_2 += 1;
            var_s2_3 += 1;
            if ((u32) var_s1_2 >= (u8) sp7C->unk_044[0].unk_00[0]) {
                var_s1_2 = 0;
                var_s6 = 0;
                sp68 = var_s3;
                goto block_56;
            }
            goto loop_28;
        }
    } else {
block_56:
        var_s3_2 = sp68;
        if (D_800E6304 != 0) {
            var_s2_4 = &D_800E6280;
loop_58:
            temp_v0_4 = var_s2_4->unk_044[1].unk_20[1];
            if ((temp_v0_4 != 0) && (var_v1_2 = D_800B5C10[temp_v0_4], (var_v1_2 != 0xFF))) {
                if (var_v1_2 == 0x6F) {
                    if (D_800E62BF == 7) {
                        if ((u32) ((u16) D_800E6374 >> 0xC) < 0xBU) {
                            temp_s0_11 = D_8011ECD0 + (var_s4 * 0x44);
                            *(s16 *)((unsigned char *)temp_s0_11 + (6620)) = (s16) var_v1_2;
                            *(s16 *)((unsigned char *)temp_s0_11 + (6634)) = func_80068898(1, var_s1_2);
                            *(s16 *)((unsigned char *)temp_s0_11 + (6638)) = func_800688F0(1, var_s1_2);
                            var_v1_2 = D_800B5C10[var_s2_4->unk_044[1].unk_20[1]];
                            goto block_65;
                        }
                    }
                } else {
                    temp_s0_12 = D_8011ECD0 + (var_s4 * 0x44);
                    *(s16 *)((unsigned char *)temp_s0_12 + (6620)) = (s16) var_v1_2;
                    *(s16 *)((unsigned char *)temp_s0_12 + (6634)) = func_80068898(1, var_s1_2);
                    *(s16 *)((unsigned char *)temp_s0_12 + (6638)) = func_800688F0(1, var_s1_2);
                    var_v1_2 = D_800B5C10[var_s2_4->unk_044[1].unk_20[1]];
block_65:
                    var_s4 += 1;
                }
                if (var_v1_2 == 0x66) {
                    if (D_800E62BF != 1) {
                        var_s3_2 = (s32) var_v1_2;
                        var_s6 = 6;
                        goto block_82;
                    }
                    goto block_71;
                }
block_71:
                if (var_v1_2 == 0x6C) {
                    var_s3_2 = (s32) var_v1_2;
                    var_s6 = 6;
                    goto block_82;
                }
                if (var_v1_2 == 0x67) {
                    var_s3_2 = (s32) var_v1_2;
                    var_s6 = 7;
                    goto block_82;
                }
                if ((var_v1_2 == 0x6F) && (D_800E62BF == 7)) {
                    var_t1 = var_s4 + 0x61;
                    if ((u32) ((u16) D_800E6374 >> 0xC) < 0xBU) {
                        var_s3_2 = (s32) var_v1_2;
                        var_s6 = 6;
                        goto block_82;
                    }
                } else {
                    goto block_82;
                }
            } else {
                var_t1 = var_s4 + 0x61;
                if (var_s6 > 0) {
                    temp_s0_13 = D_8011ECD0 + (var_s4 * 0x44);
                    *(s16 *)((unsigned char *)temp_s0_13 + (6620)) = (s16) var_s3_2;
                    *(s16 *)((unsigned char *)temp_s0_13 + (6634)) = func_80068898(1, var_s1_2);
                    *(s16 *)((unsigned char *)temp_s0_13 + (6638)) = func_800688F0(1, var_s1_2);
                    var_s4 += 1;
block_82:
                    var_t1 = var_s4 + 0x61;
                }
            }
            var_s6 -= 1;
            if (var_t1 < 0xA0) {
                var_s1_2 += 1;
                var_s2_4 += 1;
                if ((u32) var_s1_2 >= (u8) sp7C->unk_044[1].unk_00[0]) {
                    goto block_85;
                }
                goto loop_58;
            }
        } else {
block_85:
            var_s2_5 = &D_800E6280;
loop_86:
            temp_s5 = var_s2_5->unk_75E[0].unk_06;
            if (var_s2_5->unk_75E[0].unk_04 == D_800E62BE) {
                var_t3 = var_s4 + 0x61;
                if (var_s2_5->unk_75E[0].unk_05 == D_800E62BF) {
                    if (var_s2_5->unk_75E[0].unk_00 == 0xC) {
                        if ((D_800E7D30 != 0) || (var_t3 = var_s4 + 0x61, (D_800E67C8 != 0))) {
                            temp_s1_9 = temp_s5 - 1;
                            temp_s0_14 = D_8011ECD0 + (var_s4 * 0x44);
                            *(s16 *)((unsigned char *)temp_s0_14 + (6620)) = 0x62;
                            *(s16 *)((unsigned char *)temp_s0_14 + (6634)) = func_80068898(0, temp_s1_9);
                            *(s16 *)((unsigned char *)temp_s0_14 + (6638)) = func_800688F0(0, temp_s1_9);
                            goto block_93;
                        }
                    } else {
                        temp_s1_10 = temp_s5 - 1;
                        temp_s0_15 = D_8011ECD0 + (var_s4 * 0x44);
                        *(s16 *)((unsigned char *)temp_s0_15 + (6620)) = 0x62;
                        *(s16 *)((unsigned char *)temp_s0_15 + (6634)) = func_80068898(0, temp_s1_10);
                        *(s16 *)((unsigned char *)temp_s0_15 + (6638)) = func_800688F0(0, temp_s1_10);
block_93:
                        var_s4 += 1;
                        goto block_94;
                    }
                }
            } else {
block_94:
                var_t3 = var_s4 + 0x61;
            }
            if (var_t3 < 0xA0) {
                temp_t4_2 = var_s2_5->unk_75E[0].unk_04;
                var_s2_5 += 8;
                if (temp_t4_2 != 0) {
                    if (var_s2_5 == &D_800E6A80) {

                    } else {
                        goto loop_86;
                    }
                }
            }
        }
    }
}
