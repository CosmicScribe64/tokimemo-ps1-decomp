? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_80151EC0(void) {
    s32 temp_v0;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x4118, 1);
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
                D_800CA148 = 0x2C;
                break;
            case 2:                                 /* switch 2 */
                D_800CA148 = 0x2E;
                break;
            case 3:                                 /* switch 2 */
                D_800CA148 = 0x2C;
                break;
            }
            func_80152618();
            func_80083440(0U);
            func_80063668();
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        func_8007E5A0();
        break;
    case 7:                                         /* switch 1 */
        D_800E71DF = 0xE;
        func_8004284C();
        break;
    case 8:                                         /* switch 1 */
        func_80085A60();
        break;
    case 9:                                         /* switch 1 */
        func_8007E934();
        break;
    case 10:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 11:                                        /* switch 1 */
        func_80085A60();
        break;
    case 12:                                        /* switch 1 */
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
