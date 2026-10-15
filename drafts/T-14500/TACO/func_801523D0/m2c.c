void func_801523D0(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp4C;
    s16 **var_s2;
    s16 **var_s2_2;
    s16 **var_s2_3;
    s16 **var_s2_4;
    s16 **var_s2_5;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;

    if (arg2 < 8) {
        var_s2 = &D_801603F0[arg1];
        var_s0 = 0;
        var_s1 = arg0;
        do {
            func_80147674(var_s1, *var_s2, 3, 0, 0);
            var_s0 += 3;
            var_s1 += 3;
            var_s2 += 4;
        } while (var_s0 != 0x15);
        return;
    }
    if (arg2 >= 8) {
        var_s0_2 = 0;
        if (arg2 < 0x10) {
            var_s1_2 = arg0;
            var_s2_2 = &D_801603F0[arg1];
            do {
                func_80147674(var_s1_2, *var_s2_2, 4, 0, 0);
                var_s0_2 += 3;
                var_s1_2 += 3;
                var_s2_2 += 4;
            } while (var_s0_2 != 0x15);
            return;
        }
    }
    if (arg2 >= 0x10) {
        var_s0_3 = 0;
        if (arg2 < 0x1A) {
            var_s1_3 = arg0;
            var_s2_3 = &D_801603F0[arg1];
            do {
                func_80147674(var_s1_3, *var_s2_3, 5, 0, 0);
                var_s0_3 += 3;
                var_s1_3 += 3;
                var_s2_3 += 4;
            } while (var_s0_3 != 0x15);
            return;
        }
    }
    if (arg2 == 0x1D) {
        func_801335A0(0, 0x501);
        return;
    }
    if ((arg2 >= 0x1E) && (var_s0_4 = 0, ((arg2 < 0x23) != 0))) {
        var_s1_4 = arg0;
        var_s2_4 = &D_801603F0[arg1];
        do {
            func_80147674(var_s1_4, *var_s2_4, 6, 0, 0);
            var_s0_4 += 3;
            var_s1_4 += 3;
            var_s2_4 += 4;
        } while (var_s0_4 != 0x15);
        if (arg0 == 1) {
            sp4C = 0x10;
        } else if (arg0 == 2) {
            sp4C = 0x11;
        } else if (arg0 == 3) {
            sp4C = 0x12;
        }
        if (D_8015EDB4[sp4C].unk84[1] != 0) {
            func_80147C98(0, arg0, 0, 0x64, 0x3E8, 0xC8, 0x83);
        }
    } else if (arg2 >= 0x23) {
        var_s0_5 = 0;
        if (arg2 < 0x2D) {
            var_s1_5 = arg0;
            var_s2_5 = &D_801603F0[arg1];
            do {
                func_80147674(var_s1_5, *var_s2_5, 7, 0, 0);
                var_s0_5 += 3;
                var_s1_5 += 3;
                var_s2_5 += 4;
            } while (var_s0_5 != 0x15);
        }
    }
}
