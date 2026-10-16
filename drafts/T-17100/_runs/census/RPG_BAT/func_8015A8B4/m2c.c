
void func_8015A8B4(void) {
    s16 temp_v1;
    s32 temp_lo;
    s32 var_s1;
    s32 var_t7;
    u8 *var_s0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        temp_v1 = *(s16 *)((unsigned char *)var_s0 + (9286));
        if ((temp_v1 < -0xB4) || (var_t7 = 0x70 - temp_v1, ((*(s16 *)((unsigned char *)var_s0 + (9290)) < 0x8D) == 0))) {
            func_8013E810(var_s1 + 0x28, 7, 1, 0);
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
