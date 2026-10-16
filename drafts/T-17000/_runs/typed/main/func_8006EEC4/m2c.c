s32 func_8006EEC4(u8 *arg0, u8 *arg1) {
    u32 temp_t4;

    if (rand() & 7) {
        goto block_48;
    }
    *arg0 = 0;
    *arg1 = 0;
    if (!(D_800E6280.unk0 & 2) && !(D_800E6280.unk_F68.w & 0xF) && (D_800E6280.unk_100.unk_02 >= 0x4B)) {
        *arg0 = 1;
        *arg1 = D_800E6280.unk_10F8 & 1;
        return 1;
    }
    if (!(D_800E6280.unk0 & 2) && ((D_800E6280.unk_F68.w & 0xF) == 1) && (D_800E6280.unk_104.unk_02 >= 0x55)) {
        *arg0 = 2;
        *arg1 = D_800E6280.unk_10F8 & 3;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[3].unk_0C.w & 2) && ((D_800E6280.unk_F68.w & 0xF) == 2) && (D_800E6280.unk_108.unk_02 >= 0x41)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[5].unk_0C.w & 2) && ((D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 8) && (D_800E6280.unk_10C.unk_02 >= 0x54) && (D_800E6280.unk_114.unk_02 >= 0x54)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[6].unk_0C.w & 2) && ((D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9) && (D_800E6280.unk_10C.unk_02 >= 0x47) && (D_800E6280.unk_118.unk_02 >= 0x47)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[7].unk_0C.w & 2) && ((D_800E6280.unk_F68.w & 0xF) == 6) && (D_800E6280.unk_114.unk_02 >= 0x7E)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[8].unk_0C.w & 2)) {
        temp_t4 = (u32) (D_800E6280.unk_1BC[8].unk_0C.w << 0x14) >> 0x1D;
        if ((temp_t4 == 0) && (D_800E6280.unk_110.unk_02 >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((temp_t4 == 1) && (D_800E6280.unk_110.unk_02 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((temp_t4 == 2) && (D_800E6280.unk_110.unk_02 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((temp_t4 == 3) && ((D_800E6280.unk_F68.w & 0xF) == 5) && (D_800E6280.unk_110.unk_02 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
        goto block_44;
    }
block_44:
    if (!(D_800E6280.unk0 & 2) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) && (D_800E6280.unk_118.unk_02 >= 0x60)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
block_48:
    return 0;
}
