/* Written in the shape of the NAME_ENT sibling func_80142028 (T-17300). Like the sibling it is s32
 * with no return statement: the incremented step stays in v0 on the way out. */
s32 func_80145318(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053D10();
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D++;
    default:
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "別のメモリーカードに", 0);
        set_kanji_string(-0x80, 0x42, 1, "セーブしますか？", 0);
        set_kanji_string(-0x18, 4, 0xF, D_801500F0, 0);
        set_kanji_string(0x3C, 4, 1, func_80140F50(), 0);
        set_kanji_string(-0x18, 0x14, 0xF, D_8015010C, 0);
        set_kanji_string(0x3C, 0x14, 2, func_80140F94(), 0);
        k_sub_disp_start(6);
        D_800E6280.unk_110D++;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            D_800E6280.unk_110D++;
        } else if (D_800E6280.unk_F88 & 0x40) {
            func_80042908(2);
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "別のメモリーカードを", 0);
        set_kanji_string(-0x80, 0x42, 1, "差し込み口１にセットして、", 0);
        set_kanji_string(-0x80, 0x52, 1, "ボタンを押してください。", 0);
        k_sub_disp_start(3);
        D_800E6280.unk_110D++;
        break;
    case 5:
        if (D_800E6280.unk_F88 != 0) {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    }
}
