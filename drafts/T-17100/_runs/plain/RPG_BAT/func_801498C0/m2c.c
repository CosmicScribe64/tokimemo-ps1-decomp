void func_801498C0(void) {
    switch (D_8015EDAC) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        func_800AE0F0(D_8015E74C, "　　　　　　　気分最高　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EEF4();
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        if (D_8015EBAC == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        switch (D_8015EC34) {                       /* switch 2 */
        default:                                    /* switch 2 */
            D_8015EC74 = 5;
            break;
        case 1:                                     /* switch 2 */
            D_8015EC74 = 6;
            break;
        case 2:                                     /* switch 2 */
            D_8015EC74 = 7;
            break;
        case 3:                                     /* switch 2 */
            D_8015EC74 = 8;
            break;
        case 4:                                     /* switch 2 */
            D_8015EC74 = 9;
            break;
        case 5:                                     /* switch 2 */
            D_8015EC74 = 0xA;
            break;
        }
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:                                         /* switch 1 */
        func_8013D418();
        return;
    case 3:                                         /* switch 1 */
        func_8014A3DC();
        return;
    case 4:                                         /* switch 1 */
        func_8014A16C();
        return;
    case 5:                                         /* switch 1 */
        func_80149C5C();
        return;
    case 6:                                         /* switch 1 */
        func_80149AA8();
        return;
    case 7:                                         /* switch 1 */
        func_8013D3E4();
        func_8014EBA8();
        return;
    case 8:                                         /* switch 1 */
        func_8013D59C();
        return;
    case 9:                                         /* switch 1 */
        func_8014EBEC();
        D_8015EC34 = 0;
        /* fallthrough */
    default:                                        /* switch 1 */
        return;
    }
}
