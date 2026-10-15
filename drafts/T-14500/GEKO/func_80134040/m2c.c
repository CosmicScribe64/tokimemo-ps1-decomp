? func_800646CC();                                  /* extern */
extern s32 D_800E7200;
extern u8 D_800E7389;

void func_80134040(void) {
    u8 var_v0;

    func_80083808();
    if (D_800E7389 == 0) {
        func_801341A0();
    } else {
        func_80046500();
    }
    func_8007EDF8();
    func_800846C0();
    if (D_80122D38 == 0) {
        func_80066C08(2);
    }
    func_80064F48();
    func_80066334();
    func_80064DEC();
    if (D_80122D38 == 0) {
        func_80083A10();
    }
    if (D_80122D38 != 0) {
        func_80066C08(1);
        func_800578F4(0);
        func_800646CC();
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E7200 & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        var_v0 = D_8011ECD3;
        if (D_80122D04 != 0) {
            var_v0 &= 0x7F;
        }
        D_8011ECD3 = var_v0;
        if (var_v0 & 0x80) {
            func_8006BA40();
        }
    }
}
