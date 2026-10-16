void func_80145370(void) {
    typedef struct { s32 w[12]; } M2cBlock30; /* m2c memcpy of 48 bytes */
    typedef struct { s32 w[9]; } M2cBlock24; /* m2c memcpy of 36 bytes */
    s32 sp9C;
    s32 sp6C;
    s32 sp4C[1];
    s32 sp48;
    s32 *var_v0_2;
    s32 *var_v1_3;
    s32 *var_v1_4;
    GameState *var_v1;
    GameState *var_v1_2;
    s32 *var_v0;
    s32 temp_t4;
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 var_s0;

    sp9C = D_8014A56C;
    *(M2cBlock30 *)(&sp6C) = *(M2cBlock30 *)(&D_8014A570);
    *(M2cBlock24 *)(&sp48) = *(M2cBlock24 *)(&D_8014A5A0);
    switch (D_800E738A) {
    case 0:
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
        var_v1_3 = &D_8014A4F8;
        var_v0 = &sp6C;
        D_8014A4F4 = sp9C;
        do {
            temp_t9 = *var_v0;
            var_v0 += 4;
            var_v1_3 += 4;
            *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t9;
        } while ((u32) var_v0 < (u32) &sp9C);
        var_v1_4 = &D_8014A52C;
        var_v0_2 = &sp4C;
        D_8014A528 = sp48;
        do {
            temp_t7 = *(s32 *)((unsigned char *)var_v0_2 + (4));
            temp_t5 = *(s32 *)((unsigned char *)var_v0_2 + (8));
            temp_t4 = *(s32 *)((unsigned char *)var_v0_2 + (12));
            temp_t8 = (*(s32 *)var_v0_2);
            var_v0_2 += 0x10;
            var_v1_4 += 0x10;
            *(s32 *)((unsigned char *)var_v1_4 + (-12)) = temp_t7;
            *(s32 *)((unsigned char *)var_v1_4 + (-8)) = temp_t5;
            *(s32 *)((unsigned char *)var_v1_4 + (-4)) = temp_t4;
            *(s32 *)((unsigned char *)var_v1_4 + (-16)) = temp_t8;
        } while (var_v0_2 != &sp6C);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004284C();
        break;
    case 1:
        cd_read_sectors(0x1EU, 0x80180000, 0xBAD0);
        Fade_Out_Init();
        func_8004284C();
        break;
    case 2:
        Fade_Out_Main(0x1E);
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x26U, 0x8018F000, 0xB969);
            func_8004284C();
        }
        break;
    case 3:
        if ((Fade_Out_Main(0x1E) != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80132B40();
            search_load_tpage_buf_lock(D_8014A4F4, 1);
            var_s0 = 0;
            do {
                load_palette(D_80148EF4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 0x10);
            func_8014607C();
            func_80145EF4();
            func_80145F8C();
            cd_read_sectors(0xB9U, 0x8018F000, 0xB8B0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
