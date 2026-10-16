void func_8011D5C4(void) {
    s32 sp30;
    Rec24 *temp_v0;
    Rec24 *temp_v0_6;
    s16 temp_a0;
    s16 temp_t8;
    s16 temp_t9_2;
    s16 var_a0;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a0_5;
    s16 var_a0_6;
    s16 var_a0_7;
    s16 var_a0_8;
    s16 var_a0_9;
    s32 temp_at;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_t9;
    s32 var_t0;
    u16 temp_t8_2;
    u16 temp_v1;
    u8 *temp_t6;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    Rec24 *temp_v0_10;
    Rec24 *temp_v0_2;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    var_t0 = D_800B1AF0;
    switch (var_t0) {
    case 0:
        D_800EED08 = 1;
        func_800433D0(0x203);
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
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &((Rec24 *)D_800EC190)[var_a0];
            temp_v0[10].unk_08 = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = &D_800B0860[var_a0];
            var_a0 += 1;
            temp_v0[10].unk_00 = 0x82000000;
            temp_v0[10].unk_0A = 0xA0;
            temp_v0[10].unk_0E = 0;
            temp_v0[10].unk_0F = 0;
            temp_v0[10].unk_10 = 0;
            temp_v0[10].unk_0C = temp_t8;
            temp_v0[10].unk_12 = 0x1E0;
            temp_v0[10].unk_14 = 0;
            temp_v0[10].unk_15 = 0;
            temp_v0[10].unk_16 = 0;
            temp_v0[10].unk_18 = 0;
            temp_v0[10].unk_1A = 0;
            temp_v0[10].unk_1C = 0x1000;
            temp_v0[10].unk_1E = 0x1000;
            temp_v0[10].unk_20 = 0;
            temp_v0[10].unk_04 = (s16) (temp_t9 - 0xA0);
            temp_v0[10].unk_06 = -0x96;
            *(s8 *)((unsigned char *)temp_t6 + (4151)) = 0xC;
        } while (var_a0 < 2);
        func_8002746C(0x4332, 1, 0x24, 0x82000000);
        D_800B1AF0 += 1;
        return;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x140, 0xA0);
            D_800B1AF0 += 1;
        }
        return;
    case 2:
        func_800433D0(0x502);
        func_80015D28(0x31, 0x801AE000, 0xACA8);
        func_8011C650();
        D_800B1AF0 += 1;
        return;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
        }
        return;
    case 4:
        func_80012D64(D_801258C0, 0x11, 1, 0, 0);
        func_8004C46C(D_801258C4, D_801258C8, D_801258CC, D_801258D0, D_801258D4, D_801258D8);
        D_800B1AF0 += 1;
        return;
    case 5:
        var_a0_2 = 0;
        do {
            temp_lo = var_a0_2 * 0x24;
            var_a0_2 += 1;
            temp_v0_2 = (Rec24 *)((u8 *)D_800EC190 + temp_lo);
            temp_v0_2[4].unk_00 = (s32) (temp_v0_2[4].unk_00 & 0x7FFFFFFF);
        } while (var_a0_2 < 8);
        func_800433D0(0x503);
        D_800B1AF0 += 1;
        return;
    case 6:
        temp_a1 = *(u8 *)((unsigned char *)D_800EC190 + (164)) + 1;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_a1;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_a1;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_a1;
        var_a0_3 = 1;
        do {
            temp_lo_2 = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_2);
            temp_v0_3[4].unk_16 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_3[4].unk_15 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_3[4].unk_14 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
        } while (var_a0_3 < 8);
        if (*(u8 *)((unsigned char *)D_800EC190 + (164)) == 0x40) {
            D_800B1AF0 = var_t0 + 1;
        }
        return;
    case 7:
        temp_a1_2 = *(u8 *)((unsigned char *)D_800EC190 + (164)) + 1;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_a1_2;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_a1_2;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_a1_2;
        var_a0_4 = 1;
        do {
            temp_lo_3 = var_a0_4 * 0x24;
            var_a0_4 += 1;
            temp_v0_4 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_3);
            temp_v0_4[4].unk_16 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_4[4].unk_15 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_4[4].unk_14 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
        } while (var_a0_4 < 8);
        if (*(u8 *)((unsigned char *)D_800EC190 + (164)) == 0x80) {
            var_t0 += 1;
        }
block_44:
        temp_t8_2 = (D_801258DC + 1) & 0xFFFF;
        var_a0_5 = 0;
        if ((temp_t8_2 % 19U) == 0x12) {
            do {
                temp_lo_4 = var_a0_5 * 0x24;
                temp_a0 = var_a0_5 + 1;
                temp_t9_2 = temp_a0;
                temp_at = temp_t9_2 < 2;
                var_a0_5 = temp_t9_2;
                temp_v0_5 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_4);
                temp_v0_5[10].unk_06 = (s16) (temp_v0_5[10].unk_06 + 1);
            } while (temp_at != 0);
            temp_v0_6 = &((Rec24 *)D_800EC190)[temp_t9_2];
            temp_v0_6[10].unk_0A = (u16) (temp_v0_6[10].unk_0A - 1);
        }
        var_a0_6 = 0;
        if ((temp_t8_2 % 5U) == 4) {
            do {
                temp_lo_5 = var_a0_6 * 0x24;
                var_a0_6 += 1;
                temp_v0_7 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_5);
                temp_v0_7[4].unk_06 = (s16) (temp_v0_7[4].unk_06 + 1);
            } while (var_a0_6 < 6);
            var_a0_7 = 3;
            do {
                temp_lo_6 = var_a0_7 * 0x24;
                var_a0_7 += 1;
                temp_v0_8 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_6);
                temp_v1 = temp_v0_8[4].unk_0A;
                if (temp_v1 != 0) {
                    temp_v0_8[4].unk_0A = (u16) (temp_v1 - 1);
                } else {
                    temp_v0_8[1].unk_0A = (u16) (temp_v0_8[1].unk_0A - 1);
                }
            } while (var_a0_7 < 6);
        }
        D_801258DC = temp_t8_2;
        D_800B1AF0 = var_t0;
        if (temp_t8_2 == 0x12C) {
            D_800B1AF0 = var_t0 + 1;
        }
        return;
    case 8:
        goto block_44;
    case 9:
        temp_a1_3 = *(u8 *)((unsigned char *)D_800EC190 + (164)) - 2;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_a1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_a1_3;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_a1_3;
        var_a0_8 = 1;
        do {
            temp_lo_7 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_9 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_7);
            temp_v0_9[4].unk_16 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_9[4].unk_15 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
            temp_v0_9[4].unk_14 = (u8) *(u8 *)((unsigned char *)D_800EC190 + (164));
        } while (var_a0_8 < 8);
        var_a0_9 = 0;
        if (*(u8 *)((unsigned char *)D_800EC190 + (164)) == 0) {
            do {
                temp_lo_8 = var_a0_9 * 0x24;
                var_a0_9 += 1;
                temp_v0_10 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_8);
                temp_v0_10[4].unk_00 = (s32) (temp_v0_10[4].unk_00 | 0x80000000);
            } while (var_a0_9 < 8);
            var_t0 += 1;
        }
        goto block_44;
    case 10:
        switch (D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            var_t0 = D_800B1AF0;
            break;
        case 1:
            func_800469F4(0x42B7);
            var_t0 = D_800B1AF0;
            break;
        case 2:
            func_800469F4(0x42C0);
            var_t0 = D_800B1AF0;
            break;
        case 3:
            func_800469F4(0x42C9);
            var_t0 = D_800B1AF0;
            break;
        case 4:
            func_800469F4(0x42D2);
            var_t0 = D_800B1AF0;
            break;
        }
        D_800B1AF0 = var_t0 + 1;
        return;
    case 12:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        return;
    case 13:
        sp30 = var_t0;
        D_800B1AF0 = var_t0;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_t0 + 1;
        return;
    case 15:
        sp30 = var_t0;
        D_800B1AF0 = var_t0;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_t0 + 1;
        return;
    case 17:
        sp30 = var_t0;
        D_800B1AF0 = var_t0;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_t0 + 1;
        return;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        return;
    }
}
