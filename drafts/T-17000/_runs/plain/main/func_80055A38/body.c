s32 func_80055A38(s32 arg0) {
    s32 temp_t6;
    s32 var_a3;
    s32 var_t0;
    s32 var_v0;
    s8 var_v1;
    u8 *temp_a0;
    u8 *temp_a0_2;
    u8 *var_a0;
    u8 *var_a1;
    u8 *var_a2;
    u8 temp_t1;
    u8 temp_t1_2;
    u8 temp_t1_3;
    u8 temp_t1_4;

    var_a2 = D_80123324;
    var_a1 = D_80123120;
    var_v1 = 0;
loop_1:
    var_v0 = 0;
    var_a0 = var_a2;
    var_a3 = 0;
    var_t0 = 4;
    do {
        temp_t1 = (*(s32 *)var_a0);
        var_t0 += 4;
        if (temp_t1 != 0) {
            var_v0 += 1;
        }
        temp_t1_2 = *(u8 *)((unsigned char *)var_a0 + (1));
        temp_a0 = var_a0 + 1;
        if (temp_t1_2 != 0) {
            var_v0 += 1;
        }
        temp_t1_3 = *(u8 *)((unsigned char *)temp_a0 + (1));
        temp_a0_2 = temp_a0 + 1;
        if (temp_t1_3 != 0) {
            var_v0 += 1;
        }
        temp_t1_4 = *(u8 *)((unsigned char *)temp_a0_2 + (1));
        var_a3 = var_a3 + temp_t1 + temp_t1_2 + temp_t1_3 + temp_t1_4;
        if (temp_t1_4 != 0) {
            var_v0 += 1;
        }
        var_a0 = temp_a0_2 + 1 + 1;
    } while (var_t0 != 0xF00);
    temp_t6 = *(s32 *)((unsigned char *)var_a1 + (512));
    var_a1 += 0xF00;
    if ((var_a3 == temp_t6) && (var_v0 != 0)) {
        D_800E7397 = var_v1;
        return 1;
    }
    var_v1 += 1;
    var_a2 += 0xF00;
    if (var_v1 == 2) {
        return -1;
    }
    goto loop_1;
}
