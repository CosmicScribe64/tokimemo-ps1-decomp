? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */

void func_80147914(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E6280.unk_110A) {                  /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E99C();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E884();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 0x3F;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x40;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x41;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        D_800CA148 += 2;
        func_8004284C();
        break;
    case 7:                                         /* switch 1 */
        func_80085A60();
        break;
    case 8:                                         /* switch 1 */
        func_8007E99C();
        break;
    case 9:                                         /* switch 1 */
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        D_800E7D4F = ((D_800E7D4F | 2) & 0xFFE3) | 8;
        main_state_set(0xD0);
        break;
    case 10:                                        /* switch 1 */
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 11:                                        /* switch 1 */
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 12:                                        /* switch 1 */
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 13:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 14:                                        /* switch 1 */
        func_800866A0(0x622D);
        break;
    case 15:                                        /* switch 1 */
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 16:                                        /* switch 1 */
        func_8007E884();
        break;
    case 17:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 0x48;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 0x49;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 0x4A;
                break;
            }
            func_80148E68();
        }
        break;
    case 18:                                        /* switch 1 */
        func_80085A60();
        break;
    case 19:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 20:                                        /* switch 1 */
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        D_800E7D4F = (D_800E7D4F | 2) & 0xFFE3;
        func_80042908(0x8D);
        break;
    case 21:                                        /* switch 1 */
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 22:                                        /* switch 1 */
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 23:                                        /* switch 1 */
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 24:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 25:                                        /* switch 1 */
        func_800866A0(0x622D);
        break;
    case 26:                                        /* switch 1 */
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 27:                                        /* switch 1 */
        func_8007E884();
        break;
    case 28:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:                                 /* switch 4 */
            case 1:                                 /* switch 4 */
                D_800CA148 = 0x4B;
                break;
            case 2:                                 /* switch 4 */
                D_800CA148 = 0x4D;
                break;
            case 3:                                 /* switch 4 */
                D_800CA148 = 0x4F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_80148E68();
        }
        break;
    case 29:                                        /* switch 1 */
        func_80085A60();
        break;
    case 30:                                        /* switch 1 */
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {                        /* switch 5; irregular */
        case 0:                                     /* switch 5 */
        case 1:                                     /* switch 5 */
            D_800CA148 = 0x51;
            break;
        case 2:                                     /* switch 5 */
            D_800CA148 = 0x52;
            break;
        case 3:                                     /* switch 5 */
            D_800CA148 = 0x53;
            break;
        }
        func_8004284C();
        break;
    case 31:                                        /* switch 1 */
        func_80085A60();
        break;
    case 32:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 33:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 34:                                        /* switch 1 */
        func_80085A60();
        break;
    case 35:                                        /* switch 1 */
        D_800E7D4F |= 1;
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
