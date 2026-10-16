s32 func_80138F88(void) {
    s16 i;
    u8 best;
    u8 bestState;
    u8 state;
    u8 other;
    u8 a;
    u8 b;
    u8 c;

    best = 0xFF;
    bestState = 4;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            state = get_g_zyotai_s(i) & 0x7F;
            if ((state < bestState) || ((state == bestState) && (other = best, a = get_h_yuukou(i), b = get_h_tokimeki(i), c = get_h_tokimeki(other), (u32) (get_h_yuukou(other) + c) < (u32) (b + a)))) {
                best = i;
                bestState = state;
            }
        }
    }
    if ((best != 0xFF) && ((get_g_zyotai_s(best) & 0x7F) < 2)) {
        D_800E6280.unk_F5F = best;
        func_80085B3C(0xD, 0x2A);
        D_800E6280.unk_F5F = best;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_80041584();
        back_clear_switch(1);
        func_80048EB8(0);
        message_window_init();
        hizuke_init();
        tpage_buf_clear();
        func_8004E58C();
        D_800E6280.unk_10A2 = 0;
        D_800E6280.unk_10E8 = 1;
        D_800B593C = 0;
        func_8008585C();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        D_80139ADC = 0;
        D_80139AE0 = 0;
        addr_init_bustup();
        func_80084E4C();
        func_800847B8(D_800E6280.unk_F5F);
        func_80132E74();
        D_80139AD0 = (KObj *) D_80139A5C[0][6];
        D_80139AD4 = D_80139A5C[1][6];
        D_80139AD8 = D_80139A5C[2][6];
        D_80139AC0 = 0;
        D_80139ADC = 0;
        D_80139AE0 = 0;
        func_800847B8(D_800E6280.unk_F5F);
        func_801392D4();
        if (((u8) D_80139AC4 != 0) && ((u8) D_8013A2A8 == 0)) {
            D_800E6280.unk_110A += 2;
        }
        return func_8004284C();
    }
    func_8008585C();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    D_80139AD0 = (KObj *) D_8013A170[0][0][11];
    D_80139AD4 = D_8013A170[0][1][11];
    D_80139AD8 = D_8013A170[0][2][11];
    func_8006509C();
    for (i = 0; i < 15; i++) {
        icon_can_use_set(i, 0);
    }
    func_80065B0C(1);
    parameter_show_init();
    parameter_show_init();
    func_800847B8(0U);
    D_80139AC0 = 1;
    D_800E6280.unk_110A += 0x1E;
    return 0;
}
