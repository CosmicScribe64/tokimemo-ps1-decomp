s16 func_80140314();                                /* extern */
s16 func_80140698();                                /* extern */
extern u8 D_800B593C;
extern u8 D_800E738A;
extern u8 D_80120657;
extern s16 D_80120658;
extern s16 D_80120666;
extern s16 D_80120668;
extern u8 D_80146160;
extern u8 D_80146164;
extern ? D_8014616C;

s16 func_8013F224(void) {
    s16 var_v0;
    s32 var_at;
    s32 var_at_2;
    u8 temp_v1;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_8014616C, 0x6C);
    var_v0 = (&sp28[0])[D_800E738A](0x80);
    temp_v1 = D_800E738A;
    if (temp_v1 < 8U) {
        D_80120657 = D_800B593C;
        var_at = temp_v1 < 0xAU;
        if (D_80120668 == 0) {
            var_at = temp_v1 < 0xAU;
            if (D_80120658 == 0) {
                var_v0 = D_80120666 + 1;
                D_80120666 = var_v0;
                if (var_v0 >= 3) {
                    D_80120666 = 0;
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
