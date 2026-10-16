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
    load_palette(D_80145FE0, 0x11, 1, 3, 0);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x41000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 4;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_80145FD4;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_80145FD8;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_80145FD0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_80145FDC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
}
