s32 func_8006EB64(u8 *arg0, u8 *arg1) {
    u32 temp_t4;

    *arg0 = 0;
    *arg1 = 0;
    if ((rand() % 3) != 0) {
        goto block_48;
    }
    if (!(D_800E6480 & 2) && !(D_800E71E8 & 0xF) && (D_800E6382 >= 0x37)) {
        *arg0 = 1;
        *arg1 = D_800E7378 & 1;
        return 1;
    }
    if (!(D_800E64B8 & 2) && ((D_800E71E8 & 0xF) == 1) && (D_800E6386 >= 0x3C)) {
        *arg0 = 2;
        *arg1 = D_800E7378 & 3;
        return 1;
    }
    if (!(D_800E64F0 & 2) && ((D_800E71E8 & 0xF) == 2) && (D_800E638A >= 0x32)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6560 & 2) && ((D_800E71E8 & 0xF) == 3) && (((u8) D_800E644A >> 4) != 8) && (D_800E638E >= 0x51) && (D_800E6396 >= 0x51)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6598 & 2) && ((D_800E71E8 & 0xF) == 3) && (((u8) D_800E644A >> 4) != 9) && (D_800E638E >= 0x1F) && (D_800E639A >= 0x1F)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E65D0 & 2) && ((D_800E71E8 & 0xF) == 6) && (D_800E6396 >= 0x65)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6608 & 2)) {
        temp_t4 = (u32) (D_800E6608 << 0x14) >> 0x1D;
        if ((temp_t4 == 0) && (D_800E6392 >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((temp_t4 == 1) && (D_800E6392 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((temp_t4 == 2) && (D_800E6392 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((temp_t4 == 3) && ((D_800E71E8 & 0xF) == 5) && (D_800E6392 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
        goto block_44;
    }
block_44:
    if (!(D_800E6528 & 2) && (((u16) D_800E6374 >> 0xC) != ((u8) D_800E652A >> 4)) && (D_800E639A >= 0x24)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
block_48:
    return 0;
}
