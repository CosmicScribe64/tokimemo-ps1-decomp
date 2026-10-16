void func_801022C0(void) {
    typedef struct { s32 w[52]; } M2cBlockD0; /* m2c memcpy of 208 bytes */
    s32 sp2C[53];
    s16 var_a0;
    s32 temp_lo;
    u8 *temp_v0;
    u8 var_a3;

    *(M2cBlockD0 *)(&sp2C) = *(M2cBlockD0 *)(&D_801225A8);
    *(s32 *)((unsigned char *)((u8 *)sp2C + 0xCC) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801225A8.f[0x33]) + (4));
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    ((s32 (*)(s32))sp2C[D_800B1AF6])(0x80);
    if (D_800B1AF6 == 0xE) {
        var_a0 = 0;
        if (!(D_800B1AE4 & 1)) {
            if (D_800F19AB == 0x80) {
                var_a3 = 0x60;
            } else {
                var_a3 = 0x80;
            }
            D_800EAFA7 = var_a3;
            D_800EAFEB = var_a3;
            do {
                temp_lo = var_a0 * 0x24;
                var_a0 += 1;
                temp_v0 = &D_800EC190[temp_lo];
                *(u8 *)((unsigned char *)temp_v0 + (94)) = var_a3;
                *(u8 *)((unsigned char *)temp_v0 + (93)) = var_a3;
                *(u8 *)((unsigned char *)temp_v0 + (92)) = var_a3;
            } while (var_a0 < 6);
            D_800F19AB = var_a3;
        }
    }
}
