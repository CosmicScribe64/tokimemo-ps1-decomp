void func_80152384(void) {
    RpgWordTbl4 sp64;
    RpgStrTbl4 spA0;
    RpgStrTbl4 sp90;
    RpgStrTbl4 sp80;
    s32 sp74[1];
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    u8 *temp_s0;

    spA0 = D_8015EFA0;
    sp90 = D_8015EFB0;
    sp80 = D_8015EFC0;
    sp64 = D_8015EFD0;
    var_s3 = 0;
    var_s1 = 2;
    if (func_8013E9A8(5) == 0) {
        var_s1 = 3;
    }
    var_s0 = 0;
    if (var_s1 > 0) {
        do {
            var_s0 += 1;
            sp64.v[func_8013E9A8(4)] = 1;
        } while (var_s0 != var_s1);
    }
    var_s1_2 = 0;
    var_s2 = sp64.v;
    do {
        temp_s0 = D_8015EB70 + (var_s3 * 4);
        if (*var_s2 == 1) {
            func_800AE0F0(*(void **)((unsigned char *)temp_s0 + (16)), "おたから：");
            switch (D_8015EB98) {
            case 0x10000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), *(s32 *)((unsigned char *)(sp + var_s1_2) + (160)));
                break;
            case 0x20000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), *(s32 *)((unsigned char *)(sp + var_s1_2) + (144)));
                break;
            case 0x40000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), *(s32 *)((unsigned char *)(sp + var_s1_2) + (128)));
                break;
            }
            var_s3 += 1;
        }
        var_s2 += 1;
        var_s1_2 += 4;
    } while (var_s2 != &sp74);
    D_8015ED14 = var_s3 + 4;
}
