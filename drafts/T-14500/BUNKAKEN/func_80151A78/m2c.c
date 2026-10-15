? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern s8 D_800E69A2;
extern s8 D_800E71DF;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80151A78(void) {
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
                D_800CA148 = 0x26;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x27;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x28;
                break;
            }
            func_80152618();
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        func_8007E5A0();
        break;
    case 7:                                         /* switch 1 */
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = (D_800E7D4F | 2) & 0xFFE3;
        func_80042908(0x8D);
        break;
    case 8:                                         /* switch 1 */
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 9:                                         /* switch 1 */
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_80084E4C();
        func_800847B8(9U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 10:                                        /* switch 1 */
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 11:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 12:                                        /* switch 1 */
        func_800866A0(0x64D0);
        break;
    case 13:                                        /* switch 1 */
        func_8007C740();
        D_800E71DF = 9;
        func_80083474();
        if (func_8008667C(9) < 2) {
            func_80083440(0U);
        }
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 14:                                        /* switch 1 */
        func_8007E884();
        break;
    case 15:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 0x29;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 0x2A;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 0x2B;
                break;
            }
            func_80152618();
        }
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
        D_800E7D4F |= 1;
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
