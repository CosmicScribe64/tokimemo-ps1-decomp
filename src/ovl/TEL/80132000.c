#define MAIN_API_OVERRIDE_set_c_girl /* K&R void() passes D_801405E4 without u8 narrowing in func_80134FE4 (T-24126). */
#include "common.h"
#include "ovl/TEL.h"

void set_c_girl();

void func_80132000(void) {
    switch (D_800E6280.stage) {
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
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
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
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80067870();
    func_80066334();
    func_8006BA40();
}

void func_80132138(void) {
    if (*(u8 *)&D_800E6280.substep == 0) {
        sndi("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        k_sub_disp_start(0);
        D_801230D0 = (rand() & 3) + 1;
        *(u8 *)&D_800E6280.substep = 1;
    } else if (*(u8 *)&D_800E6280.substep == 1) {
        if ((D_800E6280.step_timer.u & 0x7F) == 1) {
            k_sub_reset();
            sndi("Ｔｒｒｒｒ…", 0, 0x2F);
            k_sub_disp_start(0);
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.pad_pressed & PAD_CROSS)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            func_8004284C();
        }
    }
    menu_bar_show(0);
    cal_base_show();
    k_disp_inc();
}

void func_8013226C(void) {
    /* FAKE: buffer extent preserves the original sp+0x3C address. T-10040. */
    u8 sp3C[36];
    u8 sp2C[16];

    switch (D_800E6280.substep) {
    case 0x0:
        if (D_800E6280.girls[13].flags.b[3] == 0) {
            D_800E6280.substep = 1;
        } else if ((appointment_find_today((s32) D_800E6280.year, (s32) D_800E6280.month, (s32) D_800E6280.day) > 0) && (((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C) == 0xA)) {
            D_800E6280.substep = 0x30;
        } else if (get_g_zyotai_h(GIRL_YUMI) == 4) {
            D_800E6280.substep = 0x30;
        } else if (D_800E6280.girls[13].flags.b[3] & 1) {
            D_800E6280.substep = 0x10;
        } else {
            D_800E6280.substep = 0x30;
        }
        D_800E6280.girls[13].flags.b[3] += 1;
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ハイ、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x2:
        func_8005215C();
        break;
    case 0x3:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("もしもし、", 0, 0xF);
        strcpy(sp3C, D_800E6280.player_family_name);
        strcat(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x4:
        xa_wait();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800B3628);
        strcpy(sp3C, "おっ、");
        get_p_name(sp2C, 0xD);
        strcat(sp3C, sp2C);
        strcat(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        sndi("良くかけて来てくれたな。", 1, 0xD);
        func_80052060((s32) &D_80144E3C, (s32) &D_80144E44, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x6:
        func_8005215C();
        break;
    case 0x7:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("女の子の事なら", 0, 0xF);
        sndi("任せておけって、", 1, 0xF);
        sndi("言ってたよな…。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0xA:
        func_8005215C();
        break;
    case 0xB:
        D_800E6280.substep = 0x20;
        break;
    case 0x10:
        k_sub_reset();
        gnsx(D_800B361C);
        sndi("ハイ、早乙女です。", 0, 0xA);
        func_80052060(0x01000143, 0x1E000149, -1, 0xA);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("好雄君は、", 0, 0xF);
        sndi("いらっしゃいますか？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x17:
        func_8005215C();
        break;
    case 0x18:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("もしもし", 0, 0xF);
        sndi(D_800E6280.player_family_name, 1, 0xF);
        sndi("ですけど。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x19:
        xa_wait();
        break;
    case 0x1A:
        k_sub_reset();
        gnsx(D_800B3628);
        strcpy(sp3C, "なんだ、");
        strcat(sp3C, D_800E6280.player_family_name);
        strcat(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x1B:
        func_8005215C();
        break;
    case 0x1C:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x1F:
        func_8005215C();
        break;
    case 0x20:
        k_sub_reset();
        x_taku_string_set("女の子の評価を聞く", "デートスポットを聞く", 0, 2);
        x_taku_menu_set(1, 2);
        k_sub_disp_start(2);
        D_800E6280.substep += 1;
        break;
    case 0x21:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.pad_pressed & PAD_CIRCLE) {
            switch (D_800E6280.menu_cursor_1) {
            case 0:
                D_800E6280.substep = 0x22;
                break;
            case 1:
                func_80042940(2);
                break;
            }
        }
        break;
    case 0x22:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("女の子達が、", 0, 0xF);
        sndi("俺をどう思ってるか", 1, 0xF);
        sndi("知りたいんだ。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("もしもし、", 0, 0xF);
        strcpy(sp3C, D_800E6280.player_family_name);
        strcat(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x33:
        xa_wait();
        break;
    case 0x34:
        k_sub_reset();
        gnsx(D_800B3628);
        strcpy(sp3C, "なんだ、");
        strcat(sp3C, D_800E6280.player_family_name);
        strcat(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x35:
        func_8005215C();
        break;
    case 0x36:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x39:
        func_8005215C();
        break;
    case 0x3A:
        D_800E6280.substep = 0x20;
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
    switch (*(u8 *)&D_800E6280.substep) {
    case 0x0:
        for (var_a0 = 0; var_a0 < 21; var_a0++) {
            if (D_800E6280.progress_state[var_a0] & 0x80) {
                var_v1++;
            }
        }
        if (var_v1 != 0) {
            if (D_800E6280.progress_state[0] & 0x80) {
                var_v1--;
                if (var_v1 == 0) {
                    D_800E6280.progress_state[0] = 1;
                }
            }
            for (var_a0 = 1; var_a0 < 21; var_a0++) {
                if (D_800E6280.progress_state[var_a0] & 0x80) {
                    var_v1--;
                    if (var_v1 == 0) {
                        var_a2 = var_a0;
                        D_800E6280.progress_state[var_a0] = 1;
                    }
                }
            }
            if (var_a2 == 1) {
                D_800E6280.substep = 0x10;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->met = 1;

            } else if (var_a2 == 7) {
                D_800E6280.substep = 0x20;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b2 = 1;

            } else if (var_a2 == 3) {
                D_800E6280.substep = 0x30;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b3 = 1;

            } else if (var_a2 == 0xD) {
                D_800E6280.substep = 0x40;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b4 = 1;

            } else if (var_a2 == 5) {
                D_800E6280.substep = 0x50;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b5 = 1;

            } else if (var_a2 == 0xA) {
                D_800E6280.substep = 0x60;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b6 = 1;

            } else if (var_a2 == 6) {
                D_800E6280.substep = 0x70;
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b7 = 1;

            } else if (var_a2 == 0xB) {
                ((GirlFlagBits *)D_800E6280.quarter_flags)->b8 = 1;
                D_800E6280.substep = 0x80;

            } else {
                D_800E6280.substep = 1;
            }
        } else {
            D_800E6280.substep = 1;
        }
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("いいデートスポットを", 0, 0xF);
        sndi("教えて欲しいんだ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x4:
        func_8005215C();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x6:
        xa_wait();
        break;
    case 0x7:
        k_sub_reset();
        menu_girl_taku_set(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.substep += 1;
        break;
    case 0x8:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.pad_pressed & PAD_CIRCLE) {
            if (D_800E6280.menu_cursor_1 == 0xB) {
                func_80042940(4);
            } else if (D_800E6280.menu_cursor_1 == 0xA) {
                func_80042940(3);
                D_800E6280.substep = 0x20;
            } else if ((D_800E6280.menu_cursor_1 != -1) && (D_800E6280.girls[D_800E6280.menu_cursor_1].flags.f.met)) {
                D_800E6280.substep += 1;
            }
        }
        break;
    case 0x9:
        D_800E6280.substep += 1;
        break;
    case 0xA:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.menu_cursor_1 == 0xB) {
            strcpy(sp3C, D_800B3624);
            strcat(sp3C, "、");
            strcat(sp3C, D_800B3624);
        } else {
            strcpy(sp3C, girl_short_name_table[D_800E6280.menu_cursor_1]);
            strcat(sp3C, "、");
            strcat(sp3C, girl_short_name_table[D_800E6280.menu_cursor_1]);
        }
        strcat(sp3C, "‥‥と。");
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.menu_cursor_1, 0xD);
        sndi(sp3C, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        D_800E6280.substep = 0xF0;
        break;
    case 0x20:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("この間、", 0, 0xD);
        sndi("美術館が出来たんだよ。", 1, 0xD);
        sndi("わりといいらしいぜ。", 2, 0xD);
        func_80052060(0x15000016, 0x03000024, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        D_800E6280.substep = 0xF0;
        break;
    case 0x30:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("今、水族館が", 0, 0xD);
        sndi("人気あるみたいだぜ。", 1, 0xD);
        func_80052060(0x1900009D, 0x0F0000A7, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        D_800E6280.substep = 0xF0;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("スタジアムが出来たの", 0, 0xD);
        sndi("知ってたか？", 1, 0xD);
        func_80052060(0x190000A8, 0x100000AF, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        D_800E6280.substep = 0xF0;
        break;
    case 0x50:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("植物園なんて", 0, 0xD);
        sndi("いいんじゃないか？", 1, 0xD);
        func_80052060(0x190000B0, 0x110000BA, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x51:
        func_8005215C();
        break;
    case 0x52:
        D_800E6280.substep = 0xF0;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("隣町のボーリング場なんて", 0, 0xD);
        sndi("どうだ？", 1, 0xD);
        func_80052060(0x1A000007, 0x0300000F, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        D_800E6280.substep = 0xF0;
        break;
    case 0x70:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("プラネタリウムなんて", 0, 0xD);
        sndi("暗くていいぜ。", 1, 0xD);
        func_80052060(0x1A000010, 0x0400001B, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x71:
        func_8005215C();
        break;
    case 0x72:
        D_800E6280.substep = 0xF0;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("やっと、この街にも", 0, 0xD);
        sndi("カラオケが出来たんだよ。", 1, 0xD);
        func_80052060(0x1A00001C, 0x05000027, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        D_800E6280.substep = 0xF0;
        break;
    case 0xF0:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
    ActorFixed *var_s0;
    s32 var_s1;

    switch (D_800E6280.substep) {
    case 0:
        func_8004E58C();
        func_8013CCD0();
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("こんなとこだな。", 0, 0xD);
        func_80052060(0x1E00005E, 0x12000061, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 2:
        func_8005215C();
        break;
    case 3:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 4:
        xa_wait();
        break;
    case 5:
        k_sub_reset();
        menu_girl_taku_set(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.substep += 1;
        break;
    case 6:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if ((D_800E6280.pad_pressed & PAD_CIRCLE) && (D_800E6280.menu_cursor_1 != -1)) {
            if (D_800E6280.menu_cursor_1 == 0xB) {
                D_800E6280.substep += 1;
            } else if (D_800E6280.girls[D_800E6280.menu_cursor_1].flags.f.met) {
                D_800E6280.substep += 1;
            }
        }
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        if (D_800E6280.menu_cursor_1 == 0xB) {
            get_g_name(sp3C, 0xC);
        } else {
            get_g_name(sp3C, D_800E6280.menu_cursor_1);
        }
        strcpy(sp5C, sp3C);
        strcat(sp5C, "の");
        sndi(sp5C, 0, 0xF);
        sndi("ことについて", 1, 0xF);
        sndi("知りたいんだけど。", 2, 0xF);
        k_sub_disp_start(0);
        if (D_800E6280.menu_cursor_1 == 0xA) {
            D_800E6280.substep = 0x20;
        } else if (D_800E6280.menu_cursor_1 == 0xB) {
            func_80042940(4);
        } else {
            D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.menu_cursor_1 == 0xB) {
            strcpy(sp5C, D_800B3624);
            strcat(sp5C, "、");
            strcat(sp5C, D_800B3624);
        } else {
            strcpy(sp5C, girl_short_name_table[D_800E6280.menu_cursor_1]);
            strcat(sp5C, "、");
            strcat(sp5C, girl_short_name_table[D_800E6280.menu_cursor_1]);
        }
        strcat(sp5C, "‥‥と。");
        sndi(sp5C, 0, 0xD);
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.menu_cursor_1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("い、いや。", 0, 0xF);
        sndi("そう言う訳じゃないけど…。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        func_80042940(4);
        break;
    }
    var_s0 = &((ActorFixed *)D_8011ECD0)[0x61];
    var_s1 = 0x61;
    do {
        if (var_s0->frame == 0x60) {
            var_s0->unk_1C = (s16) ((rsin((D_800E6280.step_timer.w + var_s1) << 6) / 3) + 0xAAA);
            var_s0->unk_1A = (s16) ((rsin((D_800E6280.step_timer.w + var_s1) * 0x5A) / 4) + 0xC00);
            var_s0->dy = (s32) (rsin(D_800E6280.step_timer.w << 6) * 4);
            var_s0->draw_flags = (u8) (var_s0->draw_flags | 0x10);
        }
        var_s1 += 1;
        var_s0++;
    } while (var_s1 != 0x82);
    if (D_80120668 == 0x3F) {
        cal_base_show();
        return;
    }
    appraisal_base_show();
}

void func_801345C4(void) {
    u8 state;
    switch (D_800E6280.substep) {
    case 0:
        cd_read_sectors(0x26U, 0x801A0000, 0x9D5F);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.substep = (state = D_800E6280.substep) + 1;
        }
        break;
    case 2:
        func_8004E58C();
        func_8013D03C((s32) D_800E6280.menu_cursor_1, (s32) &D_801230D4);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        if (D_800E6280.menu_cursor_1 == 0xA) {
            D_800E6280.substep = 0x20;
        }
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 3:
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("って、とこだな。", 0, 0xD);
        func_80052060(0x1E00005A, 0x1100005D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("サンキュー。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("だから、", 0, 0xF);
        sndi("そんなんじゃないって。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 42:
        func_8005215C();
        break;
    case 43:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
    appraisal_base_show();
}

void func_80134BE0(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
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
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80067870();
    func_8006BA40();
}

void func_80134DA0(void) {
    if (*(u8 *)&D_800E6280.substep == 0) {
        sndi("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        k_sub_disp_start(0);
        D_801230D0 = (rand() & 3) + 1;
        *(u8 *)&D_800E6280.substep = 1;
        if (*(u8 *)&D_800E6280.current_girl == 0xB) {
            D_801405E4 = 0xC;
        } else {
            D_801405E4 = *(u8 *)&D_800E6280.current_girl;
        }
    } else if (*(u8 *)&D_800E6280.substep == 1) {
        if ((D_800E6280.step_timer.u & 0x7F) == 1) {
            k_sub_reset();
            sndi("Ｔｒｒｒｒ…", 0, 0x2F);
            k_sub_disp_start(0);
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.pad_pressed & PAD_CROSS)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            if (appointment_find_today(*(u8 *)&D_800E6280.year, *(u8 *)&D_800E6280.month, *(u8 *)&D_800E6280.day) != 0 &&
                *(u8 *)&D_800E6280.current_girl == (u32)(D_800E6280.unk_54C[0].w << 3) >> 0x1C) {
                func_80042940(0x10);
            } else if (*(u8 *)&D_800E6280.year == 0x62 && *(u8 *)&D_800E6280.month == 2 && *(u8 *)&D_800E6280.day == 0x16) {
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
    menu_bar_show(0);
    cal_base_show();
    k_disp_inc();
}

void func_80134FE4(void) {
    u8 zy;

    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        set_c_girl(D_801405E4);
        D_800E6280.girls[D_801405E4].flags.b[3] += 1;
        gnsx(girl_short_name_table[D_801405E4]);
        sndi(D_801406E0[D_801405E4], 0, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        func_80052060((s32) D_80140710, (s32) D_80140770, (s32) D_801405E4, (s32) D_801405E4);
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(&D_800E6280.player_family_name[0]);
        sndisp("えっと… Pって言いますけど。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        if (get_g_zyotai_s(D_801405E4) == 4) {
            D_800E6280.girls[D_801405E4].affection = 0x10;
            func_80042940(3);
            if (D_801405E4 == 7) {
                D_800E6280.substep = 0;
            } else {
                D_800E6280.substep = 4;
            }
        } else {
            zy = get_g_zyotai_s(D_801405E4) & 0x7F;
            if (zy < 2U) {
                zy = D_801405E4;
                if (zy == 0) {
                    func_80042940(8);
                } else if ((zy == 7) && (D_800E6280.girls[zy].flags.b[3] & 1)) {
                    func_80042940(9);
                } else if (zy == 9) {
                    func_80042940(0xA);
                } else {
                    func_80042940(4);
                }
            } else if (zy == 2) {
                zy = D_801405E4;
                if ((zy == 0xA) && (D_800E6280.girls[zy].flags.b[3] & 1)) {
                    func_80042940(7);
                }
                func_80042940(5);
            } else {
                func_80042940(6);
            }
        }
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801352F0(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3610);
        sndi("ああ、脇役君ね。", 0, 7);
        func_80052060(0x05000000, 0x0100000B, -1, 7);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi("違うよ。", 0, 0xF);
        sndi("Pだよ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801437BC[D_801405E4], 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp("あっ、 ちょっと待って。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;

    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp("俺は、とりあえず Gの機嫌を とることにした。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;

    case 13:
        xa_wait();
        break;
    case 14:
        k_sub_reset();
        sndisp("　　　　　・ 　　　　・ 　　　　・", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        D_800E6280.substep = 0x14;
        break;
    case 20:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80143BA0[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143BCC, (s32) &D_80143C24, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 21:
        func_8005215C();
        break;
    case 22:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 23:
        xa_wait();
        break;
    case 24:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.player_family_name);
        sndisp("良かった。 何とか、Gの機嫌も 直ったみたいだぞ。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 25:
        xa_wait();
        break;
    default:
        D_800E6280.girls[D_800E6280.current_girl].affection = 0xF;
        D_800E6280.girls[D_800E6280.current_girl].affection = 0xF;
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
    switch (D_800E6280.substep) {
    case 0:
        table = D_80141084;
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_8014107C, 0, (s32) D_801405E4);
        girl = D_801405E4;
        func_80052060((s32) table, (s32) table, 0, (s32) girl);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80141080, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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

    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        temp_hi = (u32) ((u8) D_800E6280.girls[D_801405E4].flags.b[3] >> 1) % 3U;
        sp28 = temp_hi;
        sndisp(D_801410BC[temp_hi], 0, (s32) D_801405E4);
        func_80052060((s32) &D_801410D4, (s32) &D_80141104, (s32) sp28, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801410C8, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141094, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014109C, (s32) &D_801410AC, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80141098, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141040, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014104C, (s32) &D_80141064, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80141044, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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

void func_801360E4(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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

void func_801361CC(void) {
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140D18[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140D44, D_80140D9C, D_801405E4, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140F0C[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140F38, D_80140F90, D_801405E4, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        func_80067E34();
        cal_sprite_init();
        func_8013655C();
        func_8004E58C();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする日を選んで下さい", 0);
        k_disp_start(1);
        D_800E6280.substep += 1;
        menu_bar_color(1, 0x2F7FAF, 5, 0);
        break;
    case 1:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        func_8004FC10(1);
        menu_bar_show(1);
        choice = D_800E6280.menu_cursor_1;
        if ((choice != -1) && (D_800E6280.pad_pressed & PAD_CIRCLE)) {
            cal_sprite_disp_switch(0);
            D_80140558 = D_801405A4[D_800E6280.menu_cursor_1];
            choice = D_800E6280.menu_cursor_1;
            D_80140554 = D_80140564[choice];
            if ((D_800E6280.month == 0xC) && (D_80140558 == 1)) {
                D_8014055C = D_800E6280.year + 1;
            } else {
                D_8014055C = (s32) D_800E6280.year;
            }
            D_800E6280.substep += 1;
        }
        break;
    default:
        menu_bar_color(0x10, 0x2F2F7F, 3, 0);
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

    var_s0 = D_800E6280.day;
    var_t0 = 0;
    var_s6 = 0;
    var_a3 = 0;
    do {
        var_s0 += 1;
        if ((u8) D_800E6280.month_calendar[0].day_kind[0] < var_s0) {
            var_s0 = 1;
            var_s6 = 1;
        }
        temp_v0 = D_800E6280.month_calendar[var_s6].day_kind[var_s0];
        if ((temp_v0 == 1) || (temp_v0 == 2)) {

            if (appointment_find_today((s32) D_800E6280.year, D_800E6280.month + var_s6, (s32) var_s0) == 0) {

                (&spEC[0])[var_t0] = func_80068898(var_s6, (var_s0 - 1)) - 0x10;
                (&spCC[0])[var_t0] = func_800688F0(var_s6, (var_s0 - 1)) - 0x10;
                (&spAC[0])[var_t0] = 0x10;
                (&sp8C[0])[var_t0] = 0x10;
                D_80140564[var_t0] = (s32) var_s0;
                D_801405A4[var_t0] = var_s6 + D_800E6280.month;
                var_t0++;
            }
        }
        var_a3 += 1;
    } while (var_a3 != 0x1C);

    menu_set(1, var_t0 & 0xFF, &spEC[0], &spCC[0], &spAC[0], &sp8C[0]);
    D_800E6280.menu_cursor_1 = 0;
    return var_t0;
}

void func_80136724(void) {
    s32 pad_month; /* FAKE: preserves the recovered scalar frame slot, T-25055. */
    u8 *text;
    u32 lim;
    s32 hp;
    u32 pad_max; /* FAKE: preserves total and array stack homes, T-25055. */
    s32 rnd;
    s32 total;
    /* FAKE: text extent inferred from recovered stack homes, T-25055. */
    u8 sp78[0x104];
    u8 sp68[16];
    s16 sp48[16];
    TelDateEntry sp44;
    u8 ent;
    GameState *g = &D_800E6280;

    cal_base_show();
    menu_bar_show(0);
    switch (g->substep) {
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        g->substep += 1;
        break;
    case 0x1:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        if ((g->step_timer.u >= 0x11U) && (g->pad_pressed & PAD_CIRCLE)) {
            if (g->menu_cursor_0 == 0xA) {
                func_80044750(0x50D);
                g->substep = 2;
            } else if (g->menu_cursor_0 >= 0) {
                g->substep = 4;
                D_80140550 = (s32) g->menu_cursor_0;
            }
        }
        break;
    case 0x2:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(1, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        g->substep += 1;
        break;
    case 0x3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        menu_bar_show(0);
        if ((g->step_timer.u >= 0x11U) && (g->pad_pressed & PAD_CIRCLE)) {
            if (g->menu_cursor_0 == 0xA) {
                func_80044750(0x50D);
                g->substep = 0;
            } else if (g->menu_cursor_0 >= 0) {
                g->substep = 4;
                if (g->menu_cursor_0 < 6) {
                    D_80140550 = g->menu_cursor_0 + 0xA;
                }
                if ((g->menu_cursor_0 == 6) || (g->menu_cursor_0 == 7)) {
                    if ((g->month == 6) || (g->month == 7) || (g->month == 8)) {
                        D_80140550 = g->menu_cursor_0 + 0xA;
                    } else {
                        D_80140550 = g->menu_cursor_0 + 0xD;
                    }
                }
                if (g->menu_cursor_0 == 9) {
                    D_80140550 = 0x12;
                }
            }
        }
        break;
    case 0x4:
        k_sub_reset();
        gnsx(g->player_family_name);
        if (D_80140558 == 0xD) {
            D_80140558 = 1;
        }
        strcpy(sp78, D_800B3288[D_80140558]);
        strcat(sp78, "月");
        if (D_80140554 >= 0xA) {
            strcat(sp78, D_800B3288[D_80140554 / 10]);
        }
        strcat(sp78, D_800B3288[D_80140554 % 10]);
        strcat(sp78, "日");
        strcat(sp78, "に、 ");
        strcat(sp78, D_800B342C[D_80140550]);
        strcat(sp78, "へ 行かない？");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x5:
        xa_wait();
        break;
    case 0x6:
        func_80053418(sp68, sp48);
        /* FAKE: this false unsigned test keeps the girl range profitable in
         * uopt; ugen removes it. Restores colors and address sharing, T-25055. */
        if ((u32) g->current_girl < 0) {
        }
        lim = sp68[g->current_girl] >> 2;
        hp = (s16) g->param_trivia.value / 4;
        if (hp < (s32) lim) {
            total = lim;
        } else {
            total = (u32) hp;
        }
        ent = D_80141134[g->current_girl][D_80140550];
        if (ent != 0) {
            total = ent + total;
        } else {
            total = 0;
        }
        if (g->param_looks.value < 0x28) {
            total = (g->param_looks.value + total) - 0x28;
        }
        rnd = rand() & 0xFF;
        if (rnd < total) {
            g->substep = 0x20;
        } else {
            if ((total - rnd) < 0x66) {
                g->substep = 0x40;
            } else if ((total - rnd) < 0xB3) {
                g->substep = 0x60;
            } else {
                g->substep = 0x80;
            }
            g->param_stress.value += 9;
            g->param_guts.value += rand() & 7;
            parameter_show_init();
            parameter_show_init();
        }
        g->girls[g->current_girl].affection += g->frame_count & 3;
        g->girls[g->current_girl].syoushin -= 0xC;
        if (g->girls[g->current_girl].syoushin < 0) {
            g->girls[g->current_girl].syoushin = 0;
        }
        if (g->current_girl == GIRL_NOZOMI) {
            if ((u32) (get_g_zyotai_s(g->current_girl) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = g->current_girl;
            }
        } else {
            D_801405E4 = g->current_girl;
        }
        break;
    case 0x20:
        k_sub_reset();
        sp44.bits.place = D_80140550;
        sp44.bits.year = D_8014055C;
        sp44.bits.month = D_80140558;
        sp44.bits.day = D_80140554;
        sp44.bits.unk_21 = D_80140560;
        sp44.bits.unk_24 = 0;
        sp44.bits.girl = g->current_girl;
        sp44.bits.active = 1;
        sp44.bits.unk_30 = 0;
        func_8006B5BC(sp44.word);
        func_8006B0C8();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_801413C8[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_801413F8, (s32) D_80141458, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        k_sub_reset();
        gnsx(g->player_family_name);
        strcpy(sp78, "それじゃ、");
        if (D_80140558 == 0xD) {
            D_80140558 = 1;
        }
        strcat(sp78, D_800B3288[D_80140558]);
        strcat(sp78, "月");
        if (D_80140554 >= 0xA) {
            strcat(sp78, D_800B3288[D_80140554 / 10]);
        }
        strcat(sp78, D_800B3288[D_80140554 % 10]);
        strcat(sp78, "日");
        strcat(sp78, "に、 ");
        strcat(sp78, D_800B342C[D_80140550]);
        strcat(sp78, "の前で 待ち合わせということで。");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_80141680[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_801416B0, (s32) D_80141710, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(g->player_family_name);
        text = D_801418D0[D_801405E4];
        if (*text == 0x20) {
            g->substep = 0x2D;
        } else {
            sndisp(text, 0, 0xF);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        break;
    case 0x27:
        xa_wait();
        break;
    case 0x28:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        text = D_8014197C[D_801405E4];
        if (*text == 0x20) {
            g->substep = 0x2D;
        } else {
            sndisp(text, 0, (s32) g->current_girl);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        func_80052060((s32) D_801419AC, (s32) D_80141A0C, (s32) D_801405E4, (s32) g->current_girl);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        k_sub_reset();
        gnsx(g->player_family_name);
        text = D_80141AB0[D_801405E4];
        if (*text == 0x20) {
            g->substep = 0x2D;
        } else {
            sndisp(text, 0, 0xF);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        break;
    case 0x2B:
        xa_wait();
        break;
    case 0x2C:
        g->substep = 0x2D;
        break;
    case 0x2D:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x2E:
        xa_wait();
        break;
    case 0x2F:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(g->player_family_name);
        strcpy(sp78, "やったー。 ");
        strcat(sp78, D_800B3288[D_80140558]);
        strcat(sp78, "月");
        if (D_80140554 >= 0xA) {
            strcat(sp78, D_800B3288[D_80140554 / 10]);
        }
        strcat(sp78, D_800B3288[D_80140554 % 10]);
        strcat(sp78, "日");
        strcat(sp78, "が、楽しみだぜ。 ");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x30:
        xa_wait();
        break;
    case 0x31:
        g->substep = 0xFF;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        if ((g->current_girl == GIRL_NOZOMI) && ((u32) (get_g_zyotai_s(g->current_girl) & 0x7F) < 2U)) {
            D_801405E4 = 0xB;
        }
        if (g->current_girl == GIRL_SAKI) {
            hp = (D_80140554 + 6) / 7;
            /* FAKE: the second comparison uses * 1 to retain expression
             * operand order; no runtime instruction remains, T-25055. */
            if ((hp == 3) && (D_80140558 == g->month) && (g->month_calendar[0].day_kind[D_80140554] == 1)) {
                D_801405E4 = 0xC;
            } else if (((hp * 1) == 3) && (D_80140558 != g->month) && (g->month_calendar[1].day_kind[D_80140554] == 1)) {
                D_801405E4 = 0xC;
            }
        }
        sndisp(D_80141D80[D_801405E4], 0, (s32) g->current_girl);
        k_sub_disp_start(0);
        func_80052060((s32) D_80141DB4, (s32) D_80141E1C, (s32) D_801405E4, (s32) g->current_girl);
        g->substep += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(D_80141FE0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        /* fallthrough */
    case 0x43:
        xa_wait();
        break;
    case 0x44:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_80142254[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_80142288, (s32) D_801422F0, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(D_80142570[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x47:
        xa_wait();
        break;
    case 0x48:
        if (g->current_girl == GIRL_MEGUMI) {
            g->substep = 0x4D;
        } else {
            k_sub_reset();
            func_80044750(0x50B);
            sndi("ガチャ", 0, 0x2F);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        break;
    case 0x49:
        xa_wait();
        break;
    case 0x4A:
        k_sub_reset();
        gnsx(g->player_family_name);
        func_80044750(0x50C);
        if (g->current_girl == GIRL_YUKARI) {
            sndisp("言葉うつっちゃったよ。）", 0, 0x1F);
        } else {
            sndisp("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1F);
        }
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x4B:
        xa_wait();
        break;
    case 0x4C:
        g->substep = 0xFF;
        break;
    case 0x4D:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp("あの…。 また今度、 誘って下さい…。", 0, (s32) g->current_girl);
        func_80052060(0x02000163, 0x19000170, -1, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp("うん、そうするよ。 じゃあ。", 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x50:
        xa_wait();
        break;
    case 0x51:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x52:
        xa_wait();
        break;
    case 0x53:
        g->substep = 0x4A;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_80142798[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_801427C8, (s32) D_80142828, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(D_801429E0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x63:
        xa_wait();
        break;
    case 0x64:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_80142B40[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_80142B70, (s32) D_80142BD0, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (*D_80142CB0[D_801405E4] == 0x20) {
            g->substep = 0x68;
        } else {
            k_sub_reset();
            gnsx(g->player_family_name);
            sndisp(D_80142CB0[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        break;
    case 0x67:
        xa_wait();
        break;
    case 0x68:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x69:
        xa_wait();
        break;
    case 0x6A:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(g->player_family_name);
        sndisp("あーあ、間が悪かったかな。", 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x6B:
        xa_wait();
        break;
    case 0x6C:
        g->substep = 0xFF;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_80142E70[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_80142EA0, (s32) D_80142F00, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(D_8014306C[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x83:
        xa_wait();
        break;
    case 0x84:
        k_sub_reset();
        gnsx(girl_short_name_table[g->current_girl]);
        sndisp(D_801431E8[D_801405E4], 0, (s32) g->current_girl);
        func_80052060((s32) D_80143218, (s32) D_80143278, (s32) D_801405E4, (s32) g->current_girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (*D_8014333C[D_801405E4] == 0x20) {
            g->substep = 0x88;
        } else {
            k_sub_reset();
            gnsx(g->player_family_name);
            sndisp(D_8014333C[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            g->substep += 1;
        }
        break;
    case 0x87:
        xa_wait();
        break;
    case 0x88:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x89:
        xa_wait();
        break;
    case 0x8A:
        k_sub_reset();
        gnsx(g->player_family_name);
        func_80044750(0x50C);
        sndisp("やっぱり、だめか…。", 0, 0xF);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x8B:
        xa_wait();
        break;
    case 0x8C:
        g->substep = 0xFF;
        break;
    case 0xFD:
        xa_wait();
        break;
    case 0xFE:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        g->substep = 0xFD;
        break;
    case 0xFF:
    default:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
}

void func_801380D8(void) {
    GirlRecord *girl;
    u8 state;
    u32 temp_hi;

    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        D_800E6280.girls[12].flags.b[3] += 1;
        gnsx(D_800B3624);
        sndisp(D_80144194, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp("もしもし、 Pですけど。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        girl = &D_800E6280.girls[12];
        if ((u8) girl->flags.b[3] >= 0x29U) {
            temp_hi = (u8) girl->flags.b[3] % 3U;
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
        } else if ((u8) girl->flags.b[3] >= 0x1FU) {
            func_80042940(0x42);
        } else if ((u8) girl->flags.b[3] >= 0x15U) {
            func_80042940(0x41);
        } else {
            D_800E6280.substep = (state = D_800E6280.substep) + 1;
        }
        break;
    case 5:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144198, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 1, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 6:
        func_8005215C();
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014419C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
        break;
    case 12:
        xa_wait();
        break;
    case 13:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801441A4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep = (state = D_800E6280.substep) + 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801441F8, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801441FC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi(D_80144204, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014420C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144270, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80144274, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi(D_8014427C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80144284, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 13:
        func_8005215C();
        break;
    case 14:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014428C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 17:
        func_8005215C();
        break;
    case 18:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 19:
        xa_wait();
        break;
    case 20:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_80144294, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144338, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014433C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi(D_80144344, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014434C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443B0, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801443B4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi(D_801443BC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_801443C4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144428, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014442C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndi(D_80144434, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
        D_800E6280.substep += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        /* fallthrough */
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        sndisp(D_8014443C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
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
    switch (D_800E6280.substep) {
    case 0:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.player_family_name);
        sndi("あ〜あ、留守みたいだな。", 0, 0x1F);
        sndi("今日はあきらめるか……。）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        func_8004E58C();
        func_80048E78();
        main_state_set(MODE_HOLIDAY);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
