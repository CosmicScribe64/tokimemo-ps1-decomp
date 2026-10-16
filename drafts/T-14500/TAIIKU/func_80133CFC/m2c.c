? func_800485BC(s32, ?);                            /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0(?);                               /* extern */
extern ? D_800E628C;
extern ? D_800E62AC;
extern u8 D_800E738A;
extern s32 D_800E7518;
extern s32 D_800E75A4;
extern ? D_80149300;
extern s32 D_80149364;
extern ? D_80149368;
extern ? D_8014937C;
extern ? D_80149398;

void func_80133CFC(void) {
    s32 sp9C;
    s32 sp88;
    s32 sp6C;
    ? sp4C;
    s32 sp44;
    ? *var_v0_5;
    ? *var_v1_3;
    GameState *var_v0;
    GameState *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_4;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t9;
    s32 var_s0;

    sp9C = D_80149364;
    sp88.unk0 = D_80149368.unk0;
    *(s32 *)((unsigned char *)(&sp88) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149368) + (4));
    *(s32 *)((unsigned char *)(&sp88) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80149368) + (8));
    *(s32 *)((unsigned char *)(&sp88) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80149368) + (12));
    *(s32 *)((unsigned char *)(&sp88) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80149368) + (16));
    sp6C.unk0 = D_8014937C.unk0;
    *(s32 *)((unsigned char *)(&sp6C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (8));
    *(s32 *)((unsigned char *)(&sp6C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (4));
    *(s32 *)((unsigned char *)(&sp6C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (12));
    *(s32 *)((unsigned char *)(&sp6C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (16));
    *(s32 *)((unsigned char *)(&sp6C) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (24));
    *(s32 *)((unsigned char *)(&sp6C) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8014937C) + (20));
    M2C_MEMCPY_ALIGNED(&sp44, &D_80149398, 0x28);
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_80085E30(1, 1);
        func_80048EB8(0);
        func_8006BD6C(0);
        var_v0 = &D_800E6280;
        do {
            var_v0 += 4;
            var_v0->unk_1228[4] = 8;
        } while ((u32) var_v0 < (u32) &D_800E62AC);
        var_v0_2 = &D_800E6280;
        do {
            var_v0_2 += 4;
            var_v0_2->unk_1228[0x24] = 8;
        } while ((u32) var_v0_2 < (u32) &D_800E628C);
        D_800E7518 = 8;
        D_800E75A4 = 8;
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
        var_v1_3 = &D_80149300;
        var_v0_5 = &sp4C;
        D_801492F8->unk0 = sp44;
        D_801492F8[1] = sp48;
        do {
            temp_t9 = *(s32 *)((unsigned char *)var_v0_5 + (4));
            temp_t0 = *(s32 *)((unsigned char *)var_v0_5 + (8));
            temp_t5 = *(s32 *)((unsigned char *)var_v0_5 + (12));
            temp_t1 = var_v0_5->unk0;
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
        func_800862D0();
        func_8004284C();
        break;
    case 2:
        func_800862F0(0x1E);
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x1FU, 0x8018F000, 0xB79F);
            func_8004284C();
        }
        break;
    case 3:
        if ((func_800862F0(0x1E) != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_801407C0();
            func_800485BC(D_801492C4, 1);
            var_s0 = 0;
            do {
                func_80043914(D_80149FA4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
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
