s32 func_80134850(void) {
    s16 var_t8;
    u32 temp_lo;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_t0;
    u8 var_v1;

    if (D_80122EAC == 3) {
        var_v1 = D_800E738C;
        D_80120762 = 0x40;
        temp_lo = var_v1 / 14U;
        var_t8 = temp_lo + 0xA2;
        if (temp_lo >= 2U) {
            var_v1 = 0;
            var_t8 = 0xA2;
        }
        D_80120778 = var_t8;
        D_800E738C = var_v1 + 1;
    }
    var_v0 = ((DisplayActor44 *)D_8011ECD0);
    do {
        var_v0 += 0x44;
        *(u8 *)((unsigned char *)var_v0 + (6467)) = (u8) D_800E738B;
    } while ((u32) var_v0 < (u32) &D_8011EE24);
    temp_t0 = (D_800E738B - 2) & 0xFF;
    D_800E738B = temp_t0;
    if (temp_t0 == 0) {
        var_v0_2 = D_80120650;
        do {
            var_v0_2 += 0x110;
            *(s8 *)((unsigned char *)var_v0_2 + (-197)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-129)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-265)) = 0;
        } while (var_v0_2 != D_80121750);
        D_800E738B = 0;
        func_8004284C();
    }
}
