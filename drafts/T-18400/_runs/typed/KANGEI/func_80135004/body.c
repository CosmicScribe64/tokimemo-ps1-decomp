void func_80135004(void) {
    RECT sp34;
    u8 temp_v1;

    func_80133EF8();
    if ((D_80139AE0 == 2) && (D_800E6280.unk_1104.w == 1) && ((temp_v1 = D_800E6280.unk_F5F, (temp_v1 == 1)) || (temp_v1 == 3))) {
        sp34.x = 0x140;
        sp34.y = 0x80;
        sp34.w = 0x180;
        sp34.h = 0x80;
        func_8009C8E0(&sp34, (void *)0x80180000);
        func_8009C674(0);
        func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, get_g_zyotai_h(D_800E6280.unk_F5F) & 0x7F);
        func_80063668();
    }
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
}
