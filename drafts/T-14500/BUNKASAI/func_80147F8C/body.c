void func_80147F8C(void) {
    typedef struct { s32 w[74]; } M2cBlock128; /* m2c memcpy of 296 bytes */
    s32 sp28[74];

    *(M2cBlock128 *)(&sp28) = *(M2cBlock128 *)(&D_801606DC);
    *(u8 *)((unsigned char *)(&sp28 + 0x120) + (8)) = (u8) *(u8 *)((unsigned char *)(&D_801606DC + 0x120) + (8));
    func_800AE0F0(D_800CA25C, (&sp28 + (D_800E62BE * 0x63) + (D_80122CDC * 0x21)) - 0x24BD);
}
