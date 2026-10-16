void menu_girl_taku_set(s32 arg0, s32 *arg1) {
    s16 spB4;
    s16 sp9C;
    s16 sp84;
    s16 sp6C;
    s16 *var_s2;
    s16 *var_s4;
    s16 *var_s5;
    s16 *var_s6;
    s32 var_s0;
    s32 var_v0;

    var_s0 = 0;
    var_s2 = &sp84;
    var_s4 = &spB4;
    var_s5 = &sp9C;
    var_s6 = &sp6C;
    do {
        if (var_s0 == 0xB) {
            var_v0 = 0xC;
        } else {
            var_v0 = var_s0;
        }
        if (((u32) (D_800E6280.unk_1BC[var_v0].unk_0C.w << 0x1E) >> 0x1F) != 0) {
            *arg1 = set_kanji_string((s16) (((var_s0 / 3) << 6) - 0x80), (s16) (((var_s0 % 3) * 0x10) + 0x32), 0U, girl_short_name_table[var_v0], 0);
        }
        *var_s4 = ((var_s0 / 3) << 6) - 0x80;
        *var_s5 = ((var_s0 % 3) * 0x10) + 0x32;
        if (((u32) (D_800E6280.unk_1BC[var_v0].unk_0C.w << 0x1E) >> 0x1F) != 0) {
            *var_s2 = 0x30;
        } else {
            *var_s2 = 0;
        }
        *var_s6 = 0x10;
        menu_set(arg0, 0xC, (s32) &spB4, (s32) &sp9C, (s32) &sp84, (s32) &sp6C);
        var_s0 += 1;
        var_s4 += 2;
        var_s5 += 2;
        var_s2 += 2;
        var_s6 += 2;
    } while (var_s0 != 0xC);
}
