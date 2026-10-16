void func_80150460(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    D_8015E208 = D_8015DE90;
    D_8015E20C = D_8015DFCC;
    D_8015E210 = D_8015E108;
    D_800E64AE += 2;
    D_800E64B6 -= 0xA;
    check_para_limit();
    func_80150080();
    load_palette(D_8015EA6C, 0x11, 1, 2, 0);
    func_80084E90((s32) D_8015EA70, (s32) D_8015EA74, (s32) D_8015EA78, (s32) D_8015EA7C, (s32) D_8015EA80, (s32) D_8015EA84);
    func_800850D4((s32) D_8015EA60, (s32) D_8015EA64, (s32) D_8015EA5C, (s32) D_8015EA68);
    *(M2cBlock44 *)(D_801206D8) = *(M2cBlock44 *)(D_80120650);
    D_800CA360 = 1;
    func_8004284C();
}
