s32 func_8014CB24(s32 arg0) {
    s32 sp52;
    s32 sp60[8];
    s16 sp5C;
    s16 sp5A;
    s16 sp58;
    s16 sp50;
    s16 **var_s1;
    s16 **var_s1_2;
    s16 **var_s1_3;
    s16 **var_s1_4;
    s16 **var_s1_5;
    s16 **var_s1_6;
    s16 **var_s1_7;
    s16 **var_s1_8;
    s16 **var_s2;
    s16 **var_s2_2;
    s16 **var_s2_3;
    s16 **var_s2_4;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s1_4;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_10;
    s32 var_s0_11;
    s32 var_s0_12;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_s0_7;
    s32 var_s0_8;
    s32 var_s0_9;

    if (arg0 < 0xA) {
        var_s2 = D_801600C0;
        var_s0 = 0;
        do {
            temp_s1 = var_s0 + 1;
            func_80147674(temp_s1, *var_s2, 1, 0, 0);
            var_s0 = temp_s1;
            var_s2 += 4;
        } while (temp_s1 < 5);
        var_s1 = D_801600D4;
        var_s0_2 = 0;
        do {
            func_80147674(var_s0_2 + 0x19, *var_s1, 1, 0, 0);
            var_s0_2 += 1;
            var_s1 += 4;
        } while (var_s0_2 < 3);
        var_s1_2 = D_801600E0;
        var_s0_3 = 0;
        do {
            func_80147674(var_s0_3 + 0x1D, *var_s1_2, 1, 0, 0);
            var_s0_3 += 1;
            var_s1_2 += 4;
        } while (var_s0_3 < 2);
        func_80147674(8, D_801600E8, 1, 0, 0);
        var_s1_3 = D_801600EC;
        var_s0_4 = 0;
        do {
            func_80147674(var_s0_4 + 0x15, *var_s1_3, 1, 0, 0);
            var_s0_4 += 1;
            var_s1_3 += 4;
        } while (var_s0_4 != 2);
    } else if ((arg0 >= 0xE) && (var_s0_5 = 0, ((arg0 < 0x18) != 0))) {
        var_s2_2 = D_801600C0;
        do {
            temp_s1_2 = var_s0_5 + 1;
            func_80147674(temp_s1_2, *var_s2_2, 2, 0, 0);
            var_s0_5 = temp_s1_2;
            var_s2_2 += 4;
        } while (temp_s1_2 < 5);
        var_s1_4 = D_801600D4;
        var_s0_6 = 0;
        do {
            func_80147674(var_s0_6 + 0x19, *var_s1_4, 2, 0, 0);
            var_s0_6 += 1;
            var_s1_4 += 4;
        } while (var_s0_6 < 3);
        var_s1_5 = D_801600E0;
        var_s0_7 = 0;
        do {
            func_80147674(var_s0_7 + 0x1D, *var_s1_5, 2, 0, 0);
            var_s0_7 += 1;
            var_s1_5 += 4;
        } while (var_s0_7 != 2);
        func_80147674(8, D_801600E8, 2, 0, 0);
    } else if (arg0 >= 0x1E) {
        if (arg0 < 0x28) {
            func_80147674(0x1A, D_801600D8, 3, 0, 0);
            func_80147674(8, D_801600E8, 3, 0, 0);
        } else {
            goto block_23;
        }
    } else {
block_23:
        if (arg0 >= 0x2C) {
            var_at = arg0 < 0x36;
            if (arg0 < 0x36) {
                func_80147674(0x1A, D_801600D8, 4, 0, 0);
                func_80147674(8, D_801600E8, 4, 0, 0);
            } else {
                goto block_27;
            }
        } else {
            var_at = arg0 < 0x36;
block_27:
            if ((var_at == 0) && (var_s0_8 = 0, ((arg0 < 0x40) != 0))) {
                var_s2_3 = D_801600C0;
                do {
                    temp_s1_3 = var_s0_8 + 1;
                    func_80147674(temp_s1_3, *var_s2_3, 3, 0, 0);
                    var_s0_8 = temp_s1_3;
                    var_s2_3 += 4;
                } while (temp_s1_3 < 5);
                func_80147674(0x19, *D_801600D4, 3, 0, 0);
                func_80147674(0x1A, D_801600D8, 5, 0, 0);
                func_80147674(0x1B, D_801600DC, 3, 0, 0);
                var_s1_6 = D_801600E0;
                var_s0_9 = 0;
                do {
                    func_80147674(var_s0_9 + 0x1D, *var_s1_6, 3, 0, 0);
                    var_s0_9 += 1;
                    var_s1_6 += 4;
                } while (var_s0_9 != 2);
                func_80147674(8, D_801600E8, 5, 0, 0);
            } else if (arg0 >= 0x40) {
                var_s0_10 = 0;
                if (arg0 < 0x4A) {
                    var_s2_4 = D_801600C0;
                    do {
                        temp_s1_4 = var_s0_10 + 1;
                        func_80147674(temp_s1_4, *var_s2_4, 4, 0, 0);
                        var_s0_10 = temp_s1_4;
                        var_s2_4 += 4;
                    } while (temp_s1_4 < 5);
                    func_80147674(0x19, *D_801600D4, 4, 0, 0);
                    func_80147674(0x1A, D_801600D8, 6, 0, 0);
                    func_80147674(0x1B, D_801600DC, 4, 0, 0);
                    var_s1_7 = D_801600E0;
                    var_s0_11 = 0;
                    do {
                        func_80147674(var_s0_11 + 0x1D, *var_s1_7, 4, 0, 0);
                        var_s0_11 += 1;
                        var_s1_7 += 4;
                    } while (var_s0_11 < 2);
                    func_80147674(8, D_801600E8, 6, 0, 0);
                    var_s1_8 = D_801600EC;
                    var_s0_12 = 0;
                    do {
                        func_80147674(var_s0_12 + 0x15, *var_s1_8, 2, 0, 0);
                        var_s0_12 += 1;
                        var_s1_8 += 4;
                    } while (var_s0_12 != 2);
                }
            }
        }
    }
    if ((arg0 >= 0x28) && (arg0 < 0x2D)) {
        (*(s32 *)&sp60) = (s32) (*(s32 *)D_80125C60);
        *(s32 *)((unsigned char *)(&sp60) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
        *(s32 *)((unsigned char *)(&sp60) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
        *(s32 *)((unsigned char *)(&sp60) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
        *(s32 *)((unsigned char *)(&sp60) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
        *(s32 *)((unsigned char *)(&sp60) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
        *(s32 *)((unsigned char *)(&sp60) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
        *(s32 *)((unsigned char *)(&sp60) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
        func_8015D0D0((s32) (D_80128880->x << 0xC) / 360, (s32) &sp60);
        func_8015CF30((s32) (D_80128884 << 0xC) / 360, (s32) &sp60);
        func_8015D270((s32) (D_80128882 << 0xC) / 360, (s32) &sp60);
        sp58 = (s16) D_80127718;
        sp5A = (s16) D_8012771C;
        sp5C = (s16) D_80127720;
        func_800A0ECC(&sp60, &sp58, &sp50);
        *(s8 *)((unsigned char *)D_8015EDB4 + (2721)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2836)) = (s16) (D_80127498 + sp50);
        *(s16 *)((unsigned char *)D_8015EDB4 + (2838)) = (s16) (D_8012749C + sp52);
        *(s16 *)((unsigned char *)D_8015EDB4 + (2840)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2852)) = 0x28;
        return;
    }
    if (arg0 == 0x2D) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (2721)) = 0x80;
    }
}
