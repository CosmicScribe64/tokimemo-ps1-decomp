void func_801348F0(void) {
    u32 sp30;
    Rec24 *temp_v0;
    s16 temp_t8;
    s16 var_a0;
    s16 var_a0_10;
    s16 var_a0_11;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a0_5;
    s16 var_a0_6;
    s16 var_a0_7;
    s16 var_a0_8;
    s16 var_a0_9;
    s32 *temp_v0_10;
    s32 *temp_v0_11;
    s32 *temp_v0_2;
    s32 *temp_v0_3;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_t9;
    u16 temp_t6_2;
    u32 var_a3;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    u8 *temp_t6;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    var_a3 = D_800E6280.unk_1104.u;
    D_800B3D60 = 0;
    switch (var_a3) {
    case 0:
        D_80122D2C = 1;
        func_80044750(0x201);
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
            temp_v0[10].unk_08 = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = ((u8 *)&D_800E6280) + var_a0;
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
            *(s8 *)((unsigned char *)temp_t6 + (4271)) = 0xC;
        } while (var_a0 < 2);
        func_801365DC(0x43C2);
        D_800E6280.unk_1104.u += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
                func_800AE0B0("DEC_BS header Error \n");
                ((RECT *)D_800B5920)[0].w = 0;
                ((RECT *)D_800B5920)[0].h = 0;
                D_800E6280.unk_1104.u -= 1;
                return;
            }
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            D_800E6280.unk_1104.u += 1;
        } else {
            return;
        }
        break;
    case 2:
        cd_read_sectors(0x31U, 0x801AE000, 0xBCD1);
        func_801339F8();
        D_800E6280.unk_1104.u += 1;
        return;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
        }
        return;
    case 4:
        load_palette(D_8013A5B0, 0x11, 1, 0, 0);
        func_80084E90(D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8);
        D_800E6280.unk_1104.u += 1;
        return;
    case 5:
        var_a0_2 = 0xA;
        do {
            temp_lo = var_a0_2 * 0x24;
            var_a0_2 += 1;
            temp_v0_2 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
            *temp_v0_2 &= 0x7FFFFFFF;
        } while (var_a0_2 < 0xC);
        var_a0_3 = 0x3A;
        do {
            temp_lo_2 = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_2);
            *temp_v0_3 &= 0x7FFFFFFF;
        } while (var_a0_3 < 0x40);
        D_800E6280.unk_1104.u = var_a3 + 1;
        return;
    case 6:
        temp_a1 = D_8012200C + 4;
        D_8012200C = temp_a1;
        D_8012200E = temp_a1;
        D_8012200D = temp_a1;
        var_a0_4 = 0xA;
        do {
            temp_lo_3 = var_a0_4 * 0x24;
            var_a0_4 += 1;
            temp_v0_4 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_3);
            temp_v0_4->unk_16 = (u8) D_8012200C;
            temp_v0_4->unk_15 = (u8) D_8012200C;
            temp_v0_4->unk_14 = (u8) D_8012200C;
        } while (var_a0_4 < 0xC);
        var_a0_5 = 0x3B;
        do {
            temp_lo_4 = var_a0_5 * 0x24;
            var_a0_5 += 1;
            temp_v0_5 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_4);
            temp_v0_5->unk_16 = (u8) D_8012200C;
            temp_v0_5->unk_15 = (u8) D_8012200C;
            temp_v0_5->unk_14 = (u8) D_8012200C;
        } while (var_a0_5 < 0x40);
        if (D_8012200C == 0x40) {
            D_800E6280.unk_1104.u = var_a3 + 1;
        }
        return;
    case 7:
        temp_a1_2 = D_8012200C + 4;
        D_8012200C = temp_a1_2;
        D_8012200E = temp_a1_2;
        D_8012200D = temp_a1_2;
        var_a0_6 = 0xA;
        do {
            temp_lo_5 = var_a0_6 * 0x24;
            var_a0_6 += 1;
            temp_v0_6 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_5);
            temp_v0_6->unk_16 = (u8) D_8012200C;
            temp_v0_6->unk_15 = (u8) D_8012200C;
            temp_v0_6->unk_14 = (u8) D_8012200C;
        } while (var_a0_6 < 0xC);
        var_a0_7 = 0x3B;
        do {
            temp_lo_6 = var_a0_7 * 0x24;
            var_a0_7 += 1;
            temp_v0_7 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_6);
            temp_v0_7->unk_16 = (u8) D_8012200C;
            temp_v0_7->unk_15 = (u8) D_8012200C;
            temp_v0_7->unk_14 = (u8) D_8012200C;
        } while (var_a0_7 < 0x40);
        if (D_8012200C == 0x80) {
            var_a3 += 1;
        }
    case 8:
block_50:
        temp_t6_2 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t6_2;
        D_800E6280.unk_1104.u = var_a3;
        if (temp_t6_2 == 0x96) {
            D_800E6280.unk_1104.u = var_a3 + 1;
        }
        return;
    case 9:
        temp_a1_3 = D_8012200C - 4;
        D_8012200C = temp_a1_3;
        D_8012200E = temp_a1_3;
        D_8012200D = temp_a1_3;
        var_a0_8 = 0xA;
        do {
            temp_lo_7 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_8 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_7);
            temp_v0_8->unk_16 = (u8) D_8012200C;
            temp_v0_8->unk_15 = (u8) D_8012200C;
            temp_v0_8->unk_14 = (u8) D_8012200C;
        } while (var_a0_8 < 0xC);
        var_a0_9 = 0x3B;
        do {
            temp_lo_8 = var_a0_9 * 0x24;
            var_a0_9 += 1;
            temp_v0_9 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_8);
            temp_v0_9->unk_16 = (u8) D_8012200C;
            temp_v0_9->unk_15 = (u8) D_8012200C;
            temp_v0_9->unk_14 = (u8) D_8012200C;
        } while (var_a0_9 < 0x40);
        var_a0_10 = 0xA;
        if (D_8012200C == 0) {
            do {
                temp_lo_9 = var_a0_10 * 0x24;
                var_a0_10 += 1;
                temp_v0_10 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_9);
                *temp_v0_10 |= 0x80000000;
            } while (var_a0_10 < 0xC);
            var_a0_11 = 0x3A;
            do {
                temp_lo_10 = var_a0_11 * 0x24;
                var_a0_11 += 1;
                temp_v0_11 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_10);
                *temp_v0_11 |= 0x80000000;
            } while (var_a0_11 < 0x40);
            var_a3 += 1;
        }
        goto block_50;
    case 10:
        func_800462C8(0xAU, 0x80162000, 0x4350);
        D_800E6280.unk_1104.u += 1;
        return;
    case 12:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n", var_a3);
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        return;
    case 13:
        sp30 = var_a3;
        D_800E6280.unk_1104.u = var_a3;
        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.u = var_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        hizuke_disp_switch(1);
        return;
    }
}
