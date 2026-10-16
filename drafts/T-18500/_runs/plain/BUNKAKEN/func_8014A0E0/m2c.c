? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern s32 D_801562A4;
extern s32 D_801562B4;
extern s32 D_801562BC;
extern s32 D_801562C0;
extern s32 D_801562C4;
extern s32 D_801562C8;
extern s32 D_801562D0;
extern s32 D_801562D4;
extern s32 D_801562D8;
extern s32 D_801562E8;
extern s32 D_801562F0;
extern s32 D_801562F4;
extern s32 D_801562F8;
extern s32 D_801562FC;
extern s32 D_80156304;
extern s32 D_80156308;
extern s32 D_8015630C;
extern s32 D_8015631C;
extern s32 D_80156324;
extern s32 D_80156328;
extern s32 D_8015632C;
extern s32 D_80156330;
extern s32 D_80156338;

void func_8014A0E0(void) {
    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8004E9F4(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        func_800573AC();
        func_80057390(0);
        D_800E699E |= 4;
        func_80044750(0xCF);
        func_80085ED0();
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 2:                                         /* switch 1 */
        if (func_80085EEC() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                func_80044890(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                if (D_80122EBC == 0xB) {
                    D_800E71DF = 0;
                    func_800847B8(0U);
                } else {
                    D_800E71DF = (u8) D_80122EBC;
                    func_800847B8((u8) D_80122EBC);
                }
                func_8013257C();
                D_800CA148 = 0x25;
                D_800CA14C = 0;
                D_800CA160 = (s32) D_80155E90;
                D_800CA164 = (s32) D_80155E94;
                D_800CA168 = (s32) D_80155E98;
                func_8004284C();
            }
        }
        break;
    case 3:                                         /* switch 1 */
        func_80085A60();
        break;
    case 4:                                         /* switch 1 */
        if (func_80044E8C() == 1) {
            func_8005742C(0x42D8, 1);
            func_8004284C();
        }
        break;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80083440(func_80051B48(D_800E71DF) & 0x7F);
            func_80044750(0x200);
            func_8004E9F4(0);
            func_8006612C((s32) "近所の公園");
            D_80125CA0 = 0xC0;
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
            func_8004284C();
        }
        break;
    case 6:                                         /* switch 1 */
        func_8007E884();
        break;
    case 7:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80148ED0();
            if (D_80122EBC == 0xB) {
                D_800E71DF = 0;
                func_800847B8(0U);
            } else {
                D_800E71DF = (u8) D_80122EBC;
                func_800847B8((u8) D_80122EBC);
            }
            D_800CA148 = 1;
            D_800CA14C = 0;
            switch (D_80122EBC) {                   /* switch 2 */
            case 11:                                /* switch 2 */
                D_800CA160 = D_801562A4;
                D_800CA164 = D_801562D8;
                D_800CA168 = D_8015630C;
                break;
            case 4:                                 /* switch 2 */
                D_800CA160 = D_801562C8;
                D_800CA164 = D_801562FC;
                D_800CA168 = D_80156330;
                break;
            case 5:                                 /* switch 2 */
                D_800CA160 = D_801562BC;
                D_800CA164 = D_801562F0;
                D_800CA168 = D_80156324;
                break;
            case 6:                                 /* switch 2 */
                D_800CA160 = D_801562C0;
                D_800CA164 = D_801562F4;
                D_800CA168 = D_80156328;
                break;
            case 7:                                 /* switch 2 */
                D_800CA160 = D_801562B4;
                D_800CA164 = D_801562E8;
                D_800CA168 = D_8015631C;
                break;
            case 8:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][0];
                D_800CA164 = D_801562D4;
                D_800CA168 = D_80156308;
                break;
            case 9:                                 /* switch 2 */
                D_800CA160 = D_801562C4;
                D_800CA164 = D_801562F8;
                D_800CA168 = D_8015632C;
                break;
            case 10:                                /* switch 2 */
                D_800CA160 = D_801562D0;
                D_800CA164 = D_80156304;
                D_800CA168 = D_80156338;
                break;
            default:                                /* switch 2 */
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 8:                                         /* switch 1 */
        func_80085A60();
        break;
    case 9:                                         /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 11:                                        /* switch 1 */
        func_80085B3C(0x14, 0x19);
        func_80145530();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
