void func_80148A28(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
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
                D_800CA148 = 0x71;
                break;
            case 2:
                D_800CA148 = 0x72;
                break;
            case 3:
                D_800CA148 = 0x73;
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
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x74;
                break;
            case 2:
                D_800CA148 = 0x75;
                break;
            case 3:
                D_800CA148 = 0x76;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E62BE * 3)) - 0x11D;
            func_80148E68();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (D_800E62BE == 0x61) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x7D;
                break;
            case 2:
                D_800CA148 = 0x7F;
                break;
            case 3:
                D_800CA148 = 0x81;
                break;
            }
            if (D_800E638A < 0x78) {
                D_800CA148 += 1;
            }
            func_8004284C();
        } else {
            func_80042940(0x11);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x83;
            break;
        case 2:
            D_800CA148 = 0x84;
            break;
        case 3:
            D_800CA148 = 0x85;
            break;
        }
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
