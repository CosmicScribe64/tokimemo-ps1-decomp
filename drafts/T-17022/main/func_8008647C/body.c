s32 func_8008647C(void) {
    s32 cur;
    s32 list[16];
    u32 n;
    s32 i;
    s32 best;
    s32 bscore;
    s32 score;
    u32 t;

    n = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1) {
            list[n] = i;
            n = (n + 1) & 0xFF;
        }
    }
    t = get_h_yuukou(0) & 0xFF;
    bscore = get_h_tokimeki(0) + t;
    best = 0;
    for (i = 1; i < n; i++) {
        t = get_h_tokimeki(list[i]) & 0xFF;
        score = get_h_yuukou(list[i]) + t;
        if (bscore < score && (t = func_8008667C(best) & 0xFF, t >= func_8008667C(list[i]))) {
            best = list[i];
            bscore = score;
        } else if (bscore == score) {
            t = func_8008667C(best) & 0xFF;
            if (t >= func_8008667C(list[i])) {
                cur = list[i];
                if (D_800E6280.unk_1BC[best].unk_06 + D_800E6280.unk_1BC[best].unk_02 < D_800E6280.unk_1BC[cur].unk_06 + D_800E6280.unk_1BC[cur].unk_02) {
                    bscore = score;
                    best = cur;
                }
            }
        }
    }
    return best;
}
