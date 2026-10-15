extern s16 D_80120E0C;

void func_8014F88C(void) {
    s32 sp28;
    s32 var_a0;
    u32 temp_v0;

    temp_v0 = D_8015EE04;
    D_8015EE00.unk_00 += 1;
    D_80120E05 = 3;
    D_80120E49 = 3;
    if (((s32) temp_v0 >= 2) && ((s32) temp_v0 < 0x15) && (D_80120E0C == 1)) {
        switch ((u16) D_80120E1C) {                 /* switch 1 */
        case 0:                                     /* switch 1 */
        case 2:                                     /* switch 1 */
            if (func_8013E9A8(3) == 0) {
                func_8013F15C(0x50C, 1, 9);
            }
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50B, 1, 9);
            }
            break;
        case 4:                                     /* switch 1 */
        case 8:                                     /* switch 1 */
        case 12:                                    /* switch 1 */
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50A, 1, 9);
            }
            break;
        }
    }
    switch (temp_v0) {                              /* switch 2 */
    case 0:                                         /* switch 2 */
        func_8013E54C(5);
        var_a0 = 4;
        do {
            sp28 = var_a0;
            func_8013E97C(var_a0, -0x20, 0);
            var_a0 += 1;
        } while (var_a0 != 0x10);
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8013E7C0(0x1E, 2, 1, 1);
        func_8013E97C(0x1D, -0x20, 0);
        func_8013F15C(0x509, 1, 0);
        func_8014F230();
    default:                                        /* switch 2 */
        break;
    case 1:                                         /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x3D) {
            func_8014F230();
        }
        break;
    case 2:                                         /* switch 2 */
        func_8013E7C0(8, 7, 1, 1);
        func_8014F230();
        break;
    case 3:                                         /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x4B1) {
            func_8014F230();
        }
        break;
    case 4:                                         /* switch 2 */
        func_8013E7C0(8, 8, 1, 1);
        func_8014F230();
        break;
    case 5:                                         /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x961) {
            func_8014F230();
        }
        break;
    case 6:                                         /* switch 2 */
        func_8013E7C0(8, 9, 1, 1);
        func_8014F230();
        break;
    case 7:                                         /* switch 2 */
        if (D_8015EE00.unk_00 >= 0xE11) {
            func_8014F230();
        }
        break;
    case 8:                                         /* switch 2 */
        func_8013E7C0(0x1E, 3, 1, 1);
        func_8013E7C0(8, 0xA, 1, 1);
        func_8014F230();
        break;
    case 9:                                         /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x12C1) {
            func_8014F230();
        }
        break;
    case 10:                                        /* switch 2 */
        func_8013E7C0(8, 0xB, 1, 1);
        func_8014F230();
        break;
    case 11:                                        /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x1771) {
            func_8014F230();
        }
        break;
    case 12:                                        /* switch 2 */
        func_8013E7C0(0x1E, 4, 1, 1);
        func_8013E7C0(8, 0xC, 1, 1);
        func_8014F230();
        break;
    case 13:                                        /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x1C21) {
            func_8014F230();
        }
        break;
    case 14:                                        /* switch 2 */
        func_8013E7C0(8, 0xD, 1, 1);
        func_8014F230();
        break;
    case 15:                                        /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x20D1) {
            func_8014F230();
        }
        break;
    case 16:                                        /* switch 2 */
        func_8013E7C0(0x1E, 5, 1, 1);
        func_8013E7C0(8, 0xE, 1, 1);
        func_8014F230();
        break;
    case 17:                                        /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x2581) {
            func_8014F230();
        }
        break;
    case 18:                                        /* switch 2 */
        func_8013E7C0(8, 0xF, 1, 1);
        func_8014F230();
        break;
    case 19:                                        /* switch 2 */
        if (D_8015EE00.unk_00 >= 0x2A31) {
            func_8014F230();
        }
        break;
    case 20:                                        /* switch 2 */
        if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF)) {
            func_8014F278();
            func_8013F15C(0x50D, 1, 0);
            D_8015EDF0 = 0x10001001;
        }
        break;
    }
}
