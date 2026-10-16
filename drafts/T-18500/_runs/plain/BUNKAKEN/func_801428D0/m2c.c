extern u8 D_800E62BE;
extern s32 D_800E64F0;

void func_801428D0(void) {
    D_80122EBC = 3;
    switch (D_80122EB8) {                           /* switch 1; irregular */
    case 4:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 2; irregular */
        case 0x5F:                                  /* switch 2 */
            if ((func_8008667C(3) == 4) && !(D_800E64F0 & 4)) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x36);
            return;
        case 0x60:                                  /* switch 2 */
            if ((func_8008667C(3) == 4) && !(D_800E64F0 & 4)) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x37);
            return;
        case 0x61:                                  /* switch 2 */
            if ((func_8008667C(3) == 4) && !(D_800E64F0 & 4)) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x38);
            return;
        }
        break;
    case 5:                                         /* switch 1 */
        func_80042908(0x39);
        return;
    default:                                        /* switch 1 */
        func_80046500();
        return;
    }
}
