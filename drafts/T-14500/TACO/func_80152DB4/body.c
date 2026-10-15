s32 func_80152DB4(s32 arg0, s32 arg1, s32 arg2) {
    s16 **var_s2;
    s16 **var_s2_2;
    s16 **var_s2_3;
    s16 **var_s2_4;
    s16 **var_s2_5;
    s16 **var_s2_6;
    s32 temp_s0;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;
    s32 var_s1_6;

    if (arg2 < 0xA) {
        var_s2 = &D_801603F0[arg1];
        var_s0 = 0;
        var_s1 = arg0;
        do {
            func_80147674(var_s1, *var_s2, 8, 0, 0);
            var_s0 += 3;
            var_s1 += 3;
            var_s2 += 4;
        } while (var_s0 != 0x15);
        return;
    }
    if (arg2 >= 0xA) {
        var_s0_2 = 0;
        if (arg2 < 0x14) {
            var_s2_2 = &D_801603F0[arg1];
            var_s1_2 = arg0;
            do {
                func_80147674(var_s1_2, *var_s2_2, 9, 0, 0);
                var_s0_2 += 3;
                var_s1_2 += 3;
                var_s2_2 += 4;
            } while (var_s0_2 != 0x15);
            return;
        }
    }
    if (arg2 >= 0x14) {
        var_s0_3 = 0;
        if (arg2 < 0x1E) {
            var_s2_3 = &D_801603F0[arg1];
            var_s1_3 = arg0;
            do {
                func_80147674(var_s1_3, *var_s2_3, 0xA, 0, 0);
                var_s0_3 += 3;
                var_s1_3 += 3;
                var_s2_3 += 4;
            } while (var_s0_3 != 0x15);
            return;
        }
    }
    if (arg2 >= 0x1E) {
        var_at = arg2 < 0x50;
        if (arg2 < 0x50) {
            temp_s0 = func_800AE0D0();
            func_80147C98(0, 0x1C, (s16) ((temp_s0 & 0x3FF) - 0x200), (s16) ((func_800AE0D0() & 0x3FF) - 0x200), 0x3E8, 0x1F4, 0x86);
            return;
        }
        goto block_18;
    }
    var_at = arg2 < 0x50;
block_18:
    if (var_at == 0) {
        var_s0_4 = 0;
        if (arg2 < 0x5A) {
            var_s2_4 = &D_801603F0[arg1];
            var_s1_4 = arg0;
            do {
                func_80147674(var_s1_4, *var_s2_4, 0xB, 0, 0);
                var_s0_4 += 3;
                var_s1_4 += 3;
                var_s2_4 += 4;
            } while (var_s0_4 != 0x15);
            return;
        }
    }
    if (arg2 >= 0x5A) {
        var_s0_5 = 0;
        if (arg2 < 0x64) {
            var_s2_5 = &D_801603F0[arg1];
            var_s1_5 = arg0;
            do {
                func_80147674(var_s1_5, *var_s2_5, 0xC, 0, 0);
                var_s0_5 += 3;
                var_s1_5 += 3;
                var_s2_5 += 4;
            } while (var_s0_5 != 0x15);
            return;
        }
    }
    if (arg2 >= 0x64) {
        var_s0_6 = 0;
        if (arg2 < 0x6E) {
            var_s2_6 = &D_801603F0[arg1];
            var_s1_6 = arg0;
            do {
                func_80147674(var_s1_6, *var_s2_6, 0xD, 0, 0);
                var_s0_6 += 3;
                var_s1_6 += 3;
                var_s2_6 += 4;
            } while (var_s0_6 != 0x15);
        }
    }
}
