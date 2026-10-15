void func_80132BA8(void) {
    s32 sp4E;
    s32 sp3E;
    s32 sp5A;
    s32 sp4A;
    s32 sp58;
    s32 sp48;
    s32 sp56;
    s32 sp46;
    s32 sp54;
    s32 sp44;
    s32 sp52;
    s32 sp42;
    s32 sp50;
    s32 sp40;
    s32 sp6C[1];
    s32 sp5C[1];
    s16 sp4C;
    s16 sp3C;

    switch ((u8) D_800E738D) {
    case 0x0:
        D_800E73A4 = 1;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_80041584();
        func_80048E78();
        D_800E7318 = 0xA0;
        back_clear_switch(1);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_800E62B8 = 0;
        tpage_buf_clear();
        D_800E7322 = 0;
        func_8004E58C();
        D_800E7313 = 0;
        D_800E7314 = 0;
        func_8006BC28(0);
        D_800E62BA = 0xFF;
        D_8013D300 = 0;
        D_800B3220 = 0;
        D_801230D0 = 0;
        text_set_advance(0xE);
        func_801325A0(&sp3C);
        menu_set(0, 8, &sp3C, &sp4C, &sp5C, &sp6C);
        set_kanji_string(sp3C, sp4C, 0, "文字\x95\\示速度", 0);
        set_kanji_string(sp3E, sp4E, 0, "アイコンモード", 0);
        set_kanji_string(sp40, sp50, 0, "マウス設定", 0);
        set_kanji_string(sp42, sp52, 0, "メニューモード", 0);
        set_kanji_string(sp44, sp54, 0, "カー\x83\\ル速度", 0);
        set_kanji_string(sp46, sp56, 0, "音声モード", 0);
        set_kanji_string(sp48, sp58, 0, "おまけ", 0);
        set_kanji_string(sp4A, sp5A, 0, "オプション終了", 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        D_80120668 = 0;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        D_800E738D = 1;
        func_80053CE0();
        break;
    case 0x1:
        if (func_80053F04() != 0) {
            if (D_800E7395 == 1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xF1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xFF) {
                D_800E738D += 1;
            }
        } else if ((u32) D_800E73A0 >= 0x201U) {
            D_800E738D += 1;
        }
        break;
    case 0x2:
        func_80053D10();
        if (D_800E7395 == 1) {
            if (func_80054AF4(0) == 1) {
                D_800E738D += 1;
                func_80053CE0();
            } else {
                D_800E738D = 0xFF;
            }
            if (func_80054AF4(0xF) == 1) {
                D_800B3220 = 1;
            } else {
                D_800B3220 = 0;
            }
        } else {
            D_800E738D = 0xFF;
        }
        break;
    case 0x3:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            D_800E738D = 0xFF;
        }
        break;
    case 0x4:
        if (func_800552DC(0) == -1) {
            D_800E738D = 0xFF;
        } else {
            D_800E738D = 0xFF;
        }
        break;
    default:
        cd_read_sectors(0x3D, 0x801B0000, 0x9EA4);
        func_8004284C();
        break;
    }
    func_8004AC18(D_800E62BA);
}
