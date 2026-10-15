void func_80134060(void) {
    s32 sp40[5];
    s32 *sp3C;
    s32 *var_a2;
    s32 *var_a2_2;
    s32 *var_a2_3;
    s32 *var_v1;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;

    sp68[0].unk0 = (s32) D_801493C0.unk0;
    *(s32 *)((unsigned char *)(&sp68[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801493C0) + (4));
    *(s32 *)((unsigned char *)(&sp68[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801493C0) + (8));
    *(s32 *)((unsigned char *)(&sp68[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801493C0) + (12));
    *(u16 *)((unsigned char *)(&sp68[0]) + (16)) = (u16) *(u16 *)((unsigned char *)(&D_801493C0) + (16));
    sp54[0].unk0 = (s32) D_801493D4.unk0;
    *(s32 *)((unsigned char *)(&sp54[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801493D4) + (4));
    *(s32 *)((unsigned char *)(&sp54[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801493D4) + (8));
    *(s32 *)((unsigned char *)(&sp54[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801493D4) + (12));
    *(u16 *)((unsigned char *)(&sp54[0]) + (16)) = (u16) *(u16 *)((unsigned char *)(&D_801493D4) + (16));
    (*(s32 *)&sp40) = (s32) D_801493E8.unk0;
    *(s32 *)((unsigned char *)(&sp40) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801493E8) + (4));
    *(s32 *)((unsigned char *)(&sp40) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801493E8) + (8));
    *(u16 *)((unsigned char *)(&sp40) + (16)) = (u16) *(u16 *)((unsigned char *)(&D_801493E8) + (16));
    *(s32 *)((unsigned char *)(&sp40) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801493E8) + (12));
    switch (D_800E738A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            *(s32 *)((unsigned char *)(&D_800E6280 + (search_load_tpage_buf_lock(D_801492F4, 1) * 4)) + (4648)) = 0x80000206;
            D_801492F4 = 0x80000206;
            var_a2 = &D_801492C8;
            var_s0 = 0;
            do {
                sp3C = var_a2;
                temp_v1 = var_s0 + 0x80000100;
                var_s0 += 1;
                *(s32 *)((unsigned char *)(&D_800E6280 + (search_load_tpage_buf_lock(*var_a2, 1, var_a2) * 4)) + (4648)) = temp_v1;
                var_a2 += 4;
                *(s32 *)((unsigned char *)var_a2 + (-4)) = temp_v1;
            } while (var_s0 < 5);
            var_a2_2 = &D_801492DC;
            var_s0_2 = 0;
            do {
                sp3C = var_a2_2;
                temp_v1_2 = var_s0_2 + 0x80000200;
                var_s0_2 += 1;
                *(s32 *)((unsigned char *)(&D_800E6280 + (search_load_tpage_buf_lock(*var_a2_2, 1, var_a2_2) * 4)) + (4648)) = temp_v1_2;
                var_a2_2 += 4;
                *(s32 *)((unsigned char *)var_a2_2 + (-4)) = temp_v1_2;
            } while (var_s0_2 < 6);
            var_a2_3 = &D_801492F8;
            var_s0_3 = 0;
            do {
                sp3C = var_a2_3;
                temp_v1_3 = var_s0_3 + 0x80000300;
                var_s0_3 += 1;
                *(s32 *)((unsigned char *)(&D_800E6280 + (search_load_tpage_buf_lock(*var_a2_3, 1, var_a2_3) * 4)) + (4648)) = temp_v1_3;
                var_a2_3 += 4;
                *(s32 *)((unsigned char *)var_a2_3 + (-4)) = temp_v1_3;
            } while (var_s0_3 != 0xA);
            func_8004284C();
        }
        break;
    case 1:
        Fade_In_Init();
        func_80044750(0x204);
        func_8004284C();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            func_8004284C();
        }
        if (D_80120652 == 0) {
            temp_v0 = func_800AE0D0();
            D_80120652 = 5;
            D_80120653 = 0xA4;
            temp_hi = temp_v0 % 9;
            D_80120676 = (&sp68[0])[temp_hi];
            D_8012067A = (&sp54[0])[temp_hi];
            D_80120668 = 0;
            D_80120666 = *(s16 *)((unsigned char *)(sp + (temp_hi * 2)) + (64));
        }
        if (D_800E7208 & 0xF0) {
            Fade_Out_Init();
            func_80042940(4);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (D_80120652 == 0) {
            temp_v0_2 = func_800AE0D0();
            D_80120652 = 5;
            D_80120653 = 0xA4;
            temp_hi_2 = temp_v0_2 % 9;
            D_80120676 = (&sp68[0])[temp_hi_2];
            D_8012067A = (&sp54[0])[temp_hi_2];
            D_80120668 = 0;
            D_80120666 = *(s16 *)((unsigned char *)(sp + (temp_hi_2 * 2)) + (64));
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
            cd_read_sectors(0x87, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 5:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80048F64(0x60);
            D_800E74FC = -1;
            func_80133C30();
            var_v1 = &D_8014A070;
            do {
                sp3C = var_v1;
                search_load_tpage_buf_lock(*var_v1, 1);
                var_v1 += 4;
            } while (var_v1 != &D_8014A088);
            func_8004284C();
        }
        break;
    case 6:
        func_801330D4();
        func_80132BD0();
        func_80135830();
        func_80135AD0();
        func_80135DF8();
        func_80135F3C();
        func_80044750(0x505);
        Fade_In_Init();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
}
