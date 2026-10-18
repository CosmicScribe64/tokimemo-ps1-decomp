s32 func_8014718C(void) {
    /* FAKE: unused RECT reserves the observed frame words. T-17201. */
    RECT rect;
    GameState *g;
    u32 sp30;
    u32 timer;
    s16 var_v1;
    u8 x;
    /* FAKE: second fade copy holds v1 so the stored sum uses a1. T-24028. */
    u32 y;
    DateMovieWork *movie;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        func_80044750(0x201);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_8006492C(0);
        /* FAKE: one source line keeps the as1 store order of the original. T-17201. */
        D_8015CF9C = 0; for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 10].unk_08 = (var_v1 << 6) + 0x80;
            D_801217D0[var_v1 + 10].unk_00 = 0x82000000;
            D_801217D0[var_v1 + 10].unk_0A = 0xa0;
            D_801217D0[var_v1 + 10].unk_0E = 0;
            D_801217D0[var_v1 + 10].unk_0F = 0;
            D_801217D0[var_v1 + 10].unk_10 = 0;
            D_801217D0[var_v1 + 10].unk_0C = (var_v1 * 2) + 0x15;
            D_801217D0[var_v1 + 10].unk_12 = 0x1E0;
            D_801217D0[var_v1 + 10].unk_14 = 0;
            D_801217D0[var_v1 + 10].unk_15 = 0;
            D_801217D0[var_v1 + 10].unk_16 = 0;
            D_801217D0[var_v1 + 10].unk_18 = 0;
            D_801217D0[var_v1 + 10].unk_1A = 0;
            D_801217D0[var_v1 + 10].unk_1C = 0x1000;
            D_801217D0[var_v1 + 10].unk_1E = 0x1000;
            D_801217D0[var_v1 + 10].unk_20 = 0;
            D_801217D0[var_v1 + 10].unk_04 = (var_v1 << 7) - 0xA0;
            D_801217D0[var_v1 + 10].unk_06 = -0x96;
            D_800E6280.unk_10A5[var_v1 + 10] = 0xC;
        }
        func_8005742C(0x43C2, 1);
        g->unk_1104.u += 1;
        return 0;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            g->unk_1104.u += 1;
        }
        return 0;
    case 2:
        cd_read_sectors(0x31, 0x801AE000, 0xBCD1);
        func_801461B0();
        g->unk_1104.u += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
        }
        return 0;
    case 4:
        load_palette(D_8015CF80, 0x11, 1, 0, 0);
        func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98);
        g->unk_1104.u += 1;
        return 0;
    case 5:
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 &= 0x7FFFFFFF;
        }
        g->unk_1104.u += 1;
        return 0;
    case 6:
        x = (D_801217D0[4].unk_14 += 1); y = D_801217D0[4].unk_15 = x; D_801217D0[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            D_801217D0[var_v1 + 4].unk_14 = D_801217D0[var_v1 + 4].unk_15 = D_801217D0[var_v1 + 4].unk_16 = D_801217D0[4].unk_14;
        }
        if (D_801217D0[4].unk_14 == 0x40) {
            g->unk_1104.u += 1;
        }
        return 0;
    case 7:
        x = (D_801217D0[4].unk_14 += 1); y = D_801217D0[4].unk_15 = x; D_801217D0[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            D_801217D0[var_v1 + 4].unk_14 = D_801217D0[var_v1 + 4].unk_15 = D_801217D0[var_v1 + 4].unk_16 = D_801217D0[4].unk_14;
        }
        if (D_801217D0[4].unk_14 == 0x80) {
            g->unk_1104.u += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (D_801217D0[4].unk_14 -= 2); y = D_801217D0[4].unk_15 = x; D_801217D0[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            D_801217D0[var_v1 + 4].unk_14 = D_801217D0[var_v1 + 4].unk_15 = D_801217D0[var_v1 + 4].unk_16 = D_801217D0[4].unk_14;
        }
        if (D_801217D0[4].unk_14 == 0) {
            for (var_v1 = 0; var_v1 < 8; var_v1++) {
                D_801217D0[var_v1 + 4].unk_00 |= 0x80000000;
            }
            g->unk_1104.u += 1;
        }
        break;
    case 10:
        func_800462C8(0xA, 0x80162000, 0x4350);
        g->unk_1104.u += 1;
        return 0;
    case 12:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        func_800573F8(1);
        g->unk_1104.u += 1;
        return 0;
    case 13:
        sp30 = g->unk_1104.u;

        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp30;
        g->unk_1104.u++;
        return 0;
    case 15:
        sp30 = g->unk_1104.u;

        func_8013E980();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp30;
        g->unk_1104.u++;
        return 0;
    case 17:
        sp30 = g->unk_1104.u;

        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp30;
        g->unk_1104.u++;
        return 0;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        return 0;
    }
    timer = (D_8015CF9C + 1) & 0xFFFF;
    if (timer % 19U == 0x12) {
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 10].unk_06 += 1;
        }
        D_801217D0[var_v1 + 10].unk_0A -= 1;
    }
    if (timer % 5U == 4) {
        for (var_v1 = 0; var_v1 < 6; var_v1++) {
            D_801217D0[var_v1 + 4].unk_06 += 1;
        }
        for (var_v1 = 3; var_v1 < 6; var_v1++) {
            if (D_801217D0[var_v1 + 4].unk_0A != 0) {
                D_801217D0[var_v1 + 4].unk_0A -= 1;
            } else {
                D_801217D0[var_v1 + 1].unk_0A -= 1;
            }
        }
    }
    D_8015CF9C = timer;
    if (timer == 0x12C) {
        g->unk_1104.u += 1;
    }
}