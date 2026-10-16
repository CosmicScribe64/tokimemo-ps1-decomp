void func_80143968(void) {
    s32 *var_s1;
    s32 temp_a3;
    s32 temp_lo;
    s32 var_a3;
    s32 var_s0;
    u8 *temp_v0;
    u8 *temp_v0_2;

    temp_a3 = D_80150144;
    if (temp_a3 < 0x11D) {
        D_80150144 = temp_a3;
        func_8004B19C(temp_a3, 0x28, 0x28, temp_a3);
        var_a3 = D_80150144;
        var_s1 = &((DisplayActor44 *)D_8011ECD0);
        var_s0 = 0;
        do {
            *(s16 *)((unsigned char *)var_s1 + (6570)) = (s16) (*(s16 *)((unsigned char *)var_s1 + (6570)) - 1);
            if (*(s16 *)((unsigned char *)var_s1 + (6570)) < -0x8C) {
                temp_v0 = &D_801503C8 + (var_a3 * 2);
                D_80150144 = var_a3;
                func_8014387C(var_s0, (*(s32 *)temp_v0), *(u8 *)((unsigned char *)temp_v0 + (1)), var_a3);
                var_a3 = D_80150144 + 1;
            }
            temp_lo = (var_s0 + 0x60) * 0x44;
            var_s0 += 1;
            var_s1 += 17;
            temp_v0_2 = &(DisplayActor44 *)((u8 *)D_8011ECD0 + temp_lo);
            *(s16 *)((unsigned char *)temp_v0_2 + (26)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0_2 + (28)) = 0x1000;
        } while (var_s0 != 7);
        D_80150144 = var_a3;
    }
}
