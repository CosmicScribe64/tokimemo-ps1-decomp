extern u8 D_800E71DF;

void func_8013A1D4(void) {
    u8 sp2B;
    u8 temp_a0;

    func_801386C4();
    if (D_800E71DF == 2) {
        temp_a0 = func_80051B48(D_800E71DF) & 0xFF;
        if ((D_800CA2E0 == 7) && (D_80122CDC != 0)) {
            sp2B = temp_a0;
            func_80083440(temp_a0);
        }
        if (D_800CA2E0 == 6) {
            if (D_80122CDC == 0) {
                func_80083440(temp_a0);
            }
        }
    }
}
