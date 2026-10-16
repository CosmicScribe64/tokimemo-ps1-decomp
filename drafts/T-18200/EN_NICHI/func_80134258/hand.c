void func_80134258(s32 arg0) {
    s32 n;
    s32 m;
    s32 r;
    s32 i;

    func_80134784(&n, &m);
    for (i = 0; i != 10; i++) {
        if (D_8013984C[i].unk_00 == 0 && n * 10 < func_800AE0D0() % 100) {
            r = func_800AE0D0();
            if (m == 0 && r % 100 >= 61) {
                D_8013984C[i].unk_00 = 4;
                m++;
            } else {
                D_8013984C[i].unk_00 = func_800AE0D0() % 3 + 1;
                n++;
            }
            r = func_800AE0D0() % 100;
            if (r < 20) {
                D_8013984C[i].unk_04 = 0;
            } else if (r < 60) {
                D_8013984C[i].unk_04 = 1;
            } else {
                D_8013984C[i].unk_04 = 2;
            }
            r = func_800AE0D0() % 100;
            if (r < 20) {
                D_8013984C[i].unk_08 = 0;
            } else if (r < 60) {
                D_8013984C[i].unk_08 = 1;
            } else {
                D_8013984C[i].unk_08 = 2;
            }
            if (D_8013984C[i].unk_00 == 4) {
                D_8013984C[i].unk_04 = 0;
            }
            D_8013984C[i].unk_0C = 0;
            D_8013984C[i].unk_10 = 0;
            func_801344F0(i, arg0);
        }
    }
}
