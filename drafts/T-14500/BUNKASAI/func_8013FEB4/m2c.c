? func_8007E5A0();                                  /* extern */
extern u8 D_800E6679;
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_8013FEB4(void) {
    s32 temp_v0_2;
    u32 temp_v0;

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
            func_80062CD0(0x6935);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E81C();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = 0xA;
            func_80083474();
            D_800CA148 = 0x25;
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 7:                                         /* switch 1 */
        func_8007E390();
        break;
    case 8:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 7;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 8;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 9;
                break;
            case 4:                                 /* switch 2 */
                D_800CA148 = 0xA;
                break;
            }
            func_80142694();
        }
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        if (func_8008667C(0xA) == 4) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                D_800CA148 = 1;
                break;
            case 2:                                 /* switch 3 */
                D_800CA148 = 2;
                break;
            case 3:                                 /* switch 3 */
                D_800CA148 = 3;
                break;
            }
            func_8004284C();
        }
        break;
    case 11:                                        /* switch 1 */
        func_80085A60();
        break;
    case 12:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 13:                                        /* switch 1 */
        if (func_8008667C(0xA) == 4) {
            func_80085A60();
        } else {
            func_8004284C();
        }
        break;
    case 14:                                        /* switch 1 */
        func_8007E934();
        break;
    case 15:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:                                        /* switch 1 */
        func_80085A60();
        break;
    case 17:                                        /* switch 1 */
        if (func_8008667C(0xA) != 4) {
            D_800E6679 |= 0x20;
        }
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
