void func_80139E88(void) {
    RpgStrTbl4 sp80;
    s32 sp74[1];
    RpgWordTbl4 sp64;
    s32 *var_s1_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    u32 temp_v0;
    u8 *temp_s0;

    sp80 = D_8015E530;
    sp64 = D_8015E540;
    var_fp = 0;
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
                sp64.v[0] = 1;
                break;
            case 4:
            case 5:
            case 6:
                sp64.v[1] = 1;
                break;
            case 7:
            case 8:
                sp64.v[2] = 1;
                break;
            case 9:
                sp64.v[3] = 1;
                break;
            }
            var_s0 += 1;
        } while (var_s0 != var_s1);
    }
    var_s2 = 0;
    var_s1_2 = sp64.v;
    do {
        temp_s0 = D_8015EB70 + (var_fp * 4);
        if (*var_s1_2 == 1) {
            func_800AE0F0(*(void **)((unsigned char *)temp_s0 + (12)), "おたから：");
            func_800AE100(*(void **)((unsigned char *)temp_s0 + (12)), sp80.s[var_s2]);
            var_fp += 1;
        }
        var_s1_2 += 1;
        var_s2 += 4;
    } while (var_s1_2 != &sp74);
    D_8015ED14 = var_fp + 3;
}
