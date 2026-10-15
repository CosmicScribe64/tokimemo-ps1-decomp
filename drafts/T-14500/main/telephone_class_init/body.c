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
            *var_s1 += 0x10;
            set_kanji_string(*var_s2, *var_s1, 3U, "取り消し", 0);
        } else if (var_s0 == 0xC) {
            set_kanji_string(*var_s2, *var_s1, 3U, D_800B3568, 0);
        } else if (((u32) (*(&D_800E6410 + (var_s0 * 0x38)) << 0x1C) >> 0x1F) != 0) {
            set_kanji_string(*var_s2, *var_s1, 3U, *(&D_800B3534 + (var_s0 * 4)), 0);
        } else {
            *var_s3 = 0;
        }
        var_s0 += 1;
        var_s2 += 2;
        var_s1 += 2;
        var_s3 += 2;
        var_s4 += 2;
    } while (var_s0 != 0xE);
    menu_set(0, 0xE, (s32) &spBC, (s32) &spA0, (s32) &sp84, (s32) &sp68);
    D_800E7313 = 0;
    func_80048F64(0x60);
    D_80120651 = 0;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120666 = 0;
    D_80120656 = 2;
    D_80120668 = 0x3F;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
}
