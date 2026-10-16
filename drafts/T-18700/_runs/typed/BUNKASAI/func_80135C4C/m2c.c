? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */

void func_80135C4C(void) {
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u8 temp_v0;
    u8 temp_v0_5;
    u8 temp_v0_6;

    temp_v0 = D_800E6280.unk_110A;
    switch (temp_v0) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D6E);
        }
        break;
    case 3:                                         /* switch 1 */
        func_80057418(1, func_8005751C(1));
        func_8014338C();
        func_8007C740();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:                                         /* switch 1 */
        func_8007E884();
        break;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            if (D_800E6280.unk_03E == 0x60) {
                temp_v0_2 = func_8008667C(1);
                switch (temp_v0_2) {                /* switch 2 */
                case 0:                             /* switch 2 */
                case 1:                             /* switch 2 */
                    D_800CA148 = 0x26;
                    break;
                case 2:                             /* switch 2 */
                case 3:                             /* switch 2 */
                    D_800CA148 = 0x27;
                    break;
                case 4:                             /* switch 2 */
                    D_800CA148 = 0x28;
                    break;
                }
            } else {
                temp_v0_3 = func_8008667C(1);
                switch (temp_v0_3) {                /* switch 3 */
                case 0:                             /* switch 3 */
                case 1:                             /* switch 3 */
                    var_v1 = 0x22;
block_21:
                    D_800CA148 = var_v1;
                    break;
                case 2:                             /* switch 3 */
                    var_v1 = 0x23;
                    goto block_21;
                case 3:                             /* switch 3 */
                    var_v1 = 0x24;
                    goto block_21;
                case 4:                             /* switch 3 */
                    var_v1 = 0x25;
                    goto block_21;
                }
                if (D_800E6280.unk_03E == 0x61) {
                    D_800CA148 += 7;
                }
            }
            func_80136230();
        }
        break;
    case 6:                                         /* switch 1 */
        func_80085A60();
        break;
    case 7:                                         /* switch 1 */
        if ((D_800E6280.unk_03E == 0x61) && (func_8008667C(1) == 4)) {
            D_800CA148 = 0x3E;
            func_8007E5A0();
        } else {
            func_8007E99C();
        }
        break;
    case 8:                                         /* switch 1 */
        if ((D_800E6280.unk_03E == 0x61) && (func_8008667C(1) == 4)) {
            func_80085A60();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:                                         /* switch 1 */
        func_8007E934();
        break;
    case 10:                                        /* switch 1 */
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = temp_v0 + 1;
        main_state_set(0x80);
        break;
    case 11:                                        /* switch 1 */
        func_800573AC();
        func_80057390(0);
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 12:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D6E);
        }
        break;
    case 13:                                        /* switch 1 */
        func_80057418(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 14:                                        /* switch 1 */
        func_8007E884();
        break;
    case 15:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {                    /* switch 4 */
            case 0:                                 /* switch 4 */
            case 1:                                 /* switch 4 */
                var_v1_2 = 0x2D;
block_48:
                D_800CA148 = var_v1_2;
                break;
            case 2:                                 /* switch 4 */
                var_v1_2 = 0x2F;
                goto block_48;
            case 3:                                 /* switch 4 */
                var_v1_2 = 0x31;
                goto block_48;
            case 4:                                 /* switch 4 */
                var_v1_2 = 0x33;
                goto block_48;
            }
            temp_v0_5 = D_800E6280.unk_03E;
            var_v1_3 = D_800CA148;
            if ((u16) D_800E6280.unk_66C[1].unk_00 < (u32) ((temp_v0_5 * 0x64) - 0x24B8)) {
                var_v1_3 += 1;
            }
            D_800CA148 = var_v1_3;
            if (temp_v0_5 == 0x61) {
                D_800CA148 = var_v1_3 + 8;
            }
            func_80136230();
        }
        break;
    case 16:                                        /* switch 1 */
        func_80085A60();
        break;
    case 17:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 18:                                        /* switch 1 */
        if (func_8008667C(1) == 4) {
            temp_v0_6 = D_800E6280.unk_03E;
            if ((temp_v0_6 < 0x61U) && ((u16) D_800E6280.unk_66C[1].unk_00 >= (u32) ((temp_v0_6 * 0x64) - 0x24B8))) {
                D_800CA148 = 0x3E;
                func_8004284C();
            } else if (temp_v0_6 == 0x61) {
                if ((u16) D_800E6280.unk_66C[1].unk_00 >= 0x12CU) {
                    D_800CA148 = 0x3E;
                    func_8004284C();
                } else {
                    D_800CA148 = 0x3F;
                    func_8004284C();
                }
            } else {
                func_80042940(0x14);
            }
        } else {
            func_80042940(0x14);
        }
        break;
    case 19:                                        /* switch 1 */
        func_80085A60();
        break;
    case 20:                                        /* switch 1 */
        func_8007E934();
        break;
    case 21:                                        /* switch 1 */
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 22:                                        /* switch 1 */
        func_80085A60();
        break;
    case 23:                                        /* switch 1 */
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
