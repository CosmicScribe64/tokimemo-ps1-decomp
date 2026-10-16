s8 menu_check_1(s32 arg0, s16 arg1, s16 arg2) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 var_v1;
    u32 var_a3;
    u32 var_t0;
    void *temp_t1;
    void *var_t2;

    temp_t1 = &D_800E6280 + arg0;
    var_v1 = 0;
    var_a3 = -1U;
    var_t0 = 0;
    if (*(u8 *)((unsigned char *)temp_t1 + (4240)) != 0) {
        var_t2 = (arg0 * 8) + &D_800E6280;
        do {
            temp_a0 = arg1 - *(s16 *)((unsigned char *)var_t2 + (3984));
            *(s8 *)((unsigned char *)var_t2 + (3990)) = 0;
            if (((temp_a0 - *(u8 *)((unsigned char *)var_t2 + (3988))) * temp_a0) < 0) {
                temp_a0_2 = arg2 - *(s16 *)((unsigned char *)var_t2 + (3986));
                if (((temp_a0_2 - *(u8 *)((unsigned char *)var_t2 + (3989))) * temp_a0_2) < 0) {
                    *(s8 *)((unsigned char *)var_t2 + (3990)) = 1;
                    var_v1 += 1;
                    var_a3 = var_t0;
                }
            }
            var_t0 += 1;
            var_t2 += 0x10;
        } while (var_t0 < (u8) *(u8 *)((unsigned char *)(arg0 + &D_800E6280) + (4240)));
    }
    if (var_v1 == 0) {
        *(s8 *)((unsigned char *)temp_t1 + (4243)) = -1;
    }
    if (!(D_800E6280 & 4) && (var_a3 != -1U)) {
        D_800E6280.unk_1095[arg0] = (s8) var_a3;
    }
    return (s8) var_a3;
}
