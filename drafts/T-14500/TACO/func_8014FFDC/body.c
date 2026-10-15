s32 func_8014FFDC(s32 arg0) {
    s16 **var_s0;
    s16 **var_s0_2;
    s16 **var_s0_3;
    s16 **var_s0_4;
    s16 **var_s0_5;
    s16 **var_s0_6;
    s32 *var_s2;
    s32 *var_s2_10;
    s32 *var_s2_2;
    s32 *var_s2_3;
    s32 *var_s2_4;
    s32 *var_s2_5;
    s32 *var_s2_6;
    s32 *var_s2_7;
    s32 *var_s2_8;
    s32 *var_s2_9;
    s32 temp_s0;
    s32 temp_s0_10;
    s32 temp_s0_11;
    s32 temp_s0_12;
    s32 temp_s0_13;
    s32 temp_s0_14;
    s32 temp_s0_15;
    s32 temp_s0_16;
    s32 temp_s0_17;
    s32 temp_s0_18;
    s32 temp_s0_19;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_6;
    s32 temp_s0_7;
    s32 temp_s0_8;
    s32 temp_s0_9;
    s32 var_s1;
    s32 var_s1_10;
    s32 var_s1_11;
    s32 var_s1_12;
    s32 var_s1_13;
    s32 var_s1_14;
    s32 var_s1_15;
    s32 var_s1_16;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;
    s32 var_s1_6;
    s32 var_s1_7;
    s32 var_s1_8;
    s32 var_s1_9;

    if (arg0 < 0xA) {
        var_s2 = D_8016024C;
        var_s1 = 0;
        do {
            temp_s0 = var_s1 + 1;
            func_80147674(temp_s0, (s16 *) *var_s2, 1, 0, 0);
            var_s1 = temp_s0;
            var_s2 += 4;
        } while (temp_s0 != 6);
        func_80147674(0x1A, (s16 *) D_80160264, 1, 0, 0);
        return;
    }
    if (arg0 >= 0xE) {
        var_s1_2 = 0;
        if (arg0 < 0x18) {
            var_s2_2 = D_8016024C;
            do {
                temp_s0_2 = var_s1_2 + 1;
                func_80147674(temp_s0_2, (s16 *) *var_s2_2, 2, 0, 0);
                var_s1_2 = temp_s0_2;
                var_s2_2 += 4;
            } while (temp_s0_2 != 4);
            return;
        }
    }
    if (arg0 == 0x1B) {
        temp_s0_3 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_3 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        temp_s0_4 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_4 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        temp_s0_5 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_5 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        return;
    }
    if (arg0 >= 0x1C) {
        var_s1_3 = 0;
        if (arg0 < 0x1E) {
            var_s2_3 = D_8016024C;
            do {
                temp_s0_6 = var_s1_3 + 1;
                func_80147674(temp_s0_6, (s16 *) *var_s2_3, 3, 0, 0);
                var_s1_3 = temp_s0_6;
                var_s2_3 += 4;
            } while (temp_s0_6 < 4);
            var_s0 = &D_8016025C;
            var_s1_4 = 0;
            do {
                func_80147674(var_s1_4 + 5, *var_s0, 2, 0, 0);
                var_s1_4 += 1;
                var_s0 += 4;
            } while (var_s1_4 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x1E) {
        var_s1_5 = 0;
        if (arg0 < 0x22) {
            var_s2_4 = D_8016024C;
            do {
                temp_s0_7 = var_s1_5 + 1;
                func_80147674(temp_s0_7, (s16 *) *var_s2_4, 4, 0, 0);
                var_s1_5 = temp_s0_7;
                var_s2_4 += 4;
            } while (temp_s0_7 < 4);
            var_s0_2 = &D_8016025C;
            var_s1_6 = 0;
            do {
                func_80147674(var_s1_6 + 5, *var_s0_2, 3, 0, 0);
                var_s1_6 += 1;
                var_s0_2 += 4;
            } while (var_s1_6 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 3, 0, 0);
            return;
        }
    }
    if (arg0 == 0x22) {
        temp_s0_8 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_8 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        temp_s0_9 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_9 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x12C, 0xC1);
        temp_s0_10 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_10 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x1F4, 0xC1);
        return;
    }
    if (arg0 >= 0x23) {
        var_s1_7 = 0;
        if (arg0 < 0x25) {
            var_s2_5 = D_8016024C;
            do {
                temp_s0_11 = var_s1_7 + 1;
                func_80147674(temp_s0_11, (s16 *) *var_s2_5, 3, 0, 0);
                var_s1_7 = temp_s0_11;
                var_s2_5 += 4;
            } while (temp_s0_11 < 4);
            var_s0_3 = &D_8016025C;
            var_s1_8 = 0;
            do {
                func_80147674(var_s1_8 + 5, *var_s0_3, 2, 0, 0);
                var_s1_8 += 1;
                var_s0_3 += 4;
            } while (var_s1_8 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x25) {
        var_s1_9 = 0;
        if (arg0 < 0x29) {
            var_s2_6 = D_8016024C;
            do {
                temp_s0_12 = var_s1_9 + 1;
                func_80147674(temp_s0_12, (s16 *) *var_s2_6, 4, 0, 0);
                var_s1_9 = temp_s0_12;
                var_s2_6 += 4;
            } while (temp_s0_12 < 4);
            var_s0_4 = &D_8016025C;
            var_s1_10 = 0;
            do {
                func_80147674(var_s1_10 + 5, *var_s0_4, 3, 0, 0);
                var_s1_10 += 1;
                var_s0_4 += 4;
            } while (var_s1_10 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 3, 0, 0);
            return;
        }
    }
    if (arg0 == 0x29) {
        temp_s0_13 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_13 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        temp_s0_14 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_14 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x190, 0xC1);
        temp_s0_15 = func_800AE0D0();
        func_80147C98(0, 5, (s16) ((temp_s0_15 & 0x7F) - 0x40), (s16) ((func_800AE0D0() & 0x3F) - 0x20), 0x3E8, 0x12C, 0xC1);
        return;
    }
    if (arg0 >= 0x2A) {
        var_s1_11 = 0;
        if (arg0 < 0x2C) {
            var_s2_7 = D_8016024C;
            do {
                temp_s0_16 = var_s1_11 + 1;
                func_80147674(temp_s0_16, (s16 *) *var_s2_7, 3, 0, 0);
                var_s1_11 = temp_s0_16;
                var_s2_7 += 4;
            } while (temp_s0_16 < 4);
            var_s0_5 = &D_8016025C;
            var_s1_12 = 0;
            do {
                func_80147674(var_s1_12 + 5, *var_s0_5, 2, 0, 0);
                var_s1_12 += 1;
                var_s0_5 += 4;
            } while (var_s1_12 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 2, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x2C) {
        var_s1_13 = 0;
        if (arg0 < 0x31) {
            var_s2_8 = D_8016024C;
            do {
                temp_s0_17 = var_s1_13 + 1;
                func_80147674(temp_s0_17, (s16 *) *var_s2_8, 4, 0, 0);
                var_s1_13 = temp_s0_17;
                var_s2_8 += 4;
            } while (temp_s0_17 < 4);
            var_s0_6 = &D_8016025C;
            var_s1_14 = 0;
            do {
                func_80147674(var_s1_14 + 5, *var_s0_6, 3, 0, 0);
                var_s1_14 += 1;
                var_s0_6 += 4;
            } while (var_s1_14 != 2);
            func_80147674(0x1A, (s16 *) D_80160264, 3, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x35) {
        var_s1_15 = 0;
        if (arg0 < 0x3F) {
            var_s2_9 = D_8016024C;
            do {
                temp_s0_18 = var_s1_15 + 1;
                func_80147674(temp_s0_18, (s16 *) *var_s2_9, 5, 0, 0);
                var_s1_15 = temp_s0_18;
                var_s2_9 += 4;
            } while (temp_s0_18 != 4);
            func_80147674(6, D_80160260, 4, 0, 0);
            func_80147674(0x1A, (s16 *) D_80160264, 4, 0, 0);
            return;
        }
    }
    if (arg0 >= 0x43) {
        var_s1_16 = 0;
        if (arg0 < 0x4D) {
            var_s2_10 = D_8016024C;
            do {
                temp_s0_19 = var_s1_16 + 1;
                func_80147674(temp_s0_19, (s16 *) *var_s2_10, 6, 0, 0);
                var_s1_16 = temp_s0_19;
                var_s2_10 += 4;
            } while (temp_s0_19 != 4);
            func_80147674(5, D_8016025C, 4, 0, 0);
            func_80147674(6, D_80160260, 5, 0, 0);
            func_80147674(0x1A, (s16 *) D_80160264, 5, 0, 0);
        }
    }
}
