void func_8013F948(void) {
    s16 temp_a1;
    s16 temp_a1_2;
    s16 var_a0;
    s32 var_v0;
    u8 *temp_a0;
    u8 *temp_a0_2;

    var_v0 = 0;
    if ((u8) D_801499DC >= 0x1FU) {
        var_a0 = D_80120888;
        D_801499DC = 0;
        do {
            if (var_v0 == 0) {
                if (D_80149990.unk_00 == 0) {
                    if (var_a0 >= 0x90) {
                        var_a0 ^= 1;
                    } else if (var_a0 == 0x8D) {
                        var_a0 = 0x8A;
                    } else {
                        var_a0 += 1;
                    }
                }
            } else if (*(u8 *)((unsigned char *)(&D_801499CC + var_v0) + (-1)) == 0) {
                D_80120888 = var_a0;
                temp_a0 = D_8011ECD0 + (var_v0 * 0x44);
                temp_a1 = *(s16 *)((unsigned char *)temp_a0 + (7096));
                if (temp_a1 >= 0x8E) {
                    *(s16 *)((unsigned char *)temp_a0 + (7096)) = (s16) (temp_a1 ^ 1);
                    var_a0 = D_80120888;
                } else if (temp_a1 == 0x89) {
                    *(s16 *)((unsigned char *)temp_a0 + (7096)) = 0x86;
                    var_a0 = D_80120888;
                } else {
                    *(s16 *)((unsigned char *)temp_a0 + (7096)) = (s16) (temp_a1 + 1);
                    var_a0 = D_80120888;
                }
            }
            if (var_v0 == -1) {
                if (D_80149990.unk_00 == 0) {
                    if (var_a0 >= 0x90) {
                        var_a0 ^= 1;
                    } else if (var_a0 == 0x8D) {
                        var_a0 = 0x8A;
                    } else {
                        var_a0 += 1;
                    }
                }
                D_80120888 = var_a0;
            } else {
                D_80120888 = var_a0;
                if (*(&D_801499CC + var_v0) == 0) {
                    temp_a0_2 = D_8011ECD0 + (var_v0 * 0x44);
                    temp_a1_2 = *(s16 *)((unsigned char *)temp_a0_2 + (7164));
                    if (temp_a1_2 >= 0x8E) {
                        *(s16 *)((unsigned char *)temp_a0_2 + (7164)) = (s16) (temp_a1_2 ^ 1);
                    } else if (temp_a1_2 == 0x89) {
                        *(s16 *)((unsigned char *)temp_a0_2 + (7164)) = 0x86;
                    } else {
                        *(s16 *)((unsigned char *)temp_a0_2 + (7164)) = (s16) (temp_a1_2 + 1);
                    }
                }
            }
            var_a0 = D_80120888;
            var_v0 += 2;
        } while (var_v0 != 4);
    }
}
