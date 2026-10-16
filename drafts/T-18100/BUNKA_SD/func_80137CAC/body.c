void func_80137CAC(void) {
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x2F && (D_800E6280.unk_F80 & 0x20)) {
        if (func_800460EC() != 2) {
            func_80044750(0x74);
        }
        func_80137D6C();
    }
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 0x2D && (D_800E6280.unk_F80 & 0x40)) {
        func_80044750(0xC1);
        func_80044750(0xC4);
        func_80042940(0x2C);
    }
    func_80137D6C();
}
