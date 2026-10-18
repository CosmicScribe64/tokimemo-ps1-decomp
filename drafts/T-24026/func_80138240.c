/* T-24026 source-only sibling draft: four frame/table-offset mismatches. */
void func_80138240(void) {
    s32 idx;
    FnTbl37 table;

    table = D_80144FB4;
    if (D_80144F10 == 0 && D_800E6280.unk_110A >= 0x13 && D_800E6280.unk_110A < 0x1C) {
        func_8004A414(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    if (D_80144F10 == 1 && D_800E6280.unk_110A >= 0x13 && D_800E6280.unk_110A < 0x1C) {
        func_8004A414(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    idx = D_800E6280.unk_110A;
    table.f[idx]();
}
