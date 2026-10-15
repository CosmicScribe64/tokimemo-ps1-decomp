? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80143C38(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E81C();
        break;
    case 4:                                         /* switch 1 */
        D_800CA148 = 0x26;
        func_8004284C();
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        func_8007E934();
        break;
    case 7:                                         /* switch 1 */
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:                                         /* switch 1 */
        func_800866A0(0x5B25);
        break;
    case 9:                                         /* switch 1 */
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E71DF = 7;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 10:                                        /* switch 1 */
        func_8007E884();
        break;
    case 11:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 0xD;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0xE;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0xF;
                break;
            case 4:                                 /* switch 2 */
                D_800CA148 = 0x10;
                break;
            }
            func_80145FC0();
        }
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        if (func_8008667C(7) == 4) {
            func_8007E5A0();
        } else {
            func_8004284C();
        }
        break;
    case 14:                                        /* switch 1 */
        temp_v0_2 = func_8008667C(7);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 1;
            break;
        case 2:                                     /* switch 3 */
            D_800CA148 = 2;
            break;
        case 3:                                     /* switch 3 */
            D_800CA148 = 4;
            break;
        }
        func_8004284C();
        break;
    case 15:                                        /* switch 1 */
        func_80085A60();
        break;
    case 16:                                        /* switch 1 */
        if (func_8008667C(7) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 17:                                        /* switch 1 */
        func_8007E934();
        break;
    case 18:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:                                        /* switch 1 */
        func_80085A60();
        break;
    case 20:                                        /* switch 1 */
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
