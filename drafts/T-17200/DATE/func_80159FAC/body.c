void func_80159FAC(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl46 sp2C;

    sp2C = D_80160940;
    if ((((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) || ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0) && (((u8) D_800E6280.unk_110A < 0xDU) || ((u8) D_800E6280.unk_110A >= 0x21U))) {
        func_8006B900();
    }
    idx = D_800E6280.unk_110A;
    sp2C.f[idx](0x80);
    if ((D_80122D04 != 0) && (((u32) D_800E6280.unk_10F8 % 150U) == 0)) {
        func_8015A170();
    }
}
