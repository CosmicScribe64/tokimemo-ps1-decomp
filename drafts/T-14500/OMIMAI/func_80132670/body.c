void func_80132670(void) {
    u8 temp_a0;

    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "玄関");
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800CA148 = 0;
    D_800CA14C = 0;
    temp_a0 = D_800E69DD & 0xFF;
    if ((D_800E69DD == 4) && (((u16) D_800E6374 >> 0xC) == ((u8) D_800E652A >> 4))) {
        D_800CA148 = 1;
    }
    D_800E71DF = temp_a0;
    func_800847B8(temp_a0);
    func_8013285C();
    func_8004284C();
}
