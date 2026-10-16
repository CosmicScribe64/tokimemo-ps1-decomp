extern u32 D_800E7380;
extern u8 D_800E7389;

void func_80137144(void) {
    func_80083808();
    switch (D_800E7389) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_80137664();
        break;
    case 1:                                         /* switch 1 */
        func_80137818();
        break;
    case 2:                                         /* switch 1 */
        func_80137A04();
        break;
    case 3:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_80042808();
        } else {
            D_800E7380 += 1;
            if ((u32) D_800E7380 >= 0x401U) {
                func_80042908(2);
            }
        }
        break;
    case 4:                                         /* switch 1 */
        switch (D_80144E10) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80046500();
            break;
        case 1:                                     /* switch 2 */
            func_801398F0();
            break;
        case 2:                                     /* switch 2 */
            func_80137FF0();
            break;
        case 3:                                     /* switch 2 */
            func_80138B40();
            break;
        case 4:                                     /* switch 2 */
            func_8013A950();
            break;
        case 5:                                     /* switch 2 */
            func_8013A190();
            break;
        case 6:                                     /* switch 2 */
            func_8013A500();
            break;
        case 7:                                     /* switch 2 */
            func_801391D4();
            break;
        case 8:                                     /* switch 2 */
            func_80137BB0();
            break;
        case 9:                                     /* switch 2 */
            func_80046500();
            break;
        case 10:                                    /* switch 2 */
            func_8013AF10();
            break;
        default:                                    /* switch 2 */
            func_80046500();
            break;
        }
        break;
    case 5:                                         /* switch 1 */
        func_801377CC();
        break;
    case 6:                                         /* switch 1 */
        func_80137818();
        break;
    case 7:                                         /* switch 1 */
        func_8013737C();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_8007EDF8();
    func_800846C0();
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    func_80064DEC();
    func_80083A10();
    func_80047560();
}
