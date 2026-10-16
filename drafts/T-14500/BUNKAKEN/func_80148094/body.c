void func_80148094(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
                D_800CA148 = 0x54;
                break;
            case 2:
                D_800CA148 = 0x55;
                break;
            case 3:
                D_800CA148 = 0x56;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        func_80083440(3U);
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bg_fadein();
        break;
    case 11:
        D_800CA148 = 0x47;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_in();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x57;
                break;
            case 2:
                D_800CA148 = 0x58;
                break;
            case 3:
                D_800CA148 = 0x59;
                break;
            }
            func_80148E68();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
