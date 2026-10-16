void csr_load_vram(s32 arg0) {
    u8 *var_s0;
    u32 var_s1;
    u8 temp_v0;

    var_s1 = 0;
    if (D_800E6280.unk_1429 != 0) {
        var_s0 = (u8 *)&D_800E6280;
        do {
            temp_v0 = *(u8 *)(var_s0 + 0x1430);
            if (temp_v0 == 0) {
                LoadSquare(*(u16 *)(var_s0 + 0x1434), *(u16 *)(var_s0 + 0x1436), *(u16 *)(var_s0 + 0x1438), *(u16 *)(var_s0 + 0x143A), (void *) *(s32 *)(var_s0 + 0x142C));
            } else {
                LoadSquare((((temp_v0 & 0xF) << 6) + *(u16 *)(var_s0 + 0x1434)) & 0xFFFF, (((temp_v0 & 0x10) * 0x10) + *(u16 *)(var_s0 + 0x1436)) & 0xFFFF, ((u16) *(u16 *)(var_s0 + 0x1438) >> *(u8 *)(var_s0 + 0x1431)) & 0xFFFF, *(u16 *)(var_s0 + 0x143A), (void *) *(s32 *)(var_s0 + 0x142C));
            }
            var_s1 += 1;
            var_s0 += 0x10;
        } while (var_s1 < (u8) D_800E6280.unk_1429);
    }
    D_800E6280.unk_1429 = 0;
}
