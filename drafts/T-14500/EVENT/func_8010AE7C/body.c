void func_8010AE7C(void) {
    s32 sp2C[16];
    typedef struct { s32 w[56]; } M2cBlockE0; /* m2c memcpy of 224 bytes */
    *(M2cBlockE0 *)(&sp2C[0]) = *(M2cBlockE0 *)(&D_80123FCC);
    (&sp2C[0])[D_800B1AF6](0x80);
    if ((u8) D_800B1AF6 >= 0x1EU) {
        if ((u8) D_800B1AF6 < 0x25U) {
            func_8003C2C0();
        } else {
            goto block_4;
        }
    } else {
block_4:
        if (D_800B1AF6 == 0x2B) {
            func_8010AFE4();
        }
    }
}
