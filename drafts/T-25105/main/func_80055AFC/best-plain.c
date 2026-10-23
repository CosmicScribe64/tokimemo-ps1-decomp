/* T-25105 draft, 114 rows without the volatile view; see NOTES.md. */
s32 func_80055AFC(s32 arg0) {
    u32 i;
    u8 *bank;

    if (func_80055A38(0) == -1) {
        return -1;
    }
    bank = &D_80123120[(D_800E6280.unk_1117 & 1) * 0xF00];
    arg0 = D_800E6280.unk_1117 & 1;
    if (((SaveGameView *)(bank + 0x200))->unk_005 == 0) {
        return -1;
    }
    if (((SaveGameView *)(bank + 0x200))->unk_006 == 0) {
        return -1;
    }
    D_800E6280.unk_03D = ((SaveGameView *)(bank + 0x200))->unk_004;
    D_800E6280.unk_03E = ((SaveGameView *)(bank + 0x200))->unk_005;
    D_800E6280.unk_03F = ((SaveGameView *)(bank + 0x200))->unk_006;
    D_800E6280.unk_040 = ((SaveGameView *)(bank + 0x200))->unk_007;
    D_800E6280.unk_041 = ((SaveGameView *)(bank + 0x200))->unk_008;
    D_800E6280.unk_042 = ((SaveGameView *)(bank + 0x200))->unk_00A;
    for (i = 0; i < 2; i++) {
        D_800E6280.unk_044[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_00C[i];
    }
    *(GsBlockC4 *)D_800E6280.unk_0C4 = ((SaveGameView *)(bank + 0x200))->unk_08C;
    for (i = 0; i < 16; i++) {
        D_800E6280.unk_1BC[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_108[i];
    }
    for (i = 0; i < 16; i++) {
        D_800E6280.unk_53C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_488[i];
    }
    for (i = 0; i < 8; i++) {
        D_800E6280.unk_54C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_498[i];
    }
    for (i = 0; i < 12; i++) {
        ((GsWord *)D_800E6280.unk_66C)[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_4B8[i];
    }
    *(GsBlock56C *)D_800E6280.unk_56C = ((SaveGameView *)(bank + 0x200))->unk_4E8;
    k_speed_set(D_800E6280.unk_668[3]);
    D_800B6708 = D_800E6280.unk_668[2];
    for (i = 0; i < 4; i++) {
        D_800E6280.unk_728[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5E8[i];
    }
    for (i = 0; i < 16; i++) {
        D_800E6280.unk_72C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5EC[i];
    }
    for (i = 0; i < 16; i++) {
        D_800E6280.unk_73C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5FC[i];
    }
    for (i = 0; i < 16; i++) {
        D_800E6280.unk_74C[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_60C[i];
    }
    bzero(D_800E6280.unk_75E, 0x800);
    for (i = 0; i < 256; i++) {
        D_800E6280.unk_75E[i] = ((SaveGameView *)&D_80123120[0x200])[arg0].unk_61C[i];
    }
    D_800E6280.unk_F5E = func_8006C988(&D_800E6280.unk_F5F, &D_800E6280.unk_75D, 1);
    D_800E6280.unk_F6C = ((SaveGameView *)(bank + 0x200))->unk_E1C;
    D_800E6280.unk_F6D = ((SaveGameView *)(bank + 0x200))->unk_E1D;
    D_800E6280.unk_1092 = ((SaveGameView *)(bank + 0x200))->unk_E1E;
    D_800E6280.unk_F6E = ((SaveGameView *)(bank + 0x200))->unk_E1F;
    D_800E6280.unk_03C = ((SaveGameView *)(bank + 0x200))->unk_E20;
    D_800E6280.unk_F68 = ((SaveGameView *)(bank + 0x200))->unk_E24;
    D_800E6280.unk_F6F = ((SaveGameView *)(bank + 0x200))->unk_E28;
    D_800E6280.unk_F70 = ((SaveGameView *)(bank + 0x200))->unk_E29;
    D_800E6280.unk_F73 = ((SaveGameView *)(bank + 0x200))->unk_E2A;
    D_800E6280.unk_F75 = ((SaveGameView *)(bank + 0x200))->unk_E2B;
    *(GsWord *)D_800E6280.unk_F64 = ((SaveGameView *)(bank + 0x200))->unk_E2C;
    return 1;
}
