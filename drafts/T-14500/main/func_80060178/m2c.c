extern ? D_800E67BC;
extern s8 D_800E7314;
extern ? D_800E7D24;

void func_80060178(void) {
    s16 spC0;
    s16 spA4;
    s16 sp88;
    s16 sp6C;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_s3;
    s16 *var_s4;
    s32 var_s0;

    func_8004E58C();
    func_8006612C((s32) "マスコット");
    func_8005DEA0(6);
    var_s0 = 0;
    var_s1 = &spC0;
    var_s2 = &spA4;
    var_s3 = &sp88;
    var_s4 = &sp6C;
    do {
        *var_s3 = 0x28;
        *var_s4 = 0x10;
        *var_s1 = ((var_s0 % 3) * 0x2B) - 8;
        *var_s2 = ((var_s0 / 3) * 0x10) - 0x40;
        if (var_s0 == 0xD) {
            *var_s3 = 0x46;
            set_kanji_string(*var_s1, *var_s2, 5U, "\x95\\示しない", 0);
        } else if ((*(&D_800E7D24 + var_s0) != 0) || (*(&D_800E67BC + var_s0) != 0)) {
            set_kanji_string(*var_s1, *var_s2, 3U, girl_short_name_table[var_s0], 0);
        } else {
            *var_s3 = 0;
        }
        var_s0 += 1;
        var_s1 += 2;
        var_s2 += 2;
        var_s3 += 2;
        var_s4 += 2;
    } while (var_s0 != 0xE);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "マスコットを選びます", 0);
    menu_set(1, 0xE, (s32) &spC0, (s32) &spA4, (s32) &sp88, (s32) &sp6C);
    if ((D_800B5C08 == 0) || (D_800B5C08 >= 0xE)) {
        D_800E7314 = 0;
    } else {
        D_800E7314 = (s8) D_800B5C08;
    }
    k_sub_disp_start(0);
    func_8004284C();
}
