void func_80139290(void) {
    s16 var_t5;
    s32 temp_v1;
    u32 temp_lo;
    u32 temp_lo_2;
    u8 var_t0;

    switch (D_800E6280.unk_1104.w) {
    case 0x1D:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 1;
        break;
    case 0x58:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
        break;
    case 0x66:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 2;
        break;
    case 0x83:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
        break;
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xBFU) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo = (u32) ((u32) (D_800E6280.unk_1104.w - 0xBE) % 242U) / 11U;
        switch (temp_lo) {
        case 0:
            var_t5 = 4;
block_35:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = var_t5;
            break;
        case 1:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 2:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 5;
            break;
        case 3:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 6;
            break;
        case 4:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 5:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xC;
            break;
        case 6:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xB;
            break;
        case 7:
            var_t5 = 0xE;
            goto block_35;
        case 8:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 9:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 8;
            break;
        case 10:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 7;
            break;
        case 11:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 9;
            break;
        case 12:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 13:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 5;
            break;
        case 14:
            var_t5 = 6;
            goto block_35;
        case 15:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 16:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xD;
            break;
        case 17:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xB;
            break;
        case 18:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 4;
            break;
        case 19:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 20:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xA;
            break;
        case 21:
            var_t5 = 7;
            goto block_35;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
    }
    temp_v1 = D_800E6280.unk_1104.w - 0x70000000;
    switch (temp_v1) {
    default:
        var_t0 = D_800E6280.unk_03E;
        break;
    case 0x3B:
        var_t0 = D_800E6280.unk_03E;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 2;
        break;
    case 0x49:
        var_t0 = D_800E6280.unk_03E;
block_47:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
        break;
    case 0x66:
        var_t0 = D_800E6280.unk_03E;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 1;
        break;
    case 0xA1:
        var_t0 = D_800E6280.unk0;
        if (var_t0 == 0x60) {
            goto block_47;
        }
        break;
    }
    if (D_800E6280.unk_1104.w == 0x95) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1D;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
        }
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xD1U) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo_2 = (u32) ((u32) (D_800E6280.unk_1104.w - 0xC9) % 78U) / 39U;
        switch (temp_lo_2) {
        case 0:
            ((DisplayActor44 *)D_8011ECD0)->unk0 = 0x11;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x15;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x19;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1D;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x21;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x25;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x29;
            }
            break;
        case 1:
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1C;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
            }
            break;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1C;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
        }
    }
    if (temp_v1 == 0x3B) {
        if (var_t0 != 0x60) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0xF;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x13;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x17;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1B;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x1F;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x23;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x27;
            }
        }
    }
    if ((temp_v1 == 0x68) && (var_t0 == 0x60)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x12;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x16;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x1A;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1E;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x22;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x26;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x2A;
        }
    }
}
