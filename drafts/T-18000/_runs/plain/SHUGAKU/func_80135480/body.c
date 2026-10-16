void func_80135480(void) {
    typedef struct { s32 w[74]; } M2cBlock128; /* m2c memcpy of 296 bytes */
    s32 temp_v0;

    *(M2cBlock128 *)(&sp2C[0]) = *(M2cBlock128 *)(&D_8013C2F0);
    (&sp2C[0])[D_800E738A](0x80);
    temp_v0 = *(s32 *)((unsigned char *)(sp + (D_800E738A * 4)) + (44));
    if (&D_80085368 == temp_v0) {
        D_801206DB |= 0x80;
    }
    if (((func_800853FC == temp_v0) || (func_80135A80 == temp_v0)) && ((u8) D_800B593C < 8U)) {
        D_801206DB &= 0xFF7F;
        D_8012071F &= 0xFF7F;
    }
    if ((((u8) D_800E738A >= 0x16U) && ((u8) D_800E738A < 0x1CU)) || (((u8) D_800E738A >= 0x20U) && ((u8) D_800E738A < 0x25U))) {
        D_801206DF = D_80120657;
    }
    if ((u8) D_8013C2E4 != 0) {
        func_801360B0();
    }
}
