s32 parameter_up_down(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 div_a;
    s8 *row;
    s32 div_b;
    s8 *p;
    s32 v;
    u32 h;

    div_a = 1;
    div_b = 1;
    /* FAKE: T-25044, retain the raw byte lifetime for the head allocation. */
    if (D_800E6280.unk_0F4.b[1]) {}
    v = D_800E6280.unk_0F4.b[1] & 0xF;
    if (v == 3) {
        div_a = 2;
    } else if (((v == 1) || (v == 2)) && (arg0 != 5) && (arg0 != 7) && (arg0 != 6)) {
        div_b = 2;
    }
    if (arg1 != 0) {
        arg1 = 4;
    } else {
        arg1 = 1;
    }
    if (arg2 != 0) {
        arg2 = 2;
    } else {
        arg2 = 1;
    }
    if (arg0 == 4) {
        h = D_800E6280.unk_0F4.h;
        row = D_800B6928[h >> 12];
        p = row;
        for (i = 0; i != 8; i++) {
            v = *p;
            if (v > 0) {
                parameter_change(i, v * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, v << 12);
            }
            p++;
        }
        v = row[8];
        if (v < 0) {
            parameter_change(8, v * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (v << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    } else {
        row = D_800B68E0[arg0];
        p = row;
        for (i = 0; i != 8; i++) {
            v = *p;
            if (v > 0) {
                parameter_change(i, v * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, v << 12);
            }
            p++;
        }
        v = row[8];
        if (v < 0) {
            parameter_change(8, v * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (v << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    }
}
