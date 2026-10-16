void func_80133E10(void) {
    typedef struct { s32 w[43]; } M2cBlockAC; /* m2c memcpy of 172 bytes */
    s32 sp2C[43];
    s32 temp_v1;
    s32 var_t1;
    u8 temp_a0;
    u8 temp_v0;

    temp_v0 = D_800E738A;
    *(M2cBlockAC *)(&sp2C) = *(M2cBlockAC *)(&D_8013C008);
    *(s32 *)((unsigned char *)(&sp2C + 0xA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013C008 + 0xA8) + (4));
    if (temp_v0 < 8U) {
        func_8006B900();
        goto block_5;
    }
    if (temp_v0 >= 0x1DU) {
        var_t1 = temp_v0 * 4;
        if (temp_v0 < 0x23U) {
            func_8006B900();
            goto block_5;
        }
    } else {
block_5:
        var_t1 = temp_v0 * 4;
    }
    (*(s32 (**)(s32))((unsigned char *)(sp + var_t1) + (44)))(0x80);
    temp_v1 = *(s32 *)((unsigned char *)(sp + (D_800E738A * 4)) + (44));
    if (&D_80085368 == temp_v1) {
        D_801206DB |= 0x80;
        D_801206DF = D_80120657;
    }
    if (((u8) D_800E738A >= 0xDU) && ((u8) D_800E738A < 0x16U)) {
        D_801206DF = D_80120657;
    }
    temp_a0 = D_80120657 & 0xFF;
    if (func_800853FC == temp_v1) {
        D_801206DF = temp_a0;
        if (temp_a0 < 8U) {
            D_801206DB &= 0xFF7F;
        }
    }
}
