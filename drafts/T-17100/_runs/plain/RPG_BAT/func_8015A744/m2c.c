void func_8015A744(void) {
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_s1;
    s32 var_t7;
    u8 *var_s0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        temp_v1 = *(s16 *)((unsigned char *)var_s0 + (9286));
        if ((temp_v1 < -0xB4) || (var_t7 = 0x70 - temp_v1, ((*(s16 *)((unsigned char *)var_s0 + (9290)) < 0x8D) == 0))) {
            if (func_8013E9A8(5) == 0) {
                func_8013F0F4(0x501, 2, 2);
                func_8013E97C(var_s1 + 0x28, 0xF0, 0x28);
                temp_a0 = func_8013E9A8(8) + 5;
                D_8015EC50 = temp_a0;
                temp_v0 = func_8013E9A8(temp_a0);
                *(s8 *)((unsigned char *)var_s0 + (9251)) = 0x94;
                temp_a0_2 = -D_8015EC50;
                *(s16 *)((unsigned char *)var_s0 + (9294)) = (s16) temp_a0_2;
                *(s16 *)((unsigned char *)var_s0 + (9298)) = (s16) temp_v0;
                D_8015EC50 = temp_a0_2;
                D_8015EC54 = temp_v0;
            } else {
                func_8013E810(var_s1 + 0x28, 7, 1, 0);
            }
            var_t7 = 0x70 - *(s16 *)((unsigned char *)var_s0 + (9286));
        }
        temp_lo = (s32) (var_t7 << 0xC) / 240;
        var_s1 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (9206)) = (s16) temp_lo;
        *(s16 *)((unsigned char *)var_s0 + (9208)) = (s16) temp_lo;
        D_8015EC68 = (s32) *(s16 *)((unsigned char *)var_s0 + (9286));
    } while (var_s1 != 8);
}
