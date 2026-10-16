void func_800FF61C(void) {
    ? sp2C;

    M2C_MEMCPY_ALIGNED(&sp2C, &D_80121AF8, 0xC0);
    func_80078950("%d\n", D_800B1AF6);
    *(? (**)(?))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (44))(0x80);
    if (D_800EECE0 != 0) {
        *(u8 *)((unsigned char *)D_800E9620 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_800E9620 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_800E9620 + (6671)) = (u8) D_800F19AB;
    } else {
        *(u8 *)((unsigned char *)D_800E9620 + (6667)) = (u8) (D_800E9620->unk_00 & 0xFF7F);
    }
    if ((u8) D_800B1AF6 >= 0x10U) {
        if ((u8) D_800B1AF6 < 0x13U) {
            func_8010AFE4();
        }
    }
}
