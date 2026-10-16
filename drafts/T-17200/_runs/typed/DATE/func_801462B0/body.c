u16 func_801462B0(void) {
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
    s32 temp_t6_2;
    s32 temp_t7_3;
    s32 temp_t9_3;
    s32 var_at;
    u16 temp_t6;
    u8 temp_t8;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 temp_v1;
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
        D_8015CFA0 = 2;
        D_800E6280.unk_110D = 0;
        D_8015CF9C = 0;
        var_v1 = 0;
        do {
            temp_t9 = (var_v1 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t5 = var_v1 << 7;
            temp_t7 = &D_800E6280 + var_v1;
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
            temp_t7_2 = &D_800E6280 + var_v1_2;
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
block_53:
        if (((u32) D_800E6280.unk_1104.w < 9U) || ((u32) D_800E6280.unk_1104.w >= 0xCU)) {
            return 0U;
        }
        var_v1_3 = D_8012186E + D_8015CFA0;
        var_at = var_v1_3 < 0x40;
        if (var_v1_3 < 0) {
            sp60 = 0;
            var_v1_3 = sp60;
            D_8015CFA0 = (func_800AE0D0() % 4) + 2;
            var_at = var_v1_3 < 0x40;
        }
        if (var_at == 0) {
            sp60 = 0x3F;
            var_v1_3 = 0x3F;
            D_8015CFA0 = -2 - (func_800AE0D0() % 4);
        }
        var_a0 = *(u8 *)((unsigned char *)(&D_8012186E) + (1));
        D_801218DA = (u8) var_v1_3;
        D_801218B6 = (u8) var_v1_3;
        D_80121892 = (u8) var_v1_3;
        D_8012186E = (u8) var_v1_3;
        if (var_a0 != 0) {
            D_8012186A += 0x14;
            D_8012188E += 0x14;
            var_a0 = (var_a0 - 0x14) & 0xFF;
            D_80121893 -= 0x14;
        } else {
            D_8012186A -= 0x14;
            D_80121866 += 0x14;
            D_8012188E -= 0x14;
            D_8012188A += 0x14;
        }
        if (D_801218B7 != 0) {
            D_801218B2 += 0x14;
            D_801218D6 += 0x14;
            D_801218B7 -= 0x14;
            D_801218DB -= 0x14;
        } else {
            D_801218B2 -= 0x14;
            D_801218AE += 0x14;
            D_801218D6 -= 0x14;
            D_801218D2 += 0x14;
        }
        *(u8 *)((unsigned char *)(&D_8012186E) + (1)) = var_a0;
        if (((var_a0 == 0) && ((D_800E6280.unk_110D == 1) || (D_800E6280.unk_110D == 3))) || ((D_801218B7 == 0) && ((D_800E6280.unk_110D == 0) || (D_800E6280.unk_110D == 2)))) {
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
                                temp_t5_3 = var_v1_7 * 2;
                                var_v1_7 += 1;
                                temp_v0_3 = &D_801217D0[var_v1_7];
                                temp_v0_3[6].unk_0A = 0;
                                temp_v0_3[6].unk_0F = 0xA0;
                                temp_v0_3[6].unk_06 = -0x78;
                                temp_v0_3[6].unk_0C = (s16) (temp_t5_3 + 0xA);
                            } while (var_v1_7 < 2);
                            LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801DD000);
                        }
                    } else {
                        do {
                            temp_lo_2 = var_v1_6 * 0x24;
                            temp_t7_3 = var_v1_6 * 2;
                            var_v1_6 += 1;
                            temp_v0_4 = &D_801217D0[var_v1_6];
                            temp_v0_4[4].unk_0A = 0;
                            temp_v0_4[4].unk_0F = 0xA0;
                            temp_v0_4[4].unk_06 = -0x78;
                            temp_v0_4[4].unk_0C = (s16) (temp_t7_3 + 0x15);
                        } while (var_v1_6 < 2);
                        LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x801BF000);
                    }
                } else {
                    do {
                        temp_lo_3 = var_v1_5 * 0x24;
                        temp_t6_2 = var_v1_5 * 2;
                        var_v1_5 += 1;
                        temp_v0_5 = &D_801217D0[var_v1_5];
                        temp_v0_5[6].unk_0A = 0;
                        temp_v0_5[6].unk_0F = 0xA0;
                        temp_v0_5[6].unk_06 = -0x78;
                        temp_v0_5[6].unk_0C = (s16) (temp_t6_2 + 0xA);
                    } while (var_v1_5 < 2);
                    LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801A1000);
                }
            } else {
                do {
                    temp_lo_4 = var_v1_4 * 0x24;
                    temp_t9_3 = var_v1_4 * 2;
                    var_v1_4 += 1;
                    temp_v0_6 = &D_801217D0[var_v1_4];
                    temp_v0_6[4].unk_0A = 0;
                    temp_v0_6[4].unk_0F = 0xA0;
                    temp_v0_6[4].unk_06 = -0x78;
                    temp_v0_6[4].unk_0C = (s16) (temp_t9_3 + 0x15);
                } while (var_v1_4 < 2);
                LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x80183000);
            }
            temp_t8 = D_800E6280.unk_110D + 1;
            D_800E6280.unk_110D = temp_t8;
            if ((u32) (temp_t8 & 0xFF) >= 4U) {
                D_800E6280.unk_110D = 0;
            }
        }
        temp_t6 = (D_8015CF9C + 1) & 0xFFFF;
        D_8015CF9C = temp_t6;
        if (temp_t6 == 0x12C) {
            D_800E6280.unk_1104.w += 1;
        }
        return temp_t6;
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
block_52:
        goto block_53;
    case 2:
        dec_bg_cd_read(0x43D4, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_52;
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
        goto block_52;
    case 4:
        dec_bg_cd_read(0x43DE, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_52;
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
        goto block_52;
    case 6:
        dec_bg_cd_read(0x43E7, 1);
        D_800E6280.unk_1104.w += 1;
        goto block_52;
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
        goto block_52;
    case 8:
        var_v1_8 = 0;
        do {
            temp_lo_5 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_5) + (144)) = 0x02000000;
        } while (var_v1_8 < 4);
        D_800E6280.unk_1104.w += 1;
        func_80044750(0x500);
        goto block_53;
    case 9:
        temp_v0_7 = (D_80121874 + 2) & 0xFF;
        D_801218E2 = temp_v0_7;
        D_801218E1 = temp_v0_7;
        D_801218E0 = temp_v0_7;
        D_801218BE = temp_v0_7;
        D_801218BD = temp_v0_7;
        D_801218BC = temp_v0_7;
        D_8012189A = temp_v0_7;
        D_80121899 = temp_v0_7;
        D_80121898 = temp_v0_7;
        D_80121876 = temp_v0_7;
        D_80121875 = temp_v0_7;
        D_80121874 = temp_v0_7;
        if (temp_v0_7 == 0x80) {
            D_800E6280.unk_1104.w += 1;
        }
        goto block_52;
    case 10:
        goto block_52;
    case 11:
        temp_v0_8 = (D_80121874 - 2) & 0xFF;
        D_801218E2 = temp_v0_8;
        D_801218E1 = temp_v0_8;
        D_801218E0 = temp_v0_8;
        D_801218BE = temp_v0_8;
        D_801218BD = temp_v0_8;
        D_801218BC = temp_v0_8;
        D_8012189A = temp_v0_8;
        D_80121899 = temp_v0_8;
        D_80121898 = temp_v0_8;
        D_80121876 = temp_v0_8;
        D_80121875 = temp_v0_8;
        D_80121874 = temp_v0_8;
        if (temp_v0_8 == 0) {
            var_v1_9 = 0;
            do {
                temp_lo_6 = var_v1_9 * 0x24;
                var_v1_9 += 1;
                *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_6) + (144)) = 0x82000000;
            } while (var_v1_9 < 4);
            D_800E6280.unk_1104.w += 1;
        }
        goto block_53;
    case 12:
        sp50 = (s32) D_800E6280.unk_110A;
        sp4C = D_800E6280.unk_1104.w;
        read_bustup(D_800E6280.unk_110A);
        D_800E6280.unk_1104.w = sp4C;
        if (sp50 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.w = sp4C + 1;
            D_800E6280.unk_110A -= 1;
        }
        goto block_52;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.w += 1;
        }
        goto block_52;
    case 14:
        temp_v1 = D_800E6280.unk_110A;
        sp4C = D_800E6280.unk_1104.w;
        sp50 = (s32) temp_v1;
        func_80083474();
        if ((temp_v1 + 1) != D_800E6280.unk_110A) {
            D_800E6280.unk_110A = temp_v1;
            D_800E6280.unk_1104.w = 0xC;
        } else {
            D_800E6280.unk_1104.w = sp4C + 1;
            D_800E6280.unk_110A -= 1;
        }
        goto block_52;
    case 15:
        func_800462C8(0xAU, 0x80162000, 0x4359);
        D_800E6280.unk_1104.w += 1;
        goto block_52;
    case 17:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E6280.unk_1104.w -= 2;
            return 0U;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.w += 1;
        goto block_52;
    case 18:
        func_80044750(0x200);
        sp4C = D_800E6280.unk_1104.w;
        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp4C + 1;
        goto block_52;
    case 20:
        sp4C = D_800E6280.unk_1104.w;
        func_8013E980();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp4C + 1;
        goto block_52;
    case 22:
        sp4C = D_800E6280.unk_1104.w;
        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp4C + 1;
        goto block_52;
    default:
        hizuke_disp_switch(1);
        D_80122D2C = 0;
        func_8004284C();
        goto block_52;
    }
}
