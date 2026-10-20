void func_801480D0(void) {
    s32 values[14];
    s16 *score;
    u8 *spread;
    u8 *mean;
    s32 i;
    s32 j;
    s32 changed;
    s32 a;
    s32 first_random;
    s32 second_random;
    s32 offset;
    s32 angle;
    s32 sine;
    s32 minimum;
    s32 maximum;
    s32 selected;
    s32 last;

    func_800AE120(D_800E6280.unk_10F8);
    for (i = 0, score = D_80150668, spread = D_80150738, mean = D_80150728; i != 13; i++, score++, spread++, mean++) {
        first_random = func_800AE0D0();
        a = (((func_800AE0D0() & 1) * 2) - 1) * (first_random % (s32)*spread);
        second_random = func_800AE0D0();
        *score = ((*mean + (((func_800AE0D0() & 1) * 2 - 1) * (second_random % (s32)*spread))) * 5) + a;
        j = func_801464D4(i);
        if (j == 1) {
            *score += func_800AE0D0() % 50;
        } else if (j == 9) {
            *score += func_800AE0D0() % 250;
        }
        if (*score >= 501) {
            *score = 500;
        }
        func_800AE0B0("[%d:%d]\n", i, *score);
    }
    D_80150668[13] = (s16) ((EtcWork40 *)&D_801230D0)->unk_28[5];
    do {
        changed = 0;
        for (i = 0; i < 14; i++) {
            for (j = i + 1; j < 14; j++) {
                if (D_80150668[j] == D_80150668[i]) {
                    D_80150668[j]--;
                    changed++;
                }
            }
        }
    } while (changed != 0);
    for (i = 0; i < 14; i++) {
        values[i] = D_80150668[i];
    }
    for (i = 0; i < 14; i++) {
        selected = 0;
        maximum = values[0];
        for (j = 1; j < 14; j++) {
            if (maximum < values[j]) {
                maximum = values[j];
                selected = j;
            }
        }
        values[selected] = 0;
        D_80150684[i] = selected;
    }
    func_800AE0B0("順位計算\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_80150684[i]);
    }
    func_800AE0B0("順位作成\n ");
    offset = func_800AE0D0() & 3;
    for (i = 0; i < 14; i++) {
        angle = (D_80150668[i] << 12) / 1000;
        sine = func_800A0140(angle);
        D_801506BC[i] = (sine * 200) / 4096 + offset + 200;
        func_800AE0B0("[%d:%d %d %d] \n", i, angle, sine, D_801506BC[i]);
    }
    do {
        changed = 0;
        for (i = 0; i < 14; i++) {
            for (j = i + 1; j < 14; j++) {
                if (D_801506BC[j] == D_801506BC[i]) {
                    changed++;
                    if (D_80150668[j] < D_80150668[i]) {
                        D_801506BC[j]++;
                    } else {
                        D_801506BC[j]--;
                    }
                }
            }
        }
    } while (changed != 0);
    func_800AE0B0("順位ソート\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_801506BC[i]);
    }
    minimum = 1000;
    for (i = 0; i < 14; i++) {
        if (D_801506BC[i] < minimum) {
            minimum = D_801506BC[i];
        }
    }
    if (minimum >= 9) {
        a = func_800AE0D0() % (minimum - 8) + minimum - 8;
        for (i = 0; i < 14; i++) {
            D_801506BC[i] -= a;
        }
    } else if (minimum <= 0) {
        for (i = 0; i < 14; i++) {
            D_801506BC[i] += 1 - minimum;
        }
    }
    maximum = D_80150668[0];
    selected = 0;
    for (i = 1; i < 14; i++) {
        if (maximum < D_80150668[i]) {
            maximum = D_80150668[i];
            selected = i;
        }
    }
    if (maximum >= 466) {
        a = D_801506BC[selected] - 1;
        for (i = 0; i < 14; i++) {
            D_801506BC[i] -= a;
        }
    }
    func_800AE0B0("順位確定\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_801506BC[i]);
    }
    if (D_80150684[0] == 13 && D_80150684[1] == 0) {
        if (D_801506BC[0] < D_801506BC[13]) {
            a = D_801506BC[0];
            D_801506BC[0] = D_801506BC[13];
            D_801506BC[13] = a;
        }
    }
    last = D_801506BC[13];
    if (D_80150684[0] == 13 && last <= 0) {
        a = 1 - last;
        D_801506BC[13] = 1;
        last = 1;
        for (i = 0; i < 13; i++) {
            D_801506BC[i] += a;
        }
    }
    for (i = 0; i < 11; i++) {
        if (D_80150668[i] < D_80150668[13]) {
            if (D_800E6280.unk_56C[0xC8 + i] == 0) {
                D_800E6280.unk_1BC[i].unk_02 += 11;
                D_800E6280.unk_56C[0xC8 + i] = 1;
            } else {
                D_800E6280.unk_1BC[i].unk_02 += 2;
            }
        }
    }
    D_801506BC[13] = last;
    if (last == 1) {
        for (i = 0; i < 11; i++) {
            D_800E6280.unk_1BC[i].unk_02 += 2;
        }
    }
}
