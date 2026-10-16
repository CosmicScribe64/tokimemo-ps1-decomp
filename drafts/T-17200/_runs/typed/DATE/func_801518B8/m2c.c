? func_80048514(s32, s32, ?);                       /* extern */

void func_801518B8(void) {
    s16 var_s0;

    var_s0 = 0;
    do {
        func_80048514(var_s0 + 5, *(D_8015ED70 + (var_s0 * 4)), 1);
        var_s0 += 1;
    } while (var_s0 < 6);
    func_80151460();
    func_80043914(D_8015ED80, 0x11, 1, 2, 0);
    func_80084E90(D_8015ED84, D_8015ED88, D_8015ED8C, D_8015ED90, D_8015ED94, D_8015ED98);
    func_800850D4(D_8015ED74, D_8015ED78, D_8015ED70, (s32) D_8015ED7C);
    M2C_MEMCPY_ALIGNED(D_8011ECD0 + 0x19C4, D_8011ECD0 + 0x1980, 0x44);
    *(s16 *)((unsigned char *)D_8011ECD0 + (6618)) = 3;
    D_80122D10 = 2;
    D_80122D0C = 1;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6552)) = 3;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
    func_8004284C();
}
