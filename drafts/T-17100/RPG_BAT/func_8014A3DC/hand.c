/* T-17100 hand edit of the redraft: one instruction off. The original hoists `lui %hi(D_8015EC74)` above the
 * `D_8015EDB8 >= 0x1F` branch and leaves its delay slot empty; IDO hoists the D_8015EDA4 lui instead.
 * Needs `void func_8014A3DC(void);` in the header (func_801498C0 calls it first). */
void func_8014A3DC(void) {
    if (D_8015EDB0.unk_00 < 5) {
        func_8013F0F4(0x503, 0x1E, 1);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8013E7C0(0x2F, 0xD, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        func_8014EDFC(0x2F);
        return;
    case 2:
        func_8013E7C0(0x2F, 7, 1, 1);
        func_8013E7C0(0x2A, 8, 1, 1);
        func_8013E7C0(0x2B, 9, 1, 1);
        func_8013E7C0(0x2C, 0xA, 1, 1);
        func_8013E7C0(0x2D, 0xB, 1, 1);
        func_8013E7C0(0x2E, 0xC, 1, 1);
        func_8014EBD0();
        return;
    case 3:
        func_8013E97C(0x29, 0xF0, 0x38);
        func_8013E97C(0x28, 0xF0, 0x38);
        func_8013E810(0x29, 0xF, 1, 1);
        func_8014EBD0();
        return;
    case 4:
        if (D_8015EDB8 >= 0x1F) {
            D_8015EDB8 = 0;
            if (++D_8015EDA4 == D_8015EC74) {
                func_8014EBD0();
            } else {
                func_8013E810(0x29, D_8015EDA4 + 0xF, 1, 1);
                if ((D_8015EDA4 == 6) || (D_8015EDA4 == 7)) {
                    func_8013E810(0x28, 6, 1, 1);
                }
                if ((D_8015EDA4 == 8) || (D_8015EDA4 == 9)) {
                    func_8013E810(0x28, 0x19, 1, 1);
                }
            }
        }
        D_8015EDB8 += 1;
        return;
    case 5:
        func_8014ED80(0x1E);
        return;
    case 6:
        func_8014EBA8();
        /* fallthrough */
    default:
        return;
    }
}
