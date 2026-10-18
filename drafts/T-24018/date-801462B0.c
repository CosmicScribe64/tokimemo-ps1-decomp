u16 func_801462B0(void) {
    GameState *g;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 sp50;
    u32 sp4C;
    Rec24 *p;
    DateMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_800AE120(D_800E6280.unk_10F8);
        func_80044750(0x206);
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
        D_8015CFA0 = 2;
        D_800E6280.unk_110D = 0;
        D_8015CF9C = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in func_801348F0) T-24018 */
            D_801217D0[i + 4].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 4].unk_00 = 0x82000000;
            D_801217D0[i + 4].unk_0A = 0;
            D_801217D0[i + 4].unk_0E = 0;
            D_801217D0[i + 4].unk_0F = 0xA0;
            D_801217D0[i + 4].unk_10 = 0;
            D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            D_801217D0[i + 4].unk_12 = 0x1E0;
            D_801217D0[i + 4].unk_14 = 0;
            D_801217D0[i + 4].unk_15 = 0;
            D_801217D0[i + 4].unk_16 = 0;
            D_801217D0[i + 4].unk_18 = 0;
            D_801217D0[i + 4].unk_1A = 0;
            D_801217D0[i + 4].unk_1C = 0x1000;
            D_801217D0[i + 4].unk_1E = 0x1000;
            D_801217D0[i + 4].unk_20 = 0;
            D_801217D0[i + 4].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 4].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 6].unk_00 = 0x82000000;
            D_801217D0[i + 6].unk_0A = 0xA0;
            D_801217D0[i + 6].unk_0E = 0;
            D_801217D0[i + 6].unk_0F = 0;
            D_801217D0[i + 6].unk_10 = 0;
            D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            D_801217D0[i + 6].unk_12 = 0x1E0;
            D_801217D0[i + 6].unk_14 = 0;
            D_801217D0[i + 6].unk_15 = 0;
            D_801217D0[i + 6].unk_16 = 0;
            D_801217D0[i + 6].unk_18 = 0;
            D_801217D0[i + 6].unk_1A = 0;
            D_801217D0[i + 6].unk_1C = 0x1000;
            D_801217D0[i + 6].unk_1E = 0x1000;
            D_801217D0[i + 6].unk_20 = 0;
            D_801217D0[i + 6].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 6] = 0xB;
        }
        func_8005742C(0x43CB, 1);
        g->unk_1104.u += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x80183000);
            g->unk_1104.u += 1;
        }
        break;
    case 2:
        func_8005742C(0x43D4, 1);
        g->unk_1104.u += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801A1000);
            g->unk_1104.u += 1;
        }
        break;
    case 4:
        func_8005742C(0x43DE, 1);
        g->unk_1104.u += 1;
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801BF000);
            g->unk_1104.u += 1;
        }
        break;
    case 6:
        func_8005742C(0x43E7, 1);
        g->unk_1104.u += 1;
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801DD000);
            g->unk_1104.u += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            D_801217D0[i + 4].unk_00 = 0x02000000;
        }
        g->unk_1104.u += 1;
        func_80044750(0x500);
        break;
    case 9:
        value = D_801217D0[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        if (value == 0x80) {
            g->unk_1104.u += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = D_801217D0[4].unk_14;
        value -= 2;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                D_801217D0[i + 4].unk_00 = 0x82000000;
            }
            g->unk_1104.u += 1;
        }
        break;
    case 12:
        sp50 = D_800E6280.unk_110A;
        sp4C = g->unk_1104.u;
        read_bustup(D_800E6280.unk_110A);
        g->unk_1104.u = sp4C;
        if (sp50 != D_800E6280.unk_110A) {
            g->unk_1104.u = sp4C + 1;
            D_800E6280.unk_110A -= 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
        }
        break;
    case 14:
        sp50 = D_800E6280.unk_110A;
        sp4C = g->unk_1104.u;
        func_80083474();
        if (sp50 + 1 != D_800E6280.unk_110A) {
            D_800E6280.unk_110A = sp50;
            g->unk_1104.u = 12;
            break;
        }
        g->unk_1104.u = sp4C + 1;
        D_800E6280.unk_110A -= 1;
        break;
    case 15:
        func_800462C8(0xA, 0x80162000, 0x4359);
        g->unk_1104.u += 1;
        break;
    case 17:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        g->unk_1104.u += 1;
        break;
    case 18:
        func_80044750(0x200);
        sp4C = g->unk_1104.u;
        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp4C + 1;
        break;
    case 20:
        sp4C = g->unk_1104.u;
        func_8013E980();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp4C + 1;
        break;
    case 22:
        sp4C = g->unk_1104.u;
        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp4C + 1;
        break;
    default:
        func_8006492C(1);
        D_80122D2C = 0;
        func_8004284C();
        break;
    }
    if (g->unk_1104.u < 9 || g->unk_1104.u >= 12) {
        return 0;
    }
    p = D_801217D0;
    v = p[4].unk_0E;
    v += D_8015CFA0;
    if (v < 0) {
        v = 0;
        D_8015CFA0 = (func_800AE0D0() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_8015CFA0 = -2 - (func_800AE0D0() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].unk_0A += 0x14;
        p[5].unk_0A += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].unk_0A -= 0x14;
        p[4].unk_06 += 0x14;
        p[5].unk_0A -= 0x14;
        p[5].unk_06 += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].unk_0A += 0x14;
        p[7].unk_0A += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].unk_0A -= 0x14;
        p[6].unk_06 += 0x14;
        p[7].unk_0A -= 0x14;
        p[7].unk_06 += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (g->unk_110D == 1 || g->unk_110D == 3)) || (p[6].unk_0F == 0 && (g->unk_110D == 0 || g->unk_110D == 2))) {
        switch (D_800E6280.unk_110D) { /* FAKE: g-> in the test, the symbol here; one access path keeps the byte in $v0 and drops the reload T-24018 */
        case 0:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 4].unk_0A = 0;
                D_801217D0[i + 4].unk_0F = 0xA0;
                D_801217D0[i + 4].unk_06 = -0x78;
                D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_8004435C(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 6].unk_0A = 0;
                D_801217D0[i + 6].unk_0F = 0xA0;
                D_801217D0[i + 6].unk_06 = -0x78;
                D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_8004435C(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 4].unk_0A = 0;
                D_801217D0[i + 4].unk_0F = 0xA0;
                D_801217D0[i + 4].unk_06 = -0x78;
                D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_8004435C(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 6].unk_0A = 0;
                D_801217D0[i + 6].unk_0F = 0xA0;
                D_801217D0[i + 6].unk_06 = -0x78;
                D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_8004435C(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++D_800E6280.unk_110D >= 4) {
            D_800E6280.unk_110D = 0;
        }
    }
    timer = (D_8015CF9C + 1) & 0xFFFF;
    D_8015CF9C = timer;
    if (timer == 0x12C) {
        g->unk_1104.u += 1;
    }
}