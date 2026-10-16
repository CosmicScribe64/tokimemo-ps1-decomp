? func_8007E5A0();                                  /* extern */
? func_80083174();                                  /* extern */
extern u8 D_800E6448;
extern u8 D_800E644A;
extern u8 D_800E6482;
extern u8 D_800E64BA;
extern u8 D_800E64F2;
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_8013B510(void) {
    s32 temp_a0;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        if (func_8013BD84() != 0) {
            func_8004284C();
            return;
        }
        func_80042908(1);
        return;
    case 1:                                         /* switch 1 */
        func_800866A0(D_80155EA0[D_80122EBC]);
        return;
    case 2:                                         /* switch 1 */
        func_8007C740();
        if (D_80122EBC == 0xB) {
            D_800E71DF = 0;
        } else {
            D_800E71DF = (s8) D_80122EBC;
        }
        func_80083474();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        return;
    case 3:                                         /* switch 1 */
        func_8007E390();
        return;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_a0 = D_80122EBC;
            if (D_80122EBC == 0xB) {
                func_800847B8(0U);
            } else {
                D_80122EBC = D_80122EBC;
                func_800847B8((u8) temp_a0);
            }
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
            return;
        }
    default:                                        /* switch 2 */
        return;
    case 5:                                         /* switch 1 */
        func_80085A60();
        return;
    case 6:                                         /* switch 1 */
        switch (D_80122EBC) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80042940(0x11);
            return;
        case 1:                                     /* switch 2 */
        case 2:                                     /* switch 2 */
        case 3:                                     /* switch 2 */
            func_80042940(0x20);
            return;
        case 4:                                     /* switch 2 */
        case 5:                                     /* switch 2 */
        case 6:                                     /* switch 2 */
        case 7:                                     /* switch 2 */
        case 8:                                     /* switch 2 */
        case 9:                                     /* switch 2 */
        case 10:                                    /* switch 2 */
        case 11:                                    /* switch 2 */
            func_80042940(7);
            return;
        }
        break;
    case 7:                                         /* switch 1 */
        func_80083174();
        return;
    case 8:                                         /* switch 1 */
        if (D_80122CDC != 0) {
            func_80083440(D_80155F3C[D_80122EBC]);
            func_80063668();
            if (D_80122EBC == 0xB) {
                D_80122EA4 = 0;
            } else {
                D_80122EA4 = D_80122EBC;
            }
        }
        func_8004284C();
        return;
    case 9:                                         /* switch 1 */
        func_80085A60();
        return;
    case 10:                                        /* switch 1 */
        if (D_80122CDC == 0) {
            if ((D_80122EBC == 8) || (D_80122EBC == 9)) {
                func_80042940(0xE);
                return;
            }
            func_80042908(3);
            return;
        }
        func_8004284C();
        return;
    case 11:                                        /* switch 1 */
        func_8007E5A0();
        return;
    case 12:                                        /* switch 1 */
        func_80085A60();
        return;
    case 13:                                        /* switch 1 */
        func_80042908(1);
        return;
    case 14:                                        /* switch 1 */
        D_800CA148 = 8;
        func_8004284C();
        return;
    case 15:                                        /* switch 1 */
        func_80085A60();
        return;
    case 16:                                        /* switch 1 */
        func_80042908(3);
        return;
    case 17:                                        /* switch 1 */
        D_800CA148 = ((u8) D_800E644A >> 4) + 1;
        func_8004284C();
        return;
    case 18:                                        /* switch 1 */
        func_80085A60();
        return;
    case 19:                                        /* switch 1 */
        D_800E6448 |= 4;
        D_800CA148 = 7;
        func_8004284C();
        return;
    case 20:                                        /* switch 1 */
        func_80085A60();
        return;
    case 21:                                        /* switch 1 */
        func_80083174();
        return;
    case 22:                                        /* switch 1 */
        if (D_80122CDC != 0) {
            func_80083440(3U);
            func_80063668();
        }
        func_8004284C();
        return;
    case 23:                                        /* switch 1 */
        func_80085A60();
        return;
    case 24:                                        /* switch 1 */
        if (D_80122CDC != 0) {
            func_80042940(0x1C);
            return;
        }
        D_800CA148 = ((u8) D_800E644A >> 4) + 9;
        func_8004284C();
        return;
    case 25:                                        /* switch 1 */
        func_8007E5A0();
        return;
    case 26:                                        /* switch 1 */
        func_80085A60();
        return;
    case 27:                                        /* switch 1 */
        D_80122EB8 = (s32) ((u8) D_800E644A >> 4);
        D_80122EBC = 0xFF;
        func_80042908(4);
        return;
    case 28:                                        /* switch 1 */
        func_8007E5A0();
        return;
    case 29:                                        /* switch 1 */
        D_800CA148 = 0xF;
        D_80122EA4 = 0;
        func_8004284C();
        return;
    case 30:                                        /* switch 1 */
        func_80085A60();
        return;
    case 31:                                        /* switch 1 */
        func_80042908(1);
        return;
    case 32:                                        /* switch 1 */
        switch (D_80122EBC) {                       /* switch 3; irregular */
        case 1:                                     /* switch 3 */
            if (((u8) D_800E6482 >> 4) == 0) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 2:                                     /* switch 3 */
            if (((u8) D_800E64BA >> 4) == 2) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 3:                                     /* switch 3 */
            if (((u8) D_800E64F2 >> 4) == 4) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        default:                                    /* switch 3 */
            func_80046500();
            break;
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_0C.b[0] |= 4;
        func_8004284C();
        return;
    case 33:                                        /* switch 1 */
        func_80085A60();
        return;
    case 34:                                        /* switch 1 */
        D_800CA148 = 3;
        func_8004284C();
        return;
    case 35:                                        /* switch 1 */
        func_80085A60();
        return;
    case 36:                                        /* switch 1 */
        func_80083174();
        return;
    case 37:                                        /* switch 1 */
        if (D_80122CDC != 0) {
            if (D_80122EBC == 2) {
                func_80083440(4U);
            } else {
                func_80083440(3U);
            }
            func_80063668();
        }
        func_8004284C();
        return;
    case 38:                                        /* switch 1 */
        func_80085A60();
        return;
    case 39:                                        /* switch 1 */
        if (D_80122CDC != 0) {
            func_80042940(0x2B);
            return;
        }
        switch (D_80122EBC) {                       /* switch 4; irregular */
        case 1:                                     /* switch 4 */
            if (((u8) D_800E6482 >> 4) == 0) {
                D_800CA148 = 5;
            } else {
                D_800CA148 = 6;
            }
            break;
        case 2:                                     /* switch 4 */
            if (((u8) D_800E64BA >> 4) == 2) {
                D_800CA148 = 5;
            } else {
                D_800CA148 = 6;
            }
            break;
        case 3:                                     /* switch 4 */
            if (((u8) D_800E64F2 >> 4) == 4) {
                D_800CA148 = 5;
            } else {
                D_800CA148 = 6;
            }
            break;
        default:                                    /* switch 4 */
            func_80046500();
            break;
        }
        func_8004284C();
        return;
    case 40:                                        /* switch 1 */
        func_8007E5A0();
        return;
    case 41:                                        /* switch 1 */
        func_80085A60();
        return;
    case 42:                                        /* switch 1 */
        D_80122EB8 = (s32) ((u8) *(&D_800E644A + (D_80122EBC * 0x38)) >> 4);
        D_80122EBC = 0xFF;
        func_80042908(4);
        return;
    case 43:                                        /* switch 1 */
        func_8007E5A0();
        return;
    case 44:                                        /* switch 1 */
        D_800CA148 = 7;
        D_80122EA4 = D_80122EBC;
        func_8004284C();
        return;
    case 45:                                        /* switch 1 */
        func_80085A60();
        return;
    case 46:                                        /* switch 1 */
        func_80042908(1);
        return;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
