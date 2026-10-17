s32 func_8005CC40(void) {
    s32 temp_v0;
    u32 temp_hi;
    u8 temp_a0;
    u8 temp_v1;
    u8 *temp_v1_2;

    temp_v1 = D_800E6280.unk_110D;
    switch (temp_v1) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "上書きしてもいいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, func_8006CAE0(), 0);
        k_sub_disp_start(5);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 1:
        temp_v0 = D_800E6280.unk_F88;
        if (temp_v0 & 0x20) {
            D_800E6280.unk_110D = temp_v1 + 1;
        } else if (temp_v0 & 0x40) {
            func_80042940(3);
            temp_a0 = D_800E6280.unk_1118;
            if (D_800E7D10.slot_status[temp_a0] == 0xFF) {
                D_800E7D10.slot_status[temp_a0] = 0;
            }
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "データセーブ中です", 0);
        if ((u8) D_800E8BEE != 0) {
            set_kanji_string(-0x80, 0x42, 1U, "アイコンの点滅が終わるまで", 0);
            set_kanji_string(-0x80, 0x52, 1U, "電源を切らないで下さい", 0);
        }
        k_sub_disp_start(3);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 3:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            temp_hi = (u8) D_800E6280.unk_F5F % 13U;
            temp_v1_2 = (temp_hi * 8) + &D_800B39C4;
            func_80046290((*(s32 *)temp_v1_2), *(s32 *)((unsigned char *)temp_v1_2 + (4)), (s32) temp_hi);
            func_80044750(0x300);
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 4:
        if ((u8) D_800E8BEE != 0) {
            func_80053CE0();
            D_800E6280.unk_111B = 1;
            D_800E7D36 = D_800E6280.unk_1118;
            D_800E6280.unk_1118 = 0;
            func_80054E6C();
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        } else {
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        }
        break;
    }
    func_8005C7FC();
}
