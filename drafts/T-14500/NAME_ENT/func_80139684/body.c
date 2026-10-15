void func_80139684(void) {
    u8 var_a0;

    switch (D_800E738D) {
    case 0:
        if ((u32) D_800E7384 >= 0x41U) {
            D_800E738D += 1;
            return;
        }
        return;
    case 1:
        if ((u8) D_800E62BA >= 4U) {
            var_a0 = (D_800E62BA - 4) & 0xFF;
        } else {
            D_800E738D += 1;
            var_a0 = 0;
        }
        D_800E62BA = var_a0;
        set_dec_bri(var_a0);
        return;
    case 2:
        main_state_set(0x15);
        text_set_advance(0xE);
        break;
    }
}
