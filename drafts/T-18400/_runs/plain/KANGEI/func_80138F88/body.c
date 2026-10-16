s32 func_80138F88(void) {
    s16 var_a0;
    s16 var_s0_2;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v1;
    s32 var_t6;
    u32 temp_a0;
    u32 temp_t5;
    u32 var_s6;
    u8 var_s0;
    u8 var_s7;

    var_s7 = 0xFF;
    var_s6 = 4;
    var_s0 = 0;
    var_t6 = 0 * 8;
    do {
        if ((*(&D_800E6448 + ((var_t6 - var_s0) * 8)) & 2) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_s7) & 0xFF, (((u32) (get_h_yuukou(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 = (u8) (s16) (var_s0 + 1);
        var_t6 = var_s0 * 8;
    } while ((s32) var_s0 < 0xB);
    temp_t5 = get_g_zyotai_s(var_s7) & 0x7F;
    if ((var_s7 != 0xFF) && (temp_t5 < 2U)) {
        D_800E71DF = var_s7;
        func_80085B3C(0xD, 0x2A);
        D_800E71DF = var_s7;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_80041584();
        back_clear_switch(1);
        func_80048EB8(0);
        message_window_init();
        hizuke_init();
        tpage_buf_clear();
        func_8004E58C();
        D_800E7322 = 0;
        D_800E7368 = 1;
        D_800B593C = 0;
        func_8008585C();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        D_80139ADC = 0;
        D_80139AE0 = 0;
        addr_init_bustup();
        func_80084E4C();
        func_800847B8(D_800E71DF);
        func_80132E74();
        D_80139AD0 = D_80139A74;
        D_80139AD4 = D_80139A94;
        D_80139AD8 = D_80139AB4;
        D_80139AC0 = 0;
        D_80139ADC = 0;
        D_80139AE0 = 0;
        func_800847B8(D_800E71DF);
        func_801392D4();
        if (((u8) D_80139AC4 != 0) && ((u8) D_8013A2A8 == 0)) {
            D_800E738A += 2;
        }
        return func_8004284C();
    }
    func_8008585C();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    D_80139AD0 = D_8013A19C;
    D_80139AD4 = D_8013A1D0;
    D_80139AD8 = D_8013A204;
    func_8006509C();
    var_s0_2 = 0;
    var_a0 = 0;
    do {
        icon_can_use_set(var_a0, 0);
        var_s0_2 += 1;
        var_a0 = var_s0_2;
    } while (var_s0_2 < 0xF);
    func_80065B0C(1);
    parameter_show_init();
    parameter_show_init();
    func_800847B8(0U);
    D_80139AC0 = 1;
    D_800E738A += 0x1E;
    return 0;
}
