s32 func_80137C44(void) {
    TaiikuSlots *var_v1;
    TaiikuSlots *var_v1_2;
    s16 temp_t0;
    s32 temp_a1;
    s32 var_v0;
    u8 *var_a0;
    u8 *temp_a0;

    var_v1 = &D_80149208;
    var_v0 = 0;
    do {
        if (var_v1->pad00[0] != 0) {
            temp_a0 = D_8011ECD0 + (var_v0 * 0x44);
            *(s16 *)((unsigned char *)temp_a0 + (7314)) = (s16) (*(s16 *)((unsigned char *)temp_a0 + (6566)) - 8);
            if (var_v0 == 0) {
                temp_t0 = D_801492B8[D_8014920C];
                if (D_80122E94 == 1) {
                    D_80120954 = (((s16) *(s16 *)((unsigned char *)(D_8011ECD0 + (D_801491D8 * 0x44)) + (3150)) >> 3) * 6) + temp_t0;
                } else {
                    D_80120954 = (((s16) *(s16 *)((unsigned char *)(D_8011ECD0 + (var_v1->pad00[2] * 0x44)) + (3150)) >> 3) * 6) + temp_t0;
                }
            } else {
                *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(D_8011ECD0 + (var_v1->pad00[2] * 0x44)) + (3150)) >> 3) * 6) + D_801492B8[var_v1->f03]);
            }
            if ((var_v0 + 0x90) == *(s16 *)((unsigned char *)temp_a0 + (6552))) {
                if (D_80122E94 == 1) {
                    *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((D_80149320[D_801491DC] * 6) + 5);
                } else {
                    *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(D_8011ECD0 + (var_v1->pad00[2] * 0x44)) + (3150)) >> 3) * 6) + 5);
                }
            }
        }
        var_v0 += 1;
        var_v1 += 0x24;
    } while (var_v0 < 4);
    var_a0 = D_8011ECD0;
    var_v1_2 = &D_80149208;
    do {
        if ((((var_v1_2->pad24[5] == 2) && (*(u8 *)((unsigned char *)var_a0 + (7346)) != 0)) || (var_v1_2->pad24[0] == 1)) && (temp_a1 = *(s16 *)((unsigned char *)var_v1_2 + (70)) - D_8014929A, ((temp_a1 < 0x140) != 0)) && (temp_a1 >= -0x13F)) {
            *(s8 *)((unsigned char *)var_a0 + (7347)) = 0xA4;
        } else {
            *(s8 *)((unsigned char *)var_a0 + (7347)) = 0;
        }
        var_a0 += 0x44;
        var_v1_2 += 0x24;
    } while (var_a0 != D_8011ED9C);
    return (s32) D_8014929A;
}
