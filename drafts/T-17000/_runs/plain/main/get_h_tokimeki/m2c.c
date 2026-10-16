extern u8 D_800B4199;
extern u8 D_800B419A;
extern u8 D_800B419B;
extern u8 D_800B419C;
extern u8 D_800B419D;
extern u8 D_800B419E;

u32 get_h_tokimeki(s32 arg0) {
    ? sp58;
    s32 sp40;
    s16 temp_s2;
    s16 temp_s2_2;
    s16 temp_s2_3;
    s16 temp_s2_4;
    s16 temp_s2_5;
    s16 temp_s2_6;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 temp_hi;
    s32 var_a0;
    s32 var_t0;
    s32 var_v1;
    s8 *temp_s1;
    s8 *temp_s1_2;
    s8 *temp_s1_3;
    s8 *temp_s1_4;
    s8 *temp_s1_5;
    s8 *temp_s1_6;
    s8 var_t7;
    u32 var_v0;
    u8 temp_a3;

    temp_hi = arg0 % 13;
    var_v0_2 = &sp40;
    var_t0 = 1;
    var_v1 = (s32) (D_800B4100[temp_hi] * D_800E6280.unk_1BC[temp_hi].unk_02) / 100;
    do {
        var_t0 += 1;
        if (D_800B4110[temp_hi] & (1 << var_t0)) {
            *var_v0_2 = 1;
        } else {
            *var_v0_2 = 0;
        }
        var_v0_2 += 4;
    } while (var_t0 < 8);
    temp_a3 = D_800B4120[temp_hi];
    var_a0 = 4;
    var_v0_3 = &sp40;
    do {
        if (*var_v0_3 != 0) {
            switch (temp_a3) {
            case 1:
                temp_s2 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1 = D_800B4130[temp_hi];
                if ((u32) (*D_800B4198 + 0x32) < (u32) temp_s2) {
                    var_v1 += temp_s1->unk0;
                } else {
                    if ((u32) (D_800B4199 + 0x23) < (u32) temp_s2) {
                        var_t7 = temp_s1[1];
                        goto block_112;
                    }
                    if ((u32) (D_800B419A + 0x14) < (u32) temp_s2) {
                        var_v1 += temp_s1[2];
                    } else {
                        if ((u32) (D_800B419B + 0x14) < (u32) temp_s2) {
                            var_t7 = temp_s1[3];
                            goto block_112;
                        }
                        if ((u32) (D_800B419C + 5) < (u32) temp_s2) {
                            var_v1 += temp_s1[4];
                        } else if ((u32) (D_800B419D + 5) < (u32) temp_s2) {
                            var_t7 = temp_s1[5];
block_112:
                            var_v1 += var_t7;
                        } else if ((u8) D_800B419E < (u32) temp_s2) {
                            var_v1 += temp_s1[6];
                        } else {
                            var_v1 += temp_s1[7];
                        }
                    }
                }
                break;
            case 2:
                temp_s2_2 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1_2 = D_800B4130[temp_hi];
                if ((u8) *D_800B4198 < (u32) temp_s2_2) {
                    var_v1 += temp_s1_2->unk0;
                } else if ((u8) D_800B4199 < (u32) temp_s2_2) {
                    var_v1 += temp_s1_2[1];
                } else {
                    if ((u8) D_800B419A < (u32) temp_s2_2) {
                        var_t7 = temp_s1_2[2];
                        goto block_112;
                    }
                    if ((u8) D_800B419B < (u32) temp_s2_2) {
                        var_v1 += temp_s1_2[3];
                    } else if ((u8) D_800B419C < (u32) temp_s2_2) {
                        var_v1 += temp_s1_2[4];
                    } else if ((u8) D_800B419D < (u32) temp_s2_2) {
                        var_v1 += temp_s1_2[5];
                    } else {
                        if ((u8) D_800B419E < (u32) temp_s2_2) {
                            var_t7 = temp_s1_2[6];
                            goto block_112;
                        }
                        var_v1 += temp_s1_2[7];
                    }
                }
                break;
            case 3:
                temp_s2_3 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1_3 = D_800B4130[temp_hi];
                if ((u32) (*D_800B4198 + 0x32) < (u32) temp_s2_3) {
                    var_v1 += temp_s1_3->unk0;
                } else {
                    if ((u32) (D_800B4199 + 0x32) < (u32) temp_s2_3) {
                        var_t7 = temp_s1_3[1];
                        goto block_112;
                    }
                    if ((u32) (D_800B419A + 0x32) < (u32) temp_s2_3) {
                        var_v1 += temp_s1_3[2];
                    } else {
                        if ((u32) (D_800B419B + 0x14) < (u32) temp_s2_3) {
                            var_t7 = temp_s1_3[3];
                            goto block_112;
                        }
                        if ((u32) (D_800B419C + 0x14) < (u32) temp_s2_3) {
                            var_v1 += temp_s1_3[4];
                        } else {
                            if ((u32) (D_800B419D + 0x14) < (u32) temp_s2_3) {
                                var_t7 = temp_s1_3[5];
                                goto block_112;
                            }
                            if ((u32) (D_800B419E + 0x14) < (u32) temp_s2_3) {
                                var_v1 += temp_s1_3[6];
                            } else {
                                var_v1 += temp_s1_3[7];
                            }
                        }
                    }
                }
                break;
            case 4:
                temp_s2_4 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1_4 = D_800B4130[temp_hi];
                if ((u32) (*D_800B4198 + 0xA) < (u32) temp_s2_4) {
                    var_t7 = temp_s1_4->unk0;
                    goto block_112;
                }
                if ((u32) (D_800B4199 + 0xA) < (u32) temp_s2_4) {
                    var_v1 += temp_s1_4[1];
                } else {
                    if ((u32) (D_800B419A + 0xA) < (u32) temp_s2_4) {
                        var_t7 = temp_s1_4[2];
                        goto block_112;
                    }
                    if ((u32) (D_800B419B + 0xA) < (u32) temp_s2_4) {
                        var_v1 += temp_s1_4[3];
                    } else if ((u8) D_800B419C < (u32) temp_s2_4) {
                        var_v1 += temp_s1_4[4];
                    } else {
                        if ((u8) D_800B419D < (u32) temp_s2_4) {
                            var_t7 = temp_s1_4[5];
                            goto block_112;
                        }
                        if ((u8) D_800B419E < (u32) temp_s2_4) {
                            var_v1 += temp_s1_4[6];
                        } else {
                            var_v1 += temp_s1_4[7];
                        }
                    }
                }
                break;
            case 5:
                temp_s2_5 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1_5 = D_800B4130[temp_hi];
                if ((u32) (*D_800B4198 + 0x14) < (u32) temp_s2_5) {
                    var_v1 += temp_s1_5->unk0;
                } else if ((u32) (D_800B4199 + 0x14) < (u32) temp_s2_5) {
                    var_v1 += temp_s1_5[1];
                } else if ((u32) (D_800B419A + 0x14) < (u32) temp_s2_5) {
                    var_v1 += temp_s1_5[2];
                } else if ((u32) (D_800B419B + 0x14) < (u32) temp_s2_5) {
                    var_v1 += temp_s1_5[3];
                } else if ((u8) D_800B419C < (u32) temp_s2_5) {
                    var_v1 += temp_s1_5[4];
                } else {
                    if ((u32) (D_800B419D - 5) < (u32) temp_s2_5) {
                        var_t7 = temp_s1_5[5];
                        goto block_112;
                    }
                    if ((u8) D_800B419E < (u32) temp_s2_5) {
                        var_v1 += temp_s1_5[6];
                    } else {
                        var_v1 += temp_s1_5[7];
                    }
                }
                break;
            case 6:
                temp_s2_6 = *(s16 *)((unsigned char *)(&D_800E6280 + var_a0) + (254));
                temp_s1_6 = D_800B4130[temp_hi];
                if ((u8) *D_800B4198 < (u32) temp_s2_6) {
                    var_v1 += temp_s1_6->unk0;
                } else if ((u8) D_800B4199 < (u32) temp_s2_6) {
                    var_v1 += temp_s1_6[1];
                } else {
                    if ((u8) D_800B419A < (u32) temp_s2_6) {
                        var_t7 = temp_s1_6[2];
                        goto block_112;
                    }
                    if ((u8) D_800B419B < (u32) temp_s2_6) {
                        var_v1 += temp_s1_6[3];
                    } else if ((u32) (D_800B419C - 5) < (u32) temp_s2_6) {
                        var_v1 += temp_s1_6[4];
                    } else if ((u32) (D_800B419D - 5) < (u32) temp_s2_6) {
                        var_v1 += temp_s1_6[5];
                    } else if ((u32) (D_800B419E - 5) < (u32) temp_s2_6) {
                        var_v1 += temp_s1_6[6];
                    } else {
                        var_t7 = temp_s1_6[7];
                        goto block_112;
                    }
                }
                break;
            }
        }
        var_v0_3 += 4;
        var_a0 += 4;
    } while (var_v0_3 != &sp58);
    if (var_v1 >= 0x65) {
        return 0x64U;
    }
    var_v0 = var_v1 & 0xFF;
    if (var_v1 < 0) {
        var_v0 = 0;
    }
    return var_v0;
}
