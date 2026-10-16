void func_800FAA04(void) {
    Rec24 *temp_s0;
    s16 temp_s1;
    s16 var_s1;
    s32 temp_hi;

    func_800F9900();
    func_80012D64(D_80120C00, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C08, D_80120C0C, D_80120C10, D_80120C14, D_80120C18, D_80120C1C);
    func_8004C6B0(D_80120BF4, D_80120BF8, D_80120BF0, (s32) D_80120BFC);
    func_80012D64(D_80120C04, 0x12, 1, 3, 0);
    var_s1 = 4;
    do {
        temp_s0 = &D_800EC190[var_s1];
        temp_s0->unk_00 = -0x3F000000;
        temp_hi = func_800789A0() % 320;
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
        D_800B188D[var_s1] = 0xC;
        *(&D_80120CE8 + (var_s1 * 2)) = 0;
        temp_s1 = var_s1 + 1;
        D_80120D08[var_s1] = 0;
        var_s1 = temp_s1;
    } while (temp_s1 < 0x13);
    func_80011DFC();
}
