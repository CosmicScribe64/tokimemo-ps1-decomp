? func_8004F984(?, s16, s16);                       /* extern */
extern s32 D_800E7208;
extern s8 D_800E7313;
extern u8 D_800E7398;

void func_8013CEC8(void) {
    s32 sp2C;

    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    D_800E7208 &= ~0x100;
    func_8004FC10(0);
    if (D_800E7313 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = (((s8) D_800E7313 / 7) * 0x1F) - 0x82;
        D_80120C96 = (((s8) D_800E7313 % 7) * 0x10) - 0xB;
    }
    if ((D_800E7208 & 0x820) && (D_800E7313 != -1) && ((u8) (&D_800E7D15)[D_800E7313] == 1)) {
        if ((D_800E7313 + 1) == D_800E7D36) {
            func_80044750(0x501);
            func_80042940(3);
            D_800E7398 = D_800E7D36;
        } else {
            D_800E7D36 = D_800E7313 + 1;
            sp2C = (s32) D_800E7313;
            func_8004E58C();
            func_80139CB4();
            func_8004EA98();
            func_8004EAD4(0);
            func_8013B51C(sp2C);
        }
    }
    func_8013AF78(1);
    func_8013B1D8();
    func_80139C24();
    func_80142880();
}
