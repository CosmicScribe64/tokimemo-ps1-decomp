? func_800485BC(s32, ?);                            /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0(?);                               /* extern */
extern ? D_800E628C;
extern ? D_800E62AC;
extern u8 D_800E738A;
extern ? D_8014A4F8;
extern s32 D_8014A528;
extern ? D_8014A52C;
extern s32 D_8014A56C;
extern ? D_8014A570;
extern ? D_8014A5A0;

void func_80145370(void) {
    s32 sp9C;
    s32 sp6C;
    ? sp4C;
    s32 sp48;
    ? *var_v0_2;
    ? *var_v1_3;
    ? *var_v1_4;
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
    M2C_MEMCPY_ALIGNED(&sp6C, &D_8014A570, 0x30);
    M2C_MEMCPY_ALIGNED(&sp48, &D_8014A5A0, 0x24);
    switch (D_800E738A) {                           /* irregular */
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
            temp_t8 = var_v0_2->unk0;
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
        func_800862D0();
        func_8004284C();
        break;
    case 2:
        func_800862F0(0x1E);
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x26U, 0x8018F000, 0xB969);
            func_8004284C();
        }
        break;
    case 3:
        if ((func_800862F0(0x1E) != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80132B40();
            func_800485BC(D_8014A4F4, 1);
            var_s0 = 0;
            do {
                func_80043914(D_80148EF4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
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
