void func_800FD00C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;
    u8 v;

    tbl = D_801214A0;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800EECE0 != 0) {
        D_800EB02B |= 0x80;
        if (D_800B1AE4 % 40U < 20) {
            D_800EC2F8 &= 0x7FFFFFFF;
        } else {
            D_800EC2F8 |= 0x80000000;
        }
        D_800EB02F = D_800EC30C = D_800EC30D = D_800EC30E = D_800F19AB;
        return;
    }
    D_800EB02B &= 0xFF7F;
    D_800EC2F8 |= 0x80000000;
}
