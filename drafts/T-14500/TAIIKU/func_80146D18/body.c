s32 func_80146D18(void) {
    s32 sp48[1];
    s16 sp40;
    s32 *var_s0;
    s16 *var_v1_3;
    s16 temp_t4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;
    u8 *var_v1;
    u8 *var_v1_2;

    sp40 = (s32) D_8014A5F4;
    *(s32 *)((unsigned char *)(&sp40) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A5F4) + (4));
    if (D_80120872 == 0) {
        var_v0_2 = D_8011ECD0;
        if (D_8014A3DE >= 0x781) {
            var_v1 = D_8014A400;
            do {
                *(s8 *)((unsigned char *)var_v0_2 + (7074)) = 0x40;
                *(s8 *)((unsigned char *)var_v0_2 + (7075)) = 0xA4;
                if (*(u8 *)((unsigned char *)var_v1 + (1)) == 1) {
                    D_801208B7 = 0;
                }
                var_v1 += 0x1C;
                var_v0_2 += 0x44;
            } while (var_v1 != D_8014A454);
        }
    }
    if (D_801208CC == 0x4F) {
        D_801208B7 |= 0x40;
    }
    var_v1_2 = D_8014A400;
    var_a0 = 0;
    do {
        if ((*(u8 *)((unsigned char *)var_v1_2 + (1)) == 1) && (*(s16 *)((unsigned char *)(D_8011ECD0 + (var_a0 * 0x44)) + (6634)) >= (D_801208DA + 0x30))) {
            D_801208B7 = 0;
            D_801208B6 = 0;
        }
        var_a0 += 1;
        var_v1_2 += 0x1C;
    } while (var_a0 < 3);
    var_v0_3 = D_8011ECD0;
    var_v1_3 = &sp40;
    do {
        temp_t4 = *var_v1_3;
        var_v1_3 += 2;
        var_v0_3 += 0x44;
        *(s16 *)((unsigned char *)var_v0_3 + (7042)) = (s16) ((temp_t4 - (D_8014A3DE * 2)) - 0xA0);
    } while ((u32) var_v1_3 < (u32) &sp48);
    if ((D_80120962 < -0xDC) && (D_8012093F != 0)) {
        D_8012093F = 0;
    }
    if (D_801208CC == 0x4F) {
        D_801208DA = D_80120896 + 0x20;
    }
    var_v0 = 0;
    if ((D_80120676 + 0x40) >= D_80120896) {
        if (*D_8014A3D8 == 0) {
            D_801208B7 = 0xE4;
            D_801208CC = 0x4F;
            D_801208DA = D_8012091E + 0x20;
        }
        var_a0_2 = 0;
        if (*D_8014A3D8 != 0) {
            var_v0_4 = D_8014A3D8;
            do {
                var_a0_2 += 1;
                var_v0_4 += 1;
            } while (*(u8 *)((unsigned char *)var_v0_4 + (1)) != 0);
        }
        D_8014A3D8[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_s0 = &*(s32 *)((unsigned char *)(&D_8014A3D8[var_a0_2]) + (1));
            do {
                var_s0 += 1;
                *(s8 *)((unsigned char *)var_s0 + (-1)) = func_80146FA0();
            } while (var_s0 != &D_8014A3DC);
        }
        var_v0 = 1;
    }
    return var_v0;
}
