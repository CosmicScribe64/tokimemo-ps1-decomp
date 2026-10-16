void func_8014F278(void) {
    s32 *var_v1_4;
    s32 (*var_v1_3)[4];
    s32 *var_v1;
    s32 *var_v1_2;

    D_8015EDFC = 0;
    *(s32 *)((unsigned char *)(&D_8015EDFC) + (-12)) = 0;
    *(s32 *)((unsigned char *)(&D_8015EDFC) + (-8)) = 0;
    D_8015EE04 = 0;
    *(s32 *)((unsigned char *)(&D_8015EE04) + (8)) = 0;
    *(s32 *)((unsigned char *)(&D_8015EE04) + (4)) = 0;
    D_8015EE10 = 0;
    var_v1 = &D_8015EC74;
    do {
        var_v1 += 1;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
    } while ((u32) var_v1 < (u32) D_8015ED3C);
    var_v1_2 = D_8015ED3C;
    do {
        var_v1_2 += 1;
        *(s32 *)((unsigned char *)var_v1_2 + (-4)) = 0;
    } while ((u32) var_v1_2 < (u32) D_8015ED64);
    var_v1_3 = D_8015ED64;
    do {
        var_v1_3 += 4;
        *(s32 *)((unsigned char *)var_v1_3 + (-4)) = 0;
    } while ((u32) var_v1_3 < (u32) &D_8015ED94);
    var_v1_4 = &D_8011F770;
    do {
        var_v1_4 += 68;
        *(s8 *)((unsigned char *)var_v1_4 + (6257)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6325)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6393)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6461)) = 5;
    } while (var_v1_4 != &D_8011FDD0);
    if (D_8015EB98 != 0x80000000) {
        D_80120E05 = 5;
    }
}
