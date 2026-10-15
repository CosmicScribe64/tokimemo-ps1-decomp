void func_80155004(void) {
    DateFnTable53 tbl;
    s16 var_a0;
    s32 temp_lo;
    u8 var_a3;
    u8 *temp_v0;

    tbl = D_8015F82C;
    tbl.f[D_800E738A](0x80);
    if (D_800E738A == 0xE) {
        if (D_800E7208 & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        var_a0 = 0x3A;
        if (((u32) D_800E7378 % 3U) == 0) {
            if ((u8) D_8015F5D0 != 0) {
                var_a3 = 0x80;
            } else {
                var_a3 = 0x80;
                if (D_800B593C == 0x80) {
                    var_a3 = 0x60;
                }
            }
            D_80120657 = var_a3;
            D_8012069B = var_a3;
            do {
                temp_lo = var_a0 * 0x24;
                var_a0 += 1;
                temp_v0 = D_801217D0 + temp_lo;
                *(u8 *)((unsigned char *)temp_v0 + (22)) = var_a3;
                *(u8 *)((unsigned char *)temp_v0 + (21)) = var_a3;
                *(u8 *)((unsigned char *)temp_v0 + (20)) = var_a3;
            } while (var_a0 < 0x40);
            D_800B593C = var_a3;
        }
    }
}
