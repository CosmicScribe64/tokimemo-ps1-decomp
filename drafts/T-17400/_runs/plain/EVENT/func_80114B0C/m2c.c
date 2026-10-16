void func_80114B0C(void) {
    s16 *temp_v0;
    s16 *temp_v0_2;
    s16 temp_a2;
    s16 var_a1;
    s32 var_a0;

    var_a1 = 0;
    var_a0 = 0 * 2;
    do {
        temp_a2 = *(&D_8012439C + var_a0);
        temp_v0 = &D_80124394 + var_a0;
        *temp_v0 = temp_a2;
        if (*temp_v0 == 0) {
            *temp_v0 = 1;
        }
        if (*temp_v0 == 4) {
            *temp_v0 = 3;
        }
        var_a1 += 1;
        if (D_800B0F2C == D_800B0F35[D_800B1746]) {
            *temp_v0 = 3;
        }
        temp_v0_2 = &D_801243A4 + var_a0;
        *temp_v0_2 = temp_a2 - 1;
        if (*temp_v0_2 == -1) {
            *temp_v0_2 = 0;
        }
        if (*temp_v0_2 == 3) {
            *temp_v0_2 = 2;
        }
        var_a0 = var_a1 * 2;
    } while (var_a1 < 3);
    func_80011DFC(var_a0, var_a1, temp_a2, 3);
}
