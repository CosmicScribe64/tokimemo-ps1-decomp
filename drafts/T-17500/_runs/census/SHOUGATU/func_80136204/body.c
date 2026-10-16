void func_80136204(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = get_g_zyotai_s(D_800E6280.unk_F5F);
    temp_v1 = *(&D_800E6280.unk_1BC[0].unk_10.w + (D_800E6280.unk_F5F * 0x38));
    if ((D_800E6280.unk_040 == ((u32) (temp_v1 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (temp_v1 & 0xF)) && (D_80143B20 == 0) && (D_800E6280.unk_F5F == 0)) {
        D_800E6280.unk_71E |= 1;
        D_800E6280.unk_110A += 4;
    } else if ((D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_80143B20 == 0)) {
        if ((u32) (temp_v0 & 0x7F) < 2U) {
            D_800E6280.unk_71E |= 4;
            D_800E6280.unk_110A += 0x1B;
        } else {
            D_800E6280.unk_71E |= 8;
        }
    }
    func_8004284C();
}
