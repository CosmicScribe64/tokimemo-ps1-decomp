void func_80143DD0(void) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_v0_3;
    s32 temp_v0_6;
    s32 var_v0;
    s32 var_v0_2;
    u32 temp_t4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_4;
    u32 temp_v0_5;

    switch (D_800E738A) {
    case 0:
        if (D_80122EBC != 0xFF) {
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        } else {
            D_800CA148 = 1;
            D_800CA14C = 0;
            func_80042940(2);
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801324E0();
            D_800CA148 = 1;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            func_8004284C();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_two_select();
        break;
    case 4:
        if (D_80122CDC == 1) {
            func_80042940(6);
        } else {
            func_80042940(7);
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (D_80122EBC != 0xFF) {
            D_80122EBC = 0xFF;
        }
        func_80042908(2);
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        select_girl();
        break;
    case 9:
        if (D_80122CDC == D_80122EA4) {
            D_800CA148 = 4;
            D_800CA14C = 0;
            func_8004284C();
        } else if (D_80122CDC == 0xB) {
            D_800CA148 = 5;
            D_800CA14C = 0;
            func_80042940(5);
        } else {
            if ((D_80122CDC == 0) && ((u32) ((u8) D_800E644A >> 4) >= 6U)) {
                D_80122EBC = 0xB;
            } else {
                D_80122EBC = D_80122CDC;
            }
            func_800847B8((u8) D_80122CDC);
            func_80042940(0xC);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        D_800CA148 = 2;
        func_80042940(7);
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_800866A0(D_80155EA0[D_80122EBC]);
        break;
    case 14:
        addr_init_bustup();
        if (D_80122EBC == 0xB) {
            D_800E71DF = 0;
        } else {
            D_800E71DF = (s8) D_80122EBC;
        }
        func_80083474();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            if (D_80122EBC == 0) {
                func_80042940(0x14);
            } else if (D_80122EBC < 4) {
                func_80042940(0x1F);
            } else {
                func_80042940(0x2A);
            }
        }
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        D_80122EBC = 0xFF;
        func_80042908(2);
        break;
    case 20:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0x10;
            break;
        case 2:
            D_800CA148 = 0x1B;
            break;
        case 3:
            D_800CA148 = 0x24;
            break;
        case 4:
            D_800CA148 = 0x2D;
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x11;
            break;
        case 2:
            D_800CA148 = 0x1C;
            break;
        case 3:
            D_800CA148 = 0x25;
            break;
        case 4:
            D_800CA148 = 0x2E;
            break;
        }
        D_800CA148 += (u8) D_800E644A >> 4;
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (func_8008667C(0) == 4) {
            D_800CA148 = 0x34;
            func_80042940(0x11);
        } else {
            if (D_800E6448 & 4) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x18;
            }
            func_8004284C();
        }
        D_800E6448 = (s8) ((u8) D_800E6448 | 4);
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x19;
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 0x22;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x2B;
            func_8004284C();
            break;
        }
        /* fallthrough */
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        D_80122EBC = 0xFF;
        func_80042908(2);
        break;
    case 31:
        temp_v0_4 = func_8008667C(D_80122EBC);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 8;
            break;
        case 2:
            D_800CA148 = 0xF;
            break;
        case 3:
            D_800CA148 = 0x15;
            break;
        case 4:
            D_800CA148 = 0x1B;
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        temp_v0_5 = func_8008667C(D_80122EBC);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 9;
            break;
        case 2:
            D_800CA148 = 0x10;
            break;
        case 3:
            D_800CA148 = 0x16;
            break;
        case 4:
            D_800CA148 = 0x1C;
            break;
        }
        if (((D_80122EBC == 1) && (((u8) D_800E6482 >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) D_800E64BA >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) D_800E64F2 >> 4) == 5))) {
            D_800CA148 += 1;
        }
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        if (func_8008667C(D_80122EBC) == 4) {
            D_800CA148 = 0xE;
            func_80042940(0x11);
        } else {
            if ((D_80122EBC == 1) && (func_8008667C(1) < 2)) {
                D_800CA148 = 0x12;
            } else if (((u32) (*(&D_800E6448 + (D_80122EBC * 0x38)) << 0x1D) >> 0x1F) == 1) {
                D_800CA148 = 0xB;
            } else {
                D_800CA148 = 0x14;
            }
            func_8004284C();
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_0C.b[0] |= 4;
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        temp_v0_6 = func_8008667C(D_80122EBC);
        switch (temp_v0_6) {
        case 0:
        case 1:
            D_800CA148 = 0xC;
            if (((D_80122EBC == 1) && (((u8) D_800E6482 >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) D_800E64BA >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) D_800E64F2 >> 4) == 5))) {
                D_800CA148 = 0xD;
            }
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 0x13;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x19;
            func_8004284C();
            break;
        }
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        normal_date_girl_out();
        D_800CA148 = 0xE;
        break;
    case 40:
        normal_date_speak();
        break;
    case 41:
        D_80122EBC = 0xFF;
        func_80042908(2);
        break;
    case 42:
        temp_a3 = D_80122EBC;
        temp_a0 = temp_a3;
        if (temp_a3 == 0xB) {
            var_v0 = get_g_zyotai_s(0, temp_a3);
        } else {
            D_80122EBC = temp_a3;
            var_v0 = get_g_zyotai_s(temp_a0, temp_a3);
        }
        temp_t4 = var_v0 & 0x7F;
        switch (temp_t4) {
        case 0:
        case 1:
            D_800CA148 = 3;
            break;
        case 2:
            D_800CA148 = 4;
            break;
        case 3:
            D_800CA148 = 5;
            break;
        case 4:
            if ((D_80122EBC == 4) && (((u16) D_800E6374 >> 0xC) == ((u8) D_800E652A >> 4))) {
                D_800CA148 = 8;
            } else {
                D_800CA148 = 6;
            }
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        temp_a3_2 = D_80122EBC;
        temp_a0_2 = temp_a3_2;
        if (temp_a3_2 == 0xB) {
            var_v0_2 = get_g_zyotai_s(0, temp_a3_2);
        } else {
            D_80122EBC = temp_a3_2;
            var_v0_2 = get_g_zyotai_s(temp_a0_2, temp_a3_2);
        }
        if (var_v0_2 == 4) {
            func_80042940(0x11);
        } else if ((D_80122EBC == 8) && (func_8008667C(8) >= 2)) {
            func_8004284C();
        } else {
            func_80042908(3);
        }
        break;
    case 45:
        switch (D_800E62BE) {
        case 0x5F:
            D_800CA148 = 9;
            break;
        case 0x60:
            D_800CA148 = 0xA;
            break;
        case 0x61:
            D_800CA148 = 0xB;
            break;
        }
        if (func_8008667C(8) == 3) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 46:
        normal_date_speak();
        break;
    case 47:
        D_800CA148 = 0xF;
        func_8004284C();
        break;
    case 48:
        normal_date_speak();
        break;
    case 49:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
}
