/* NON_MATCHING: T-10320, temporaries: the original keeps (D_800F6468 & 7) in $t6/$t7 and the index in $v0, IDO uses $v1/$v0 (about 25 lines) */
void func_80137188(void) {
    u32 t;
    u32 v;

    t = D_800F6468 & 7;
    v = t;
    if (t != 0) {
        v = (t - 1) & 0xFF;
    }
    D_80146178 = v;
    func_800BCE10(&D_800D9320, &D_8014617C[v * 5]);
    v = D_80146178;
    if (v == 0) {
        D_800F54B6 -= 5;
    }
    D_80146178 = v;
    if (v == 6) {
        D_800F54B6 += 5;
    }
    func_8008FB00();
}
