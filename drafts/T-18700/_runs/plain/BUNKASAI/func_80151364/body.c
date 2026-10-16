void func_80151364(void) {
    s16 var_v1;
    s16 var_v1_2;
    u32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E71DF = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                var_v1 = 2;
block_16:
                D_800CA148 = var_v1;
                break;
            case 2:
                var_v1 = 4;
                goto block_16;
            case 3:
                var_v1 = 6;
                goto block_16;
            case 4:
                var_v1 = 8;
                goto block_16;
            }
            if (D_800E62BE == 0x60) {
                D_800CA148 += 0xC;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadeout();
        D_800CA148 = 0;
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (func_8008667C(0) == 4) {
            if ((u16) D_800E68EC >= (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_2 = 0xC;
            } else {
                var_v1_2 = 0xD;
            }
        } else {
            var_v1_2 = 0xB;
            if ((u16) D_800E68EC >= (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_2 = 0xA;
            }
        }
        D_800CA148 = var_v1_2;
        if (D_800E62BE == 0x60) {
            D_800CA148 = var_v1_2 + 0xC;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
