void func_80132834(void) {
    s16 r;
    /* FAKE: inherited sibling capacity; exact original list capacity unknown. T-24523. */
    s16 candidates[13];
    s16 rank_scores[3];
    s16 i;
    s16 count;

    D_80134520 = 0;
    D_80134548 = 0;
    D_8013453C = 0;
    D_80134544 = 0;
    count = 0;
    if (((CharFlags *)&D_800E6280.unk_1BC[9].unk_0C)->b1 == 0 &&
        D_800E6280.unk_1BC[9].unk_02 >= 20) {
        D_80134544 = 1;
        D_800E6280.unk_1BC[9].unk_06 = 50;
    }
    D_80134540[0] = D_80134540[1] = D_80134540[2] = 0xFF;
    rank_scores[0] = rank_scores[1] = rank_scores[2] = 0;
    for (i = 0; i < 11; i++) {
        if (((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b1 != 0 &&
            get_h_tokimeki(i) >= 0x46U) {
            if (rank_scores[0] < D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = rank_scores[0];
                rank_scores[0] = D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = D_80134540[0];
                D_80134540[0] = i;
            } else if (rank_scores[1] < D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = i;
            } else if (rank_scores[2] < D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06) {
                rank_scores[2] = D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06;
                D_80134540[2] = i;
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if ((((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b1 != 0 ||
             (i == 9 && D_80134544 == 1)) &&
            (get_h_yuukou(i) >= 0x32U || (D_80134544 == 1 && i == 9))) {
            candidates[count] = i;
            D_8013453C += 1;
            count += 1;
            ((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b8 = 1;
        }
    }
    if (D_8013453C > 0) {
        r = (u32)D_800E6280.unk_10F8 % (u32)D_8013453C;
        for (i = 0; i < D_8013453C; i++) {
            D_80134524[i] = candidates[(i + r) % D_8013453C];
        }
        func_8013387C();
    }
    for (i = 0; i < D_8013453C; i++) {
        if (D_80134524[i] == 0 && D_800E6280.unk_1BC[0].unk_02 >= 70) {
            D_80134524[i] = D_80134524[D_8013453C - 1];
            D_80134524[D_8013453C - 1] = 0;
        } else if (D_80134524[i] == 0 && D_80134544 != 0) {
            D_80134524[i] = D_80134524[D_8013453C - 1];
            D_80134524[D_8013453C - 1] = 0;
        } else if (D_80134524[i] == 0) {
            D_80134524[i] = D_80134524[0];
            D_80134524[0] = 0;
        }
        if (D_80134524[i] == 9 && D_80134544 != 0) {
            D_80134524[i] = D_80134524[0];
            D_80134524[0] = 9;
        }
    }
    func_8004284C();
}
