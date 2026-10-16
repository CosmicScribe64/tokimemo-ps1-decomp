void func_80138740(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    u8 temp_a0;
    u8 temp_v1_3;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    temp_v1 = D_800E6280.unk_1BC[0].unk_10.w;
    temp_a0 = D_800E6280.unk_03F;
    if ((temp_a0 == (temp_v1 & 0xF)) && (D_800E6280.unk_040 == ((u32) (temp_v1 << 0x17) >> 0x1B))) {
        D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
        D_800E6280.unk_722 = 0;
        D_800E6280.unk_75D = D_800E6280.unk_F5F;
        D_800E6280.unk_F5F = 0;
        func_80042908(6);
        return;
    }
    temp_v1_2 = D_800E6280.unk_0F8;
    if (((temp_v1_2 & 0xF) == temp_a0) && (((u32) (temp_v1_2 << 0x17) >> 0x1B) == D_800E6280.unk_040)) {
        D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
        D_800E6280.unk_722 = 0;
        D_800E6280.unk_75D = D_800E6280.unk_F5F;
        func_80042908(7);
        return;
    }
    temp_v1_3 = D_800E6280.unk_1109;
    if ((temp_v1_3 == 4) && (D_800E6280.unk_F5F == 2) && (D_800CA2CC == 2)) {
        D_800E6280.unk_721 = temp_v1_3 + 1;
        D_800E6280.unk_722 = 0;
        func_80042908(8);
        return;
    }
    if ((temp_v1_3 == 2) && (D_800E6280.unk_F5F == 5) && ((u32) (temp_v0 & 0x7F) < 2U)) {
        D_800E6280.unk_721 = temp_v1_3 + 1;
        D_800E6280.unk_722 = 0;
        func_80042908(0xB);
        return;
    }
    func_8004284C();
}
