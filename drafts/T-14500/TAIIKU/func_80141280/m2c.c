? func_800859E8();                                  /* extern */
extern u8 D_800E6375;
extern u8 D_800E738A;
extern ? D_80122ED0;
extern ? D_8014A088;

void func_80141280(void) {
    s32 sp38;
    s32 *temp_v0;
    s32 *temp_v1;
    u8 *var_v1;

    sp38.unk0 = D_8014A088.unk0;
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A088) + (4));
    *(s32 *)((unsigned char *)(&sp38) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A088) + (8));
    *(s32 *)((unsigned char *)(&sp38) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A088) + (12));
    *(s32 *)((unsigned char *)(&sp38) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A088) + (16));
    *(s32 *)((unsigned char *)(&sp38) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8014A088) + (20));
    switch (D_80122EC8) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_80044750(0xBF);
        func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
        func_80085E30(1, 0);
        D_80122ECC = 0;
        func_8008585C();
        D_8014A070->unk0 = sp38;
        D_8014A070[1] = sp3C;
        temp_v1 = &(&sp38)[2];
        temp_v0 = &D_8014A070[2];
        *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (4));
        *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (8));
        *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (12));
        temp_v0->unk0 = temp_v1->unk0;
        func_800573AC(2 * 4, 2);
        func_80064E84();
        func_8004E9F4(0);
        func_8006612C((s32) "体育祭");
        func_80065F34(0);
        func_8006764C(0);
        func_800649D4();
        func_80064FA4(0);
        func_80063930(0);
        func_80086424();
        D_80122EC8 += 1;
        return;
    case 1:                                         /* switch 1 */
        func_80057390(0);
        D_80122EC8 += 1;
        func_80086424();
        return;
    case 2:                                         /* switch 1 */
        if (func_80044E8C() == 1) {
            func_80044750(0x202);
            func_8005742C(0x4020, 1);
            D_80122EC8 += 1;
        }
        func_80086424();
        return;
    case 3:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(1U, 0x801F0000, 0xBA16);
            func_80065F34(1);
            D_80122EC8 += 1;
        }
        func_80086424();
        return;
    case 4:                                         /* switch 1 */
        func_8007E81C();
        if (D_800E738A != 0) {
            D_800E738A = 0;
            D_80122EC8 += 1;
        }
        func_80086424();
        return;
    case 5:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            D_80122EC8 += 1;
            func_80084E4C();
            func_800847B8(0U);
            func_80132B90();
            func_80141B70();
            D_800CA148 = 0;
            if (D_800E6375 & 2) {
                D_800CA148 = 6;
            }
            D_800CA14C = 0;
            D_800CA160 = D_80148F00;
            D_800CA164 = D_80148F04;
            D_800CA168 = D_80148F08;
        }
        func_80086424();
        return;
    case 6:                                         /* switch 1 */
        func_80085A60();
        if (D_800E738A != 0) {
            D_800CA148 = 1;
            D_80122EC8 += 1;
            D_800E738A = 0;
        }
        func_80086424();
        return;
    case 7:                                         /* switch 1 */
        func_800859E8();
        if (D_800E738A != 0) {
            func_80141964();
            D_800E738A = 0;
            D_80122EC8 = 8;
        }
        func_80086424();
        return;
    case 8:                                         /* switch 1 */
        func_801419F8();
        if (D_800E738A != 0) {
            switch (D_80122CDC) {                   /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                D_80122EC8 = 0xC;
                break;
            case 1:                                 /* switch 2 */
                D_80122EC8 = 0xD;
                break;
            case 2:                                 /* switch 2 */
                func_80141AE8();
                break;
            default:                                /* switch 2 */
                func_80046500();
                break;
            }
            D_800E738A = 0;
        }
        func_80086424();
        return;
    case 9:                                         /* switch 1 */
        func_800859E8();
        if (D_800E738A != 0) {
            D_80122EC8 = 0xA;
            D_800E738A = 0;
        }
        func_80086424();
        return;
    case 10:                                        /* switch 1 */
        func_801419F8();
        if (D_800E738A != 0) {
            switch (D_80122CDC) {                   /* switch 3; irregular */
            case 0:                                 /* switch 3 */
                D_80122EC8 = 0xE;
                break;
            case 1:                                 /* switch 3 */
                D_80122EC8 = 0xF;
                break;
            case 2:                                 /* switch 3 */
                func_80141B28();
                break;
            default:                                /* switch 3 */
                func_80046500();
                break;
            }
            D_800E738A = 0;
        }
        func_80086424();
        return;
    case 11:                                        /* switch 1 */
        func_800859E8();
        if (D_800E738A != 0) {
            D_80122EC8 = 8;
            D_800E738A = 0;
        }
        func_80086424();
        return;
    case 12:                                        /* switch 1 */
        func_801452D0();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        return;
    case 13:                                        /* switch 1 */
        func_80133C80();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 14:                                        /* switch 1 */
        func_80142240();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 15:                                        /* switch 1 */
        func_80138A34();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 16:                                        /* switch 1 */
        func_80132000();
        if (D_80122ECC == 2) {
            D_80122EC8 += 1;
            return;
        }
        break;
    case 17:                                        /* switch 1 */
        func_80044750(0xBF);
        func_800674B0();
        func_80086424();
        D_80122EC8 += 1;
        return;
    case 18:                                        /* switch 1 */
        if (func_80044E8C() == 1) {
            func_800573AC();
            func_80057390(0);
            var_v1 = D_80122DD0;
            do {
                var_v1 += 0x10;
                *(s32 *)((unsigned char *)var_v1 + (-12)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-8)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-16)) = 0;
            } while (var_v1 != &D_80122ED0);
            func_80072B5C(1);
        }
        func_80086424();
        return;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
