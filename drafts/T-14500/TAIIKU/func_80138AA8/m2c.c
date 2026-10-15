s32 func_800485BC(s32, ?);                          /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0(?, s32 *);                        /* extern */
extern ? D_800E628C;
extern u8 D_800E738A;
extern ? D_800E74A8;
extern s32 D_800E7518;
extern s32 D_80149DA4;
extern ? D_80149DAC;
extern s32 D_80149DF4;
extern ? D_80149E2C;
extern ? D_80149E34;
extern ? D_80149E7C;

void func_80138AA8(void) {
    ? spAC;
    s32 spA4;
    s32 sp5C;
    s32 sp48;
    ? *var_v1_3;
    GameState *var_v1;
    GameState *var_v1_2;
    s32 *temp_v0;
    s32 *temp_v1;
    s32 *var_s0;
    s32 *var_s0_2;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_v1_2;
    s32 var_s1;
    s32 var_s1_2;

    spA4.unk0 = D_80149E2C.unk0;
    *(s32 *)((unsigned char *)(&spA4) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149E2C) + (4));
    M2C_MEMCPY_ALIGNED(&sp5C, &D_80149E34, 0x48);
    sp48.unk0 = D_80149E7C.unk0;
    *(s32 *)((unsigned char *)(&sp48) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149E7C) + (4));
    *(s32 *)((unsigned char *)(&sp48) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80149E7C) + (8));
    *(s32 *)((unsigned char *)(&sp48) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80149E7C) + (12));
    *(s32 *)((unsigned char *)(&sp48) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80149E7C) + (16));
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_80085E30(1, 1);
        var_v1 = &D_800E6280;
        do {
            var_v1 += 4;
            var_v1->unk_1228[4] = 8;
        } while ((u32) var_v1 < (u32) &D_800E628C);
        var_v1_2 = &D_800E6280;
        do {
            var_v1_2 += 4;
            var_v1_2->unk_1228[0x24] = 8;
        } while ((u32) var_v1_2 < (u32) &D_800E628C);
        D_800E7518 = 8;
        func_80048EB8(0);
        func_8006BD6C(0);
        var_s0 = &D_80149DA4;
        var_v0 = &spA4;
        do {
            temp_t4 = *var_v0;
            var_v0 += 4;
            var_s0 += 4;
            *(s32 *)((unsigned char *)var_s0 + (-4)) = temp_t4;
        } while ((u32) var_v0 < (u32) &spAC);
        var_v1_3 = &D_80149DAC;
        var_v0_2 = &sp5C;
        do {
            temp_t3 = *var_v0_2;
            var_v0_2 += 4;
            var_v1_3 += 4;
            *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t3;
        } while ((u32) var_v0_2 < (u32) &spA4);
        D_80149DF4 = sp48;
        temp_v1 = &(&sp48)[1];
        temp_v0 = &(&D_80149DF4)[1];
        *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (4));
        *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (8));
        *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (12));
        temp_v0->unk0 = temp_v1->unk0;
        func_8004284C();
        break;
    case 1:
        cd_read_sectors(0x1EU, 0x80180000, 0xBAD0);
        func_800862D0();
        func_8004284C();
        break;
    case 2:
        func_800862F0(0x1E, &sp48);
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x27U, 0x8018F000, 0xBAEE);
            func_8004284C();
        }
        break;
    case 3:
        if ((func_800862F0(0x1E, &sp48) != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80141BB0();
            var_s0_2 = &D_80149DA4;
            var_s1 = 0;
            do {
                temp_v1_2 = var_s1 + 0x80000300;
                var_s1 += 1;
                *(&D_800E74A8 + (func_800485BC(*var_s0_2, 1) * 4)) = temp_v1_2;
                var_s0_2 += 4;
                *(s32 *)((unsigned char *)var_s0_2 + (-4)) = temp_v1_2;
            } while (var_s1 < 2);
            var_s1_2 = 0;
            do {
                func_80043914(D_8014A0B4, (var_s1_2 + 0x10) & 0xFF, 1, 1, var_s1_2);
                var_s1_2 += 1;
            } while (var_s1_2 != 0x10);
            cd_read_sectors(0xB9U, 0x8018F000, 0xBA17);
            func_80139AB0();
            func_80139DDC();
            func_80139E34();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
