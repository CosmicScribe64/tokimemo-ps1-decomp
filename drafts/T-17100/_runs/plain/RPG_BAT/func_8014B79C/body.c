s32 func_8014B79C(void) {
    s32 sp30;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_t9;
    s32 temp_v0;
    u32 temp_t8;

    temp_a0 = D_8015EE48;
    if (temp_a0 != 0) {
        switch (D_8015EE44.unk_00) {
        case 0x0:
            func_8013E7C0(0x24, 0x32, 5, 1);
            func_8014EB58(0);
            break;
        case 0x1:
            if (func_8013E90C(0x24) != 0) {
                func_8014EB58(0);
            }
            break;
        case 0x2:
            func_8013E7C0(0x24, 3, 1, 1);
            func_8014EB58(0);
            break;
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
            temp_v0 = D_8015EE44.unk_00 - 3;
            sp30 = temp_v0;
            if (temp_v0 >= D_8015E748) {
                D_8015EE44.unk_00 = 0xFFFE;
            } else {
                temp_t9 = D_8015EE4C + 1;
                D_8015EE4C = temp_t9;
                if (temp_t9 == 1) {
                    k_disp_start(set_kanji_string(-0x7E, -0x66, 0, *(s32 *)(D_8015E740 + (sp30 * 4)), 0));
                }
                if ((temp_a0 == 4) && (func_800460EC() == 2)) {
                    D_8015EE4C = D_8015E744 + 1;
                    D_8015EE48 = 1;
                }
                temp_a0_2 = D_8015EE48;
                temp_t8 = temp_a0_2 - 0x80;
                if (D_8015E744 < D_8015EE4C) {
                    if (temp_a0_2 >= 4) {
                        if (temp_a0_2 >= 5) {
                            D_8015EE48 = temp_a0_2;
                            switch (temp_t8) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                                D_8015EE48 = temp_a0_2;
                                if (sp30 != (temp_a0_2 & 0x7F)) {
                                    k_sub_reset(temp_a0_2);
                                    func_8014EB58(0);
                                }
                                break;
                            }
                        } else if (temp_a0_2 != 4) {
                            D_8015EE48 = temp_a0_2;
                        } else {
                            D_8015EE48 = 1;
                        }
                    } else if (temp_a0_2 != 1) {
                        D_8015EE48 = temp_a0_2;
                        if (temp_a0_2 != 3) {

                        }
                    } else {
                        D_8015EE48 = temp_a0_2;
                        k_sub_reset(temp_a0_2);
                        func_8014EB58(0);
                    }
                }
            }
            break;
        case 0xFFFE:
            func_8013E7C0(0x24, 0x32, 0x21, 1);
            D_80120FF8 = 6;
            func_8014EB58(0);
            break;
        default:
            if (func_8013E90C(0x24) != 0) {
                func_8013E7C0(0x24, 0xFF, 0, 0);
                D_8015EE4C = 0;
                D_8015EE44.unk_00 = 0;
                D_8015EE48 = 0;
            }
            break;
        }
    }
}
