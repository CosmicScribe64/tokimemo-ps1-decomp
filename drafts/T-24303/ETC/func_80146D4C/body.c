s32 func_80146D4C(void) {
    s32 grade;
    s32 score;
    EtcWork40 *w;
    s32 ret;
    s32 main;
    s32 sub;
    s32 bio;

    main = 0;
    sub = 0;
    switch (D_800E6280.unk_041) {
    case 0:
        main = D_800E6280.unk_100.unk_02;
        bio = func_8006C334(0, 0, 1, 0x10);
        break;
    case 1:
        main = D_800E6280.unk_104.unk_02;
        bio = func_8006C334(0, 0, 1, 0x10);
        break;
    case 2:
        main = D_800E6280.unk_104.unk_02 * 7 / 10;
        sub = D_800E6280.unk_110.unk_02 * 3 / 10;
        if (sub >= 0x27) {
            sub = 0x26;
        }
        bio = func_8006C334(0, 1, 1, 0x10);
        break;
    case 3:
        sub = D_800E6280.unk_100.unk_02 * 3 / 10;
        main = D_800E6280.unk_110.unk_02 * 7 / 10;
        bio = func_8006C334(0, 0, 1, 0x10);
        if (sub >= 0x27) {
            sub = 0x26;
        }
        break;
    case 4:
        main = D_800E6280.unk_108.unk_02 * 8 / 10;
        sub = D_800E6280.unk_110.unk_02 * 2 / 10;
        bio = func_8006C334(0, 1, 0, 0x10);
        if (sub >= 0x1A) {
            sub = 0x19;
        }
        break;
    default:
        func_80046500();
        break;
    }
    ret = main + sub + bio - 8;
    if (D_800E6280.unk_0F4.b[1] & 2) {
        ret /= 2;
    }
    func_800AE0B0("main %d sub %d bio %d ret %d ", main, sub, bio, ret);
    if (ret < 0) {
        ret = 1;
    }
    ret = func_80146400() + ret;
    sub = func_801464D4(0xF);
    ret = (func_800AE0D0() % 10) * sub + ret;
    w = (EtcWork40 *)&D_801230D0;
    if (ret < 0x80) {
        w->unk_28[D_800E6280.unk_041] = ret * 100 / 128;
    } else {
        w->unk_28[D_800E6280.unk_041] = 100 - (func_800AE0D0() & 3);
    }
    func_800AE0B0("今日の成績 %d\t", w->unk_28[D_800E6280.unk_041]);
    w->unk_28[5] += w->unk_28[D_800E6280.unk_041];
    func_800AE0B0("合計点 %d\n", w->unk_28[5]);
    score = w->unk_28[D_800E6280.unk_041];
    if (score < 0x28) {
        grade = 0;
    } else if (score < 0x3C) {
        grade = 1;
    } else if (score < 0x50) {
        grade = 2;
    } else if (score < 0x5F) {
        grade = 3;
    } else {
        grade = 4;
    }
    if (grade == 0) {
        D_801230F4 += 1;
    }
    w->unk_10[D_800E6280.unk_041] = grade;
    return grade;
}