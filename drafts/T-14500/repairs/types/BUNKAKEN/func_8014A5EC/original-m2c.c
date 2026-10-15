extern u8 D_800E62BE;
extern ? D_8015633C;

void func_8014A5EC(void) {
    ? sp28;

    M2C_MEMCPY_ALIGNED(&sp28, &D_8015633C, 0x128);
    *(u8 *)((unsigned char *)(&sp28 + 0x120) + (8)) = (u8) *(u8 *)((unsigned char *)(&D_8015633C + 0x120) + (8));
    func_800AE0F0(D_800CA25C, (&sp28 + (D_800E62BE * 0x63) + (D_80122CDC * 0x21)) - 0x24BD);
}
