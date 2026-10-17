void func_80136A2C(void) {
    s32 i;

    if (D_800B3CFC != 0) {
        D_800B3CF8 = 0;
        D_800B3CFC = 0;
        D_80139B18++;
    }
    if (D_80139B18 > 0) {
        for (i = 0; i < 10; i++) {
            if (D_8013984C[i].unk_00 > 0 && D_8013984C[i].unk_00 < 4) {
                if (D_80139B18 < 3) {
                    D_8013984C[i].unk_00 = D_8013984C[i].unk_04 + 1;
                } else {
                    D_8013984C[i].unk_00 = 4;
                    D_8013984C[i].unk_04 = 0;
                }
            }
        }
    }
}
