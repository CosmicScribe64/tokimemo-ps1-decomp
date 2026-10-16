void func_801428D0(void) {
    D_80122EBC = 3;
    switch (D_80122EB8) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x36);
            return;
        case 0x60:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x37);
            return;
        case 0x61:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x38);
            return;
        }
        break;
    case 5:
        func_80042908(0x39);
        return;
    default:
        func_80046500();
        return;
    }
}
