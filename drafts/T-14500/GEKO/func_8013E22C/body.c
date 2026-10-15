void func_8013E22C(void) {
    Rec24 *temp_s0;
    s16 temp_s1;
    s16 var_s1;
    s32 temp_hi;
    s32 temp_v1;

    D_800E74BC = 1;
    LoadSquare(0x140U, 0U, 0x40U, 0x80U, D_801463B0);
    func_8013DF70();
    load_palette(D_80146394, 0x12, 1, 3, 0);
    var_s1 = 4;
    do {
        temp_s0 = &D_801217D0[var_s1];
        temp_s0->unk_00 = -0x3F000000;
        temp_s0->unk_04 = (func_800AE0D0() % 84) - 0x9F;
        temp_hi = func_800AE0D0() % 130;
        temp_s0->unk_08 = 8;
        temp_s0->unk_0A = 0x18;
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
        temp_s0->unk_06 = temp_hi - 0x5F;
        temp_s0->unk_1E = 0x1000;
        temp_s0->unk_20 = 0;
        *(&D_800E7325 + var_s1) = 0x8C;
        temp_v1 = var_s1 * 2;
        *(&D_801463D8 + temp_v1) = 0;
        temp_s1 = var_s1 + 1;
        *(&D_801463F8 + temp_v1) = 0;
        var_s1 = temp_s1;
    } while (temp_s1 < 0xE);
    func_8004284C();
}
