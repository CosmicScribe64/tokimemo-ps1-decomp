s32 func_80135838(void) {
    if ((u8) D_80139ABC != 0) {
        func_80042808();
        return 0;
    }
    if (D_800E69DD == 0xFF) {
        func_80042808();
        return 0;
    }
    func_800AE120(D_800E7378);
    if (((func_800AE0D0() % 256) >= 5) || ((u8) D_800E69DD >= 0xBU)) {
        func_80042808();
        return 0;
    }
    if (!(D_800E6280.unk_1BC[D_800E69DD].unk_0C.w & 2)) {
        func_80042808();
        return 0;
    }
    if ((D_800E62C0 == ((u32) (D_800E644C << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E644C & 0xF))) {
        func_80042808();
        return 0;
    }
    if ((D_800E62C0 == ((u32) (D_800E6378 << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E6378 & 0xF))) {
        func_80042808();
        return 0;
    }
    D_800E71DF = D_800E69DD;
    D_80139ABC = 1;
    D_800E637C = (s32) D_800E637C / 2;
    *(s32 *)((unsigned char *)(&D_800E6280.unk_1BC[D_800E69DD]) + (8)) = 0;
    back_clear_switch(1, &D_800E6280, 0x38);
    func_80048EB8(0);
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    func_80084E4C();
    func_800847B8(D_800E71DF);
    func_80132E74();
    func_801349D4();
    D_80139AC0 = 0;
    func_80085B3C(0xE, 0xD);
    return func_8004284C();
}
