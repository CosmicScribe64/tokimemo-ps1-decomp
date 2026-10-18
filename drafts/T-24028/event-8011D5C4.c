s32 func_8011D5C4(void) {
    /* FAKE: unused RECT reserves the observed frame words. T-24028. */
    RECT rect;
    EventMovieState *g;
    u32 sp30;
    u32 timer;
    s16 var_v1;
    u8 x;
    /* FAKE: second fade copy holds v1 so the stored sum uses a1. T-24028. */
    u32 y;
    EventMovieWork *movie;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;

    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_800433D0(0x203);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        D_80080710 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_800352B4(0);
        /* FAKE: one source line keeps the as1 store order of the original. T-24028. */
        D_801258DC = 0; for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0A = 0xa0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0C = (var_v1 * 2) + 0x15;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_06 = -0x96;
            D_800B0860[var_v1 + 0x1037] = 0xC;
        }
        func_8002746C(0x4332, 1);
        g->unk_00 += 1;
        return 0;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x140, 0xA0);
            g->unk_00 += 1;
        }
        return 0;
    case 2:
        func_800433D0(0x502);
        func_80015D28(0x31, 0x801AE000, 0xACA8);
        func_8011C650();
        g->unk_00 += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
        }
        return 0;
    case 4:
        func_80012D64(D_801258C0, 0x11, 1, 0, 0);
        func_8004C46C(D_801258C4, D_801258C8, D_801258CC, D_801258D0, D_801258D4, D_801258D8);
        g->unk_00 += 1;
        return 0;
    case 5:
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 &= 0x7FFFFFFF;
        }
        func_800433D0(0x503);
        g->unk_00 += 1;
        return 0;
    case 6:
        x = (((Rec24 *)D_800EC190)[4].unk_14 += 1); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x40) {
            g->unk_00 += 1;
        }
        return 0;
    case 7:
        x = (((Rec24 *)D_800EC190)[4].unk_14 += 1); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x80) {
            g->unk_00 += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (((Rec24 *)D_800EC190)[4].unk_14 -= 2); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0) {
            for (var_v1 = 0; var_v1 < 8; var_v1++) {
                ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 |= 0x80000000;
            }
            g->unk_00 += 1;
        }
        break;
    case 10:
        switch (*(u32 *)&D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        g->unk_00 += 1;
        return 0;
    case 12:
        func_8002755C(1);
        func_80027438(1);
        g->unk_00 += 1;
        return 0;
    case 13:
        sp30 = g->unk_00;

        func_8011A62C();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    case 15:
        sp30 = g->unk_00;

        func_8011A7FC();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    case 17:
        sp30 = g->unk_00;

        func_8011A838();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        return 0;
    }
    timer = (D_801258DC + 1) & 0xFFFF;
    if (timer % 19U == 0x12) {
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_06 += 1;
        }
        ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0A -= 1;
    }
    if (timer % 5U == 4) {
        for (var_v1 = 0; var_v1 < 6; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 += 1;
        }
        for (var_v1 = 3; var_v1 < 6; var_v1++) {
            if (((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A != 0) {
                ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A -= 1;
            } else {
                ((Rec24 *)D_800EC190)[var_v1 + 1].unk_0A -= 1;
            }
        }
    }
    D_801258DC = timer;
    if (timer == 0x12C) {
        g->unk_00 += 1;
    }
}