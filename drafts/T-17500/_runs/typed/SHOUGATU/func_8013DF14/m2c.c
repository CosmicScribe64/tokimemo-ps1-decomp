extern s16 D_80145FDC;

void func_8013DF14(void) {
    func_80085B3C(4, 0);
    D_800E6280.unk_1BC[0xC].unk_0C.b[1] = (((((u32) (D_800E6280.unk_1BC[0xC].unk_0C.w << 0x14) >> 0x1D) + 1) * 2) & 0xE) | (D_800E6280.unk_1BC[0xC].unk_0C.b[1] & 0xFFF1);
    if ((u32) ((u32) (D_800E6280.unk_1BC[0xC].unk_0C.w << 0x14) >> 0x1D) >= 4U) {
        D_800E6280.unk_1BC[0xC].unk_0C.b[1] = (D_800E6280.unk_1BC[0xC].unk_0C.b[1] & 0xFFF1) | 6;
    }
    func_800AE0F0(D_800CA188, "伊集院");
    func_800AE0F0(girl_name_buf, "伊集院");
    func_800AE0F0(D_800CA1DC, "パーティー会場");
    D_80145F2C = 9;
    func_8004284C();
    func_8013DD90();
    func_80043914(D_80145FE0, 0x11, 1, 3, 0);
    func_80048F64(0x60);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 0xC;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6530)) = 1;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6584)) = 0x41000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6531)) = 4;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6540)) = (s32) D_80145FD4;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6544)) = (s32) D_80145FD8;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6580)) = (s32) D_80145FD0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6548)) = (s16) D_80145FDC;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6552)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6595)) = 0x11;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0x78;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6535)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6533)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 8;
}
