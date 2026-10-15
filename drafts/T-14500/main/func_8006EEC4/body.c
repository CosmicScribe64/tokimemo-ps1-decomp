s32 func_8006EEC4(u8 *arg0, u8 *arg1) {
    u32 variant;
    /* FAKE: unsigned locals preserve the original comparison operand order.
     * Direct literals reverse the three bne operands. T-10880. */
    u32 one = 1;
    u32 two = 2;
    u32 three = 3;

    if (rand() & 7) {
        return 0;
    }
    *arg0 = 0;
    *arg1 = 0;
    if (!(D_800E6280.unk_1BC[1].unk_0C.f.b1) && !((u32)D_800E6280.unk_F68.w & 0xF) && (D_800E6280.unk_100.unk_02 >= 0x4B)) {
        *arg0 = 1;
        *arg1 = D_800E6280.unk_10F8 & 1;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[2].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 1) && (D_800E6280.unk_104.unk_02 >= 0x55)) {
        *arg0 = 2;
        *arg1 = D_800E6280.unk_10F8 & 3;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[3].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 2) && (D_800E6280.unk_108.unk_02 >= 0x41)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[5].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 8) && (D_800E6280.unk_10C.unk_02 >= 0x54) && (D_800E6280.unk_114.unk_02 >= 0x54)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[6].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 3) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9) && (D_800E6280.unk_10C.unk_02 >= 0x47) && (D_800E6280.unk_118.unk_02 >= 0x47)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[7].unk_0C.f.b1) && (((u32)D_800E6280.unk_F68.w & 0xF) == 6) && (D_800E6280.unk_114.unk_02 >= 0x7E)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6280.unk_1BC[8].unk_0C.f.b1)) {
        variant = D_800E6280.unk_1BC[8].unk_0C.f.f9;
        if ((variant == 0) && (D_800E6280.unk_110.unk_02 >= 0x24)) {
            *arg0 = 8;
            /* FAKE: zero-valued comparison preserves the variant live range
             * and the original delay-slot nop. T-10880. */
            *arg1 = (one == variant) * 0;
            return 1;
        }
        if ((one == variant) && (D_800E6280.unk_110.unk_02 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((two == variant) && (D_800E6280.unk_110.unk_02 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((three == variant) && (((u32)D_800E6280.unk_F68.w & 0xF) == 5) && (D_800E6280.unk_110.unk_02 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }
    if (!(D_800E6280.unk_1BC[4].unk_0C.f.b1) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) && (D_800E6280.unk_118.unk_02 >= 0x60)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
    return 0;
}
