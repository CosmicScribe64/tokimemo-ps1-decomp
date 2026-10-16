void func_801388D8(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_3;
    u8 temp_a0;
    u8 temp_v1_2;
    u8 temp_v1_4;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    temp_v1 = D_800E6280.unk_1BC[0].unk_10.w;
    temp_a0 = D_800E6280.unk_03F;
    if ((temp_a0 == (temp_v1 & 0xF)) && (D_800E6280.unk_040 == ((u32) (temp_v1 << 0x17) >> 0x1B))) {
        temp_v1_2 = D_800E6280.unk_F5F;
        if ((temp_v1_2 == 4) && ((u32) (temp_v0 & 0x7F) < 2U)) {
            D_800E6280.unk_721 = D_800E6280.unk_1109;
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        } else {
            D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
            D_800E6280.unk_722 = 0;
        }
        D_800E6280.unk_75D = temp_v1_2;
        D_800E6280.unk_F5F = 0;
        func_80042908(6);
        return;
    }
    temp_v1_3 = D_800E6280.unk_0F8;
    if (((temp_v1_3 & 0xF) == temp_a0) && (((u32) (temp_v1_3 << 0x17) >> 0x1B) == D_800E6280.unk_040)) {
        temp_v1_4 = D_800E6280.unk_F5F;
        if ((temp_v1_4 == 4) && ((u32) (temp_v0 & 0x7F) < 2U)) {
            D_800E6280.unk_721 = D_800E6280.unk_1109;
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        } else {
            D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
            D_800E6280.unk_722 = 0;
        }
        D_800E6280.unk_75D = temp_v1_4;
        D_800E6280.unk_F5F = temp_v1_4;
        func_80042908(7);
        return;
    }
    func_8004284C();
}
