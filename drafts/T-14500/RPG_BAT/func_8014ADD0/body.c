void func_8014ADD0(s32 arg0, s32 arg1) {
    s32 sp28;
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;

    if (D_8015EDB4 == 0) {
        func_8013F0F4(0x501, 1, 0);
        func_8013E97C(0x2A, arg0, arg1);
        func_8013E97C(0x2B, arg0, arg1);
        func_8013E97C(0x2F, arg0, arg1);
        func_8013E810(0x2B, 0xE, 5, 1);
    }
    if (D_8015EDB4 == 1) {
        func_8013E810(0x2A, 9, 1, 1);
    }
    if ((D_8015ECEC > 0) && (D_8015ECEC < 0xF)) {
        if (((D_8015ECF8 == 0) || (D_8015ECF8 >= 9)) && ((D_8015ECF4 == 0) || (D_8015ECF4 >= 9)) && (D_8015ECF0 == 0) && (func_8013E9A8(5) == 0)) {
            D_8015ECF0 = 1;
            func_8013E97C(0x2C, (func_8013E9A8(0x20) + arg0) - 0x10, arg1 - 8);
            func_8013E810(0x2C, 0xA, 1, 1);
        }
        if (((D_8015ECF0 == 0) || (D_8015ECF0 >= 9)) && ((D_8015ECF8 == 0) || (D_8015ECF8 >= 9)) && (D_8015ECF4 == 0) && (func_8013E9A8(5) == 0)) {
            sp28 = arg1 - 8;
            D_8015ECF4 = 1;
            func_8013E97C(0x2D, (func_8013E9A8(0x20) + arg0) - 0x10, sp28);
            func_8013E810(0x2D, 0xB, 1, 1);
        }
        temp_v1 = D_8015ECF4;
        if (((temp_v1 == 0) || (D_8015ECF4 = temp_v1, ((temp_v1 < 9) == 0))) && ((D_8015ECF0 == 0) || (D_8015ECF4 = temp_v1, ((D_8015ECF0 < 9) == 0)))) {
            D_8015ECF4 = temp_v1;
            if ((D_8015ECF8 == 0) && (func_8013E9A8(5) == 0)) {
                sp28 = arg1 - 8;
                D_8015ECF8 = 1;
                func_8013E97C(0x2E, (func_8013E9A8(0x20) + arg0) - 0x10, sp28);
                func_8013E810(0x2E, 0xC, 1, 1);
            }
        }
    }
    temp_v1_2 = D_8015ECF4;
    temp_v0 = D_8015ECF8;
    if (D_8015ECF0 != 0) {
        D_8012122A = (arg1 - (D_8015ECF0 * 4)) - 0x80;
        temp_t5 = D_8015ECF0 + 1;
        D_8015ECF0 = temp_t5;
        if (temp_t5 >= 0x21) {
            D_8015ECF0 = 0;
        }
    }
    D_8015ECF4 = temp_v1_2;
    if (temp_v1_2 != 0) {
        D_8012126E = (arg1 - (temp_v1_2 * 4)) - 0x80;
        temp_v1_3 = temp_v1_2 + 1;
        D_8015ECF4 = temp_v1_3;
        if (temp_v1_3 >= 0x21) {
            D_8015ECF4 = 0;
        }
    }
    D_8015ECF8 = temp_v0;
    if (temp_v0 != 0) {
        D_801212B2 = (arg1 - (temp_v0 * 4)) - 0x80;
        temp_v0_2 = temp_v0 + 1;
        D_8015ECF8 = temp_v0_2;
        if (temp_v0_2 >= 0x21) {
            D_8015ECF8 = 0;
        }
    }
    if (!(D_801211BE & 1)) {
        func_8013E810(0x2B, 0xF, 5, 1);
        temp_t7 = D_8015ECEC + 1;
        D_8015ECEC = temp_t7;
        if (temp_t7 == 5) {
            func_8013F0F4(0x502, 1, 0);
            func_8013E810(0x2F, 0x10, 5, 1);
        }
        if (D_8015ECEC == 0x10) {
            func_8013E810(0x2B, 0x11, 5, 1);
            func_8013E810(0x2C, 0xFF, 1, 0);
            func_8013E810(0x2D, 0xFF, 1, 0);
            func_8013E810(0x2E, 0xFF, 1, 0);
        }
        if (D_8015ECEC == 0x11) {
            func_8013E810(0x2B, 0xFF, 1, 0);
            func_8014EBA8();
        }
    }
    if ((D_8015ECEC == 0x10) && (D_801211D4 == 0x10)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
    }
    if (!(D_801212CE & 1)) {
        func_8013E810(0x2F, 0xFF, 1, 0);
    }
    D_8015EDB4 += 1;
}
