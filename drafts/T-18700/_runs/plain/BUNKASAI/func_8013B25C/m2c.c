? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E62BE;
extern s8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_8013B25C(void) {
    s16 temp_v1;
    s16 var_v1;
    s16 var_v1_2;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40FC, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x525B);
        }
        break;
    case 3:                                         /* switch 1 */
        func_80057418(1, func_8005751C(1));
        func_801431D4();
        func_8007C740();
        D_800E71DF = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 4:                                         /* switch 1 */
        func_8007E884();
        break;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                var_v1 = 2;
block_15:
                D_800CA148 = var_v1;
                break;
            case 2:                                 /* switch 2 */
                var_v1 = 3;
                goto block_15;
            case 3:                                 /* switch 2 */
                var_v1 = 4;
                goto block_15;
            case 4:                                 /* switch 2 */
                var_v1 = 5;
                goto block_15;
            }
            D_800CA148 = (D_800CA148 + (D_800E62BE * 4)) - 0x17C;
            func_8013C3C4();
        }
        break;
    case 6:                                         /* switch 1 */
        func_80085A60();
        break;
    case 7:                                         /* switch 1 */
        if ((D_800E62BE == 0x5F) && (func_8008667C(2) == 4)) {
            D_800CA148 = 0x45;
            func_8007E5A0();
        } else {
            func_8007E99C();
        }
        break;
    case 8:                                         /* switch 1 */
        if ((D_800E62BE == 0x5F) && (func_8008667C(2) == 4)) {
            func_80085A60();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:                                         /* switch 1 */
        func_8007E934();
        break;
    case 10:                                        /* switch 1 */
        D_80122EAC = 3;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 11:                                        /* switch 1 */
        func_800866A0(0x525B);
        break;
    case 12:                                        /* switch 1 */
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E71DF = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 13:                                        /* switch 1 */
        func_8007E884();
        break;
    case 14:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {                    /* switch 3 */
            case 0:                                 /* switch 3 */
            case 1:                                 /* switch 3 */
                var_v1_2 = 0xE;
block_38:
                D_800CA148 = var_v1_2;
                break;
            case 2:                                 /* switch 3 */
                var_v1_2 = 0xF;
                goto block_38;
            case 3:                                 /* switch 3 */
                var_v1_2 = 0x10;
                goto block_38;
            case 4:                                 /* switch 3 */
                var_v1_2 = 0x11;
                goto block_38;
            }
            temp_v1 = (D_800CA148 + (D_800E62BE * 4)) - 0x17C;
            D_800CA148 = temp_v1;
            if (D_800E62BE == 0x5F) {
                D_800CA148 = temp_v1 + 0xC;
            }
            func_8013C3C4();
        }
        break;
    case 15:                                        /* switch 1 */
        func_80085A60();
        break;
    case 16:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 17:                                        /* switch 1 */
        func_8007E934();
        break;
    case 18:                                        /* switch 1 */
        D_800CA148 = 1;
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
