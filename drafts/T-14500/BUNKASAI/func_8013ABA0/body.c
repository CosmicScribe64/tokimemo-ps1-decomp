void func_8013ABA0(void) {
    u32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        addr_init_bustup();
        D_800E71DF = 8;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2D;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x69;
                break;
            case 2:
                D_800CA148 = 0x6A;
                break;
            case 3:
                D_800CA148 = 0x6B;
                break;
            case 4:
                D_800CA148 = 0x6C;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
