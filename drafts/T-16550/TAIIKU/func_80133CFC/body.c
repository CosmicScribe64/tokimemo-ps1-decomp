void func_80133CFC(void) {
    s32 sp48;
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 sp9C;
    s32 sp88;
    s32 sp6C;
    s32 sp4C[1];
    s32 sp44;
    s32 *var_v0_5;
    GameState *var_v0;
    GameState *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_4;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 *var_v1_3;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t9;
    s32 var_s0;

    sp9C = D_80149364;
    sp88 = D_80149368.v[0];
    *(s32 *)((unsigned char *)(&sp88) + (4)) = (s32) D_80149368.v[1];
    *(s32 *)((unsigned char *)(&sp88) + (8)) = (s32) D_80149368.v[2];
    *(s32 *)((unsigned char *)(&sp88) + (12)) = (s32) D_80149368.v[3];
    *(s32 *)((unsigned char *)(&sp88) + (16)) = (s32) D_80149368.v[4];
    sp6C = D_8014937C.v[0];
    *(s32 *)((unsigned char *)(&sp6C) + (8)) = (s32) D_8014937C.v[2];
    *(s32 *)((unsigned char *)(&sp6C) + (4)) = (s32) D_8014937C.v[1];
    *(s32 *)((unsigned char *)(&sp6C) + (12)) = (s32) D_8014937C.v[3];
    *(s32 *)((unsigned char *)(&sp6C) + (16)) = (s32) D_8014937C.v[4];
    *(s32 *)((unsigned char *)(&sp6C) + (24)) = (s32) D_8014937C.v[6];
    *(s32 *)((unsigned char *)(&sp6C) + (20)) = (s32) D_8014937C.v[5];
    *(M2cBlock28 *)(&sp44) = *(M2cBlock28 *)(&D_80149398);
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 1);
        func_80048EB8(0);
        func_8006BD6C(0);
        var_v0 = &D_800E6280;
        do {
            var_v0 += 4;
            var_v0->unk_1228[4] = 8;
        } while ((u32) var_v0 < (u32) &D_800E6280.unk_01C[1].unk_04);
        var_v0_2 = &D_800E6280;
        do {
            var_v0_2 += 4;
            var_v0_2->unk_1228[0x24] = 8;
        } while ((u32) var_v0_2 < (u32) &D_800E6280.unk_006[6]);
        D_800E6280.unk_1228[0x1C] = 8;
        D_800E6280.unk_1228[0x3F] = 8;
        var_v1 = D_801492C8;
        var_v0_3 = &sp88;
        D_801492C4 = sp9C;
        do {
            temp_t3 = *var_v0_3;
            var_v0_3 += 4;
            var_v1 += 4;
            *(s32 *)((unsigned char *)var_v1 + (-4)) = temp_t3;
        } while ((u32) var_v0_3 < (u32) &sp9C);
        var_v1_2 = D_801492DC;
        var_v0_4 = &sp6C;
        do {
            temp_t6 = *var_v0_4;
            var_v0_4 += 4;
            var_v1_2 += 4;
            *(s32 *)((unsigned char *)var_v1_2 + (-4)) = temp_t6;
        } while ((u32) var_v0_4 < (u32) &sp88);
        var_v1_3 = &D_801492F8[2];
        var_v0_5 = &sp4C;
        (*(s32 *)D_801492F8) = sp44;
        D_801492F8[1] = sp48;
        do {
            temp_t9 = *(s32 *)((unsigned char *)var_v0_5 + (4));
            temp_t0 = *(s32 *)((unsigned char *)var_v0_5 + (8));
            temp_t5 = *(s32 *)((unsigned char *)var_v0_5 + (12));
            temp_t1 = (*(s32 *)var_v0_5);
            var_v0_5 += 0x10;
            var_v1_3 += 0x10;
            *(s32 *)((unsigned char *)var_v1_3 + (-12)) = temp_t9;
            *(s32 *)((unsigned char *)var_v1_3 + (-8)) = temp_t0;
            *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t5;
            *(s32 *)((unsigned char *)var_v1_3 + (-16)) = temp_t1;
        } while (var_v0_5 != &sp6C);
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
            cd_read_sectors(0x1FU, 0x8018F000, 0xB79F);
            func_8004284C();
        }
        break;
    case 3:
        if ((Fade_Out_Main(0x1E) != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_801407C0();
            search_load_tpage_buf_lock(D_801492C4, 1);
            var_s0 = 0;
            do {
                load_palette(D_80149FA4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 0x10);
            func_80135480();
            func_801356E0();
            func_80135778();
            cd_read_sectors(0xC1U, 0x8018F000, 0xB6DE);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
