void func_80158764(void) {
    s16 temp_a1;
    s16 var_a1;
    s16 var_a1_3;
    s16 var_a1_4;
    s16 var_a1_5;
    s16 var_a1_6;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_7;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_6;
    u8 temp_v0;
    u8 temp_v1;

    temp_v0 = D_800E6280.unk_110A;
    switch (temp_v0) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        var_a1 = 0x2F;
        if (D_800E6280.unk_03E == 0x5F) {
            var_a1 = 0x2E;
        }
        D_800CA148 = var_a1;
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = temp_v0 + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x5D41);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(1);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            case 4:
                D_800CA148 = 4;
                break;
            }
            func_80158E3C();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        var_a1_3 = 0x8D;
        if (func_8008667C(1) == 4) {
            var_a1_3 = 9;
        }
        D_800CA148 = var_a1_3;
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_8004284C();
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x8E;
            break;
        case 2:
            D_800CA148 = 0x8F;
            break;
        case 3:
            D_800CA148 = 0x90;
            break;
        case 4:
            func_80042940(0x15);
            break;
        }
        temp_a1 = D_800CA148;
        temp_v1 = D_800E6280.unk_03E;
        if (temp_v1 == 0x60) {
            D_800CA148 = temp_a1 + 0x18;
        } else {
            D_800CA148 = temp_a1;
            if (temp_v1 == 0x61) {
                D_800CA148 = temp_a1 + 0x1B;
            }
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0xA3;
                break;
            case 2:
                D_800CA148 = 0xA4;
                break;
            case 3:
                D_800CA148 = 0xA5;
                break;
            }
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            default:
                var_a1_4 = 0x93;
                if (temp_v0_5 != 3) {
                    var_a1_4 = D_800CA148;
                }
                break;
            case 0:
            case 1:
                var_a1_4 = 0x91;
                break;
            case 2:
                var_a1_4 = 0x92;
                break;
            }
            temp_v0_6 = (u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4;
            if (temp_v0_6 == 1) {
                var_a1_4 += 3;
            }
            D_800CA148 = var_a1_4;
            if (D_800E6280.unk_03E == 0x61) {
                D_800CA148 = var_a1_4;
                if (temp_v0_6 == 0) {
                    D_800CA148 = var_a1_4 + 0x1B;
                }
            }
        }
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        temp_v0_7 = func_8008667C(1);
        switch (temp_v0_7) {
        default:
            var_a1_5 = 0x99;
            if (temp_v0_7 != 3) {
                var_a1_5 = D_800CA148;
            }
            break;
        case 0:
        case 1:
            var_a1_5 = 0x97;
            break;
        case 2:
            var_a1_5 = 0x98;
            break;
        }
        if (!(D_800E6280.unk_1BC[1].unk_0C.w & 4)) {
            var_a1_5 += 3;
        }
        if (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1) {
            D_800CA148 = var_a1_5 + 6;
        } else {
            D_800CA148 = var_a1_5;
            if (D_800E6280.unk_03E == 0x61) {
                D_800CA148 = var_a1_5 + 0x18;
            }
        }
        func_8004284C();
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            func_80042940(0x15);
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        if (func_8008667C(1) == 4) {
            D_800CA148 = 0xB5;
            func_8004284C();
        } else if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            var_a1_6 = 0xC;
            if (func_8008667C(1) < 2) {
                var_a1_6 = 0xD;
            }
            D_800CA148 = var_a1_6;
            func_8004284C();
        } else {
            func_80042940(0x18);
        }
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_bg_fadeout();
        break;
    case 25:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
