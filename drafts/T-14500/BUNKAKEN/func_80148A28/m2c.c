? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E62BE;
extern s16 D_800E638A;
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80148A28(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x467C, 1);
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
                D_800CA148 = 0x71;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x72;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x73;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        func_8007E99C();
        break;
    case 7:                                         /* switch 1 */
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:                                         /* switch 1 */
        func_800866A0(0x622D);
        break;
    case 9:                                         /* switch 1 */
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:                                        /* switch 1 */
        func_8007E884();
        break;
    case 11:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 0x74;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 0x75;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 0x76;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E62BE * 3)) - 0x11D;
            func_80148E68();
        }
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        if (D_800E62BE == 0x61) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:                                 /* switch 4 */
            case 1:                                 /* switch 4 */
                D_800CA148 = 0x7D;
                break;
            case 2:                                 /* switch 4 */
                D_800CA148 = 0x7F;
                break;
            case 3:                                 /* switch 4 */
                D_800CA148 = 0x81;
                break;
            }
            if (D_800E638A < 0x78) {
                D_800CA148 += 1;
            }
            func_8004284C();
        } else {
            func_80042940(0x11);
        }
        break;
    case 14:                                        /* switch 1 */
        func_80085A60();
        break;
    case 15:                                        /* switch 1 */
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {                        /* switch 5; irregular */
        case 0:                                     /* switch 5 */
        case 1:                                     /* switch 5 */
            D_800CA148 = 0x83;
            break;
        case 2:                                     /* switch 5 */
            D_800CA148 = 0x84;
            break;
        case 3:                                     /* switch 5 */
            D_800CA148 = 0x85;
            break;
        }
        func_8004284C();
        break;
    case 16:                                        /* switch 1 */
        func_80085A60();
        break;
    case 17:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 18:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:                                        /* switch 1 */
        func_80085A60();
        break;
    case 20:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
