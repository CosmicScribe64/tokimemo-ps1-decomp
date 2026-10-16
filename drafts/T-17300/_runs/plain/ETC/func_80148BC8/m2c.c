extern ? D_800E6448;
extern s32 D_800E66B0;
extern s16 D_8015067C;
extern s16 D_8015067E;
extern s16 D_80150682;
extern s32 D_80150684;
extern ? D_801506BC;
extern s32 D_801506E4;
extern s32 D_801506E8;

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
    s32 temp_v1;
    s32 var_s2;

    var_s3 = &D_80150684;
    var_s2 = 0;
    do {
        temp_v1 = *var_s3;
        if (temp_v1 == 0xD) {
            temp_s0 = &D_80150630[var_s2];
            temp_s1 = &D_8015064C[var_s2];
            func_80148F7C((s32) D_80150682, *temp_s0 + 0x86, (s32) *temp_s1, 1, 0);
            func_80148F7C(0, *temp_s0 + 0x8E, (s32) *temp_s1, 0, 0);
            func_80148F7C(D_801506F0, *temp_s0 + 4, (s32) *temp_s1, 0, 1);
            if (var_s2 >= 7) {
                D_801506F4 = 1;
            } else {
                D_801506F4 = 0;
            }
        } else if (temp_v1 == 0xA) {
            temp_s0_2 = &D_80150630[var_s2];
            temp_s1_2 = &D_8015064C[var_s2];
            func_80148F7C((s32) D_8015067C, *temp_s0_2 + 0x86, (s32) *temp_s1_2, 1, 0);
            func_80148F7C(0, *temp_s0_2 + 0x8E, (s32) *temp_s1_2, 0, 0);
            func_80148F7C(D_801506E4, *temp_s0_2 + 4, (s32) *temp_s1_2, 0, 1);
        } else if (temp_v1 == 0xB) {
            if (((u32) (D_800E66B0 << 0x14) >> 0x1D) != 0) {
                temp_s0_3 = &D_80150630[var_s2];
                temp_s1_3 = &D_8015064C[var_s2];
                func_80148F7C((s32) D_8015067E, *temp_s0_3 + 0x86, (s32) *temp_s1_3, 1, 0);
                func_80148F7C(0, *temp_s0_3 + 0x8E, (s32) *temp_s1_3, 0, 0);
                func_80148F7C(D_801506E8, *temp_s0_3 + 4, (s32) *temp_s1_3, 0, 1);
            }
        } else if (*(&D_800E6448 + (temp_v1 * 0x38)) & 2) {
            temp_s0_4 = &D_80150630[var_s2];
            temp_s1_4 = &D_8015064C[var_s2];
            func_80148F7C((s32) D_80150668.value[temp_v1], *temp_s0_4 + 0x86, (s32) *temp_s1_4, 1, 0);
            func_80148F7C(0, *temp_s0_4 + 0x8E, (s32) *temp_s1_4, 0, 0);
            func_80148F7C(*(&D_801506BC + (*var_s3 * 4)), *temp_s0_4 + 4, (s32) *temp_s1_4, 0, 1);
        }
        var_s2 += 1;
        var_s3 += 4;
    } while (var_s2 != 0xE);
}
