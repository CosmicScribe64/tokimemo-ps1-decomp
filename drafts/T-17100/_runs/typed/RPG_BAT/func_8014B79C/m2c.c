void func_8014B79C(void) {
    s32 sp30;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_t9;
    s32 temp_v0;
    u32 temp_t8;

    temp_a0 = D_8015EE44.unk_04;
    if (temp_a0 != 0) {
        switch (D_8015EE44.unk_00) {                /* switch 1; irregular */
        case 0x0:                                   /* switch 1 */
            func_8013E7C0(0x24, 0x32, 5, 1);
            func_8014EB58(0);
            break;
        case 0x1:                                   /* switch 1 */
            if (func_8013E90C(0x24) != 0) {
                func_8014EB58(0);
            }
            break;
        case 0x2:                                   /* switch 1 */
            func_8013E7C0(0x24, 3, 1, 1);
            func_8014EB58(0);
            break;
        case 0x3:                                   /* switch 1 */
        case 0x4:                                   /* switch 1 */
        case 0x5:                                   /* switch 1 */
        case 0x6:                                   /* switch 1 */
        case 0x7:                                   /* switch 1 */
        case 0x8:                                   /* switch 1 */
        case 0x9:                                   /* switch 1 */
        case 0xA:                                   /* switch 1 */
        case 0xB:                                   /* switch 1 */
        case 0xC:                                   /* switch 1 */
        case 0xD:                                   /* switch 1 */
            temp_v0 = D_8015EE44.unk_00 - 3;
            sp30 = temp_v0;
            if (temp_v0 >= D_8015E748) {
                D_8015EE44.unk_00 = 0xFFFE;
            } else {
                temp_t9 = D_8015EE44.unk_08 + 1;
                D_8015EE44.unk_08 = temp_t9;
                if (temp_t9 == 1) {
                    func_8004E884(func_8004E788(-0x7E, -0x66, 0, *(D_8015E740 + (sp30 * 4)), 0));
                }
                if ((temp_a0 == 4) && (func_800460EC() == 2)) {
                    D_8015EE44.unk_08 = D_8015E744 + 1;
                    D_8015EE44.unk_04 = 1;
                }
                temp_a0_2 = D_8015EE44.unk_04;
                temp_t8 = temp_a0_2 - 0x80;
                if (D_8015E744 < D_8015EE44.unk_08) {
                    if (temp_a0_2 >= 4) {
                        if (temp_a0_2 >= 5) {
                            D_8015EE44.unk_04 = temp_a0_2;
                            switch (temp_t8) {      /* switch 2 */
                            case 0:                 /* switch 2 */
                            case 1:                 /* switch 2 */
                            case 2:                 /* switch 2 */
                            case 3:                 /* switch 2 */
                            case 4:                 /* switch 2 */
                            case 5:                 /* switch 2 */
                            case 6:                 /* switch 2 */
                            case 7:                 /* switch 2 */
                            case 8:                 /* switch 2 */
                            case 9:                 /* switch 2 */
                            case 10:                /* switch 2 */
                                D_8015EE44.unk_04 = temp_a0_2;
                                if (sp30 != (temp_a0_2 & 0x7F)) {
                                    func_8004EAAC(temp_a0_2);
                                    func_8014EB58(0);
                                }
                                break;
                            }
                        } else if (temp_a0_2 != 4) {
                            D_8015EE44.unk_04 = temp_a0_2;
                        } else {
                            D_8015EE44.unk_04 = 1;
                        }
                    } else if (temp_a0_2 != 1) {
                        D_8015EE44.unk_04 = temp_a0_2;
                        if (temp_a0_2 != 3) {

                        }
                    } else {
                        D_8015EE44.unk_04 = temp_a0_2;
                        func_8004EAAC(temp_a0_2);
                        func_8014EB58(0);
                    }
                }
            }
            break;
        case 0xFFFE:                                /* switch 1 */
            func_8013E7C0(0x24, 0x32, 0x21, 1);
            *(s16 *)((unsigned char *)D_8011ECD0 + (9000)) = 6;
            func_8014EB58(0);
            break;
        default:                                    /* switch 1 */
            if (func_8013E90C(0x24) != 0) {
                func_8013E7C0(0x24, 0xFF, 0, 0);
                D_8015EE44.unk_08 = 0;
                D_8015EE44.unk_00 = 0;
                D_8015EE44.unk_04 = 0;
            }
            break;
        }
    }
}
