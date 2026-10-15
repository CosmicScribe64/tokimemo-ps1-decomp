? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E738A;

void func_80137804(void) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E884();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 2;
                break;
            case 2:                                 /* switch 2 */
            case 3:                                 /* switch 2 */
                D_800CA148 = 4;
                break;
            }
            func_80139DEC();
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
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 3;
            break;
        case 2:                                     /* switch 3 */
        case 3:                                     /* switch 3 */
            D_800CA148 = 5;
            break;
        }
        func_8007E884();
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 11:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
