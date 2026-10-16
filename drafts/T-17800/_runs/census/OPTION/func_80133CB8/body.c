void func_80133CB8(void) {
    s16 sp6C[4];
    s16 sp64[4];
    s16 sp5C[4];
    s16 sp54[4];
    s16 *var_a2;
    s16 *var_a3;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t5;
    s16 temp_t6_2;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 var_a1;
    s16 var_t0;
    u32 temp_hi;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_t6;

    if (D_800E6280.unk_110D == 0) {
        D_80120910 = 2;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *D_80120912 = 0x7FFF;
    } else {
        var_v1 = sp6C;
        var_a0 = sp64;
        var_a1 = -0x38;
        var_a2 = sp5C;
        var_a3 = sp54;
        var_t0 = 0x16;
        temp_t7 = ((s16) *D_80120912 / 17) * 0x10;
        var_v0 = D_80120650;
        D_80120914 = ((s16) D_80120914 / 17) * 0x10;
        *D_80120912 = temp_t7;
        if (temp_t7 < 0x1001) {
            D_80120914 = 0x1000;
            *D_80120912 = 0x1000;
            do {
                temp_t6 = *(u8 *)((unsigned char *)var_v0 + (751));
                *var_v1 = -0x20;
                *var_a0 = var_a1;
                temp_t5 = *var_a0;
                temp_t9 = *var_v1;
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (704)) = var_t0;
                var_t0 += 2;
                var_a1 += 0x18;
                var_v1 += 1;
                var_a0 += 1;
                var_a2 += 1;
                var_a3 += 1;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x50;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                *(s8 *)((unsigned char *)var_v0 + (683)) = (s8) (temp_t6 | 0x80);
                *(s16 *)((unsigned char *)var_v0 + (722)) = temp_t5;
                *(s16 *)((unsigned char *)var_v0 + (718)) = temp_t9;
            } while ((u32) var_v0 < (u32) &D_8012071C);
            menu_set(1, 3, sp6C, sp64, sp5C, sp54);
            temp_hi = (u8) D_800E71EC % 3U;
            switch (temp_hi) {
            default:
            case 0:
                D_800E6280.unk_1094 = 0;
                break;
            case 1:
                D_800E6280.unk_1094 = 1;
                break;
            case 2:
                D_800E6280.unk_1094 = 2;
                break;
            }
            func_8006509C();
            var_v0_2 = D_80125C10;
            do {
                (*(s32 *)var_v0_2) = (s16) ((*(s32 *)var_v0_2) + 0xC8);
                temp_t9_2 = *(s16 *)((unsigned char *)var_v0_2 + (12));
                temp_t7_2 = *(s16 *)((unsigned char *)var_v0_2 + (8));
                temp_t6_2 = *(s16 *)((unsigned char *)var_v0_2 + (4)) + 0xC8;
                var_v0_2 += 0x10;
                *(s16 *)((unsigned char *)var_v0_2 + (-8)) = (s16) (temp_t7_2 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-4)) = (s16) (temp_t9_2 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-12)) = temp_t6_2;
            } while (var_v0_2 != &D_80125C50);
            func_800654D0((s32) D_800E71ED);
            func_8004284C();
        }
    }
    func_801324E8();
}
