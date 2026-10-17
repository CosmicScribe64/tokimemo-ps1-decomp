void func_80136724(void) {
    s32 pad_month; /* FAKE: frame slot of a promoted local of the original, T-19022 */
    u8 *text;
    u32 lim;
    s32 hp;
    u32 hi;
    s32 rnd;
    s32 total;
    u8 sp78[0x104];
    u8 sp68[16];
    s16 sp48[16];
    TelApptWord sp44;
    u8 ent;
    GameState *g = &D_800E6280;

    cal_base_show();
    menu_bar_show(0);
    switch (g->unk_110D) {
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        g->unk_110D += 1;
        break;
    case 0x1:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        if ((g->unk_1104.u >= 0x11U) && (g->unk_F88 & 0x20)) {
            if (g->unk_1093 == 0xA) {
                func_80044750(0x50D);
                g->unk_110D = 2;
            } else if (g->unk_1093 >= 0) {
                g->unk_110D = 4;
                D_80140550 = (s32) g->unk_1093;
            }
        }
        break;
    case 0x2:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(1, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        g->unk_110D += 1;
        break;
    case 0x3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        menu_bar_show(0);
        if ((g->unk_1104.u >= 0x11U) && (g->unk_F88 & 0x20)) {
            if (g->unk_1093 == 0xA) {
                func_80044750(0x50D);
                g->unk_110D = 0;
            } else if (g->unk_1093 >= 0) {
                g->unk_110D = 4;
                if (g->unk_1093 < 6) {
                    D_80140550 = g->unk_1093 + 0xA;
                }
                if ((g->unk_1093 == 6) || (g->unk_1093 == 7)) {
                    if ((g->unk_03F == 6) || (g->unk_03F == 7) || (g->unk_03F == 8)) {
                        D_80140550 = g->unk_1093 + 0xA;
                    } else {
                        D_80140550 = g->unk_1093 + 0xD;
                    }
                }
                if (g->unk_1093 == 9) {
                    D_80140550 = 0x12;
                }
            }
        }
        break;
    case 0x4:
        k_sub_reset();
        gnsx(g->unk_0D4);
        if (D_80140558 == 0xD) {
            D_80140558 = 1;
        }
        func_800AE0F0(sp78, D_800B3288[D_80140558]);
        func_800AE100(sp78, "月");
        if (D_80140554 >= 0xA) {
            func_800AE100(sp78, D_800B3288[D_80140554 / 10]);
        }
        func_800AE100(sp78, D_800B3288[D_80140554 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "へ 行かない？");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x5:
        xa_wait();
        break;
    case 0x6:
        func_80053418(sp68, sp48);
        lim = sp68[g->unk_F5F] >> 2;
        hp = (s16) g->unk_110.unk_02 / 4;
        if (hp < (s32) lim) {
            hi = lim;
        } else {
            hi = (u32) hp;
        }
        ent = D_80141134[g->unk_F5F][D_80140550];
        if (ent != 0) {
            total = ent + hi;
        } else {
            total = 0;
        }
        if (g->unk_114.unk_02 < 0x28) {
            total = (g->unk_114.unk_02 + total) - 0x28;
        }
        rnd = func_800AE0D0() & 0xFF;
        if (rnd < total) {
            g->unk_110D = 0x20;
        } else {
            if ((total - rnd) < 0x66) {
                g->unk_110D = 0x40;
            } else if ((total - rnd) < 0xB3) {
                g->unk_110D = 0x60;
            } else {
                g->unk_110D = 0x80;
            }
            g->unk_11C.unk_02 += 9;
            g->unk_118.unk_02 += func_800AE0D0() & 7;
            parameter_show_init();
            parameter_show_init();
        }
        g->unk_1BC[g->unk_F5F].unk_06 += g->unk_10F8 & 3;
        g->unk_1BC[g->unk_F5F].unk_0A -= 0xC;
        if (g->unk_1BC[g->unk_F5F].unk_0A < 0) {
            g->unk_1BC[g->unk_F5F].unk_0A = 0;
        }
        if (g->unk_F5F == 6) {
            if ((u32) (get_g_zyotai_s(g->unk_F5F) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = g->unk_F5F;
            }
        } else {
            D_801405E4 = g->unk_F5F;
        }
        break;
    case 0x20:
        k_sub_reset();
        sp44.b.place = D_80140550;
        sp44.b.f5 = D_8014055C;
        sp44.b.month = D_80140558;
        sp44.b.day = D_80140554;
        sp44.b.f21 = D_80140560;
        sp44.b.b24 = 0;
        sp44.b.girl = g->unk_F5F;
        sp44.b.b29 = 1;
        sp44.b.rest = 0;
        func_8006B5BC(sp44.w);
        func_8006B0C8();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_801413C8[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_801413F8, (s32) D_80141458, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        k_sub_reset();
        gnsx(g->unk_0D4);
        func_800AE0F0(sp78, "それじゃ、");
        if (D_80140558 == 0xD) {
            D_80140558 = 1;
        }
        func_800AE100(sp78, D_800B3288[D_80140558], D_80140558);
        func_800AE100(sp78, "月");
        if (D_80140554 >= 0xA) {
            func_800AE100(sp78, D_800B3288[D_80140554 / 10]);
        }
        func_800AE100(sp78, D_800B3288[D_80140554 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "の前で 待ち合わせということで。");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_80141680[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_801416B0, (s32) D_80141710, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(g->unk_0D4);
        text = D_801418D0[D_801405E4];
        if (*text == 0x20) {
            g->unk_110D = 0x2D;
        } else {
            sndisp(text, 0, 0xF);
            k_sub_disp_start(0);
            g->unk_110D += 1;
        }
        break;
    case 0x27:
        xa_wait();
        break;
    case 0x28:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        text = D_8014197C[D_801405E4];
        if (*text == 0x20) {
            g->unk_110D = 0x2D;
        } else {
            sndisp(text, 0, (s32) g->unk_F5F);
            k_sub_disp_start(0);
            g->unk_110D += 1;
        }
        func_80052060((s32) D_801419AC, (s32) D_80141A0C, (s32) D_801405E4, (s32) g->unk_F5F);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        k_sub_reset();
        gnsx(g->unk_0D4);
        text = D_80141AB0[D_801405E4];
        if (*text == 0x20) {
            g->unk_110D = 0x2D;
        } else {
            sndisp(text, 0, 0xF);
            k_sub_disp_start(0);
            g->unk_110D += 1;
        }
        break;
    case 0x2B:
        xa_wait();
        break;
    case 0x2C:
        g->unk_110D = 0x2D;
        break;
    case 0x2D:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x2E:
        xa_wait();
        break;
    case 0x2F:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(g->unk_0D4);
        func_800AE0F0(sp78, "やったー。 ");
        func_800AE100(sp78, D_800B3288[D_80140558], D_80140558);
        func_800AE100(sp78, "月");
        if (D_80140554 >= 0xA) {
            func_800AE100(sp78, D_800B3288[D_80140554 / 10]);
        }
        func_800AE100(sp78, D_800B3288[D_80140554 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "が、楽しみだぜ。 ");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x30:
        xa_wait();
        break;
    case 0x31:
        g->unk_110D = 0xFF;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        if ((g->unk_F5F == 6) && ((u32) (get_g_zyotai_s(g->unk_F5F) & 0x7F) < 2U)) {
            D_801405E4 = 0xB;
        }
        ent = D_801405E4;
        if (g->unk_F5F == 4) {
            hp = (D_80140554 + 6) / 7;
            if ((hp == 3) && (D_80140558 == g->unk_03F) && (g->unk_044[0].unk_00[D_80140554] == 1)) {
                ent = 0xC;
            } else if ((hp == 3) && (D_80140558 != g->unk_03F) && (g->unk_044[1].unk_00[D_80140554] == 1)) {
                ent = 0xC;
            }
        }
        D_801405E4 = ent;
        sndisp(D_80141D80[ent], 0, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        func_80052060((s32) D_80141DB4, (s32) D_80141E1C, (s32) D_801405E4, (s32) g->unk_F5F);
        g->unk_110D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(D_80141FE0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        /* fallthrough */
    case 0x43:
        xa_wait();
        break;
    case 0x44:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_80142254[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_80142288, (s32) D_801422F0, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(D_80142570[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x47:
        xa_wait();
        break;
    case 0x48:
        if (g->unk_F5F == 9) {
            g->unk_110D = 0x4D;
        } else {
            k_sub_reset();
            func_80044750(0x50B);
            sndi("ガチャ", 0, 0x2F);
            k_sub_disp_start(0);
            g->unk_110D += 1;
        }
        break;
    case 0x49:
        xa_wait();
        break;
    case 0x4A:
        k_sub_reset();
        gnsx(g->unk_0D4);
        func_80044750(0x50C);
        if (g->unk_F5F == 5) {
            sndisp("言葉うつっちゃったよ。）", 0, 0x1F);
        } else {
            sndisp("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1F);
        }
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x4B:
        xa_wait();
        break;
    case 0x4C:
        g->unk_110D = 0xFF;
        break;
    case 0x4D:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp("あの…。 また今度、 誘って下さい…。", 0, (s32) g->unk_F5F);
        func_80052060(0x02000163, 0x19000170, -1, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp("うん、そうするよ。 じゃあ。", 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x50:
        xa_wait();
        break;
    case 0x51:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x52:
        xa_wait();
        break;
    case 0x53:
        g->unk_110D = 0x4A;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_80142798[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_801427C8, (s32) D_80142828, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(D_801429E0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x63:
        xa_wait();
        break;
    case 0x64:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_80142B40[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_80142B70, (s32) D_80142BD0, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (*D_80142CB0[D_801405E4] == 0x20) {
            g->unk_110D = 0x68;
        } else {
            k_sub_reset();
            gnsx(g->unk_0D4);
            sndisp(D_80142CB0[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            g->unk_110D += 1;
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
        g->unk_110D += 1;
        break;
    case 0x69:
        xa_wait();
        break;
    case 0x6A:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(g->unk_0D4);
        sndisp("あーあ、間が悪かったかな。", 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x6B:
        xa_wait();
        break;
    case 0x6C:
        g->unk_110D = 0xFF;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_80142E70[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_80142EA0, (s32) D_80142F00, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(D_8014306C[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x83:
        xa_wait();
        break;
    case 0x84:
        k_sub_reset();
        gnsx(girl_short_name_table[g->unk_F5F]);
        sndisp(D_801431E8[D_801405E4], 0, (s32) g->unk_F5F);
        func_80052060((s32) D_80143218, (s32) D_80143278, (s32) D_801405E4, (s32) g->unk_F5F);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (*D_8014333C[D_801405E4] == 0x20) {
            g->unk_110D = 0x88;
        } else {
            k_sub_reset();
            gnsx(g->unk_0D4);
            sndisp(D_8014333C[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            g->unk_110D += 1;
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
        g->unk_110D += 1;
        break;
    case 0x89:
        xa_wait();
        break;
    case 0x8A:
        k_sub_reset();
        gnsx(g->unk_0D4);
        func_80044750(0x50C);
        sndisp("やっぱり、だめか…。", 0, 0xF);
        k_sub_disp_start(0);
        g->unk_110D += 1;
        break;
    case 0x8B:
        xa_wait();
        break;
    case 0x8C:
        g->unk_110D = 0xFF;
        break;
    case 0xFD:
        xa_wait();
        break;
    case 0xFE:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        g->unk_110D = 0xFD;
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
