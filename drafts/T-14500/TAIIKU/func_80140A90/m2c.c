? func_8007E5A0();                                  /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C();                                  /* extern */
? func_8007EAAC();                                  /* extern */
? func_80083174();                                  /* extern */
? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */
extern s16 D_800E6476;
extern s16 D_800E647A;
extern s16 D_800E647E;
extern s8 D_800E71DF;
extern u8 D_800E738A;
extern s32 D_800E7510;
extern s16 D_8012069C;
extern u8 D_8014A064;

s32 func_80140A90(void) {
    u8 *var_v1;
    u8 temp_t6;
    u8 temp_t8;
    u8 temp_t9;

    func_80083808();
    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        D_80122EA0 = 0;
        func_8004E58C();
        func_80048390();
        D_800E7510 = -1;
        func_80048E78();
        func_8005742C(0x413C, 1);
        D_8014A064 = 0;
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8005751C(1);
            func_80062CD0(0x5F5D);
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
        func_8007EAAC();
        break;
    case 3:                                         /* switch 1 */
        func_8006B900();
        if (cd_read_sync_status() & 1) {
            func_8004E9F4(0);
            func_8006612C((s32) "グラウンド");
            func_8004E884(1);
            func_8007C740();
            D_800E71DF = 1;
            func_80083474();
        }
        break;
    case 4:                                         /* switch 1 */
        func_8006B900();
        cd_read_sectors(1U, 0x801E8000, 0xB7BE);
        func_8004284C();
        break;
    case 5:                                         /* switch 1 */
        func_8006B900();
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(1U);
            func_80141B70();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8014A0A0;
            D_800CA164 = D_8014A0A4;
            D_800CA168 = D_8014A0A8;
            func_8004284C();
        }
        break;
    case 6:                                         /* switch 1 */
    case 8:                                         /* switch 1 */
    case 12:                                        /* switch 1 */
    case 14:                                        /* switch 1 */
        func_80085A60();
        func_8006B900();
        break;
    case 7:                                         /* switch 1 */
        func_8007E390();
        func_8006B900();
        break;
    case 9:                                         /* switch 1 */
        func_80083174();
        func_8006B900();
        break;
    case 10:                                        /* switch 1 */
        switch (D_80122CDC) {                       /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            func_80042940(0x11);
            break;
        case 1:                                     /* switch 2 */
            func_80042940(0xB);
            break;
        }
        func_8006B900();
        break;
    case 11:                                        /* switch 1 */
        D_800E647A -= 1;
        D_800E6476 -= 1;
        D_800E647E += 0xA;
        func_80083440(3U);
        func_80063668();
        D_800CA148 = 0xA;
        D_800CA14C = 0;
        func_8006B900();
        func_8004284C();
        break;
    case 13:                                        /* switch 1 */
        func_8006B900();
        func_8007E5A0();
        break;
    case 15:                                        /* switch 1 */
        func_8007E934();
        break;
    case 16:                                        /* switch 1 */
        D_8014A064 = 2;
        break;
    case 17:                                        /* switch 1 */
        func_80085A60();
        func_8006B900();
        break;
    case 18:                                        /* switch 1 */
        func_8006B900();
        func_8005742C(0x407A, 1);
        func_8004284C();
        break;
    case 19:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 20:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8004E9F4(0);
            func_8006612C((s32) "校舎裏");
            D_80125CA0 = 0xC0;
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x45U, 0x80197000, 0xBB15);
            func_8004500C(0, 0x201);
            func_8004284C();
        }
        break;
    case 21:                                        /* switch 1 */
        func_8007E884();
        break;
    case 22:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80142180();
            func_80043914(D_8014A0F0, 0x11, 1, 2, 0);
            func_80084E90(D_8014A0F4, D_8014A0F8, D_8014A0FC, D_8014A100, D_8014A104, D_8014A108);
            func_800850D4(D_8014A0E4, D_8014A0E8, D_8014A0E0, (s32) D_8014A0EC);
            var_v1 = D_8011ECD0;
            do {
                temp_t8 = *(u8 *)((unsigned char *)var_v1 + (6532));
                var_v1 += 0x44;
                *(s8 *)((unsigned char *)var_v1 + (6464)) = (s8) (temp_t8 | 8);
            } while (var_v1 != D_8011ED58);
            M2C_MEMCPY_ALIGNED(D_801206D8, D_80120650, 0x44);
            D_801206EE = 4;
            func_8004284C();
        }
        break;
    case 23:                                        /* switch 1 */
        func_80085A60();
        break;
    case 24:                                        /* switch 1 */
        func_80047550();
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) "ドシン", 0);
        func_80044750(0x60A);
        func_80085ED0();
        func_8004284C();
        break;
    case 25:                                        /* switch 1 */
        if (func_80085EEC() >= 0x3D) {
            func_8005742C(0x4721, 1);
            func_8004E9F4(1);
            func_8004284C();
        }
        break;
    case 26:                                        /* switch 1 */
        func_8007E99C();
        break;
    case 27:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 28:                                        /* switch 1 */
        func_80085368();
        D_801206DB |= 0x80;
        temp_t9 = (D_801206DF + 4) & 0xFF;
        D_801206DF = temp_t9;
        if (temp_t9 >= 0x80U) {
            D_801206DF = 0x80;
        }
        break;
    case 29:                                        /* switch 1 */
        func_80085A60();
        if (func_8004ECB4() != 0) {
            D_801206DA = 0;
            D_801206F0 = 0;
        } else {
            D_801206DA = 1;
        }
        break;
    case 30:                                        /* switch 1 */
        func_80085A60();
        break;
    case 31:                                        /* switch 1 */
        D_801206EE = 5;
        func_8004284C();
        break;
    case 32:                                        /* switch 1 */
        func_80085A60();
        if (func_8004ECB4() != 0) {
            D_801206DA = 0;
            D_801206F0 = 0;
        } else {
            D_801206DA = 1;
        }
        break;
    case 33:                                        /* switch 1 */
        D_80120666 = 3;
        D_801206AA = 1;
        D_801206AC = 0;
        D_8012069C = 0;
        func_8004284C();
        break;
    case 34:                                        /* switch 1 */
        func_80085A60();
        break;
    case 35:                                        /* switch 1 */
        func_80085A60();
        if (func_8004ECB4() != 0) {
            D_801206DA = 0;
            D_801206F0 = 0;
        } else {
            D_801206DA = 1;
        }
        break;
    case 36:                                        /* switch 1 */
        func_80085A60();
        break;
    case 37:                                        /* switch 1 */
        func_800853FC();
        temp_t6 = (D_801206DF - 4) & 0xFF;
        D_801206DF = temp_t6;
        if (temp_t6 < 3U) {
            D_801206DF = 0;
        }
        break;
    case 38:                                        /* switch 1 */
        D_8014A064 = 1;
        func_80085B3C(2, 0xD);
        D_800E647A += 2;
        D_800E6476 += 1;
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_80086424();
    return (s32) D_8014A064;
}
