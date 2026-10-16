s32 func_80134B94(s32 arg0) {
    s32 i;
    s32 n;
    s32 r;

    r = func_800AE0D0() % 15;
    n = 0;
    if (arg0 == 0) {
        for (i = 0; i < 6; i++) {
            if (D_8014CC74[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC74[n] == 0) {
            func_800AE090(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
        if (n == 6) {
            func_800AE090(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
        if (D_8014CC74[0] == 0) {
            func_800AE090(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
    }
    if (arg0 == 1) {
        for (i = 0; i < 6; i++) {
            if (D_8014CC7C[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC7C[n] == 0) {
            func_800AE090(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
        if (n == 6) {
            func_800AE090(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
        if (D_8014CC7C[0] == 0) {
            func_800AE090(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
    }
    if (arg0 == 2) {
        for (i = 0; i < 10; i++) {
            if (D_8014CC84[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC84[n] == 0) {
            func_800AE090(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
        if (n == 10) {
            func_800AE090(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
        if (D_8014CC84[0] == 0) {
            func_800AE090(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
    }
}
