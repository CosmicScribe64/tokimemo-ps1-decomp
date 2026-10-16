void func_801336AC(void) {
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x28 && (D_800E6280.unk_F80 & 0x20)) {
        if (func_800460EC() != 2) {
            func_80044750(0x74);
        }
        func_8013376C();
    }
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x26 && (D_800E6280.unk_F80 & 0x40)) {
        func_80044750(0xC1);
        func_80044750(0xC4);
        func_80042940(0x25);
    }
    func_8013376C();
}
