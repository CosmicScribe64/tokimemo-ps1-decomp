void func_80137144(void) {
    func_80083808();
    switch (D_800E7389) {
    case 0:
        func_80137664();
        break;
    case 1:
        func_80137818();
        break;
    case 2:
        func_80137A04();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80042808();
        } else {
            if (D_800E7380++ >= 0x401U) {
                func_80042908(2);
            }
        }
        break;
    case 4:
        switch (D_80144E10) {
        case 0:
            func_80046500();
            break;
        case 1:
            func_801398F0();
            break;
        case 2:
            func_80137FF0();
            break;
        case 3:
            func_80138B40();
            break;
        case 4:
            func_8013A950();
            break;
        case 5:
            func_8013A190();
            break;
        case 6:
            func_8013A500();
            break;
        case 7:
            func_801391D4();
            break;
        case 8:
            func_80137BB0();
            break;
        case 9:
            func_80046500();
            break;
        case 10:
            func_8013AF10();
            break;
        default:
            func_80046500();
            break;
        }
        break;
    case 5:
        func_801377CC();
        break;
    case 6:
        func_80137818();
        break;
    case 7:
        func_8013737C();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}
