void func_8013B994(void) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_at_4;
    u8 *var_a0;

    var_a2 = 0;
    var_a3 = 0;
    if ((u8) D_800E6280.unk_F6C == 2) {
        var_a0 = D_80125C10;
        do {
            temp_v0 = (*(s32 *)var_a0);
            var_at = temp_v0 < -0x86;
            if (temp_v0 >= 0x7D) {
                var_a2 += 1;
                var_at = temp_v0 < -0x86;
            }
            if (var_at != 0) {
                var_a2 += 1;
            }
            temp_v1 = *(s16 *)((unsigned char *)var_a0 + (2));
            var_at_2 = temp_v1 < 0x55;
            if (temp_v1 < -0x54) {
                var_a2 += 1;
                var_at_2 = temp_v1 < 0x55;
            }
            if (var_at_2 == 0) {
                var_a2 += 1;
            }
            if ((temp_v0 == 0) && (temp_v1 == 0)) {
                var_a3 += 1;
            }
            temp_v0_2 = *(s16 *)((unsigned char *)var_a0 + (4));
            var_at_3 = temp_v0_2 < -0x86;
            if (temp_v0_2 >= 0x7D) {
                var_a2 += 1;
                var_at_3 = temp_v0_2 < -0x86;
            }
            if (var_at_3 != 0) {
                var_a2 += 1;
            }
            temp_v1_2 = *(s16 *)((unsigned char *)var_a0 + (6));
            var_a0 += 8;
            var_at_4 = temp_v1_2 < 0x55;
            if (temp_v1_2 < -0x54) {
                var_a2 += 1;
                var_at_4 = temp_v1_2 < 0x55;
            }
            if (var_at_4 == 0) {
                var_a2 += 1;
            }
            if ((temp_v0_2 == 0) && (temp_v1_2 == 0)) {
                var_a3 += 1;
            }
        } while (var_a0 != &D_80125C50);
    }
    sp34 = var_a2;
    sp30 = var_a3;
    func_800AE0B0("err %d \n", var_a2, var_a2, var_a3);
    func_800AE0B0("zero %d \n", sp30);
    if (var_a3 >= 2) {
        if (var_a2 != 0) {
            D_800E6280.unk_F6C = 0;
            sp2C = (s32) (u8) D_800E6280.unk_F6C;
            func_8006509C();
            icon_disp_switch(0);
            D_800E6280.unk_F6C = (s8) sp2C;
        }
    }
}
