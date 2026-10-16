s32 func_8013BD84(void) {
    s32 i;
    s32 best;
    s16 max;
    s16 v;

    best = 0;
    if ((get_h_yuukou(0) >= 0x32U) && (get_h_yuukou(0) >= 0x50U)) {
        max = D_800E6280.unk_1BC[0].unk_06 + D_800E6280.unk_1BC[0].unk_02;
    } else {
        max = 0;
    }
    if (D_800E6280.unk_03E == 0x60) {
        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 4) || (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 1)) {
            max = 0;
        }
    }
    for (i = 1; i < 11; i++) {
        if ((get_h_tokimeki(i) >= 0x32U) && (get_h_yuukou(i) >= 0x50U) && (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1)) {
            v = D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06;
            if (D_800E6280.unk_03E == 0x60) {
                if ((((u8) D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) == 4) || (((u8) D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) == 1)) {
                    v = 0;
                }
            }
            if (max < v) {
                best = i;
                max = v;
            }
        }
    }
    if ((get_h_tokimeki(best) >= 0x32U) && (get_h_yuukou(best) >= 0x50U)) {
        if ((best == 0) && (D_800E6280.unk_03E == 0x60)) {
            if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 4) || (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 1)) {
                return 0;
            }
        }
        if ((best == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) >= 6U)) {
            best = 11;
        }
        D_80122EBC = best;
        return 1;
    }
    return 0;
}
