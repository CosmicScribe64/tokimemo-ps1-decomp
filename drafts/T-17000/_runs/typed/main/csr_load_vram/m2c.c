void csr_load_vram(s32 arg0) {
    GameState *var_s0;
    u32 var_s1;
    u8 temp_v0;

    var_s1 = 0;
    if (D_800E6280.unk_1429 != 0) {
        var_s0 = &D_800E6280;
        do {
            temp_v0 = var_s0->unk_142C[0].unk_04;
            if (temp_v0 == 0) {
                LoadSquare(var_s0->unk_142C[0].unk_08, var_s0->unk_142C[0].unk_0A, var_s0->unk_142C[0].unk_0C, var_s0->unk_142C[0].unk_0E, (void *) var_s0->unk_142C[0].unk_00);
            } else {
                LoadSquare((((temp_v0 & 0xF) << 6) + var_s0->unk_142C[0].unk_08) & 0xFFFF, (((temp_v0 & 0x10) * 0x10) + var_s0->unk_142C[0].unk_0A) & 0xFFFF, ((u16) var_s0->unk_142C[0].unk_0C >> var_s0->unk_142C[0].unk_05) & 0xFFFF, var_s0->unk_142C[0].unk_0E, (void *) var_s0->unk_142C[0].unk_00);
            }
            var_s1 += 1;
            var_s0 += 0x10;
        } while (var_s1 < (u8) D_800E6280.unk_1429);
    }
    D_800E6280.unk_1429 = 0;
}
