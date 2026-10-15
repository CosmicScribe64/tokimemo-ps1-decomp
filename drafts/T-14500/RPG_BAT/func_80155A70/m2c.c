void func_80155A70(void) {
    if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF) && (D_8015EDF0 & 0x80000000) && (((s32) D_8015EDA0 / 10) < (D_8015EDA0 - D_8015ED94))) {
        func_8013D030();
        D_8015EDF0 = 1;
    }
    if (D_8015EDF0 == 1) {
        switch (D_8015EE04) {
        case 0:
            func_8013E7C0(0x1D, 5, 1, 1);
            func_800AE0F0(D_8015E904, "如月「ああっ！　めまいが！");
            func_8014B738((s32) D_8015E9CC, 1, 0x1F4);
            func_8013F1C4(0x03000008, 0x02000010, 0);
            func_8014F230();
            break;
        case 1:
            func_8014F350(0x46);
            break;
        case 2:
            func_8013E7C0(0x1D, 6, 1, 1);
            func_8013F15C(0x511, 1, 0);
            func_8014F230();
            break;
        case 3:
            if (func_800460EC() == 2) {
                D_8015EE4C = D_8015E744 + 1;
                func_8014F230();
            }
            if (D_8015EE4C >= 0x1F5) {
                D_8015EE4C = D_8015E744 + 1;
                func_8014F230();
            }
            break;
        case 4:
            func_8014F350(0x14);
            break;
        case 5:
            func_8013E7C0(0x1D, 7, 1, 1);
            func_8013F15C(0x512, 1, 0);
            func_8014F230();
            break;
        case 6:
            func_8014F350(0x1E);
            break;
        case 7:
            func_8014F278();
            D_8015EDF0 = 0x1000;
            break;
        }
    } else {
    default:
    }
}
