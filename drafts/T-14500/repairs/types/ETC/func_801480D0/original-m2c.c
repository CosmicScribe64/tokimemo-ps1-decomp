extern ? D_800E6328;
extern s16 D_800E643E;
extern s16 D_800E6476;
extern s16 D_800E64AE;
extern ? D_800E64E8;
extern s32 D_800E7378;
extern s32 D_8012310C;
extern s16 D_80150668;
extern s16 D_8015066A;
extern ? D_8015066C;
extern s16 D_80150682;
extern s32 D_80150684;
extern s32 D_80150688;
extern s32 D_801506BC;
extern s32 D_801506C0;
extern ? D_801506C4;
extern u8 D_80150728;
extern u8 D_80150738;

void func_801480D0(void) {
    s32 spBC;
    s32 spAC;
    s32 spA4;
    ? spA0;
    ? sp70;
    s32 sp6C;
    s32 sp68;
    u8 *sp54;
    ? *var_s0_4;
    ? *var_s1_5;
    ? *var_s1_6;
    ? *var_s1_7;
    ? *var_v0;
    ? *var_v0_3;
    s16 *temp_a1;
    s16 *var_s0;
    s16 *var_s0_2;
    s16 *var_s0_3;
    s16 *var_s0_6;
    s16 *var_s0_8;
    s16 *var_s0_9;
    s16 temp_a2_2;
    s16 temp_a2_3;
    s16 temp_a2_5;
    s16 temp_t3_3;
    s16 temp_t4;
    s16 temp_t4_4;
    s16 temp_t6_4;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t8_5;
    s16 temp_t9;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s16 var_a0_4;
    s32 *temp_a2_4;
    s32 *temp_v1_6;
    s32 *var_a1;
    s32 *var_s0_5;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 *var_s1_4;
    s32 *var_s1_8;
    s32 *var_s1_9;
    s32 *var_v0_2;
    s32 temp_a2;
    s32 temp_a2_6;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_t0;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t4_2;
    s32 temp_t4_3;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1_10;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 temp_v1_9;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_at;
    s32 var_s0_7;
    s32 var_s2;
    s32 var_s2_10;
    s32 var_s2_2;
    s32 var_s2_3;
    s32 var_s2_4;
    s32 var_s2_5;
    s32 var_s2_6;
    s32 var_s2_7;
    s32 var_s2_8;
    s32 var_s2_9;
    s32 var_s3_2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t0_3;
    u8 *var_s3;
    void *temp_v1_11;

    func_800AE120(D_800E7378);
    var_s3 = &D_80150728;
    var_s0 = &D_80150668;
    sp54 = &D_80150738;
    var_s2 = 0;
    do {
        temp_s1 = func_800AE0D0();
        spBC = (((func_800AE0D0() & 1) * 2) - 1) * (temp_s1 % (s32) *sp54);
        temp_s1_2 = func_800AE0D0();
        *var_s0 = ((*var_s3 + ((((func_800AE0D0() & 1) * 2) - 1) * (temp_s1_2 % (s32) *sp54))) * 5) + spBC;
        temp_v0 = func_801464D4(var_s2);
        if (temp_v0 != 1) {
            if (temp_v0 == 9) {
                *var_s0 += func_800AE0D0() % 250;
            }
        } else {
            *var_s0 += func_800AE0D0() % 50;
        }
        if (*var_s0 >= 0x1F5) {
            *var_s0 = 0x1F4;
        }
        func_800AE0B0("[%d:%d]\n", var_s2, *var_s0);
        var_s2 += 1;
        sp54 += 1;
        var_s0 += 2;
        var_s3 += 1;
    } while (var_s2 != 0xD);
    D_80150682 = (s16) D_8012310C;
    var_a0 = 0;
    do {
        var_a3 = 0;
        var_t0 = 1;
loop_10:
        var_s2_2 = var_t0;
        if (var_t0 < 0xE) {
            temp_a2 = (0xE - var_t0) & 3;
            temp_a1 = &(&D_80150668)[var_a3];
            if (temp_a2 != 0) {
                var_s0_2 = &(&D_80150668)[var_s2_2];
                do {
                    temp_a2_2 = *var_s0_2;
                    var_s2_2 += 1;
                    if (temp_a2_2 == *temp_a1) {
                        var_a0 += 1;
                        *var_s0_2 = temp_a2_2 - 1;
                    }
                    var_s0_2 += 2;
                } while ((temp_a2 + var_t0) != var_s2_2);
                if (var_s2_2 != 0xE) {
                    goto block_17;
                }
            } else {
block_17:
                var_s0_3 = &(&D_80150668)[var_s2_2];
                do {
                    temp_a2_3 = var_s0_3->unk0;
                    if (temp_a2_3 == *temp_a1) {
                        var_s0_3->unk0 = temp_a2_3 - 1;
                        var_a0 += 1;
                    }
                    temp_v1 = *(s16 *)((unsigned char *)var_s0_3 + (2));
                    if (temp_v1 == *temp_a1) {
                        *(s16 *)((unsigned char *)var_s0_3 + (2)) = (s16) (temp_v1 - 1);
                        var_a0 += 1;
                    }
                    temp_v1_2 = *(s16 *)((unsigned char *)var_s0_3 + (4));
                    if (temp_v1_2 == *temp_a1) {
                        *(s16 *)((unsigned char *)var_s0_3 + (4)) = (s16) (temp_v1_2 - 1);
                        var_a0 += 1;
                    }
                    temp_v1_3 = *(s16 *)((unsigned char *)var_s0_3 + (6));
                    if (temp_v1_3 == *temp_a1) {
                        var_a0 += 1;
                        *(s16 *)((unsigned char *)var_s0_3 + (6)) = (s16) (temp_v1_3 - 1);
                    }
                    var_s0_3 += 8;
                } while (var_s0_3 != &D_80150684);
            }
        }
        var_a3 = var_t0;
        var_t0 += 1;
        if (var_a3 < 0xE) {
            goto loop_10;
        }
        var_a0 = 0;
    } while (var_a0 != 0);
    var_s0_4 = &D_8015066C;
    var_v0 = &sp70;
    sp6C = (s32) D_8015066A;
    sp68 = (s32) D_80150668;
    do {
        temp_t4 = *(s16 *)((unsigned char *)var_s0_4 + (2));
        temp_t9 = *(s16 *)((unsigned char *)var_s0_4 + (4));
        temp_t8 = *(s16 *)((unsigned char *)var_s0_4 + (6));
        temp_t7 = var_s0_4->unk0;
        var_v0 += 0x10;
        var_s0_4 += 8;
        *(s32 *)((unsigned char *)var_v0 + (-12)) = (s32) temp_t4;
        *(s32 *)((unsigned char *)var_v0 + (-8)) = (s32) temp_t9;
        *(s32 *)((unsigned char *)var_v0 + (-4)) = (s32) temp_t8;
        *(s32 *)((unsigned char *)var_v0 + (-16)) = (s32) temp_t7;
    } while (var_v0 != &spA0);
    var_a1 = &D_80150684;
    var_s3_2 = 0;
    do {
        var_a0_2 = sp68;
        var_s2_3 = 1;
        var_v0_2 = &sp6C;
loop_33:
        temp_v1_4 = *var_v0_2;
        if (var_a0_2 < temp_v1_4) {
            var_a0_2 = temp_v1_4;
            var_s3_2 = var_s2_3;
        }
        var_s2_3 += 1;
        var_v0_2 += 4;
        if (var_s2_3 < 0xE) {
            goto loop_33;
        }
        var_a1 += 4;
        (&sp68)[var_s3_2] = 0;
        *(s32 *)((unsigned char *)var_a1 + (-4)) = var_s3_2;
        var_s3_2 = 0;
    } while ((u32) var_a1 < (u32) &D_801506BC);
    func_800AE0B0("順位計算\n ", var_a1, &sp68, &D_801506BC);
    var_s0_5 = &D_80150684;
    var_s2_4 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_4, *var_s0_5);
        var_s2_4 += 1;
        var_s0_5 += 4;
    } while (var_s2_4 < 0xE);
    func_800AE0B0("順位作成\n ");
    var_t0_2 = func_800AE0D0() & 3;
    var_s1 = &D_801506BC;
    var_s0_6 = &D_80150668;
    var_s2_5 = 0;
    do {
        temp_lo = (s32) (*var_s0_6 << 0xC) / 1000;
        spAC = var_t0_2;
        spA4 = temp_lo;
        temp_v0_2 = func_800A0140(temp_lo);
        temp_v1_5 = ((s32) (temp_v0_2 * 0xC8) / 4096) + var_t0_2 + 0xC8;
        *var_s1 = temp_v1_5;
        func_800AE0B0("[%d:%d %d %d] \n", var_s2_5, spA4, temp_v0_2, temp_v1_5);
        var_s2_5 += 1;
        var_s0_6 += 2;
        var_s1 += 4;
    } while (var_s2_5 != 0xE);
    var_a0_3 = 0;
    do {
        var_a3_2 = 0;
        var_t0_3 = 1;
loop_43:
        var_s2_6 = var_t0_3;
        if (var_t0_3 < 0xE) {
            temp_a2_4 = &(&D_801506BC)[var_a3_2];
            if ((0xE - var_t0_3) & 1) {
                temp_v1_6 = &(&D_801506BC)[var_t0_3];
                temp_v0_3 = *temp_v1_6;
                var_s2_6 = var_t0_3 + 1;
                if (temp_v0_3 == *temp_a2_4) {
                    var_a0_3 += 1;
                    if ((&D_80150668)[var_t0_3] < (&D_80150668)[var_a3_2]) {
                        *temp_v1_6 = temp_v0_3 + 1;
                    } else {
                        *temp_v1_6 = temp_v0_3 - 1;
                    }
                }
                if (var_s2_6 != 0xE) {
                    goto block_50;
                }
            } else {
block_50:
                var_s1_2 = &(&D_801506BC)[var_s2_6];
                do {
                    temp_v1_7 = var_s1_2->unk0;
                    if (temp_v1_7 == *temp_a2_4) {
                        var_a0_3 += 1;
                        if ((&D_80150668)[var_s2_6].unk0 < (&D_80150668)[var_a3_2]) {
                            var_s1_2->unk0 = temp_v1_7 + 1;
                        } else {
                            var_s1_2->unk0 = temp_v1_7 - 1;
                        }
                    }
                    temp_v1_8 = *(s32 *)((unsigned char *)var_s1_2 + (4));
                    if (temp_v1_8 == *temp_a2_4) {
                        var_a0_3 += 1;
                        if (*(s16 *)((unsigned char *)(&(&D_80150668)[var_s2_6]) + (2)) < (&D_80150668)[var_a3_2]) {
                            *(s32 *)((unsigned char *)var_s1_2 + (4)) = (s32) (temp_v1_8 + 1);
                        } else {
                            *(s32 *)((unsigned char *)var_s1_2 + (4)) = (s32) (temp_v1_8 - 1);
                        }
                    }
                    var_s2_6 += 2;
                    var_s1_2 += 8;
                } while (var_s2_6 != 0xE);
            }
        }
        var_a3_2 = var_t0_3;
        var_t0_3 += 1;
        if (var_a3_2 < 0xE) {
            goto loop_43;
        }
        var_a0_3 = 0;
    } while (var_a0_3 != 0);
    func_800AE0B0("順位\x83\\ート\n ", &D_80150668);
    var_s1_3 = &D_801506BC;
    var_s2_7 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_7, *var_s1_3);
        var_s2_7 += 1;
        var_s1_3 += 4;
    } while (var_s2_7 < 0xE);
    var_s0_7 = 0x3E8;
    var_s1_4 = &D_801506BC;
    do {
        temp_v1_9 = *var_s1_4;
        var_s1_4 += 4;
        var_at = (u32) var_s1_4 < (u32) &D_801506F4;
        if (temp_v1_9 < var_s0_7) {
            var_s0_7 = temp_v1_9;
            var_at = (u32) var_s1_4 < (u32) &D_801506F4;
        }
    } while (var_at != 0);
    if (var_s0_7 >= 9) {
        var_s1_5 = &D_801506C4;
        temp_v1_10 = ((func_800AE0D0() % (s32) (var_s0_7 - 8)) + var_s0_7) - 8;
        D_801506BC -= temp_v1_10;
        D_801506C0 -= temp_v1_10;
        do {
            temp_t3 = *(s32 *)((unsigned char *)var_s1_5 + (4));
            temp_t4_2 = *(s32 *)((unsigned char *)var_s1_5 + (8));
            temp_t6 = *(s32 *)((unsigned char *)var_s1_5 + (12));
            temp_t7_2 = var_s1_5->unk0 - temp_v1_10;
            var_s1_5 += 0x10;
            *(s32 *)((unsigned char *)var_s1_5 + (-4)) = (s32) (temp_t6 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-8)) = (s32) (temp_t4_2 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-12)) = (s32) (temp_t3 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-16)) = temp_t7_2;
        } while (var_s1_5 != &D_801506F4);
    } else if (var_s0_7 <= 0) {
        temp_v0_4 = 1 - var_s0_7;
        D_801506BC += temp_v0_4;
        D_801506C0 += temp_v0_4;
        var_s1_6 = &D_801506C4;
        do {
            temp_t7_3 = *(s32 *)((unsigned char *)var_s1_6 + (4));
            temp_t9_2 = *(s32 *)((unsigned char *)var_s1_6 + (8));
            temp_t5 = *(s32 *)((unsigned char *)var_s1_6 + (12));
            temp_t8_2 = var_s1_6->unk0 + temp_v0_4;
            var_s1_6 += 0x10;
            *(s32 *)((unsigned char *)var_s1_6 + (-4)) = (s32) (temp_t5 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-8)) = (s32) (temp_t9_2 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-12)) = (s32) (temp_t7_3 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-16)) = temp_t8_2;
        } while (var_s1_6 != &D_801506F4);
    }
    var_a0_4 = D_80150668;
    var_s0_8 = &D_8015066A;
    var_s2_8 = 1;
    do {
        temp_a2_5 = *var_s0_8;
        if (var_a0_4 < temp_a2_5) {
            var_a0_4 = temp_a2_5;
            var_s3_2 = var_s2_8;
        }
        var_s2_8 += 1;
        var_s0_8 += 2;
    } while (var_s2_8 < 0xE);
    if (var_a0_4 >= 0x1D2) {
        temp_t0 = (&D_801506BC)[var_s3_2] - 1;
        D_801506BC -= temp_t0;
        D_801506C0 -= temp_t0;
        var_s1_7 = &D_801506C4;
        do {
            temp_t8_3 = *(s32 *)((unsigned char *)var_s1_7 + (4));
            temp_t3_2 = *(s32 *)((unsigned char *)var_s1_7 + (8));
            temp_t4_3 = *(s32 *)((unsigned char *)var_s1_7 + (12));
            temp_t6_2 = var_s1_7->unk0 - temp_t0;
            var_s1_7 += 0x10;
            *(s32 *)((unsigned char *)var_s1_7 + (-4)) = (s32) (temp_t4_3 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-8)) = (s32) (temp_t3_2 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-12)) = (s32) (temp_t8_3 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-16)) = temp_t6_2;
        } while (var_s1_7 != &D_801506F4);
    }
    func_800AE0B0("順位確定\n ");
    var_s1_8 = &D_801506BC;
    var_s2_9 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_9, *var_s1_8);
        var_s2_9 += 1;
        var_s1_8 += 4;
    } while (var_s2_9 < 0xE);
    var_s2_10 = 0;
    if ((D_80150684 == 0xD) && (D_80150688 == 0)) {
        temp_t8_4 = D_801506BC;
        if (temp_t8_4 < D_801506F0) {
            D_801506BC = D_801506F0;
            D_801506F0 = temp_t8_4;
        }
    }
    temp_a2_6 = D_801506F0;
    if ((D_80150684 == 0xD) && (temp_a2_6 <= 0)) {
        temp_a3 = 1 - temp_a2_6;
        D_801506BC += temp_a3;
        var_s1_9 = &D_801506C0;
        D_801506F0 = 1;
        do {
            temp_t6_3 = *(s32 *)((unsigned char *)var_s1_9 + (4));
            temp_t7_4 = *(s32 *)((unsigned char *)var_s1_9 + (8));
            temp_t9_3 = *(s32 *)((unsigned char *)var_s1_9 + (12));
            temp_t5_2 = var_s1_9->unk0 + temp_a3;
            var_s1_9 += 0x10;
            *(s32 *)((unsigned char *)var_s1_9 + (-4)) = (s32) (temp_t9_3 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-8)) = (s32) (temp_t7_4 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-12)) = (s32) (temp_t6_3 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-16)) = temp_t5_2;
        } while (var_s1_9 != &D_801506F0);
    }
    var_s0_9 = &D_80150668;
    do {
        temp_v1_11 = &D_800E6280 + var_s2_10;
        if (*var_s0_9 < D_80150682) {
            if (*(u8 *)((unsigned char *)temp_v1_11 + (1588)) == 0) {
                D_800E6280.unk_1BC[var_s2_10].unk_02 += 0xB;
                *(u8 *)((unsigned char *)temp_v1_11 + (1588)) = 1U;
            } else {
                D_800E6280.unk_1BC[var_s2_10].unk_02 += 2;
            }
        }
        var_s2_10 += 1;
        var_s0_9 += 2;
    } while (var_s2_10 < 0xB);
    D_801506F0 = temp_a2_6;
    if (temp_a2_6 == 1) {
        D_800E643E += 2;
        D_800E6476 += 2;
        D_800E64AE += 2;
        var_v0_3 = &D_800E6328;
        do {
            temp_t8_5 = *(s16 *)((unsigned char *)var_v0_3 + (502));
            temp_t3_3 = *(s16 *)((unsigned char *)var_v0_3 + (558));
            temp_t4_4 = *(s16 *)((unsigned char *)var_v0_3 + (614));
            temp_t6_4 = *(s16 *)((unsigned char *)var_v0_3 + (446)) + 2;
            var_v0_3 += 0xE0;
            *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t4_4 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t3_3 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t8_5 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (222)) = temp_t6_4;
        } while (var_v0_3 != &D_800E64E8);
    }
}
