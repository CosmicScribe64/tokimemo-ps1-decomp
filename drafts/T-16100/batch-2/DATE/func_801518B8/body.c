void func_801518B8(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s16 var_s0;

    var_s0 = 0;
    do {
        load_tpage_buf_lock(var_s0 + 5, *(s32 *)(D_8015ED70 + (var_s0 * 4)), 1);
        var_s0 += 1;
    } while (var_s0 < 6);
    func_80151460();
    load_palette(D_8015ED80, 0x11, 1, 2, 0);
    func_80084E90(D_8015ED84, D_8015ED88, D_8015ED8C, D_8015ED90, D_8015ED94, D_8015ED98);
    func_800850D4(D_8015ED74, D_8015ED78, D_8015ED70, (s32) D_8015ED7C);
    *(M2cBlock44 *)(D_80120694) = *(M2cBlock44 *)(D_80120650);
    D_801206AA = 3;
    D_80122D10 = 2;
    D_80122D0C = 1;
    D_80120668 = 3;
    D_801206AC = 3;
    func_8004284C();
}
