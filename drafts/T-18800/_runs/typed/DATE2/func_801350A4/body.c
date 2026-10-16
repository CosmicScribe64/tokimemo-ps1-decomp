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
    u8 *temp_t6;
    u8 *temp_t6_2;
    Rec24 *temp_v0_10;
    Rec24 *temp_v0_11;
    Rec24 *temp_v0_12;
    Rec24 *temp_v0_13;
    Rec24 *temp_v0_14;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    temp_a3 = D_800E6280.unk_1104.w;
    D_800B3D60 = 0;
    switch (temp_a3) {
    case 0:
        func_80044750(0xCF);
        D_80122D2C = 1;
        ((RECT *)D_800B5920)[0].h = 0;
        ((RECT *)D_800B5920)[0].y = 0;
        ((RECT *)D_800B5920)[0].w = 0;
        ((RECT *)D_800B5920)[0].x = 0;
        D_800B5939 = 0;
        D_800B5938 = 0;
        D_800B5948 = -1;
        ((RECT *)D_800B5920)[1].y = 0;
        D_800B593A = 0;
        hizuke_disp_switch(0);
        D_8013A5CC = 0;
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_a0];
            temp_v0[4].unk_08 = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = (u8 *)&D_800E6280 + var_a0;
            var_a0 += 1;
            temp_v0[4].unk_00 = 0x82000000;
            temp_v0[4].unk_0A = 0xA0;
            temp_v0[4].unk_0E = 0;
            temp_v0[4].unk_0F = 0;
            temp_v0[4].unk_10 = 0;
            temp_v0[4].unk_0C = temp_t8;
            temp_v0[4].unk_12 = 0x1E0;
            temp_v0[4].unk_14 = 0;
            temp_v0[4].unk_15 = 0;
            temp_v0[4].unk_16 = 0;
            temp_v0[4].unk_18 = 0;
            temp_v0[4].unk_1A = 0;
            temp_v0[4].unk_1C = 0x1000;
            temp_v0[4].unk_1E = 0x1000;
            temp_v0[4].unk_20 = 0;
            temp_v0[4].unk_04 = (s16) (temp_t9 - 0xA0);
            temp_v0[4].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t6 + (4265)) = 0xB;
        } while (var_a0 < 2);
        var_a0_2 = 0;
        do {
            temp_t8_2 = (var_a0_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_a0_2];
            temp_v0_2[6].unk_08 = (s16) ((var_a0_2 << 6) + 0x80);
            temp_t9_2 = var_a0_2 << 7;
            temp_t6_2 = (u8 *)&D_800E6280 + var_a0_2;
            var_a0_2 += 1;
            temp_v0_2[6].unk_00 = 0x82000000;
            temp_v0_2[6].unk_0A = 0;
            temp_v0_2[6].unk_0E = 0;
            temp_v0_2[6].unk_0F = 0xA0;
            temp_v0_2[6].unk_10 = 0;
            temp_v0_2[6].unk_0C = temp_t8_2;
            temp_v0_2[6].unk_12 = 0x1E0;
            temp_v0_2[6].unk_14 = 0;
            temp_v0_2[6].unk_15 = 0;
            temp_v0_2[6].unk_16 = 0;
            temp_v0_2[6].unk_18 = 0;
            temp_v0_2[6].unk_1A = 0;
            temp_v0_2[6].unk_1C = 0x1000;
            temp_v0_2[6].unk_1E = 0x1000;
            temp_v0_2[6].unk_20 = 0;
            temp_v0_2[6].unk_04 = (s16) (temp_t9_2 - 0xA0);
            temp_v0_2[6].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t6_2 + (4267)) = 0xB;
        } while (var_a0_2 < 2);
        dec_bg_cd_read(0x43B1, 1);
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
        dec_bg_cd_read(0x43AA, 1);
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
        dec_bg_cd_read(0x43A3, 1);
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
        dec_bg_cd_read(0x439C, 1);
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
        dec_bg_cd_read(0x4395, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 14:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 15:
        dec_bg_cd_read(0x438D, 1);
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
            temp_v0_3 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
            temp_v0_3[4].unk_00 = (s32) (temp_v0_3[4].unk_00 & 0x7FFFFFFF);
        } while (var_a0_3 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 19:
        var_a0_4 = 4;
        do {
            sp5A = var_a0_4;
            sprite_brightness(var_a0_4, 0x80);
            var_a0_4 += 1;
        } while (var_a0_4 < 8);
        var_a0_5 = 0;
        do {
            temp_lo_2 = var_a0_5 * 0x24;
            var_a0_5 += 1;
            temp_v0_4 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_2);
            temp_v0_4[4].unk_00 = 0x02000000;
            temp_v0_4[4].unk_0A = 0xA0;
            temp_v0_4[4].unk_0F = 0;
            temp_v0_4[4].unk_06 = -0x78;
        } while (var_a0_5 < 2);
        var_a0_6 = 0;
        do {
            temp_lo_3 = var_a0_6 * 0x24;
            var_a0_6 += 1;
            temp_v0_5 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_3);
            temp_v0_5[6].unk_00 = 0x02000000;
            temp_v0_5[6].unk_0A = 0;
            temp_v0_5[6].unk_0F = 0xA0;
            temp_v0_5[6].unk_06 = -0x78;
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
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        var_a0_7 = 0;
        do {
            temp_lo_4 = var_a0_7 * 0x24;
            var_a0_7 += 1;
            temp_v0_6 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_4);
            temp_v0_6[4].unk_0A = 0xA0;
            temp_v0_6[4].unk_0F = 0;
            temp_v0_6[4].unk_06 = -0x78;
        } while (var_a0_7 < 2);
        var_a0_8 = 0;
        do {
            temp_lo_5 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_7 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_5);
            temp_v0_7[6].unk_0A = 0;
            temp_v0_7[6].unk_0F = 0xA0;
            temp_v0_7[6].unk_06 = -0x78;
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
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        var_a0_9 = 0;
        do {
            temp_lo_6 = var_a0_9 * 0x24;
            var_a0_9 += 1;
            temp_v0_8 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_6);
            temp_v0_8[4].unk_0A = 0xA0;
            temp_v0_8[4].unk_0F = 0;
            temp_v0_8[4].unk_06 = -0x78;
        } while (var_a0_9 < 2);
        var_a0_10 = 0;
        do {
            temp_lo_7 = var_a0_10 * 0x24;
            var_a0_10 += 1;
            temp_v0_9 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_7);
            temp_v0_9[6].unk_0A = 0;
            temp_v0_9[6].unk_0F = 0xA0;
            temp_v0_9[6].unk_06 = -0x78;
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
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        var_a0_11 = 0;
        do {
            temp_lo_8 = var_a0_11 * 0x24;
            var_a0_11 += 1;
            temp_v0_10 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_8);
            temp_v0_10[4].unk_0A = 0xA0;
            temp_v0_10[4].unk_0F = 0;
            temp_v0_10[4].unk_06 = -0x78;
        } while (var_a0_11 < 2);
        var_a0_12 = 0;
        do {
            temp_lo_9 = var_a0_12 * 0x24;
            var_a0_12 += 1;
            temp_v0_11 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_9);
            temp_v0_11[6].unk_0A = 0;
            temp_v0_11[6].unk_0F = 0xA0;
            temp_v0_11[6].unk_06 = -0x78;
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
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        var_a0_13 = 0;
        do {
            temp_lo_10 = var_a0_13 * 0x24;
            var_a0_13 += 1;
            temp_v0_12 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_10);
            temp_v0_12[4].unk_0A = 0xA0;
            temp_v0_12[4].unk_0F = 0;
            temp_v0_12[4].unk_06 = -0x78;
        } while (var_a0_13 < 2);
        var_a0_14 = 0;
        do {
            temp_lo_11 = var_a0_14 * 0x24;
            var_a0_14 += 1;
            temp_v0_13 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_11);
            temp_v0_13[6].unk_0A = 0;
            temp_v0_13[6].unk_0F = 0xA0;
            temp_v0_13[6].unk_06 = -0x78;
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
            temp_v0_14 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_12);
            temp_v0_14[4].unk_00 = (s32) (temp_v0_14[4].unk_00 | 0x80000000);
        } while (var_a0_15 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 30:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) D_800E6280.unk_110A;
        read_bustup(temp_a3);
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
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n", temp_a3);
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
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
        hizuke_disp_switch(1);
        break;
    }
}
