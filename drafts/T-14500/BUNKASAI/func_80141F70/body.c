void func_80141F70(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x6935);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E71DF = 0xA;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
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
                D_800CA148 = 0xF;
                break;
            }
            func_80142694();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(0xA);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 4;
            break;
        case 2:
            D_800CA148 = 5;
            break;
        case 3:
            D_800CA148 = 6;
            break;
        case 4:
            func_80042940(0xC);
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (func_8008667C(0xA) < 2) {
            D_800CA148 = 0x75;
        }
        normal_date_girl_out();
        break;
    case 13:
        if ((func_8008667C(0xA) == 4) || (func_8008667C(0xA) < 2)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
