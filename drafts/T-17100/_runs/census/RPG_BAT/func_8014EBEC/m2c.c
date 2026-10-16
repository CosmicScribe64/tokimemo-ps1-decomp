extern s32 D_8011F770;
extern s32 D_8011FDD0;
void func_8014EBEC(void) {
    s32 *var_v1;
    s32 (*var_v0_3)[4];
    s32 *var_v0;
    s32 *var_v0_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s0;
    s32 var_s0_2;

    D_8015EDA4 = 0;
    D_8015ED98 = 0;
    *(s32 *)((unsigned char *)(&D_8015ED98) + (4)) = 0;
    D_8015EDAC = 0;
    D_8015EDB4 = 0;
    *(s32 *)((unsigned char *)(&D_8015EDB4) + (-4)) = 0;
    *(s32 *)((unsigned char *)(&D_8015EDB4) + (4)) = 0;
    var_v0 = &D_8015EC74;
    do {
        var_v0 += 4;
        *(s32 *)((unsigned char *)var_v0 + (-4)) = 0;
    } while ((u32) var_v0 < (u32) D_8015ED3C);
    var_v0_2 = D_8015ED3C;
    do {
        var_v0_2 += 4;
        *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
    } while ((u32) var_v0_2 < (u32) D_8015ED64);
    var_v0_3 = D_8015ED64;
    do {
        var_v0_3 += 4;
        *(s32 *)((unsigned char *)var_v0_3 + (-4)) = 0;
    } while ((u32) var_v0_3 < (u32) &D_8015ED94);
    var_s0 = 0x28;
    if (D_8015EB98 != 0x80000000) {
        var_a0 = 0x28;
        do {
            func_8013E7C0(var_a0, 0xFF, 1, 0);
            var_s0 += 1;
            var_a0 = var_s0;
        } while (var_s0 != 0x30);
    }
    var_s0_2 = 0x32;
    var_a0_2 = 0x32;
    do {
        func_8013E7C0(var_a0_2, 0xFF, 1, 0);
        var_s0_2 += 1;
        var_a0_2 = var_s0_2;
    } while (var_s0_2 < 0x38);
    var_v1 = &D_8011F770;
    do {
        var_v1 += 0x110;
        *(s8 *)((unsigned char *)var_v1 + (6257)) = 5;
        *(s8 *)((unsigned char *)var_v1 + (6325)) = 5;
        *(s8 *)((unsigned char *)var_v1 + (6393)) = 5;
        *(s8 *)((unsigned char *)var_v1 + (6461)) = 5;
    } while (var_v1 != &D_8011FDD0);
    if (D_8015EB98 != 0x80000000) {
        D_80120E05 = 5;
    }
}
