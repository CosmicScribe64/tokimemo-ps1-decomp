extern u8 D_800E62BF;
extern u8 D_800E64BA;
extern s32 D_800E6608;
extern u8 D_800E699C;
extern u8 D_800E71DF;
extern u32 D_800E7384;
extern u8 D_800E738D;

s32 func_8013737C(void) {
    s8 var_t3;
    u8 temp_a0;
    u8 temp_t9;
    u8 var_v0;
    void *var_v0_2;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_80044750(0xCF);
        func_800674B0();
        var_v0 = (D_800E738D + 1) & 0xFF;
    }
    D_800E738D = var_v0;
    if (var_v0 == 1) {
        D_800E7384 += 1;
        if ((u32) D_800E7384 >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E738D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E738D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    D_800E71DF = D_80144E10;
    if ((D_80144E10 == 2) && ((D_80144E14 == 2) || (D_80144E14 == 3)) && (((u8) D_800E64BA >> 4) == 3)) {
        func_80085B3C(8, (D_80144E10 * 0xA) + 6);
    } else if ((D_80144E10 == 4) && (D_80144E14 == 0) && ((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
        func_80085B3C(8, (D_80144E10 * 0xA) + 3);
    } else {
        func_80085B3C(8, (D_80144E10 * 0xA) + D_80144E14);
    }
    if (D_80144E10 == 0xA) {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E699C].unk_01].unk_06 = 0x28;
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E699C].unk_01].unk_06 = 0x32;
    }
    temp_a0 = D_800E6280.unk_69C[D_800E699C].unk_01;
    if (temp_a0 != 8) {
        var_v0_2 = &D_800E6280 + (temp_a0 * 0x38);
        var_t3 = D_800E6280.unk_1BC[temp_a0].unk_0C.b[0] | 2;
        goto block_26;
    }
    D_800E6280.unk_1BC[8].unk_0C.b[1] = (((((u32) (D_800E6608 << 0x14) >> 0x1D) + 1) * 2) & 0xE) | (D_800E6280.unk_1BC[8].unk_0C.b[1] & 0xFFF1);
    if (((u32) (D_800E6280.unk_1BC[8].unk_0C.w << 0x14) >> 0x1D) == 4) {
        temp_t9 = D_800E6280.unk_69C[D_800E699C].unk_01;
        var_v0_2 = &D_800E6280 + (temp_t9 * 0x38);
        var_t3 = D_800E6280.unk_1BC[temp_t9].unk_0C.b[0] | 2;
block_26:
        *(s8 *)((unsigned char *)var_v0_2 + (456)) = var_t3;
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    return func_80072B5C(1);
}
