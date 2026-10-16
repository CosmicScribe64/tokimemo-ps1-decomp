void func_800FF61C(void) {
    typedef struct { s32 w[48]; } M2cBlockC0; /* m2c memcpy of 192 bytes */
    s32 sp2C[48];

    *(M2cBlockC0 *)(&sp2C) = *(M2cBlockC0 *)(&D_80121AF8);
    func_80078950("%d\n", D_800B1AF6);
    (*(s32 (**)(s32))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (44)))(0x80);
    if (D_800EECE0 != 0) {
        D_800EB02B |= 0x80;
        D_800EB02F = D_800F19AB;
    } else {
        D_800EB02B &= 0xFF7F;
    }
    if ((u8) D_800B1AF6 >= 0x10U) {
        if ((u8) D_800B1AF6 < 0x13U) {
            func_8010AFE4();
        }
    }
}
