/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_801480D0(void) {
    s32 values[14];
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

    func_800AE120(D_800E7378);
    for (i = 0; i < 13; i++) {
        first_random = func_800AE0D0();
        a = (((func_800AE0D0() & 1) * 2) - 1) * (first_random % D_80150738[i]);
        second_random = func_800AE0D0();
        D_80150668.value[i] = ((D_80150728[i] + (((func_800AE0D0() & 1) * 2 - 1) * (second_random % D_80150738[i]))) * 5) + a;
        j = func_801464D4(i);
        if (j == 1) {
            D_80150668.value[i] += func_800AE0D0() % 50;
        } else if (j == 9) {
            D_80150668.value[i] += func_800AE0D0() % 250;
        }
        if (D_80150668.value[i] >= 501) {
            D_80150668.value[i] = 500;
        }
        func_800AE0B0("[%d:%d]\n", i, D_80150668.value[i]);
    }
    D_80150668.value[13] = (s16) D_8012310C;
    do {
        changed = 0;
        for (i = 0; i < 14; i++) {
            for (j = i + 1; j < 14; j++) {
                if (D_80150668.value[j] == D_80150668.value[i]) {
                    D_80150668.value[j]--;
                    changed++;
                }
            }
        }
    } while (changed != 0);
    for (i = 0; i < 14; i++) {
        values[i] = D_80150668.value[i];
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
        D_80150668.order[i] = selected;
    }
    func_800AE0B0("順位計算\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_80150668.order[i]);
    }
    func_800AE0B0("順位作成\n ");
    offset = func_800AE0D0() & 3;
    for (i = 0; i < 14; i++) {
        angle = (D_80150668.value[i] << 12) / 1000;
        sine = func_800A0140(angle);
        D_80150668.rank[i] = (sine * 200) / 4096 + offset + 200;
        func_800AE0B0("[%d:%d %d %d] \n", i, angle, sine, D_80150668.rank[i]);
    }
    do {
        changed = 0;
        for (i = 0; i < 14; i++) {
            for (j = i + 1; j < 14; j++) {
                if (D_80150668.rank[j] == D_80150668.rank[i]) {
                    changed++;
                    if (D_80150668.value[j] < D_80150668.value[i]) {
                        D_80150668.rank[j]++;
                    } else {
                        D_80150668.rank[j]--;
                    }
                }
            }
        }
    } while (changed != 0);
    func_800AE0B0("順位\x83\\ート\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_80150668.rank[i]);
    }
    minimum = 1000;
    for (i = 0; i < 14; i++) {
        if (D_80150668.rank[i] < minimum) {
            minimum = D_80150668.rank[i];
        }
    }
    if (minimum >= 9) {
        a = func_800AE0D0() % (minimum - 8) + minimum - 8;
        for (i = 0; i < 14; i++) {
            D_80150668.rank[i] -= a;
        }
    } else if (minimum <= 0) {
        for (i = 0; i < 14; i++) {
            D_80150668.rank[i] += 1 - minimum;
        }
    }
    maximum = D_80150668.value[0];
    selected = 0;
    for (i = 1; i < 14; i++) {
        if (maximum < D_80150668.value[i]) {
            maximum = D_80150668.value[i];
            selected = i;
        }
    }
    if (maximum >= 466) {
        a = D_80150668.rank[selected] - 1;
        for (i = 0; i < 14; i++) {
            D_80150668.rank[i] -= a;
        }
    }
    func_800AE0B0("順位確定\n ");
    for (i = 0; i < 14; i++) {
        func_800AE0B0("[%d:%d] \n", i, D_80150668.rank[i]);
    }
    if (D_80150668.order[0] == 13 && D_80150668.order[1] == 0) {
        if (D_80150668.rank[0] < D_80150668.rank[13]) {
            a = D_80150668.rank[0];
            D_80150668.rank[0] = D_80150668.rank[13];
            D_80150668.rank[13] = a;
        }
    }
    last = D_80150668.rank[13];
    if (D_80150668.order[0] == 13 && last <= 0) {
        a = 1 - last;
        D_80150668.rank[13] = 1;
        last = 1;
        for (i = 0; i < 13; i++) {
            D_80150668.rank[i] += a;
        }
    }
    for (i = 0; i < 11; i++) {
        if (D_80150668.value[i] < D_80150668.value[13]) {
            if (D_800E6280.unk_56C[0xC8 + i] == 0) {
                D_800E6280.unk_1BC[i].unk_02 += 11;
                D_800E6280.unk_56C[0xC8 + i] = 1;
            } else {
                D_800E6280.unk_1BC[i].unk_02 += 2;
            }
        }
    }
    D_80150668.rank[13] = last;
    if (last == 1) {
        for (i = 0; i < 15; i++) {
            D_800E6280.unk_1BC[i].unk_02 += 2;
        }
    }
}
