? func_8007E5A0();                                  /* extern */
extern u8 D_800E62BE;
extern u8 D_800E65D1;
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_801438E4(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x5B25);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E81C();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = 7;
            func_80083474();
            if (D_800E62BE == 0x5F) {
                D_800CA148 = 0x24;
            } else {
                D_800CA148 = 0x25;
            }
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        func_8004284C();
        break;
    case 7:                                         /* switch 1 */
        func_8007E390();
        break;
    case 8:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 5;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 6;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 7;
                break;
            case 4:                                 /* switch 2 */
                D_800CA148 = 8;
                break;
            }
            if (D_800E62BE == 0x60) {
                D_800CA148 += 4;
            }
            func_80145FC0();
        }
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        temp_v0_2 = func_8008667C(7);
        switch (temp_v0_2) {                        /* switch 3 */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 1;
            break;
        case 2:                                     /* switch 3 */
            D_800CA148 = 2;
            break;
        case 3:                                     /* switch 3 */
            D_800CA148 = 3;
            break;
        case 4:                                     /* switch 3 */
            func_8004284C();
            break;
        }
        func_8004284C();
        break;
    case 11:                                        /* switch 1 */
        func_80085A60();
        break;
    case 12:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 13:                                        /* switch 1 */
        func_8007E934();
        break;
    case 14:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:                                        /* switch 1 */
        func_80085A60();
        break;
    case 16:                                        /* switch 1 */
        if (func_8008667C(7) != 4) {
            if (D_800E62BE == 0x5F) {
                D_800E65D1 |= 0x10;
            } else {
                D_800E65D1 |= 0x20;
            }
        }
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
