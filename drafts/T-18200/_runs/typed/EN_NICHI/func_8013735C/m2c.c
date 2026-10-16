? func_80085ED0(s16 *, s16, s16, s16);              /* extern */
s32 func_80085EEC();                                /* extern */
extern u8 D_80139C30;

void func_8013735C(void) {
    DisplayActor44 *temp_v0_2;
    DisplayActor44 *temp_v1;
    DisplayActor44 *temp_v1_2;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s16 *temp_a0;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_a1_3;
    s16 temp_a1_4;
    s16 temp_a2;
    s16 temp_a3;
    s16 temp_v0;
    s16 var_a1;
    s16 var_a2;
    s16 var_a3;
    s16 var_t0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_v0_3;
    s32 var_at;
    s32 var_v1;
    u8 temp_t4;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_v1_3;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8006BA40();
        *(s16 *)((unsigned char *)D_8011ECD0 + (7042)) = (s16) D_8011ECD0->unk_26;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7046)) = (s16) D_8011ECD0->unk_2A;
        if ((D_800E6280.unk_F88 & 0x20) && (D_80139C30 != 0)) {
            temp_t4 = (D_80139C30 - 1) & 0xFF;
            D_80139C34[temp_t4] = 1;
            D_80139C3C[temp_t4] = ((u16) D_80139C58 >> 4) + 1;
            temp_v1 = &D_8011ECD0[temp_t4];
            *(s16 *)((unsigned char *)temp_v1 + (6702)) = (s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7042));
            *(s8 *)((unsigned char *)temp_v1 + (6665)) = 4;
            *(s16 *)((unsigned char *)temp_v1 + (6706)) = (s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7046));
            D_80139C30 = temp_t4;
            func_80044750(0x50C);
        }
        func_80137C58();
        func_80138CCC();
        if ((D_80139C30 == 0) && (*D_80139C34 == 0) && (D_80139C34[1] == 0) && (D_80139C34[2] == 0) && (D_80139C34[3] == 0) && (D_80139C34[4] == 0)) {
            func_8004284C();
        }
        break;
    case 1:                                         /* switch 1 */
        *(u8 *)((unsigned char *)D_8011ECD0 + (7959)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7959)) | 0x80);
        var_v1 = 0;
        if (*D_80139C4C != 0xFF) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = (s16) *D_80139C4C;
            var_v1 = 1;
        }
        *(s8 *)((unsigned char *)D_8011ECD0 + (6665)) = 1;
        temp_a0 = &D_80139C4C[1];
        temp_v0 = temp_a0->unk0;
        if (temp_v0 != 0xFF) {
            temp_lo = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(D_8011ECD0 + temp_lo) + (6688)) = temp_v0;
        }
        temp_a1 = *(s16 *)((unsigned char *)temp_a0 + (2));
        temp_v0_2 = &D_8011ECD0[1];
        *(s8 *)((unsigned char *)temp_v0_2 + (6665)) = 1;
        if (temp_a1 != 0xFF) {
            temp_lo_2 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(D_8011ECD0 + temp_lo_2) + (6688)) = temp_a1;
        }
        temp_a1_2 = *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s8 *)((unsigned char *)temp_v0_2 + (6733)) = 1;
        if (temp_a1_2 != 0xFF) {
            temp_lo_3 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(D_8011ECD0 + temp_lo_3) + (6688)) = temp_a1_2;
        }
        temp_a1_3 = *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s8 *)((unsigned char *)temp_v0_2 + (6801)) = 1;
        if (temp_a1_3 != 0xFF) {
            temp_lo_4 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(D_8011ECD0 + temp_lo_4) + (6688)) = temp_a1_3;
        }
        *(s8 *)((unsigned char *)temp_v0_2 + (6869)) = 1;
        if (var_v1 < 5) {
            var_v0 = &D_8011ECD0[var_v1];
            do {
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (6620)) = 0xFF;
            } while ((u32) var_v0 < (u32) (D_8011ECD0 + 0x154));
        }
        func_80085ED0(temp_a0, temp_a1_3, 1, 0x44);
        D_80139C30 = 0;
        func_8004284C();
        break;
    case 2:                                         /* switch 1 */
        if (func_80085EEC() >= 0x2E) {
            if ((*(s16 *)((unsigned char *)(&D_8011ECD0[D_80139C30]) + (6688)) == 0xFF) || ((u8) D_80139C30 >= 5U)) {
                if ((*D_80139C4C == 0x1F) && (D_80139C4C[1] == 0x1F) && (D_80139C4C[2] == 0x1F) && (D_80139C4C[3] == 0x1F) && (D_80139C4C[4] == 0x1F)) {
                    func_80042940(5);
                } else {
                    temp_a3 = D_80139C4C[3];
                    temp_a2 = D_80139C4C[2];
                    temp_a1_4 = D_80139C4C[1];
                    if ((*D_80139C4C == 0x20) && (temp_a1_4 == 0x20) && (temp_a2 == 0x20) && (temp_a3 == 0x20) && (D_80139C4C[4] == 0x20)) {
                        func_80042940(6);
                    } else {
                        func_80085ED0((s16 *) *D_80139C4C, temp_a1_4, temp_a2, temp_a3);
                        func_8004284C();
                    }
                }
                func_80044750(0x50B);
            } else {
                func_80044750(0x50A);
                temp_v1_2 = &D_8011ECD0[D_80139C30];
                *(s8 *)((unsigned char *)temp_v1_2 + (6667)) = 0x84;
                *(s16 *)((unsigned char *)temp_v1_2 + (6702)) = (s16) ((D_80139C30 << 6) - 0x80);
                *(s16 *)((unsigned char *)temp_v1_2 + (6706)) = 0x38;
                D_80139C30 += 1;
                func_80085ED0();
            }
        }
        break;
    case 3:                                         /* switch 1 */
        if (func_80085EEC() >= 0x79) {
            if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) < 4U) {
                func_80048EB8(0);
                func_8004284C();
            } else {
                *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6535)) - 4);
            }
            var_v0_2 = D_8011ECD0 + 0x88;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6603)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
            do {
                *(u8 *)((unsigned char *)var_v0_2 + (6535)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
                *(u8 *)((unsigned char *)var_v0_2 + (6603)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
                *(u8 *)((unsigned char *)var_v0_2 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
                var_v0_2 += 0x110;
                *(u8 *)((unsigned char *)var_v0_2 + (6467)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
            } while (var_v0_2 != (D_8011ECD0 + 0x5D8));
        }
        break;
    case 4:                                         /* switch 1 */
        if (((*D_80139C4C < 0x10) && (var_a1 = D_80139C4C[1], ((var_a1 < 0x10) != 0)) && (var_a2 = D_80139C4C[2], ((var_a2 < 0x10) != 0)) && (var_a3 = D_80139C4C[3], ((var_a3 < 0x10) != 0)) && (var_t0 = D_80139C4C[4], ((var_t0 < 0x10) != 0))) || (var_t0 = D_80139C4C[4], var_a3 = D_80139C4C[3], var_a2 = D_80139C4C[2], var_a1 = D_80139C4C[1], (*D_80139C4C != 0xFF)) || (var_a1 != 0xFF)) {

        }
        if ((*D_80139C4C == 0x10) && (var_a1 == 0x10) && (var_a2 == 0x10) && (var_a3 == 0x10) && (var_t0 == 0x10) && (D_800E6280.unk_F5F == 2)) {
            D_80122E9C = 3;
        } else {
            temp_v1_3 = D_800E6280.unk_F5F;
            temp_v0_3 = temp_v1_3 + 0x14;
            var_at = *D_80139C4C < 0x21;
            if ((temp_v0_3 == *D_80139C4C) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == var_a1)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == var_a2)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == var_a3))) {
                if ((temp_v0_3 == var_t0) && (temp_v1_3 != 2)) {
                    D_80122E9C = 3;
                } else {
                    var_at = *D_80139C4C < 0x21;
                    goto block_73;
                }
            } else {
block_73:
                if ((var_at != 0) && (var_a1 < 0x21) && (var_a2 < 0x21) && (var_a3 < 0x21) && (var_t0 < 0x21)) {
                    D_80122E9C = 1;
                } else {
                    D_80122E9C = 4;
                }
            }
        }
        main_state_set((s32) D_800E6280.unk_720);
        func_80042908((s32) D_800E6280.unk_721);
        func_80042940((s32) D_800E6280.unk_722);
        break;
    case 5:                                         /* switch 1 */
        temp_t5 = D_800E6280.unk_110D;
        switch (temp_t5) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80046290(0x1A0000B1, 0x170000B7, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            break;
        case 1:                                     /* switch 2 */
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
            break;
        case 2:                                     /* switch 2 */
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
            break;
        case 3:                                     /* switch 2 */
            func_80085ED0();
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            break;
        case 4:                                     /* switch 2 */
            if (func_80085EEC() >= 0x79) {
                func_80085ED0();
                func_80042940(3);
            }
            break;
        default:                                    /* switch 2 */
            func_80046500();
            break;
        }
        break;
    case 6:                                         /* switch 1 */
        temp_t7 = D_800E6280.unk_110D;
        switch (temp_t7) {                          /* switch 3 */
        case 0:                                     /* switch 3 */
            func_80046290(0x03000000, 0x01000007, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            break;
        case 1:                                     /* switch 3 */
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
            break;
        case 2:                                     /* switch 3 */
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
            break;
        case 3:                                     /* switch 3 */
            func_80085ED0();
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            break;
        case 4:                                     /* switch 3 */
            if (func_80085EEC() >= 0x79) {
                func_80085ED0();
                func_80042940(3);
            }
            break;
        default:                                    /* switch 3 */
            func_80046500();
            break;
        }
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
