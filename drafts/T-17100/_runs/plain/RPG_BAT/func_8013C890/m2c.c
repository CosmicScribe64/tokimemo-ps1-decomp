void func_8013C890(void) {
    s32 temp_t6;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v1;
    s32 temp_v1_2;

    if ((func_80144640() < D_8015EDC8) && !(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF) && !(D_8015EDF0 & 0xFF)) {
        D_8015EBDC = func_8013EA00(0x14, 0x28);
        D_8015EDC8 = 0;
        temp_v1 = D_8015EDC4 | func_80144B7C();
        D_8015EDC4 = temp_v1;
        if (!(temp_v1 & 0xFF)) {
            D_8015EDC8 = func_8013EA00(0x3C, 0x5A);
        }
    }
    temp_v1_2 = D_8015EDC4;
    if (temp_v1_2 & 0xFF) {
        temp_v0 = temp_v1_2 & 0xFFFF0000;
        switch (temp_v0) {                          /* switch 1; irregular */
        default:                                    /* switch 1 */
            temp_t6 = temp_v1_2 & 0xFFFF;
            if (temp_v0 != 0x100000) {
                temp_t8 = temp_v1_2 & 0xFFFF;
                switch (temp_v0) {                  /* switch 2; irregular */
                default:                            /* switch 2 */
                    D_8015EDC4 = temp_v1_2;
                    break;
                case 0x200000:                      /* switch 2 */
                case 0x400000:                      /* switch 2 */
                    D_8015EDC4 = temp_v1_2;
                    switch (temp_t8) {              /* switch 7 */
                    case 1:                         /* switch 7 */
                        func_80158A0C();
                        break;
                    case 2:                         /* switch 7 */
                        func_80158C4C();
                        break;
                    case 3:                         /* switch 7 */
                        func_8015906C();
                        break;
                    case 4:                         /* switch 7 */
                        func_80159214();
                        break;
                    case 5:                         /* switch 7 */
                        func_80159620();
                        break;
                    }
                    break;
                case 0x8000000:                     /* switch 2 */
                    D_8015EDC4 = temp_v1_2;
                    temp_v0_2 = temp_v1_2 & 0xFFFF;
                    switch (temp_v0_2) {            /* switch 8; irregular */
                    case 1:                         /* switch 8 */
                        func_8013FD8C();
                        break;
                    case 2:                         /* switch 8 */
                        func_80140CC8();
                        break;
                    }
                    break;
                }
            } else {
                D_8015EDC4 = temp_v1_2;
                switch (temp_t6) {                  /* switch 6 */
                case 1:                             /* switch 6 */
                    func_80138120();
                    break;
                case 2:                             /* switch 6 */
                    func_801387F0();
                    break;
                case 3:                             /* switch 6 */
                    func_80138170();
                    break;
                case 4:                             /* switch 6 */
                    func_80138324();
                    break;
                case 5:                             /* switch 6 */
                    func_80138B9C();
                    break;
                case 6:                             /* switch 6 */
                    func_80138F6C();
                    break;
                case 7:                             /* switch 6 */
                    func_80139460();
                    break;
                }
            }
            break;
        case 0x10000:                               /* switch 1 */
            D_8015EDC4 = temp_v1_2;
            temp_v0_3 = temp_v1_2 & 0xFFFF;
            switch (temp_v0_3) {                    /* switch 3; irregular */
            case 1:                                 /* switch 3 */
                func_80152080();
                break;
            case 2:                                 /* switch 3 */
                func_80145CC0();
                break;
            }
            break;
        case 0x20000:                               /* switch 1 */
            D_8015EDC4 = temp_v1_2;
            temp_v0_4 = temp_v1_2 & 0xFFFF;
            switch (temp_v0_4) {                    /* switch 4; irregular */
            case 1:                                 /* switch 4 */
                func_80152080();
                break;
            case 2:                                 /* switch 4 */
                func_80146C30();
                break;
            }
            break;
        case 0x40000:                               /* switch 1 */
            D_8015EDC4 = temp_v1_2;
            temp_v0_5 = temp_v1_2 & 0xFFFF;
            switch (temp_v0_5) {                    /* switch 5; irregular */
            case 1:                                 /* switch 5 */
                func_80152080();
                break;
            case 2:                                 /* switch 5 */
                func_80133108();
                break;
            }
            break;
        case 0x80000000:                            /* switch 1 */
            D_8015EDC4 = temp_v1_2;
            temp_v0_6 = temp_v1_2 & 0xFFFF;
            switch (temp_v0_6) {                    /* switch 9; irregular */
            case 1:                                 /* switch 9 */
                func_80156D78();
                break;
            case 2:                                 /* switch 9 */
                func_80157158();
                break;
            case 3:                                 /* switch 9 */
                func_80157640();
                break;
            case 4:                                 /* switch 9 */
                func_80156CC0();
                break;
            }
            break;
        }
    }
}
