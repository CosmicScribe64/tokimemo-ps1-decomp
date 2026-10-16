void func_80154118(void) {
    typedef struct { s32 w[15]; } M2cBlock3C; /* m2c memcpy of 60 bytes */
    s32 sp28[15];

    *(M2cBlock3C *)(&sp28) = *(M2cBlock3C *)(&D_8015F4DC);
    if ((u8) D_800E738A < 0xCU) {
        if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80)) {
            func_8006B900();
        } else if ((D_800CA2F8 == 0x11) || (D_800CA2F8 == 0x10)) {
            func_8006B900();
        }
    }
    (*(s32 (**)(s32))((unsigned char *)(sp + (D_800E738A * 4)) + (40)))(0x80);
}
