void func_80139290(void) {
    s16 var_t5;
    s32 temp_v1;
    u32 temp_lo;
    u32 temp_lo_2;
    u8 var_t0;

    switch (D_800E6280.unk_1104.w) {                /* switch 1; irregular */
    case 0x1D:                                      /* switch 1 */
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 1;
        break;
    case 0x58:                                      /* switch 1 */
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0;
        break;
    case 0x66:                                      /* switch 1 */
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 2;
        break;
    case 0x83:                                      /* switch 1 */
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
        break;
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xBFU) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo = (u32) ((u32) (D_800E6280.unk_1104.w - 0xBE) % 242U) / 11U;
        switch (temp_lo) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            var_t5 = 4;
block_35:
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = var_t5;
            break;
        case 1:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 2:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 5;
            break;
        case 3:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 6;
            break;
        case 4:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 5:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xC;
            break;
        case 6:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xB;
            break;
        case 7:                                     /* switch 2 */
            var_t5 = 0xE;
            goto block_35;
        case 8:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 9:                                     /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 8;
            break;
        case 10:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 7;
            break;
        case 11:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 9;
            break;
        case 12:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 13:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 5;
            break;
        case 14:                                    /* switch 2 */
            var_t5 = 6;
            goto block_35;
        case 15:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 16:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xD;
            break;
        case 17:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xB;
            break;
        case 18:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 4;
            break;
        case 19:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
            break;
        case 20:                                    /* switch 2 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xA;
            break;
        case 21:                                    /* switch 2 */
            var_t5 = 7;
            goto block_35;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 3;
    }
    temp_v1 = D_800E6280.unk_1104.w - 0x70000000;
    switch (temp_v1) {                              /* switch 3; irregular */
    default:                                        /* switch 3 */
        var_t0 = D_800E6280.unk_03E;
        break;
    case 0x3B:                                      /* switch 3 */
        var_t0 = D_800E6280.unk_03E;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 2;
        break;
    case 0x49:                                      /* switch 3 */
        var_t0 = D_800E6280.unk_03E;
block_47:
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0;
        break;
    case 0x66:                                      /* switch 3 */
        var_t0 = D_800E6280.unk_03E;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 1;
        break;
    case 0xA1:                                      /* switch 3 */
        var_t0 = D_800E6280.unk0;
        if (var_t0 == 0x60) {
            goto block_47;
        }
        break;
    }
    if (D_800E6280.unk_1104.w == 0x95) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x14;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x18;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1D;
        if (D_80122EAC & 1) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x20;
        }
        if (D_80122EAC & 2) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x24;
        }
        if (D_80122EAC & 4) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x28;
        }
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xD1U) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo_2 = (u32) ((u32) (D_800E6280.unk_1104.w - 0xC9) % 78U) / 39U;
        switch (temp_lo_2) {                        /* switch 4; irregular */
        case 0:                                     /* switch 4 */
            D_8011ECD0->unk0 = 0x11;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x15;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x19;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1D;
            if (D_80122EAC & 1) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x21;
            }
            if (D_80122EAC & 2) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x25;
            }
            if (D_80122EAC & 4) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x29;
            }
            break;
        case 1:                                     /* switch 4 */
            *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x10;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x14;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x18;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1C;
            if (D_80122EAC & 1) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x20;
            }
            if (D_80122EAC & 2) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x24;
            }
            if (D_80122EAC & 4) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x28;
            }
            break;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x14;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x18;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1C;
        if (D_80122EAC & 1) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x20;
        }
        if (D_80122EAC & 2) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x24;
        }
        if (D_80122EAC & 4) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x28;
        }
    }
    if (temp_v1 == 0x3B) {
        if (var_t0 != 0x60) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0xF;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x13;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x17;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1B;
            if (D_80122EAC & 1) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x1F;
            }
            if (D_80122EAC & 2) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x23;
            }
            if (D_80122EAC & 4) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x27;
            }
        }
    }
    if ((temp_v1 == 0x68) && (var_t0 == 0x60)) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x12;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x16;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x1A;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x1E;
        if (D_80122EAC & 1) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0x22;
        }
        if (D_80122EAC & 2) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6824)) = 0x26;
        }
        if (D_80122EAC & 4) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6892)) = 0x2A;
        }
    }
}
