void func_80158764(void) {
    GameState *state;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_7;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_6;
    state = &D_800E6280;
    switch (state->unk_110A) {
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
        if (state->unk_03E == 0x5F) {
            D_800CA148 = 0x2E;
        } else {
            D_800CA148 = 0x2F;
        }
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
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x5D41);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->unk_F5F = 1;
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
        if (func_8008667C(1) == 4) {
            D_800CA148 = 9;
        } else {
            D_800CA148 = 0x8D;
        }
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
        if (state->unk_03E == 0x60) {
            D_800CA148 = D_800CA148 + 0x18;
        } else {
            if (state->unk_03E == 0x61) {
                D_800CA148 = D_800CA148 + 0x1B;
            }
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
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
            case 0:
            case 1:
                D_800CA148 = 0x91;
                break;
            case 2:
                D_800CA148 = 0x92;
                break;
            case 3:
                D_800CA148 = 0x93;
                break;
            }
            temp_v0_6 = state->unk_1BC[1].unk_0C.b[2] >> 4;
            if (temp_v0_6 == 1) {
                D_800CA148 += 3;
            }
            if (state->unk_03E == 0x61) {
                if (temp_v0_6 == 0) {
                    D_800CA148 = D_800CA148 + 0x1B;
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
        case 0:
        case 1:
            D_800CA148 = 0x97;
            break;
        case 2:
            D_800CA148 = 0x98;
            break;
        case 3:
            D_800CA148 = 0x99;
            break;
        }
        if (!(state->unk_1BC[1].unk_0C.f.b2)) {
            D_800CA148 += 3;
        }
        if ((state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
            D_800CA148 = D_800CA148 + 6;
        } else {
            if (state->unk_03E == 0x61) {
                D_800CA148 = D_800CA148 + 0x18;
            }
        }
        func_8004284C();
        if (!(state->unk_1BC[1].unk_0A & 3)) {
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
        } else if (!(state->unk_1BC[1].unk_0A & 3)) {
            if (func_8008667C(1) < 2) {
                D_800CA148 = 0xD;
            } else {
                D_800CA148 = 0xC;
            }
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
