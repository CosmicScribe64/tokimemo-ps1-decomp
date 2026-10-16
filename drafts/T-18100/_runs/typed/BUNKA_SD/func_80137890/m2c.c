void func_80137890(void) {
    switch ((u8) D_800E6280.unk_1109) {             /* switch 1; irregular */
    case 0x0:                                       /* switch 1 */
        func_80066C08(2);
        func_8006492C(0);
        func_80064F48();
        func_80042808();
        return;
    case 0x1:                                       /* switch 1 */
        switch (D_80122EB8) {                       /* switch 2 */
        case 2:                                     /* switch 2 */
            switch (D_800E6280.unk_03E) {           /* switch 3; irregular */
            case 0x5F:                              /* switch 3 */
                func_80042908(0xF0);
                return;
            case 0x60:                              /* switch 3 */
                func_80042908(0xF1);
                return;
            case 0x61:                              /* switch 3 */
                func_80042908(0xF2);
                return;
            }
            break;
        case 3:                                     /* switch 2 */
            func_80042908(0xF3);
            return;
        case 5:                                     /* switch 2 */
            func_80042908(0xF4);
            return;
        case 0:                                     /* switch 2 */
            func_80042908(0xF5);
            return;
        case 1:                                     /* switch 2 */
            switch (D_800E6280.unk_03E) {           /* switch 4; irregular */
            case 0x5F:                              /* switch 4 */
                func_80042908(0xF6);
                return;
            case 0x60:                              /* switch 4 */
                func_80042908(0xF7);
                return;
            case 0x61:                              /* switch 4 */
                func_80042908(0xF8);
                return;
            }
            break;
        default:                                    /* switch 2 */
            return;
        }
        break;
    case 0x10:                                      /* switch 1 */
        if ((u32) ((u16) D_800E6280.unk_000 >> 0xC) < 6U) {
            main_state_set(0x70);
        } else {
            main_state_set(0x71);
        }
        func_80042908((s32) D_80122EB0);
        func_80042940((s32) D_80122EB4);
        return;
    case 0xF0:                                      /* switch 1 */
        func_801362EC();
        return;
    case 0xF1:                                      /* switch 1 */
        func_801351DC();
        return;
    case 0xF2:                                      /* switch 1 */
        func_80135A2C();
        return;
    case 0xF3:                                      /* switch 1 */
        func_801345BC();
        return;
    case 0xF4:                                      /* switch 1 */
        func_80138ADC();
        return;
    case 0xF5:                                      /* switch 1 */
        func_801322EC();
        return;
    case 0xF6:                                      /* switch 1 */
        func_801336AC();
        return;
    case 0xF7:                                      /* switch 1 */
        func_80137CAC();
        return;
    case 0xF8:                                      /* switch 1 */
        func_80136B3C();
        break;
    }
}
