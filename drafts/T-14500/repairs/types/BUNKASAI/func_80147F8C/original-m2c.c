extern u8 D_800E62BE;
extern ? D_801606DC;

void func_80147F8C(void) {
    ? sp28;

    M2C_MEMCPY_ALIGNED(&sp28, &D_801606DC, 0x128);
    *(u8 *)((unsigned char *)(&sp28 + 0x120) + (8)) = (u8) *(u8 *)((unsigned char *)(&D_801606DC + 0x120) + (8));
    func_800AE0F0(D_800CA25C, (&sp28 + (D_800E62BE * 0x63) + (D_80122CDC * 0x21)) - 0x24BD);
}
