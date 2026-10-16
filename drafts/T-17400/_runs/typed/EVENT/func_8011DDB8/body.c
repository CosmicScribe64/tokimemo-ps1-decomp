void func_8011DDB8(void) {
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
    s32 var_a3;
    u16 temp_a1;
    u16 temp_t5;
    u16 temp_t8_3;
    u16 temp_t8_4;
    u8 *temp_t6;
    u8 *temp_t6_2;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    Rec24 *temp_v0_10;
    Rec24 *temp_v0_11;
    Rec24 *temp_v0_12;
    Rec24 *temp_v0_13;
    Rec24 *temp_v0_14;
    Rec24 *temp_v0_15;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    var_a3 = D_800B1AF0;
    switch (var_a3) {
    case 0:
        D_800EED08 = 1;
        D_800F1996 = 0;
        D_800F1992 = 0;
        D_800F1994 = 0;
        *D_800F1990 = 0;
        D_800F19A9 = 0;
        D_800F19A8 = 0;
        D_80080710 = -1;
        D_800F199A = 0;
        D_800F19AA = 0;
        func_800352B4(0);
        D_801258DC = 0;
        var_v1 = 0;
        do {
            temp_t8 = (var_v1 * 2) + 0x15;
            temp_v0 = &((Rec24 *)D_800EC190)[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t9 = var_v1 << 7;
            temp_t6 = &D_800B0860[var_v1];
            var_v1 += 1;
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
            *(s8 *)((unsigned char *)temp_t6 + (4145)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t8_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &((Rec24 *)D_800EC190)[var_v1_2];
            temp_v0_2[6].unk_08 = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t9_2 = var_v1_2 << 7;
            temp_t6_2 = &D_800B0860[var_v1_2];
            var_v1_2 += 1;
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
            *(s8 *)((unsigned char *)temp_t6_2 + (4147)) = 0xB;
        } while (var_v1_2 < 2);
        func_8002746C(0x4321, 1, 0xA0, -0x78);
        D_800B1AF0 += 1;
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
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
            return;
        }
        return;
    case 2:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x80183000);
        D_800B1AF0 += 1;
        return;
    case 3:
        func_8002746C(0x431A, 1);
        D_800B1AF0 += 1;
        return;
    case 5:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x8019C000);
        D_800B1AF0 += 1;
        return;
    case 6:
        func_8002746C(0x4313, 1);
        D_800B1AF0 += 1;
        return;
    case 8:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x801B5000);
        D_800B1AF0 += 1;
        return;
    case 9:
        func_8002746C(0x430C, 1);
        D_800B1AF0 += 1;
        return;
    case 11:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x801CE000);
        D_800B1AF0 += 1;
        return;
    case 12:
        func_8002746C(0x4305, 1);
        D_800B1AF0 += 1;
        return;
    case 14:
        func_8002469C(0x140, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 15:
        func_8002746C(0x42ED, 1);
        D_800B1AF0 += 1;
        return;
    case 17:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 18:
        func_8002746C(0x42F5, 1);
        D_800B1AF0 += 1;
        return;
    case 20:
        func_8002469C(0x280, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 21:
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            temp_v0_3 = (Rec24 *)((u8 *)D_800EC190 + temp_lo);
            temp_v0_3[4].unk_00 = (s32) (temp_v0_3[4].unk_00 & 0x7FFFFFFF);
        } while (var_v1_3 < 4);
        D_800B1AF0 = var_a3 + 1;
        return;
    case 22:
        temp_v1 = (*(u8 *)((unsigned char *)D_800EC190 + (164)) + 2) & 0xFF;
        *(u8 *)((unsigned char *)D_800EC190 + (274)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (273)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (272)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (238)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (237)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (236)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (202)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (201)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (200)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_v1;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_v1;
        D_800B1AF0 = var_a3;
        if (temp_v1 == 0x80) {
            D_800B1AF0 = var_a3 + 1;
            func_800433D0(0x500);
            return;
        }
        break;
    case 23:
        temp_a1 = D_801258DC;
        D_800B1AF0 = var_a3;
        if (!(temp_a1 & 7)) {
            func_8011ED50(4);
        }
        temp_t8_3 = (temp_a1 + 1) & 0xFFFF;
        D_801258DC = temp_t8_3;
        if (temp_t8_3 == 0x140) {
            D_801258DC = 0;
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 24:
        D_800B1AF0 = var_a3 + 1;
        return;
    case 25:
        func_80012D40(0);
        var_v1_4 = 0;
        do {
            temp_lo_2 = var_v1_4 * 0x24;
            var_v1_4 += 1;
            temp_v0_4 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_2);
            temp_v0_4[4].unk_00 = (s32) (temp_v0_4[4].unk_00 | 0x80000000);
        } while (var_v1_4 < 4);
        D_800B1AF0 += 1;
        return;
    case 26:
        func_8002746C(0x42FD, 1);
        D_800B1AF0 += 1;
        return;
    case 28:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x280, 0);
        D_800B1AF0 += 1;
        func_80012D40(1);
        return;
    case 29:
        var_v1_5 = 0;
        do {
            temp_lo_3 = var_v1_5 * 0x24;
            var_v1_5 += 1;
            temp_v0_5 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_3);
            temp_v0_5[4].unk_00 = 0x02000000;
            temp_v0_5[4].unk_0A = 0xA0;
            temp_v0_5[4].unk_0F = 0;
            temp_v0_5[4].unk_06 = -0x78;
        } while (var_v1_5 < 2);
        var_v1_6 = 0;
        do {
            temp_lo_4 = var_v1_6 * 0x24;
            var_v1_6 += 1;
            temp_v0_6 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_4);
            temp_v0_6[6].unk_00 = 0x02000000;
            temp_v0_6[6].unk_0A = 0;
            temp_v0_6[6].unk_0F = 0xA0;
            temp_v0_6[6].unk_06 = -0x78;
        } while (var_v1_6 < 2);
        D_801258DC = 0;
        func_800433D0(0x501);
        D_800B1AF0 += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_8011ED50(8);
        temp_t8_4 = (D_801258DC + 1) & 0xFFFF;
        D_801258DC = temp_t8_4;
        if (temp_t8_4 == 0x14) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 31:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801CE000);
        var_v1_7 = 0;
        do {
            temp_lo_5 = var_v1_7 * 0x24;
            var_v1_7 += 1;
            temp_v0_7 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_5);
            temp_v0_7[4].unk_0A = 0xA0;
            temp_v0_7[4].unk_0F = 0;
            temp_v0_7[4].unk_06 = -0x78;
        } while (var_v1_7 < 2);
        var_v1_8 = 0;
        do {
            temp_lo_6 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            temp_v0_8 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_6);
            temp_v0_8[6].unk_0A = 0;
            temp_v0_8[6].unk_0F = 0xA0;
            temp_v0_8[6].unk_06 = -0x78;
        } while (var_v1_8 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 33:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801B5000);
        var_v1_9 = 0;
        do {
            temp_lo_7 = var_v1_9 * 0x24;
            var_v1_9 += 1;
            temp_v0_9 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_7);
            temp_v0_9[4].unk_0A = 0xA0;
            temp_v0_9[4].unk_0F = 0;
            temp_v0_9[4].unk_06 = -0x78;
        } while (var_v1_9 < 2);
        var_v1_10 = 0;
        do {
            temp_lo_8 = var_v1_10 * 0x24;
            var_v1_10 += 1;
            temp_v0_10 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_8);
            temp_v0_10[6].unk_0A = 0;
            temp_v0_10[6].unk_0F = 0xA0;
            temp_v0_10[6].unk_06 = -0x78;
        } while (var_v1_10 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 35:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x8019C000);
        var_v1_11 = 0;
        do {
            temp_lo_9 = var_v1_11 * 0x24;
            var_v1_11 += 1;
            temp_v0_11 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_9);
            temp_v0_11[4].unk_0A = 0xA0;
            temp_v0_11[4].unk_0F = 0;
            temp_v0_11[4].unk_06 = -0x78;
        } while (var_v1_11 < 2);
        var_v1_12 = 0;
        do {
            temp_lo_10 = var_v1_12 * 0x24;
            var_v1_12 += 1;
            temp_v0_12 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_10);
            temp_v0_12[6].unk_0A = 0;
            temp_v0_12[6].unk_0F = 0xA0;
            temp_v0_12[6].unk_06 = -0x78;
        } while (var_v1_12 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 36:
        func_8011ED50(8);
        if ((u16) D_801258DC >= 8U) {
            temp_v1_2 = *(u8 *)((unsigned char *)D_800EC190 + (164)) - 4;
            *(u8 *)((unsigned char *)D_800EC190 + (274)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (273)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (272)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (238)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (237)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (236)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (202)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (201)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (200)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_v1_2;
            *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_v1_2;
        }
        temp_t5 = (D_801258DC + 1) & 0xFFFF;
        D_801258DC = temp_t5;
        if (temp_t5 == 0x14) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 37:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x80183000);
        var_v1_13 = 0;
        do {
            temp_lo_11 = var_v1_13 * 0x24;
            var_v1_13 += 1;
            temp_v0_13 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_11);
            temp_v0_13[4].unk_0A = 0xA0;
            temp_v0_13[4].unk_0F = 0;
            temp_v0_13[4].unk_06 = -0x78;
        } while (var_v1_13 < 2);
        var_v1_14 = 0;
        do {
            temp_lo_12 = var_v1_14 * 0x24;
            var_v1_14 += 1;
            temp_v0_14 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_12);
            temp_v0_14[6].unk_0A = 0;
            temp_v0_14[6].unk_0F = 0xA0;
            temp_v0_14[6].unk_06 = -0x78;
        } while (var_v1_14 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 38:
        func_8011ED50(8);
        temp_v1_3 = (*(u8 *)((unsigned char *)D_800EC190 + (164)) - 4) & 0xFF;
        *(u8 *)((unsigned char *)D_800EC190 + (274)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (273)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (272)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (238)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (237)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (236)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (202)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (201)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (200)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_v1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_v1_3;
        if (temp_v1_3 < 4U) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 39:
        var_v1_15 = 0;
        do {
            temp_lo_13 = var_v1_15 * 0x24;
            var_v1_15 += 1;
            temp_v0_15 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_13);
            temp_v0_15[4].unk_00 = (s32) (temp_v0_15[4].unk_00 | 0x80000000);
        } while (var_v1_15 < 4);
        D_800B1AF0 = var_a3 + 1;
        return;
    case 40:
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        sp54 = (s32) D_800B1AF6;
        func_8004AFB8();
        D_800B1AF0 = var_a3;
        if (sp54 != D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800B1AF0 = var_a3 + 1;
            return;
        }
        break;
    case 42:
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_a3 + 1;
        return;
    case 43:
        D_800B1AF0 = var_a3;
        switch (D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            var_a3 = D_800B1AF0;
            break;
        case 1:
            func_800469F4(0x42B7);
            var_a3 = D_800B1AF0;
            break;
        case 2:
            func_800469F4(0x42C0);
            var_a3 = D_800B1AF0;
            break;
        case 3:
            func_800469F4(0x42C9);
            var_a3 = D_800B1AF0;
            break;
        case 4:
            func_800469F4(0x42D2);
            var_a3 = D_800B1AF0;
            break;
        }
        D_800B1AF0 = var_a3 + 1;
        return;
    case 45:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        return;
    case 46:
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_a3 + 1;
        return;
    case 48:
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        return;
    case 50:
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        return;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        break;
    }
}
