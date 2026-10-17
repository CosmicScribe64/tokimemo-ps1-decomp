void func_8013B488(void) {
    u32 t;
    s16 sp3A;
    u8 sp39;
    u8 x;
    GameState *g = &D_800E6280;

    sp39 = func_80051A68(g->unk_F5F) & 0x7F;
    if ((*(u8 *)&g->unk_110D) >= 0xBU) {
        (*(u8 *)&g->unk_110D) = 0xA;
    }
    if ((*(u8 *)&g->unk_110D)++ == 0) {
        t = sp39;
        D_800CA158 = ((t >= 2U) + (t >= 3U) + (t >= 4U)) * 3;
        if (t == 4) {
            func_80083440(4);
            D_800CA234[D_80122CDC] = 0;
        }
    }
    sp3A = D_800CA234[D_80122CDC] + D_800CA158;
    if (g->unk_75C != 0) {
        sp3A = 0xA;
    }
    if ((g->unk_75C != 0) && (sp39 < 2U) && (g->unk_F5F == 6)) {
        sp3A = 0x12;
    }
    if (D_80122D08 >= 0) {
        if (D_80122D08 == g->unk_72C[g->unk_F5F]) {
            if ((g->unk_73C[g->unk_F5F] != 0) && (g->unk_1108 == 0x50)) {
                if ((u8) D_8015B680 == 0) {
                    if (g->unk_75C == 0) {
                        if (D_800CA2F8 != 0) {
                            if (D_800CA2F8 != 0x12) {
                                sp3A = 0xB;
                                if ((*(u8 *)&g->unk_110D) == 1) {
                                    func_80083440(3);
                                    g->unk_1BC[g->unk_F5F].unk_02 -= 1;
                                    g->unk_1BC[g->unk_F5F].unk_06 -= 2;
                                    g->unk_1BC[g->unk_F5F].unk_0A += 5;
                                    func_80084D3C();
                                }
                                if (sp39 < 2U) {
                                    if (g->unk_F5F == 6) {
                                        sp3A = 0x13;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    D_800CA134 = (u8 *) &sp3A;
    D_800CA138 = &D_800CA15C;
    t = func_80051A68(g->unk_F5F) & 0x7F;
    if (((g->unk_F5F == 1) || (g->unk_F5F == 5)) && (t < 2U) && (*(s16 *)&D_800CA15C == 3)) {
        func_80083440(1);
    }
    D_800CA13C = D_8015BE18;
    D_800CA140 = D_8015BE84;
    D_800CA144 = D_8015BEF0;
    func_80082764(D_80122CDC, 1, 0);
}
