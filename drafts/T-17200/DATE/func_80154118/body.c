void func_80154118(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl15 sp28;

    sp28 = D_8015F4DC;
    if ((u8) D_800E6280.unk_110A < 0xCU) {
        if ((((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) || ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B))) && (D_800B593C == 0x80)) {
            func_8006B900();
        } else if ((D_800CA2F8 == 0x11) || (D_800CA2F8 == 0x10)) {
            func_8006B900();
        }
    }
    idx = D_800E6280.unk_110A;
    sp28.f[idx](0x80);
}
