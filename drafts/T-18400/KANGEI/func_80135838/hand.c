s32 func_80135838(void) {
    GameState *g;

    g = &D_800E6280;
    if ((u8) D_80139ABC != 0) {
        func_80042808();
        return 0;
    }
    if (D_800E6280.unk_75D == 0xFF) {
        func_80042808();
        return 0;
    }
    func_800AE120(D_800E6280.unk_10F8);
    if (((func_800AE0D0() % 256) >= 5) || (D_800E6280.unk_75D >= 0xB)) {
        func_80042808();
        return 0;
    }
    if (!D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_0C.f.b1) {
        func_80042808();
        return 0;
    }
    if ((((u32) (g->unk_1BC[0].unk_10.w << 0x17) >> 0x1B) == g->unk_040) && ((g->unk_1BC[0].unk_10.w & 0xF) == g->unk_03F)) {
        func_80042808();
        return 0;
    }
    if ((((u32) (g->unk_0F8 << 0x17) >> 0x1B) == g->unk_040) && ((g->unk_0F8 & 0xF) == g->unk_03F)) {
        func_80042808();
        return 0;
    }
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    D_80139ABC = 1;
    *(s32 *)&D_800E6280.unk_0FC /= 2;
    *(s32 *)&D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_08 = 0;
    back_clear_switch(1);
    func_80048EB8(0);
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_80132E74();
    func_801349D4();
    D_80139AC0 = 0;
    func_80085B3C(0xE, 0xD);
    return func_8004284C();
}
