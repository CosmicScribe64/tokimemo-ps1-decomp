void func_8013B25C(void) {
    s16 temp_v1;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x525B);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_801431D4();
        addr_init_bustup();
        D_800E71DF = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
                D_800CA148 = 3;
                break;
            case 3:
                D_800CA148 = 4;
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E62BE * 4)) - 0x17C;
            func_8013C3C4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((D_800E62BE == 0x5F) && (func_8008667C(2) == 4)) {
            D_800CA148 = 0x45;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((D_800E62BE == 0x5F) && (func_8008667C(2) == 4)) {
            normal_date_speak();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EAC = 3;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 11:
        func_800866A0(0x525B);
        break;
    case 12:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E71DF = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 13:
        normal_date_bggirl_fadein();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0xE;
                break;
            case 2:
                D_800CA148 = 0xF;
                break;
            case 3:
                D_800CA148 = 0x10;
                break;
            case 4:
                D_800CA148 = 0x11;
                break;
            }
            temp_v1 = (D_800CA148 + (D_800E62BE * 4)) - 0x17C;
            D_800CA148 = temp_v1;
            if (D_800E62BE == 0x5F) {
                D_800CA148 = temp_v1 + 0xC;
            }
            func_8013C3C4();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
