s16 func_8013F224(void) {
    s16 var_v0;
    s32 var_at;
    s32 var_at_2;
    u8 temp_v1;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_8014616C, 0x6C);
    var_v0 = (&sp28[0])[D_800E6280.unk_110A](0x80);
    temp_v1 = D_800E6280.unk_110A;
    if (temp_v1 < 8U) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) D_800B593C;
        var_at = temp_v1 < 0xAU;
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6552)) == 0) {
            var_at = temp_v1 < 0xAU;
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6536)) == 0) {
                var_v0 = *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) + 1;
                *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = var_v0;
                if (var_v0 >= 3) {
                    *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
                    var_v0 = 0;
                }
                goto block_5;
            }
        }
    } else {
block_5:
        var_at = temp_v1 < 0xAU;
    }
    if (var_at == 0) {
        var_at_2 = temp_v1 < 0xAU;
        if (D_80146160 != 0) {
            var_v0 = func_80140314();
            goto block_9;
        }
    } else {
block_9:
        var_at_2 = temp_v1 < 0xAU;
    }
    if (var_at_2 == 0) {
        if (D_80146164 != 0) {
            var_v0 = func_80140698();
        }
    }
    return var_v0;
}
