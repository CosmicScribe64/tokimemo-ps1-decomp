extern u8 D_80120A4E;
extern s8 D_80120A92;
extern u8 D_80120A93;
extern u8 D_80120B1B;
extern ? D_801491E1;
extern ? D_80149274;
extern s16 D_8014929A;
extern ? D_8014985C;

s32 func_80137EA8(void) {
    ? sp40;
    s16 sp38;
    ? *sp34;
    ? *var_v1_4;
    TaiikuSlots *var_v1;
    TaiikuSlots *var_v1_2;
    s16 *var_v1_3;
    s16 temp_t4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;

    sp38.unk0 = (s32) D_8014985C.unk0;
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014985C) + (4));
    if (D_80120A4E == 0) {
        var_v0_2 = D_8011ECD0;
        if (D_8014929A >= 0x781) {
            var_v1 = &D_80149208;
            do {
                *(s8 *)((unsigned char *)var_v0_2 + (7550)) = 0x40;
                *(s8 *)((unsigned char *)var_v0_2 + (7551)) = 0xA4;
                if (var_v1->pad24[6] == 1) {
                    D_80120A93 = 0;
                }
                var_v1 += 0x24;
                var_v0_2 += 0x44;
            } while (var_v1 != &D_80149274);
        }
    }
    if (D_80120AA8 == 0x8D) {
        D_80120A93 |= 0x40;
    }
    var_v1_2 = &D_80149208;
    var_a0 = 0;
    do {
        if ((var_v1_2->pad24[6] == 1) && (*(s16 *)((unsigned char *)(D_8011ECD0 + (var_a0 * 0x44)) + (6634)) >= (D_80120AB6 + 0x30))) {
            D_80120A93 = 0;
            D_80120A92 = 0;
        }
        var_a0 += 1;
        var_v1_2 += 0x24;
    } while (var_a0 < 3);
    var_v0_3 = D_8011ECD0;
    var_v1_3 = &sp38;
    do {
        temp_t4 = *var_v1_3;
        var_v1_3 += 2;
        var_v0_3 += 0x44;
        *(s16 *)((unsigned char *)var_v0_3 + (7518)) = (s16) ((temp_t4 - (D_8014929A * 2)) - 0xA0);
    } while ((u32) var_v1_3 < (u32) &sp40);
    if ((D_80120B3E < -0xDC) && (D_80120B1B != 0)) {
        D_80120B1B = 0;
    }
    if (D_80120AA8 == 0x8D) {
        D_80120AB6 = D_80120A72 + 0x20;
    }
    var_v0 = 0;
    if ((D_80120676 + 0x40) >= D_80120A72) {
        if (*D_801491E0 == 0) {
            D_80120A93 = 0xE4;
            D_80120AA8 = 0x8D;
            D_80120AB6 = D_80120AFA + 0x20;
        }
        var_a0_2 = 0;
        if (*D_801491E0 != 0) {
            var_v0_4 = D_801491E0;
            do {
                var_a0_2 += 1;
                var_v0_4 += 1;
            } while (var_v0_4[1] != 0);
        }
        D_801491E0[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_v1_4 = &*(? *)((unsigned char *)(&D_801491E0[var_a0_2]) + (1));
            do {
                sp34 = var_v1_4;
                var_v1_4 += 1;
                *(s8 *)((unsigned char *)var_v1_4 + (-1)) = func_8013815C();
            } while (var_v1_4 != &D_801491E4);
        }
        if (D_80122E94 == 1) {
            D_801491E1.unk0 = 2;
            *(s8 *)((unsigned char *)(&D_801491E1) + (1)) = 4;
            *(s8 *)((unsigned char *)(&D_801491E1) + (2)) = 1;
            *(s8 *)((unsigned char *)(&D_801491E1) + (-1)) = 3;
        }
        var_v0 = 1;
    }
    return var_v0;
}
