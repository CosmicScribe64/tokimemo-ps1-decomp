s32 holiday_tel_exit(void) {
    GameState *g = &D_800E6280;
    s32 k;

    if (g->unk_110D != 11) {
        if ((g->unk_1104.w & 7) == 7) {
            if (g->unk_110D == 0) {
                func_80044750(0x504);
            } else if (g->unk_110D >= 4) {
                k = g->unk_110D - 4;
                if (g->unk_F5F == 13) {
                    func_80044750((u16)(D_800B3778[10][k] + 0x505));
                } else {
                    func_80044750((u16)(D_800B3778[g->unk_F5F][k] + 0x505));
                }
            }
            g->unk_110D++;
        }
    } else if (g->unk_F5F == 13) {
        g->unk_1093 = 0;
        k_sub_reset();
        restore_bgm();
        main_state_set(0x45);
        func_80042908(0);
    } else if (g->unk_F5F == 11) {
        k_sub_reset();
        g->unk_1093 = 12;
        D_801230D0 = 11;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    } else {
        k_sub_reset();
        g->unk_1093 = g->unk_F5F + 1;
        D_801230D0 = g->unk_F5F;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    }
}
