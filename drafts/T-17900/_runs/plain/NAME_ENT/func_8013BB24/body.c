void func_8013BB24(void) {
    u8 temp_v0;

    switch (D_800E738D) {
    case 0:
        D_800B3C6C = 0;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_80041584();
        D_800E7318 = 0xA0;
        func_80048E78();
        if (D_800E73A4 != 0) {
            func_800AE0B0("option setting!");
            D_8014D0AC = (s32) D_800E73A4;
            temp_v0 = get_k_speed();
            D_8014D0B0 = temp_v0;
            func_800AE0B0("kanji_speed%d \n", temp_v0);
            D_8014D0B4 = D_800E71EC;
            func_800AE0B0("icon_mode %d \n", D_800E71EC);
            D_8014D0B8 = D_800E7312;
            func_800AE0B0("menu_mode %d \n", D_800E7312);
            D_8014D0BC = D_800E71EF;
            func_800AE0B0("cursor_speed %d \n", D_800E71EF);
            D_8014D0C0 = D_800E62BC;
            func_800AE0B0("sound_mode %d \n", D_800E62BC);
            D_8014D0C4 = D_800E71F0;
            func_800AE0B0("click_mode %d \n", D_800E71F0);
            D_8014D0C8 = D_800E71F3;
            func_800AE0B0("click_speed %d \n", D_800E71F3);
            D_8014D0CC = D_800E71F5;
            func_800AE0B0("mouse_ok %d \n", D_800E71F5);
        } else {
            D_8014D0AC = 0;
        }
        D_800E73A4 = 0;
        D_8014D0A8 = 1;
        D_8014D0A4 = 0;
        func_800AE120(D_800E7D10);
        k_speed_set(1);
        back_clear_switch(1);
        func_8006764C(0);
        tpage_buf_clear();
        D_800E7322 = 0;
        func_80065F34(0);
        func_8004E58C();
        text_set_advance(0xE);
        D_800E62BA = 0;
        D_801230D0 = 0xFF;
        func_8006BC28(0);
        func_8006BD6C(0);
        D_8014D0E0 = 1;
        D_8014D0E4 = -0x50;
        D_8014D0E8 = -0x50;
        D_8014D0EC = 8;
        D_8014D0F0 = 8;
        make_color_bar16(&D_800E78BC, 0xDF7FCF, 0xDF7FCF, 0x10, 0);
        load_palette(&D_800E78BC, 0x17, 1, 1, 0);
        func_80046754(0x80180000, 0x1A, 0);
        func_80044890(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        D_800E738D += 1;
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004284C();
        }
        break;
    }
}
