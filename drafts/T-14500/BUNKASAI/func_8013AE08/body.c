void func_8013AE08(void) {
    u32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (D_800E62BE == 0x5F) {
            D_800CA148 = 0x2E;
        } else {
            D_800CA148 = 0x2F;
        }
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x5012);
        break;
    case 9:
        addr_init_bustup();
        D_800E71DF = 8;
        func_80083474();
        func_8014338C();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = (D_800E62BE * 3) - 0xAF;
                break;
            case 2:
                D_800CA148 = (D_800E62BE * 3) - 0xAE;
                break;
            case 3:
                D_800CA148 = (D_800E62BE * 3) - 0xAD;
                break;
            case 4:
                D_800CA148 = 0x77;
                break;
            }
            func_8013B120();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
