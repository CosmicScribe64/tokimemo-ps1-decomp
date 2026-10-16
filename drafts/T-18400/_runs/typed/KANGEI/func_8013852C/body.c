void func_8013852C(void) {
    u8 var_v1;

    if ((u8) D_800E6280.unk_03F >= 4U) {
        var_v1 = (D_800E6280.unk_03E - 0x5F) & 0xFF;
    } else {
        var_v1 = (D_800E6280.unk_03E - 0x60) & 0xFF;
    }
    D_80139AD0 = (KObj *) D_8013A170[1][0][0xB];
    D_80139AD4 = D_8013A170[1][1][0xB];
    D_80139AD8 = D_8013A170[1][2][0xB];
    D_80139ADC = (D_800E6280.unk_F5F * 3) + var_v1;
    D_8013A2AC = var_v1;
    func_8004284C();
}
