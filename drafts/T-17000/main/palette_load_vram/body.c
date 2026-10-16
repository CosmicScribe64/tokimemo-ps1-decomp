void palette_load_vram(s32 arg0) {
    GameState *sp74;
    u16 sp72;
    u16 sp70;
    u16 sp6E;
    u32 sp68;
    GameState *sp30;
    GameState *var_t1;
    s32 *var_v0_11;
    s32 *var_v0_12;
    s32 *var_v0_14;
    s32 *var_v0_15;
    s32 *var_v0_17;
    s32 *var_v0_18;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_5;
    s32 *var_v0_6;
    s32 *var_v0_8;
    s32 *var_v0_9;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a0_5;
    s32 temp_a0_6;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 temp_a2_4;
    s32 temp_t7;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_v0;
    s32 var_v0_10;
    s32 var_v0_13;
    s32 var_v0_16;
    s32 var_v0_4;
    s32 var_v0_7;
    s32 var_v1;
    s32 var_v1_10;
    s32 var_v1_11;
    s32 var_v1_12;
    s32 var_v1_13;
    s32 var_v1_14;
    s32 var_v1_15;
    s32 var_v1_16;
    s32 var_v1_17;
    s32 var_v1_18;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    s32 var_v1_7;
    s32 var_v1_8;
    s32 var_v1_9;
    u32 var_ra;
    u8 temp_t2;
    u8 temp_t8;
    u8 temp_v0;

    sp74 = &D_800E6280;
    var_ra = 0;
    if (D_800E6280.unk_1428 != 0) {
        var_t1 = &D_800E6280;
        do {
            temp_v0 = var_t1->unk_1328[0].unk_04;
            temp_t2 = var_t1->unk_1328[0].unk_07;
            temp_t7 = temp_t2 << 7;
            sp70 = (temp_v0 & 0x1F) + 0x1E0;
            if (temp_v0 < 0x20U) {
                temp_t8 = var_t1->unk_1328[0].unk_06;
                sp6E = 0x100;
                sp72 = 0;
                switch (temp_t8) {
                case 0:
                    temp_a2 = temp_t7 + 0x80;
                    var_v0 = temp_t7;
                    if (temp_t7 < temp_a2) {
                        if (((temp_t2 * 0) + 0x80) & 1) {
                            temp_a0 = temp_t7 * 4;
                            var_v0_2 = var_t1->unk_1328[0].unk_00 + temp_a0;
                            var_v1 = *var_v0_2;
                            if (var_v1 & 0x80000000) {
                                *var_v0_2 = var_v1 & 0x7FFF7FFF;
                                var_v0_2 = var_t1->unk_1328[0].unk_00 + temp_a0;
                                var_v1 = *var_v0_2;
                            }
                            if (var_v1 & 0x8000) {
                                *var_v0_2 = var_v1 & 0x7FFF7FFF;
                            }
                            var_v0 = temp_t7 + 1;
                            if (var_v0 != temp_a2) {
                                goto block_12;
                            }
                        } else {
block_12:
                            var_a0 = var_v0 * 4;
                            do {
                                var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                var_v1_2 = *var_v0_3;
                                if (var_v1_2 & 0x80000000) {
                                    *var_v0_3 = var_v1_2 & 0x7FFF7FFF;
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                    var_v1_2 = *var_v0_3;
                                }
                                if (var_v1_2 & 0x8000) {
                                    *var_v0_3 = var_v1_2 & 0x7FFF7FFF;
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                }
                                var_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (4));
                                if (var_v1_3 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_3 + (4)) = (s32) (var_v1_3 & 0x7FFF7FFF);
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                    var_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (4));
                                }
                                var_a0 += 8;
                                if (var_v1_3 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_3 + (4)) = (s32) (var_v1_3 & 0x7FFF7FFF);
                                }
                            } while (var_a0 != (temp_a2 * 4));
                        }
                    }
                    break;
                default:
                    break;
                case 2:
                    temp_a1 = temp_t7 + 0x78;
                    var_v0_4 = temp_t7;
                    if (temp_t7 < temp_a1) {
                        if (((temp_t2 * 0) + 0x78) & 1) {
                            temp_a0_2 = temp_t7 * 4;
                            var_v0_5 = var_t1->unk_1328[0].unk_00 + temp_a0_2;
                            var_v1_4 = *var_v0_5;
                            if (var_v1_4 & 0x80000000) {
                                *var_v0_5 = var_v1_4 & 0x7FFF7FFF;
                                var_v0_5 = var_t1->unk_1328[0].unk_00 + temp_a0_2;
                                var_v1_4 = *var_v0_5;
                            }
                            if (var_v1_4 & 0x8000) {
                                *var_v0_5 = var_v1_4 & 0x7FFF7FFF;
                            }
                            var_v0_4 = temp_t7 + 1;
                            if (var_v0_4 != temp_a1) {
                                goto block_31;
                            }
                        } else {
block_31:
                            var_a0_2 = var_v0_4 * 4;
                            do {
                                var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                var_v1_5 = *var_v0_6;
                                if (var_v1_5 & 0x80000000) {
                                    *var_v0_6 = var_v1_5 & 0x7FFF7FFF;
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                    var_v1_5 = *var_v0_6;
                                }
                                if (var_v1_5 & 0x8000) {
                                    *var_v0_6 = var_v1_5 & 0x7FFF7FFF;
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                }
                                var_v1_6 = *(s32 *)((unsigned char *)var_v0_6 + (4));
                                if (var_v1_6 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_6 + (4)) = (s32) (var_v1_6 & 0x7FFF7FFF);
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                    var_v1_6 = *(s32 *)((unsigned char *)var_v0_6 + (4));
                                }
                                var_a0_2 += 8;
                                if (var_v1_6 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_6 + (4)) = (s32) (var_v1_6 & 0x7FFF7FFF);
                                }
                            } while (var_a0_2 != (temp_a1 * 4));
                        }
                    }
                    temp_a2_2 = temp_t7 + 0x80;
                    var_v0_7 = temp_a1;
                    if (temp_a1 < temp_a2_2) {
                        temp_a0_3 = temp_a1 * 4;
                        if (((temp_t7 - temp_a1) + 0x80) & 1) {
                            var_v0_8 = var_t1->unk_1328[0].unk_00 + temp_a0_3;
                            var_v1_7 = *var_v0_8;
                            if (var_v1_7 & 0x7FFF0000) {
                                *var_v0_8 = var_v1_7 | 0x80000000;
                                var_v0_8 = var_t1->unk_1328[0].unk_00 + temp_a0_3;
                                var_v1_7 = *var_v0_8;
                            }
                            if (var_v1_7 & 0x7FFF) {
                                *var_v0_8 = var_v1_7 | 0x8000;
                            }
                            var_v0_7 = temp_a1 + 1;
                            if (var_v0_7 != temp_a2_2) {
                                goto block_48;
                            }
                        } else {
block_48:
                            var_a0_3 = var_v0_7 * 4;
                            var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                            do {
                                var_v1_8 = *var_v0_9;
                                if (var_v1_8 & 0x7FFF0000) {
                                    *var_v0_9 = var_v1_8 | 0x80000000;
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                    var_v1_8 = *var_v0_9;
                                }
                                if (var_v1_8 & 0x7FFF) {
                                    *var_v0_9 = var_v1_8 | 0x8000;
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                }
                                var_v1_9 = *(s32 *)((unsigned char *)var_v0_9 + (4));
                                if (var_v1_9 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_9 + (4)) = (s32) (var_v1_9 | 0x80000000);
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                    var_v1_9 = *(s32 *)((unsigned char *)var_v0_9 + (4));
                                }
                                var_a0_3 += 8;
                                if (var_v1_9 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_9 + (4)) = (s32) (var_v1_9 | 0x8000);
                                }
                                var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                            } while (var_a0_3 != (temp_a2_2 * 4));
                        }
                    }
                    break;
                case 3:
                    temp_a2_3 = temp_t7 + 0x80;
                    var_v0_10 = temp_t7;
                    if (temp_t7 < temp_a2_3) {
                        if (((temp_t2 * 0) + 0x80) & 1) {
                            temp_a0_4 = temp_t7 * 4;
                            var_v0_11 = var_t1->unk_1328[0].unk_00 + temp_a0_4;
                            var_v1_10 = *var_v0_11;
                            if (var_v1_10 & 0x7FFF0000) {
                                *var_v0_11 = var_v1_10 | 0x80000000;
                                var_v0_11 = var_t1->unk_1328[0].unk_00 + temp_a0_4;
                                var_v1_10 = *var_v0_11;
                            }
                            if (var_v1_10 & 0x7FFF) {
                                *var_v0_11 = var_v1_10 | 0x8000;
                            }
                            var_v0_10 = temp_t7 + 1;
                            if (var_v0_10 != temp_a2_3) {
                                goto block_66;
                            }
                        } else {
block_66:
                            var_a0_4 = var_v0_10 * 4;
                            do {
                                var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                var_v1_11 = *var_v0_12;
                                if (var_v1_11 & 0x7FFF0000) {
                                    *var_v0_12 = var_v1_11 | 0x80000000;
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                    var_v1_11 = *var_v0_12;
                                }
                                if (var_v1_11 & 0x7FFF) {
                                    *var_v0_12 = var_v1_11 | 0x8000;
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                }
                                var_v1_12 = *(s32 *)((unsigned char *)var_v0_12 + (4));
                                if (var_v1_12 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_12 + (4)) = (s32) (var_v1_12 | 0x80000000);
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                    var_v1_12 = *(s32 *)((unsigned char *)var_v0_12 + (4));
                                }
                                var_a0_4 += 8;
                                if (var_v1_12 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_12 + (4)) = (s32) (var_v1_12 | 0x8000);
                                }
                            } while (var_a0_4 != (temp_a2_3 * 4));
                        }
                    }
                    break;
                case 4:
                    temp_a1_2 = temp_t7 + 0x70;
                    var_v0_13 = temp_t7;
                    if (temp_t7 < temp_a1_2) {
                        if (((temp_t2 * 0) + 0x70) & 1) {
                            temp_a0_5 = temp_t7 * 4;
                            var_v0_14 = var_t1->unk_1328[0].unk_00 + temp_a0_5;
                            var_v1_13 = *var_v0_14;
                            if (var_v1_13 & 0x80000000) {
                                *var_v0_14 = var_v1_13 & 0x7FFF7FFF;
                                var_v0_14 = var_t1->unk_1328[0].unk_00 + temp_a0_5;
                                var_v1_13 = *var_v0_14;
                            }
                            if (var_v1_13 & 0x8000) {
                                *var_v0_14 = var_v1_13 & 0x7FFF7FFF;
                            }
                            var_v0_13 = temp_t7 + 1;
                            if (var_v0_13 != temp_a1_2) {
                                goto block_84;
                            }
                        } else {
block_84:
                            var_a0_5 = var_v0_13 * 4;
                            do {
                                var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                var_v1_14 = *var_v0_15;
                                if (var_v1_14 & 0x80000000) {
                                    *var_v0_15 = var_v1_14 & 0x7FFF7FFF;
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                    var_v1_14 = *var_v0_15;
                                }
                                if (var_v1_14 & 0x8000) {
                                    *var_v0_15 = var_v1_14 & 0x7FFF7FFF;
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                }
                                var_v1_15 = *(s32 *)((unsigned char *)var_v0_15 + (4));
                                if (var_v1_15 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_15 + (4)) = (s32) (var_v1_15 & 0x7FFF7FFF);
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                    var_v1_15 = *(s32 *)((unsigned char *)var_v0_15 + (4));
                                }
                                var_a0_5 += 8;
                                if (var_v1_15 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_15 + (4)) = (s32) (var_v1_15 & 0x7FFF7FFF);
                                }
                            } while (var_a0_5 != (temp_a1_2 * 4));
                        }
                    }
                    temp_a2_4 = temp_t7 + 0x80;
                    var_v0_16 = temp_a1_2;
                    if (temp_a1_2 < temp_a2_4) {
                        temp_a0_6 = temp_a1_2 * 4;
                        if (((temp_t7 - temp_a1_2) + 0x80) & 1) {
                            var_v0_17 = var_t1->unk_1328[0].unk_00 + temp_a0_6;
                            var_v1_16 = *var_v0_17;
                            if (var_v1_16 & 0x7FFF0000) {
                                *var_v0_17 = var_v1_16 | 0x80000000;
                                var_v0_17 = var_t1->unk_1328[0].unk_00 + temp_a0_6;
                                var_v1_16 = *var_v0_17;
                            }
                            if (var_v1_16 & 0x7FFF) {
                                *var_v0_17 = var_v1_16 | 0x8000;
                            }
                            var_v0_16 = temp_a1_2 + 1;
                            if (var_v0_16 != temp_a2_4) {
                                goto block_101;
                            }
                        } else {
block_101:
                            var_a0_6 = var_v0_16 * 4;
                            var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                            do {
                                var_v1_17 = *var_v0_18;
                                if (var_v1_17 & 0x7FFF0000) {
                                    *var_v0_18 = var_v1_17 | 0x80000000;
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                    var_v1_17 = *var_v0_18;
                                }
                                if (var_v1_17 & 0x7FFF) {
                                    *var_v0_18 = var_v1_17 | 0x8000;
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                }
                                var_v1_18 = *(s32 *)((unsigned char *)var_v0_18 + (4));
                                if (var_v1_18 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_18 + (4)) = (s32) (var_v1_18 | 0x80000000);
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                    var_v1_18 = *(s32 *)((unsigned char *)var_v0_18 + (4));
                                }
                                var_a0_6 += 8;
                                if (var_v1_18 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_18 + (4)) = (s32) (var_v1_18 | 0x8000);
                                }
                                var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                            } while (var_a0_6 != (temp_a2_4 * 4));
                        }
                    }
                    break;
                }
            } else {
                sp6E = 0x20;
                if (temp_v0 < 0x40U) {
                    sp72 = 0x100;
                } else {
                    sp72 = 0x120;
                }
            }
            sp68 = var_ra;
            sp30 = var_t1;
            LoadSquare(sp72, sp70, sp6E, 1U, var_t1->unk_1328[0].unk_00 + (temp_t7 * 4));
            var_ra += 1;
            var_t1 = (GameState *) &var_t1->unk_006[2];
        } while (var_ra < (u8) sp74->unk_1428);
    }
    D_800E6280.unk_1428 = 0;
}
