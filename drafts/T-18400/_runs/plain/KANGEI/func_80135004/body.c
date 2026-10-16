void func_80135004(void) {
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;

    func_80133EF8();
    if ((D_80139AE0 == 2) && (D_800E7384 == 1) && ((D_800E71DF == 1) || (D_800E71DF == 3))) {
        sp34 = 0x140;
        sp36 = 0x80;
        sp38 = 0x180;
        sp3A = 0x80;
        func_8009C8E0((RECT *) &sp34, (void *)0x80180000);
        func_8009C674(0);
        func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, get_g_zyotai_h(D_800E71DF) & 0x7F);
        func_80063668();
    }
    D_800E7384 += 1;
}
