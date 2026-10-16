void func_8013852C(void) {
    u8 year;

    if (D_800E6280.unk_03F >= 4) {
        year = D_800E6280.unk_03E - 0x5F;
    } else {
        year = D_800E6280.unk_03E - 0x60;
    }
    D_80139AD0 = (KObj *) D_8013A170[1][0][11];
    D_80139AD4 = D_8013A170[1][1][11];
    D_80139AD8 = D_8013A170[1][2][11];
    D_80139ADC = D_800E6280.unk_F5F * 3 + year;
    D_8013A2AC = year;
    func_8004284C();
}
