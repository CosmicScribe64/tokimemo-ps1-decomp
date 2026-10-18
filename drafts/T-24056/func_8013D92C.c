/* T-24056 source-only sibling port; not matched. See README.md. */
void func_8013D92C(void) {
    /* FAKE: named arithmetic range for diagnostic allocation; original spelling unknown. T-24056. */
    s32 limit;

    limit = D_800E6280.unk_1BC[12].unk_0C.f.f9 * 25 + 150;
    if (D_800E6280.unk_10C.unk_02 >= (s16)limit) {
        D_80145F2C = (D_800E6280.unk_1BC[14].unk_0C.f.f9 >= 1) * 3 + 4;
        D_800E6280.unk_1BC[14].unk_0C.f.f9 += 1;
        func_8004284C();
        return;
    }
    D_80145F2C += 1;
    D_800E6280.unk_118.unk_02 += 10;
    D_800E6280.unk_11C.unk_02 += 10;
    check_para_limit();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}
