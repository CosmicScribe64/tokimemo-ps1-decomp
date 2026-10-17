s32 func_8013D5FC(void) {
    GameState *g;
    u8 n;
    u32 m;

    g = &D_800E6280;

    if ((g->unk_F5F == 7) && (g->unk_56C[0x31] != 0) && (*(u8 *)&D_8015B68C == 1)) {
        func_80083440(4);
        D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 4;
        D_800CA234[0] = D_800CA234[1] = D_800CA234[2] = 2;
        D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 4;
        D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;
        D_800CA148 += 2;
        g->unk_110A += 0xD;
        return 0;
    } else {
        if ((g->unk_F5F == 8) && (*(u8 *)&D_8015B68C == 3) && (g->unk_1BC[8].unk_14[0x18] != 0)) {
            m = g->unk_110D;
            if (m++ == 0) {
                D_800CA14C = 2;
            }
            g->unk_110D = m;
        }
        n = g->unk_110D;
        if ((g->unk_F5F == 8) && (*(u8 *)&D_8015B68C == 4) && (g->unk_1BC[8].unk_14[0x19] != 0)) {
            if (n++ == 0) {
                D_800CA14C = 2;
            }
        }
        g->unk_110D = n;
        if (n >= 0xBU) {
            g->unk_110D = 0xA;
        }
        func_80085A60(g->unk_F5F, *(u8 *)&D_8015B68C, 8U);
        if ((g->unk_F5F == 8) && (*(u8 *)&D_8015B68C == 3) && (g->unk_1BC[8].unk_14[0x18] == 0) && (D_800CA14C == 3)) {
            func_80044750(0x74);
            func_8004E9F4(1);
            D_800CA148 += 1;
            D_800CA14C = 0;
            D_80122CE4 = 0;
            func_8004284C();
        }
        if ((g->unk_F5F == 8) && (*(u8 *)&D_8015B68C == 4) && (g->unk_1BC[8].unk_14[0x19] == 0) && (D_800CA14C == 3)) {
            func_80044750(0x74);
            func_8004E9F4(1);
            D_800CA148 += 1;
            D_800CA14C = 0;
            D_80122CE4 = 0;
            func_8004284C();
        }
    }
}
