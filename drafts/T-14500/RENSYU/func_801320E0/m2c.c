extern u16 D_800E6374;
extern ? D_800E68EE;
extern s32 D_800E7374;
extern u8 D_800E738A;

void func_801320E0(void) {
    u32 temp_t4;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_80044750(0xBF);
        func_80044890(0, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_8008585C();
        func_80064E84();
        func_80064FA4(0);
        func_8006764C(0);
        func_80063930(0);
        func_800649D4();
        func_8004E9F4(0);
        func_8006612C((s32) "練習試合");
        func_80065F34(0);
        func_800573AC();
        func_80057390(0);
        func_800AE120(D_800E7374);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        if (func_80044E8C() == 1) {
            func_80044750(0x206);
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
        cd_read_sectors(7U, 0x801E8000, 0xB037);
        func_8004284C();
        break;
    case 3:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_800847B8(0U);
            func_80084E4C();
            func_80132000();
            D_800CA160 = D_801342B0;
            D_800CA164 = D_801342B4;
            temp_t4 = (u16) D_800E6374 >> 0xC;
            D_800CA168 = D_801342B8;
            switch (temp_t4) {                      /* switch 2 */
            case 6:                                 /* switch 2 */
                D_800CA148 = 0;
                break;
            case 7:                                 /* switch 2 */
                D_800CA148 = 1;
                break;
            case 8:                                 /* switch 2 */
                D_800CA148 = 2;
                break;
            case 9:                                 /* switch 2 */
                D_800CA148 = 3;
                break;
            case 10:                                /* switch 2 */
                D_800CA148 = 4;
                break;
            default:                                /* switch 2 */
                func_80046500();
                break;
            }
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 4:                                         /* switch 1 */
    case 6:                                         /* switch 1 */
        func_80085A60();
        break;
    case 5:                                         /* switch 1 */
        D_800CA148 = (*(&D_800E68EE + (((u16) D_800E6374 >> 0xC) * 4)) & 0xF) + 5;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 7:                                         /* switch 1 */
        func_80042808();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_80086424();
}
