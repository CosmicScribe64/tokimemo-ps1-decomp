? func_8004BBA8(? *, ?, ?, ?, s32);                 /* extern */
? func_8004EBEC(?);                                 /* extern */
u8 func_8004EC14();                                 /* extern */
extern s8 D_800E62BA;
extern u8 D_800E62BC;
extern u8 D_800E71EC;
extern u8 D_800E71EF;
extern u8 D_800E71F0;
extern u8 D_800E71F3;
extern u8 D_800E71F5;
extern u8 D_800E7312;
extern s32 D_800E7318;
extern s8 D_800E7322;
extern u8 D_800E738D;
extern u8 D_800E73A4;
extern ? D_800E78BC;

void func_8013BB24(void) {
    u8 temp_v0;

    switch (D_800E738D) {                           /* irregular */
    case 0:
        D_800B3C6C = 0;
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_80041584();
        D_800E7318 = 0xA0;
        func_80048E78();
        if (D_800E73A4 != 0) {
            func_800AE0B0("option setting!");
            D_8014D0AC = (s32) D_800E73A4;
            temp_v0 = func_8004EC14();
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
        func_8004EBEC(1);
        func_800438F0(1);
        func_8006764C(0);
        func_80048390();
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
        func_8004BBA8(&D_800E78BC, 0xDF7FCF, 0xDF7FCF, 0x10, 0);
        func_80043914(&D_800E78BC, 0x17, 1, 1, 0);
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
