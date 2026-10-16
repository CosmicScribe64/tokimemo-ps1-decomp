void func_80143580(void) {
    s32 sp6C;
    s32 sp64;
    s32 *var_v1;
    s32 *var_v1_2;
    GameState *var_a2;
    GameState *var_v0_4;
    Rec24 *var_v1_3;
    s16 temp_t6;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_a0;
    s16 var_a1;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_5;
    s32 var_a1_2;
    s32 var_t0_2;
    s32 var_t0_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    u8 temp_a1_4;
    u8 temp_t2;
    u8 temp_t2_2;
    u8 temp_t2_3;
    u8 temp_v0_2;
    u8 var_t0;
    u8 var_v0_6;

    var_v0 = &sp6C;
    var_v1 = &D_80184000;
    do {
        *var_v0 = (s32) var_v1;
        var_v0 += 1;
    } while ((s32) var_v1 < 0x801A0000);
    var_t0 = D_800E6280.unk_110D;
    switch (var_t0) {
    case 0:
        temp_a1 = *(u8 *)((unsigned char *)D_801217D0 + (2322));
        *(u8 *)((unsigned char *)D_801217D0 + (2321)) = temp_a1;
        *(u8 *)((unsigned char *)D_801217D0 + (2322)) = (u8) (temp_a1 + 1);
        *(u8 *)((unsigned char *)D_801217D0 + (2320)) = temp_a1;
        if (temp_a1 >= 0x81U) {
            D_801217D0[64].unk_00 = 0x01000000;
            D_8014D170 = 0;
            D_800E6280.unk_110D = var_t0 + 1;
        }
        break;
    case 1:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            D_801217D0[64].unk_04 = (s16) (D_801217D0[64].unk_04 - 1);
            if (D_801217D0[64].unk_04 < -0xF0) {
                D_801217D0[64].unk_00 = 0x51000000;
                D_800E6280.unk_110D = var_t0 + 1;
            }
        }
        break;
    case 2:
        temp_a1_2 = *(u8 *)((unsigned char *)D_801217D0 + (2322));
        *(u8 *)((unsigned char *)D_801217D0 + (2321)) = temp_a1_2;
        *(u8 *)((unsigned char *)D_801217D0 + (2322)) = (u8) (temp_a1_2 - 1);
        *(u8 *)((unsigned char *)D_801217D0 + (2320)) = temp_a1_2;
        if (temp_a1_2 != 0) {
            temp_t2 = *(u8 *)((unsigned char *)D_801217D0 + (2358));
            *(u8 *)((unsigned char *)D_801217D0 + (2357)) = temp_t2;
            *(u8 *)((unsigned char *)D_801217D0 + (2358)) = (u8) (temp_t2 + 1);
            *(u8 *)((unsigned char *)D_801217D0 + (2356)) = temp_t2;
            if (temp_t2 >= 0x81U) {
                D_801217D0[64].unk_00 = 0x80000000;
                D_801217D0[65].unk_00 = 0x01000000;
                D_800E6280.unk_110D = var_t0 + 1;
            }
        }
        /* fallthrough */
    case 3:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            D_801217D0[64].unk_04 = (s16) (D_801217D0[64].unk_04 - 1);
            D_801217D0[65].unk_04 = (s16) (D_801217D0[65].unk_04 + 1);
            if (D_801217D0[65].unk_04 >= -0xEF) {
                func_8004A1A0((s32) &sp6C, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000, 0xF, 1, 3, 0);
                *(u8 *)((unsigned char *)D_801217D0 + (2321)) = 0U;
                D_801217D0[64].unk_00 = 0x51000000;
                *(u8 *)((unsigned char *)D_801217D0 + (2322)) = 0U;
                *(u8 *)((unsigned char *)D_801217D0 + (2320)) = 0U;
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
        }
        break;
    case 4:
        func_80143498();
        temp_t2_2 = *(u8 *)((unsigned char *)D_801217D0 + (2358));
        *(u8 *)((unsigned char *)D_801217D0 + (2357)) = temp_t2_2;
        *(u8 *)((unsigned char *)D_801217D0 + (2358)) = (u8) (temp_t2_2 - 1);
        *(u8 *)((unsigned char *)D_801217D0 + (2356)) = temp_t2_2;
        if (temp_t2_2 != 0) {
            temp_a1_3 = *(u8 *)((unsigned char *)D_801217D0 + (2322));
            *(u8 *)((unsigned char *)D_801217D0 + (2321)) = temp_a1_3;
            *(u8 *)((unsigned char *)D_801217D0 + (2322)) = (u8) (temp_a1_3 + 1);
            *(u8 *)((unsigned char *)D_801217D0 + (2320)) = temp_a1_3;
            if (temp_a1_3 >= 0x81U) {
                D_801217D0[65].unk_00 = 0x80000000;
                D_801217D0[64].unk_00 = 0x01000000;
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
        }
        var_t0 = D_800E6280.unk_110D;
        /* fallthrough */
    case 5:
        D_800E6280.unk_110D = var_t0;
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            if (D_801217D0[65].unk_04 < -0xA0) {
                D_801217D0[65].unk_04 = (s16) (D_801217D0[65].unk_04 + 1);
            }
            temp_v0 = D_801217D0[64].unk_04 - 1;
            D_800E6280.unk_110D = var_t0;
            if (D_801217D0[64].unk_04 < -0x140) {
                D_801217D0[64].unk_00 = 0x51000000;
                D_800E6280.unk_110D = var_t0 + 1;
            }
            D_801217D0[64].unk_04 = temp_v0;
            func_8004B19C((s32) temp_v0, 0, 0);
            if (D_801217D0[64].unk_04 == -0x118) {
                func_8004E58C();
                set_kanji_string(-0x90, 0x14, 0, (s32) "そのたもとで卒業の日に、", 0);
                set_kanji_string(-0x90, 0x24, 0, (s32) "女の子からの告白で生まれた", 0);
                set_kanji_string(-0x90, 0x34, 0, (s32) "恋人達は、永遠に幸せな関係に", 0);
                set_kanji_string(-0x90, 0x44, 0, (s32) "なれるという伝説が…", 0);
                k_disp_start(4);
                k_disp_switch(0, 0);
                k_disp_switch(1, 0);
                k_disp_switch(2, 0);
                k_disp_switch(3, 0);
            }
        }
        break;
    case 6:
        if (((D_800E6280.unk_10F8 & 3) == 3) && (D_801217D0[64].unk_04 >= -0x13F)) {
            D_801217D0[64].unk_04 = (s16) (D_801217D0[64].unk_04 - 1);
        }
        temp_a1_4 = *(u8 *)((unsigned char *)D_801217D0 + (2322));
        *(u8 *)((unsigned char *)D_801217D0 + (2321)) = temp_a1_4;
        *(u8 *)((unsigned char *)D_801217D0 + (2322)) = (u8) (temp_a1_4 - 1);
        *(u8 *)((unsigned char *)D_801217D0 + (2320)) = temp_a1_4;
        if (temp_a1_4 == 0) {
            D_801217D0[64].unk_00 = 0x80000000;
            var_v0_2 = &sp6C;
            do {
                var_v1_2 = &D_801B4000;
loop_32:
                *var_v0_2 = (s32) var_v1_2;
                var_v0_2 += 1;
                if ((s32) var_v1_2 < 0x801D0000) {
                    goto loop_32;
                }
                var_v0_2 = &sp6C;
            } while ((s32) (var_v1_2 + 0x4000) < 0x801D0000);
            func_8004A1A0((s32) &sp6C, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000, 0xE, 1, 3, 0);
            *(u8 *)((unsigned char *)D_801217D0 + (2357)) = 0U;
            *(u8 *)((unsigned char *)D_801217D0 + (2358)) = 0U;
            *(u8 *)((unsigned char *)D_801217D0 + (2356)) = 0U;
            var_a2 = &D_800E6280;
            var_v1_3 = D_801217D0;
            var_t0_2 = 0;
            var_a0 = -0xD0;
            var_a1 = 9;
            do {
                temp_t6 = 0x1FF - var_t0_2;
                var_t0_2 += 1;
                var_v1_3->unk_04 = var_a0;
                var_v1_3->unk_0C = var_a1;
                var_a1 += 2;
                var_a0 += 0x50;
                var_v1_3 += 1;
                var_a2 += 1;
                *(s32 *)((unsigned char *)var_v1_3 + (-36)) = 0x01000000;
                *(s16 *)((unsigned char *)var_v1_3 + (-30)) = -0x78;
                *(s16 *)((unsigned char *)var_v1_3 + (-28)) = 0xFF;
                *(s16 *)((unsigned char *)var_v1_3 + (-26)) = 0xF0;
                *(s16 *)((unsigned char *)var_v1_3 + (-18)) = temp_t6;
                *(s16 *)((unsigned char *)var_v1_3 + (-8)) = 0x1000;
                *(s16 *)((unsigned char *)var_v1_3 + (-6)) = 0x1000;
                var_a2->unk_10A4 = 7;
            } while (var_t0_2 != 3);
            D_800E6280.unk_10A5[1] = 8;
            D_801217D0[1].unk_06 = -0x6E;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 7:
        temp_v0_2 = D_801217D0[2].unk_16;
        D_801217D0[2].unk_15 = temp_v0_2;
        D_801217D0[2].unk_14 = temp_v0_2;
        D_801217D0[1].unk_16 = temp_v0_2;
        D_801217D0[1].unk_15 = temp_v0_2;
        D_801217D0[1].unk_14 = temp_v0_2;
        D_801217D0->unk_16 = temp_v0_2;
        D_801217D0->unk_15 = temp_v0_2;
        D_801217D0->unk_14 = temp_v0_2;
        D_801217D0[2].unk_16 = (u8) (temp_v0_2 + 1);
        if (temp_v0_2 >= 0x81U) {
            D_800E6280.unk_110D = var_t0 + 1;
        } else {
            temp_t2_3 = *(u8 *)((unsigned char *)D_801217D0 + (2358));
            *(u8 *)((unsigned char *)D_801217D0 + (2357)) = temp_t2_3;
            *(u8 *)((unsigned char *)D_801217D0 + (2358)) = (u8) (temp_t2_3 + 1);
            *(u8 *)((unsigned char *)D_801217D0 + (2356)) = temp_t2_3;
        }
        break;
    case 8:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            temp_v0_3 = D_801217D0->unk_04;
            D_801217D0->unk_04 = temp_v0_3 - 1;
            if (temp_v0_3 < -0x160) {
                func_80044750(0xD1);
                func_8004E58C();
                cd_read_sectors(0x31U, 0x80180000, 0xA017);
                *(u8 *)((unsigned char *)D_801217D0 + (2320)) = 0U;
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            } else {
                D_801217D0[2].unk_04 = (s16) (D_801217D0[2].unk_04 + 1);
            }
        }
        break;
    case 9:
        temp_a0 = *(u8 *)((unsigned char *)D_801217D0 + (2320));
        if ((u8) *(u8 *)((unsigned char *)D_801217D0 + (2320)) < 0xFEU) {
            *(u8 *)((unsigned char *)D_801217D0 + (2320)) = (u8) ((*(u8 *)((unsigned char *)D_801217D0 + (2320)) + 1) & 0xFF);
            func_8004AC18((s32) temp_a0);
        } else {
            func_8004AC18(0xFF);
            D_801217D0[1].unk_00 = 0x80000000;
            D_801217D0->unk_00 = 0x80000000;
            D_801217D0[65].unk_00 = 0x80000000;
            D_801217D0[64].unk_00 = 0x01000000;
            *(u8 *)((unsigned char *)D_801217D0 + (2321)) = 0x80U;
            *(u8 *)((unsigned char *)D_801217D0 + (2322)) = 0x80U;
            *(u8 *)((unsigned char *)D_801217D0 + (2320)) = 0x80U;
            D_801217D0[2].unk_00 = 0x80000000;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            *(u8 *)((unsigned char *)D_801217D0 + (2356)) = 0xFFU;
        }
        break;
    case 10:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp6C, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000, 0xF, 1, 3, 0);
            D_801217D0[64].unk_00 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000, 0x9DF6);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 11:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            var_t0_3 = 0;
            var_a1_2 = 0x10;
            do {
                sp64 = var_t0_3;
                load_palette((s32 *)0x80180000, var_a1_2 & 0xFF, 1, 2, var_t0_3);
                var_t0_3 += 1;
                var_a1_2 = var_t0_3 + 0x10;
            } while (var_t0_3 < 3);
            col2sepia(0x80180000, D_800E6280.unk_163C, 8, 4, 0);
            load_palette(D_800E6280.unk_163C, 0x10, 1, 2, 0);
            func_80048F64(0x60);
            ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 4U;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = 0x801A2624;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = 0x801A263C;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = 0x801A2610;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 12:
        if (D_800E6280.unk_1104.w == 0xC8) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = (u8) (((DisplayActor44 *)D_8011ECD0)[96].unk_02 | 1);
        }
        temp_a0_2 = *(u8 *)((unsigned char *)D_801217D0 + (2356));
        if (*(u8 *)((unsigned char *)D_801217D0 + (2356)) != 0) {
            *(u8 *)((unsigned char *)D_801217D0 + (2356)) = (u8) ((*(u8 *)((unsigned char *)D_801217D0 + (2356)) - 1) & 0xFF);
            func_8004AC18((s32) temp_a0_2);
        } else if ((u32) D_800E6280.unk_1104.w >= 0x12DU) {
            D_800E6280.unk_110D = var_t0 + 1;
        } else if (!(((DisplayActor44 *)D_8011ECD0)[96].unk_02 & 1)) {
            D_800E6280.unk_110D = var_t0 + 1;
        }
        break;
    case 13:
        do {
            var_v1_4 = 0x80186000;
            var_v0_3 = &sp6C;
loop_63:
            *var_v0_3 = var_v1_4;
            var_v1_4 += 0x4000;
            var_v0_3 += 1;
            if (var_v1_4 < 0x8019E000) {
                goto loop_63;
            }
        } while ((var_v1_4 + 0x4000) < 0x8019E000);
        func_8004A1A0((s32) &sp6C, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        D_801217D0[64].unk_00 = 0x80000000;
        D_801217D0[65].unk_00 = 0x01000000;
        load_palette((s32 *)0x8019E000, 0xF, 1, 2, 0);
        col2sepia(0x8019E000, D_800E6280.unk_163C, 8, 4, 0);
        var_v0_4 = &D_800E6280;
        var_v1_5 = 0;
        do {
            if ((u32) var_v0_4 < (u32) &D_800E6280.unk_044[0].unk_20[0x1C]) {
                *(s32 *)((unsigned char *)var_v0_4 + (6204)) = (s32) var_v0_4->unk_163C[0];
            } else {
                *(s32 *)((unsigned char *)var_v0_4 + (6204)) = (s32) *(&D_8019E000 + var_v1_5);
            }
            var_v0_4 += 4;
            var_v1_5 += 4;
        } while (var_v0_4 != &D_800E6280.unk_1BC[1].unk_0C);
        load_palette((s32 *) D_800E6280.unk_183C, 0xF, 1, 2, 0);
        col2sepia(0x80180200, D_800E6280.unk_163C, 8, 4, 0);
        load_palette(D_800E6280.unk_163C, 0x11, 1, 2, 0);
        func_80048E78();
        func_8004E58C();
        func_80046290(0x39D, 0x060003A9, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        set_kanji_string(-0x90, 0x44, 0, (s32) "あなたが…、", 0);
        k_disp_start(1);
        k_disp_switch(0, 0);
        D_800E6280.unk_1104.w = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 14:
        if (func_800460EC() & 4) {
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 15:
        func_8014472C();
        func_80144908();
        if (D_800E6280.unk_F88 & 0xFFFF) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if ((u32) D_800E6280.unk_1104.w >= 0x101U) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if ((func_800460EC() & 2) && ((u32) D_800E6280.unk_1104.w >= 0x79U)) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 16:
        do {
            var_v1_6 = 0x801BA7F0;
            var_v0_5 = &sp6C;
loop_82:
            *var_v0_5 = var_v1_6;
            var_v1_6 += 0x4000;
            var_v0_5 += 1;
            if (var_v1_6 < 0x801D27F0) {
                goto loop_82;
            }
        } while ((var_v1_6 + 0x4000) < 0x801D27F0);
        func_8004A1A0((s32) &sp6C, 0, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
        D_801217D0[65].unk_00 = 0x80000000;
        D_801217D0[64].unk_00 = 0x01000000;
        load_palette((s32 *)0x801D27F0, 0xE, 1, 2, 0);
        col2sepia(0x801D27F0, (s32 *) D_800E6280.unk_183C, 8, 4, 0);
        load_palette((s32 *) D_800E6280.unk_183C, 0xE, 1, 2, 0);
        col2sepia(0x80180400, D_800E6280.unk_163C, 8, 4, 0);
        load_palette(D_800E6280.unk_163C, 0x12, 1, 2, 0);
        func_80048E78();
        func_80048F64(0x60);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0U;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x41000000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0xA4;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = 0x801D6B60;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = 0x801D6B90;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = 0x801D6B54;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        var_v0_6 = D_800E6280.unk_F5F;
        D_800E6280.unk_1104.w = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        if (var_v0_6 >= 0x11U) {
            var_v0_6 = ((var_v0_6 & 0xF) + 0x10) & 0xFF;
        }
        if ((var_v0_6 == 0x10) || (D_800E6280.unk_F5F = var_v0_6, (((u32) D_800E6280.unk_F80 >> 0x10) != 0))) {
            D_800E6280.unk_F5F = 0x10;
            func_80046290(0x3AA, 0x070003B5, 0xF);
            func_8004E58C();
            set_kanji_string(-0x90, 0x44, 0, (s32) "す・き・で・す", 0);
            k_disp_start(1);
            k_disp_switch(0, 0);
            func_80044750(0x300);
        }
        break;
    case 17:
        if (D_800E6280.unk_F5F == 0x10) {
            if ((func_800460EC() & 4) && ((u32) D_800E6280.unk_1104.w >= 0x21U)) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 3U;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 1;
                D_800E6280.unk_1104.w = 0;
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
        } else {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 3U;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 1;
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D = var_t0 + 1;
        }
        break;
    case 18:
        if ((u32) D_800E6280.unk_1104.w >= 0x41U) {
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D = var_t0 + 1;
        }
        break;
    case 19:
        func_8004AC18(D_800E6280.unk_1104.w);
        if ((u32) D_800E6280.unk_1104.w >= 0xFFU) {
            func_8004284C();
        }
        break;
    }
    func_80144654();
}
