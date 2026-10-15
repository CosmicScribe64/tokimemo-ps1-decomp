void func_8005A410(void) {
    s32 sp28;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_at_4;
    s32 var_v0;
    u8 *var_a1;

    var_v0 = 0;
    var_a2 = 0;
    if (D_800E71EC == 2) {
        var_a1 = D_80125C10;
        do {
            temp_v1 = (*(s32 *)var_a1);
            var_at = temp_v1 < -0x86;
            if (temp_v1 >= 0x7D) {
                var_v0 += 1;
                var_at = temp_v1 < -0x86;
            }
            if (var_at != 0) {
                var_v0 += 1;
            }
            temp_a0 = *(s16 *)((unsigned char *)var_a1 + (2));
            var_at_2 = temp_a0 < 0x55;
            if (temp_a0 < -0x54) {
                var_v0 += 1;
                var_at_2 = temp_a0 < 0x55;
            }
            if (var_at_2 == 0) {
                var_v0 += 1;
            }
            if ((temp_v1 == 0) && (temp_a0 == 0)) {
                var_a2 += 1;
            }
            temp_v1_2 = *(s16 *)((unsigned char *)var_a1 + (4));
            var_at_3 = temp_v1_2 < -0x86;
            if (temp_v1_2 >= 0x7D) {
                var_v0 += 1;
                var_at_3 = temp_v1_2 < -0x86;
            }
            if (var_at_3 != 0) {
                var_v0 += 1;
            }
            temp_a0_2 = *(s16 *)((unsigned char *)var_a1 + (6));
            var_a1 += 8;
            var_at_4 = temp_a0_2 < 0x55;
            if (temp_a0_2 < -0x54) {
                var_v0 += 1;
                var_at_4 = temp_a0_2 < 0x55;
            }
            if (var_at_4 == 0) {
                var_v0 += 1;
            }
            if ((temp_v1_2 == 0) && (temp_a0_2 == 0)) {
                var_a2 += 1;
            }
        } while (var_a1 != &D_80125C50);
    }
    if (var_a2 >= 2) {
        if (var_v0 != 0) {
            D_800E71ED = 0xF;
            sp28 = (s32) D_800E71EC;
            D_800E71EC = 0;
            func_8006509C();
            icon_disp_switch(0);
            D_800E71EC = (u8) sp28;
        }
    }
}
