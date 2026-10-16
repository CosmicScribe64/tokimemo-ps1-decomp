void func_80141820(void) {
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 *var_v1_3;
    s32 var_a0;
    u8 *temp_v0;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 temp_t9_2;
    u8 var_a1;
    u8 var_a3;
    u8 *temp_v0_2;
    u8 *temp_v0_3;
    u8 *temp_v0_4;

    var_a3 = D_800E738D;
    switch (var_a3) {
    case 0:
        func_8004E58C(&D_800E7D10, var_a3);
        set_kanji_string(-0x40, 0x20, 0xF, "システムファイルを作成して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, &D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, &D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
        k_disp_start(6);
        D_800E738D += 1;
    default:
        break;
    case 1:
        if (D_800E7208 & 0x20) {
            D_800E738D = var_a3 + 1;
        } else if (D_800E7208 & 0x40) {
            func_80056284(&D_800E7D10, var_a3);
            func_80042908(1);
        }
        break;
    case 2:
        func_8004E58C(&D_800E7D10, var_a3);
        set_kanji_string(-0x40, 0x20, 0xF, "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, "抜かないで下さい。", 0);
        k_disp_start(3);
        D_800E7384 = 0;
        D_800E738D += 1;
        break;
    case 3:
        if ((D_800E7208 != 0) || ((u32) D_800E7384 >= 0x41U)) {
            D_800E738D = var_a3 + 1;
        }
        break;
    case 4:
        func_8004E58C(&D_800E7D10, var_a3);
        set_kanji_string(-0x40, 0x20, 0xF, "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, "何かボタンを押してください。", 0);
        k_disp_start(2);
        D_800E738D += 1;
        break;
    case 5:
        if (D_800E7208 != 0) {
            D_800E738D = var_a3 + 1;
        }
        break;
    case 6:
        func_8004E58C(&D_800E7D10, var_a3);
        set_kanji_string(-0x40, 0x20, 0xF, "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        var_v0 = &D_8014D060;
        do {
            var_v0 += 4;
            *(s8 *)((unsigned char *)var_v0 + (-4)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-3)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-2)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while (var_v0 != &D_8014D070);
        D_800E7398 = 1;
        D_800E738D += 1;
        break;
    case 7:
        if (func_80054694(0, &D_800E7D10, var_a3) == 1) {
            D_800E8BEE = 1;
            D_800E738D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 8:
        var_a1 = D_800E7398;
        if (var_a1 >= 0xFU) {
            D_800E738D = 0x20;
            var_v1 = &D_800E7D10;
            var_v0_2 = &D_8014D060;
            do {
                temp_t0 = *(u8 *)((unsigned char *)var_v0_2 + (1));
                temp_t1 = *(u8 *)((unsigned char *)var_v0_2 + (2));
                temp_t2 = *(u8 *)((unsigned char *)var_v0_2 + (3));
                temp_t9 = (*(s32 *)var_v0_2);
                var_v1 += 4;
                var_v0_2 += 4;
                *(u8 *)((unsigned char *)var_v1 + (17)) = temp_t0;
                *(u8 *)((unsigned char *)var_v1 + (18)) = temp_t1;
                *(u8 *)((unsigned char *)var_v1 + (19)) = temp_t2;
                *(u8 *)((unsigned char *)var_v1 + (16)) = temp_t9;
            } while (var_v1 != &D_800E7D20);
        } else {
            if (*(u8 *)((unsigned char *)(&D_800E7D10 + var_a1) + (4)) == 0) {
loop_25:
                temp_t5 = (D_800E7398 + 1) & 0xFF;
                var_a1 = temp_t5;
                if (temp_t5 >= 0xFU) {
                    var_v1_2 = &D_800E7D10;
                    var_v0_3 = &D_8014D060;
                    do {
                        temp_t7 = *(u8 *)((unsigned char *)var_v0_3 + (1));
                        temp_t8 = *(u8 *)((unsigned char *)var_v0_3 + (2));
                        temp_t9_2 = *(u8 *)((unsigned char *)var_v0_3 + (3));
                        temp_t6 = (*(s32 *)var_v0_3);
                        var_v1_2 += 4;
                        var_v0_3 += 4;
                        *(u8 *)((unsigned char *)var_v1_2 + (17)) = temp_t7;
                        *(u8 *)((unsigned char *)var_v1_2 + (18)) = temp_t8;
                        *(u8 *)((unsigned char *)var_v1_2 + (19)) = temp_t9_2;
                        *(u8 *)((unsigned char *)var_v1_2 + (16)) = temp_t6;
                    } while (var_v1_2 != &D_800E7D20);
                    var_a3 = 0x20;
                } else {
                    D_800E7398 = var_a1;
                    if (*(u8 *)((unsigned char *)(&D_800E7D10 + *(u8 *)((unsigned char *)(&D_800E6280) + (4376))) + (4)) != 0) {
                        var_a3 = D_800E738D;
                        var_a1 = D_800E7398;
                    } else {
                        goto loop_25;
                    }
                }
            }
            D_800E7398 = var_a1;
            D_800E738D = var_a3;
            if (var_a3 != 0x20) {
                D_800E7D36 = var_a1;
                D_800E7398 = var_a1;
                D_800E738D = var_a3 + 1;
                func_80053CE0(&D_800E7D20, var_a1, &D_800E7D10, var_a3);
            }
        }
        break;
    case 9:
        if (func_8005448C(&D_800E7D10, var_a3) == 4) {
            func_80053D10();
            D_800E738D += 1;
        } else if (D_800E7395 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 10:
        if (func_80055A38(0, &D_800E7D10, var_a3) == 1) {
            D_800E738D += 1;
        } else {
            *(s8 *)((unsigned char *)(&D_800E7D10 + D_800E7398) + (4)) = 0xFF;
            D_800E7398 += 1;
            D_800E738D = 8;
        }
        break;
    case 11:
        if (func_80055AFC(0, &D_800E7D10, var_a3) == 1) {
            func_80054E6C();
            D_800E7398 += 1;
            var_v1_3 = &D_800E6280;
            D_800E738D = 8;
            var_a0 = 0;
            do {
                if (*(u8 *)((unsigned char *)var_v1_3 + (1340)) != 0) {
                    temp_v0 = &D_8014D060 + var_a0;
                    *temp_v0 += 1;
                }
                if (*(u8 *)((unsigned char *)var_v1_3 + (1341)) != 0) {
                    temp_v0_2 = &D_8014D060 + var_a0;
                    *(u8 *)((unsigned char *)temp_v0_2 + (1)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (1)) + 1);
                }
                if (*(u8 *)((unsigned char *)var_v1_3 + (1342)) != 0) {
                    temp_v0_3 = &D_8014D060 + var_a0;
                    *(u8 *)((unsigned char *)temp_v0_3 + (2)) = (u8) (*(u8 *)((unsigned char *)temp_v0_3 + (2)) + 1);
                }
                if (*(u8 *)((unsigned char *)var_v1_3 + (1343)) != 0) {
                    temp_v0_4 = &D_8014D060 + var_a0;
                    *(u8 *)((unsigned char *)temp_v0_4 + (3)) = (u8) (*(u8 *)((unsigned char *)temp_v0_4 + (3)) + 1);
                }
                var_a0 += 4;
                var_v1_3 += 4;
            } while (var_a0 != 0x10);
        } else {
            *(s8 *)((unsigned char *)(&D_800E7D10 + D_800E7398) + (4)) = 0xFF;
            D_800E7398 += 1;
            D_800E738D = 8;
        }
        break;
    case 32:
        func_80053CE0(&D_800E7D10, var_a3);
        D_800E7D36 = 0;
        D_800E738D += 1;
        break;
    case 33:
        if (func_80054388(&D_800E7D10, var_a3) == 0x13) {
            func_80053D10();
            func_80042908(2);
            func_80042940(0);
            D_800E738D = 2;
            func_80044750(0x50F);
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}
