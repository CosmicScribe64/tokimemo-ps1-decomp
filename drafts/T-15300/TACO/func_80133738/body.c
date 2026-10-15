void func_80133738(void) {
    s32 var_v1;
    TcPair8 *temp_v0;

    var_v1 = 3;
    if ((D_800E6280.unk_1109 & 1) && (D_800E6280.unk_110A == 1)) {
        if ((D_8015EDBC == 0) && (D_800E6280.unk_F88 & 0x10) && (D_800E6280.unk_10F8 & 0xFF)) {
            if (func_80045288() == 1) {
                    if (func_800460EC() & 2) {
                    if (D_8015EDB4[8].unk78 < D_8015EDB4->unk78) {
                        var_v1 = (func_800AE0D0() & 1) + 2;
                    } else if (D_800E6280.unk_10F4 & 1) {
                        var_v1 = func_800AE0D0() % 6;
                    }
                    temp_v0 = &D_8015E238[var_v1];
                    func_80046290(temp_v0->a, temp_v0->b, 8);
                    func_80044750(0x300);
                }
            }
        }
    }
}
