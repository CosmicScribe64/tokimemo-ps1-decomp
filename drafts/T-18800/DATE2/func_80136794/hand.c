s32 func_80136794(void) {
    s32 v;

    v = func_80051A68(D_800E6280.unk_F5F);
    if ((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F && ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) {
        if (((u8)v & 0x7F) < 2U) {
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
            D_800E6280.unk_71E = (D_800E6280.unk_71E & 0xFFF7) | 4;
            D_800E6280.unk_F5F = D_800E6280.unk_75D;
            func_800847B8(D_800E6280.unk_75D);
            func_80042908(4);
            return 0;
        }
        D_800E6280.unk_71E |= 8;
    }
    func_8004284C();
}
