void func_8015A744(void) {
    DisplayActor44 *var_s0;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_s1;
    s32 var_t7;

    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1 = 0;
    do {
        temp_v1 = var_s0[136].unk_26;
        if ((temp_v1 < -0xB4) || (var_t7 = 0x70 - temp_v1, ((var_s0[136].unk_2A < 0x8D) == 0))) {
            if (func_8013E9A8(5) == 0) {
                func_8013F0F4(0x501, 2, 2);
                func_8013E97C(var_s1 + 0x28, 0xF0, 0x28);
                temp_a0 = func_8013E9A8(8) + 5;
                D_8015EC50 = temp_a0;
                temp_v0 = func_8013E9A8(temp_a0);
                var_s0[136].unk_03 = 0x94;
                temp_a0_2 = -D_8015EC50;
                var_s0[136].unk_2E = (s16) temp_a0_2;
                *(s16 *)((unsigned char *)var_s0 + (9298)) = (s16) temp_v0;
                D_8015EC50 = temp_a0_2;
                D_8015EC54 = temp_v0;
            } else {
                func_8013E810(var_s1 + 0x28, 7, 1, 0);
            }
            var_t7 = 0x70 - var_s0[136].unk_26;
        }
        temp_lo = (s32) (var_t7 << 0xC) / 240;
        var_s1 += 1;
        var_s0 += 1;
        var_s0[135].unk_1A = (s16) temp_lo;
        *(s16 *)((unsigned char *)var_s0 + (9208)) = (s16) temp_lo;
        D_8015EC58.unk_10 = (s32) var_s0[136].unk_26;
    } while (var_s1 != 8);
}
