extern u16 D_80125CC2;

u16 func_8007B64C(void) {
    u16 *temp_v0;
    u16 var_v0;
    u32 temp_t8;
    u32 var_a0;

    var_v0 = D_80125CC0;
    if (var_v0 >= 0x21U) {
        var_v0 = 0x20;
    }
    if (var_v0 == 0) {
        D_80125CC0 = var_v0;
        return 0U;
    }
    var_a0 = 1;
    if (var_v0 >= 2U) {
        D_80125CC0 = var_v0;
        do {
            temp_v0 = &(&D_80125CC0)[var_a0];
            temp_v0->unk0 = *(u16 *)((unsigned char *)temp_v0 + (2));
            temp_t8 = (var_a0 + 1) & 0xFFFF;
            var_a0 = temp_t8;
        } while (temp_t8 < (u16) D_80125CC0);
        var_v0 = D_80125CC0;
    }
    D_80125CC0 = var_v0 - 1;
    return D_80125CC2;
}
