void func_8014718C(void) {
    u32 sp30;
    Rec24 *temp_v0;
    Rec24 *temp_v0_6;
    s16 temp_a0;
    s16 temp_t6_2;
    s16 temp_t8;
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
    u16 temp_t8_2;
    u16 temp_v1;
    u32 var_a3;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    u8 *temp_t6;
    u8 *temp_v0_10;
    u8 *temp_v0_2;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_7;
    u8 *temp_v0_8;
    u8 *temp_v0_9;

    var_a3 = D_800E7384;
    D_800B3D60 = 0;
    switch (var_a3) {
    case 0:
        func_80044750(0x201);
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
        hizuke_disp_switch(0);
        D_8015CF9C = 0;
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_a0];
            *(s16 *)((unsigned char *)temp_v0 + (368)) = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = (u8 *)&D_800E6280 + var_a0;
            var_a0 += 1;
            *(s32 *)((unsigned char *)temp_v0 + (360)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0 + (370)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0 + (374)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (375)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (376)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (372)) = temp_t8;
            *(s16 *)((unsigned char *)temp_v0 + (378)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0 + (380)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (381)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (382)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (384)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (386)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (388)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0 + (390)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0 + (392)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (364)) = (s16) (temp_t9 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (366)) = -0x96;
            *(s8 *)((unsigned char *)temp_t6 + (4271)) = 0xC;
        } while (var_a0 < 2);
        dec_bg_cd_read(0x43C2, 1);
        D_800E7384 += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            D_800E7384 += 1;
        }
        return;
    case 2:
        cd_read_sectors(0x31U, 0x801AE000, 0xBCD1);
        func_801461B0();
        D_800E7384 += 1;
        return;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            D_800E7384 += 1;
        }
        return;
    case 4:
        load_palette(D_8015CF80, 0x11, 1, 0, 0);
        func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98);
        D_800E7384 += 1;
        return;
    case 5:
        var_a0_2 = 0;
        do {
            temp_lo = var_a0_2 * 0x24;
            var_a0_2 += 1;
            temp_v0_2 = D_801217D0 + temp_lo;
            *(s32 *)((unsigned char *)temp_v0_2 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_2 + (144)) & 0x7FFFFFFF);
        } while (var_a0_2 < 8);
        D_800E7384 = var_a3 + 1;
        return;
    case 6:
        temp_a1 = D_80121874 + 1;
        D_80121874 = temp_a1;
        D_80121876 = temp_a1;
        D_80121875 = temp_a1;
        var_a0_3 = 1;
        do {
            temp_lo_2 = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = D_801217D0 + temp_lo_2;
            *(u8 *)((unsigned char *)temp_v0_3 + (166)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_3 + (165)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_3 + (164)) = (u8) D_80121874;
        } while (var_a0_3 < 8);
        if (D_80121874 == 0x40) {
            D_800E7384 = var_a3 + 1;
        }
        return;
    case 7:
        temp_a1_2 = D_80121874 + 1;
        D_80121874 = temp_a1_2;
        D_80121876 = temp_a1_2;
        D_80121875 = temp_a1_2;
        var_a0_4 = 1;
        do {
            temp_lo_3 = var_a0_4 * 0x24;
            var_a0_4 += 1;
            temp_v0_4 = D_801217D0 + temp_lo_3;
            *(u8 *)((unsigned char *)temp_v0_4 + (166)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_4 + (165)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_4 + (164)) = (u8) D_80121874;
        } while (var_a0_4 < 8);
        if (D_80121874 == 0x80) {
            var_a3 += 1;
        }
block_40:
        temp_t8_2 = (D_8015CF9C + 1) & 0xFFFF;
        var_a0_5 = 0;
        if ((temp_t8_2 % 19U) == 0x12) {
            do {
                temp_lo_4 = var_a0_5 * 0x24;
                temp_a0 = var_a0_5 + 1;
                temp_t6_2 = temp_a0;
                temp_at = temp_t6_2 < 2;
                var_a0_5 = temp_t6_2;
                temp_v0_5 = D_801217D0 + temp_lo_4;
                *(s16 *)((unsigned char *)temp_v0_5 + (366)) = (s16) (*(s16 *)((unsigned char *)temp_v0_5 + (366)) + 1);
            } while (temp_at != 0);
            temp_v0_6 = &D_801217D0[temp_t6_2];
            *(u16 *)((unsigned char *)temp_v0_6 + (370)) = (u16) (*(u16 *)((unsigned char *)temp_v0_6 + (370)) - 1);
        }
        var_a0_6 = 0;
        if ((temp_t8_2 % 5U) == 4) {
            do {
                temp_lo_5 = var_a0_6 * 0x24;
                var_a0_6 += 1;
                temp_v0_7 = D_801217D0 + temp_lo_5;
                *(s16 *)((unsigned char *)temp_v0_7 + (150)) = (s16) (*(s16 *)((unsigned char *)temp_v0_7 + (150)) + 1);
            } while (var_a0_6 < 6);
            var_a0_7 = 3;
            do {
                temp_lo_6 = var_a0_7 * 0x24;
                var_a0_7 += 1;
                temp_v0_8 = D_801217D0 + temp_lo_6;
                temp_v1 = *(u16 *)((unsigned char *)temp_v0_8 + (154));
                if (temp_v1 != 0) {
                    *(u16 *)((unsigned char *)temp_v0_8 + (154)) = (u16) (temp_v1 - 1);
                } else {
                    *(u16 *)((unsigned char *)temp_v0_8 + (46)) = (u16) (*(u16 *)((unsigned char *)temp_v0_8 + (46)) - 1);
                }
            } while (var_a0_7 < 6);
        }
        D_800E7384 = var_a3;
        D_8015CF9C = temp_t8_2;
        if (temp_t8_2 == 0x12C) {
            D_800E7384 = var_a3 + 1;
        }
        return;
    case 8:
        goto block_40;
    case 9:
        temp_a1_3 = D_80121874 - 2;
        D_80121874 = temp_a1_3;
        D_80121876 = temp_a1_3;
        D_80121875 = temp_a1_3;
        var_a0_8 = 1;
        do {
            temp_lo_7 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_9 = D_801217D0 + temp_lo_7;
            *(u8 *)((unsigned char *)temp_v0_9 + (166)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_9 + (165)) = (u8) D_80121874;
            *(u8 *)((unsigned char *)temp_v0_9 + (164)) = (u8) D_80121874;
        } while (var_a0_8 < 8);
        var_a0_9 = 0;
        if (D_80121874 == 0) {
            do {
                temp_lo_8 = var_a0_9 * 0x24;
                var_a0_9 += 1;
                temp_v0_10 = D_801217D0 + temp_lo_8;
                *(s32 *)((unsigned char *)temp_v0_10 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_10 + (144)) | 0x80000000);
            } while (var_a0_9 < 8);
            var_a3 += 1;
        }
        goto block_40;
    case 10:
        func_800462C8(0xAU, 0x80162000, 0x4350);
        D_800E7384 += 1;
        return;
    case 12:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E7384 -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        D_800E7384 += 1;
        return;
    case 13:
        sp30 = var_a3;
        D_800E7384 = var_a3;
        func_8013E7B0();
        D_800E738A -= 1;
        D_800E7384 = var_a3 + 1;
        return;
    case 15:
        sp30 = var_a3;
        D_800E7384 = var_a3;
        func_8013E980();
        D_800E738A -= 1;
        D_800E7384 = var_a3 + 1;
        return;
    case 17:
        sp30 = var_a3;
        D_800E7384 = var_a3;
        func_8013E9BC();
        D_800E738A -= 1;
        D_800E7384 = var_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        hizuke_disp_switch(1);
        return;
    }
}
