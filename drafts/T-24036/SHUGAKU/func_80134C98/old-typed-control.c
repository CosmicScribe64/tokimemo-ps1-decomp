void func_80134C98(void) {
    s32 temp_v1;
    s32 temp_v1_2;
    u8 temp_v0;

    temp_v1 = D_800E6280.unk_1BC[0].unk_10.w;
    temp_v0 = D_800E6280.unk_03F;
    if ((temp_v0 == (temp_v1 & 0xF)) && (D_800E6280.unk_040 == ((u32) (temp_v1 << 0x17) >> 0x1B))) {
        D_800E6280.unk_721 = 5;
        D_800E6280.unk_722 = 0;
        D_800E6280.unk_75D = D_800E6280.unk_F5F;
        D_800E6280.unk_F5F = 0;
        func_80042908(6);
        return;
    }
    temp_v1_2 = D_800E6280.unk_0F8;
    if (((temp_v1_2 & 0xF) == temp_v0) && (((u32) (temp_v1_2 << 0x17) >> 0x1B) == D_800E6280.unk_040)) {
        D_800E6280.unk_721 = 5;
        D_800E6280.unk_722 = 0;
        D_800E6280.unk_75D = D_800E6280.unk_F5F;
        D_800E6280.unk_71E = D_800E6280.unk_71E | 8;
        func_80042908(7);
        return;
    }
    func_8004284C();
}
