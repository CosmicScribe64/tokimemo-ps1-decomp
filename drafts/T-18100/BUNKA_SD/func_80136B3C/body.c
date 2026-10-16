void func_80136B3C(void) {
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x26 && (D_800E6280.unk_F80 & 0x20)) {
        if (func_800460EC() != 2) {
            func_80044750(0x74);
        }
        func_80136BFC();
    }
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x24 && (D_800E6280.unk_F80 & 0x40)) {
        func_80044750(0xC1);
        func_80044750(0xC4);
        func_80042940(0x23);
    }
    func_80136BFC();
}
