void func_801428EC(void) {
    s16 sp42;
    s16 sp40;
    s16 var_t0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a1_2;
    u8 *temp_v0;
    u8 *temp_v0_2;

    var_v1 = 0x1C - (((s16) D_8012B920 / 512) * 4);
    var_t0 = 1;
    if (D_8012B920 == 0x1000) {
        var_v1 = 0;
        var_t0 = 0;
    }
    temp_v0 = (*(&D_801483E8 + (D_80148828[D_80148818] + (D_800F53DE * 0xC))) * 8) + &D_8014882C;
    temp_a0 = (D_8012B92A - var_t0) - 0x1C;
    temp_a1 = (D_8012B92E + var_v1) - 0x1C;
    sp42 = var_v1;
    sp40 = var_t0;
    func_80142A8C(temp_a0, temp_a1, D_8012B92A - 1, temp_a1, temp_a0, D_8012B92E - 1, (*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), 9, 0);
    temp_v0_2 = (*(&D_801483E8 + (D_80148828[D_80148818] + (D_800F53DE * 0xC))) * 8) + &D_8014882C;
    temp_a0_2 = D_8012B92A + var_t0 + 0x1C;
    temp_a1_2 = (D_8012B92E - var_v1) + 0x1C;
    func_80142A8C(temp_a0_2, temp_a1_2, temp_a0_2, (s32) D_8012B92E, (s32) D_8012B92A, temp_a1_2, (*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), 9, 0);
    func_80054FCC(0xB);
}
