void func_80140E70(void) {
    s32 temp_hi;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_8015EDDC) {
    case 0:
        func_8013E97C(0x3B, 0x48, 0x60);
        func_8013E810(0x3B, 2, 1, 1);
        load_palette(D_8015EEEC, 0x12, 1, 4, 0);
        func_8014EF6C();
        break;
    case 1:
        temp_v0 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_v0;
        if (temp_v0 == 0x1E) {
            func_8013F15C(0x504, 1, 0);
        }
        if (!(D_8015EDE4 & 3)) {
            temp_hi = (s32) ((s32) D_8015EDE4 / 4) % 5;
            D_8015EDE4 = D_8015EDE4;
            load_palette(D_8015EEEC, 0x12, 1, 4, temp_hi);
        }
        if (D_8015EDE4 >= 0x4F) {
            func_8014EF6C();
        }
        break;
    case 2:
        temp_v0_2 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_v0_2;
        if (!(temp_v0_2 & 3)) {
            D_8015EDE4 = temp_v0_2;
            load_palette(D_8015EEEC, 0x12, 1, 4, ((s32) (temp_v0_2 / 4) % 6) + 5);
        }
        if (D_8015EDE4 >= 0x17) {
            func_8014EF6C();
        }
        break;
    case 3:
        load_palette(D_8015EED8, 0x12, 1, 4, 4);
        func_8014EF3C();
        break;
    }
}
