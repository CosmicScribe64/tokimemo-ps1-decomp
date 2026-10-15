? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80148094(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E738A) {                           /* switch 1 */
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
                D_800CA148 = 0x54;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x55;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x56;
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
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:                                         /* switch 1 */
        func_800866A0(0x622D);
        break;
    case 9:                                         /* switch 1 */
        func_8007C740();
        D_800E71DF = 6;
        func_80083474();
        func_80083440(3U);
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:                                        /* switch 1 */
        func_8007E81C();
        break;
    case 11:                                        /* switch 1 */
        D_800CA148 = 0x47;
        func_8004284C();
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        func_8007E390();
        break;
    case 14:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 0x57;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 0x58;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 0x59;
                break;
            }
            func_80148E68();
        }
        break;
    case 15:                                        /* switch 1 */
        func_80085A60();
        break;
    case 16:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 17:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:                                        /* switch 1 */
        func_80085A60();
        break;
    case 19:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
