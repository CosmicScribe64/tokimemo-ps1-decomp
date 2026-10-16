void func_8013AFEC(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800E6280.unk_1BC[10].unk_0C.b[0] |= 8;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800E6280.unk_75D = 0xA;
    D_800E6280.unk_F5F = 0xA;
    func_800847B8(0xAU);
    func_800AE130(D_800CA17C, "%sさん", &D_800E6354);
    func_80136F90();
    D_80144DFC = D_80144DB0;
    D_80144E00 = D_80144DD4;
    D_80144E04 = D_80144DF8;
    func_8004284C();
}
