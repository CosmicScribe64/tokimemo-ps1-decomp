void func_8013FD6C(void) {
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
    u8 *temp_s0_2;
    u8 *temp_v0;

    D_8015BD34 += 1;
    if (((s32) D_8015BD34 % (s32) ((func_800AE0D0() % 60) + 0x3C)) == 0) {
        temp_t3 = D_8015BD38 + 1;
        if (D_8015BD38 < 4) {
            D_8015BD38 = temp_t3;
            temp_v0 = D_8011ECD0 + (temp_t3 * 0x44);
            *(s16 *)((unsigned char *)temp_v0 + (6482)) = 3;
            *(u8 *)((unsigned char *)temp_v0 + (6463)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (6463)) | 0x80);
            *(s16 *)((unsigned char *)temp_v0 + (6484)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (6468)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (6462)) = 1;
            *(s16 *)((unsigned char *)temp_v0 + (6502)) = 0xA0;
            temp_v0_2 = func_800AE0D0();
            var_t1 = temp_v0_2 & 0x7FFF;
            if ((temp_v0_2 < 0) && (var_t1 != 0)) {
                var_t1 -= 0x8000;
            }
            *(s32 *)((unsigned char *)(D_8011ECD0 + (D_8015BD38 * 0x44)) + (6504)) = (s32) (var_t1 - 0x4000);
            *(s32 *)((unsigned char *)(&D_8015BCB4[D_8015BD38]) + (-4)) = (s32) (((func_800AE0D0() % 9) * -0x10000) + 0xFFB80000);
            *(s16 *)((unsigned char *)(D_8011ECD0 + (D_8015BD38 * 0x44)) + (6498)) = (s16) ((func_800AE0D0() % 256) - 0x80);
        }
    }
    if ((u8) D_800E738A >= 0x26U) {
        D_8015BD38 = 1;
        var_s1 = 1;
        do {
            temp_lo = var_s1 * 0x44;
            var_s1 += 1;
            temp_s0 = &D_8011ECD0[temp_lo];
            *(u8 *)((unsigned char *)temp_s0 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6531)) & 0x7F);
        } while (var_s1 < 4);
    }
    var_s1_2 = 0;
    if (D_8015BD38 > 0) {
        do {
            temp_v1 = &D_8015BCB4[var_s1_2];
            temp_s0_2 = D_8011ECD0 + (var_s1_2 * 0x44);
            temp_a0 = *(s16 *)((unsigned char *)temp_s0_2 + (6550));
            if (temp_a0 == 3) {
                temp_v0_3 = *(s32 *)((unsigned char *)temp_s0_2 + (6568));
                *(s32 *)((unsigned char *)temp_s0_2 + (6568)) = (s32) (temp_v0_3 + ((s32) (temp_v1->w - temp_v0_3) >> 5));
                temp_a0_2 = *(s16 *)((unsigned char *)temp_s0_2 + (6570)) - (s16) temp_v1->h[1];
                *(s32 *)((unsigned char *)temp_s0_2 + (6564)) = (s32) (*(s32 *)((unsigned char *)temp_s0_2 + (6564)) + *(s32 *)((unsigned char *)temp_s0_2 + (6572)));
                *(s8 *)((unsigned char *)temp_s0_2 + (6535)) = (s8) ((temp_a0_2 / 2) + 0x40);
                if (temp_a0_2 < 0xA) {
                    *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = (s16) ((*(s16 *)((unsigned char *)temp_s0_2 + (6550)) - (func_800AE0D0() % 2)) - 1);
                    if ((D_800E738A == 0x25) && ((u32) D_800E7384 >= 0x1F5U)) {
                        if (var_s1_2 < 2) {
                            *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = 0;
                        }
                    }
                    *(s16 *)((unsigned char *)temp_s0_2 + (6552)) = 0;
                    *(s16 *)((unsigned char *)temp_s0_2 + (6536)) = 0;
                    *(s8 *)((unsigned char *)temp_s0_2 + (6530)) = 3;
                    *(s8 *)((unsigned char *)temp_s0_2 + (6535)) = 0x80;
                }
            } else {
                temp_v0_4 = *(s16 *)((unsigned char *)temp_s0_2 + (6552));
                if ((temp_v0_4 == 0) && (*(s16 *)((unsigned char *)temp_s0_2 + (6536)) == 0)) {
                    if ((*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & 0x80) && (temp_a0 != 0)) {
                        func_80044750(0x500);
                    }
                    if ((D_800E738A == 0x25) && ((u32) D_800E7384 >= 0xC9U) && ((u32) D_800E7384 < 0x1F5U)) {
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80);
                    } else if ((D_800E738A == 0x25) && ((u32) D_800E7384 >= 0x2BDU)) {
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80);
                    } else if ((D_800E738A == 0x25) && ((u32) D_800E7384 >= 0x1F5U) && (var_s1_2 >= 2)) {
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80);
                    } else {
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) | 0x80);
                        *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = 3;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6552)) = 0;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6536)) = 0;
                        *(s8 *)((unsigned char *)temp_s0_2 + (6530)) = 1;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6570)) = 0xA0;
                        temp_v0_5 = func_800AE0D0();
                        var_t2 = temp_v0_5 & 0x7FFF;
                        if ((temp_v0_5 < 0) && (var_t2 != 0)) {
                            var_t2 -= 0x8000;
                        }
                        *(s32 *)((unsigned char *)temp_s0_2 + (6572)) = (s32) (var_t2 - 0x4000);
                        D_8015BCB4[var_s1_2].w = ((func_800AE0D0() % 9) * -0x10000) + 0xFFD80000;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = (s16) ((func_800AE0D0() % 256) - 0x80);
                        if ((D_800E738A == 0x25) && ((u32) D_800E7384 >= 0x1F5U)) {
                            if (var_s1_2 == 0) {
                                *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = -0x50;
                            } else if (var_s1_2 == 1) {
                                *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = 0x50;
                            } else {
                                *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & 0x7F);
                            }
                        }
                    }
                } else if ((temp_a0 == 0) && (temp_v0_4 == 0xA) && (*(s16 *)((unsigned char *)temp_s0_2 + (6536)) == 0)) {
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
