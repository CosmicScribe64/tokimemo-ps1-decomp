? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E62BE;
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80151738(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        if (D_80122EB8 == 2) {
            func_8005742C(0x40FC, 1);
        } else {
            func_8005742C(0x4105, 1);
        }
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E99C();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E884();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 0x1A;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x1B;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x1C;
                break;
            }
            if (D_80122EB8 == 3) {
                D_800CA148 += 9;
            }
            func_80152618();
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
        func_800866A0(0x64D0);
        break;
    case 9:                                         /* switch 1 */
        func_8007C740();
        D_800E71DF = 9;
        func_80083474();
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 10:                                        /* switch 1 */
        func_8007E884();
        break;
    case 11:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 0x1D;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 0x1E;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 0x1F;
                break;
            }
            if (D_800E62BE == 0x61) {
                D_800CA148 += 3;
            }
            func_80152618();
        }
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 14:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:                                        /* switch 1 */
        func_80085A60();
        break;
    case 16:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
