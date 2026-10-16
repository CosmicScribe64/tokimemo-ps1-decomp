s32 func_80145DC4(void) {
    s32 *var_s2;
    s32 *var_s2_2;
    s32 *var_s2_3;
    s32 *var_s2_4;
    s32 *var_s2_5;
    s32 *var_s2_6;
    s32 *var_s2_7;
    s32 *var_s2_8;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_6;
    s32 temp_s0_7;
    s32 temp_s0_8;
    s32 temp_s4;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;
    s32 var_s1_6;
    s32 var_s1_7;
    s32 var_s1_8;

    D_8012274C = 0;
    D_80122750 = D_801274EC;
    temp_s4 = D_800E7384;
    D_80122754 = -0x7D0;
    func_80099540(&D_80122740);
    switch (D_800E738D) {
    case 0:
        func_801449D8((s32) &D_800E78BC, 0, 0xA);
        func_801449D8((s32) &D_800E7ABC, 0, 8);
        var_s1 = 0;
        if (temp_s4 < 0x1E) {
            var_s2 = &D_8015F82C;
            do {
                temp_s0 = var_s1 + 1;
                func_80147488(temp_s0, *var_s2, 1);
                var_s1 = temp_s0;
                var_s2 += 4;
            } while (temp_s0 != 2);
        } else if ((temp_s4 >= 0x1E) && (var_s1_2 = 0, ((temp_s4 < 0x5A) != 0))) {
            var_s2_2 = &D_8015F82C;
            do {
                temp_s0_2 = var_s1_2 + 1;
                func_80147488(temp_s0_2, *var_s2_2, 2);
                var_s1_2 = temp_s0_2;
                var_s2_2 += 4;
            } while (temp_s0_2 != 2);
        } else if ((temp_s4 >= 0x5A) && (var_s1_3 = 0, ((temp_s4 < 0x96) != 0))) {
            var_s2_3 = &D_8015F82C;
            do {
                temp_s0_3 = var_s1_3 + 1;
                func_80147488(temp_s0_3, *var_s2_3, 3);
                var_s1_3 = temp_s0_3;
                var_s2_3 += 4;
            } while (temp_s0_3 != 2);
        } else if ((temp_s4 >= 0x96) && (var_s1_4 = 0, ((temp_s4 < 0xB4) != 0))) {
            var_s2_4 = &D_8015F82C;
            do {
                temp_s0_4 = var_s1_4 + 1;
                func_80147488(temp_s0_4, *var_s2_4, 4);
                var_s1_4 = temp_s0_4;
                var_s2_4 += 4;
            } while (temp_s0_4 != 2);
        } else if ((temp_s4 >= 0xB4) && (var_s1_5 = 0, ((temp_s4 < 0xD2) != 0))) {
            var_s2_5 = &D_8015F82C;
            do {
                temp_s0_5 = var_s1_5 + 1;
                func_80147488(temp_s0_5, *var_s2_5, 5);
                var_s1_5 = temp_s0_5;
                var_s2_5 += 4;
            } while (temp_s0_5 != 2);
        } else if ((temp_s4 >= 0xD2) && (var_s1_6 = 0, ((temp_s4 < 0x10E) != 0))) {
            var_s2_6 = &D_8015F82C;
            do {
                temp_s0_6 = var_s1_6 + 1;
                func_80147488(temp_s0_6, *var_s2_6, 6);
                var_s1_6 = temp_s0_6;
                var_s2_6 += 4;
            } while (temp_s0_6 != 2);
        } else if ((temp_s4 >= 0x10E) && (var_s1_7 = 0, ((temp_s4 < 0x14A) != 0))) {
            var_s2_7 = &D_8015F82C;
            do {
                temp_s0_7 = var_s1_7 + 1;
                func_80147488(temp_s0_7, *var_s2_7, 7);
                var_s1_7 = temp_s0_7;
                var_s2_7 += 4;
            } while (temp_s0_7 != 2);
        } else if ((temp_s4 >= 0x14A) && (var_s1_8 = 0, ((temp_s4 < 0x168) != 0))) {
            var_s2_8 = &D_8015F82C;
            do {
                temp_s0_8 = var_s1_8 + 1;
                func_80147488(temp_s0_8, *var_s2_8, 8);
                var_s1_8 = temp_s0_8;
                var_s2_8 += 4;
            } while (temp_s0_8 != 2);
        } else if (temp_s4 == 0x168) {
            D_800E7384 = 0;
            D_800E738D += 1;
        }
        if (temp_s4 == 0x1E) {
            func_801335A0(0, 0x505);
        }
        if (temp_s4 == 0xAA) {
            func_801335A0(0, 0x507);
        }
        if (temp_s4 == 0x12C) {
            func_801335A0(0, 0x506);
        }
        break;
    case 1:
        if (temp_s4 == 1) {
            D_80127090.unk0 = 0x80000000;
            D_801270A0.unk0 = 0x80000000;
        } else if (temp_s4 >= 0x3C) {
            func_8004284C();
        }
        break;
    }
    D_800E7384 += 1;
}
