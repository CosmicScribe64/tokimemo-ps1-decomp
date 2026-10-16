extern ? D_801463F8;

void func_8013A820(void) {
    Rec24 *temp_s0;
    s16 temp_s1;
    s16 var_s1;
    s32 temp_hi;
    s32 temp_v0;

    func_80139910();
    func_80043914(D_80146390, 0x11, 1, 2, 0);
    func_80084E90(D_80146398, D_8014639C, D_801463A0, D_801463A4, D_801463A8, D_801463AC);
    func_800850D4(D_80146384, D_80146388, D_80146380, (s32) D_8014638C);
    func_80043914(D_80146394, 0x12, 1, 3, 0);
    var_s1 = 4;
    do {
        temp_s0 = &D_801217D0[var_s1];
        temp_s0->unk_00 = -0x3F000000;
        temp_hi = func_800AE0D0() % 320;
        temp_s0->unk_06 = -0x78;
        temp_s0->unk_08 = 8;
        temp_s0->unk_0A = 8;
        temp_s0->unk_0C = 5;
        temp_s0->unk_0E = 0;
        temp_s0->unk_0F = 0;
        temp_s0->unk_10 = 0;
        temp_s0->unk_12 = 0x1F2;
        temp_s0->unk_14 = 0;
        temp_s0->unk_15 = 0;
        temp_s0->unk_16 = 0;
        temp_s0->unk_18 = 0;
        temp_s0->unk_1A = 0;
        temp_s0->unk_1C = 0x1000;
        temp_s0->unk_04 = temp_hi - 0x9F;
        temp_s0->unk_1E = 0x1000;
        temp_s0->unk_20 = 0;
        D_800E6280.unk_10A5[var_s1] = 0xC;
        temp_v0 = var_s1 * 2;
        *(&D_801463D8 + temp_v0) = 0;
        temp_s1 = var_s1 + 1;
        *(&D_801463F8 + temp_v0) = 0;
        var_s1 = temp_s1;
    } while (temp_s1 < 0x13);
    func_8004284C();
}
