void func_801346FC(void) {
    s16 text_x;
    s16 panel_x;
    s32 index_home; /* FAKE: preserves the original table offsets after index inlining. T-10890. */
    TaiikuS11 sprites;
    TaiText11 first_text;
    TaiText11 second_text;
    TaiWords11 delta_x;
    TaiWords11 delta_y;

    sprites = D_801493FC;
    first_text = D_80149414;
    second_text = D_80149440;
    delta_x = D_8014946C;
    delta_y = D_80149498;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_800867CC();
        if ((D_80122EA0 == 1) && (D_801491D4 == 0)) {
            D_801491D4 = 1;
            func_80044750(0x1605);
            func_80044750(0x700);
        }
        func_801368B8();
        D_80149208.player.unk_18 = ((s32) D_801491E8 >> 1) + 1;
        D_80149208.player.unk_10 = 1;
        func_80136BB4();
        func_80136E80();
        func_80137C44();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(((s16) D_80149298.halves.high >> 1) & 0x7F, 0xF, 0x70, 0x80, 0x80);
        func_80132E20((s32) D_80149298.halves.high, 3);
        func_80137EA8();
        func_80137440();
        func_801365C8();
        D_80149298.word += 0x18000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26--;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < -0xAF) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xB0;
        }
        if (D_80149298.halves.high >= 0x280) {
            func_80044750(0x606);
            ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x87;
            ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0xA4;
            ((DisplayActor44 *)D_8011ECD0)[106].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[104].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[104].unk_03 = 0xA4;
            ((DisplayActor44 *)D_8011ECD0)[105].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[105].unk_03 = 0xA4;
            ((DisplayActor44 *)D_8011ECD0)[105].unk_18 = sprites.v[D_801491EC];
            set_kanji_string((s16) (((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x40), 0x40, 0, first_text.value[D_801491EC], 0);
            set_kanji_string((s16) (((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x48), 0x50, 0, second_text.value[D_801491EC], 0);
            k_disp_start(2);
            text_x = ((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x30;
            ((DisplayActor44 *)D_8011ECD0)[105].unk_26 = text_x + 0x50;
            ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = text_x;
            ((Entry8 *)get_k_work(0))->unk_00 = ((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x40;
            ((Entry8 *)get_k_work(1))->unk_00 = ((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x48;
            func_8004284C();
        }
        break;
    case 1:
        if ((D_80122EA0 != 0) && (D_801491D4 == 0)) {
            D_801491D4 = 1;
            func_80044750(0x1605);
            func_80044750(0x700);
        }
        func_801368B8();
        func_80136E80();
        func_80137C44();
        func_80137544();
        func_801365C8();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
        func_80132E20((s32) D_80149298.halves.high, 3);
        func_80137EA8();
        if (D_80149208.player.unk_01 != 0) {
            func_8004284C();
        }
        break;
    case 2:
        func_801368B8();
        func_80136E80();
        func_80137C44();
        func_801365C8();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
        func_80137EA8();
        if (func_80137784(0) != 0) {
            D_80149208.player.unk_01 = 0U;
            D_80149208.player.unk_00 = 1;
            if (D_80122E94 == 1) {
                D_801491E8 += delta_x.value[D_80149320[D_801491DC]];
                D_801491E4 += delta_y.value[D_80149320[D_801491DC]];
            } else {
                D_801491E8 += delta_x.value[D_801491EC];
                D_801491E4 += delta_y.value[D_801491EC];
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
