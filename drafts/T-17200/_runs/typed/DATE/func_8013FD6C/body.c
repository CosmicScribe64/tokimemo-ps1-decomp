void func_8013FD6C(void) {
    DisplayActor44 *temp_s0_2;
    DisplayActor44 *temp_v0;
    GsWord *temp_v1;
    s16 temp_a0;
    s16 temp_t3;
    s16 temp_v0_4;
    s16 var_s1;
    s16 var_s1_2;
    s32 temp_a0_2;
    s32 temp_lo;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_5;
    s32 var_t1;
    s32 var_t2;
    u8 *temp_s0;

    D_8015BD34 += 1;
    if (((s32) D_8015BD34 % (s32) ((func_800AE0D0() % 60) + 0x3C)) == 0) {
        temp_t3 = D_8015BD38 + 1;
        if (D_8015BD38 < 4) {
            D_8015BD38 = temp_t3;
            temp_v0 = &((DisplayActor44 *)D_8011ECD0)[temp_t3];
            temp_v0[95].unk_16 = 3;
            temp_v0[95].unk_03 = (u8) (temp_v0[95].unk_03 | 0x80);
            temp_v0[95].unk_18 = 0;
            temp_v0[95].unk_08 = 0;
            temp_v0[95].unk_02 = 1;
            temp_v0[95].unk_2A = 0xA0;
            temp_v0_2 = func_800AE0D0();
            var_t1 = temp_v0_2 & (s32) &D_7FFF;
            if ((temp_v0_2 < 0) && (var_t1 != 0)) {
                var_t1 -= &D_7FFF + 1;
            }
            *(s32 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[D_8015BD38]) + (6504)) = (s32) (var_t1 - 0x4000);
            *(s32 *)((unsigned char *)(&D_8015BCB4[D_8015BD38]) + (-4)) = (s32) (((func_800AE0D0() % 9) * -0x10000) + 0xFFB80000);
            *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[D_8015BD38]) + (6498)) = (s16) ((func_800AE0D0() % 256) - 0x80);
        }
    }
    if ((u8) D_800E6280.unk_110A >= 0x26U) {
        D_8015BD38 = 1;
        var_s1 = 1;
        do {
            temp_lo = var_s1 * 0x44;
            var_s1 += 1;
            temp_s0 = (DisplayActor44 *)((u8 *)D_8011ECD0 + temp_lo);
            *(u8 *)((unsigned char *)temp_s0 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6531)) & 0x7F);
        } while (var_s1 < 4);
    }
    var_s1_2 = 0;
    if (D_8015BD38 > 0) {
        do {
            temp_v1 = &D_8015BCB4[var_s1_2];
            temp_s0_2 = &((DisplayActor44 *)D_8011ECD0)[var_s1_2];
            temp_a0 = temp_s0_2[96].unk_16;
            if (temp_a0 == 3) {
                temp_v0_3 = *(s32 *)((u8 *)&temp_s0_2[96] + 40);
                *(s32 *)((u8 *)&temp_s0_2[96] + 40) = (s32) (temp_v0_3 + ((s32) (temp_v1->w - temp_v0_3) >> 5));
                temp_a0_2 = temp_s0_2[96].unk_2A - (s16) temp_v1->h[1];
                *(s32 *)((u8 *)&temp_s0_2[96] + 36) = (s32) (*(s32 *)((u8 *)&temp_s0_2[96] + 36) + *(s32 *)((u8 *)&temp_s0_2[96] + 44));
                temp_s0_2[96].unk_07 = (s8) ((temp_a0_2 / 2) + 0x40);
                if (temp_a0_2 < 0xA) {
                    temp_s0_2[96].unk_16 = (s16) ((temp_s0_2[96].unk_16 - (func_800AE0D0() % 2)) - 1);
                    if ((D_800E6280.unk_110A == 0x25) && ((u32) D_800E6280.unk_1104.w >= 0x1F5U)) {
                        if (var_s1_2 < 2) {
                            temp_s0_2[96].unk_16 = 0;
                        }
                    }
                    temp_s0_2[96].unk_18 = 0;
                    temp_s0_2[96].unk_08 = 0;
                    temp_s0_2[96].unk_02 = 3;
                    temp_s0_2[96].unk_07 = 0x80;
                }
            } else {
                temp_v0_4 = temp_s0_2[96].unk_18;
                if ((temp_v0_4 == 0) && (temp_s0_2[96].unk_08 == 0)) {
                    if ((temp_s0_2[96].unk_03 & 0x80) && (temp_a0 != 0)) {
                        func_80044750(0x500);
                    }
                    if ((D_800E6280.unk_110A == 0x25) && ((u32) D_800E6280.unk_1104.w >= 0xC9U) && ((u32) D_800E6280.unk_1104.w < 0x1F5U)) {
                        temp_s0_2[96].unk_03 = (u8) (temp_s0_2[96].unk_03 & ~0x80);
                    } else if ((D_800E6280.unk_110A == 0x25) && ((u32) D_800E6280.unk_1104.w >= 0x2BDU)) {
                        temp_s0_2[96].unk_03 = (u8) (temp_s0_2[96].unk_03 & ~0x80);
                    } else if ((D_800E6280.unk_110A == 0x25) && ((u32) D_800E6280.unk_1104.w >= 0x1F5U) && (var_s1_2 >= 2)) {
                        temp_s0_2[96].unk_03 = (u8) (temp_s0_2[96].unk_03 & ~0x80);
                    } else {
                        temp_s0_2[96].unk_03 = (u8) (temp_s0_2[96].unk_03 | 0x80);
                        temp_s0_2[96].unk_16 = 3;
                        temp_s0_2[96].unk_18 = 0;
                        temp_s0_2[96].unk_08 = 0;
                        temp_s0_2[96].unk_02 = 1;
                        temp_s0_2[96].unk_2A = 0xA0;
                        temp_v0_5 = func_800AE0D0();
                        var_t2 = temp_v0_5 & (s32) &D_7FFF;
                        if ((temp_v0_5 < 0) && (var_t2 != 0)) {
                            var_t2 -= &D_7FFF + 1;
                        }
                        *(s32 *)((u8 *)&temp_s0_2[96] + 44) = (s32) (var_t2 - 0x4000);
                        D_8015BCB4[var_s1_2].w = ((func_800AE0D0() % 9) * -0x10000) + 0xFFD80000;
                        temp_s0_2[96].unk_26 = (s16) ((func_800AE0D0() % 256) - 0x80);
                        if ((D_800E6280.unk_110A == 0x25) && ((u32) D_800E6280.unk_1104.w >= 0x1F5U)) {
                            if (var_s1_2 == 0) {
                                temp_s0_2[96].unk_26 = -0x50;
                            } else if (var_s1_2 == 1) {
                                temp_s0_2[96].unk_26 = 0x50;
                            } else {
                                temp_s0_2[96].unk_03 = (u8) (temp_s0_2[96].unk_03 & 0x7F);
                            }
                        }
                    }
                } else if ((temp_a0 == 0) && (temp_v0_4 == 0xA) && (temp_s0_2[96].unk_08 == 0)) {
                    func_80044750(0x500);
                }
            }
            var_s1_2 += 1;
        } while (var_s1_2 < D_8015BD38);
    }
    if ((u8) D_8015BD3C < 0x80U) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 0xE, (s32) D_8015BD3C, 4);
    } else {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 0xE, 0xC0, 4);
    }
    dtd_on(0xE);
}
