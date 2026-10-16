void func_8014A8FC(void) {
    s32 *var_v0;
    s16 temp_s0;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 *var_s1_4;
    s32 *var_s1_5;
    s32 *var_s1_6;
    s32 *var_s1_7;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_s0_7;
    s32 var_s0_8;
    s32 var_s1_8;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_v0;
    u8 temp_v0_2;

    message_window_show();
    func_80066C08(2);
    func_80066334();
    temp_s0 = D_8015DE4C;
    if (D_8015DE4C < 0x20) {
        if ((u8) D_8012276C < 0xC0U) {
            temp_t7 = D_8012276E + 6;
            D_8012276D = temp_t7;
            D_8012276C = temp_t7;
            temp_t8 = D_8012277E + 1;
            D_8012277D = temp_t8;
            D_8012277C = temp_t8;
            temp_v0 = D_8012278E + 1;
            D_8012278D = temp_v0;
            D_8012278C = temp_v0;
            D_8012276E = temp_t7;
            D_8012277E = temp_t8;
            D_8012278E = temp_v0;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (temp_s0 < 0xF) {
            D_800E62B8 += 1;
        }
        goto block_52;
    }
    if ((D_8015DE4C >= 0x20) && (D_8015DE4C < 0x54)) {
        var_s0 = 0;
        var_s1 = D_8015DE34;
        do {
            func_8014AD9C(var_s0, *var_s1, 1);
            var_s0 += 1;
            var_s1 += 4;
        } while (var_s0 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x54) && (D_8015DE4C < 0x68)) {
        var_s0_2 = 0;
        var_s1_2 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_2, *var_s1_2, 2);
            var_s0_2 += 1;
            var_s1_2 += 4;
        } while (var_s0_2 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x68) && (D_8015DE4C < 0x72)) {
        var_s0_3 = 0;
        var_s1_3 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_3, *var_s1_3, 3);
            var_s0_3 += 1;
            var_s1_3 += 4;
        } while (var_s0_3 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x72) && (D_8015DE4C < 0x7C)) {
        var_s0_4 = 0;
        var_s1_4 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_4, *var_s1_4, 4);
            var_s0_4 += 1;
            var_s1_4 += 4;
        } while (var_s0_4 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x7C) && (D_8015DE4C < 0x9A)) {
        var_s0_5 = 0;
        var_s1_5 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_5, *var_s1_5, 5);
            var_s0_5 += 1;
            var_s1_5 += 4;
        } while (var_s0_5 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x9A) && (D_8015DE4C < 0xAE)) {
        var_s0_6 = 0;
        var_s1_6 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_6, *var_s1_6, 6);
            var_s0_6 += 1;
            var_s1_6 += 4;
        } while (var_s0_6 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0xAE) && (D_8015DE4C < 0xEA)) {
        var_s0_7 = 0;
        var_s1_7 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_7, *var_s1_7, 7);
            var_s0_7 += 1;
            var_s1_7 += 4;
        } while (var_s0_7 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x10E) && (D_8015DE4C < 0x12E)) {
        if (D_8012276C != 0) {
            temp_t3 = D_8012276E - 6;
            D_8012276D = temp_t3;
            D_8012276C = temp_t3;
            temp_t4 = D_8012277E - 1;
            D_8012277D = temp_t4;
            D_8012277C = temp_t4;
            temp_v0_2 = D_8012278E - 1;
            D_8012278D = temp_v0_2;
            D_8012278C = temp_v0_2;
            D_8012276E = temp_t3;
            D_8012277E = temp_t4;
            D_8012278E = temp_v0_2;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (temp_s0 < 0x11D) {
            D_800E62B8 -= 1;
        }
        if (D_8012276C == 0) {
            var_v0 = &D_80122670;
            *D_80122660 = 0x80000000;
            do {
                var_v0 += 0x40;
                *(s32 *)((unsigned char *)var_v0 + (-48)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-32)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-64)) = 0x80000000;
            } while (var_v0 != &D_801226F0);
        }
        goto block_52;
    }
    var_s0_8 = 2;
    if (D_8015DE4C >= 0x138) {
        func_8014B1C4();
block_52:
        var_s0_8 = 2;
    }
    var_s1_8 = 9;
    do {
        func_8014AD9C(var_s0_8, D_8015DE3C, var_s1_8);
        var_s0_8 += 1;
        var_s1_8 += 1;
    } while (var_s0_8 != 0xB);
    D_8015DE4C += 1;
}
