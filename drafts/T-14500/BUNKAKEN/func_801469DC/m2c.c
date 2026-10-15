? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E738A;

void func_801469DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40EA, 1);
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
                D_800CA148 = 0xB;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0xC;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0xD;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        D_800CA148 = 1;
        func_8007E99C();
        break;
    case 7:                                         /* switch 1 */
        func_80085A60();
        break;
    case 8:                                         /* switch 1 */
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 0xE;
            break;
        case 2:                                     /* switch 3 */
            D_800CA148 = 0x10;
            break;
        case 3:                                     /* switch 3 */
            D_800CA148 = 0x11;
            break;
        }
        func_8007E884();
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        if (func_8008667C(6) >= 2) {
            func_80042940(0xE);
        } else {
            func_8004284C();
        }
        break;
    case 11:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 12:                                        /* switch 1 */
        func_8007E884();
        break;
    case 13:                                        /* switch 1 */
        func_80085A60();
        break;
    case 14:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 15:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:                                        /* switch 1 */
        func_80085A60();
        break;
    case 17:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
