void func_8013554C(void) {
    s16 sp90;
    s16 sp84;
    s16 sp78;
    s16 sp6C;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_s5;
    s16 *var_s6;
    s16 temp_t0;
    s16 var_s4;
    s32 *var_s3;
    s32 temp_t4;
    s32 var_a0;
    s32 var_s0;

    switch (D_800E738D) {
    case 0:
        D_80120910 = 2;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E738D = 1;
        *D_80120912 = 0x7FFF;
        break;
    case 1:
        temp_t0 = ((s16) *D_80120912 / 17) * 0x10;
        D_80120914 = ((s16) D_80120914 / 17) * 0x10;
        *D_80120912 = temp_t0;
        if (temp_t0 < 0x1001) {
            D_80120914 = 0x1000;
            *D_80120912 = 0x1000;
            k_sub_reset(0x1000);
            var_s3 = &D_8013D350;
            var_s1 = &sp90;
            var_s2 = &sp84;
            var_s4 = -0x38;
            var_s5 = &sp78;
            var_s6 = &sp6C;
            do {
                *var_s1 = -0x20;
                *var_s2 = var_s4;
                *var_s5 = 0xA0;
                *var_s6 = 0x10;
                set_kanji_string((s16) (*var_s1 - 4), *var_s2, 0, *var_s3, 0);
                var_s3 += 4;
                var_s1 += 2;
                var_s2 += 2;
                var_s4 += 0x18;
                var_s5 += 2;
                var_s6 += 2;
            } while ((u32) var_s3 < (u32) &D_8013D360);
            menu_set(1, 4, &sp90, &sp84, &sp78, &sp6C);
            temp_t4 = D_800E7312 & 3;
            switch (temp_t4) {
            case 0:
                D_80120954 -= 1;
                D_800E7314 = 0;
                break;
            case 1:
                D_80120998 -= 1;
                D_800E7314 = 1;
                break;
            case 3:
                D_801209DC -= 1;
                D_800E7314 = 2;
                break;
            case 2:
                D_80120A20 -= 1;
                D_800E7314 = 3;
                break;
            }
            set_kanji_string(-0x80, 0x40, 0xF, (s32) "　カー\x83\\ルが、方向キーの上下左右に", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32) "反応して動きます。", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32) "（マウスと同等です）", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32) "　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32) "メニューアイテムの順番通りにカー\x83\\ル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32) "が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32) "　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32) "その方向のメニューアイテムにカー\x83\\ル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32) "が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32) "　通常は、マウスライクと同じ動作を", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32) "します。Ｌ１ボタンを押している間は、", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32) "メニューＡと同じ動作をします", 0);
            k_sub_disp_start(0x10);
            var_s0 = 0;
            var_a0 = 0xC;
            do {
                k_disp_switch(var_a0, 0);
                var_s0 += 1;
                var_a0 = var_s0 + 0xC;
            } while (var_s0 != 0xC);
            func_8004284C();
        }
        break;
    }
    func_801324E8();
}
