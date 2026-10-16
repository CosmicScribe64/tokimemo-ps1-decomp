
void func_801347E0(void) {
    u32 state;
    s32 girl;

    girl = D_800E6280.unk_75D;
    D_800E6280.unk_F5F = girl;
    func_800847B8(girl);
    D_80122CDC = D_80122CEC;
    state = get_g_zyotai_s(6) & 0x7F;
    girl = D_800E6280.unk_F5F;
    if ((girl == 6) && (state < 2) && (D_80143B20 == 0)) {
        D_80143AF4 = D_80143A60;
        D_80143AF8 = D_80143A9C;
        D_80143AFC = D_80143AD8;
    }
    func_8004284C();
}
