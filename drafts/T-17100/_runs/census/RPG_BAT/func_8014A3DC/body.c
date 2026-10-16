void func_8014A3DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    if (D_8015EDB0.unk_00 < 5) {
        func_8013F0F4(0x503, 0x1E, 1);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8013E7C0(0x2F, 0xD, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        func_8014EDFC(0x2F);
        return;
    case 2:
        func_8013E7C0(0x2F, 7, 1, 1);
        func_8013E7C0(0x2A, 8, 1, 1);
        func_8013E7C0(0x2B, 9, 1, 1);
        func_8013E7C0(0x2C, 0xA, 1, 1);
        func_8013E7C0(0x2D, 0xB, 1, 1);
        func_8013E7C0(0x2E, 0xC, 1, 1);
        func_8014EBD0();
        return;
    case 3:
        func_8013E97C(0x29, 0xF0, 0x38);
        func_8013E97C(0x28, 0xF0, 0x38);
        func_8013E810(0x29, 0xF, 1, 1);
        func_8014EBD0();
        return;
    case 4:
        if (D_8015EDB0.unk_08 >= 0x1F) {
            temp_v0 = D_8015EDA4 + 1;
            D_8015EDB0.unk_08 = 0;
            if (temp_v0 == D_8015EC74) {
                D_8015EDA4 = temp_v0;
                func_8014EBD0();
            } else {
                D_8015EDA4 = temp_v0;
                func_8013E810(0x29, temp_v0 + 0xF, 1, 1);
                temp_v0_2 = D_8015EDA4;
                if ((temp_v0_2 == 6) || (D_8015EDA4 = temp_v0_2, (temp_v0_2 == 7))) {
                    func_8013E810(0x28, 6, 1, 1);
                }
                temp_v0_3 = D_8015EDA4;
                if ((temp_v0_3 == 8) || (D_8015EDA4 = temp_v0_3, (temp_v0_3 == 9))) {
                    func_8013E810(0x28, 0x19, 1, 1);
                }
            }
        }
        D_8015EDB0.unk_08 += 1;
        return;
    case 5:
        func_8014ED80(0x1E);
        return;
    case 6:
        func_8014EBA8();
        /* fallthrough */
    default:
        return;
    }
}
