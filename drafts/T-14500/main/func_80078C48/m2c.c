extern u8 D_800E6375;
extern u8 D_800E738E;

void func_80078C48(void) {
    s32 sp28;
    s32 var_a2;
    u8 temp_v0;

    sp28 = ovl_load_lba;
    back_clear_switch(1);
    ovl_load_buf = 0x80132000;
    D_800B6D30 = 0;
    temp_v0 = D_800E738E;
    if ((u8) D_800E738E >= 0x14U) {
        switch ((u8) D_800E738E) {                  /* switch 1; irregular */
        default:                                    /* switch 1 */
            if (temp_v0 != 0x35) {

            }
            break;
        case 0x54:                                  /* switch 1 */
        case 0x55:                                  /* switch 1 */
        case 0x59:                                  /* switch 1 */
        case 0x5A:                                  /* switch 1 */
        case 0x5B:                                  /* switch 1 */
        case 0x5D:                                  /* switch 1 */
        case 0x72:                                  /* switch 1 */
            func_80057640(0xA0);
            D_800B6D30 = 1;
            break;
        }
    } else if ((temp_v0 != 8) && (temp_v0 != 0x13)) {

    }
    if ((D_800E6375 & 0xF) == 3) {
        D_801217D0->unk_00 = 0x80000000;
    }
    switch (temp_v0) {                              /* switch 2; irregular */
    case 0x3A:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x96AA;
        break;
    case 0x82:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x934A;
        break;
    case 0x74:                                      /* switch 2 */
    case 0x75:                                      /* switch 2 */
    case 0x76:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x958A;
        break;
    case 0x81:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x982A;
        break;
    case 0x80:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x91CA;
        break;
    case 0x72:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x99AA;
        break;
    case 0x5F:                                      /* switch 2 */
    case 0x60:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x952A;
        break;
    case 0x73:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x97CA;
        break;
    case 0x70:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x916A;
        break;
    case 0x71:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x910A;
        break;
    case 0x11:                                      /* switch 2 */
    case 0x13:                                      /* switch 2 */
    case 0x14:                                      /* switch 2 */
    case 0x15:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x95EA;
        break;
    case 0x43:                                      /* switch 2 */
    case 0x45:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x9A0A;
        break;
    case 0x63:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x92EA;
        break;
    case 0x35:                                      /* switch 2 */
    case 0x36:                                      /* switch 2 */
    case 0x37:                                      /* switch 2 */
    case 0x91:                                      /* switch 2 */
    case 0xC4:                                      /* switch 2 */
        /* fallthrough */
    case 0xC2:                                      /* switch 2 */
    case 0xC3:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x93AA;
        break;
    case 0x50:                                      /* switch 2 */
    case 0x5E:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x922A;
        break;
    case 0x58:                                      /* switch 2 */
    case 0x5C:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x928A;
        break;
    case 0x61:                                      /* switch 2 */
    case 0x62:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x970A;
        break;
    case 0x56:                                      /* switch 2 */
    case 0x57:                                      /* switch 2 */
    case 0x59:                                      /* switch 2 */
    case 0x5A:                                      /* switch 2 */
    case 0x5D:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x946A;
        break;
    case 0x51:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x98EA;
        break;
    case 0x54:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x9ACA;
        break;
    case 0x52:                                      /* switch 2 */
    case 0x53:                                      /* switch 2 */
    case 0x55:                                      /* switch 2 */
    case 0x5B:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x988A;
        break;
    case 0x20:                                      /* switch 2 */
    case 0x21:                                      /* switch 2 */
    case 0x22:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x976A;
        break;
    case 0xD0:                                      /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x9A6A;
        break;
    default:                                        /* switch 2 */
        ovl_load_sectors = 0x60;
        var_a2 = 0x994A;
        break;
    }
    if (sp28 == var_a2) {
        ovl_load_lba = var_a2;
        ovl_load_state = 3;
        return;
    }
    ovl_load_lba = var_a2;
    cd_read_sectors(0x60U, ovl_load_buf, var_a2);
    ovl_load_state = 1;
}
