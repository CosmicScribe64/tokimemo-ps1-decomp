void func_801464A8(void) {
    u32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E71DF = 3;
            func_80083474();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xC;
                break;
            case 2:
                D_800CA148 = 0xD;
                break;
            case 3:
                D_800CA148 = 0xE;
                break;
            case 4:
                D_800CA148 = 0xF;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        if (func_8008667C(3) == 4) {
            D_800CA148 = 0x45;
            func_8004284C();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bg_fadeout();
        break;
    case 11:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
