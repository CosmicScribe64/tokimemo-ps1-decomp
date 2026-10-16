void x_taku_menu_set(s32 arg0, s32 arg1) {
    s32 sp50[16];
    s32 sp58[16];
    s32 sp60[16];
    s16 *var_a0;
    s16 *var_a0_2;
    s16 *var_v0;
    s16 *var_v0_2;
    s16 *var_v1;
    s16 *var_v1_2;
    s32 temp_a1;
    s32 var_t0;

    var_t0 = 0;
    if (arg1 > 0) {
        temp_a1 = arg1 & 3;
        if (temp_a1 != 0) {
            var_v0 = &(&sp50[0])[0];
            var_a0 = &(&sp58[0])[0];
            var_v1 = &(&sp60[0])[0];
            do {
                var_t0 += 1;
                *var_v1 = -0x78;
                *var_a0 = 0xF0;
                *var_v0 = 0x10;
                var_v1 += 1;
                var_a0 += 1;
                var_v0 += 1;
            } while (temp_a1 != var_t0);
            if (var_t0 != arg1) {
                goto block_5;
            }
        } else {
block_5:
            var_a0_2 = &(&sp58[0])[var_t0];
            var_v1_2 = &(&sp60[0])[var_t0];
            var_v0_2 = &(&sp50[0])[var_t0];
            do {
                var_v0_2 += 4;
                *(s16 *)((unsigned char *)var_v1_2 + (2)) = -0x78;
                *(s16 *)((unsigned char *)var_a0_2 + (2)) = 0xF0;
                *(s16 *)((unsigned char *)var_v0_2 + (-6)) = 0x10;
                *(s16 *)((unsigned char *)var_v1_2 + (4)) = -0x78;
                *(s16 *)((unsigned char *)var_a0_2 + (4)) = 0xF0;
                *(s16 *)((unsigned char *)var_v0_2 + (-4)) = 0x10;
                *(s16 *)((unsigned char *)var_v1_2 + (6)) = -0x78;
                *(s16 *)((unsigned char *)var_a0_2 + (6)) = 0xF0;
                *(s16 *)((unsigned char *)var_v0_2 + (-2)) = 0x10;
                var_v1_2 += 4;
                var_a0_2 += 4;
                *(s16 *)((unsigned char *)var_v1_2 + (-8)) = -0x78;
                *(s16 *)((unsigned char *)var_a0_2 + (-8)) = 0xF0;
                *(s16 *)((unsigned char *)var_v0_2 + (-8)) = 0x10;
            } while (var_v0_2 != &(&sp50[0])[arg1]);
        }
    }
    if (arg1 == 3) {
        menu_set(arg0, 3, (s32) &sp60[0], (s32) D_800B40F0, (s32) &sp58[0], (s32) &sp50[0]);
        return;
    }
    menu_set(arg0, 2, (s32) &sp60[0], (s32) D_800B40F0, (s32) &sp58[0], (s32) &sp50[0]);
}
