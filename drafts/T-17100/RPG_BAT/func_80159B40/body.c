s32 func_80159B40(void) {
    RpgStrTbl4 sp88;
    RpgStrTbl4 sp78;
    s32 sp5C[4];
    s32 *var_s0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s7;
    u32 temp_v0;
    u8 *var_s1_2;

    sp88 = D_8015EFE0;
    sp78 = D_8015EFF0;
    sp5C[0] = D_8015F000;
    *(s32 *)((unsigned char *)(&sp5C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015F000) + (4));
    var_s7 = 0;
    *(s32 *)((unsigned char *)(&sp5C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015F000) + (12));
    *(s32 *)((unsigned char *)(&sp5C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015F000) + (8));
    var_s1 = 2;
    if (func_8013E9A8(5) == 0) {
        var_s1 = 3;
    }
    var_s0 = 0;
    if (var_s1 > 0) {
        do {
            temp_v0 = func_8013E9A8(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
            case 3:
                sp5C[0] = 1;
                break;
            case 4:
            case 5:
            case 6:
                sp5C[1] = 1;
                break;
            case 7:
            case 8:
                sp5C[2] = 1;
                break;
            case 9:
                sp5C[3] = 1;
                break;
            }
            var_s0 += 1;
        } while (var_s0 != var_s1);
    }
    var_s1_2 = (0 * 4) + D_8015EB70;
    var_s2 = 0;
    var_s0_2 = sp5C;
    do {
        if (*var_s0_2 == 1) {
            func_800AE0F0(*(void **)((unsigned char *)var_s1_2 + (12)), "おたから：");
            func_800AE100(*(void **)((unsigned char *)var_s1_2 + (12)), sp88.s[var_s2 / 4]);
            var_s7 += 1;
            var_s1_2 += 4;
        }
        var_s0_2 += 1;
        var_s2 += 4;
    } while (var_s0_2 != &sp5C[4]);
    func_800AE0F0(*(void **)((unsigned char *)var_s1_2 + (12)), "獲得称号：");
    func_800AE100(*(void **)((unsigned char *)var_s1_2 + (12)), sp78.s[func_8013E9A8(4)]);
    D_8015ED14 = var_s7 + 4;
}
