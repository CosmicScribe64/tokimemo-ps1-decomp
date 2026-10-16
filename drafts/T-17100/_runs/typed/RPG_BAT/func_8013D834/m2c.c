void func_8013D834(void) {
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 == 0) {
        switch (D_8015EB98) {                       /* switch 1; irregular */
        case 0x10000:                               /* switch 1 */
            func_80044890(1, 0xC3A8, 0xC38D, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x20000:                               /* switch 1 */
            func_80044890(1, 0xC38B, 0xC370, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x40000:                               /* switch 1 */
            func_80044890(1, 0xC36E, 0xC355, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x80000000:                            /* switch 1 */
            func_80044890(1, 0xC430, 0xC402, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x8000000:                             /* switch 1 */
            func_80044890(1, 0xC3C9, 0xC3AA, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x100000:                              /* switch 1 */
            switch (D_8015EB9C) {                   /* switch 2 */
            case 2:                                 /* switch 2 */
            case 4:                                 /* switch 2 */
            case 5:                                 /* switch 2 */
                func_80044890(1, 0xC2E8, 0xC2B6, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
            case 3:                                 /* switch 2 */
            case 10:                                /* switch 2 */
                func_80044890(1, 0xC31F, 0xC2EA, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            case 6:                                 /* switch 2 */
            case 7:                                 /* switch 2 */
            case 8:                                 /* switch 2 */
            case 9:                                 /* switch 2 */
                func_80044890(1, 0xC353, 0xC321, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            }
            break;
        }
        D_800E6280.unk_110D = temp_v1 + 1;
        return;
    }
    if (temp_v1 == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else if (temp_v1 == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else {
        if (temp_v1 == 3) {
            if (D_8015EBA4 == 1) {
                switch (D_8015EBA0) {               /* switch 3 */
                case 1:                             /* switch 3 */
                case 5:                             /* switch 3 */
                case 6:                             /* switch 3 */
                case 7:                             /* switch 3 */
                case 8:                             /* switch 3 */
                case 10:                            /* switch 3 */
                    func_80044890(0, 0xC22E, 0xC211, 0, 0, 0);
                    break;
                case 9:                             /* switch 3 */
                case 11:                            /* switch 3 */
                    func_80044890(0, 0xC244, 0xC230, 0, 0, 0);
                    break;
                case 0:                             /* switch 3 */
                case 3:                             /* switch 3 */
                case 4:                             /* switch 3 */
                    func_80044890(0, 0xC265, 0xC246, 0, 0, 0);
                    break;
                case 2:                             /* switch 3 */
                    func_80044890(0, 0xC286, 0xC267, 0, 0, 0);
                    break;
                }
                D_800E6280.unk_110D = temp_v1 + 1;
                return;
            }
            D_800E6280.unk_110D = temp_v1 + 3;
            return;
        }
        if (temp_v1 == 4) {
            if (func_80044E8C() == 1) {
                D_800E6280.unk_110D += 1;
            }
        } else if (temp_v1 == 5) {
            if (func_80044F94(0) == 1) {
                D_800E6280.unk_110D += 1;
            }
        } else {
            func_80042808();
        }
    }
}
