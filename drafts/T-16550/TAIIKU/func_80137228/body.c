void func_80137228(void) {
    TaiRunnersA *var_v1;
    s32 var_a0;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_v0;
    u8 *var_a3;
    u8 *var_t0;
    u8 temp_a3;

    var_t0 = D_8011ECD0;
    var_v1 = &D_80149208;
    var_v0 = 0;
    var_a0 = 0;
    do {
        var_v1->cpu[0].unk_20.word += var_v1->cpu[0].unk_18;
        var_a2 = var_v1->cpu[0].unk_20.halves.high - D_8014929A;
        var_a0 += 8;
        if ((D_80120A72 + var_a0 + 8) < (var_a2 - 0x60)) {
            temp_a3 = var_v1->cpu[0].unk_06;
            if (temp_a3 == 0) {
                D_80120AA8 = 0x8D;
                if (temp_a3 == 0) {
                    var_a2_2 = 0;
                    if (*D_801491E0 != 0) {
                        var_a3 = D_801491E0;
                        do {
                            var_a2_2 += 1;
                            var_a3 += 1;
                        } while (var_a3[1] != 0);
                    }
                    D_801491E0[var_a2_2] = var_v0 + 2;
                }
                var_v1->cpu[0].unk_06 = 1;
                var_a2 = var_v1->cpu[0].unk_20.halves.high - D_8014929A;
            }
        }
        *(s16 *)((unsigned char *)var_t0 + (6634)) = (s16) (var_a2 - 0xA0);
        if ((var_a2 >= 0x1E1) || (var_a2 < -0xA0)) {
            *(s8 *)((unsigned char *)var_t0 + (6599)) = 0;
        } else {
            *(s8 *)((unsigned char *)var_t0 + (6599)) = 0xA4;
        }
        var_v0 += 1;
        var_v1 += 0x24;
        var_t0 += 0x44;
    } while (var_v0 != 3);
}
