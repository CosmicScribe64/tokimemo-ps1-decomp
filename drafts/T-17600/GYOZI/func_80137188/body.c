s32 func_80137188(void) {
    u8 temp_t7;
    u8 temp_v0;
    u8 var_v0;

    temp_t7 = D_800F6468 & 7;
    var_v0 = temp_t7;
    if (temp_t7 != 0) {
        var_v0 = (temp_t7 - 1) & 0xFF;
    }
    D_80146178 = var_v0;
    func_800BCE10(D_800D9320, (var_v0 * 5) + D_8014617C);
    temp_v0 = D_80146178;
    if (temp_v0 == 0) {
        D_800F54B6 -= 5;
    }
    D_80146178 = temp_v0;
    if (temp_v0 == 6) {
        D_800F54B6 += 5;
    }
    func_8008FB00();
}
