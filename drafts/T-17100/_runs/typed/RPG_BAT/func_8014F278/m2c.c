extern ? D_8015EDFC;

void func_8014F278(void) {
    DisplayActor44 *var_v1_4;
    s32 (*var_v1_3)[4];
    s32 *var_v1;
    s32 *var_v1_2;

    D_8015EDFC.unk0 = 0;
    *(s32 *)((unsigned char *)(&D_8015EDFC) + (-12)) = 0;
    *(s32 *)((unsigned char *)(&D_8015EDFC) + (-8)) = 0;
    D_8015EE00.unk_04 = 0;
    D_8015EE00.unk_0C = 0;
    D_8015EE00.unk_08 = 0;
    D_8015EE10 = 0;
    var_v1 = &D_8015EC74;
    do {
        var_v1 += 4;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
    } while ((u32) var_v1 < (u32) D_8015ED3C);
    var_v1_2 = D_8015ED3C;
    do {
        var_v1_2 += 4;
        *(s32 *)((unsigned char *)var_v1_2 + (-4)) = 0;
    } while ((u32) var_v1_2 < (u32) D_8015ED64);
    var_v1_3 = D_8015ED64;
    do {
        var_v1_3 += 4;
        *(s32 *)((unsigned char *)var_v1_3 + (-4)) = 0;
    } while ((u32) var_v1_3 < (u32) &D_8015ED94);
    var_v1_4 = D_8011ECD0 + 0xAA0;
    do {
        var_v1_4 += 0x110;
        *(s8 *)((unsigned char *)var_v1_4 + (6257)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6325)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6393)) = 5;
        *(s8 *)((unsigned char *)var_v1_4 + (6461)) = 5;
    } while (var_v1_4 != (D_8011ECD0 + 0x1100));
    if (D_8015EB98 != 0x80000000) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (8501)) = 5;
    }
}
