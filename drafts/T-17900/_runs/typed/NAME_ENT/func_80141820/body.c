void func_80141820(void) {
    GameState *var_v1_3;
    NameMetricWork *var_v1;
    NameMetricWork *var_v1_2;
    s32 temp_v0;
    s32 var_a0;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 temp_t9_2;
    u8 temp_v0_2;
    u8 var_a1;
    u8 var_a3;

    var_a3 = D_800E6280.unk_110D;
    switch (var_a3) {
    case 0:
        func_8004E58C();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを作成して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
        k_disp_start(6);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    default:
        break;
    case 1:
        temp_v0 = D_800E6280.unk_F88;
        if (temp_v0 & 0x20) {
            D_800E6280.unk_110D = var_a3 + 1;
        } else if (temp_v0 & 0x40) {
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_8004E58C();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        k_disp_start(3);
        D_800E6280.unk_1104.w = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 3:
        if ((D_800E6280.unk_F88 != 0) || ((u32) D_800E6280.unk_1104.w >= 0x41U)) {
            D_800E6280.unk_110D = var_a3 + 1;
        }
        break;
    case 4:
        func_8004E58C();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        k_disp_start(2);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 5:
        if (D_800E6280.unk_F88 != 0) {
            D_800E6280.unk_110D = var_a3 + 1;
        }
        break;
    case 6:
        func_8004E58C();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        var_v0 = D_8014D060;
        do {
            var_v0 += 4;
            *(s8 *)((unsigned char *)var_v0 + (-4)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-3)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-2)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while (var_v0 != D_8014D070);
        D_800E6280.unk_1118 = 1;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 7:
        if (func_80054694(0) == 1) {
            D_800E8BEE = 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            func_80042908(4);
        }
        break;
    case 8:
        var_a1 = D_800E6280.unk_1118;
        if (var_a1 >= 0xFU) {
            D_800E6280.unk_110D = 0x20;
            var_v1 = ((NameMetricWork *)&D_800E7D10);
            var_v0_2 = D_8014D060;
            do {
                temp_t0 = var_v0_2[1];
                temp_t1 = var_v0_2[2];
                temp_t2 = var_v0_2[3];
                temp_t9 = (*(s32 *)var_v0_2);
                var_v1 += 4;
                var_v0_2 += 4;
                var_v1->slot_status[0xD] = temp_t0;
                var_v1->slot_status[0xE] = temp_t1;
                var_v1->slot_status[0xF] = temp_t2;
                var_v1->slot_status[0xC] = temp_t9;
            } while (var_v1 != &((NameMetricWork *)&D_800E7D10)->slot_status[0xC]);
        } else {
            if (((NameMetricWork *)&D_800E7D10)->slot_status[var_a1] == 0) {
loop_25:
                temp_t5 = (D_800E6280.unk_1118 + 1) & 0xFF;
                var_a1 = temp_t5;
                if (temp_t5 >= 0xFU) {
                    var_v1_2 = ((NameMetricWork *)&D_800E7D10);
                    var_v0_3 = D_8014D060;
                    do {
                        temp_t7 = var_v0_3[1];
                        temp_t8 = var_v0_3[2];
                        temp_t9_2 = var_v0_3[3];
                        temp_t6 = (*(s32 *)var_v0_3);
                        var_v1_2 += 4;
                        var_v0_3 += 4;
                        var_v1_2->slot_status[0xD] = temp_t7;
                        var_v1_2->slot_status[0xE] = temp_t8;
                        var_v1_2->slot_status[0xF] = temp_t9_2;
                        var_v1_2->slot_status[0xC] = temp_t6;
                    } while (var_v1_2 != &((NameMetricWork *)&D_800E7D10)->slot_status[0xC]);
                    var_a3 = 0x20;
                } else {
                    D_800E6280.unk_1118 = var_a1;
                    if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] != 0) {
                        var_a3 = D_800E6280.unk_110D;
                        var_a1 = D_800E6280.unk_1118;
                    } else {
                        goto loop_25;
                    }
                }
            }
            D_800E6280.unk_1118 = var_a1;
            D_800E6280.unk_110D = var_a3;
            if (var_a3 != 0x20) {
                ((NameMetricWork *)&D_800E7D10)->selected_slot = var_a1;
                D_800E6280.unk_1118 = var_a1;
                D_800E6280.unk_110D = var_a3 + 1;
                func_80053CE0();
            }
        }
        break;
    case 9:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            temp_v0_2 = D_800E6280.unk_1115;
            if (temp_v0_2 == 0xF0) {
                func_80056284();
                func_80042908(1);
            } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (temp_v0_2 == 0xD0)) {
                func_80053D10();
                func_80042908(0xFF);
            }
        }
        break;
    case 10:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            temp_a1 = D_800E6280.unk_1118;
            ((NameMetricWork *)&D_800E7D10)->slot_status[temp_a1] = 0xFF;
            D_800E6280.unk_1118 = temp_a1 + 1;
            D_800E6280.unk_110D = 8;
        }
        break;
    case 11:
        if (func_80055AFC(0) == 1) {
            func_80054E6C();
            D_800E6280.unk_1118 = D_800E6280.unk_1118 + 1;
            var_v1_3 = &D_800E6280;
            D_800E6280.unk_110D = 8;
            var_a0 = 0;
            do {
                if (var_v1_3->unk_53C[0] != 0) {
                    temp_v0_3 = &D_8014D060[var_a0];
                    (*(s32 *)temp_v0_3) += 1;
                }
                if (var_v1_3->unk_53C[1] != 0) {
                    temp_v0_4 = &D_8014D060[var_a0];
                    *(u8 *)((unsigned char *)temp_v0_4 + (1)) = (u8) (*(u8 *)((unsigned char *)temp_v0_4 + (1)) + 1);
                }
                if (var_v1_3->unk_53C[2] != 0) {
                    temp_v0_5 = &D_8014D060[var_a0];
                    *(u8 *)((unsigned char *)temp_v0_5 + (2)) = (u8) (*(u8 *)((unsigned char *)temp_v0_5 + (2)) + 1);
                }
                if (var_v1_3->unk_53C[3] != 0) {
                    temp_v0_6 = &D_8014D060[var_a0];
                    *(u8 *)((unsigned char *)temp_v0_6 + (3)) = (u8) (*(u8 *)((unsigned char *)temp_v0_6 + (3)) + 1);
                }
                var_a0 += 4;
                var_v1_3 += 4;
            } while (var_a0 != 0x10);
        } else {
            temp_a1_2 = D_800E6280.unk_1118;
            ((NameMetricWork *)&D_800E7D10)->slot_status[temp_a1_2] = 0xFF;
            D_800E6280.unk_1118 = temp_a1_2 + 1;
            D_800E6280.unk_110D = 8;
        }
        break;
    case 32:
        func_80053CE0();
        ((NameMetricWork *)&D_800E7D10)->selected_slot = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 33:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(2);
            func_80042940(0);
            D_800E6280.unk_110D = 2;
            func_80044750(0x50F);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}
