void func_8014B8B4(s32 arg0, s32 arg1, s32 arg2) {
    s32 *var_s2;
    s32 *var_s2_2;
    s32 *var_s2_3;
    s32 *var_s2_4;
    s32 *var_s2_5;
    s32 *var_s2_6;
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

    if (arg2 < 4) {
        var_s2 = &D_8016001C[arg1];
        var_s0 = 0;
        var_s1 = arg0;
        do {
            func_80147674(var_s1, (s16 *) *var_s2, 1, 0, 0);
            var_s0 += 1;
            var_s1 += 1;
            var_s2 += 4;
        } while (var_s0 != 3);
    } else if ((arg2 >= 4) && (var_s0_2 = 0, ((arg2 < 8) != 0))) {
        var_s2_2 = &D_8016001C[arg1];
        var_s1_2 = arg0;
        do {
            func_80147674(var_s1_2, (s16 *) *var_s2_2, 2, 0, 0);
            var_s0_2 += 1;
            var_s1_2 += 1;
            var_s2_2 += 4;
        } while (var_s0_2 != 3);
    } else if ((arg2 >= 0xC) && (var_s0_3 = 0, ((arg2 < 0x16) != 0))) {
        var_s2_3 = &D_8016001C[arg1];
        var_s1_3 = arg0;
        do {
            func_80147674(var_s1_3, (s16 *) *var_s2_3, 3, 0, 0);
            var_s0_3 += 1;
            var_s1_3 += 1;
            var_s2_3 += 4;
        } while (var_s0_3 != 3);
    } else if ((arg2 >= 0x18) && (var_s0_4 = 0, ((arg2 < 0x1C) != 0))) {
        var_s2_4 = &D_8016001C[arg1];
        var_s1_4 = arg0;
        do {
            func_80147674(var_s1_4, (s16 *) *var_s2_4, 1, 0, 0);
            var_s0_4 += 1;
            var_s1_4 += 1;
            var_s2_4 += 4;
        } while (var_s0_4 != 3);
    } else if ((arg2 >= 0x20) && (var_s0_5 = 0, ((arg2 < 0x2A) != 0))) {
        var_s2_5 = &D_8016001C[arg1];
        var_s1_5 = arg0;
        do {
            func_80147674(var_s1_5, (s16 *) *var_s2_5, 4, 0, 0);
            var_s0_5 += 1;
            var_s1_5 += 1;
            var_s2_5 += 4;
        } while (var_s0_5 != 3);
    } else if (arg2 >= 0x2E) {
        var_s0_6 = 0;
        if (arg2 < 0x32) {
            var_s2_6 = &D_8016001C[arg1];
            var_s1_6 = arg0;
            do {
                func_80147674(var_s1_6, (s16 *) *var_s2_6, 2, 0, 0);
                var_s0_6 += 1;
                var_s1_6 += 1;
                var_s2_6 += 4;
            } while (var_s0_6 != 3);
        }
    }
    if ((arg2 >= 0x14) && (arg2 < 0x1E)) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (2449)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2568)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2585)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2704)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2716)) = 0x14;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2721)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2840)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2852)) = 0x14;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2857)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2976)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2988)) = 0x14;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3129)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (3248)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3260)) = 0x14;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3265)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (3384)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3396)) = 0x14;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3401)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (3520)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3532)) = 0x14;
        return;
    }
    if (arg2 == 0x1E) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (2449)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2585)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2721)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2857)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3129)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3265)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (3401)) = 0x80;
    }
}
