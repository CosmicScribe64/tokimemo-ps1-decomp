? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
extern u8 D_800E62BE;
extern s16 D_800E647E;
extern s32 D_800E6480;
extern u8 D_800E6482;
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80158764(void) {
    s16 temp_a1;
    s16 var_a1;
    s16 var_a1_2;
    s16 var_a1_3;
    s16 var_a1_4;
    s16 var_a1_5;
    s16 var_a1_6;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_6;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_5;

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
        var_a1 = 0x2F;
        if (D_800E62BE == 0x5F) {
            var_a1 = 0x2E;
        }
        D_800CA148 = var_a1;
        func_8004284C();
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        func_8007E934();
        break;
    case 7:                                         /* switch 1 */
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 8:                                         /* switch 1 */
        func_800866A0(0x5D41);
        break;
    case 9:                                         /* switch 1 */
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E71DF = 1;
        func_80083474();
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        break;
    case 10:                                        /* switch 1 */
        func_8007E884();
        break;
    case 11:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(1);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                var_a1_2 = 1;
block_24:
                D_800CA148 = var_a1_2;
                break;
            case 2:                                 /* switch 2 */
                var_a1_2 = 2;
                goto block_24;
            case 3:                                 /* switch 2 */
                var_a1_2 = 3;
                goto block_24;
            case 4:                                 /* switch 2 */
                var_a1_2 = 4;
                goto block_24;
            }
            func_80158E3C();
        }
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        var_a1_3 = 0x8D;
        if (func_8008667C(1) == 4) {
            var_a1_3 = 9;
        }
        D_800CA148 = var_a1_3;
        func_8004284C();
        break;
    case 14:                                        /* switch 1 */
        func_80085A60();
        break;
    case 15:                                        /* switch 1 */
        func_8004284C();
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {                        /* switch 3 */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 0x8E;
            break;
        case 2:                                     /* switch 3 */
            D_800CA148 = 0x8F;
            break;
        case 3:                                     /* switch 3 */
            D_800CA148 = 0x90;
            break;
        case 4:                                     /* switch 3 */
            func_80042940(0x15);
            break;
        }
        temp_a1 = D_800CA148;
        if (D_800E62BE == 0x60) {
            D_800CA148 = temp_a1 + 0x18;
        } else {
            D_800CA148 = temp_a1;
            if (D_800E62BE == 0x61) {
                D_800CA148 = temp_a1 + 0x1B;
            }
        }
        break;
    case 16:                                        /* switch 1 */
        func_80085A60();
        break;
    case 17:                                        /* switch 1 */
        if (!(D_800E647E & 3)) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:                                 /* switch 4 */
            case 1:                                 /* switch 4 */
                D_800CA148 = 0xA3;
                break;
            case 2:                                 /* switch 4 */
                D_800CA148 = 0xA4;
                break;
            case 3:                                 /* switch 4 */
                D_800CA148 = 0xA5;
                break;
            }
        } else {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {                    /* switch 5; irregular */
            default:                                /* switch 5 */
                var_a1_4 = 0x93;
                if (temp_v0_4 != 3) {
                    var_a1_4 = D_800CA148;
                }
                break;
            case 0:                                 /* switch 5 */
            case 1:                                 /* switch 5 */
                var_a1_4 = 0x91;
                break;
            case 2:                                 /* switch 5 */
                var_a1_4 = 0x92;
                break;
            }
            temp_v0_5 = (u8) D_800E6482 >> 4;
            if (temp_v0_5 == 1) {
                var_a1_4 += 3;
            }
            D_800CA148 = var_a1_4;
            if (D_800E62BE == 0x61) {
                D_800CA148 = var_a1_4;
                if (temp_v0_5 == 0) {
                    D_800CA148 = var_a1_4 + 0x1B;
                }
            }
        }
        func_8004284C();
        break;
    case 18:                                        /* switch 1 */
        func_80085A60();
        break;
    case 19:                                        /* switch 1 */
        temp_v0_6 = func_8008667C(1);
        switch (temp_v0_6) {                        /* switch 6; irregular */
        default:                                    /* switch 6 */
            var_a1_5 = 0x99;
            if (temp_v0_6 != 3) {
                var_a1_5 = D_800CA148;
            }
            break;
        case 0:                                     /* switch 6 */
        case 1:                                     /* switch 6 */
            var_a1_5 = 0x97;
            break;
        case 2:                                     /* switch 6 */
            var_a1_5 = 0x98;
            break;
        }
        if (!(D_800E6480 & 4)) {
            var_a1_5 += 3;
        }
        if (((u8) D_800E6482 >> 4) == 1) {
            D_800CA148 = var_a1_5 + 6;
        } else {
            D_800CA148 = var_a1_5;
            if (D_800E62BE == 0x61) {
                D_800CA148 = var_a1_5 + 0x18;
            }
        }
        func_8004284C();
        if (!(D_800E647E & 3)) {
            func_80042940(0x15);
        }
        break;
    case 20:                                        /* switch 1 */
        func_80085A60();
        break;
    case 21:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 22:                                        /* switch 1 */
        if (func_8008667C(1) == 4) {
            D_800CA148 = 0xB5;
            func_8004284C();
        } else if (!(D_800E647E & 3)) {
            var_a1_6 = 0xC;
            if (func_8008667C(1) < 2) {
                var_a1_6 = 0xD;
            }
            D_800CA148 = var_a1_6;
            func_8004284C();
        } else {
            func_80042940(0x18);
        }
        break;
    case 23:                                        /* switch 1 */
        func_80085A60();
        break;
    case 24:                                        /* switch 1 */
        func_8007E934();
        break;
    case 25:                                        /* switch 1 */
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 26:                                        /* switch 1 */
        func_80085A60();
        break;
    case 27:                                        /* switch 1 */
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
