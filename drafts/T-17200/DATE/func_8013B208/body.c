void func_8013B208(void) {
    GameState *g;
    s16 sp2A;
    s16 sp28;

    g = &D_800E6280;

    /* FAKE: discarded entry assignment suppresses compare CVT rewriting; otherwise the first
     * branch operands are exchanged (same fix as the EVENT twin func_80117D80, T-14300). T-17200 */
    sp2A = D_800CA2FC;
    D_800CA134 = (u8 *) &sp2A;
    D_800CA13C = D_8015BD98;
    D_800CA140 = D_8015BDA8;
    sp2A = 0;
    D_800CA144 = D_8015BDB8;
    if ((D_80122CDC == 0) && (g->unk_10C.unk_02 < 0x28) && (D_800CA2FC == 1) && (D_800CA2F8 == 0x10) &&
        ((g->unk_F5F == 0) || (g->unk_F5F == 1) || (g->unk_F5F == 3) ||
         (g->unk_F5F == 4) || (g->unk_F5F == 5) || (g->unk_F5F == 8) ||
         (g->unk_F5F == 9))) {
        D_800CA22C[0] = 3;
        D_800CA21C[0] = 3;
        D_800CA224[0] = 3;
        D_800CA234[0] = 2;
        func_80083440(3);
        sp28 = 5;
        D_800CA138 = (u8 *) &sp28;
        if (func_80082764(D_80122CDC, 0, 0) != 0) {
            func_8004284C();
        }
    } else if (((D_80122CDC == 0) && (g->unk_10C.unk_02 < 0x28) && (D_800CA2FC == 0) && (D_800CA2F8 == 0x13) &&
                ((g->unk_F5F == 0) || (g->unk_F5F == 1) || (g->unk_F5F == 3) ||
                 (g->unk_F5F == 6) || (g->unk_F5F == 7))) ||
               ((D_80122CDC == 0) && (g->unk_10C.unk_02 < 0x64) && (D_800CA2FC == 0) && (D_800CA2F8 == 0x14) &&
                ((g->unk_F5F == 0) || (g->unk_F5F == 4) || (g->unk_F5F == 6) ||
                 (g->unk_F5F == 7) || (g->unk_F5F == 8) || (g->unk_F5F == 0xA)))) {
        D_800CA22C[0] = 3;
        D_800CA21C[0] = 3;
        D_800CA224[0] = 3;
        D_800CA234[0] = 2;
        func_80083440(3);
        sp28 = 6;
        D_800CA138 = (u8 *) &sp28;
        if (func_80082764(D_80122CDC, 0, 0) != 0) {
            func_8004284C();
        }
    } else {
        func_8004284C();
    }
}
