extern ? D_80160450;
extern s16 *D_80160454;

void func_80152944(s32 arg0, s32 arg1) {
    s16 **var_s0;
    s16 **var_s0_2;
    s16 **var_s0_3;
    s16 **var_s0_4;
    s16 **var_s0_5;
    s16 **var_s0_6;
    s16 **var_s0_7;
    s16 **var_s0_8;
    s16 **var_s0_9;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;
    s32 var_s1_6;
    s32 var_s1_7;
    s32 var_s1_8;
    s32 var_s1_9;

    if (arg1 < 8) {
        var_s0 = &D_80160444;
        var_s1 = arg0;
        do {
            func_80147674(var_s1, *(s16 **)((unsigned char *)var_s0 + (4)), 1, 0, 0);
            var_s0 += 4;
            var_s1 += 1;
        } while (var_s0 != &D_80160454);
        return;
    }
    if (arg1 >= 8) {
        var_s1_2 = arg0;
        if (arg1 < 0x10) {
            var_s0_2 = &D_80160444;
            do {
                func_80147674(var_s1_2, *(s16 **)((unsigned char *)var_s0_2 + (4)), 2, 0, 0);
                var_s0_2 += 4;
                var_s1_2 += 1;
            } while (var_s0_2 != &D_80160454);
            return;
        }
    }
    if (arg1 >= 0x10) {
        var_s1_3 = arg0;
        if (arg1 < 0x18) {
            var_s0_3 = &D_80160444;
            do {
                func_80147674(var_s1_3, *(s16 **)((unsigned char *)var_s0_3 + (4)), 3, 0, 0);
                var_s0_3 += 4;
                var_s1_3 += 1;
            } while (var_s0_3 != &D_80160454);
            return;
        }
    }
    if (arg1 >= 0x18) {
        var_s1_4 = arg0;
        if (arg1 < 0x20) {
            var_s0_4 = &D_80160444;
            do {
                func_80147674(var_s1_4, *(s16 **)((unsigned char *)var_s0_4 + (4)), 4, 0, 0);
                var_s0_4 += 4;
                var_s1_4 += 1;
            } while (var_s0_4 != &D_80160454);
            return;
        }
    }
    if (arg1 >= 0x20) {
        var_s1_5 = arg0;
        if (arg1 < 0x28) {
            var_s0_5 = &D_80160444;
            do {
                func_80147674(var_s1_5, *(s16 **)((unsigned char *)var_s0_5 + (4)), 5, 0, 0);
                var_s0_5 += 4;
                var_s1_5 += 1;
            } while (var_s0_5 != &D_80160454);
            return;
        }
    }
    if (arg1 >= 0x28) {
        var_s1_6 = arg0;
        if (arg1 < 0x2E) {
            var_s0_6 = &D_80160444;
            do {
                func_80147674(var_s1_6, *(s16 **)((unsigned char *)var_s0_6 + (4)), 6, 0, 0);
                var_s0_6 += 4;
                var_s1_6 += 1;
            } while (var_s0_6 != &D_80160454);
            return;
        }
    }
    if (arg1 >= 0x2E) {
        var_s1_7 = arg0;
        if (arg1 < 0x36) {
            var_s0_7 = &D_80160444;
            do {
                func_80147674(var_s1_7, *(s16 **)((unsigned char *)var_s0_7 + (4)), 7, 0, 0);
                var_s0_7 += 4;
                var_s1_7 += 1;
            } while (var_s0_7 != &D_80160450);
            return;
        }
    }
    if (arg1 >= 0x36) {
        var_s1_8 = arg0;
        if (arg1 < 0x3E) {
            var_s0_8 = &D_80160444;
            do {
                func_80147674(var_s1_8, *(s16 **)((unsigned char *)var_s0_8 + (4)), 8, 0, 0);
                var_s0_8 += 4;
                var_s1_8 += 1;
            } while (var_s0_8 != &D_80160450);
            func_80147674(0x20, D_80160454, 7, 0, 0);
            return;
        }
    }
    if (arg1 >= 0x3E) {
        var_s1_9 = arg0;
        if (arg1 < 0x46) {
            var_s0_9 = &D_80160444;
            do {
                func_80147674(var_s1_9, *(s16 **)((unsigned char *)var_s0_9 + (4)), 9, 0, 0);
                var_s0_9 += 4;
                var_s1_9 += 1;
            } while (var_s0_9 != &D_80160450);
            func_80147674(0x20, D_80160454, 8, 0, 0);
        }
    }
}
