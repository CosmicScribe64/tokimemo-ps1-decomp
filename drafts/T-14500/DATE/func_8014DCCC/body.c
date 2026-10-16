/* NON_MATCHING: T-10320 / T-12100, sixteen register/operand rows remain.
 * A byte-field pointer restores the original table offset and frame size. */
void func_8014DCCC(void) {
    u8 *selector;
    FnTbl39 tbl;
    void (*fn)();

    selector = &D_800E6280.unk_110A;
    tbl = D_8015E3C4;
    fn = tbl.f[*selector];
    if (place_init2 == fn) {
        D_80120653 |= 0x80;
    }
    if (normal_date_bg_out == fn) {
        D_80120653 &= 0xFF7F;
    }
    fn(0x80);
    if (D_80122D04 != 0) {
        func_8014E010();
    }
    if (*selector == 0xE) {
        func_8014E108();
    }
    if (*selector >= 0x19 && *selector < 0x24) {
        func_80140D38();
    }
}
