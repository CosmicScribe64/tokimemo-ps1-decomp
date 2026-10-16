? func_8004E93C(s32, ?);                            /* extern */

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
    s32 temp_t4;
    s32 var_a0;
    s32 var_s0;
    u8 **var_s3;

    switch (D_800E6280.unk_110D) {                  /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 2;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x7FFF;
        break;
    case 1:                                         /* switch 1 */
        temp_t0 = ((s16) D_8011ECD0->unk0 / 17) * 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) (((s16) D_8011ECD0->unk0 / 17) * 0x10);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t0;
        if (temp_t0 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x1000;
            func_8004EAAC(0x1000);
            var_s3 = D_8013D350;
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
                func_8004E788((s16) (*var_s1 - 4), *var_s2, 0, (s32) *var_s3, 0);
                var_s3 += 4;
                var_s1 += 2;
                var_s2 += 2;
                var_s4 += 0x18;
                var_s5 += 2;
                var_s6 += 2;
            } while ((u32) var_s3 < (u32) &D_8013D360);
            func_8004F870(1, 4, &sp90, &sp84, &sp78, &sp6C);
            temp_t4 = D_800E6280.unk_1092 & 3;
            switch (temp_t4) {                      /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                *(s16 *)((unsigned char *)D_8011ECD0 + (7300)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7300)) - 1);
                D_800E6280.unk_1094 = 0;
                break;
            case 1:                                 /* switch 2 */
                *(s16 *)((unsigned char *)D_8011ECD0 + (7368)) = (s16) (D_8011ECD0->unk0 - 1);
                D_800E6280.unk_1094 = 1;
                break;
            case 3:                                 /* switch 2 */
                *(s16 *)((unsigned char *)D_8011ECD0 + (7436)) = (s16) (D_8011ECD0->unk0 - 1);
                D_800E6280.unk_1094 = 2;
                break;
            case 2:                                 /* switch 2 */
                *(s16 *)((unsigned char *)D_8011ECD0 + (7504)) = (s16) (D_8011ECD0->unk0 - 1);
                D_800E6280.unk_1094 = 3;
                break;
            }
            func_8004E788(-0x80, 0x40, 0xF, (s32) "　カー\x83\\ルが、方向キーの上下左右に", 0);
            func_8004E788(-0x80, 0x50, 0xF, (s32) "反応して動きます。", 0);
            func_8004E788(-0x80, 0x60, 0xF, (s32) "（マウスと同等です）", 0);
            func_8004E788(-0x80, 0x40, 0xF, (s32) "　メニュー選択時に、方向キーを押すと", 0);
            func_8004E788(-0x80, 0x50, 0xF, (s32) "メニューアイテムの順番通りにカー\x83\\ル", 0);
            func_8004E788(-0x80, 0x60, 0xF, (s32) "が移動します", 0);
            func_8004E788(-0x80, 0x40, 0xF, (s32) "　メニュー選択時に、方向キーを押すと", 0);
            func_8004E788(-0x80, 0x50, 0xF, (s32) "その方向のメニューアイテムにカー\x83\\ル", 0);
            func_8004E788(-0x80, 0x60, 0xF, (s32) "が移動します", 0);
            func_8004E788(-0x80, 0x40, 0xF, (s32) "　通常は、マウスライクと同じ動作を", 0);
            func_8004E788(-0x80, 0x50, 0xF, (s32) "します。Ｌ１ボタンを押している間は、", 0);
            func_8004E788(-0x80, 0x60, 0xF, (s32) "メニューＡと同じ動作をします", 0);
            func_8004EAD4(0x10);
            var_s0 = 0;
            var_a0 = 0xC;
            do {
                func_8004E93C(var_a0, 0);
                var_s0 += 1;
                var_a0 = var_s0 + 0xC;
            } while (var_s0 != 0xC);
            func_8004284C();
        }
        break;
    }
    func_801324E8();
}
