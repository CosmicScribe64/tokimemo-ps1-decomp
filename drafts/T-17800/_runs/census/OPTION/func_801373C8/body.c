s32 func_801373C8(void) {
    s16 sp74[6];
    s16 sp68[6];
    s16 sp5C[6];
    s16 sp50[6];
    s16 sp5E;
    s16 *var_a3;
    s16 *var_a0;
    s16 *var_a2;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t8;
    s16 var_a1;
    s16 var_t0;
    u8 *var_v0;
    u8 temp_t9;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 != 0) {
        var_a0 = sp68;
        var_a1 = 8;
        var_a2 = sp5C;
        var_a3 = sp50;
        var_t0 = 0x3C;
        temp_t6 = ((s16) D_80121726 / 17) * 0x10;
        D_80121726 = temp_t6;
        var_v0 = D_80120650;
        D_80121728 = ((s16) D_80121728 / 17) * 0x10;
        if (temp_t6 < 0x1001) {
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            var_v1 = sp74;
            do {
                temp_t9 = *(u8 *)((unsigned char *)var_v0 + (1023));
                *var_v1 = -0x50;
                *var_a0 = var_a1;
                temp_t8 = *var_a0;
                temp_t7 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (976)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x10;
                var_v1 += 1;
                var_a0 += 1;
                var_a2 += 1;
                var_a3 += 1;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0xC4;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (955)) = (s8) (temp_t9 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (994)) = temp_t8;
                *(s16 *)((unsigned char *)var_v0 + (990)) = temp_t7;
            } while (var_v0 != D_801206D8);
            D_80120A4F = 0;
            D_80120A93 = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    sp5C[0] = 0;
                }
                if (!(D_800E7D50 & 1)) {
                    sp5E = 0;
                }
            }
            menu_set(1, 2, sp74, sp68, sp5C, sp50);
        }
    }
    if (temp_v1 == 0) {
        D_80121724 = 7;
        D_80121732 = -0xA0;
        D_80121736 = -0x78;
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        D_8012093F &= 0xFF7F;
        D_80120983 &= 0xFF7F;
        D_801209C7 &= 0xFF7F;
        D_80120A0B &= 0xFF7F;
        temp_t7_2 = ((s16) *D_80120912 / 9) * 8;
        *D_80120912 = temp_t7_2;
        if (temp_t7_2 < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004284C();
    }
    D_801208FB = 0;
    D_8012170F = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    return 0;
}
