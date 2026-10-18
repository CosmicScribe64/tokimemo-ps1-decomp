void func_80135004(void) {
    s32 one;
    RECT rect;

    func_80133EF8();
    if ((D_80139AE0 == 2) && ((one = 1) == D_800E6280.unk_1104.w)) {
        if ((one == D_800E6280.unk_F5F) || (D_800E6280.unk_F5F == 3)) {
            rect.x = 0x140;
            rect.y = 0x80;
            rect.w = 0x180;
            rect.h = 0x80;
            func_8009C8E0(&rect, (void *)0x80180000);
            func_8009C674(0);
            func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, get_g_zyotai_h(D_800E6280.unk_F5F) & 0x7F);
            func_80063668();
        }
    }
    D_800E6280.unk_1104.w += 1;
}
