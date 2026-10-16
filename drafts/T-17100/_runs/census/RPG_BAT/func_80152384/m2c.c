s32 func_800AE100();
extern s32 sp;
void func_80152384(void) {
    s32 sp64[16];
    s32 spA0[4];
    s32 sp90[4];
    s32 sp80[4];
    s32 sp74[1];
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    u8 *temp_s0;

    (*(s32 *)&spA0) = (s32) D_8015EFA0;
    *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (4));
    *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (12));
    *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (8));
    (*(s32 *)&sp90) = (s32) D_8015EFB0;
    *(s32 *)((unsigned char *)(&sp90) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (4));
    *(s32 *)((unsigned char *)(&sp90) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (12));
    *(s32 *)((unsigned char *)(&sp90) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (8));
    (*(s32 *)&sp80) = (s32) D_8015EFC0;
    *(s32 *)((unsigned char *)(&sp80) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (4));
    *(s32 *)((unsigned char *)(&sp80) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (12));
    *(s32 *)((unsigned char *)(&sp80) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (8));
    sp64[0] = D_8015EFD0;
    var_s3 = 0;
    *(s32 *)((unsigned char *)(&sp64[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (4));
    *(s32 *)((unsigned char *)(&sp64[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (8));
    *(s32 *)((unsigned char *)(&sp64[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (12));
    var_s1 = 2;
    if (func_8013E9A8(5) == 0) {
        var_s1 = 3;
    }
    var_s0 = 0;
    if (var_s1 > 0) {
        do {
            var_s0 += 1;
            (&sp64[0])[func_8013E9A8(4)] = 1;
        } while (var_s0 != var_s1);
    }
    var_s1_2 = 0;
    var_s2 = &sp64[0];
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
        var_s2 += 4;
        var_s1_2 += 4;
    } while (var_s2 != &sp74);
    D_8015ED14 = var_s3 + 4;
}
