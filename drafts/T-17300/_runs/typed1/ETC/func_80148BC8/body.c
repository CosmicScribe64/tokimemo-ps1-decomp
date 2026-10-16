void func_80148BC8(void) {
    s16 *temp_s0;
    s16 *temp_s0_2;
    s16 *temp_s0_3;
    s16 *temp_s0_4;
    s16 *temp_s1;
    s16 *temp_s1_2;
    s16 *temp_s1_3;
    s16 *temp_s1_4;
    s32 *var_s3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 var_s2;

    var_s3 = &D_80150668 + 0x1C;
    var_s2 = 0;
    do {
        temp_v1 = *var_s3;
        if (temp_v1 == 0xD) {
            temp_v0 = var_s2 * 2;
            temp_s0 = &D_80150630 + temp_v0;
            temp_s1 = &D_8015064C + temp_v0;
            func_80148F7C(D_80150668.value[13], *temp_s0 + 0x86, *temp_s1, 1, 0);
            func_80148F7C(0, *temp_s0 + 0x8E, *temp_s1, 0, 0);
            func_80148F7C((s16) D_80150668.rank[13], *temp_s0 + 4, *temp_s1, 0, 1);
            if (var_s2 >= 7) {
                D_801506F4 = 1;
            } else {
                D_801506F4 = 0;
            }
        } else if (temp_v1 == 0xA) {
            temp_v0_2 = var_s2 * 2;
            temp_s0_2 = &D_80150630 + temp_v0_2;
            temp_s1_2 = &D_8015064C + temp_v0_2;
            func_80148F7C(D_80150668.value[10], *temp_s0_2 + 0x86, *temp_s1_2, 1, 0);
            func_80148F7C(0, *temp_s0_2 + 0x8E, *temp_s1_2, 0, 0);
            func_80148F7C((s16) D_80150668.rank[10], *temp_s0_2 + 4, *temp_s1_2, 0, 1);
        } else if (temp_v1 == 0xB) {
            if (((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (1072)) << 0x14) >> 0x1D) != 0) {
                temp_v0_3 = var_s2 * 2;
                temp_s0_3 = &D_80150630 + temp_v0_3;
                temp_s1_3 = &D_8015064C + temp_v0_3;
                func_80148F7C(D_80150668.value[11], *temp_s0_3 + 0x86, *temp_s1_3, 1, 0);
                func_80148F7C(0, *temp_s0_3 + 0x8E, *temp_s1_3, 0, 0);
                func_80148F7C((s16) D_80150668.rank[11], *temp_s0_3 + 4, *temp_s1_3, 0, 1);
            }
        } else if (*(&D_800E6280 + (temp_v1 * 0x38)) & 2) {
            temp_v0_4 = var_s2 * 2;
            temp_s0_4 = &D_80150630 + temp_v0_4;
            temp_s1_4 = &D_8015064C + temp_v0_4;
            func_80148F7C(*(&D_80150668 + (temp_v1 * 2)), *temp_s0_4 + 0x86, *temp_s1_4, 1, 0);
            func_80148F7C(0, *temp_s0_4 + 0x8E, *temp_s1_4, 0, 0);
            func_80148F7C((s16) *(&D_80150668 + (*var_s3 * 4)), *temp_s0_4 + 4, *temp_s1_4, 0, 1);
        }
        var_s2 += 1;
        var_s3 += 1;
    } while (var_s2 != 0xE);
}
