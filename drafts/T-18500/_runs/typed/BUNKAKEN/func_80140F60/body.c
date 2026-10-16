void func_80140F60(void) {
    D_80122EBC = 2;
    switch (D_80122EB8) {
    case 2:
        if ((func_8008667C(2) == 4) && !(D_800E6280.unk_1BC[2].unk_0C.w & 4)) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        func_80042908(0x31);
        return;
    case 3:
        switch (D_800E6280.unk0) {
        case 0x5F:
            func_80042908(0x32);
            break;
        case 0x60:
            func_80042908(0x33);
            break;
        case 0x61:
            func_80042908(0x34);
            break;
        }
        if ((func_8008667C(2) == 4) && !(D_800E6280.unk_1BC[2].unk_0C.w & 4)) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        return;
    default:
        func_80046500();
        break;
    }
}
