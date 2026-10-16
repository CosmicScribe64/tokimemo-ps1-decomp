void func_801330E4(void) {
    ? sp78;
    s16 sp58;
    s16 sp54;
    ? sp30;
    s16 *temp_v1;
    s16 var_a0;
    s16 var_v1;
    u32 temp_t1;
    u8 var_v0;

    M2C_MEMCPY_ALIGNED(&sp30, &D_8013C3B0, 0x24);
    D_800E6280.unk_03E = 0x62;
    D_800E6280.unk_03F = 3;
    D_800E6280.unk_040 = 1;
    D_800E6280.unk_041 = 6;
    func_80053418((s32) &sp78, (s32) &sp58);
    D_800E6280.unk_F5F = 0xD;
    if ((sp58 < 0x1F4) && ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x46U) && (D_800E6280.unk_56C[0x4F] == 2)) {
        D_800E6280.unk_F5F = 0xC;
    } else {
        var_a0 = 0x1F3;
        var_v0 = 0;
        do {
            temp_v1 = &(&sp58)[var_v0];
            if ((D_800E6280.unk_1BC[var_v0].unk_0C.w & 2) && (var_a0 < *temp_v1)) {
                D_800E6280.unk_F5F = var_v0;
                var_a0 = *temp_v1;
            }
            var_v0 = (u8) (s16) (var_v0 + 1);
        } while ((s32) var_v0 < 0xB);
    }
    if (D_800E6280.unk_F5F == 0xD) {
        temp_t1 = (u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D;
        var_v1 = temp_t1 + ((D_800E6280.unk_56C[0x4C] != 0) * 2) + ((D_800E6280.unk_56C[0x4E] != 0) * 2);
        if (var_v1 >= 0x12) {
            var_v1 = 0x11;
        }
        if (temp_t1 != 0) {
            if (D_800E6280.unk_56C[0x4D] == 0) {
                sp54 = var_v1;
                if ((func_800AE0D0() % 256) < *(s16 *)((unsigned char *)(sp + (var_v1 * 2)) + (48))) {
                    D_800E6280.unk_F5F = 0xB;
                }
            }
        }
    }
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    func_800AE0F0(D_800CA1DC, "教室");
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80132000();
    D_800CA160 = D_8013C28C;
    D_800CA164 = D_8013C2D0;
    D_800CA168 = D_8013C314;
    func_8004284C();
}
