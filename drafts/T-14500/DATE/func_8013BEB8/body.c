void func_8013BEB8(void) {
    if (D_800E6280.unk_F5F != 6) {
        func_8004284C();
        D_800E6280.unk_110A += 8;
        return;
    }
    if (D_800E6280.unk_10C.unk_02 >= 0x96) {
        D_800CA148 += 2;
        func_80042940(0x2D);
        return;
    }
    D_800CA234[0] = D_800CA22C[2] = D_800CA22C[0] = D_800CA22C[3] = 2;
    D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 3;
    D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;
    func_8004284C();
}
