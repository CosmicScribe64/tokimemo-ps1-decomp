void func_8013554C(void) {
    GameState *g = &D_800E6280;
    s32 i;
    u8 t;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    t = g->unk_110D;
    switch (t) {
    case 0:
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 2;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        g->unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        break;
    case 1:
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            k_sub_reset(0x1000);
            for (i = 0; i < 4; i++) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0xA0;
                d[i] = 0x10;
                set_kanji_string(a[i] - 4, b[i], 0, (s32)D_8013D350[i], 0);
            }
            menu_set(1, 4, a, b, c, d);
            switch (g->unk_1092 & 3) {
            case 0:
                ((DisplayActor44 *)D_80120650)[11].unk_18--;
                g->unk_1094 = 0;
                break;
            case 1:
                ((DisplayActor44 *)D_80120650)[12].unk_18--;
                g->unk_1094 = 1;
                break;
            case 3:
                ((DisplayActor44 *)D_80120650)[13].unk_18--;
                g->unk_1094 = 2;
                break;
            case 2:
                ((DisplayActor44 *)D_80120650)[14].unk_18--;
                g->unk_1094 = 3;
                break;
            }
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　カーソルが、方向キーの上下左右に", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"反応して動きます。", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"（マウスと同等です）", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"メニューアイテムの順番通りにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"その方向のメニューアイテムにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　通常は、マウスライクと同じ動作を", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"します。Ｌ１ボタンを押している間は、", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"メニューＡと同じ動作をします", 0);
            k_sub_disp_start(0x10);
            for (i = 0; i < 12; i++) {
                k_disp_switch(i + 0xC, 0);
            }
            func_8004284C();
        }
        break;
    }
    func_801324E8();
}
