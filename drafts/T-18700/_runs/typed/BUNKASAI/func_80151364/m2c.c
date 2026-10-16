? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */

void func_80151364(void) {
    s16 var_v1;
    s16 var_v1_2;
    u32 temp_v0;
    u8 temp_t6;
    u8 var_v0;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8007E934();
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_8007C740();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:                                         /* switch 1 */
        func_8007E81C();
        break;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {                      /* switch 2 */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                var_v1 = 2;
block_16:
                D_800CA148 = var_v1;
                break;
            case 2:                                 /* switch 2 */
                var_v1 = 4;
                goto block_16;
            case 3:                                 /* switch 2 */
                var_v1 = 6;
                goto block_16;
            case 4:                                 /* switch 2 */
                var_v1 = 8;
                goto block_16;
            }
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 += 0xC;
            }
            func_80153FA4();
        }
        break;
    case 6:                                         /* switch 1 */
        func_8007E390();
        break;
    case 7:                                         /* switch 1 */
        func_80085A60();
        break;
    case 8:                                         /* switch 1 */
        func_80085A60();
        break;
    case 9:                                         /* switch 1 */
        func_8007E99C();
        D_800CA148 = 0;
        break;
    case 10:                                        /* switch 1 */
        func_80085A60();
        break;
    case 11:                                        /* switch 1 */
        func_8007E884();
        break;
    case 12:                                        /* switch 1 */
        if (func_8008667C(0) == 4) {
            var_v0 = D_800E6280.unk_03E;
            if ((u16) D_800E6280.unk_66C[0].unk_00 >= (u32) ((var_v0 * 0x64) - 0x24B8)) {
                var_v1_2 = 0xC;
            } else {
                var_v1_2 = 0xD;
            }
        } else {
            var_v0 = D_800E6280.unk_03E;
            var_v1_2 = 0xB;
            if ((u16) D_800E6280.unk_66C[0].unk_00 >= (u32) ((var_v0 * 0x64) - 0x24B8)) {
                var_v1_2 = 0xA;
            }
        }
        D_800CA148 = var_v1_2;
        if (var_v0 == 0x60) {
            D_800CA148 = var_v1_2 + 0xC;
        }
        func_8004284C();
        break;
    case 13:                                        /* switch 1 */
        func_80085A60();
        break;
    case 14:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 15:                                        /* switch 1 */
        func_8007E934();
        break;
    case 16:                                        /* switch 1 */
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:                                        /* switch 1 */
        func_80085A60();
        break;
    case 18:                                        /* switch 1 */
        func_80142BC8();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
