? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_801464A8(void) {
    u32 temp_v0;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x4118, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_8007C740();
            D_800E71DF = 3;
            func_80083474();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:                                         /* switch 1 */
        func_8007E884();
        break;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                D_800CA148 = 0xC;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0xD;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0xE;
                break;
            case 4:                                 /* switch 2 */
                D_800CA148 = 0xF;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:                                         /* switch 1 */
        func_80085A60();
        break;
    case 7:                                         /* switch 1 */
        func_8007E5A0();
        break;
    case 8:                                         /* switch 1 */
        if (func_8008667C(3) == 4) {
            D_800CA148 = 0x45;
            func_8004284C();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        func_8007E934();
        break;
    case 11:                                        /* switch 1 */
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
