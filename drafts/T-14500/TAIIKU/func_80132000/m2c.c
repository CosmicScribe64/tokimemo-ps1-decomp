? func_8007E5A0();                                  /* extern */
extern s8 D_800E62BA;
extern u8 D_800E71DF;
extern s32 D_800E7374;
extern u8 D_800E738A;
extern s32 D_80122E90;
extern u8 D_80148EC0;
extern ? D_80148EC4;

void func_80132000(void) {
    s32 sp88;
    s32 sp84;
    u8 sp83;
    u8 sp82;
    u8 sp81;
    ? sp74;
    s32 sp70;
    ? sp3C;
    GameState *var_v0;
    s32 temp_v1;
    s32 var_a3_2;
    s8 var_a3;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u8 *temp_a0_2;
    u8 *temp_v1_2;
    u8 *var_v1_3;
    u8 temp_a0;
    u8 temp_a0_3;
    u8 temp_a1;
    u8 temp_t3;
    u8 var_a0;
    u8 var_a1;
    u8 var_v1;
    u8 var_v1_2;

    M2C_MEMCPY_ALIGNED(&sp3C, &D_80148EC4, 0x28);
    *(s32 *)((unsigned char *)(&sp3C + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80148EC4 + 0x24) + (4));
    func_80083808();
    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        func_80085E30(1, 0);
        func_800573AC();
        func_80057390(0);
        func_8008585C();
        func_80048E78();
        func_8004E9F4(0);
        func_8006612C((s32) "フォークダンス");
        func_8004E884(1);
        func_80065F34(0);
        func_80063930(0);
        func_80064FA4(0);
        func_80064E84();
        func_800AE120(D_800E7374);
        D_80122E90 = 0;
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        var_v1 = 0;
        if (func_80044E8C() == 1) {
            var_v0 = &D_800E6280;
            var_a3 = 0;
            do {
                if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
                    *(&sp74 + var_v1) = var_a3;
                    var_v1 = (var_v1 + 1) & 0xFF;
                }
                var_a3 += 1;
                var_v0 += 0x38;
            } while (var_a3 != 0xB);
            if (var_v1 < 3U) {
                D_80122ECC = 2;
            } else {
                if (D_80122E90 == 0) {
                    sp83 = var_v1;
                    func_8005742C(0x4133, 1);
                }
                D_800E62BA = 0x80;
                sp83 = var_v1;
                temp_a0 = *(&sp74 + (func_800AE0D0() % var_v1));
                sp82 = temp_a0;
                var_v1_2 = var_v1;
                var_a0 = temp_a0;
                var_a1 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                if (D_80122E90 != 0) {
                    var_a0 = sp75;
                    var_a1 = sp76;
                } else {
                    if (var_a0 == 0) {
                        do {
                            sp83 = var_v1_2;
                            sp81 = var_a1;
                            var_a0 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                        } while (var_a0 == 0);
                    }
                    if ((var_a0 == var_a1) || (var_a1 == 0)) {
                        do {
loop_18:
                            sp83 = var_v1_2;
                            sp82 = var_a0;
                            var_a1 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                            if (var_a0 == var_a1) {
                                goto loop_18;
                            }
                        } while (var_a1 == 0);
                    }
                    D_80122E90 += 1;
                }
                D_80148EC0.unk0 = 0;
                *(u8 *)((unsigned char *)(&D_80148EC0) + (1)) = var_a0;
                *(u8 *)((unsigned char *)(&D_80148EC0) + (2)) = var_a1;
                temp_v1 = func_800AE0D0() & 0x1F;
                sp84 = temp_v1;
                var_a3_2 = 0;
                if (temp_v1 > 0) {
                    do {
                        sp88 = var_a3_2;
                        sp70 = func_800AE0D0() % 3;
                        temp_v1_2 = &(&D_80148EC0)[sp70];
                        temp_a1 = *temp_v1_2;
                        var_a3_2 += 1;
                        temp_a0_2 = &(&D_80148EC0)[func_800AE0D0() % 3];
                        *temp_v1_2 = *temp_a0_2;
                        *temp_a0_2 = temp_a1;
                    } while (var_a3_2 < sp84);
                }
                func_8004284C();
            }
        }
        break;
    case 2:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80132970();
            func_8004500C(0, 0x200);
            func_80057418(1, func_8005751C(1));
            func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_80148EC0.unk0 * 4)) + (60)));
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E81C();
        break;
    case 4:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = D_80148EC0.unk0;
            func_80083474();
        }
        break;
    case 5:                                         /* switch 1 */
        cd_read_sectors(2U, 0x801E8000, 0xB6DC);
        func_8004284C();
        break;
    case 6:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(D_80148EC0.unk0);
            func_80133BF0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_801491B0;
            D_800CA164 = D_801491B4;
            D_800CA168 = D_801491B8;
            func_8004284C();
        }
        break;
    case 7:                                         /* switch 1 */
    case 8:                                         /* switch 1 */
    case 9:                                         /* switch 1 */
    case 12:                                        /* switch 1 */
    case 15:                                        /* switch 1 */
    case 17:                                        /* switch 1 */
    case 22:                                        /* switch 1 */
    case 25:                                        /* switch 1 */
    case 27:                                        /* switch 1 */
    case 32:                                        /* switch 1 */
    case 35:                                        /* switch 1 */
    case 40:                                        /* switch 1 */
        func_80085A60();
        break;
    case 10:                                        /* switch 1 */
        temp_v0 = func_8008667C((s32) D_80148EC0.unk0);
        switch (temp_v0) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
        case 1:                                     /* switch 2 */
            D_800CA148 = (D_80148EC0.unk0 * 3) + 5;
            break;
        case 2:                                     /* switch 2 */
        case 3:                                     /* switch 2 */
            D_800CA148 = (D_80148EC0.unk0 * 3) + 6;
            break;
        case 4:                                     /* switch 2 */
            D_800CA148 = (D_80148EC0.unk0 * 3) + 7;
            break;
        }
        func_8004284C();
        break;
    case 11:                                        /* switch 1 */
    case 20:                                        /* switch 1 */
    case 30:                                        /* switch 1 */
        func_8007E390();
        break;
    case 13:                                        /* switch 1 */
    case 23:                                        /* switch 1 */
    case 33:                                        /* switch 1 */
    case 38:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 14:                                        /* switch 1 */
    case 24:                                        /* switch 1 */
        D_800CA148 = 1;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 16:                                        /* switch 1 */
        func_800847B8(*(u8 *)((unsigned char *)(&D_80148EC0) + (1)));
        func_8004284C();
        break;
    case 18:                                        /* switch 1 */
        func_80062CD0(*(s32 *)((unsigned char *)(sp + (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 4)) + (60)));
        func_8004284C();
        break;
    case 19:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = *(u8 *)((unsigned char *)(&D_80148EC0) + (1));
            func_80083474();
        }
        break;
    case 21:                                        /* switch 1 */
        temp_v0_2 = func_8008667C((s32) *(u8 *)((unsigned char *)(&D_80148EC0) + (1)));
        switch (temp_v0_2) {                        /* switch 3 */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 5;
            break;
        case 2:                                     /* switch 3 */
        case 3:                                     /* switch 3 */
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 6;
            break;
        case 4:                                     /* switch 3 */
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 7;
            break;
        }
        func_8004284C();
        break;
    case 26:                                        /* switch 1 */
        func_800847B8(*(u8 *)((unsigned char *)(&D_80148EC0) + (2)));
        func_8004284C();
        break;
    case 28:                                        /* switch 1 */
        func_80062CD0(*(s32 *)((unsigned char *)(sp + (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 4)) + (60)));
        func_8004284C();
        break;
    case 29:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = *(u8 *)((unsigned char *)(&D_80148EC0) + (2));
            func_80083474();
        }
        break;
    case 31:                                        /* switch 1 */
        temp_a0_3 = *(u8 *)((unsigned char *)(&D_80148EC0) + (2));
        if ((D_800E6280.unk_1BC[*(u8 *)((unsigned char *)(&D_80148EC0) + (2))].unk_0A >= 0x4C) || (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) = (u8) *(u8 *)((unsigned char *)(&D_80148EC0) + (2)), (func_8008667C((s32) temp_a0_3) == 4))) {
            func_80044750(0xBF);
            func_80083440(4U);
            func_80042940(0x26);
        } else {
            temp_v0_3 = func_8008667C((s32) *(u8 *)((unsigned char *)(&D_80148EC0) + (2)));
            switch (temp_v0_3) {                    /* switch 4 */
            case 0:                                 /* switch 4 */
            case 1:                                 /* switch 4 */
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 5;
                break;
            case 2:                                 /* switch 4 */
            case 3:                                 /* switch 4 */
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 6;
                break;
            case 4:                                 /* switch 4 */
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 7;
                break;
            }
            func_8004284C();
        }
        break;
    case 34:                                        /* switch 1 */
        D_800CA148 = 3;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 36:                                        /* switch 1 */
    case 41:                                        /* switch 1 */
        func_8007E934();
        break;
    case 37:                                        /* switch 1 */
    case 42:                                        /* switch 1 */
        var_v1_3 = &D_80148EC0;
        do {
            temp_t3 = *var_v1_3;
            var_v1_3 += 1;
            D_800E6280.unk_1BC[temp_t3].unk_06 += 1;
        } while (var_v1_3 != (&D_80148EC0 + 3));
        D_80122ECC = 2;
        break;
    case 39:                                        /* switch 1 */
        D_800CA148 = 4;
        D_800CA14C = 0;
        func_8004284C();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_80086424();
}
