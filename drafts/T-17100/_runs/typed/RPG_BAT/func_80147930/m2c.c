extern s32 D_8015EBD8;
extern s32 D_8015EBE0;

void func_80147930(void) {
    s32 temp_v0;
    s32 var_a1;
    s32 var_v1;

    var_a1 = D_8015EBFC.unk_14;
    var_v1 = D_8015EBFC.unk_10;
    if ((var_a1 + var_v1) < 0x14) {
        switch (D_8015EB9C) {                       /* switch 1 */
        case 0:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 2:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 3:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 4:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 5:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 6:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 10:                                    /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 8:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 1:                                     /* switch 1 */
            var_v1 = 0x64;
block_16:
            var_a1 = 0x64;
            break;
        case 7:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 9:                                     /* switch 1 */
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 13:                                    /* switch 1 */
            var_v1 = 0x64;
            goto block_16;
        case 14:                                    /* switch 1 */
            var_v1 = 0x64;
            goto block_16;
        }
    }
    D_8015EBFC.unk_10 = var_v1;
    D_8015EBFC.unk_14 = var_a1;
    switch (D_8015EB9C) {                           /* switch 2 */
    case 0:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EDC8 = 0;
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        return;
    case 2:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        return;
    case 3:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        return;
    case 4:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        return;
    case 5:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        return;
    case 6:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBD8 += 0xA;
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
    default:                                        /* switch 2 */
        return;
    case 10:                                        /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBE0 = 0x32;
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        break;
    case 8:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        temp_v0 = func_8013E9A8(var_a1 + var_v1);
        if (D_8015EBDC >= 0x65) {
            return;
        }
        if (temp_v0 < D_8015EBFC.unk_00) {
            D_8015EBFC.unk_10 = D_8015EBFC.unk_00 - 5;
            return;
        }
        D_8015EBDC = 0x168;
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        break;
        return;
    case 7:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        if (D_8015EDC4 & 0x100000) {
            if ((D_8015EBD4 + *D_8015EBCC + D_8015EBD0) == 3) {
                return;
            }
            if (func_8013E9A8(0x64) < 0x32) {

            } else {
                return;
            }
        } else {
            return;
        }
        break;
    case 9:                                         /* switch 2 */
        D_8015EBFC.unk_10 = var_v1;
        D_8015EBFC.unk_14 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return;
        }
        D_8015EBFC.unk_14 = D_8015EBFC.unk_00 - 5;
        if (D_8015EDF0 != 0x40000000) {

        } else {
            if (func_8013E9A8(5) == 0) {
                return;
            }
            return;
        }
        break;
    case 13:                                        /* switch 2 */
        if (D_8015EDEC < 8) {
            return;
        }
        if (func_8013E9A8(2) == 0) {
            return;
        }
        break;
    }
}
