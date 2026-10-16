u32 menu_set(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3, s16 *arg4, s16 *arg5) {
    s16 *var_a1;
    s16 *var_a3;
    s16 *var_t0;
    s16 *var_t1;
    s16 temp_t9;
    s32 temp_v1;
    s32 var_v0_2;
    u32 var_v0;
    u8 temp_s0;
    void *var_t2;
    void *var_t2_2;

    temp_v1 = arg0 * 8;
    temp_s0 = arg1 & 0xFF;
    var_t2 = temp_v1 + &D_800E6280;
    var_v0_2 = 0;
    do {
        var_v0_2 += 1;
        var_t2 += 0x10;
        *(s8 *)((unsigned char *)var_t2 + (3975)) = 0;
        *(s8 *)((unsigned char *)var_t2 + (3973)) = 0;
        *(s8 *)((unsigned char *)var_t2 + (3972)) = 0;
    } while (var_v0_2 < 0x10);
    var_a3 = arg3;
    var_t0 = arg4;
    var_t1 = arg5;
    var_a1 = arg2;
    if (temp_s0 >= 0x11U) {
        return -1U;
    }
    *(u8 *)((unsigned char *)(arg0 + &D_800E6280) + (4240)) = temp_s0;
    D_800E6280.unk_1095[arg0] = -1;
    if (temp_v1 >= 0x10) {
        return -1U;
    }
    var_v0 = 0;
    if (temp_s0 != 0) {
        var_t2_2 = temp_v1 + &D_800E6280;
        do {
            var_v0 += 1;
            *(s16 *)((unsigned char *)var_t2_2 + (3984)) = (s16) *var_a1;
            *(s16 *)((unsigned char *)var_t2_2 + (3986)) = (s16) *var_a3;
            var_a1 += 2;
            var_a3 += 2;
            if ((*var_t1 * *var_t0) == 0) {
                *(s8 *)((unsigned char *)var_t2_2 + (3991)) = 1;
            } else {
                *(s8 *)((unsigned char *)var_t2_2 + (3991)) = 0;
            }
            var_t2_2 += 0x10;
            *(s8 *)((unsigned char *)var_t2_2 + (3972)) = (s8) *var_t0;
            temp_t9 = *var_t1;
            var_t0 += 2;
            var_t1 += 2;
            *(s8 *)((unsigned char *)var_t2_2 + (3974)) = 0;
            *(s8 *)((unsigned char *)var_t2_2 + (3973)) = (s8) temp_t9;
        } while (var_v0 < temp_s0);
    }
    return var_v0;
}
