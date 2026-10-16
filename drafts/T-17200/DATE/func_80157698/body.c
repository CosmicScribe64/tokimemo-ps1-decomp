void func_80157698(void) {
    s32 var_at;
    s32 var_at_2;
    u32 temp_t6;

    func_8014C5C8();
    temp_t6 = get_g_zyotai_s(6) & 0x7F;
    if ((temp_t6 >= 2U) && ((u16) D_800CA154 == 8) && (D_800E6280.unk_1104.w == 1)) {
        func_80083440(2);
    } else if (temp_t6 >= 2U) {
        var_at = temp_t6 < 2U;
        if (((u16) D_800CA154 == 0xA) && (var_at = temp_t6 < 2U, (D_800E6280.unk_1104.w == 1))) {
            func_80083440(3);
        } else {
            goto block_9;
        }
    } else {
        var_at = temp_t6 < 2U;
block_9:
        if (var_at == 0) {
            var_at_2 = temp_t6 < 2U;
            if (((u16) D_800CA154 == 6) && (var_at_2 = temp_t6 < 2U, (D_800E6280.unk_1104.w == 1))) {
                func_80083440(1);
            } else {
                goto block_14;
            }
        } else {
            var_at_2 = temp_t6 < 2U;
block_14:
            if (var_at_2 == 0) {
                if (((u16) D_800CA154 == 0xC) && (D_800E6280.unk_1104.w == 1)) {
                    func_80083440(1);
                }
            }
        }
    }
    D_800E6280.unk_1104.w += 1;
}
