void func_8013D4CC(void) {
    s32 idx;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    D_800E6280.unk_F88 &= ~0x100;
    func_8004FC10(0);
    idx = D_800E6280.unk_1093;
    if (D_800E6280.unk_1093 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((D_800E6280.unk_F88 & 0x820) && (D_800E6280.unk_1093 != -1) && (u8)(&D_800E7D15)[idx] == 1) {
        if (idx + 1 == D_800E6280.unk_1118) {
            func_80042940(0x24);
        } else {
            func_80042940(0x22);
            D_800E6280.unk_1118 = D_800E6280.unk_1093 + 1;
        }
    }
    func_8013AF78(1);
    func_8013B1D8();
    func_80139C24();
    func_80142880();
}
