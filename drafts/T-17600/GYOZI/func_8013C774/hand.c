void func_8013C774(void) {
    s32 idx; /* declared before table: its stack slot sits above it (T-3330) */
    GzCallbacks90 table;

    table = D_80147C6C;
    idx = D_800F647A;
    table.callbacks[idx]();
    if (D_800F62F0 & 0x100) {
        if (D_800F53DF >= 6 && D_800F53DF < 9) {
            func_8004EE18(D_80147AE0, 0x11, 1, 0, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 0, 0);
        }
        func_800BCDF0("S\n");
    }
    if (D_800F62F0 & 0x800) {
        if (D_800F53DF >= 6 && D_800F53DF < 9) {
            func_8004EE18(D_80147AE0, 0x11, 1, 2, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 2, 0);
        }
        func_800BCDF0("T\n");
    }
}
