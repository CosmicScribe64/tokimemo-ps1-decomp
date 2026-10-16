void func_800430C0(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a1;
    s32 var_v0;

    D_800E7204 = D_800E7200;
    temp_v0 = func_80043010();
    temp_v1 = temp_v0 & 0xFFFF0000;
    if (temp_v1 == -0x10000) {
        var_a1 = temp_v0 & 0xFFFF;
        if (var_a1 == 0xFFFF) {
            var_a1 = 0;
        }
    } else {
        var_a1 = temp_v0;
        if ((temp_v0 & 0xFFFF) == 0xFFFF) {
            var_a1 = temp_v1;
            if (temp_v1 == -0x10000) {
                var_a1 = 0;
            }
        }
    }
    var_v0 = ~D_800E7204 & var_a1;
    D_800E720C = D_800E7204 & ~var_a1;
    if ((D_800B3CA0 != 0) && (D_800E7378 & 1)) {
        var_v0 |= 0x20;
    }
    D_800E7208 = var_v0;
    if (D_800E7391 == 1) {
        D_800E7208 = var_v0;
        if (var_v0 & 0x08000800) {
            if (D_800E7390 == 0) {
                D_800E7390 = 1;
                D_800E7392 = 0;
            } else {
                D_800E7392 = 1;
                D_800E7390 = 0;
            }
        }
    }
    D_800E7200 = var_a1;
    if (D_800E7388 != 0xD0) {
        D_800E7200 = var_a1;
        if (D_800E7388 != 0x20) {
            D_800E7200 = var_a1;
            if (D_800E7388 != 0x90) {
                D_800E7200 = var_a1;
                if (D_800E71F4 == 1) {
                    if ((var_a1 & 0x90C) == 0x90C) {
                        D_800E7200 = var_a1;
                        if ((u8) D_800B3CA4 >= 0x81U) {
                            D_800B3CA4 = 0;
                            func_800789E0();
                            func_800591D8(0);
                            func_800412E0(0);
                            main_state_set(0);
                            func_80041584();
                            func_80048DD0(0);
                            back_clear_switch(1);
                            dec_bg_reset();
                            func_8004482C();
                            tpage_buf_clear();
                            func_80044750(0x7F);
                            func_800424FC();
                            func_8007BDE8();
                            func_8007B844();
                            func_80042488();
                            func_800452C4();
                            func_80041878();
                            func_80042058();
                            func_80041F48();
                            if (D_800E62B5 != 0) {
                                func_80053D10();
                            }
                            func_8006BC28(0);
                            return;
                        }
                        D_800B3CA4 = (D_800B3CA4 + 1) & 0xFF;
                        goto block_31;
                    }
                    D_800E7200 = var_a1;
                    if ((var_a1 & 0x090C0000) == 0x090C0000) {
                        if ((u8) D_800B3CA4 >= 0x81U) {
                            D_800B3CA4 = 0;
                            func_800789E0();
                            func_800591D8(0);
                            func_800412E0(0);
                            main_state_set(0x11);
                            func_80041584();
                            func_80048DD0(0);
                            back_clear_switch(1);
                            dec_bg_reset();
                            func_8004482C();
                            tpage_buf_clear();
                            func_80044750(0x7F);
                            func_800424FC();
                            func_8007BDE8();
                            func_8007B844();
                            func_80042488();
                            func_800452C4();
                            func_80041878();
                            func_80042058();
                            func_80041F48();
                            if (D_800E62B5 != 0) {
                                func_80053D10();
                            }
                            func_8006BC28(0);
                            return;
                        }
                        D_800B3CA4 = (D_800B3CA4 + 1) & 0xFF;
                        goto block_31;
                    }
                    D_800B3CA4 = 0;
                    goto block_31;
                }
            }
        }
    }
block_31:
    func_80043448();
}
