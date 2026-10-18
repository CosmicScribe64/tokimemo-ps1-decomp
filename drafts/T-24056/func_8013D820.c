/* T-24056 source-only sibling port; not matched. See README.md. */
void func_8013D820(void) {
    u32 level = D_800E6280.unk_1BC[12].unk_0C.f.f9;

    if (D_800E6280.unk_10C.unk_02 >= (s16)(level * 25 + 150) &&
        D_800E6280.unk_1BC[14].unk_0C.f.f9 != 0) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        return;
    }
    if (D_800E6280.unk_114.unk_02 >= (s16)(level * 30 - (level >= 2) * 10 + 70)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}
