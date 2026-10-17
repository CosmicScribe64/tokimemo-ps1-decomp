s32 func_8013AF1C(void) {
    GameState *g = &D_800E6280;
    if (g->unk_110D++ == 0) {
        if ((g->unk_03F == 3) && (g->unk_040 == 0x15) && (g->unk_03E != 0x60)) {
            D_8015BF0C = 0;
        } else if ((g->unk_03F == 3) && (g->unk_040 == 0x14) && (g->unk_03E == 0x60)) {
            D_8015BF0C = 0;
        } else if ((g->unk_03E == 0x5F) && (g->unk_03F == 7) && (g->unk_040 == 0x17)) {
            D_8015BF0C = 1;
        } else if ((g->unk_03E == 0x60) && (g->unk_03F == 7) && (g->unk_040 == 0x1C)) {
            D_8015BF0C = 1;
        } else if ((g->unk_03E == 0x61) && (g->unk_03F == 7) && (g->unk_040 == 0x1B)) {
            D_8015BF0C = 1;
        } else if ((g->unk_03F == 9) && (g->unk_040 == 0x17) && (g->unk_03E != 0x60)) {
            if (g->unk_10F8 & 1) {
                D_8015BF0C = 2;
            } else {
                D_8015BF0C = 3;
            }
        } else if ((g->unk_03F == 9) && (g->unk_040 == 0x16) && (g->unk_03E == 0x60)) {
            if (g->unk_10F8 & 1) {
                D_8015BF0C = 2;
            } else {
                D_8015BF0C = 3;
            }
        } else if ((g->unk_03F == 0xC) && (g->unk_040 == 0x17)) {
            D_8015BF0C = 4;
        } else {
            func_8004284C();
            return 0;
        }
    }
    if (g->unk_110D >= 0xBU) {
        g->unk_110D = 0xA;
    }
    D_800CA134 = (u8 *) &D_8015BF0C;
    D_800CA138 = (u8 *) &D_8015BF10;
    D_800CA13C = D_8015BE20;
    D_800CA140 = D_8015BE8C;
    D_800CA144 = D_8015BEF8;
    return func_80082764(D_80122CDC, 1, 0);
}
