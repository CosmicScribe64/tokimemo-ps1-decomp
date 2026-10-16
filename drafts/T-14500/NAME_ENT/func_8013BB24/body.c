void func_8013BB24(void) {
    u8 speed;

    switch (D_800E6280.unk_110D) {
    case 0:
        D_800B3C6C = 0;
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_80041584();
        *(s32 *)&D_800E6280.unk_1095[3] = 0xA0;
        func_80048E78();
        if (D_800E6280.unk_1124 != 0) {
            func_800AE0B0("option setting!");
            D_8014D0AC = D_800E6280.unk_1124;
            speed = get_k_speed();
            D_8014D0B0 = speed; func_800AE0B0("kanji_speed%d \n", speed);
            D_8014D0B4 = *(u8 *)&D_800E6280.unk_F6C;
            func_800AE0B0("icon_mode %d \n", *(u8 *)&D_800E6280.unk_F6C);
            D_8014D0B8 = *(u8 *)&D_800E6280.unk_1092;
            func_800AE0B0("menu_mode %d \n", *(u8 *)&D_800E6280.unk_1092);
            D_8014D0BC = *(u8 *)&D_800E6280.unk_F6F;
            func_800AE0B0("cursor_speed %d \n", *(u8 *)&D_800E6280.unk_F6F);
            D_8014D0C0 = *(u8 *)&D_800E6280.unk_03C;
            func_800AE0B0("sound_mode %d \n", *(u8 *)&D_800E6280.unk_03C);
            D_8014D0C4 = *(u8 *)&D_800E6280.unk_F70;
            func_800AE0B0("click_mode %d \n", *(u8 *)&D_800E6280.unk_F70);
            D_8014D0C8 = *(u8 *)&D_800E6280.unk_F73;
            func_800AE0B0("click_speed %d \n", *(u8 *)&D_800E6280.unk_F73);
            D_8014D0CC = *(u8 *)&D_800E6280.unk_F75;
            func_800AE0B0("mouse_ok %d \n", *(u8 *)&D_800E6280.unk_F75);
        } else {
            D_8014D0AC = 0;
        }
        D_800E6280.unk_1124 = 0;
        D_8014D0A8 = 1;
        D_8014D0A4 = 0;
        srand(D_800E7D10);
        k_speed_set(1);
        back_clear_switch(1);
        func_8006764C(0);
        tpage_buf_clear();
        D_800E6280.unk_10A2 = 0;
        func_80065F34(0);
        func_8004E58C();
        text_set_advance(0xE);
        D_800E6280.unk_03A = 0;
        D_801230D0 = 0xFF;
        func_8006BC28(0);
        func_8006BD6C(0);
        D_8014D0E0 = 1;
        D_8014D0E4 = -0x50;
        D_8014D0E8 = -0x50;
        D_8014D0EC = 8;
        D_8014D0F0 = 8;
        make_color_bar16(&D_800E6280.unk_163C, 0xDF7FCF, 0xDF7FCF, 0x10, 0);
        load_palette(&D_800E6280.unk_163C, 0x17, 1, 1, 0);
        func_80046754(0x80180000, 0x1A, 0);
        func_80044890(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004284C();
        }
        break;
    }
}
