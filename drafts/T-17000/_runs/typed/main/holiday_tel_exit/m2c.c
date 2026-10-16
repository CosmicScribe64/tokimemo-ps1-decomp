extern ? D_800B3778;
extern ? D_800B37BE;

void holiday_tel_exit(void) {
    s32 temp_v0;
    u8 temp_a0;

    temp_a0 = D_800E6280.unk_110D;
    if (temp_a0 != 0xB) {
        if ((D_800E6280.unk_1104.w & 7) == 7) {
            if (temp_a0 == 0) {
                func_80044750(0x504);
            } else if (temp_a0 >= 4U) {
                temp_v0 = temp_a0 - 4;
                if (D_800E6280.unk_F5F == 0xD) {
                    func_80044750((*(&D_800B37BE + temp_v0) + 0x505) & 0xFFFF);
                } else {
                    func_80044750((*(&D_800B3778 + ((D_800E6280.unk_F5F * 7) + temp_v0)) + 0x505) & 0xFFFF);
                }
            }
            D_800E6280.unk_110D = temp_a0 + 1;
        }
    } else if (D_800E6280.unk_F5F == 0xD) {
        D_800E6280.unk_1093 = 0;
        k_sub_reset();
        restore_bgm();
        main_state_set(0x45);
        func_80042908(0);
    } else if (D_800E6280.unk_F5F == 0xB) {
        k_sub_reset();
        D_800E6280.unk_1093 = 0xC;
        D_801230D0 = 0xB;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    } else {
        k_sub_reset();
        D_800E6280.unk_1093 = D_800E6280.unk_F5F + 1;
        D_801230D0 = (s32) D_800E6280.unk_F5F;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    }
}
