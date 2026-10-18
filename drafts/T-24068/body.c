/* NON_MATCHING: T-24068 source-only baseline,48 resolved +/- rows. */
void func_80140AE4(void) {
    func_80140BC4();
    func_80140DC8();
    func_80140E80();
    if (D_801500DC == 0) {
        if ((func_80143240() == 1) && (D_800E6280.unk_F80 & 0x09000000)) {
            if (D_801500D8 != 1) {
                func_80044750(0x50E);
            }
            D_801500D8 = 1;
        }
    } else if (D_801500DC == 1) {
        if ((u32)D_800B3CFC == 1) {
            func_80044750(0x50E);
            if (D_801500D8 == 1) {
                D_801500D8 = 3;
            } else {
                D_801500D8 = 2;
            }
        }
    }
}
