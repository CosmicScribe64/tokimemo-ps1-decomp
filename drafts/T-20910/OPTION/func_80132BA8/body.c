void func_80132BA8(void) {
    Arrs4x8 sp3C;

    switch ((u8) D_800E6280.unk_110D) {
    case 0x0:
        D_800E6280.unk_1124 = 1;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_80041584();
        func_80048E78();
        *(s32 *)((unsigned char *)(&D_800E6280) + (4248)) = 0xA0;
        back_clear_switch(1);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_038 = 0;
        tpage_buf_clear();
        D_800E6280.unk_10A2 = 0;
        func_8004E58C();
        D_800E6280.unk_1093 = 0;
        D_800E6280.unk_1094 = 0;
        func_8006BC28(0);
        D_800E6280.unk_03A = 0xFF;
        D_8013D300 = 0;
        D_800B3220 = 0;
        D_801230D0 = 0;
        text_set_advance(0xE);
        func_801325A0(&sp3C);
        menu_set(0, 8, sp3C.a, sp3C.b, sp3C.c, sp3C.d);
        set_kanji_string(sp3C.a[0], sp3C.b[0], 0, (s32) "文字\x95\\示速度", 0);
        set_kanji_string(sp3C.a[1], sp3C.b[1], 0, (s32) "アイコンモード", 0);
        set_kanji_string(sp3C.a[2], sp3C.b[2], 0, (s32) "マウス設定", 0);
        set_kanji_string(sp3C.a[3], sp3C.b[3], 0, (s32) "メニューモード", 0);
        set_kanji_string(sp3C.a[4], sp3C.b[4], 0, (s32) "カー\x83\\ル速度", 0);
        set_kanji_string(sp3C.a[5], sp3C.b[5], 0, (s32) "音声モード", 0);
        set_kanji_string(sp3C.a[6], sp3C.b[6], 0, (s32) "おまけ", 0);
        set_kanji_string(sp3C.a[7], sp3C.b[7], 0, (s32) "オプション終了", 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        D_800E6280.unk_110D = 1;
        func_80053CE0();
        break;
    case 0x1:
        if (func_80053F04() != 0) {
            if (D_800E6280.unk_1115 == 1) {
                D_800E6280.unk_110D += 1;
            } else if (D_800E6280.unk_1115 == 0xF1) {
                D_800E6280.unk_110D += 1;
            } else if (D_800E6280.unk_1115 == 0xFF) {
                D_800E6280.unk_110D += 1;
            }
        } else if ((u32) D_800E6280.unk0 >= 0x201U) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 0x2:
        func_80053D10();
        if (D_800E6280.unk_1115 == 1) {
            if (func_80054AF4(0) == 1) {
                D_800E6280.unk_110D += 1;
                func_80053CE0();
            } else {
                D_800E6280.unk_110D = 0xFF;
            }
            if (func_80054AF4(0xF) == 1) {
                D_800B3220 = 1;
            } else {
                D_800B3220 = 0;
            }
        } else {
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 0x3:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk0 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 0x4:
        if (func_800552DC(0) == -1) {
            D_800E6280.unk_110D = 0xFF;
        } else {
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    default:
        cd_read_sectors(0x3DU, 0x801B0000, 0x9EA4);
        func_8004284C();
        break;
    }
    func_8004AC18((s32) D_800E6280.unk_03A);
}
