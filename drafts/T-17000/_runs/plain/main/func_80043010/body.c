s32 func_80043010(void) {
    s32 temp_t5;
    u8 var_a0;
    u8 var_v0;

    var_v0 = D_800E71F4;
    if (var_v0 != 0xFF) {
        if (var_v0 != 0) {
            return (~D_800E8C32 << 0x18) + ((~D_800E8C33 & 0xFF) << 0x10) + ((~D_800E8BF2 & 0xFF) << 8) + (~D_800E8BF3 & 0xFF);
        }
        var_a0 = 0;
        temp_t5 = ~D_800E8BF3 & 0xFF;
        if (temp_t5 & 8) {
            var_a0 = 0x20;
        }
        if (temp_t5 & 4) {
            var_a0 |= 0x40;
        }
        var_v0 = var_a0;
        /* Duplicate return node #8. Try simplifying control flow for better match */
        return (s32) var_v0;
    }
    return (s32) var_v0;
}
