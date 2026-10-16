s32 func_8005F730(void) {
    s16 positions_y[5];
    s16 positions_x[5];
    s16 widths[5];
    s16 heights[5];
    s16 *temp_a0;
    s16 *temp_a2;
    s16 *temp_a3;
    s16 *temp_v1;
    s32 temp_a1;

    func_8004E58C();
    func_8006612C((s32) "カーソル速度");
    func_8005DEA0(4);
    heights[0] = 0x10;
    widths[0] = 0x6C;
    positions_x[0] = -0x40;
    positions_y[0] = -4;
    temp_v1 = &(&positions_y[0])[1];
    temp_a0 = &(&positions_x[0])[1];
    temp_a1 = 1 * 0x10;
    temp_a2 = &(&widths[0])[1];
    temp_a3 = &(&heights[0])[1];
    (*(s32 *)temp_a0) = temp_a1 - 0x40;
    *(s16 *)((unsigned char *)temp_a0 + (6)) = (s16) (temp_a1 - 0x10);
    *(s16 *)((unsigned char *)temp_a0 + (4)) = (s16) (temp_a1 - 0x20);
    *(s16 *)((unsigned char *)temp_a0 + (2)) = (s16) (temp_a1 - 0x30);
    *(s16 *)((unsigned char *)temp_a3 + (2)) = 0x10;
    *(s16 *)((unsigned char *)temp_a3 + (4)) = 0x10;
    *(s16 *)((unsigned char *)temp_a3 + (6)) = 0x10;
    *(s16 *)((unsigned char *)temp_a2 + (2)) = 0x6C;
    *(s16 *)((unsigned char *)temp_a2 + (4)) = 0x6C;
    *(s16 *)((unsigned char *)temp_a2 + (6)) = 0x6C;
    *(s16 *)((unsigned char *)temp_v1 + (2)) = -4;
    *(s16 *)((unsigned char *)temp_v1 + (4)) = -4;
    *(s16 *)((unsigned char *)temp_v1 + (6)) = -4;
    (*(s32 *)temp_v1) = -4;
    (*(s32 *)temp_a2) = 0x6C;
    (*(s32 *)temp_a3) = 0x10;
    if (((u8) D_800E7D2C >= 9U) || ((u8) D_800E67C4 >= 2U)) {
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
    menu_set(1, 5, (s32) &positions_y[0], (s32) &positions_x[0], (s32) &widths[0], (s32) &heights[0]);
    switch (D_800E71EF) {
    default:
        D_800E7314 = 0;
        break;
    case 2:
        D_800E7314 = 1;
        break;
    case 3:
        D_800E7314 = 2;
        break;
    case 4:
        D_800E7314 = 3;
        break;
    case 6:
        D_800E7314 = 4;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}
