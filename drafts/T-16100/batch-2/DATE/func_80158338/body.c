void func_80158338(void) {
    typedef struct { s32 w[63]; } M2cBlockFC; /* m2c memcpy of 252 bytes */
    s32 sp28[63];

    *(M2cBlockFC *)(&sp28) = *(M2cBlockFC *)(&D_8016020C);
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0)) {
        func_8006B900();
    }
    (*(s32 (**)(s32))((unsigned char *)(sp + (D_800E738A * 4)) + (40)))(0x80);
}
