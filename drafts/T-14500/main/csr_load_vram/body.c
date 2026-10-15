void csr_load_vram(s32 arg0) {
    GameState *base;
    s32 var_s1;
    s32 temp_v0;

    base = &D_800E6280;
    var_s1 = 0;
    if (base->unk_1429 != 0) {
        do {
            temp_v0 = base->unk_142C[var_s1].unk_04;
            if (temp_v0 == 0) {
                LoadSquare(base->unk_142C[var_s1].unk_08, base->unk_142C[var_s1].unk_0A, base->unk_142C[var_s1].unk_0C, base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            } else {
                LoadSquare((((temp_v0 & 0xF) << 6) + base->unk_142C[var_s1].unk_08), (((temp_v0 & 0x10) * 0x10) + base->unk_142C[var_s1].unk_0A), ((u16) base->unk_142C[var_s1].unk_0C >> base->unk_142C[var_s1].unk_05), base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            }
            var_s1 += 1;
        } while (var_s1 < (u8) base->unk_1429);
    }
    base->unk_1429 = 0U;
}
