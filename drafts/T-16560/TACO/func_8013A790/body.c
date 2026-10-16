void func_8013A790(s32 arg0, s32 arg1) {
    s32 sp44;
    s32 *temp_v0_2;
    s32 *temp_v0_4;
    s32 *var_v0;
    s32 *var_v0_4;
    s32 *var_v0_8;
    s32 temp_a0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 var_a2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s2;
    s32 var_s4;
    s32 var_v1;
    s32 var_v1_2;
    u8 temp_v0;
    u8 *temp_v0_3;
    u8 *temp_v0_5;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_5;
    u8 *var_v0_6;
    u8 *var_v0_7;

    var_s4 = 0x7530;
    var_a2 = 0;
    if (arg1 == 0) {
        if (D_800E6280.unk_F80 & 1) {
            var_s4 = 0x2710;
        } else if (D_800E6280.unk_F80 & 2) {
            var_s4 = 0xC350;
        }
        if (D_800E6280.unk_F80 & 4) {
            var_a2 = 0x28;
        }
        if (D_800E6280.unk_F80 & 8) {
            var_a2 = -0x28;
        }
    } else if (arg1 == 1) {
        temp_v0 = D_8015EDB4->unk2;
        if ((temp_v0 & 1) == 1) {
            var_a2 = 0x28;
        }
        if ((temp_v0 & 2) == 2) {
            var_a2 = -0x28;
        }
    } else if (arg1 == 2) {
        var_a2 = 0x28;
        var_s4 = 0x4E20;
    } else if (arg1 == 3) {
        var_a2 = 0x28;
        var_s4 = 0x2710;
    }
    var_s2 = 0;
    if (var_a2 > 0) {
        var_s0 = 0;
        do {
            temp_v0_2 = D_8015F4EC + var_s0;
            *temp_v0_2 += var_a2;
            var_v0 = D_8015F4EC + var_s0;
            temp_v1 = *var_v0;
            if (temp_v1 >= 0x1001) {
                *var_v0 = temp_v1 - 0x1000;
                var_v0 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0 + (8)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (8)) + var_a2);
            var_v0_2 = D_8015F4EC + var_s0;
            temp_v1_2 = *(s32 *)((unsigned char *)var_v0_2 + (8));
            if (temp_v1_2 >= 0x1001) {
                *(s32 *)((unsigned char *)var_v0_2 + (8)) = (s32) (temp_v1_2 - 0x1000);
                var_v0_2 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0_2 + (16)) = (s32) (*(s32 *)((unsigned char *)var_v0_2 + (16)) + var_a2);
            var_v0_3 = D_8015F4EC + var_s0;
            temp_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (16));
            if (temp_v1_3 >= 0x1001) {
                *(s32 *)((unsigned char *)var_v0_3 + (16)) = (s32) (temp_v1_3 - 0x1000);
                var_v0_3 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0_3 + (24)) = (s32) (*(s32 *)((unsigned char *)var_v0_3 + (24)) + var_a2);
            temp_v0_3 = D_8015F4EC + var_s0;
            temp_v1_4 = *(s32 *)((unsigned char *)temp_v0_3 + (24));
            var_s0 += 0x20;
            if (temp_v1_4 >= 0x1001) {
                *(s32 *)((unsigned char *)temp_v0_3 + (24)) = (s32) (temp_v1_4 - 0x1000);
            }
        } while (var_s0 != 0x320);
    }
    if (var_a2 < 0) {
        var_s0_2 = 0;
        do {
            temp_v0_4 = D_8015F4EC + var_s0_2;
            *temp_v0_4 += var_a2;
            var_v0_4 = D_8015F4EC + var_s0_2;
            temp_v1_5 = *var_v0_4;
            if (temp_v1_5 < 0) {
                *var_v0_4 = temp_v1_5 + 0x1000;
                var_v0_4 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_4 + (8)) = (s32) (*(s32 *)((unsigned char *)var_v0_4 + (8)) + var_a2);
            var_v0_5 = D_8015F4EC + var_s0_2;
            temp_v1_6 = *(s32 *)((unsigned char *)var_v0_5 + (8));
            if (temp_v1_6 < 0) {
                *(s32 *)((unsigned char *)var_v0_5 + (8)) = (s32) (temp_v1_6 + 0x1000);
                var_v0_5 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_5 + (16)) = (s32) (*(s32 *)((unsigned char *)var_v0_5 + (16)) + var_a2);
            var_v0_6 = D_8015F4EC + var_s0_2;
            temp_v1_7 = *(s32 *)((unsigned char *)var_v0_6 + (16));
            if (temp_v1_7 < 0) {
                *(s32 *)((unsigned char *)var_v0_6 + (16)) = (s32) (temp_v1_7 + 0x1000);
                var_v0_6 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_6 + (24)) = (s32) (*(s32 *)((unsigned char *)var_v0_6 + (24)) + var_a2);
            temp_v0_5 = D_8015F4EC + var_s0_2;
            temp_v1_8 = *(s32 *)((unsigned char *)temp_v0_5 + (24));
            var_s0_2 += 0x20;
            if (temp_v1_8 < 0) {
                *(s32 *)((unsigned char *)temp_v0_5 + (24)) = (s32) (temp_v1_8 + 0x1000);
            }
        } while (var_s0_2 != 0x320);
    }
    var_s0_3 = 0;
    do {
        temp_a0 = var_s2 + 1;
        var_v0_7 = D_8015F4EC + var_s0_3;
        var_v1 = *(s32 *)((unsigned char *)var_v0_7 + (4));
        if (var_v1 == 0) {
            sp44 = temp_a0;
            if ((func_800AE0D0() % temp_a0) == 0) {
                var_s2 = temp_a0;
                *(D_8015F4EC + var_s0_3) = func_800AE0D0() % 4096;
                if (arg0 == 0) {
                    *(s32 *)((unsigned char *)(D_8015F4EC + var_s0_3) + (4)) = 0x3A980;
                } else {
                    *(s32 *)((unsigned char *)(D_8015F4EC + var_s0_3) + (4)) = 0x3E8;
                }
            }
            var_v0_7 = D_8015F4EC + var_s0_3;
            var_v1 = *(s32 *)((unsigned char *)var_v0_7 + (4));
        } else {
            var_s2 = temp_a0;
        }
        if (arg0 == 0) {
            *(s32 *)((unsigned char *)var_v0_7 + (4)) = (s32) (var_v1 - ((s32) (var_v1 * 0x3E8) / var_s4));
            var_v0_8 = D_8015F4EC + var_s0_3;
            var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            if (var_v1_2 < 0xBB9) {
                *(s32 *)((unsigned char *)var_v0_8 + (4)) = 0;
                var_v0_8 = D_8015F4EC + var_s0_3;
                var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            }
        } else {
            *(s32 *)((unsigned char *)var_v0_7 + (4)) = (s32) (var_v1 + ((s32) (var_v1 * 0x3E8) / var_s4));
            var_v0_8 = D_8015F4EC + var_s0_3;
            var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            if (var_v1_2 >= 0x3A980) {
                *(s32 *)((unsigned char *)var_v0_8 + (4)) = 0;
                var_v0_8 = D_8015F4EC + var_s0_3;
                var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            }
        }
        if (var_v1_2 != 0) {
            func_8013AD64(*var_v0_8, var_v1_2 / 1000, -1, arg1);
        }
        var_s0_3 += 8;
    } while (var_s0_3 != 0x320);
}
