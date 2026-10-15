/* FAKE: s32 with normal bare returns preserves the original v0 liveness;
 * only the decoder-error path returns zero. Real return type unknown. T-12100. */
s32 func_80147958(void) {
    GameState *g;
    s32 current;
    s32 sp54;
    u32 sp50;
    RECT rect;
    DateMovieWork *movie;
    /* FAKE: unused view reserves the observed frame slot. T-12100. */
    DateMovieHeader8 *header;
    u8 value;
    u32 color;
    s16 var_v1;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        func_80044750(0x7F);
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
        D_8015CF9C = 0;
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_08 = (var_v1 << 6) + 0x80;
            D_801217D0[var_v1 + 4].unk_00 = 0x82000000;
            D_801217D0[var_v1 + 4].unk_0A = 0xa0;
            D_801217D0[var_v1 + 4].unk_0E = 0;
            D_801217D0[var_v1 + 4].unk_0F = 0x0;
            D_801217D0[var_v1 + 4].unk_10 = 0;
            D_801217D0[var_v1 + 4].unk_0C = (var_v1 * 2) + 0x15;
            D_801217D0[var_v1 + 4].unk_12 = 0x1E0;
            D_801217D0[var_v1 + 4].unk_14 = 0;
            D_801217D0[var_v1 + 4].unk_15 = 0;
            D_801217D0[var_v1 + 4].unk_16 = 0;
            D_801217D0[var_v1 + 4].unk_18 = 0;
            D_801217D0[var_v1 + 4].unk_1A = 0;
            D_801217D0[var_v1 + 4].unk_1C = 0x1000;
            D_801217D0[var_v1 + 4].unk_1E = 0x1000;
            D_801217D0[var_v1 + 4].unk_20 = 0;
            D_801217D0[var_v1 + 4].unk_04 = (var_v1 << 7) - 0xA0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
            D_800E6280.unk_10A5[var_v1 + 4] = 0xb;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_08 = (var_v1 << 6) + 0x80;
            D_801217D0[var_v1 + 6].unk_00 = 0x82000000;
            D_801217D0[var_v1 + 6].unk_0A = 0x0;
            D_801217D0[var_v1 + 6].unk_0E = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xa0;
            D_801217D0[var_v1 + 6].unk_10 = 0;
            D_801217D0[var_v1 + 6].unk_0C = (var_v1 * 2) + 0xa;
            D_801217D0[var_v1 + 6].unk_12 = 0x1E0;
            D_801217D0[var_v1 + 6].unk_14 = 0;
            D_801217D0[var_v1 + 6].unk_15 = 0;
            D_801217D0[var_v1 + 6].unk_16 = 0;
            D_801217D0[var_v1 + 6].unk_18 = 0;
            D_801217D0[var_v1 + 6].unk_1A = 0;
            D_801217D0[var_v1 + 6].unk_1C = 0x1000;
            D_801217D0[var_v1 + 6].unk_1E = 0x1000;
            D_801217D0[var_v1 + 6].unk_20 = 0;
            D_801217D0[var_v1 + 6].unk_04 = (var_v1 << 7) - 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
            D_800E6280.unk_10A5[var_v1 + 6] = 0xb;
        }
        func_8005742C(0x43B1, 1);
        g->unk_1104.u += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x80183000);
        g->unk_1104.u += 1;
        return;
    case 3:
        func_8005742C(0x43AA, 1);
        g->unk_1104.u += 1;
        return;
    case 5:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x8019C000);
        g->unk_1104.u += 1;
        return;
    case 6:
        func_8005742C(0x43A3, 1);
        g->unk_1104.u += 1;
        return;
    case 8:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801B5000);
        g->unk_1104.u += 1;
        return;
    case 9:
        func_8005742C(0x439C, 1);
        g->unk_1104.u += 1;
        return;
    case 11:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801CE000);
        g->unk_1104.u += 1;
        return;
    case 12:
        func_8005742C(0x4395, 1);
        g->unk_1104.u += 1;
        return;
    case 14:
        func_800536AC(0x140, 0, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 15:
        func_8005742C(0x437D, 1);
        g->unk_1104.u += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 18:
        func_8005742C(0x4385, 1);
        g->unk_1104.u += 1;
        return;
    case 20:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 21:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 & 0x7FFFFFFF);
        }
        g->unk_1104.u++;
        return;
    case 22:
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

        if (color == 0x80) {
            g->unk_1104.u++;
            return;
        }
        break;
    case 23:
        if (!(D_8015CF9C & 7)) {
            func_80148924(4);
                if (((u16) D_8015CF9C % 24U) == 0) {
                func_80044750(0x503);
                    }
        }
        if (++D_8015CF9C == 0x140) {
            D_8015CF9C = 0;
            D_800E6280.unk_1104.u += 1;
        }
        break;
    case 24:
        g->unk_1104.u++;
        return;
    case 25:
        func_800438F0(0);
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_1104.u += 1;
        return;
    case 26:
        func_8005742C(0x438D, 1);
        g->unk_1104.u += 1;
        return;
    case 28:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x280, 0);
        g->unk_1104.u += 1;
        func_800438F0(1);
        return;
    case 29:
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = 0x02000000;
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_00 = 0x02000000;
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        func_80044750(0x500);
        g->unk_1104.u += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_80148924(8);
        if (++D_8015CF9C == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 31:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 33:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 35:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 36:
        func_80148924(8);
        if ((u16) D_8015CF9C >= 8U) {
            value = D_801217D0[4].unk_14;
        value -= 4;
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
        }
        if (++D_8015CF9C == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 37:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 38:
        func_80148924(8);
        value = D_801217D0[4].unk_14;
        value -= 4;
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

        if (color < 4U) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 39:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_1104.u++;
        return;
    case 40:
        sp50 = g->unk_1104.u;

        sp54 = (s32) D_800E6280.unk_110A;
        /* FAKE: one source line preserves as1 callback load scheduling. T-12100. */
        func_80083B24(D_800E6280.unk_110A); g->unk_1104.u = sp50; if (sp54 != D_800E6280.unk_110A) { D_800E6280.unk_110A -= 1; g->unk_1104.u++; } break;
    case 42:
        sp54 = D_800E6280.unk_110A;
        sp50 = g->unk_1104.u;

        func_80083474();
        current = sp54 + 1;
        if (D_800E6280.unk_110A != current) {
            D_800E6280.unk_110A = sp54;
            g->unk_1104.u = 0x28;
            return;
        }
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 43:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        g->unk_1104.u += 1;
        return;
    case 45:
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
        return;
    case 46:
        sp50 = g->unk_1104.u;

        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 48:
        sp50 = g->unk_1104.u;

        func_8013E980();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 50:
        sp50 = g->unk_1104.u;

        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        break;
    }
    return;
}
