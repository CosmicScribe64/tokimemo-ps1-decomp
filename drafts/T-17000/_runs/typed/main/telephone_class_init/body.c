void telephone_class_init(void) {
    s16 spBC;
    s16 spA0;
    s16 sp84;
    s16 sp68;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_s3;
    s16 *var_s4;
    s32 var_s0;

    set_kanji_string(-0x80, -0x54, 3U, "電話番号", 0);
    var_s0 = 0;
    var_s2 = &spBC;
    var_s1 = &spA0;
    var_s3 = &sp84;
    var_s4 = &sp68;
    do {
        *var_s3 = 0x48;
        *var_s4 = 0xE;
        *var_s2 = ((var_s0 / 5) * 0x58) - 0x88;
        *var_s1 = ((var_s0 % 5) * 0x10) - 0x38;
        if (var_s0 == 0) {
            set_kanji_string(*var_s2, *var_s1, 3U, D_800B356C, 0);
        } else if (var_s0 == 0xD) {
            *var_s1 += 8;
            set_kanji_string(*var_s2, *var_s1, 3U, "取り消し", 0);
        } else if (var_s0 == 0xC) {
            set_kanji_string(*var_s2, *var_s1, 3U, D_800B3568, 0);
        } else if (((u32) (*(&D_800E6280.unk_120[0x70] + (var_s0 * 0x38)) << 0x1C) >> 0x1F) != 0) {
            set_kanji_string(*var_s2, *var_s1, 3U, *(&D_800B3534 + (var_s0 * 4)), 0);
        } else {
            *var_s3 = 0;
        }
        var_s0 += 1;
        var_s2 += 1;
        var_s1 += 1;
        var_s3 += 1;
        var_s4 += 1;
    } while (var_s0 != 0xE);
    menu_set(0, 0xE, (s32) &spBC, (s32) &spA0, (s32) &sp84, (s32) &sp68);
    D_800E6280.unk_1093 = 0;
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x40000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6540)) = sprite_frame_table;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6544)) = D_800C9A14;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6580)) = D_800C9730;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_800C9A54;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 2;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x3F;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x13;
}
