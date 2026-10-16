extern s16 D_80120914;
extern s16 D_80121724;
extern s16 D_80121732;
extern s16 D_80121736;
extern u8 D_800E738D;
extern u8 D_80120983;
extern u8 D_801209C7;
extern u8 D_80120A0B;
s32 func_80138080(void) {
    s32 spB4[1];
    s32 sp94[1];
    s16 sp74;
    s32 sp54[1];
    s32 *var_a0;
    s32 *var_a1;
    s32 *var_a3;
    s16 *var_a2;
    s16 temp_t6;
    s16 temp_t8;
    s16 var_t0;
    s32 *var_t1;
    s32 var_v0;
    u8 *var_v1;
    u8 temp_v1;

    temp_v1 = D_800E738D;
    if (temp_v1 == 1) {
        var_t1 = &D_800E7D10;
        var_a0 = &spB4;
        var_a1 = &sp94;
        var_a2 = &sp74;
        var_a3 = &sp54;
        var_t0 = 0x5C;
        temp_t6 = ((s16) D_80121726 / 17) * 0x10;
        D_80121726 = temp_t6;
        var_v0 = 0;
        D_80121728 = ((s16) D_80121728 / 17) * 0x10;
        if (temp_t6 < 0x1001) {
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            var_v1 = D_80120650;
            do {
                *(s16 *)((unsigned char *)var_v1 + (1044)) = var_t0;
                var_t0 += 2;
                var_a0 += 2;
                var_a1 += 2;
                var_a3 += 2;
                var_t1 += 1;
                var_v1 += 0x44;
                *var_a2 = 0x50;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v1 + (955)) = 0;
                *(s16 *)((unsigned char *)var_a0 + (-2)) = (s16) (((var_v0 / 7) << 7) - 0x78);
                *(s16 *)((unsigned char *)var_v1 + (990)) = (s16) *(s16 *)((unsigned char *)var_a0 + (-2));
                *(s16 *)((unsigned char *)var_a1 + (-2)) = (s16) (((var_v0 % 7) * 0x12) - 0x48);
                *(s16 *)((unsigned char *)var_v1 + (994)) = (s16) *(s16 *)((unsigned char *)var_a1 + (-2));
                if ((*(u8 *)((unsigned char *)var_t1 + (19)) == 0) && (D_800E6280.unk_53C[var_v0] == 0)) {
                    *var_a2 = 0;
                }
                var_v0 += 1;
                var_a2 += 2;
            } while (var_v0 != 0xE);
            menu_set(1, 0xE, &spB4, &sp94, &sp74, &sp54);
        }
    }
    if (temp_v1 == 0) {
        D_80121724 = 8;
        D_80121732 = -0xA0;
        D_80121736 = -0x78;
        D_800E738D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        D_8012093F &= 0xFF7F;
        D_80120983 &= 0xFF7F;
        D_801209C7 &= 0xFF7F;
        D_80120A0B &= 0xFF7F;
        temp_t8 = ((s16) D_80120914 / 9) * 8;
        D_80120914 = temp_t8;
        if (temp_t8 < 9) {
            D_800E738D = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-0x80, -0x58, 0, (s32) "エンディング", 0);
        k_sub_disp_start(1);
        func_8004284C();
    }
    D_801208FB = 0;
    D_8012170F = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    return 0;
}
