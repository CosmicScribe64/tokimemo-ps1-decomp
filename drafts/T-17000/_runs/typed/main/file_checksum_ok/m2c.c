s32 file_checksum_ok(s32 arg0, u8 *arg1) {
    s32 temp_t0;
    s32 var_v0;
    u32 temp_a2;
    u32 var_v1;
    u8 *var_a0;
    u8 *var_a3;
    void *temp_a0;

    temp_a2 = ((arg0 & 0xFF) << 0xB) - 4;
    var_v0 = 0;
    var_v1 = 0;
    if (temp_a2 != 0) {
        var_a0 = arg1;
        var_a3 = arg1 + 1;
        do {
            var_v1 += 2;
            temp_t0 = (var_v0 + *var_a0 + *var_a3) & 0xFFFF;
            var_a0 += 2;
            var_a3 += 2;
            var_v0 = temp_t0;
        } while (var_v1 < temp_a2);
    }
    temp_a0 = var_v1 + arg1;
    if (var_v0 != ((temp_a0->unk0 << 8) + *(u8 *)((unsigned char *)temp_a0 + (1)))) {
        return 0;
    }
    return 1;
}
