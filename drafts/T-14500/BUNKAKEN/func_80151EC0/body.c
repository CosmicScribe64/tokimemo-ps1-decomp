void func_80151EC0(void) {
    s32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x2C;
                break;
            case 2:
                D_800CA148 = 0x2E;
                break;
            case 3:
                D_800CA148 = 0x2C;
                break;
            }
            func_80152618();
            func_80083440(0U);
            func_80063668();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        D_800E71DF = 0xE;
        func_8004284C();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
