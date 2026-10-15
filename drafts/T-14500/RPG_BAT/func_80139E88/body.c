void func_80139E88(void) {
    s32 sp80[4];
    s32 sp74[1];
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s32 *var_s1_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    u32 temp_v0;
    u8 *temp_s0;

    (*(s32 *)&sp80) = (s32) D_8015E530;
    *(s32 *)((unsigned char *)(&sp80) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015E530) + (4));
    *(s32 *)((unsigned char *)(&sp80) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015E530) + (12));
    *(s32 *)((unsigned char *)(&sp80) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015E530) + (8));
    sp64 = D_8015E540;
    *(s32 *)((unsigned char *)(&sp64) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015E540) + (4));
    var_fp = 0;
    *(s32 *)((unsigned char *)(&sp64) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015E540) + (12));
    *(s32 *)((unsigned char *)(&sp64) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015E540) + (8));
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
                sp64 = 1;
                break;
            case 4:
            case 5:
            case 6:
                sp68 = 1;
                break;
            case 7:
            case 8:
                sp6C = 1;
                break;
            case 9:
                sp70 = 1;
                break;
            }
            var_s0 += 1;
        } while (var_s0 != var_s1);
    }
    var_s2 = 0;
    var_s1_2 = &sp64;
    do {
        temp_s0 = D_8015EB70 + (var_fp * 4);
        if (*var_s1_2 == 1) {
            func_800AE0F0(*(void **)((unsigned char *)temp_s0 + (12)), "おたから：");
            func_800AE100(*(void **)((unsigned char *)temp_s0 + (12)), *(var_s2 + &sp80));
            var_fp += 1;
        }
        var_s1_2 += 4;
        var_s2 += 4;
    } while (var_s1_2 != &sp74);
    D_8015ED14 = var_fp + 3;
}
