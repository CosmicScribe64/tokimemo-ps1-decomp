void func_80135C4C(void) {
    s16 var_v1_3;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            func_800866A0(0x5D6E);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        addr_init_bustup();
        D_800E71DF = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (D_800E62BE == 0x60) {
                temp_v0 = func_8008667C(1);
                switch (temp_v0) {
                case 0:
                case 1:
                    D_800CA148 = 0x26;
                    break;
                case 2:
                case 3:
                    D_800CA148 = 0x27;
                    break;
                case 4:
                    D_800CA148 = 0x28;
                    break;
                }
            } else {
                temp_v0_2 = func_8008667C(1);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    D_800CA148 = 0x22;
                    break;
                case 2:
                    D_800CA148 = 0x23;
                    break;
                case 3:
                    D_800CA148 = 0x24;
                    break;
                case 4:
                    D_800CA148 = 0x25;
                    break;
                }
                if (D_800E62BE == 0x61) {
                    D_800CA148 += 7;
                }
            }
            func_80136230();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((D_800E62BE == 0x61) && (func_8008667C(1) == 4)) {
            D_800CA148 = 0x3E;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((D_800E62BE == 0x61) && (func_8008667C(1) == 4)) {
            normal_date_speak();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D6E);
        }
        break;
    case 13:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E71DF = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x2D;
                break;
            case 2:
                D_800CA148 = 0x2F;
                break;
            case 3:
                D_800CA148 = 0x31;
                break;
            case 4:
                D_800CA148 = 0x33;
                break;
            }
            var_v1_3 = D_800CA148;
            if ((u16) D_800E68F0 < (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_3 += 1;
            }
            D_800CA148 = var_v1_3;
            if (D_800E62BE == 0x61) {
                D_800CA148 = var_v1_3 + 8;
            }
            func_80136230();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        if (func_8008667C(1) == 4) {
            if (((u8) D_800E62BE < 0x61U) && ((u16) D_800E68F0 >= (u32) ((D_800E62BE * 0x64) - 0x24B8))) {
                D_800CA148 = 0x3E;
                func_8004284C();
            } else if (D_800E62BE == 0x61) {
                if ((u16) D_800E68F0 >= 0x12CU) {
                    D_800CA148 = 0x3E;
                    func_8004284C();
                } else {
                    D_800CA148 = 0x3F;
                    func_8004284C();
                }
            } else {
                func_80042940(0x14);
            }
        } else {
            func_80042940(0x14);
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
