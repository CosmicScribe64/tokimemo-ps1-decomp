void func_8011B848(void) {
    s16 temp_t3;
    s16 var_s1;
    s16 var_s1_2;
    s32 *temp_v1;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_t1;
    s32 var_t7;
    u8 *temp_s0;
    u8 temp_a1;
    u8 var_t1_2;
    u8 *temp_s0_2;
    u8 *temp_v0;

    D_8012585C += 1;
    if (((s32) D_8012585C % (s32) ((func_800789A0() % 60) + 0x3C)) == 0) {
        temp_t3 = D_80125860 + 1;
        if (D_80125860 < 4) {
            D_80125860 = temp_t3;
            temp_v0 = D_800E9620 + (temp_t3 * 0x44);
            *(s16 *)((unsigned char *)temp_v0 + (6482)) = 3;
            *(u8 *)((unsigned char *)temp_v0 + (6463)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (6463)) | 0x80);
            *(s16 *)((unsigned char *)temp_v0 + (6484)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (6468)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (6462)) = 1;
            *(s16 *)((unsigned char *)temp_v0 + (6502)) = 0xA0;
            temp_v0_2 = func_800789A0();
            var_t1 = temp_v0_2 & 0x7FFF;
            if ((temp_v0_2 < 0) && (var_t1 != 0)) {
                var_t1 -= 0x8000;
            }
            *(s32 *)((unsigned char *)(D_800E9620 + (D_80125860 * 0x44)) + (6504)) = (s32) (var_t1 - 0x4000);
            *(s32 *)((unsigned char *)(&D_801257DC[D_80125860]) + (-4)) = (s32) (((func_800789A0() % 9) * -0x10000) + 0xFFB80000);
            *(s16 *)((unsigned char *)(D_800E9620 + (D_80125860 * 0x44)) + (6498)) = (s16) ((func_800789A0() % 256) - 0x80);
        }
    }
    temp_a1 = D_800B1AF6;
    if (temp_a1 >= 0x26U) {
        D_80125860 = 1;
        var_s1 = 1;
        do {
            temp_lo = var_s1 * 0x44;
            var_s1 += 1;
            temp_s0 = &D_800E9620[temp_lo];
            *(u8 *)((unsigned char *)temp_s0 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6531)) & 0x7F);
        } while (var_s1 < 4);
    }
    var_s1_2 = 0;
    if (D_80125860 > 0) {
        temp_a2 = D_800B1AF0;
        do {
            temp_v1 = &D_801257DC[var_s1_2];
            temp_s0_2 = D_800E9620 + (var_s1_2 * 0x44);
            if (*(s16 *)((unsigned char *)temp_s0_2 + (6550)) == 3) {
                temp_v0_3 = *(s32 *)((unsigned char *)temp_s0_2 + (6568));
                *(s32 *)((unsigned char *)temp_s0_2 + (6568)) = (s32) (temp_v0_3 + ((s32) ((*(s32 *)temp_v1) - temp_v0_3) >> 5));
                temp_a0 = *(s16 *)((unsigned char *)temp_s0_2 + (6570)) - *(s16 *)((unsigned char *)temp_v1 + (2));
                *(s32 *)((unsigned char *)temp_s0_2 + (6564)) = (s32) (*(s32 *)((unsigned char *)temp_s0_2 + (6564)) + *(s32 *)((unsigned char *)temp_s0_2 + (6572)));
                *(s8 *)((unsigned char *)temp_s0_2 + (6535)) = (s8) ((temp_a0 / 2) + 0x40);
                if (temp_a0 < 0xA) {
                    *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = (s16) ((*(s16 *)((unsigned char *)temp_s0_2 + (6550)) - (func_800789A0(temp_a0, temp_a1, (u32) temp_a2) % 2)) - 1);
                    if ((D_800B1AF6 == 0x25) && ((u32) D_800B1AF0 >= 0x1F5U)) {
                        if (var_s1_2 < 2) {
                            *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = 0;
                        }
                    }
                    *(s16 *)((unsigned char *)temp_s0_2 + (6552)) = 0;
                    *(s16 *)((unsigned char *)temp_s0_2 + (6536)) = 0;
                    *(s8 *)((unsigned char *)temp_s0_2 + (6530)) = 3;
                    *(s8 *)((unsigned char *)temp_s0_2 + (6535)) = 0x80;
                }
            } else if ((*(s16 *)((unsigned char *)temp_s0_2 + (6552)) == 0) && (*(s16 *)((unsigned char *)temp_s0_2 + (6536)) == 0)) {
                if ((temp_a1 == 0x25) && ((u32) temp_a2 >= 0xC9U) && ((u32) temp_a2 < 0x1F5U)) {
                    *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80);
                } else {
                    if ((temp_a1 == 0x25) && ((u32) temp_a2 >= 0x2BDU)) {
                        var_t1_2 = *(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80;
                        goto block_42;
                    }
                    if ((temp_a1 == 0x25) && ((u32) temp_a2 >= 0x1F5U) && (var_s1_2 >= 2)) {
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) & ~0x80);
                    } else {
                        func_800433D0(0x500);
                        *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_s0_2 + (6531)) | 0x80);
                        *(s16 *)((unsigned char *)temp_s0_2 + (6550)) = 3;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6552)) = 0;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6536)) = 0;
                        *(s8 *)((unsigned char *)temp_s0_2 + (6530)) = 1;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6570)) = 0xA0;
                        temp_v0_4 = func_800789A0();
                        var_t7 = temp_v0_4 & 0x7FFF;
                        if ((temp_v0_4 < 0) && (var_t7 != 0)) {
                            var_t7 -= 0x8000;
                        }
                        *(s32 *)((unsigned char *)temp_s0_2 + (6572)) = (s32) (var_t7 - 0x4000);
                        D_801257DC[var_s1_2] = ((func_800789A0() % 9) * -0x10000) + 0xFFD80000;
                        *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = (s16) ((func_800789A0() % 256) - 0x80);
                        if ((D_800B1AF6 == 0x25) && ((u32) D_800B1AF0 >= 0x1F5U)) {
                            if (var_s1_2 == 0) {
                                *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = -0x50;
                            } else if (var_s1_2 == 1) {
                                *(s16 *)((unsigned char *)temp_s0_2 + (6566)) = 0x50;
                            } else {
                                var_t1_2 = *(u8 *)((unsigned char *)temp_s0_2 + (6531)) & 0x7F;
block_42:
                                *(u8 *)((unsigned char *)temp_s0_2 + (6531)) = var_t1_2;
                            }
                        }
                    }
                }
            }
            var_s1_2 += 1;
        } while (var_s1_2 < D_80125860);
    }
    if ((u8) D_80125864 < 0x80U) {
        func_8001A650(-0xA0, -0x78, 0x140, 0xA0, 0xE, (s32) D_80125864, 4);
    } else {
        func_8001A650(-0xA0, -0x78, 0x140, 0xA0, 0xE, 0xC0, 4);
    }
    func_8001A16C(0xE);
}
