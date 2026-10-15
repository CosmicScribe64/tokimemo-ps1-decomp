void func_80137804(void) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
            case 3:
                D_800CA148 = 4;
                break;
            }
            func_80139DEC();
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
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 3;
            break;
        case 2:
        case 3:
            D_800CA148 = 5;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
