void func_80147F58(s32 arg0) {
    s16 sp64[8];
    s16 *temp_s1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s32 temp_s5;
    s32 temp_v1;
    s32 var_s3;
    s32 var_t2;
    s32 var_t2_2;
    s32 var_t7;
    u16 *var_s1;
    u16 *var_s1_2;
    u16 *var_s2;
    u8 *temp_s0;

    (*(s32 *)&sp64) = (s32) D_8014A668;
    *(s32 *)((unsigned char *)(&sp64) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A668) + (4));
    *(s32 *)((unsigned char *)(&sp64) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A668) + (8));
    *(s32 *)((unsigned char *)(&sp64) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A668) + (12));
    if (*(&D_8014A460 + (arg0 * 0x1C)) == 0) {
        var_s2 = D_8014A4C4;
        temp_s5 = arg0 * 0x13;
        temp_s0 = (arg0 * 0x44) + D_8011ECD0;
        var_s3 = 0;
        do {
            temp_s1 = (u8 *)sp64 + var_s3;
            if (*(s16 *)((unsigned char *)temp_s0 + (6552)) == (*var_s2 + temp_s5)) {
                temp_v1 = *temp_s1 + *(s16 *)((unsigned char *)temp_s0 + (6566));
                var_t2 = temp_v1 + 0x23;
                if ((temp_v1 + 0x46) < *(s16 *)((unsigned char *)temp_s0 + (6838))) {
                    func_80148194(arg0);
                    var_t2 = *temp_s1 + *(s16 *)((unsigned char *)temp_s0 + (6566)) + 0x23;
                }
                if (*(s16 *)((unsigned char *)temp_s0 + (6838)) < var_t2) {
                    func_80148228(arg0);
                }
            }
            var_s2 += 2;
            var_s3 += 2;
        } while ((u32) var_s2 < (u32) &D_8014A4D4);
        var_s1 = D_8014A4D8;
        do {
            if (*(s16 *)((unsigned char *)temp_s0 + (6552)) == (*var_s1 + temp_s5)) {
                temp_v1_2 = *(s16 *)((unsigned char *)temp_s0 + (6566));
                var_t7 = temp_v1_2 + 0x3B;
                if ((temp_v1_2 + 0x5E) < *(s16 *)((unsigned char *)temp_s0 + (6838))) {
                    func_80148194(arg0);
                    var_t7 = *(s16 *)((unsigned char *)temp_s0 + (6566)) + 0x3B;
                }
                if (*(s16 *)((unsigned char *)temp_s0 + (6838)) < var_t7) {
                    func_80148228(arg0);
                }
            }
            var_s1 += 2;
        } while ((u32) var_s1 < (u32) D_8014A4E4);
        var_s1_2 = &D_8014A4E8;
        do {
            if (*(s16 *)((unsigned char *)temp_s0 + (6552)) == (*var_s1_2 + temp_s5)) {
                temp_v1_3 = *(s16 *)((unsigned char *)temp_s0 + (6566));
                var_t2_2 = temp_v1_3 + 0x19;
                if ((temp_v1_3 + 0x3C) < *(s16 *)((unsigned char *)temp_s0 + (6838))) {
                    func_80148194(arg0);
                    var_t2_2 = *(s16 *)((unsigned char *)temp_s0 + (6566)) + 0x19;
                }
                if (*(s16 *)((unsigned char *)temp_s0 + (6838)) < var_t2_2) {
                    func_80148228(arg0);
                }
            }
            var_s1_2 += 2;
        } while (var_s1_2 != &D_8014A4F4);
    }
}
