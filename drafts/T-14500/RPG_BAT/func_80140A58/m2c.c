extern u8 D_801215BA;

void func_80140A58(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_8015EDDC) {
    case 0:
        func_8013E97C(0x3A, -8, 0);
        func_8013E7C0(0x3A, 0xF, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF6C();
        return;
    case 1:
        if (!(D_801215BA & 1)) {
            func_8013E7C0(0x3A, 0x10, 5, 1);
            func_8013F15C(0x50A, 1, 0);
            func_8014EF6C();
            return;
        }
    default:
        return;
    case 2:
        if (!(D_801215BA & 1)) {
            func_8013F15C(0x50A, 1, 0);
            func_8013E7C0(0x3A, 0x10, 5, 1);
            temp_v0 = D_8015EDE4 + 1;
            D_8015EDE4 = temp_v0;
            if (temp_v0 >= 4) {
                func_8013E7C0(0x3A, 0x10, 1, 1);
                func_8013E810(0x3B, 0x12, 5, 1);
                func_8014EF6C();
                return;
            }
        }
        break;
    case 3:
        func_8014F138(0x3B);
        return;
    case 4:
        func_8013E7C0(0x3A, 0x11, 5, 1);
        if (!(D_8015ED98 & 0x100)) {
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        func_8013F15C(0x50B, 1, 0);
        func_8014EF6C();
        return;
    case 5:
        if (!(D_801215BA & 1)) {
            func_8013F15C(0x50B, 1, 0);
            func_8013E7C0(0x3A, 0x11, 5, 1);
            temp_v0_2 = D_8015EDE4 + 1;
            D_8015EDE4 = temp_v0_2;
            if (temp_v0_2 >= 9) {
                func_8013E7C0(0x3A, 0xFF, 1, 0);
                func_8013E810(0x3B, 0x13, 5, 1);
                func_8014EF6C();
                return;
            }
        }
        break;
    case 6:
        func_8014F138(0x3B);
        return;
    case 7:
        func_8013E810(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    }
}
