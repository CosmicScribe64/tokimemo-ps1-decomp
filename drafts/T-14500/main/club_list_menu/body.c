void club_list_menu(void) {
    s32 pad[2];
    s32 first;
    s32 i;
    s16 x[12];
    s16 y[12];
    s16 w[12];
    s16 h[12];

    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x3F;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
    set_kanji_string(-0x80, -0x52, 3, "部活一覧", 0);
    first = -1;
    for (i = 0; i < 0xB; i++) {
        x[i] = (i / 5) * 0x54 - 0x80;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x40;
        h[i] = 0x10;
        if (!(D_800E6280.unk_66C[i].unk_03 & 1)) {
            set_kanji_string(x[i], y[i], 3, club_name_table[i], 0);
            if (first == -1) {
                first = i;
            }
        } else {
            w[i] = 0;
        }
    }
    w[11] = 0x40;
    h[11] = 0x10;
    set_kanji_string(x[11] = 0x28, y[11] = 8, 3, "取り消し", 0);
    menu_set(1, 0xC, x, y, w, h);
    D_800E6280.unk_1094 = first;
    k_sub_reset_point_set();
    sndi("入部する部活を選択してください", 0, 0xFF);
    k_sub_disp_start(1);
}
