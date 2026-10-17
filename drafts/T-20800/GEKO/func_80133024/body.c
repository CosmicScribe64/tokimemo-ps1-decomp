s32 func_80133024(void) {
    GekoMonthDays sp38;
    s16 temp_v1;
    s16 var_s0;
    u8 temp_a2;

    sp38 = D_80144D28;
    var_s0 = 1;
    if (D_800E6280.unk_03E == 0x60) {
        sp38.day[1] = 0x1D;
    }
    D_80144C50 = 0;
    do {
        temp_a2 = D_800E6280.unk_040;
        temp_v1 = sp38.day[D_800E6280.unk_03F - 1];
        if ((u32) temp_v1 >= (u32) (temp_a2 + var_s0)) {
            func_80132D04(0, (s32) (s16) ((temp_a2 + var_s0) - 1));
        } else {
            func_80132D04(1, (s32) (s16) (((temp_a2 + var_s0) - temp_v1) - 1));
        }
        var_s0 += 1;
    } while (var_s0 < 0x14);
    func_80132B24();
}
