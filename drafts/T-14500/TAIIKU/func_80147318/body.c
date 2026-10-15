void func_80147318(void) {
    s32 temp_a1;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s2;
    u8 *temp_v0_2;
    u8 *var_s1;
    u8 temp_v0;
    u8 temp_v1;
    u8 *temp_a0;

    var_s1 = D_8014A454;
    var_s0 = 0;
    var_s2 = 0;
    do {
        temp_v0 = *(u8 *)((unsigned char *)var_s1 + (40));
        switch (temp_v0) {
        case 0:
            temp_v0_2 = &D_8014A400[var_s2];
            if ((u32) *(u32 *)((unsigned char *)temp_v0_2 + (4)) >= 6U) {
                temp_v1 = (*(s32 *)temp_v0_2);
                temp_a0 = D_8011ECD0 + (var_s0 * 0x44);
                temp_a1 = (var_s0 * 0x13) + 0x13;
                if (temp_v1 >= 7U) {
                    (*(s32 *)temp_v0_2) = 2;
                } else {
                    (*(s32 *)temp_v0_2) = temp_v1 + 1;
                }
                *(u32 *)((unsigned char *)temp_v0_2 + (4)) = 0U;
                temp_v1_2 = *(s32 *)((unsigned char *)var_s1 + (36));
                if (temp_v1_2 > 0) {
                    *(s16 *)((unsigned char *)temp_a0 + (6620)) = (s16) (*(&D_8014A4D4 + ((*(s32 *)temp_v0_2) * 2)) + temp_a1);
                } else if (temp_v1_2 < 0) {
                    *(s16 *)((unsigned char *)temp_a0 + (6620)) = (s16) (D_8014A4E4[(*(s32 *)temp_v0_2)] + temp_a1);
                } else {
                    *(s16 *)((unsigned char *)temp_a0 + (6620)) = (s16) (D_8014A4C4[(*(s32 *)temp_v0_2)] + temp_a1);
                }
            }
            break;
        case 1:
            if ((u8) *(u8 *)((unsigned char *)var_s1 + (41)) < 0x32U) {
                *(s16 *)((unsigned char *)(D_8011ECD0 + (var_s0 * 0x44)) + (6620)) = (s16) ((var_s0 * 2) + 0x53);
            }
            break;
        case 2:
            if ((u8) *(u8 *)((unsigned char *)var_s1 + (41)) < 0x32U) {
                *(s16 *)((unsigned char *)(D_8011ECD0 + (var_s0 * 0x44)) + (6620)) = (s16) ((var_s0 * 2) + 0x54);
            }
            break;
        default:
            func_80046500();
            break;
        }
        var_s0 += 1;
        var_s2 += 0x1C;
        var_s1 += 0x1C;
    } while (var_s0 != 3);
}
