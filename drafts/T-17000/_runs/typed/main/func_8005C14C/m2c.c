u8 func_8005C14C(void) {
    u8 temp_t0;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_t9;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    switch (var_v0) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xFU, "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        temp_t7 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t7;
        D_800E6280.unk_110D = temp_t7;
    default:
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            temp_t9 = (var_v0 + 1) & 0xFF;
            var_v0 = temp_t9;
            D_800E6280.unk_110D = temp_t9;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xFU, "初期化していいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, func_8006CAE0(), 0);
        k_sub_disp_start(5);
        temp_t0 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t0;
        D_800E6280.unk_110D = temp_t0;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            if (func_80054108() == 1) {
                var_v0 = func_80042940(0);
            } else {
                func_8005C4CC(1);
                func_80042908(0);
                var_v0 = func_80042940(2);
            }
        } else if (D_800E6280.unk_F88 & 0x40) {
            temp_t3 = (var_v0 + 1) & 0xFF;
            var_v0 = temp_t3;
            D_800E6280.unk_110D = temp_t3;
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xFU, "別のメモリーカードを", 0);
        set_kanji_string(-0x80, 0x42, 1U, "差し込み口１", 0);
        set_kanji_string(-0x80, 0x42, 0xFU, "　　　　　　にセットして、", 0);
        set_kanji_string(-0x80, 0x52, 0xFU, "○ボタンを押してください。", 0);
        k_sub_disp_start(4);
        temp_t4 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t4;
        D_800E6280.unk_110D = temp_t4;
        break;
    case 5:
        if (D_800E6280.unk_F88 & 0x20) {
            func_8005C4CC(1);
            func_80042908(0);
            var_v0 = func_80042940(2);
        }
        break;
    }
    return var_v0;
}
