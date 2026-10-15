void func_8014280C(void) {
    s32 sp38;
    s32 *sp30;
    GameState *var_v0;
    s32 *var_a3;
    s32 *var_a3_2;
    s32 *var_v1;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_a2;
    s32 var_a2_2;

    switch (D_800E738A) {
    case 0:
        var_a2 = 0;
        if (cd_read_sync_status() & 1) {
            var_a3 = &D_8014A208;
            do {
                sp38 = var_a2;
                sp30 = var_a3;
                temp_v1 = var_a2 + 0x80000100;
                *(&D_800E74A8 + (search_load_tpage_buf_lock(*var_a3, 1, var_a2, var_a3) * 4)) = temp_v1;
                var_a2 += 1;
                var_a3 = &var_a3[1];
                *(s32 *)((unsigned char *)var_a3 + (-4)) = temp_v1;
            } while (var_a2 < 8);
            var_a3_2 = &D_8014A228;
            var_a2_2 = 0;
            do {
                sp38 = var_a2_2;
                sp30 = var_a3_2;
                temp_v1_2 = var_a2_2 + 0x80000200;
                *(&D_800E74A8 + (search_load_tpage_buf_lock(*var_a3_2, 1, var_a2_2, var_a3_2) * 4)) = temp_v1_2;
                var_a2_2 += 1;
                var_a3_2 = &var_a3_2[1];
                *(s32 *)((unsigned char *)var_a3_2 + (-4)) = temp_v1_2;
            } while (var_a2_2 != 0xB);
            func_8004284C();
        }
        break;
    case 1:
        Fade_In_Init();
        func_8004500C(1, 3);
        func_8004284C();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            func_8004284C();
        }
        if (D_800E7208 & 0xF0) {
            Fade_Out_Init();
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_80042940(5);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (D_8012067A >= 0x61) {
            *(s32 *)((unsigned char *)D_8011ECD0 + (6568)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6568)) + 0xFFFE8000);
        } else {
            if ((u8) D_8014A110 == 0) {
                D_80120666 = 1;
            } else {
                D_80120666 = 3;
            }
            switch (D_800E738D) {
            case 0:
                if ((u8) D_8014A110 != 0) {
                    func_80046290(0x1E000050, 0x06000060, 1);
                } else {
                    func_80046290(0x1F000394, 0x440003A4, 0);
                }
                func_80044750(0x300);
                D_800E738D += 1;
                break;
            case 1:
            case 3:
            case 5:
                if (func_800460EC() == 4) {
                    D_800E738D += 1;
                }
                break;
            case 2:
                if (func_800460EC() == 2) {
                    if ((u8) D_8014A110 != 0) {
                        func_80046290(0x1F00009F, 0x140000A9, 1);
                    } else {
                        func_80046290(0x1F0003A5, 0x450003B1, 0);
                    }
                    func_80044750(0x300);
                    D_800E738D += 1;
                }
                break;
            case 4:
                if (func_800460EC() == 2) {
                    if ((u8) D_8014A110 != 0) {
                        func_80046290(0x1F0000AA, 0x150000B7, 1);
                    } else {
                        func_80046290(0x0A0003C3, 0x430003D0, 0);
                    }
                    func_80044750(0x300);
                    D_800E738D += 1;
                }
                break;
            case 6:
                if (func_800460EC() == 2) {
                    Vblnk_Timer_Init();
                    D_800E738D += 1;
                }
                break;
            case 7:
                if (Vblnk_Timer() >= 0xB5) {
                    D_800E738D = 0;
                }
                break;
            }
        }
        if (D_800E7208 & 0xF0) {
            Fade_Out_Init();
            func_80044750(0x74);
            func_8004284C();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 5:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80133C30();
            var_v1 = D_8014A070;
            do {
                sp30 = var_v1;
                search_load_tpage_buf_lock(*var_v1, 1);
                var_v1 = &var_v1[1];
            } while (var_v1 != &D_8014A088);
            func_8004284C();
        }
        break;
    case 6:
        func_80048F64(0x60);
        var_v0 = &D_800E6280;
        do {
            var_v0 += 4;
            var_v0->unk_1228[0x14] = -1;
        } while (var_v0 != &D_800E628C);
        func_801330D4();
        func_80132BD0();
        func_80143814();
        func_80044750(0x505);
        Fade_In_Init();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
}
