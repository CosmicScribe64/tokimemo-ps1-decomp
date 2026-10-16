extern ? D_8014D168;

void func_80140F90(void) {
    GameState *var_a0;
    NameMetricWork *var_a2;
    NameMetricWork *var_a2_2;
    NameMetricWork *var_a2_3;
    s32 temp_v0;
    s32 var_a3;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;
    u8 *temp_v1_4;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 *var_a1_3;
    u8 *var_a1_4;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 temp_a0_4;
    u8 temp_t0;
    u8 temp_t0_2;
    u8 temp_t2;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t9;
    u8 temp_t9_2;
    u8 temp_v0_10;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 temp_v0_9;
    u8 var_t0;
    u8 var_t1;

    var_t1 = D_800E6280.unk_110D;
    switch (var_t1) {
    case 0:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "システムファイルを作り直して", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
        func_8004E788(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
        func_8004E884(6);
        var_a1 = D_8014D158;
        do {
            var_a1 += 4;
            *(s8 *)((unsigned char *)var_a1 + (-4)) = 0;
            *(s8 *)((unsigned char *)var_a1 + (-3)) = 0;
            *(s8 *)((unsigned char *)var_a1 + (-2)) = 0;
            *(s8 *)((unsigned char *)var_a1 + (-1)) = 0;
        } while (var_a1 != &D_8014D168);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    default:
        break;
    case 1:
        temp_v0 = D_800E6280.unk_F88;
        if (temp_v0 & 0x20) {
            var_a2 = &D_800E7D10;
            var_a1_2 = D_8014D158;
            do {
                temp_t2 = var_a2->flags[1];
                temp_t3 = var_a2->flags[2];
                temp_t4 = var_a2->flags[3];
                temp_t9 = var_a2->flags[0];
                var_a2 += 4;
                var_a1_2 += 4;
                *(u8 *)((unsigned char *)var_a1_2 + (-3)) = temp_t2;
                *(u8 *)((unsigned char *)var_a1_2 + (-2)) = temp_t3;
                *(u8 *)((unsigned char *)var_a1_2 + (-1)) = temp_t4;
                *(u8 *)((unsigned char *)var_a1_2 + (-4)) = temp_t9;
            } while (var_a2 != &D_800E7D10.slot_status[0xC]);
            D_800E6280.unk_110D = var_t1 + 1;
        } else if (temp_v0 & 0x40) {
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        func_8004E884(3);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 3:
        if ((D_800E6280.unk_F88 != 0) || ((u32) D_800E6280.unk_1104.w >= 0x41U)) {
            D_800E6280.unk_110D = var_t1 + 1;
        }
        break;
    case 4:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        func_8004E884(2);
        D_800E6280.unk_1104.w = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 5:
        if (D_800E6280.unk_F88 != 0) {
            D_800E6280.unk_110D = var_t1 + 1;
        }
        break;
    case 6:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        func_8004E884(1);
        var_v1 = D_8014D060;
        do {
            var_v1 += 4;
            *(s8 *)((unsigned char *)var_v1 + (-4)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (-3)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (-2)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (-1)) = 0;
        } while (var_v1 != D_8014D070);
        D_800E6280.unk_1118 = 1;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 7:
        var_t0 = D_800E6280.unk_1118;
        if (var_t0 >= 0xFU) {
            var_v1_2 = D_8014D060;
            var_a1_3 = D_8014D158;
            var_a2_2 = &D_800E7D10;
            do {
                temp_a0 = var_a1_3->unk0;
                temp_v0_2 = var_v1_2->unk0;
                if (temp_a0 < temp_v0_2) {
                    var_a2_2->flags[0] = temp_v0_2;
                } else {
                    var_a2_2->flags[0] = temp_a0;
                }
                temp_v0_3 = var_v1_2->unk0;
                if ((temp_v0_3 != 0) && (temp_v0_3 != var_a1_3->unk0)) {
                    var_a2_2->flags[0] = 1;
                }
                temp_a0_2 = var_a1_3[1];
                temp_v0_4 = var_v1_2[1];
                if (temp_a0_2 < temp_v0_4) {
                    var_a2_2->flags[1] = temp_v0_4;
                } else {
                    var_a2_2->flags[1] = temp_a0_2;
                }
                temp_v0_5 = var_v1_2[1];
                var_v1_2 += 2;
                if ((temp_v0_5 != 0) && (temp_v0_5 != var_a1_3[1])) {
                    var_a2_2->flags[1] = 1;
                }
                var_a2_2 += 2;
                var_a1_3 += 2;
            } while (var_a2_2 != &D_800E7D10.slot_status[0xC]);
            D_800E6280.unk_110D = 0x20;
        } else {
            var_a1_4 = D_8014D158;
            if (D_800E7D10.slot_status[var_t0] == 0) {
loop_38:
                temp_t9_2 = (D_800E6280.unk_1118 + 1) & 0xFF;
                var_t0 = temp_t9_2;
                if (temp_t9_2 >= 0xFU) {
                    var_a2_3 = &D_800E7D10;
                    var_v1_3 = D_8014D060;
                    do {
                        temp_a0_3 = var_a1_4->unk0;
                        temp_v0_6 = var_v1_3->unk0;
                        if (temp_a0_3 < temp_v0_6) {
                            var_a2_3->flags[0] = temp_v0_6;
                        } else {
                            var_a2_3->flags[0] = temp_a0_3;
                        }
                        temp_v0_7 = var_v1_3->unk0;
                        if ((temp_v0_7 != 0) && (temp_v0_7 != var_a1_4->unk0)) {
                            var_a2_3->flags[0] = 1;
                        }
                        temp_a0_4 = var_a1_4[1];
                        temp_v0_8 = var_v1_3[1];
                        if (temp_a0_4 < temp_v0_8) {
                            var_a2_3->flags[1] = temp_v0_8;
                        } else {
                            var_a2_3->flags[1] = temp_a0_4;
                        }
                        temp_v0_9 = var_v1_3[1];
                        var_v1_3 += 2;
                        if ((temp_v0_9 != 0) && (temp_v0_9 != var_a1_4[1])) {
                            var_a2_3->flags[1] = 1;
                        }
                        var_a2_3 += 2;
                        var_a1_4 += 2;
                    } while (var_a2_3 != &D_800E7D10.slot_status[0xC]);
                    var_t1 = 0x20;
                } else {
                    D_800E6280.unk_1118 = var_t0;
                    if (D_800E7D10.slot_status[D_800E6280.unk_1118] != 0) {
                        var_t1 = D_800E6280.unk_110D;
                        var_t0 = D_800E6280.unk_1118;
                    } else {
                        goto loop_38;
                    }
                }
            }
            D_800E6280.unk_1118 = var_t0;
            D_800E6280.unk_110D = var_t1;
            if (var_t1 != 0x20) {
                D_800E7D10.selected_slot = var_t0;
                D_800E6280.unk_1118 = var_t0;
                D_800E6280.unk_110D = var_t1 + 1;
                func_80053CE0();
            }
        }
        break;
    case 8:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            temp_v0_10 = D_800E6280.unk_1115;
            if (temp_v0_10 == 0xF0) {
                func_80056284();
                func_80042908(1);
            } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (temp_v0_10 == 0xD0)) {
                func_80053D10();
                func_80042908(0xFF);
            }
        }
        break;
    case 9:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            temp_t0 = D_800E6280.unk_1118;
            D_800E7D10.slot_status[temp_t0] = 0xFF;
            D_800E6280.unk_1118 = temp_t0 + 1;
            D_800E6280.unk_110D = 7;
        }
        break;
    case 10:
        if (func_80055AFC(0) == 1) {
            func_80054E6C();
            D_800E6280.unk_1118 = D_800E6280.unk_1118 + 1;
            var_a0 = &D_800E6280;
            D_800E6280.unk_110D = 7;
            var_a3 = 0;
            do {
                temp_v1 = &D_8014D060[var_a3];
                if (var_a0->unk_53C[0] != 0) {
                    temp_v1->unk0 += 1;
                }
                temp_v1_2 = &D_8014D060[var_a3];
                if (var_a0->unk_53C[1] != 0) {
                    *(u8 *)((unsigned char *)temp_v1_2 + (1)) = (u8) (*(u8 *)((unsigned char *)temp_v1_2 + (1)) + 1);
                }
                temp_v1_3 = &D_8014D060[var_a3];
                if (var_a0->unk_53C[2] != 0) {
                    *(u8 *)((unsigned char *)temp_v1_3 + (2)) = (u8) (*(u8 *)((unsigned char *)temp_v1_3 + (2)) + 1);
                }
                temp_v1_4 = &D_8014D060[var_a3];
                if (var_a0->unk_53C[3] != 0) {
                    *(u8 *)((unsigned char *)temp_v1_4 + (3)) = (u8) (*(u8 *)((unsigned char *)temp_v1_4 + (3)) + 1);
                }
                var_a3 += 4;
                var_a0 += 4;
            } while (var_a3 != 0x10);
        } else {
            temp_t0_2 = D_800E6280.unk_1118;
            D_800E7D10.slot_status[temp_t0_2] = 0xFF;
            D_800E6280.unk_1118 = temp_t0_2 + 1;
            D_800E6280.unk_110D = 7;
        }
        break;
    case 32:
        func_80053CE0();
        D_800E7D10.selected_slot = 0;
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
