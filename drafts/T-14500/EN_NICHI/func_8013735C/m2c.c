? func_80085ED0(s16 *, s16, s16, s16);              /* extern */
s32 func_80085EEC();                                /* extern */
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E69A2;
extern u8 D_800E71DF;
extern s32 D_800E7208;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern ? D_8011EE24;
extern ? D_8011F2A8;
extern u8 D_80139C30;
extern u8 D_80139C35;
extern u8 D_80139C36;
extern u8 D_80139C37;
extern u8 D_80139C38;
extern s16 D_80139C4E;
extern s16 D_80139C50;
extern s16 D_80139C52;
extern s16 D_80139C54;

void func_8013735C(void) {
    s16 *temp_a0;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_a1_3;
    s16 temp_v0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_v0_3;
    s32 var_at;
    s32 var_v1;
    u8 *var_v0_2;
    u8 temp_t4;
    void *temp_v0_2;
    void *temp_v1;
    void *temp_v1_2;
    void *var_v0;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8006BA40();
        D_80120852 = D_8011ECF6;
        D_80120856 = D_8011ECFA;
        if ((D_800E7208 & 0x20) && (D_80139C30 != 0)) {
            temp_t4 = (D_80139C30 - 1) & 0xFF;
            D_80139C34[temp_t4] = 1;
            D_80139C3C[temp_t4] = ((u16) D_80139C58 >> 4) + 1;
            temp_v1 = D_8011ECD0 + (temp_t4 * 0x44);
            *(s16 *)((unsigned char *)temp_v1 + (6702)) = (s16) D_80120852;
            *(s8 *)((unsigned char *)temp_v1 + (6665)) = 4;
            *(s16 *)((unsigned char *)temp_v1 + (6706)) = (s16) D_80120856;
            D_80139C30 = temp_t4;
            func_80044750(0x50C);
        }
        func_80137C58();
        func_80138CCC();
        if ((D_80139C30 == 0) && (*D_80139C34 == 0) && (D_80139C35 == 0) && (D_80139C36 == 0) && (D_80139C37 == 0) && (D_80139C38 == 0)) {
            func_8004284C();
        }
        break;
    case 1:                                         /* switch 1 */
        D_80120BE7 |= 0x80;
        var_v1 = 0;
        if (*D_80139C4C != 0xFF) {
            D_801206F0 = *D_80139C4C;
            var_v1 = 1;
        }
        D_801206D9 = 1;
        temp_a0 = &D_80139C4C[1];
        temp_v0 = temp_a0->unk0;
        if (temp_v0 != 0xFF) {
            temp_lo = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo]) + (6688)) = temp_v0;
        }
        temp_a1 = *(s16 *)((unsigned char *)temp_a0 + (2));
        temp_v0_2 = D_8011ECD0 + (1 * 0x44);
        *(s8 *)((unsigned char *)temp_v0_2 + (6665)) = 1;
        if (temp_a1 != 0xFF) {
            temp_lo_2 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_2]) + (6688)) = temp_a1;
        }
        temp_a1_2 = *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s8 *)((unsigned char *)temp_v0_2 + (6733)) = 1;
        if (temp_a1_2 != 0xFF) {
            temp_lo_3 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_3]) + (6688)) = temp_a1_2;
        }
        temp_a1_3 = *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s8 *)((unsigned char *)temp_v0_2 + (6801)) = 1;
        if (temp_a1_3 != 0xFF) {
            temp_lo_4 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_4]) + (6688)) = temp_a1_3;
        }
        *(s8 *)((unsigned char *)temp_v0_2 + (6869)) = 1;
        if (var_v1 < 5) {
            var_v0 = (var_v1 * 0x44) + D_8011ECD0;
            do {
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (6620)) = 0xFF;
            } while ((u32) var_v0 < (u32) &D_8011EE24);
        }
        func_80085ED0(temp_a0, temp_a1_3, 1, 0x44);
        D_80139C30 = 0;
        func_8004284C();
        break;
    case 2:                                         /* switch 1 */
        if (func_80085EEC() >= 0x2E) {
            if ((*(s16 *)((unsigned char *)(D_8011ECD0 + (D_80139C30 * 0x44)) + (6688)) == 0xFF) || ((u8) D_80139C30 >= 5U)) {
                if ((*D_80139C4C == 0x1F) && (D_80139C4E == 0x1F) && (D_80139C50 == 0x1F) && (D_80139C52 == 0x1F) && (D_80139C54 == 0x1F)) {
                    func_80042940(5);
                } else if ((*D_80139C4C == 0x20) && (D_80139C4E == 0x20) && (D_80139C50 == 0x20) && (D_80139C52 == 0x20) && (D_80139C54 == 0x20)) {
                    func_80042940(6);
                } else {
                    func_80085ED0((s16 *) *D_80139C4C, D_80139C4E, D_80139C50, D_80139C52);
                    func_8004284C();
                }
                func_80044750(0x50B);
            } else {
                func_80044750(0x50A);
                temp_v1_2 = D_8011ECD0 + (D_80139C30 * 0x44);
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
            if ((u8) D_80120657 < 4U) {
                func_80048EB8(0);
                func_8004284C();
            } else {
                D_80120657 -= 4;
            }
            var_v0_2 = D_8011ED58;
            D_8012069B = D_80120657;
            do {
                *(u8 *)((unsigned char *)var_v0_2 + (6535)) = (u8) D_80120657;
                *(u8 *)((unsigned char *)var_v0_2 + (6603)) = (u8) D_80120657;
                *(u8 *)((unsigned char *)var_v0_2 + (6671)) = (u8) D_80120657;
                var_v0_2 += 0x110;
                *(u8 *)((unsigned char *)var_v0_2 + (6467)) = (u8) D_80120657;
            } while (var_v0_2 != &D_8011F2A8);
        }
        break;
    case 4:                                         /* switch 1 */
        if (((*D_80139C4C < 0x10) && (D_80139C4E < 0x10) && (D_80139C50 < 0x10) && (D_80139C52 < 0x10) && (D_80139C54 < 0x10)) || (*D_80139C4C != 0xFF) || (D_80139C4E != 0xFF)) {

        }
        if ((*D_80139C4C == 0x10) && (D_80139C4E == 0x10) && (D_80139C50 == 0x10) && (D_80139C52 == 0x10) && (D_80139C54 == 0x10) && (D_800E71DF == 2)) {
            D_80122E9C = 3;
        } else {
            temp_v0_3 = D_800E71DF + 0x14;
            var_at = *D_80139C4C < 0x21;
            if ((temp_v0_3 == *D_80139C4C) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C4E)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C50)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C52))) {
                if ((temp_v0_3 == D_80139C54) && (D_800E71DF != 2)) {
                    D_80122E9C = 3;
                } else {
                    var_at = *D_80139C4C < 0x21;
                    goto block_73;
                }
            } else {
block_73:
                if ((var_at != 0) && (D_80139C4E < 0x21) && (D_80139C50 < 0x21) && (D_80139C52 < 0x21) && (D_80139C54 < 0x21)) {
                    D_80122E9C = 1;
                } else {
                    D_80122E9C = 4;
                }
            }
        }
        main_state_set((s32) D_800E69A0);
        func_80042908((s32) D_800E69A1);
        func_80042940((s32) D_800E69A2);
        break;
    case 5:                                         /* switch 1 */
        switch (D_800E738D) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80046290(0x1A0000B1, 0x170000B7, 0xE);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:                                     /* switch 2 */
            if (func_800460EC() == 4) {
                D_800E738D += 1;
            }
            break;
        case 2:                                     /* switch 2 */
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E738D += 1;
            }
            break;
        case 3:                                     /* switch 2 */
            func_80085ED0();
            D_800E738D += 1;
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
        switch (D_800E738D) {                       /* switch 3 */
        case 0:                                     /* switch 3 */
            func_80046290(0x03000000, 0x01000007, 0xE);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:                                     /* switch 3 */
            if (func_800460EC() == 4) {
                D_800E738D += 1;
            }
            break;
        case 2:                                     /* switch 3 */
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E738D += 1;
            }
            break;
        case 3:                                     /* switch 3 */
            func_80085ED0();
            D_800E738D += 1;
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
