void func_8015A8B4(void) {
    DisplayActor44 *var_s0;
    s16 temp_v1;
    s32 temp_lo;
    s32 var_s1;
    s32 var_t7;

    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1 = 0;
    do {
        temp_v1 = var_s0[136].unk_26;
        if ((temp_v1 < -0xB4) || (var_t7 = 0x70 - temp_v1, ((var_s0[136].unk_2A < 0x8D) == 0))) {
            func_8013E810(var_s1 + 0x28, 7, 1, 0);
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
