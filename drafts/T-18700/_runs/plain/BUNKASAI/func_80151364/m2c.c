? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
extern u8 D_800E62BE;
extern u16 D_800E68EC;
extern s8 D_800E71DF;
extern u8 D_800E738A;

void func_80151364(void) {
    s16 var_v1;
    s16 var_v1_2;
    u32 temp_v0;

    switch (D_800E738A) {                           /* switch 1 */
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
            D_800E71DF = 0;
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
            if (D_800E62BE == 0x60) {
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
            if ((u16) D_800E68EC >= (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_2 = 0xC;
            } else {
                var_v1_2 = 0xD;
            }
        } else {
            var_v1_2 = 0xB;
            if ((u16) D_800E68EC >= (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_2 = 0xA;
            }
        }
        D_800CA148 = var_v1_2;
        if (D_800E62BE == 0x60) {
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
