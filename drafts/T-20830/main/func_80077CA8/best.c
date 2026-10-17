u8 func_80077CA8(void) {
    GameState *g = &D_800E6280;
    s32 x;

    x = g->unk_03E;
    switch (x) {
    default:
        return x;
    case 0x5F:
        if (g->unk_03F < 5U) {
            return 0;
        }
        if (g->unk_03F < 8U) {
            return 1;
        }
        if (g->unk_03F < 0xBU) {
            return 2;
        }
        return 3;
    case 0x60:
        if (g->unk_03F < 2U) {
            return 3;
        }
        if (g->unk_03F < 5U) {
            return 4;
        }
        if (g->unk_03F < 8U) {
            return 5;
        }
        if (g->unk_03F < 0xBU) {
            return 6;
        }
        return 7;
    case 0x61:
        if (g->unk_03F == 8) {
            return 0xD;
        }
        if (g->unk_03F < 2U) {
            return 7;
        }
        if (g->unk_03F < 5U) {
            return 8;
        }
        if (g->unk_03F < 8U) {
            return 9;
        }
        if (g->unk_03F < 0xBU) {
            return 0xA;
        }
        return 0xB;
    case 0x62:
        if (g->unk_03F < 2U) {
            return 0xB;
        }
        return 0xC;
    }
}
