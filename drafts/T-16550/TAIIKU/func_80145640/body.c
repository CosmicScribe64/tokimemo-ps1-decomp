void func_80145640(void) {
    s32 sp38;
    s32 *sp30;
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
            var_a3 = &D_8014A4F8;
            do {
                sp38 = var_a2;
                sp30 = var_a3;
                temp_v1 = var_a2 + 0x80000100;
                *(&D_800E74A8 + (search_load_tpage_buf_lock(*var_a3, 1, var_a2, var_a3) * 4)) = temp_v1;
                var_a2 += 1;
                var_a3 = &var_a3[1];
                *(s32 *)((unsigned char *)var_a3 + (-4)) = temp_v1;
            } while (var_a2 < 0xC);
            var_a3_2 = &D_8014A53C;
            var_a2_2 = 5;
            do {
                sp38 = var_a2_2;
                sp30 = var_a3_2;
                temp_v1_2 = var_a2_2 + 0x80000200;
                *(&D_800E74A8 + (search_load_tpage_buf_lock(*var_a3_2, 1, var_a2_2, var_a3_2) * 4)) = temp_v1_2;
                var_a2_2 += 1;
                var_a3_2 = &var_a3_2[1];
                *(s32 *)((unsigned char *)var_a3_2 + (-4)) = temp_v1_2;
            } while (var_a2_2 != 9);
            func_8004284C();
        }
        break;
    case 1:
        func_80044750(0x201);
        Fade_In_Init();
        func_8004284C();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            func_8004284C();
        }
        if (D_800E7208 & 0xF0) {
            Fade_Out_Init();
            func_80042940(4);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        switch (D_800E738D) {
        case 0:
            func_80046290(0x1F000330, 0x3F000356, 0);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:
        case 3:
            if (func_800460EC() == 4) {
                D_800E738D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80046290(0x1F000357, 0x40000360, 0);
                func_80044750(0x300);
                D_800E738D += 1;
            }
            break;
        case 4:
            if (func_800460EC() == 2) {
                Vblnk_Timer_Init();
                D_800E738D += 1;
            }
            break;
        case 5:
            if (Vblnk_Timer() >= 0xB5) {
                D_800E738D = 0;
            }
            break;
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
        func_801330D4();
        D_800E74FC = -1;
        func_80132BD0();
        func_80146338();
        func_80044750(0x505);
        Fade_In_Init();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
}
