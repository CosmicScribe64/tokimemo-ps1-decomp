void func_80159FAC(void) {
    typedef struct { s32 w[46]; } M2cBlockB8; /* m2c memcpy of 184 bytes */
    s32 sp2C[46];

    *(M2cBlockB8 *)(&sp2C) = *(M2cBlockB8 *)(&D_80160940);
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0) && (((u8) D_800E738A < 0xDU) || ((u8) D_800E738A >= 0x21U))) {
        func_8006B900();
    }
    (*(s32 (**)(s32))((unsigned char *)(sp + (D_800E738A * 4)) + (44)))(0x80);
    if ((D_80122D04 != 0) && (((u32) D_800E7378 % 150U) == 0)) {
        func_8015A170();
    }
}
