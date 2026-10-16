void func_8014AC74(void) {
    s16 var_v0;
    s32 *var_s5;
    s32 temp_lo;
    s32 temp_s3;
    s32 var_s2;
    void *temp_s1;
    void *temp_s4;

    var_s5 = &D_8015EBAC;
    var_s2 = 0;
    do {
        if (*(s32 *)((unsigned char *)var_s5 + (200)) == 1) {
            temp_lo = var_s2 * 3;
            temp_s4 = (var_s2 * 8) + &D_8015EBAC;
            temp_s1 = D_8011ECD0 + (var_s2 * 0xCC);
            temp_s3 = temp_lo + 5;
            if (!(*(u8 *)((unsigned char *)temp_s1 + (6802)) & 1)) {
                func_8013E810(temp_lo + 4, 7, 1, 1);
            }
            var_v0 = *(s16 *)((unsigned char *)temp_s1 + (6960));
            if (var_v0 >= 0x10) {
                var_v0 = 0x1E - var_v0;
            }
            func_8013E97C(temp_s3, *(s32 *)((unsigned char *)temp_s4 + (220)), *(s32 *)((unsigned char *)temp_s4 + (224)) - (var_v0 * 8));
            if (!(*(u8 *)((unsigned char *)temp_s1 + (6938)) & 1)) {
                func_8013E810(temp_lo + 4, 0xFF, 1, 0);
                func_8013E810(temp_s3, 0xFF, 1, 0);
                func_8013E810(temp_lo + 6, 0xFF, 1, 0);
            }
        }
        var_s2 += 1;
        var_s5 += 4;
    } while (var_s2 != 4);
}
