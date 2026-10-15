void func_801498C0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　気分最高　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EEF4();
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        if (D_8015EBAC == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        switch (D_8015EC34) {
        default:
            D_8015EC74 = 5;
            break;
        case 1:
            D_8015EC74 = 6;
            break;
        case 2:
            D_8015EC74 = 7;
            break;
        case 3:
            D_8015EC74 = 8;
            break;
        case 4:
            D_8015EC74 = 9;
            break;
        case 5:
            D_8015EC74 = 0xA;
            break;
        }
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8014A3DC();
        return;
    case 4:
        func_8014A16C();
        return;
    case 5:
        func_80149C5C();
        return;
    case 6:
        func_80149AA8();
        return;
    case 7:
        func_8013D3E4();
        func_8014EBA8();
        return;
    case 8:
        func_8013D59C();
        return;
    case 9:
        func_8014EBEC();
        D_8015EC34 = 0;
        /* fallthrough */
    default:
        return;
    }
}
