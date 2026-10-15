s32 func_80132114(void) {
    s16 spA4;
    s16 sp94;
    s16 sp84;
    s16 sp74;
    s16 *var_s0;
    s16 *var_s1;
    s16 *var_s4;
    s16 *var_s5;
    s16 var_s3;
    s32 *var_s2;

    func_8004E58C();
    func_80048EB8(0);
    back_clear_switch(1);
    D_800E62B7 = 0xA0;
    D_800E62B6 = 0xA0;
    D_800E62B8 = 0xA0;
    func_8006612C((s32) "Ｑ＆Ａコーナー");
    set_kanji_string(-0x68, 0x5C, 2, (s32) "スタートボタンでゲームに戻れます", 0);
    k_sub_reset_point_set();
    var_s2 = &D_80139EE0;
    var_s0 = &spA4;
    var_s1 = &sp94;
    var_s3 = -0x3C;
    var_s4 = &sp84;
    var_s5 = &sp74;
    do {
        *var_s0 = -0x60;
        *var_s1 = var_s3;
        *var_s4 = 0x80;
        *var_s5 = 0x10;
        set_kanji_string(*var_s0, *var_s1, 0xF, *var_s2, 0);
        var_s2 += 4;
        var_s0 += 2;
        var_s1 += 2;
        var_s3 += 0x10;
        var_s4 += 2;
        var_s5 += 2;
    } while (var_s2 != &D_80139F00);
    menu_set(0, 8, &spA4, &sp94, &sp84, &sp74);
    k_disp_start(set_kanji_string(-0x10, -0x50, 0xF, (s32) "項目を選んでください", 0));
    func_80042808();
}
