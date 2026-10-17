void func_80143F24(void) {
    GameState *g = &D_800E6280;
    s32 i;

    func_80142AE8();
    if (g->unk_03A != 0) {
        for (i = 0; i < 3; i++) {
            ((u8 *)&D_80121818)[i * 0x24 + 0x14] = ((u8 *)&D_80121818)[i * 0x24 + 0x15] = ((u8 *)&D_80121818)[i * 0x24 + 0x16] = g->unk_03A;
        }
        g->unk_03A -= 1;
    }
    D_80150138 -= 0xC8;
    if (D_80150138 < -0x7530) {
        func_80059048();
        if (g->unk_F5F >= 0xD) {
            main_state_set(0x12);
        } else {
            main_state_set(0xC3);
        }
    }
}
