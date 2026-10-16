? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */

void func_8014A0E0(void) {
    switch (D_800E6280.unk_110A) {                  /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8004E9F4(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        func_800573AC();
        func_80057390(0);
        D_800E6280.unk_71E |= 4;
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
                    D_800E6280.unk_F5F = 0;
                    func_800847B8(0U);
                } else {
                    D_800E6280.unk_F5F = (u8) D_80122EBC;
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
            func_80083440(func_80051B48(D_800E6280.unk_F5F) & 0x7F);
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
                D_800E6280.unk_F5F = 0;
                func_800847B8(0U);
            } else {
                D_800E6280.unk_F5F = (u8) D_80122EBC;
                func_800847B8((u8) D_80122EBC);
            }
            D_800CA148 = 1;
            D_800CA14C = 0;
            switch (D_80122EBC) {                   /* switch 2 */
            case 11:                                /* switch 2 */
                D_800CA160 = D_801562A0[0][1];
                D_800CA164 = D_801562A0[1][1];
                D_800CA168 = D_801562A0[2][1];
                break;
            case 4:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][0xA];
                D_800CA164 = D_801562A0[1][0xA];
                D_800CA168 = D_801562A0[2][0xA];
                break;
            case 5:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][7];
                D_800CA164 = D_801562A0[1][7];
                D_800CA168 = D_801562A0[2][7];
                break;
            case 6:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][8];
                D_800CA164 = D_801562A0[1][8];
                D_800CA168 = D_801562A0[2][8];
                break;
            case 7:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][5];
                D_800CA164 = D_801562A0[1][5];
                D_800CA168 = D_801562A0[2][5];
                break;
            case 8:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][0];
                D_800CA164 = D_801562A0[1][0];
                D_800CA168 = D_801562A0[2][0];
                break;
            case 9:                                 /* switch 2 */
                D_800CA160 = D_801562A0[0][9];
                D_800CA164 = D_801562A0[1][9];
                D_800CA168 = D_801562A0[2][9];
                break;
            case 10:                                /* switch 2 */
                D_800CA160 = D_801562A0[0][0xC];
                D_800CA164 = D_801562A0[1][0xC];
                D_800CA168 = D_801562A0[2][0xC];
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
