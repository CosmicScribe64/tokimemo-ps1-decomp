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
    /* FAKE: repeated field use and halfword conversion recover the promoted
     * girl byte and arithmetic temporary numbering; original source unknown. T-24054 */
    D_80139ADC = (u16)((D_800E6280.unk_F5F << 2) - D_800E6280.unk_F5F) + year;
    D_8013A2AC = year;
    func_8004284C();
}
