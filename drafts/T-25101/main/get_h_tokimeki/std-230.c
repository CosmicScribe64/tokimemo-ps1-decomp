u32 get_h_tokimeki(s32 arg0) {
    s32 t;
    s32 v;
    s32 i;
    u8 bits;
    u8 type;
    u32 l0;
    u32 l1;
    u32 l2;
    u32 l3;
    u32 l4;
    u32 l5;
    u32 l6;
    s32 flags[8];
    s8 *tbl;

    t = arg0 % 13;
    v = D_800E6280.unk_1BC[t].unk_02 * D_800B4100[t];
    v /= 100;
    bits = D_800B4110[t];
    for (i = 1; i < 8; i++) {
        if (bits & (1 << i)) {
            flags[i] = 1;
        } else {
            flags[i] = 0;
        }
    }
    type = D_800B4120[t];
    l0 = D_800B4198[0];
    l1 = D_800B4198[1];
    l2 = D_800B4198[2];
    l3 = D_800B4198[3];
    l4 = D_800B4198[4];
    l5 = D_800B4198[5];
    l6 = D_800B4198[6];
    for (i = 1; i < 7; i++) {
        if (flags[i]) {
            switch (type) {
            case 1:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0 + 50) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1 + 35) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2 + 20) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3 + 20) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4 + 5) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5 + 5) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            case 2:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            case 3:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0 + 50) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1 + 50) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2 + 50) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3 + 20) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4 + 20) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5 + 20) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6 + 20) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            case 4:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0 + 10) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1 + 10) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2 + 10) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3 + 10) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            case 5:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0 + 20) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1 + 20) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2 + 20) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3 + 20) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5 - 5) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            case 6:
                tbl = D_800B4130[t];
                if ((&D_800E6280.unk_0FC)[i].unk_02 > l0) {
                    v += tbl[0];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l1) {
                    v += tbl[1];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l2) {
                    v += tbl[2];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l3) {
                    v += tbl[3];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l4 - 5) {
                    v += tbl[4];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l5 - 5) {
                    v += tbl[5];
                } else if ((&D_800E6280.unk_0FC)[i].unk_02 > l6 - 5) {
                    v += tbl[6];
                } else {
                    v += tbl[7];
                }
                break;
            }
        }
    }
    if (v >= 0x65) {
        return 0x64;
    }
    if (v < 0) {
        return 0;
    }
    return v & 0xFF;
}
