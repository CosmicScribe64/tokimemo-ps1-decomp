extern u8 D_800E62BC;
extern s8 D_800E7314;
extern u8 D_800E738D;
extern ? D_8012071C;
extern s16 D_80120910;
extern s16 D_80120914;
extern s16 D_8012091E;
extern s16 D_80120954;
extern s16 D_80120998;
extern s16 D_801209DC;

void func_80136688(void) {
    s16 sp78;
    s16 sp6C;
    ? sp60;
    ? sp54;
    ? *var_a2;
    ? *var_a3;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 var_a1;
    s16 var_t0;
    u32 temp_hi;
    u8 *var_v0;
    u8 temp_t9;

    if (D_800E738D == 0) {
        D_80120910 = 5;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E738D = 1;
        *D_80120912 = 0x7FFF;
    } else {
        var_v1 = &sp78;
        var_a0 = &sp6C;
        var_a1 = 0x18;
        var_a2 = &sp60;
        var_a3 = &sp54;
        var_t0 = 0x2E;
        temp_t6 = ((s16) *D_80120912 / 17) * 0x10;
        var_v0 = D_80120650;
        D_80120914 = ((s16) D_80120914 / 17) * 0x10;
        *D_80120912 = temp_t6;
        if (temp_t6 < 0x1001) {
            D_80120914 = 0x1000;
            *D_80120912 = 0x1000;
            do {
                temp_t9 = *(u8 *)((unsigned char *)var_v0 + (751));
                *var_v1 = 0;
                *var_a0 = var_a1;
                temp_t8 = *var_a0;
                temp_t7 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (704)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x18;
                var_v1 += 2;
                var_a0 += 2;
                var_a2 += 2;
                var_a3 += 2;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x80;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (683)) = (s8) (temp_t9 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (722)) = temp_t8;
                *(s16 *)((unsigned char *)var_v0 + (718)) = temp_t7;
            } while (var_v0 != &D_8012071C);
            func_8004F870(1, 3, &sp78, &sp6C, &sp60, &sp54);
            temp_hi = (u8) D_800E62BC % 3U;
            switch (temp_hi) {                      /* irregular */
            case 1:
                D_80120954 += 1;
                D_800E7314 = 0;
                break;
            case 0:
                D_80120998 += 1;
                D_800E7314 = 1;
                break;
            default:
            case 2:
                D_801209DC += 1;
                D_800E7314 = 2;
                break;
            }
            func_8004284C();
        }
    }
    func_801324E8();
}
