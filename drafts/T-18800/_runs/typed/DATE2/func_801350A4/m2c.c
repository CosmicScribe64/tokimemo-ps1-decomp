? func_800573F8(?);                                 /* extern */
extern s16 D_800B5922;
extern s16 D_800B5924;
extern s16 D_800B5926;
extern s16 D_800B592A;
extern s8 D_800B593A;

void func_801350A4(void) {
    s16 sp5A;
    s32 sp54;
    s32 sp50;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    Rec24 *temp_v0;
    Rec24 *temp_v0_2;
    s16 temp_t8;
    s16 temp_t8_2;
    s16 var_a0;
    s16 var_a0_10;
    s16 var_a0_11;
    s16 var_a0_12;
    s16 var_a0_13;
    s16 var_a0_14;
    s16 var_a0_15;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a0_5;
    s16 var_a0_6;
    s16 var_a0_7;
    s16 var_a0_8;
    s16 var_a0_9;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_11;
    s32 temp_lo_12;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_t9;
    s32 temp_t9_2;
    u16 temp_t6_3;
    u16 temp_t9_3;
    u8 temp_v1;
    u8 temp_v1_2;
    void *temp_t6;
    void *temp_t6_2;
    void *temp_v0_10;
    void *temp_v0_11;
    void *temp_v0_12;
    void *temp_v0_13;
    void *temp_v0_14;
    void *temp_v0_3;
    void *temp_v0_4;
    void *temp_v0_5;
    void *temp_v0_6;
    void *temp_v0_7;
    void *temp_v0_8;
    void *temp_v0_9;

    temp_a3 = D_800E6280.unk_1104.w;
    D_800B3D60 = 0;
    switch (temp_a3) {
    case 0:
        func_80044750(0xCF);
        D_80122D2C = 1;
        D_800B5926 = 0;
        D_800B5922 = 0;
        D_800B5924 = 0;
        *D_800B5920 = 0;
        D_800B5939 = 0;
        D_800B5938 = 0;
        D_800B5948 = -1;
        D_800B592A = 0;
        D_800B593A = 0;
        func_8006492C(0);
        D_8013A5CC = 0;
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_a0];
            *(s16 *)((unsigned char *)temp_v0 + (152)) = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = &D_800E6280 + var_a0;
            var_a0 += 1;
            *(s32 *)((unsigned char *)temp_v0 + (144)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0 + (158)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (160)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (156)) = temp_t8;
            *(s16 *)((unsigned char *)temp_v0 + (162)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0 + (164)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (165)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (166)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (168)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (170)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (172)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0 + (174)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0 + (176)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (temp_t9 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (150)) = -0x78;
            *(s8 *)((unsigned char *)temp_t6 + (4265)) = 0xB;
        } while (var_a0 < 2);
        var_a0_2 = 0;
        do {
            temp_t8_2 = (var_a0_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_a0_2];
            *(s16 *)((unsigned char *)temp_v0_2 + (224)) = (s16) ((var_a0_2 << 6) + 0x80);
            temp_t9_2 = var_a0_2 << 7;
            temp_t6_2 = &D_800E6280 + var_a0_2;
            var_a0_2 += 1;
            *(s32 *)((unsigned char *)temp_v0_2 + (216)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0_2 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (230)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_2 + (232)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (228)) = temp_t8_2;
            *(s16 *)((unsigned char *)temp_v0_2 + (234)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0_2 + (236)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (237)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (238)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (240)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (242)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (244)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0_2 + (246)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0_2 + (248)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (220)) = (s16) (temp_t9_2 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0_2 + (222)) = -0x78;
            *(s8 *)((unsigned char *)temp_t6_2 + (4267)) = 0xB;
        } while (var_a0_2 < 2);
        func_8005742C(0x43B1, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 31:
    case 34:
    case 37:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x80183000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 3:
        func_8005742C(0x43AA, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 5:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x8019C000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 6:
        func_8005742C(0x43A3, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 8:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x801B5000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 9:
        func_8005742C(0x439C, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 11:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x801CE000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 12:
        func_8005742C(0x4395, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 14:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 15:
        func_8005742C(0x438D, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 18:
        var_a0_3 = 0;
        do {
            temp_lo = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = D_801217D0 + temp_lo;
            *(s32 *)((unsigned char *)temp_v0_3 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_3 + (144)) & 0x7FFFFFFF);
        } while (var_a0_3 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 19:
        var_a0_4 = 4;
        do {
            sp5A = var_a0_4;
            func_80084C98(var_a0_4, 0x80);
            var_a0_4 += 1;
        } while (var_a0_4 < 8);
        var_a0_5 = 0;
        do {
            temp_lo_2 = var_a0_5 * 0x24;
            var_a0_5 += 1;
            temp_v0_4 = D_801217D0 + temp_lo_2;
            *(s32 *)((unsigned char *)temp_v0_4 + (144)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_4 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_4 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_4 + (150)) = -0x78;
        } while (var_a0_5 < 2);
        var_a0_6 = 0;
        do {
            temp_lo_3 = var_a0_6 * 0x24;
            var_a0_6 += 1;
            temp_v0_5 = D_801217D0 + temp_lo_3;
            *(s32 *)((unsigned char *)temp_v0_5 + (216)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_5 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_5 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_5 + (222)) = -0x78;
        } while (var_a0_6 < 2);
        D_8013A5CC = 0;
        func_80044750(0x500);
        D_800E6280.unk_1104.w += 1;
        return;
    case 20:
    case 22:
    case 24:
        func_80135DA4(8);
        temp_t9_3 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t9_3;
        if (temp_t9_3 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 21:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        var_a0_7 = 0;
        do {
            temp_lo_4 = var_a0_7 * 0x24;
            var_a0_7 += 1;
            temp_v0_6 = D_801217D0 + temp_lo_4;
            *(s16 *)((unsigned char *)temp_v0_6 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_6 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_6 + (150)) = -0x78;
        } while (var_a0_7 < 2);
        var_a0_8 = 0;
        do {
            temp_lo_5 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_7 = D_801217D0 + temp_lo_5;
            *(s16 *)((unsigned char *)temp_v0_7 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_7 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_7 + (222)) = -0x78;
        } while (var_a0_8 < 2);
        D_8013A5CC = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 23:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        var_a0_9 = 0;
        do {
            temp_lo_6 = var_a0_9 * 0x24;
            var_a0_9 += 1;
            temp_v0_8 = D_801217D0 + temp_lo_6;
            *(s16 *)((unsigned char *)temp_v0_8 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_8 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_8 + (150)) = -0x78;
        } while (var_a0_9 < 2);
        var_a0_10 = 0;
        do {
            temp_lo_7 = var_a0_10 * 0x24;
            var_a0_10 += 1;
            temp_v0_9 = D_801217D0 + temp_lo_7;
            *(s16 *)((unsigned char *)temp_v0_9 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_9 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_9 + (222)) = -0x78;
        } while (var_a0_10 < 2);
        D_8013A5CC = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 25:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        var_a0_11 = 0;
        do {
            temp_lo_8 = var_a0_11 * 0x24;
            var_a0_11 += 1;
            temp_v0_10 = D_801217D0 + temp_lo_8;
            *(s16 *)((unsigned char *)temp_v0_10 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_10 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_10 + (150)) = -0x78;
        } while (var_a0_11 < 2);
        var_a0_12 = 0;
        do {
            temp_lo_9 = var_a0_12 * 0x24;
            var_a0_12 += 1;
            temp_v0_11 = D_801217D0 + temp_lo_9;
            *(s16 *)((unsigned char *)temp_v0_11 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_11 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_11 + (222)) = -0x78;
        } while (var_a0_12 < 2);
        D_8013A5CC = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 26:
        func_80135DA4(8);
        if ((u16) D_8013A5CC >= 8U) {
            temp_v1 = *(u8 *)((unsigned char *)D_801217D0 + (164)) - 4;
            *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v1;
            *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v1;
        }
        temp_t6_3 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t6_3;
        if (temp_t6_3 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 27:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        var_a0_13 = 0;
        do {
            temp_lo_10 = var_a0_13 * 0x24;
            var_a0_13 += 1;
            temp_v0_12 = D_801217D0 + temp_lo_10;
            *(s16 *)((unsigned char *)temp_v0_12 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_12 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_12 + (150)) = -0x78;
        } while (var_a0_13 < 2);
        var_a0_14 = 0;
        do {
            temp_lo_11 = var_a0_14 * 0x24;
            var_a0_14 += 1;
            temp_v0_13 = D_801217D0 + temp_lo_11;
            *(s16 *)((unsigned char *)temp_v0_13 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_13 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_13 + (222)) = -0x78;
        } while (var_a0_14 < 2);
        D_8013A5CC = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 28:
        func_80135DA4(8);
        temp_v1_2 = (*(u8 *)((unsigned char *)D_801217D0 + (164)) - 4) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v1_2;
        if (temp_v1_2 < 4U) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 29:
        var_a0_15 = 0;
        do {
            temp_lo_12 = var_a0_15 * 0x24;
            var_a0_15 += 1;
            temp_v0_14 = D_801217D0 + temp_lo_12;
            *(s32 *)((unsigned char *)temp_v0_14 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_14 + (144)) | 0x80000000);
        } while (var_a0_15 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 30:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) D_800E6280.unk_110A;
        func_80083B24(temp_a3);
        D_800E6280.unk_1104.w = temp_a3;
        if (sp54 != D_800E6280.unk_110A) {
            D_800E6280.unk_110A -= 1;
            D_800E6280.unk_1104.w = temp_a3 + 1;
            return;
        }
        break;
    case 32:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_80083474();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 33:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        D_800E6280.unk_1104.w += 1;
        return;
    case 35:
        if (((D_80125C04->unk0 & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n", temp_a3);
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E6280.unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        func_800573F8(1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 36:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp50 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        break;
    }
}
