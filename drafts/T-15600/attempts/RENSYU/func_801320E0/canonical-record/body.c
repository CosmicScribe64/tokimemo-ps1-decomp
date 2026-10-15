void func_801320E0(void) {
    u32 temp_t4;

    switch (D_800E738A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(0, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_8008585C();
        message_window_init();
        icon_disp_switch(0);
        func_8006764C(0);
        parameter_disp_switch(0);
        hizuke_init();
        k_reset(0);
        func_8006612C((s32) "練習試合");
        func_80065F34(0);
        dec_bg_reset();
        set_dec_bri(0);
        func_800AE120(D_800E7374);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x206);
            func_8004284C();
        }
        break;
    case 2:
        cd_read_sectors(7U, 0x801E8000, 0xB037);
        func_8004284C();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800847B8(0U);
            func_80084E4C();
            func_80132000();
            D_800CA160 = D_801342B0;
            D_800CA164 = D_801342B4;
            temp_t4 = (u16) D_800E6374 >> 0xC;
            D_800CA168 = D_801342B8;
            switch (temp_t4) {
            case 6:
                D_800CA148 = 0;
                break;
            case 7:
                D_800CA148 = 1;
                break;
            case 8:
                D_800CA148 = 2;
                break;
            case 9:
                D_800CA148 = 3;
                break;
            case 10:
                D_800CA148 = 4;
                break;
            default:
                func_80046500();
                break;
            }
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 4:
    case 6:
        normal_date_speak();
        break;
    case 5:
        D_800CA148 = (D_800E6280.unk_66C[(u16) D_800E6374 >> 0xC].unk_02 & 0xF) + 5;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 7:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
