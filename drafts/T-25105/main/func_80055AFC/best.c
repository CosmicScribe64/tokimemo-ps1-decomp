/* T-25105 draft, 113 rows (funcdiff --resolve --version c); see NOTES.md. */
s32 func_80055AFC(s32 arg0) {
    /* FAKE: T-25105, a volatile view of D_800E6280.unk_1117 (the splat label of the slot byte). The volatile load
       stops IDO's copy propagation of arg0 into the bank base, as the original compiler does not propagate it. */
    extern volatile u8 D_800E7397;
    GameState *g;
    u32 i;

    if (func_80055A38(0) == -1) {
        return -1;
    }
    arg0 = D_800E7397 & 1;
    g = &D_800E6280;
    if (((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_005 == 0) {
        return -1;
    }
    if (((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_006 == 0) {
        return -1;
    }
    g->unk_03D = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_004;
    g->unk_03E = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_005;
    g->unk_03F = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_006;
    g->unk_040 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_007;
    g->unk_041 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_008;
    g->unk_042 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_00A;
    for (i = 0; i < 2; i++) {
        g->unk_044[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_00C[i];
    }
    *(GsBlockC4 *)g->unk_0C4 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_08C;
    for (i = 0; i < 16; i++) {
        g->unk_1BC[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_108[i];
    }
    for (i = 0; i < 16; i++) {
        g->unk_53C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_488[i];
    }
    for (i = 0; i < 8; i++) {
        g->unk_54C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_498[i];
    }
    for (i = 0; i < 12; i++) {
        ((GsWord *)g->unk_66C)[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_4B8[i];
    }
    *(GsBlock56C *)g->unk_56C = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_4E8;
    k_speed_set(g->unk_668[3]);
    D_800B6708 = g->unk_668[2];
    for (i = 0; i < 4; i++) {
        g->unk_728[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5E8[i];
    }
    for (i = 0; i < 16; i++) {
        g->unk_72C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5EC[i];
    }
    for (i = 0; i < 16; i++) {
        g->unk_73C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5FC[i];
    }
    for (i = 0; i < 16; i++) {
        g->unk_74C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_60C[i];
    }
    bzero(g->unk_75E, 0x800);
    for (i = 0; i < 256; i++) {
        g->unk_75E[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_61C[i];
    }
    g->unk_F5E = func_8006C988(&g->unk_F5F, &g->unk_75D, 1);
    g->unk_F6C = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1C;
    g->unk_F6D = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1D;
    g->unk_1092 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1E;
    g->unk_F6E = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1F;
    g->unk_03C = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E20;
    g->unk_F68 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E24;
    g->unk_F6F = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E28;
    g->unk_F70 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E29;
    g->unk_F73 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2A;
    g->unk_F75 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2B;
    *(GsWord *)g->unk_F64 = ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2C;
    return 1;
}
