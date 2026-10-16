void func_801347E0(void) {
    u32 temp_t8;

    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    D_80122CDC = D_80122CEC;
    temp_t8 = get_g_zyotai_s(6) & 0x7F;
    if ((D_800E6280.unk_F5F == 6) && (temp_t8 < 2U) && (D_80143B20 == 0)) {
        D_80143AF4 = D_80143A60;
        D_80143AF8 = D_80143A9C;
        D_80143AFC = D_80143AD8;
    }
    func_8004284C();
}
