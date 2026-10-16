void func_80133308(void) {
    s16 var_s1;
    s32 temp_s2;
    s32 temp_t3;
    s32 temp_v0;
    s32 var_s6;
    u8 temp_t5;
    u8 temp_t7;

    temp_v0 = get_g_zyotai_s(D_800E6280.unk_F5F);
    if (D_80122CDC != 0) {
        temp_t7 = D_800E6280.unk_F5F;
        D_80144C54 = 0;
        temp_t3 = ((u32) D_800E6280.unk_10F8 % (u32) ((((s16) D_800E6280.unk_1BC[temp_t7].unk_02 / 20) & 0xFF) + 1)) & 0xFF;
        if ((u32) (temp_v0 & 0x7F & 0xFF) < 2U) {
            D_800E6280.unk_1BC[temp_t7].unk_0A = D_800E6280.unk_1BC[temp_t7].unk_0A + temp_t3 + 0xA;
        } else {
            D_800E6280.unk_1BC[temp_t7].unk_0A = D_800E6280.unk_1BC[temp_t7].unk_0A + temp_t3 + 6;
        }
        D_80144C3C += 2;
    } else {
        temp_t5 = D_800E6280.unk_F5F;
        D_80144C54 = 1;
        if ((temp_t5 == 0) || (temp_t5 == 7) || (temp_t5 == 1) || (temp_t5 == 2) || (temp_t5 == 8) || (var_s6 = 4, (temp_t5 == 9))) {
            var_s6 = 0;
            if ((u32) (temp_v0 & 0x7F & 0xFF) >= 3U) {
                D_80144C54 = 0;
            } else {
                var_s6 = 4;
            }
        }
        var_s1 = 0;
        do {
            if ((((u32) (D_800E6280.unk_1BC[var_s1].unk_0C.w << 0x1E) >> 0x1F) != 0) && (var_s1 != D_800E6280.unk_F5F)) {
                temp_s2 = ((s16) D_800E6280.unk_1BC[var_s1].unk_02 / 20) & 0xFF;
                D_800E6280.unk_1BC[var_s1].unk_0A = D_800E6280.unk_1BC[var_s1].unk_0A + var_s6 + ((func_800AE0D0() % (u32) (temp_s2 + 1)) & 0xFF);
            }
            var_s1 += 1;
        } while (var_s1 < 0xB);
    }
    check_para_limit();
    func_8004284C();
}
