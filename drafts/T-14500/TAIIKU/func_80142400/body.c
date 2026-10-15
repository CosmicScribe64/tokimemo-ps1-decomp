void func_80142400(void) {
    s32 sp92;
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 spB8[1];
    s16 spA4;
    s32 sp94[1];
    s16 sp90;
    s32 sp70;
    s32 sp44;
    s32 *var_v0_4;
    s32 *var_v1_3;
    s32 *var_v1_4;
    s32 *var_v1_6;
    GameState *var_v1;
    GameState *var_v1_2;
    s16 *var_v0_3;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t6;
    s16 temp_t7;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 temp_t0;
    s32 temp_t8;
    s32 var_s1;
    u8 *var_s0;
    u8 *var_v1_5;

    spA4 = (s32) D_8014A27C;
    *(s32 *)((unsigned char *)(&spA4) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (8));
    *(s32 *)((unsigned char *)(&spA4) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (4));
    *(s32 *)((unsigned char *)(&spA4) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (12));
    *(s32 *)((unsigned char *)(&spA4) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (16));
    sp90 = (s32) D_8014A290;
    *(s32 *)((unsigned char *)(&sp90) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (4));
    *(s32 *)((unsigned char *)(&sp90) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (8));
    *(s32 *)((unsigned char *)(&sp90) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (12));
    *(s32 *)((unsigned char *)(&sp90) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (16));
    sp70 = D_8014A2A4;
    *(s32 *)((unsigned char *)(&sp70) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (4));
    *(s32 *)((unsigned char *)(&sp70) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (12));
    *(s32 *)((unsigned char *)(&sp70) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (8));
    *(s32 *)((unsigned char *)(&sp70) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (16));
    *(s32 *)((unsigned char *)(&sp70) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (20));
    *(s32 *)((unsigned char *)(&sp70) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (28));
    *(s32 *)((unsigned char *)(&sp70) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (24));
    *(M2cBlock28 *)(&sp44) = *(M2cBlock28 *)(&D_8014A2C4);
    *(s32 *)((unsigned char *)(&sp44 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2C4 + 0x24) + (4));
    switch (D_800E738A) {
    case 0:
        if ((u8) D_8014A110 == 1) {
            func_80044750(0xBF);
        }
        func_80044890(0, 0xBF98, 0xBF79, 0xC20F, 0xC1EF, 0xD6B7);
        func_80085E30(1, 1);
        var_v1 = &D_800E6280;
        do {
            var_v1 += 4;
            var_v1->unk_1228[4] = 8;
        } while ((u32) var_v1 < (u32) &D_800E62AC);
        var_v1_2 = &D_800E6280;
        do {
            var_v1_2 += 4;
            var_v1_2->unk_1228[0x24] = 8;
        } while ((u32) var_v1_2 < (u32) &D_800E628C);
        var_v1_3 = &D_8014A208;
        var_v0 = &sp70;
        do {
            temp_t8 = *var_v0;
            var_v0 += 4;
            var_v1_3 += 4;
            *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t8;
        } while ((u32) var_v0 < (u32) &sp90);
        var_v1_4 = &D_8014A228;
        var_v0_2 = &sp44;
        do {
            temp_t0 = *var_v0_2;
            var_v0_2 += 4;
            var_v1_4 += 4;
            *(s32 *)((unsigned char *)var_v1_4 + (-4)) = temp_t0;
        } while ((u32) var_v0_2 < (u32) &sp70);
        var_v1_5 = D_8014A1D4;
        var_v0_3 = &spA4;
        do {
            temp_t2 = *var_v0_3;
            var_v0_3 += 2;
            var_v1_5 += 2;
            *(s16 *)((unsigned char *)var_v1_5 + (-2)) = temp_t2;
        } while ((u32) var_v0_3 < (u32) &spB8);
        var_v1_6 = &D_8014A1EC;
        var_v0_4 = &sp94;
        D_8014A1E8 = sp90;
        *(s16 *)((unsigned char *)(&D_8014A1E8) + (2)) = sp92;
        do {
            temp_t4 = *(s16 *)((unsigned char *)var_v0_4 + (2));
            temp_t7 = *(s16 *)((unsigned char *)var_v0_4 + (4));
            temp_t6 = *(s16 *)((unsigned char *)var_v0_4 + (6));
            temp_t3 = (*(s32 *)var_v0_4);
            var_v0_4 += 8;
            var_v1_6 += 8;
            *(s16 *)((unsigned char *)var_v1_6 + (-6)) = temp_t4;
            *(s16 *)((unsigned char *)var_v1_6 + (-4)) = temp_t7;
            *(s16 *)((unsigned char *)var_v1_6 + (-2)) = temp_t6;
            *(s16 *)((unsigned char *)var_v1_6 + (-8)) = temp_t3;
        } while (var_v0_4 != &spA4);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            cd_read_sectors(0x1EU, 0x80180000, 0xBAD0);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x4FU, 0x8018F000, 0xB861);
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 3:
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80145190();
            var_s0 = &D_8014A1FC;
            do {
                search_load_tpage_buf_lock(*var_s0, 1);
                var_s0 += 4;
            } while ((u32) var_s0 < (u32) &D_8014A208);
            var_s1 = 0;
            do {
                load_palette(D_8014A378, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
                var_s1 += 1;
            } while (var_s1 != 0x10);
            func_8014357C();
            func_80143398();
            func_80143470();
            cd_read_sectors(0xA1U, 0x8018F000, 0xB7C0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
