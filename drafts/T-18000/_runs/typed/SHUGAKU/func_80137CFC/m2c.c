? func_800639D8();                                  /* extern */
? func_80065048(s16, ?);                            /* extern */
extern s8 D_800CA2BA;

void func_80137CFC(void) {
    s16 var_a0;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_lo;
    u8 *temp_t2;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8008585C();
    func_800AE0F0(D_800CA19C, "修学旅行");
    func_8004E9F4(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0x80;
    D_800B5940 = 0;
    func_800649D4();
    func_80064E84();
    func_8006509C();
    var_s0 = 0;
    var_a0 = 0;
    do {
        func_80065048(var_a0, 0);
        var_s0 += 1;
        var_a0 = var_s0;
    } while (var_s0 < 0xF);
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    D_800B593C = 0x80;
    D_800CA2DC = 0;
    D_800CA2E0 = 0;
    func_8007C740();
    func_800649D4();
    func_80084E4C();
    func_800847B8(0U);
    func_800AE0F0(D_800CA188, D_800E6280.unk_0D4);
    func_80136380();
    D_800CA2D0 = D_8013C44C;
    D_800CA2D4 = D_8013C480;
    D_800CA2D8 = D_8013C4B4;
    D_80122CE0 = 0xFF;
    D_800CA2EC = 0;
    var_s0_2 = 0;
    do {
        temp_lo = var_s0_2 * 0x38;
        temp_t2 = &D_800CA2B0[var_s0_2];
        var_s0_2 += 1;
        *temp_t2 = ((u32) (D_800E6280.unk_1BC[0].unk_0C.b[temp_lo] << 0x1E) >> 0x1F) != 0;
    } while (var_s0_2 < 0xA);
    D_800CA2BA = 0;
    D_800CA2F0 = 0;
    func_8004284C();
}
