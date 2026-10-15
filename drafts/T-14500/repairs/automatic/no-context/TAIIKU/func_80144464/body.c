s32 func_80144464(void) {
    s32 sp40[1];
    s16 sp38;
    s32 *var_s0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_4;
    s16 *var_v1;
    s16 temp_t6;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_5;

    var_v0_2 = &D_8011ECD0;
    sp38 = (s32) D_8014A318;
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A318) + (4));
    do {
        var_v0_2 += 0x44;
        *(s8 *)((unsigned char *)var_v0_2 + (6802)) = 0x40;
        *(s8 *)((unsigned char *)var_v0_2 + (6803)) = 0xA4;
    } while ((u32) var_v0_2 < (u32) &D_8011ED9C);
    var_v0_3 = &D_8014A13C;
    var_a0 = 0;
    do {
        if ((*(u8 *)((unsigned char *)var_v0_3 + (37)) == 1) && ((D_801207CA + 0x30) < (*(s16 *)((unsigned char *)(&D_8011ECD0 + (var_a0 * 0x44)) + (6634)) + 0x40))) {
            D_801207EB = 0;
            D_801207EA = 0;
        }
        var_a0 += 1;
        var_v0_3 += 0x24;
    } while (var_a0 < 3);
    var_v0_4 = &D_8011ECD0;
    var_v1 = &sp38;
    do {
        temp_t6 = *var_v1;
        var_v1 += 2;
        var_v0_4 += 0x44;
        *(s16 *)((unsigned char *)var_v0_4 + (6838)) = (s16) ((temp_t6 - D_8014A1CE) - 0xA0);
    } while ((u32) var_v1 < (u32) &sp40);
    if (D_80120800 == 0x32) {
        D_801207EB |= 0x40;
        D_8012080E = D_801207CA + 0x20;
    }
    if ((D_80120896 < -0xDC) && (D_80120873 != 0)) {
        D_80120873 = 0;
    }
    var_v0 = 0;
    if ((D_80120676 + 0x40) >= D_801207CA) {
        if (D_8014A120 == 0) {
            D_801207EB = 0xE4;
            D_80120800 = 0x32;
            D_8012080E = D_801207CA + 0x20;
        }
        var_a0_2 = 0;
        if (D_8014A120 != 0) {
            var_v0_5 = &D_8014A120;
            do {
                var_a0_2 += 1;
                var_v0_5 += 1;
            } while (*(u8 *)((unsigned char *)var_v0_5 + (1)) != 0);
        }
        (&D_8014A120)[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_s0 = &*(s32 *)((unsigned char *)(&(&D_8014A120)[var_a0_2]) + (1));
            do {
                var_s0 += 1;
                *(s8 *)((unsigned char *)var_s0 + (-1)) = func_801446A0();
            } while (var_s0 != &D_8014A124);
        }
        var_v0 = 1;
    }
    return var_v0;
}
