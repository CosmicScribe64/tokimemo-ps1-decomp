void func_8013E430(void) {
    s32 n;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    D_800E6280.unk_F88 &= ~0x100;
    func_8004FC10(0);
    n = D_800E6280.unk_1093;
    if (D_800E6280.unk_1093 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = n / 7 * 0x1F - 0x82;
        D_80120C96 = n % 7 * 0x10 - 0xB;
    }
    if ((D_800E6280.unk_F88 & 0x820) && (D_800E6280.unk_1093 != -1)) {
        func_80044750(0x501);
        D_800E6280.unk_1118 = n + 1;
        if ((u8)(&D_800E7D15)[n] != 0) {
            func_8004284C();
            D_800E6280.unk_1118 = D_800E6280.unk_1093 + 1;
        } else if (D_800E8BEE != 0) {
            func_80042940(0xA8);
        } else {
            func_80042940(0xA9);
        }
    }
    func_80142880();
}
