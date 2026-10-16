extern u8 D_800E62BE;
extern u32 D_800E7384;
extern s16 D_801207BC;

void func_80139290(void) {
    s16 var_t5;
    s32 temp_v1;
    u32 temp_lo;
    u32 temp_lo_2;

    switch (D_800E7384) {                           /* switch 1; irregular */
    case 0x1D:                                      /* switch 1 */
        D_801206AC = 1;
        break;
    case 0x58:                                      /* switch 1 */
        D_801206AC = 0;
        break;
    case 0x66:                                      /* switch 1 */
        D_801206AC = 2;
        break;
    case 0x83:                                      /* switch 1 */
        D_801206AC = 3;
        break;
    }
    if (((u32) D_800E7384 >= 0xBFU) && ((u32) D_800E7384 < 0x70000000U)) {
        temp_lo = (u32) ((u32) (D_800E7384 - 0xBE) % 242U) / 11U;
        switch (temp_lo) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            var_t5 = 4;
block_35:
            D_801206AC = var_t5;
            break;
        case 1:                                     /* switch 2 */
            D_801206AC = 3;
            break;
        case 2:                                     /* switch 2 */
            D_801206AC = 5;
            break;
        case 3:                                     /* switch 2 */
            D_801206AC = 6;
            break;
        case 4:                                     /* switch 2 */
            D_801206AC = 3;
            break;
        case 5:                                     /* switch 2 */
            D_801206AC = 0xC;
            break;
        case 6:                                     /* switch 2 */
            D_801206AC = 0xB;
            break;
        case 7:                                     /* switch 2 */
            var_t5 = 0xE;
            goto block_35;
        case 8:                                     /* switch 2 */
            D_801206AC = 3;
            break;
        case 9:                                     /* switch 2 */
            D_801206AC = 8;
            break;
        case 10:                                    /* switch 2 */
            D_801206AC = 7;
            break;
        case 11:                                    /* switch 2 */
            D_801206AC = 9;
            break;
        case 12:                                    /* switch 2 */
            D_801206AC = 3;
            break;
        case 13:                                    /* switch 2 */
            D_801206AC = 5;
            break;
        case 14:                                    /* switch 2 */
            var_t5 = 6;
            goto block_35;
        case 15:                                    /* switch 2 */
            D_801206AC = 3;
            break;
        case 16:                                    /* switch 2 */
            D_801206AC = 0xD;
            break;
        case 17:                                    /* switch 2 */
            D_801206AC = 0xB;
            break;
        case 18:                                    /* switch 2 */
            D_801206AC = 4;
            break;
        case 19:                                    /* switch 2 */
            D_801206AC = 3;
            break;
        case 20:                                    /* switch 2 */
            D_801206AC = 0xA;
            break;
        case 21:                                    /* switch 2 */
            var_t5 = 7;
            goto block_35;
        }
    }
    if (D_800E7384 == 0x70000000) {
        D_801206AC = 3;
    }
    temp_v1 = D_800E7384 - 0x70000000;
    switch (temp_v1) {                              /* switch 3; irregular */
    case 0x3B:                                      /* switch 3 */
        D_801206AC = 2;
        break;
    case 0x49:                                      /* switch 3 */
block_47:
        D_801206AC = 0;
        break;
    case 0x66:                                      /* switch 3 */
        D_801206AC = 1;
        break;
    case 0xA1:                                      /* switch 3 */
        if (D_800E62BE == 0x60) {
            goto block_47;
        }
        break;
    }
    if (D_800E7384 == 0x95) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1D;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (((u32) D_800E7384 >= 0xD1U) && ((u32) D_800E7384 < 0x70000000U)) {
        temp_lo_2 = (u32) ((u32) (D_800E7384 - 0xC9) % 78U) / 39U;
        switch (temp_lo_2) {                        /* switch 4; irregular */
        case 0:                                     /* switch 4 */
            D_801206F0 = 0x11;
            D_80120734 = 0x15;
            D_80120778 = 0x19;
            D_801207BC = 0x1D;
            if (D_80122EAC & 1) {
                D_80120734 = 0x21;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x25;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x29;
            }
            break;
        case 1:                                     /* switch 4 */
            D_801206F0 = 0x10;
            D_80120734 = 0x14;
            D_80120778 = 0x18;
            D_801207BC = 0x1C;
            if (D_80122EAC & 1) {
                D_80120734 = 0x20;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x24;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x28;
            }
            break;
        }
    }
    if (D_800E7384 == 0x70000000) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1C;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (temp_v1 == 0x3B) {
        if (D_800E62BE != 0x60) {
            D_801206F0 = 0xF;
            D_80120734 = 0x13;
            D_80120778 = 0x17;
            D_801207BC = 0x1B;
            if (D_80122EAC & 1) {
                D_80120734 = 0x1F;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x23;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x27;
            }
        }
    }
    if ((temp_v1 == 0x68) && (D_800E62BE == 0x60)) {
        D_801206F0 = 0x12;
        D_80120734 = 0x16;
        D_80120778 = 0x1A;
        D_801207BC = 0x1E;
        if (D_80122EAC & 1) {
            D_80120734 = 0x22;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x26;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x2A;
        }
    }
}
