s32 geko_judge(s8 *arg0) {
    ? sp7C;
    ? sp5C;
    s32 temp_hi;
    s32 var_s1;
    u32 temp_s0;
    u32 var_s2;

    *arg0 = 0;
    temp_s0 = (u32) D_800E7D10 % 11U;
    func_80053418((s32) &sp7C, (s32) &sp5C);
    var_s1 = 0;
    var_s2 = temp_s0;
loop_1:
    temp_hi = (s32) var_s2 % 11;
    if ((((u32) (D_800E6280.unk_1BC[temp_hi].unk_0C.w << 0x1E) >> 0x1F) != 0) && (D_800E6280.unk_1BC[temp_hi].unk_0A < 0x3C) && (get_g_zyotai_s(temp_hi) != 4) && ((u8) *(&sp7C + temp_hi) >= 0x11U) && ((rand() % 100) < 0x32)) {
        var_s1 += 1;
        if (D_800B6740 != temp_hi) {
            *arg0 = (s8) temp_hi;
            D_800B6740 = (s8) temp_hi;
            return 1;
        }
        goto block_9;
    }
    var_s1 += 1;
block_9:
    var_s2 += 1;
    if (var_s1 == 0xB) {
        return 0;
    }
    goto loop_1;
}
