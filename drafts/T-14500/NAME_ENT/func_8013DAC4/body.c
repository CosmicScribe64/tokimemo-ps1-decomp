void func_8013DAC4(void) {
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
    if ((D_800E6280.unk_F88 & 0x820) && (D_800E6280.unk_1093 != -1)) {
        func_8004284C();
        D_800E6280.unk_1118 = D_800E6280.unk_1093 + 1;
    }
}
