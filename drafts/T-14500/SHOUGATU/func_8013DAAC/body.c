s32 func_8013DAAC(void) {
    if (((D_800E644C & 0xF) == 0xC) && (((u32) (D_800E644C << 0x17) >> 0x1B) == 0x18) && !(D_800E699E & 0x20)) {
        D_800E699E |= 0x20;
        D_800E6280.unk_69C[D_800E699C].unk_01 = 0;
        main_state_set(0x57);
        return 0;
    }
    if (((D_800E6378 & 0xF) == 0xC) && (((u32) (D_800E6378 << 0x17) >> 0x1B) == 0x18) && !(D_800E699E & 0x40)) {
        D_800E699E |= 0x40;
        main_state_set(0x56);
        return 0;
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    func_800AE0F0(D_800CA19C, "伊集院邸");
    D_800E7322 = 0;
    D_800E7368 = 1;
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    D_80145F2C = 0;
    D_80145F30 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    func_800AE0F0(D_800CA16C, &D_800E6354);
    if ((u32) ((u32) (D_800E6280.unk_1BC[0xC].unk_0C.w << 0x14) >> 0x1D) >= 3U) {
        D_800E6280.unk_1BC[0xC].unk_0C.b[1] = (D_800E6280.unk_1BC[0xC].unk_0C.b[1] & 0xFFF1) | 4;
    }
    if (((u32) (D_800E66E8 << 0x14) >> 0x1D) == 0) {
        func_800AE0F0(D_800CA188, "黒服");
    } else {
        func_800AE0F0(D_800CA188, "外井");
    }
    D_800E71DF = 0xC;
    func_8013C070();
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    return func_8004284C();
}
