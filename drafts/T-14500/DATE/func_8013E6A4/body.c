void func_8013E6A4(void) {
    s32 d;
    u8 x;

    x = func_80051B48(D_800E6280.unk_F5F);
    func_80085A60();
    d = D_800E6280.unk_1104.w;
    D_800E6280.unk_1104.w = d + 1;
    if (d == 1) {
        if (D_800CA14C == 2) {
            func_80083440(x);
        }
    }
}
