#include "common.h"
#include "ovl/TEL.h"

void func_80132000(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132060();
        return;
    case 1:
        func_80134BE0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80132060(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132138();
        break;
    case 1:
        func_8013226C();
        break;
    case 2:
        func_80132FC4();
        break;
    case 3:
        func_80133D54();
        break;
    case 4:
        func_801345C4();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_80066334();
    func_8006BA40();
}

void func_80132138(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            func_8004284C();
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}

void func_8013226C(void) {
    /* FAKE: buffer extent preserves the original sp+0x3C address. T-10040. */
    u8 sp3C[36];
    u8 sp2C[16];

    switch (D_800E6280.unk_110D) {
    case 0x0:
        if (D_800E6280.unk_1BC[13].unk_0C.b[3] == 0) {
            D_800E6280.unk_110D = 1;
        } else if ((appointment_find_today((s32) D_800E6280.unk_03E, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) > 0) && (((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C) == 0xA)) {
            D_800E6280.unk_110D = 0x30;
        } else if (get_g_zyotai_h(0xA) == 4) {
            D_800E6280.unk_110D = 0x30;
        } else if (D_800E6280.unk_1BC[13].unk_0C.b[3] & 1) {
            D_800E6280.unk_110D = 0x10;
        } else {
            D_800E6280.unk_110D = 0x30;
        }
        D_800E6280.unk_1BC[13].unk_0C.b[3] += 1;
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ハイ、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x2:
        func_8005215C();
        break;
    case 0x3:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし、", 0, 0xF);
        func_800AE0F0(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4:
        xa_wait();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "おっ、");
        get_p_name(sp2C, 0xD);
        func_800AE100(sp3C, sp2C);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        sndi("良くかけて来てくれたな。", 1, 0xD);
        func_80052060((s32) &D_80144E3C, (s32) &D_80144E44, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x6:
        func_8005215C();
        break;
    case 0x7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の事なら", 0, 0xF);
        sndi("任せておけって、", 1, 0xF);
        sndi("言ってたよな…。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x8:
        xa_wait();
        break;
    case 0x9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("もちろん。", 0, 0xD);
        sndi("何について聞きたいんだ？", 1, 0xD);
        func_80052060(0x180000B8, 0x140000C1, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xA:
        func_8005215C();
        break;
    case 0xB:
        D_800E6280.unk_110D = 0x20;
        break;
    case 0x10:
        k_sub_reset();
        gnsx(D_800B361C);
        sndi("ハイ、早乙女です。", 0, 0xA);
        func_80052060(0x01000143, 0x1E000149, -1, 0xA);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("好雄君は、", 0, 0xF);
        sndi("いらっしゃいますか？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x13:
        xa_wait();
        break;
    case 0x14:
        k_sub_reset();
        gnsx(D_800B361C);
        sndi("あっ、お兄ちゃんですね。", 0, 0xA);
        sndi("ちょっと待ってて下さい。", 1, 0xA);
        sndi("おにーちゃーん、電話ー！", 2, 0xA);
        func_80052060(0x020001A9, 0x1F0001C4, -1, 0xA);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x15:
        func_8005215C();
        break;
    case 0x16:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ハイ、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x17:
        func_8005215C();
        break;
    case 0x18:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし", 0, 0xF);
        sndi(D_800E6280.unk_0D4, 1, 0xF);
        sndi("ですけど。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x19:
        xa_wait();
        break;
    case 0x1A:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "なんだ、");
        func_800AE100(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1B:
        func_8005215C();
        break;
    case 0x1C:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1D:
        xa_wait();
        break;
    case 0x1E:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あたりまえのこと聞くなよ。", 0, 0xD);
        sndi("ところで", 1, 0xD);
        sndi("どうしたんだ今日は？", 2, 0xD);
        func_80052060(0x1400009A, 0x0C0000AB, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1F:
        func_8005215C();
        break;
    case 0x20:
        k_sub_reset();
        x_taku_string_set("女の子の評価を聞く", "デートスポットを聞く", 0, 2);
        x_taku_menu_set(1, 2);
        k_sub_disp_start(2);
        D_800E6280.unk_110D += 1;
        break;
    case 0x21:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.unk_F88 & 0x20) {
            switch (D_800E6280.unk_1094) {
            case 0:
                D_800E6280.unk_110D = 0x22;
                break;
            case 1:
                func_80042940(2);
                break;
            }
        }
        break;
    case 0x22:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子達が、", 0, 0xF);
        sndi("俺をどう思ってるか", 1, 0xF);
        sndi("知りたいんだ。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("評価か…。", 0, 0xD);
        sndi("ちょっと待ってくれよ。", 1, 0xD);
        func_80052060(0x19000085, 0x0D00008F, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("女の子の、", 0, 0xD);
        sndi("お前に対する評価は…。", 1, 0xD);
        func_80052060(0x19000090, 0x0E00009C, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x27:
        func_8005215C();
        break;
    case 0x28:
        func_80042940(3);
        break;
    case 0x30:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("はい、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし、", 0, 0xF);
        func_800AE0F0(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x33:
        xa_wait();
        break;
    case 0x34:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "なんだ、");
        func_800AE100(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x35:
        func_8005215C();
        break;
    case 0x36:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x37:
        xa_wait();
        break;
    case 0x38:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あたりまえのこと聞くなよ。", 0, 0xD);
        sndi("ところで、", 1, 0xD);
        sndi("どうしたんだ今日は？", 2, 0xD);
        func_80052060(0x1400009A, 0x0C0000AB, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x39:
        func_8005215C();
        break;
    case 0x3A:
        D_800E6280.unk_110D = 0x20;
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80132FC4(void) {
    /* FAKE: extent preserves the observed sp+0x3C buffer home. T-10040. */
    u8 sp3C[44];
    s32 var_a0;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0;
    var_a2 = 0;
    /* FAKE: scalar byte view avoids the redundant selector copy. T-10630. */
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0x0:
        for (var_a0 = 0; var_a0 < 21; var_a0++) {
            if (D_800E6280.unk_120[var_a0] & 0x80) {
                var_v1++;
            }
        }
        if (var_v1 != 0) {
            if (D_800E6280.unk_120[0] & 0x80) {
                var_v1--;
                if (var_v1 == 0) {
                    D_800E6280.unk_120[0] = 1;
                }
            }
            for (var_a0 = 1; var_a0 < 21; var_a0++) {
                if (D_800E6280.unk_120[var_a0] & 0x80) {
                    var_v1--;
                    if (var_v1 == 0) {
                        var_a2 = var_a0;
                        D_800E6280.unk_120[var_a0] = 1;
                    }
                }
            }
            if (var_a2 == 1) {
                D_800E6280.unk_110D = 0x10;
                ((CharFlags *)D_800E6280.unk_F64)->b1 = 1;

            } else if (var_a2 == 7) {
                D_800E6280.unk_110D = 0x20;
                ((CharFlags *)D_800E6280.unk_F64)->b2 = 1;

            } else if (var_a2 == 3) {
                D_800E6280.unk_110D = 0x30;
                ((CharFlags *)D_800E6280.unk_F64)->b3 = 1;

            } else if (var_a2 == 0xD) {
                D_800E6280.unk_110D = 0x40;
                ((CharFlags *)D_800E6280.unk_F64)->b4 = 1;

            } else if (var_a2 == 5) {
                D_800E6280.unk_110D = 0x50;
                ((CharFlags *)D_800E6280.unk_F64)->b5 = 1;

            } else if (var_a2 == 0xA) {
                D_800E6280.unk_110D = 0x60;
                ((CharFlags *)D_800E6280.unk_F64)->b6 = 1;

            } else if (var_a2 == 6) {
                D_800E6280.unk_110D = 0x70;
                ((CharFlags *)D_800E6280.unk_F64)->b7 = 1;

            } else if (var_a2 == 0xB) {
                ((CharFlags *)D_800E6280.unk_F64)->b8 = 1;
                D_800E6280.unk_110D = 0x80;

            } else {
                D_800E6280.unk_110D = 1;
            }
        } else {
            D_800E6280.unk_110D = 1;
        }
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("いいデートスポットを", 0, 0xF);
        sndi("教えて欲しいんだ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x2:
        xa_wait();
        break;
    case 0x3:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("悪いな。今は特に無いんだ。", 0, 0xD);
        sndi("別の事なら教えてやれるぜ。", 1, 0xD);
        func_80052060(0x140000D0, 0x0F0000E2, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4:
        func_8005215C();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x6:
        xa_wait();
        break;
    case 0x7:
        k_sub_reset();
        func_80051BDC(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.unk_110D += 1;
        break;
    case 0x8:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.unk_F88 & 0x20) {
            if (D_800E6280.unk_1094 == 0xB) {
                func_80042940(4);
            } else if (D_800E6280.unk_1094 == 0xA) {
                func_80042940(3);
                D_800E6280.unk_110D = 0x20;
            } else if ((D_800E6280.unk_1094 != -1) && (D_800E6280.unk_1BC[D_800E6280.unk_1094].unk_0C.f.b1)) {
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 0x9:
        D_800E6280.unk_110D += 1;
        break;
    case 0xA:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.unk_1094 == 0xB) {
            func_800AE0F0(sp3C, D_800B3624);
            func_800AE100(sp3C, "、");
            func_800AE100(sp3C, D_800B3624);
        } else {
            func_800AE0F0(sp3C, girl_short_name_table[D_800E6280.unk_1094]);
            func_800AE100(sp3C, "、");
            func_800AE100(sp3C, girl_short_name_table[D_800E6280.unk_1094]);
        }
        func_800AE100(sp3C, "‥‥と。");
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.unk_1094, 0xD);
        sndi(sp3C, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xB:
        func_8005215C();
        break;
    case 0xC:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あったあった。", 0, 0xD);
        sndi("耳かっぽじって", 1, 0xD);
        sndi("よく聞けよ。", 2, 0xD);
        func_80052060(0x19000064, 0x0A00006E, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xD:
        func_8005215C();
        break;
    case 0xE:
        func_80042940(4);
        break;
    case 0x10:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ちょっと遠いけど、", 0, 0xD);
        sndi("きらめき中央公園なんて", 1, 0xD);
        sndi("わりといい雰囲気だぜ。", 2, 0xD);
        func_80052060(0x15000001, 0x02000015, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x20:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("この間、", 0, 0xD);
        sndi("美術館が出来たんだよ。", 1, 0xD);
        sndi("わりといいらしいぜ。", 2, 0xD);
        func_80052060(0x15000016, 0x03000024, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x30:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("今、水族館が", 0, 0xD);
        sndi("人気あるみたいだぜ。", 1, 0xD);
        func_80052060(0x1900009D, 0x0F0000A7, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("スタジアムが出来たの", 0, 0xD);
        sndi("知ってたか？", 1, 0xD);
        func_80052060(0x190000A8, 0x100000AF, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x50:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("植物園なんて", 0, 0xD);
        sndi("いいんじゃないか？", 1, 0xD);
        func_80052060(0x190000B0, 0x110000BA, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x51:
        func_8005215C();
        break;
    case 0x52:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("隣町のボーリング場なんて", 0, 0xD);
        sndi("どうだ？", 1, 0xD);
        func_80052060(0x1A000007, 0x0300000F, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x70:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("プラネタリウムなんて", 0, 0xD);
        sndi("暗くていいぜ。", 1, 0xD);
        func_80052060(0x1A000010, 0x0400001B, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x71:
        func_8005215C();
        break;
    case 0x72:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("やっと、この街にも", 0, 0xD);
        sndi("カラオケが出来たんだよ。", 1, 0xD);
        func_80052060(0x1A00001C, 0x05000027, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0xF0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xF1:
        xa_wait();
        break;
    case 0xF2:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("お前も", 0, 0xD);
        sndi("まめに情報誌でも読めよ。", 1, 0xD);
        func_80052060(0x1A000028, 0x06000032, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xF3:
        func_8005215C();
        break;
    case 0xF4:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80133D54(void) {
    /* FAKE: buffer extent preserves the original frame and sp+0x5C address. T-10040. */
    u8 sp5C[44];
    u8 sp3C[32];
    TelRec44 *var_s0;
    s32 var_s1;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_8013CCD0();
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("こんなとこだな。", 0, 0xD);
        func_80052060(0x1E00005E, 0x12000061, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        func_8005215C();
        break;
    case 3:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 4:
        xa_wait();
        break;
    case 5:
        k_sub_reset();
        func_80051BDC(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if ((D_800E6280.unk_F88 & 0x20) && (D_800E6280.unk_1094 != -1)) {
            if (D_800E6280.unk_1094 == 0xB) {
                D_800E6280.unk_110D += 1;
            } else if (D_800E6280.unk_1BC[D_800E6280.unk_1094].unk_0C.f.b1) {
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        if (D_800E6280.unk_1094 == 0xB) {
            get_g_name(sp3C, 0xC);
        } else {
            get_g_name(sp3C, D_800E6280.unk_1094);
        }
        func_800AE0F0(sp5C, sp3C);
        func_800AE100(sp5C, "の");
        sndi(sp5C, 0, 0xF);
        sndi("ことについて", 1, 0xF);
        sndi("知りたいんだけど。", 2, 0xF);
        k_sub_disp_start(0);
        if (D_800E6280.unk_1094 == 0xA) {
            D_800E6280.unk_110D = 0x20;
        } else if (D_800E6280.unk_1094 == 0xB) {
            func_80042940(4);
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        xa_wait();
        break;
    case 9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あぁ、いいぜ。", 0, 0xD);
        sndi("ちょっと待ってくれよ。", 1, 0xD);
        func_80052060(0x180000C2, 0x150000C9, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.unk_1094 == 0xB) {
            func_800AE0F0(sp5C, D_800B3624);
            func_800AE100(sp5C, "、");
            func_800AE100(sp5C, D_800B3624);
        } else {
            func_800AE0F0(sp5C, girl_short_name_table[D_800E6280.unk_1094]);
            func_800AE100(sp5C, "、");
            func_800AE100(sp5C, girl_short_name_table[D_800E6280.unk_1094]);
        }
        func_800AE100(sp5C, "‥‥と。");
        sndi(sp5C, 0, 0xD);
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.unk_1094, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 12:
        func_8005215C();
        break;
    case 13:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あったあった。", 0, 0xD);
        sndi("耳かっぽじって", 1, 0xD);
        sndi("よく聞けよ。", 2, 0xD);
        func_80052060(0x19000064, 0x0A00006E, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 14:
        func_8005215C();
        break;
    case 15:
        func_80042940(4);
        break;
    case 32:
        xa_wait();
        break;
    case 33:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("何だ、お前。", 0, 0xD);
        sndi("優美に気があるのか？", 1, 0xD);
        func_80052060(0x1900006F, 0x0B00007A, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("い、いや。", 0, 0xF);
        sndi("そう言う訳じゃないけど…。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 36:
        xa_wait();
        break;
    case 37:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("別に隠さなくてもいいよ。", 0, 0xD);
        sndi("へぇ、お前が優美をねぇ。", 1, 0xD);
        sndi("よし、特別に教えてやろう。", 2, 0xD);
        func_80052060(0x120000C7, 0x090000E5, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        func_80042940(4);
        break;
    }
    var_s0 = &((TelRec44 *)D_8011ECD0)[0x61];
    var_s1 = 0x61;
    do {
        if (var_s0->unk_18 == 0x60) {
            var_s0->unk_1C = (s16) ((func_800A0070((D_800E6280.unk_1104.w + var_s1) << 6) / 3) + 0xAAA);
            var_s0->unk_1A = (s16) ((func_800A0070((D_800E6280.unk_1104.w + var_s1) * 0x5A) / 4) + 0xC00);
            var_s0->unk_30 = (s32) (func_800A0070(D_800E6280.unk_1104.w << 6) * 4);
            var_s0->unk_03 = (u8) (var_s0->unk_03 | 0x10);
        }
        var_s1 += 1;
        var_s0++;
    } while (var_s1 != 0x82);
    if (D_80120668 == 0x3F) {
        cal_base_show();
        return;
    }
    func_80069178();
}

void func_801345C4(void) {
    u8 state;
    switch (D_800E6280.unk_110D) {
    case 0:
        cd_read_sectors(0x26U, 0x801A0000, 0x9D5F);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        }
        break;
    case 2:
        func_8004E58C();
        func_8013D03C((s32) D_800E6280.unk_1094, (s32) &D_801230D4);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        if (D_800E6280.unk_1094 == 0xA) {
            D_800E6280.unk_110D = 0x20;
        }
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 3:
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("って、とこだな。", 0, 0xD);
        func_80052060(0x1E00005A, 0x1100005D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("なーに、いいってことよ。", 0, 0xD);
        sndi("女の子の情報なら", 1, 0xD);
        sndi("俺に任せてくれよ。", 2, 0xD);
        func_80052060(0x140000BE, 0x0E0000CF, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 11:
        xa_wait();
        break;
    case 12:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    case 32:
        xa_wait();
        break;
    case 33:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("って、とこだな。", 0, 0xD);
        func_80052060(0x1E00005A, 0x1100005D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 36:
        xa_wait();
        break;
    case 37:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("なんなら、", 0, 0xD);
        sndi("俺が優美に", 1, 0xD);
        sndi("言ってやろうか？", 2, 0xD);
        func_80052060(0x1900007B, 0x0C000084, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("だから、", 0, 0xF);
        sndi("そんなんじゃないって。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 40:
        xa_wait();
        break;
    case 41:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("まぁ、いいか。", 0, 0xD);
        sndi("じゃ、頑張れよ。", 1, 0xD);
        func_80052060(0x0A0000E3, 0x0E0000EA, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 42:
        func_8005215C();
        break;
    case 43:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 44:
        xa_wait();
        break;
    case 45:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    if (D_80120668 == 0x3F) {
        cal_base_show();
        return;
    }
    func_80069178();
}

void func_80134BE0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80134DA0();
        break;
    case 0x1:
        func_80134FE4();
        break;
    case 0x2:
        func_80135FFC();
        break;
    case 0x3:
        func_801352F0();
        break;
    case 0x4:
        func_801360E4();
        break;
    case 0x8:
        func_8013586C();
        break;
    case 0x9:
        func_80135A44();
        break;
    case 0xA:
        func_80135C4C();
        break;
    case 0x5:
        func_801361CC();
        break;
    case 0x7:
        func_80135E28();
        break;
    case 0x6:
        func_801362B4();
        break;
    case 0x10:
        func_801396EC();
        break;
    case 0x30:
        func_80136724();
        break;
    case 0x20:
        func_8013639C();
        break;
    case 0x40:
        func_801380D8();
        break;
    case 0x41:
        func_801384F0();
        break;
    case 0x42:
        func_8013883C();
        break;
    case 0x43:
        func_80138D20();
        break;
    case 0x44:
        func_80139064();
        break;
    case 0x45:
        func_801393A8();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_8006BA40();
}

void func_80134DA0(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
        if (*(u8 *)&D_800E6280.unk_F5F == 0xB) {
            D_801405E4 = 0xC;
        } else {
            D_801405E4 = *(u8 *)&D_800E6280.unk_F5F;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            if (appointment_find_today(*(u8 *)&D_800E6280.unk_03E, *(u8 *)&D_800E6280.unk_03F, *(u8 *)&D_800E6280.unk_040) != 0 &&
                *(u8 *)&D_800E6280.unk_F5F == (u32)(D_800E6280.unk_54C[0].w << 3) >> 0x1C) {
                func_80042940(0x10);
            } else if (*(u8 *)&D_800E6280.unk_03E == 0x62 && *(u8 *)&D_800E6280.unk_03F == 2 && *(u8 *)&D_800E6280.unk_040 == 0x16) {
                func_80042940(0x10);
            } else if (func_8013655C() == 0) {
                func_80042940(0x10);
            } else if (D_801405E4 == 0xC) {
                func_80042940(0x40);
            } else {
                func_8004284C();
            }
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80134FE4);

void func_801352F0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3610);
        sndi("ああ、脇役君ね。", 0, 7);
        func_80052060(0x05000000, 0x0100000B, -1, 7);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("違うよ。", 0, 0xF);
        sndi("Pだよ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_801434D4[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143500, (s32) &D_80143558, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801437BC[D_801405E4], 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80143958[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143984, (s32) &D_801439DC, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("あっ、 ちょっと待って。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;

    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("俺は、とりあえず Gの機嫌を とることにした。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;

    case 13:
        xa_wait();
        break;
    case 14:
        k_sub_reset();
        sndisp("　　　　　・ 　　　　・ 　　　　・", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        D_800E6280.unk_110D = 0x14;
        break;
    case 20:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80143BA0[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143BCC, (s32) &D_80143C24, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 21:
        func_8005215C();
        break;
    case 22:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 23:
        xa_wait();
        break;
    case 24:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp("良かった。 何とか、Gの機嫌も 直ったみたいだぞ。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 25:
        xa_wait();
        break;
    default:
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0xF;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0xF;
        func_80048E78();
        func_8004E58C();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_8013586C(void) {
    /* T-10040: share the XA table address across dialogue setup. */
    u8 girl;
    u32 *table;
    switch (D_800E6280.unk_110D) {
    case 0:
        table = D_80141084;
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_8014107C, 0, (s32) D_801405E4);
        girl = D_801405E4;
        func_80052060((s32) table, (s32) table, 0, (s32) girl);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141080, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, (s32) D_801405E4);
        girl = D_801405E4;
        func_80052060((s32) D_80140B04, (s32) D_80140B5C, (s32) girl, (s32) girl);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80135A44(void) {
    u32 sp28;
    u32 temp_hi;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        temp_hi = (u32) ((u8) D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] >> 1) % 3U;
        sp28 = temp_hi;
        sndisp(D_801410BC[temp_hi], 0, (s32) D_801405E4);
        func_80052060((s32) &D_801410D4, (s32) &D_80141104, (s32) sp28, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801410C8, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_801410CC, 0, (s32) D_801405E4);
        func_80052060((s32) &D_801410D4, (s32) &D_80141104, 4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80135C4C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141094, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014109C, (s32) &D_801410AC, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141098, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) D_80140B04, (s32) D_80140B5C, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80135E28(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141040, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014104C, (s32) &D_80141064, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141044, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141048, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014104C, (s32) &D_80141064, 2, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80135FFC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}

void func_801360E4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}

void func_801361CC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140D18[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140D44, D_80140D9C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801362B4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140F0C[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140F38, D_80140F90, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_8013639C(void) {
    /* T-10040: separate signed-byte loads through an unsigned index home. */
    u32 choice;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80067E34();
        cal_sprite_init();
        func_8013655C();
        func_8004E58C();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする日を選んで下さい", 0);
        k_disp_start(1);
        D_800E6280.unk_110D += 1;
        func_80050B00(1, 0x2F7FAF, 5, 0);
        break;
    case 1:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        func_8004FC10(1);
        menu_bar_show(1);
        choice = D_800E6280.unk_1094;
        if ((choice != -1) && (D_800E6280.unk_F88 & 0x20)) {
            func_80069218(0);
            D_80140558 = D_801405A4[D_800E6280.unk_1094];
            choice = D_800E6280.unk_1094;
            D_80140554 = D_80140564[choice];
            if ((D_800E6280.unk_03F == 0xC) && (D_80140558 == 1)) {
                D_8014055C = D_800E6280.unk_03E + 1;
            } else {
                D_8014055C = (s32) D_800E6280.unk_03E;
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    default:
        func_80050B00(0x10, 0x2F2F7F, 3, 0);
        func_80042940(0x30);
        break;
    }
    func_80068EC0();
}

s32 func_8013655C(void) {
    /* T-10040: month offset precedes the four halfword arrays in the frame. */
    s32 var_s6;
    s16 spEC[16];
    s16 spCC[16];
    s16 spAC[16];
    s16 sp8C[16];
    s32 var_a3;
    s32 var_t0;
    u8 temp_v0;
    u32 var_s0;

    var_s0 = D_800E6280.unk_040;
    var_t0 = 0;
    var_s6 = 0;
    var_a3 = 0;
    do {
        var_s0 += 1;
        if ((u8) D_800E6280.unk_044[0].unk_00[0] < var_s0) {
            var_s0 = 1;
            var_s6 = 1;
        }
        temp_v0 = D_800E6280.unk_044[var_s6].unk_00[var_s0];
        if ((temp_v0 == 1) || (temp_v0 == 2)) {

            if (appointment_find_today((s32) D_800E6280.unk_03E, D_800E6280.unk_03F + var_s6, (s32) var_s0) == 0) {

                (&spEC[0])[var_t0] = func_80068898(var_s6, (var_s0 - 1)) - 0x10;
                (&spCC[0])[var_t0] = func_800688F0(var_s6, (var_s0 - 1)) - 0x10;
                (&spAC[0])[var_t0] = 0x10;
                (&sp8C[0])[var_t0] = 0x10;
                D_80140564[var_t0] = (s32) var_s0;
                D_801405A4[var_t0] = var_s6 + D_800E6280.unk_03F;
                var_t0++;
            }
        }
        var_a3 += 1;
    } while (var_a3 != 0x1C);

    func_8004F870(1, var_t0 & 0xFF, &spEC[0], &spCC[0], &spAC[0], &sp8C[0]);
    D_800E6280.unk_1094 = 0;
    return var_t0;
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80136724);

void func_801380D8(void) {
    Rec38 *girl;
    u8 state;
    u32 temp_hi;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        D_800E6280.unk_1BC[12].unk_0C.b[3] += 1;
        gnsx(D_800B3624);
        sndisp(D_80144194, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("もしもし、 Pですけど。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        girl = &D_800E6280.unk_1BC[12];
        if ((u8) girl->unk_0C.b[3] >= 0x29U) {
            temp_hi = (u8) girl->unk_0C.b[3] % 3U;
            switch (temp_hi) {
            case 0:
                func_80042940(0x43);
                break;
            case 1:
                func_80042940(0x44);
                break;
            case 2:
                func_80042940(0x45);
                break;
            }
        } else if ((u8) girl->unk_0C.b[3] >= 0x1FU) {
            func_80042940(0x42);
        } else if ((u8) girl->unk_0C.b[3] >= 0x15U) {
            func_80042940(0x41);
        } else {
            D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        }
        break;
    case 5:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144198, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 1, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 6:
        func_8005215C();
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014419C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 8:
        xa_wait();
        break;
    case 9:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801441A0, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 3, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 12:
        xa_wait();
        break;
    case 13:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801441A4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 14:
        xa_wait();
        break;
    case 15:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801384F0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801441F8, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801441FC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144200, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144204, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144208, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014420C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_8013883C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144270, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144274, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144278, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_8014427C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144280, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144284, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144288, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 6, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        func_8005215C();
        break;
    case 14:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014428C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144290, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 8, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 17:
        func_8005215C();
        break;
    case 18:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 19:
        xa_wait();
        break;
    case 20:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144294, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 21:
        xa_wait();
        break;
    case 22:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80138D20(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144338, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014433C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144340, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144344, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144348, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014434C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_80139064(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443B0, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801443B4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443B8, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_801443BC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443C0, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801443C4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801393A8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144428, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014442C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144430, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144434, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144438, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014443C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801396EC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80044750(0x50C);
        func_80050DFC(D_800E6280.unk_0D4);
        func_80051010("あ〜あ、留守みたいだな。", 0, 0x1F);
        func_80051010("今日はあきらめるか……。）", 1, 0x1F);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80051DBC();
        break;
    case 2:
        func_8004E58C();
        func_80048E78();
        main_state_set(0x46);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
