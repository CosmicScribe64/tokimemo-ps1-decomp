s32 func_8005F730(void) {
    s32 i;
    s16 positions_y[5];
    s16 positions_x[5];
    s16 widths[5];
    s16 heights[5];

    func_8004E58C();
    func_8006612C("カーソル速度");
    func_8005DEA0(4);
    for (i = 0; i < 5; i++) {
        positions_y[i] = -4;
        positions_x[i] = i * 16 - 0x40;
        widths[i] = 0x6C;
        heights[i] = 0x10;
    }
    if (((u8) D_800E7D2C >= 9U) || ((u8) D_800E6280.unk_53C[8] >= 2U)) {
        set_kanji_string(positions_y[0], positions_x[0], 3U, "超遅い", 0);
        set_kanji_string(positions_y[1], positions_x[1], 3U, "遅い", 0);
        set_kanji_string(positions_y[2], positions_x[2], 3U, "普通", 0);
        set_kanji_string(positions_y[3], positions_x[3], 3U, "速い", 0);
        set_kanji_string(positions_y[4], positions_x[4], 3U, "超速い", 0);
    } else {
        set_kanji_string(positions_y[0], positions_x[0], 3U, "遅い", 0);
        set_kanji_string(positions_y[1], positions_x[1], 3U, "少し遅い", 0);
        set_kanji_string(positions_y[2], positions_x[2], 3U, "普通", 0);
        set_kanji_string(positions_y[3], positions_x[3], 3U, "少し速い", 0);
        set_kanji_string(positions_y[4], positions_x[4], 3U, "速い", 0);
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "カーソルの移動速度を設定します", 0);
    menu_set(1, 5, positions_y, positions_x, widths, heights);
    switch (D_800E6280.unk_F6F) {
    default:
        D_800E6280.unk_1094 = 0;
        break;
    case 2:
        D_800E6280.unk_1094 = 1;
        break;
    case 3:
        D_800E6280.unk_1094 = 2;
        break;
    case 4:
        D_800E6280.unk_1094 = 3;
        break;
    case 6:
        D_800E6280.unk_1094 = 4;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}
