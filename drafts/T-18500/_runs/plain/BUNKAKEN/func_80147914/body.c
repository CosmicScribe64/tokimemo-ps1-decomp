void func_80147914(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3F;
                break;
            case 2:
                D_800CA148 = 0x40;
                break;
            case 3:
                D_800CA148 = 0x41;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 += 2;
        func_8004284C();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = ((D_800E7D4F | 2) & 0xFFE3) | 8;
        main_state_set(0xD0);
        break;
    case 10:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 11:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 12:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 14:
        func_800866A0(0x622D);
        break;
    case 15:
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 16:
        normal_date_bggirl_fadein();
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x48;
                break;
            case 2:
                D_800CA148 = 0x49;
                break;
            case 3:
                D_800CA148 = 0x4A;
                break;
            }
            func_80148E68();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = (D_800E7D4F | 2) & 0xFFE3;
        func_80042908(0x8D);
        break;
    case 21:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 22:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 23:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 25:
        func_800866A0(0x622D);
        break;
    case 26:
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 27:
        normal_date_bggirl_fadein();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x4B;
                break;
            case 2:
                D_800CA148 = 0x4D;
                break;
            case 3:
                D_800CA148 = 0x4F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_80148E68();
        }
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x51;
            break;
        case 2:
            D_800CA148 = 0x52;
            break;
        case 3:
            D_800CA148 = 0x53;
            break;
        }
        func_8004284C();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        normal_date_bggirl_fadeout();
        break;
    case 33:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        D_800E7D4F |= 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
