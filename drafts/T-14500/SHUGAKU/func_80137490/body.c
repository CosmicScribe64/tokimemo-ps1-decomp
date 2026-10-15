void func_80137490(void) {
    u8 var_v0;

    func_80083808();
    if ((D_800E7389 == 0) && ((u8) D_800E738A < 0x11U)) {
        func_80065B0C(1);
    }
    switch (D_800E7389) {
    case 0:
        func_801378F0();
        break;
    case 1:
        func_80138A60();
        break;
    case 2:
        func_80138B20();
        break;
    case 3:
        func_80139C80();
        break;
    case 4:
        func_80139D40();
        break;
    case 5:
        func_8013AA70();
        break;
    case 6:
        func_80132274();
        break;
    case 7:
        func_801336A4();
        break;
    case 8:
        func_80135480();
        break;
    case 9:
        func_80133E10();
        break;
    case 10:
        func_80134930();
        break;
    case 11:
        func_80134560();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    if (D_80122CE0 == 0xFF) {
        if ((D_800E7389 == 0) && ((u8) D_800E738A < 0x11U)) {
            func_80066C08(1);
            func_800578F4(0);
            parameter_show();
        } else {
            func_80083A10();
            func_80066C08(2);
            parameter_show();
        }
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E7200 & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        var_v0 = D_8011ECD3;
        if (D_80122D04 != 0) {
            var_v0 &= 0x7F;
        }
        D_8011ECD3 = var_v0;
        if (var_v0 & 0x80) {
            func_8006BA40();
        }
    }
    func_80066334();
    message_window_show();
    hizuke_show();
    func_80047560();
}
