/* NON_MATCHING: T-11110, T-11600, 8 lines: chained halfword store scheduling and $v0 vs $t6 for third chain (regdiff: the
   original's third 3 is a uopt colour, the draft's a ugen temporary); separate stores, chain orders, one-line chains and an
   s16/s32/u8 local for the constant did not reach it. */
void func_80118868(void) {
    if (D_800B1746 != 6) {
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    if (D_800B095E >= 0x96) {
        D_80094730 += 3;
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    (&D_801243A4)[0] = (&D_8012439C)[2] = (&D_8012439C)[0] = (&D_8012439C)[3] = 2;
    (&D_8012438C)[0] = (&D_8012438C)[1] = (&D_8012438C)[2] = 3;
    (&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 3;
    func_80011DFC();
}
