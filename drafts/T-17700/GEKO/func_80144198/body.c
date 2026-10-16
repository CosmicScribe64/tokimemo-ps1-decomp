u8 func_80144198(s32 arg0) {
    GekoMonthDays sp18;
    s16 temp_t0;
    s32 var_v0;
    s32 var_v1;
    u8 temp_a3;
    u8 temp_v0;
    u8 var_a0;
    u8 var_a1;

    temp_v0 = D_800E6280.unk_03E;
    sp18 = D_80147C54;
    if (temp_v0 == 0x60) {
        sp18.day[1] = 0x1D;
    }
    temp_a3 = D_800E6280.unk_03F;
    temp_t0 = sp18.day[temp_a3 - 1];
    var_v1 = (D_800E6280.unk_040 - D_800E6280.unk_041) + 6;
    var_a1 = temp_v0;
    var_a0 = temp_a3;
    if (temp_t0 < var_v1) {
        var_a0 = temp_a3 + 1;
        var_v1 -= temp_t0;
        if ((s32) var_a0 >= 0xD) {
            var_a0 = 1;
            var_a1 = temp_v0 + 1;
        }
    }
    if (arg0 == 0) {
        return var_a1;
    }
    var_v0 = var_v1;
    if (arg0 == 1) {
        var_v0 = (s32) var_a0;
    }
    return (u8) var_v0;
}
