void func_8013F0DC(void) {
    s32 sel;
    s32 pad; /* FAKE: third local slot for the 0x38 frame (T-11370) */
    s32 idx;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    D_800E6280.unk_F88 &= ~0x100;
    func_8004FC10(0);
    sel = D_800E6280.unk_1093;
    idx = sel;
    if (sel != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((D_800E6280.unk_F88 & 0x820) && (sel != -1) && (u8)(&D_800E7D15)[idx] == 1) {
        func_80044750(0x501);
        func_8004284C();
        D_800E6280.unk_1118 = ++idx;
    }
    func_80142880();
}
