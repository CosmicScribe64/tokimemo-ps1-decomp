void func_801469DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
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
                D_800CA148 = 0xB;
                break;
            case 2:
                D_800CA148 = 0xC;
                break;
            case 3:
                D_800CA148 = 0xD;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0xE;
            break;
        case 2:
            D_800CA148 = 0x10;
            break;
        case 3:
            D_800CA148 = 0x11;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(6) >= 2) {
            func_80042940(0xE);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
