s32 func_8005F730(void) {
    s32 sp6A;
    s32 sp5E;
    s32 sp6E;
    s32 sp60;
    s32 sp6C;
    s32 sp64;
    s32 sp70;
    s32 sp62;
    s16 sp68;
    s16 sp5C;
    s16 sp50;
    s16 sp44;
    s16 *temp_a0;
    s16 *temp_a2;
    s16 *temp_a3;
    s16 *temp_v1;
    s32 temp_a1;

    func_8004E58C();
    func_8006612C((s32) "カー\x83\\ル速度");
    func_8005DEA0(4);
    sp44 = 0x10;
    sp50 = 0x6C;
    sp5C = -0x40;
    sp68 = -4;
    temp_v1 = &(&sp68)[1];
    temp_a0 = &(&sp5C)[1];
    temp_a1 = 1 * 0x10;
    temp_a2 = &(&sp50)[1];
    temp_a3 = &(&sp44)[1];
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
        set_kanji_string(sp68, sp5C, 3U, "超遅い", 0);
        set_kanji_string(sp6A, sp5E, 3U, "遅い", 0);
        set_kanji_string(sp6C, sp60, 3U, "普通", 0);
        set_kanji_string(sp6E, sp62, 3U, "速い", 0);
        set_kanji_string(sp70, sp64, 3U, "超速い", 0);
    } else {
        set_kanji_string(sp68, sp5C, 3U, "遅い", 0);
        set_kanji_string(sp6A, sp5E, 3U, "少し遅い", 0);
        set_kanji_string(sp6C, sp60, 3U, "普通", 0);
        set_kanji_string(sp6E, sp62, 3U, "少し速い", 0);
        set_kanji_string(sp70, sp64, 3U, "速い", 0);
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "カー\x83\\ルの移動速度を設定します", 0);
    menu_set(1, 5, (s32) &sp68, (s32) &sp5C, (s32) &sp50, (s32) &sp44);
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
