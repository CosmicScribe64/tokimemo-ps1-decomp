void func_8013852C(void) {
    u8 var_v1;

    if ((u8) D_800E62BF >= 4U) {
        var_v1 = (D_800E62BE - 0x5F) & 0xFF;
    } else {
        var_v1 = (D_800E62BE - 0x60) & 0xFF;
    }
    D_80139AD0 = D_8013A238;
    D_80139AD4 = D_8013A26C;
    D_80139AD8 = D_8013A2A0;
    D_80139ADC = (D_800E71DF * 3) + var_v1;
    D_8013A2AC = var_v1;
    func_8004284C();
}
