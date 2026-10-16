extern u8 D_800E71DF;
extern s32 D_800E7384;

void func_8013E6A4(void) {
    u8 sp2B;

    sp2B = func_80051B48(D_800E71DF);
    func_80085A60();
    if ((D_800E7384++ ^ 1) == 0) {
        if (D_800CA14C == 2) {
            func_80083440((s32) sp2B);
        }
    }
}
