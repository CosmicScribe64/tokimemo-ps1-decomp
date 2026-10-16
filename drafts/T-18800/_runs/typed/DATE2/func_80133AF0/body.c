void func_80133AF0(void) {
    s16 sp60;
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s32 sp50;
    s32 sp4C;
    Rec24 *temp_v0;
    Rec24 *temp_v0_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    s16 var_v1_5;
    s16 var_v1_6;
    s16 var_v1_7;
    s16 var_v1_8;
    s16 var_v1_9;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t5_3;
    s32 temp_t6;
    s32 temp_t8;
    s32 temp_t9_3;
    s32 var_at;
    u16 temp_t9_4;
    u8 temp_t7_3;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 var_a0;
    u8 *temp_t7;
    u8 *temp_t7_2;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;

    D_800B3D60 = 0;
    switch (D_800E6280.unk_1104.w) {
    case 0:
        D_80122D2C = 1;
        func_800AE120((s32) D_800E6280.unk_10F8);
        func_80044750(0x206);
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
        D_8013A5D0 = 2;
        D_800E6280.unk_110D = 0;
        D_8013A5CC = 0;
        var_v1 = 0;
        do {
            temp_t9 = (var_v1 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t5 = var_v1 << 7;
            temp_t7 = (u8 *)&D_800E6280 + var_v1;
            var_v1 += 1;
            temp_v0[4].unk_00 = 0x82000000;
            temp_v0[4].unk_0A = 0;
            temp_v0[4].unk_0E = 0;
            temp_v0[4].unk_0F = 0xA0;
            temp_v0[4].unk_10 = 0;
            temp_v0[4].unk_0C = temp_t9;
            temp_v0[4].unk_12 = 0x1E0;
            temp_v0[4].unk_14 = 0;
            temp_v0[4].unk_15 = 0;
            temp_v0[4].unk_16 = 0;
            temp_v0[4].unk_18 = 0;
            temp_v0[4].unk_1A = 0;
            temp_v0[4].unk_1C = 0x1000;
            temp_v0[4].unk_1E = 0x1000;
            temp_v0[4].unk_20 = 0;
            temp_v0[4].unk_04 = (s16) (temp_t5 - 0xA0);
            temp_v0[4].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t9_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_v1_2];
            temp_v0_2[6].unk_08 = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t5_2 = var_v1_2 << 7;
            temp_t7_2 = (u8 *)&D_800E6280 + var_v1_2;
            var_v1_2 += 1;
            temp_v0_2[6].unk_00 = 0x82000000;
            temp_v0_2[6].unk_0A = 0xA0;
            temp_v0_2[6].unk_0E = 0;
            temp_v0_2[6].unk_0F = 0;
            temp_v0_2[6].unk_10 = 0;
            temp_v0_2[6].unk_0C = temp_t9_2;
            temp_v0_2[6].unk_12 = 0x1E0;
            temp_v0_2[6].unk_14 = 0;
            temp_v0_2[6].unk_15 = 0;
            temp_v0_2[6].unk_16 = 0;
            temp_v0_2[6].unk_18 = 0;
            temp_v0_2[6].unk_1A = 0;
            temp_v0_2[6].unk_1C = 0x1000;
            temp_v0_2[6].unk_1E = 0x1000;
            temp_v0_2[6].unk_20 = 0;
            temp_v0_2[6].unk_04 = (s16) (temp_t5_2 - 0xA0);
            temp_v0_2[6].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7_2 + (4267)) = 0xB;
        } while (var_v1_2 < 2);
        dec_bg_cd_read(0x43CB, 1);
        D_800E6280.unk_1104.w += 1;
block_49:
        if (((u32) D_800E6280.unk_1104.w < 9U) || ((u32) D_800E6280.unk_1104.w >= 0xCU)) {
            return;
        }
        var_v1_3 = D_801217D0->unk0 + D_8013A5D0;
        var_at = var_v1_3 < 0x40;
        if (var_v1_3 < 0) {
            sp60 = 0;
            var_v1_3 = sp60;
            D_8013A5D0 = (func_800AE0D0() % 4) + 2;
            var_at = var_v1_3 < 0x40;
        }
        if (var_at == 0) {
            sp60 = 0x3F;
            var_v1_3 = 0x3F;
            D_8013A5D0 = -2 - (func_800AE0D0() % 4);
        }
        var_a0 = *(u8 *)((unsigned char *)D_801217D0 + (159));
        *(s8 *)((unsigned char *)D_801217D0 + (266)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (230)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (194)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (158)) = (s8) var_v1_3;
        if (var_a0 != 0) {
            *(u16 *)((unsigned char *)D_801217D0 + (154)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (154)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (190)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (190)) + 0x14);
            var_a0 = (var_a0 - 0x14) & 0xFF;
            *(u8 *)((unsigned char *)D_801217D0 + (195)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (195)) - 0x14);
        } else {
            *(u16 *)((unsigned char *)D_801217D0 + (154)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (154)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (150)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (150)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (190)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (190)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (186)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (186)) + 0x14);
        }
        if (*(u8 *)((unsigned char *)D_801217D0 + (231)) != 0) {
            *(u16 *)((unsigned char *)D_801217D0 + (226)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (226)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (262)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (262)) + 0x14);
            *(u8 *)((unsigned char *)D_801217D0 + (231)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (231)) - 0x14);
            *(u8 *)((unsigned char *)D_801217D0 + (267)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (267)) - 0x14);
        } else {
            D_801217D0->unk0 = (s16) ((u16) D_801217D0->unk0 - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (222)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (222)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (262)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (262)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (258)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (258)) + 0x14);
        }
        *(u8 *)((unsigned char *)D_801217D0 + (159)) = var_a0;
        if (((var_a0 == 0) && ((D_800E6280.unk_110D == 1) || (D_800E6280.unk_110D == 3))) || ((*(u8 *)((unsigned char *)D_801217D0 + (231)) == 0) && ((D_800E6280.unk_110D == 0) || (D_800E6280.unk_110D == 2)))) {
            var_v1_4 = 0;
            if (D_800E6280.unk_110D != 0) {
                var_v1_5 = 0;
                if (D_800E6280.unk_110D != 1) {
                    var_v1_6 = 0;
                    if (D_800E6280.unk_110D != 2) {
                        var_v1_7 = 0;
                        if (D_800E6280.unk_110D != 3) {

                        } else {
                            do {
                                temp_lo = var_v1_7 * 0x24;
                                temp_t6 = var_v1_7 * 2;
                                var_v1_7 += 1;
                                temp_v0_3 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
                                temp_v0_3[6].unk_0A = 0;
                                temp_v0_3[6].unk_0F = 0xA0;
                                temp_v0_3[6].unk_06 = -0x78;
                                temp_v0_3[6].unk_0C = (s16) (temp_t6 + 0xA);
                            } while (var_v1_7 < 2);
                            LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801DD000);
                        }
                    } else {
                        do {
                            temp_lo_2 = var_v1_6 * 0x24;
                            temp_t8 = var_v1_6 * 2;
                            var_v1_6 += 1;
                            temp_v0_4 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_2);
                            temp_v0_4[4].unk_0A = 0;
                            temp_v0_4[4].unk_0F = 0xA0;
                            temp_v0_4[4].unk_06 = -0x78;
                            temp_v0_4[4].unk_0C = (s16) (temp_t8 + 0x15);
                        } while (var_v1_6 < 2);
                        LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x801BF000);
                    }
                } else {
                    do {
                        temp_lo_3 = var_v1_5 * 0x24;
                        temp_t9_3 = var_v1_5 * 2;
                        var_v1_5 += 1;
                        temp_v0_5 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_3);
                        temp_v0_5[6].unk_0A = 0;
                        temp_v0_5[6].unk_0F = 0xA0;
                        temp_v0_5[6].unk_06 = -0x78;
                        temp_v0_5[6].unk_0C = (s16) (temp_t9_3 + 0xA);
                    } while (var_v1_5 < 2);
                    LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801A1000);
                }
            } else {
                do {
                    temp_lo_4 = var_v1_4 * 0x24;
                    temp_t5_3 = var_v1_4 * 2;
                    var_v1_4 += 1;
                    temp_v0_6 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_4);
                    temp_v0_6[4].unk_0A = 0;
                    temp_v0_6[4].unk_0F = 0xA0;
                    temp_v0_6[4].unk_06 = -0x78;
                    temp_v0_6[4].unk_0C = (s16) (temp_t5_3 + 0x15);
                } while (var_v1_4 < 2);
                LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x80183000);
            }
            temp_t7_3 = D_800E6280.unk_110D + 1;
            D_800E6280.unk_110D = temp_t7_3;
            if ((u32) (temp_t7_3 & 0xFF) >= 4U) {
                D_800E6280.unk_110D = 0;
            }
        }
        temp_t9_4 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t9_4;
        if (temp_t9_4 == 0x96) {
            D_800E6280.unk_1104.w += 1;
        }
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x80183000);
            D_800E6280.unk_1104.w += 1;
        }
block_48:
        goto block_49;
    case 2:
        dec_bg_cd_read(0x43D4, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_48;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801A1000);
            D_800E6280.unk_1104.w += 1;
        }
        goto block_48;
    case 4:
        dec_bg_cd_read(0x43DE, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_48;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801BF000);
            D_800E6280.unk_1104.w += 1;
        }
        goto block_48;
    case 6:
        dec_bg_cd_read(0x43E7, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_48;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801DD000);
            D_800E6280.unk_1104.w += 1;
        }
        goto block_48;
    case 8:
        var_v1_8 = 0;
        do {
            temp_lo_5 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_5) + (144)) = 0x02000000;
        } while (var_v1_8 < 4);
        D_800E6280.unk_1104.w += 1;
        func_80044750(0x500);
        goto block_49;
    case 9:
        temp_v0_7 = (*(u8 *)((unsigned char *)D_801217D0 + (164)) + 2) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v0_7;
        if (temp_v0_7 == 0x80) {
            D_800E6280.unk_1104.w += 1;
        }
        goto block_48;
    case 10:
        goto block_48;
    case 11:
        temp_v0_8 = (*(u8 *)((unsigned char *)D_801217D0 + (164)) - 2) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v0_8;
        if (temp_v0_8 == 0) {
            var_v1_9 = 0;
            do {
                temp_lo_6 = var_v1_9 * 0x24;
                var_v1_9 += 1;
                *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_6) + (144)) = 0x82000000;
            } while (var_v1_9 < 4);
            D_800E6280.unk_1104.w += 1;
        }
        goto block_49;
    case 12:
        sp50 = (s32) D_800E6280.unk_110A;
        sp4C = D_800E6280.unk_1104.w;
        read_bustup();
        D_800E6280.unk_1104.w = sp4C;
        if (sp50 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.w = sp4C + 1;
            D_800E6280.unk_110A -= 1;
        }
        goto block_48;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.w += 1;
        }
        goto block_48;
    case 14:
        sp4C = D_800E6280.unk_1104.w;
        func_80083474();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp4C + 1;
        goto block_48;
    case 15:
        func_800462C8(0xAU, 0x80162000, 0x4359);
        D_800E6280.unk_1104.w += 1;
        goto block_48;
    case 17:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.w += 1;
        goto block_48;
    case 18:
        func_80044750(0x200);
        sp4C = D_800E6280.unk_1104.w;
        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp4C + 1;
        goto block_48;
    default:
        hizuke_disp_switch(1);
        D_80122D2C = 0;
        func_8004284C();
        goto block_48;
    }
}
