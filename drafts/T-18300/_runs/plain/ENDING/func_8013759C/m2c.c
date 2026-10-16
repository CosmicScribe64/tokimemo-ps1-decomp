void func_8013759C(void) {
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_lo;
    s32 temp_t9;
    s32 temp_v0;
    u8 temp_t6;

    func_8004E9F4(0);
    temp_t6 = (D_8011F513 - 1) & 0xFF;
    D_8011F513 = temp_t6;
    if (temp_t6 < 8U) {
        D_8011F513 = 0;
        func_8004284C();
    }
    var_s0 = 4;
    D_8011F4CF = D_8011F513;
    do {
        func_80084C98(var_s0, D_8011F513);
        var_s0 += 1;
    } while (var_s0 < 0x13);
    var_s0_2 = 0;
    do {
        temp_lo = var_s0_2 * 0x44;
        var_s0_2 += 1;
        *(u8 *)((unsigned char *)(&D_8011ECD0[temp_lo]) + (6535)) = (u8) D_8011F513;
    } while (var_s0_2 < 0xE);
    temp_t9 = ((u32) (D_8011F513 * 0xC0) >> 7) & 0xFF;
    temp_v0 = ((u32) (D_8011F513 * 0xFF) >> 7) & 0xFF;
    func_80085678(-0xA0, -0x78, 0x140, 0xF0, 0xE, (temp_t9 << 0x10) | (temp_t9 << 8) | temp_v0, ((((u32) (D_8011F513 << 6) >> 7) & 0xFF) << 0x10) | ((((u32) (D_8011F513 << 7) >> 7) & 0xFF) << 8) | temp_v0);
    func_8004955C(0xE);
}
