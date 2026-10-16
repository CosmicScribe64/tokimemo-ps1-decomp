? func_800573F8(?);                                 /* extern */
extern s16 D_800B5922;
extern s16 D_800B5924;
extern s16 D_800B5926;
extern s16 D_800B592A;
extern s8 D_800B593A;
extern u8 D_80121875;
extern u8 D_80121876;
extern u8 D_80121898;
extern u8 D_80121899;
extern u8 D_8012189A;
extern u8 D_801218BC;
extern u8 D_801218BD;
extern u8 D_801218BE;
extern u8 D_801218E0;
extern u8 D_801218E1;
extern u8 D_801218E2;

void func_80147958(void) {
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
    s16 var_v1;
    s16 var_v1_10;
    s16 var_v1_11;
    s16 var_v1_12;
    s16 var_v1_13;
    s16 var_v1_14;
    s16 var_v1_15;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    s16 var_v1_5;
    s16 var_v1_6;
    s16 var_v1_7;
    s16 var_v1_8;
    s16 var_v1_9;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_11;
    s32 temp_lo_12;
    s32 temp_lo_13;
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
    s32 var_a1;
    u16 temp_t6_3;
    u16 temp_t9_3;
    u16 temp_t9_4;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;
    void *temp_t6;
    void *temp_t6_2;
    void *temp_v0_10;
    void *temp_v0_11;
    void *temp_v0_12;
    void *temp_v0_13;
    void *temp_v0_14;
    void *temp_v0_15;
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
        func_80044750(0x7F);
        D_80122D2C = 1;
        D_800B5926 = 0;
        D_800B5922 = 0;
        D_800B5924 = 0;
        *D_800B5920 = 0;
        D_800B5939 = 0;
        *D_800B5938 = 0;
        D_800B5948 = -1;
        D_800B592A = 0;
        D_800B593A = 0;
        func_8006492C(0);
        D_8015CF9C = 0;
        var_v1 = 0;
        do {
            temp_t8 = (var_v1 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_v1];
            *(s16 *)((unsigned char *)temp_v0 + (152)) = (s16) ((var_v1 << 6) + 0x80);
            temp_t9 = var_v1 << 7;
            temp_t6 = &D_800E6280 + var_v1;
            var_v1 += 1;
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
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t8_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_v1_2];
            *(s16 *)((unsigned char *)temp_v0_2 + (224)) = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t9_2 = var_v1_2 << 7;
            temp_t6_2 = &D_800E6280 + var_v1_2;
            var_v1_2 += 1;
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
        } while (var_v1_2 < 2);
        func_8005742C(0x43B1, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
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
        func_800536AC(0x140, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 15:
        func_8005742C(0x437D, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 18:
        func_8005742C(0x4385, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 20:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 21:
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            temp_v0_3 = D_801217D0 + temp_lo;
            *(s32 *)((unsigned char *)temp_v0_3 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_3 + (144)) & 0x7FFFFFFF);
        } while (var_v1_3 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 22:
        temp_v1 = (D_80121874 + 2) & 0xFF;
        D_801218E2 = temp_v1;
        D_801218E1 = temp_v1;
        D_801218E0 = temp_v1;
        D_801218BE = temp_v1;
        D_801218BD = temp_v1;
        D_801218BC = temp_v1;
        D_8012189A = temp_v1;
        D_80121899 = temp_v1;
        D_80121898 = temp_v1;
        D_80121876 = temp_v1;
        D_80121875 = temp_v1;
        D_80121874 = temp_v1;
        if (temp_v1 == 0x80) {
            D_800E6280.unk_1104.w = temp_a3 + 1;
            return;
        }
        break;
    case 23:
        var_a1 = D_8015CF9C + 1;
        if (!(D_8015CF9C & 7)) {
            func_80148924(4);
            var_a1 = D_8015CF9C + 1;
            if (((u16) D_8015CF9C % 24U) == 0) {
                func_80044750(0x503);
                var_a1 = D_8015CF9C + 1;
            }
        }
        temp_t9_3 = var_a1 & 0xFFFF;
        D_8015CF9C = temp_t9_3;
        if (temp_t9_3 == 0x140) {
            D_8015CF9C = 0;
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 24:
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 25:
        func_800438F0(0);
        var_v1_4 = 0;
        do {
            temp_lo_2 = var_v1_4 * 0x24;
            var_v1_4 += 1;
            temp_v0_4 = D_801217D0 + temp_lo_2;
            *(s32 *)((unsigned char *)temp_v0_4 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_4 + (144)) | 0x80000000);
        } while (var_v1_4 < 4);
        D_800E6280.unk_1104.w += 1;
        return;
    case 26:
        func_8005742C(0x438D, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 28:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x280, 0);
        D_800E6280.unk_1104.w += 1;
        func_800438F0(1);
        return;
    case 29:
        var_v1_5 = 0;
        do {
            temp_lo_3 = var_v1_5 * 0x24;
            var_v1_5 += 1;
            temp_v0_5 = D_801217D0 + temp_lo_3;
            *(s32 *)((unsigned char *)temp_v0_5 + (144)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_5 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_5 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_5 + (150)) = -0x78;
        } while (var_v1_5 < 2);
        var_v1_6 = 0;
        do {
            temp_lo_4 = var_v1_6 * 0x24;
            var_v1_6 += 1;
            temp_v0_6 = D_801217D0 + temp_lo_4;
            *(s32 *)((unsigned char *)temp_v0_6 + (216)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_6 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_6 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_6 + (222)) = -0x78;
        } while (var_v1_6 < 2);
        D_8015CF9C = 0;
        func_80044750(0x500);
        D_800E6280.unk_1104.w += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_80148924(8);
        temp_t9_4 = (D_8015CF9C + 1) & 0xFFFF;
        D_8015CF9C = temp_t9_4;
        if (temp_t9_4 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 31:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        var_v1_7 = 0;
        do {
            temp_lo_5 = var_v1_7 * 0x24;
            var_v1_7 += 1;
            temp_v0_7 = D_801217D0 + temp_lo_5;
            *(s16 *)((unsigned char *)temp_v0_7 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_7 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_7 + (150)) = -0x78;
        } while (var_v1_7 < 2);
        var_v1_8 = 0;
        do {
            temp_lo_6 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            temp_v0_8 = D_801217D0 + temp_lo_6;
            *(s16 *)((unsigned char *)temp_v0_8 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_8 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_8 + (222)) = -0x78;
        } while (var_v1_8 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 33:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        var_v1_9 = 0;
        do {
            temp_lo_7 = var_v1_9 * 0x24;
            var_v1_9 += 1;
            temp_v0_9 = D_801217D0 + temp_lo_7;
            *(s16 *)((unsigned char *)temp_v0_9 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_9 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_9 + (150)) = -0x78;
        } while (var_v1_9 < 2);
        var_v1_10 = 0;
        do {
            temp_lo_8 = var_v1_10 * 0x24;
            var_v1_10 += 1;
            temp_v0_10 = D_801217D0 + temp_lo_8;
            *(s16 *)((unsigned char *)temp_v0_10 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_10 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_10 + (222)) = -0x78;
        } while (var_v1_10 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 35:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        var_v1_11 = 0;
        do {
            temp_lo_9 = var_v1_11 * 0x24;
            var_v1_11 += 1;
            temp_v0_11 = D_801217D0 + temp_lo_9;
            *(s16 *)((unsigned char *)temp_v0_11 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_11 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_11 + (150)) = -0x78;
        } while (var_v1_11 < 2);
        var_v1_12 = 0;
        do {
            temp_lo_10 = var_v1_12 * 0x24;
            var_v1_12 += 1;
            temp_v0_12 = D_801217D0 + temp_lo_10;
            *(s16 *)((unsigned char *)temp_v0_12 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_12 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_12 + (222)) = -0x78;
        } while (var_v1_12 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 36:
        func_80148924(8);
        if ((u16) D_8015CF9C >= 8U) {
            temp_v1_2 = D_80121874 - 4;
            D_801218E2 = temp_v1_2;
            D_801218E1 = temp_v1_2;
            D_801218E0 = temp_v1_2;
            D_801218BE = temp_v1_2;
            D_801218BD = temp_v1_2;
            D_801218BC = temp_v1_2;
            D_8012189A = temp_v1_2;
            D_80121899 = temp_v1_2;
            D_80121898 = temp_v1_2;
            D_80121876 = temp_v1_2;
            D_80121875 = temp_v1_2;
            D_80121874 = temp_v1_2;
        }
        temp_t6_3 = (D_8015CF9C + 1) & 0xFFFF;
        D_8015CF9C = temp_t6_3;
        if (temp_t6_3 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 37:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        var_v1_13 = 0;
        do {
            temp_lo_11 = var_v1_13 * 0x24;
            var_v1_13 += 1;
            temp_v0_13 = D_801217D0 + temp_lo_11;
            *(s16 *)((unsigned char *)temp_v0_13 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_13 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_13 + (150)) = -0x78;
        } while (var_v1_13 < 2);
        var_v1_14 = 0;
        do {
            temp_lo_12 = var_v1_14 * 0x24;
            var_v1_14 += 1;
            temp_v0_14 = D_801217D0 + temp_lo_12;
            *(s16 *)((unsigned char *)temp_v0_14 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_14 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_14 + (222)) = -0x78;
        } while (var_v1_14 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 38:
        func_80148924(8);
        temp_v1_3 = (D_80121874 - 4) & 0xFF;
        D_801218E2 = temp_v1_3;
        D_801218E1 = temp_v1_3;
        D_801218E0 = temp_v1_3;
        D_801218BE = temp_v1_3;
        D_801218BD = temp_v1_3;
        D_801218BC = temp_v1_3;
        D_8012189A = temp_v1_3;
        D_80121899 = temp_v1_3;
        D_80121898 = temp_v1_3;
        D_80121876 = temp_v1_3;
        D_80121875 = temp_v1_3;
        D_80121874 = temp_v1_3;
        if (temp_v1_3 < 4U) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 39:
        var_v1_15 = 0;
        do {
            temp_lo_13 = var_v1_15 * 0x24;
            var_v1_15 += 1;
            temp_v0_15 = D_801217D0 + temp_lo_13;
            *(s32 *)((unsigned char *)temp_v0_15 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_15 + (144)) | 0x80000000);
        } while (var_v1_15 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 40:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) D_800E6280.unk_110A;
        func_80083B24(D_800E6280.unk_110A);
        D_800E6280.unk_1104.w = temp_a3;
        if (sp54 != D_800E6280.unk_110A) {
            D_800E6280.unk_110A -= 1;
            D_800E6280.unk_1104.w = temp_a3 + 1;
            return;
        }
        break;
    case 42:
        temp_v1_4 = D_800E6280.unk_110A;
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) temp_v1_4;
        func_80083474();
        if ((temp_v1_4 + 1) != D_800E6280.unk_110A) {
            D_800E6280.unk_110A = temp_v1_4;
            D_800E6280.unk_1104.w = 0x28;
            return;
        }
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp50 + 1;
        return;
    case 43:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        D_800E6280.unk_1104.w += 1;
        return;
    case 45:
        if (((D_80125C04->unk0 & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E6280.unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        func_800573F8(1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 46:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 48:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E980();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 50:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        break;
    }
}
