void func_80136724(void) {
    s32 sp17C;
    u8 sp78[0x104];
    u8 sp68[16];
    s16 sp48[16];
    GsWord sp44;                                        /* compiler-managed */
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_lo;
    s32 temp_t3;
    s32 temp_v0_2;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_t2;
    s32 var_v1_2;
    u32 temp_t2;
    u32 var_v1;
    u8 *temp_a0_2;
    u8 *temp_a0_3;
    u8 *temp_a0_4;
    u8 temp_t2_2;
    u8 temp_t3_2;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_v0;
    u8 var_t1;

    cal_base_show();
    menu_bar_show(0);
    switch ((u8) D_800E6280.unk_110D) {
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        D_800E6280.unk_110D += 1;
        break;
    case 0x1:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        if (((u32) D_800E6280.unk_1104.w >= 0x11U) && (D_800E6280.unk_F88 & 0x20)) {
            if (D_800E6280.unk_1093 == 0xA) {
                func_80044750(0x50D);
                D_800E6280.unk_110D = 2;
            } else if (D_800E6280.unk_1093 >= 0) {
                D_800E6280.unk_110D = 4;
                D_80140550 = (s32) D_800E6280.unk_1093;
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
        D_800E6280.unk_110D += 1;
        break;
    case 0x3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        menu_bar_show(0);
        if (((u32) D_800E6280.unk_1104.w >= 0x11U) && (D_800E6280.unk_F88 & 0x20)) {
            if (D_800E6280.unk_1093 == 0xA) {
                func_80044750(0x50D);
                D_800E6280.unk_110D = 0;
            } else if (D_800E6280.unk_1093 >= 0) {
                D_800E6280.unk_110D = 4;
                if (D_800E6280.unk_1093 < 6) {
                    D_80140550 = D_800E6280.unk_1093 + 0xA;
                }
                if ((D_800E6280.unk_1093 == 6) || (D_800E6280.unk_1093 == 7)) {
                    if ((D_800E6280.unk_03F == 6) || (D_800E6280.unk_03F == 7) || (D_800E6280.unk_03F == 8)) {
                        D_80140550 = D_800E6280.unk_1093 + 0xA;
                    } else {
                        D_80140550 = D_800E6280.unk_1093 + 0xD;
                    }
                }
                if (D_800E6280.unk_1093 == 9) {
                    D_80140550 = 0x12;
                }
            }
        }
        break;
    case 0x4:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        var_a2 = D_80140558;
        if (var_a2 == 0xD) {
            var_a2 = 1;
        }
        D_80140558 = var_a2;
        func_800AE0F0(sp78, D_800B3288[var_a2]);
        func_800AE100(sp78, "月");
        temp_a3 = D_80140554;
        if (temp_a3 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "へ 行かない？");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x5:
        xa_wait();
        break;
    case 0x6:
        func_80053418((s32) &sp68, (s32) &sp48);
        temp_t2 = (u8) *(u8 *)(sp68 + D_800E6280.unk_F5F) >> 2;
        temp_t3 = (s16) D_800E6280.unk_110.unk_02 / 4;
        if (temp_t3 < (s32) temp_t2) {
            var_v1 = temp_t2;
        } else {
            var_v1 = (u32) temp_t3;
        }
        temp_v0 = (*D_80141134)[(D_800E6280.unk_F5F * 0x15) + D_80140550];
        if (temp_v0 != 0) {
            var_v1_2 = temp_v0 + var_v1;
        } else {
            var_v1_2 = 0;
        }
        if (D_800E6280.unk_114.unk_02 < 0x28) {
            var_v1_2 = (D_800E6280.unk_114.unk_02 + var_v1_2) - 0x28;
        }
        sp17C = var_v1_2;
        temp_a0 = func_800AE0D0() & 0xFF;
        temp_v0_2 = var_v1_2 - temp_a0;
        if (temp_a0 < var_v1_2) {
            D_800E6280.unk_110D = 0x20;
        } else {
            if (temp_v0_2 < 0x66) {
                D_800E6280.unk_110D = 0x40;
            } else if (temp_v0_2 < 0xB3) {
                D_800E6280.unk_110D = 0x60;
            } else {
                D_800E6280.unk_110D = 0x80;
            }
            D_800E6280.unk_11C.unk_02 += 9;
            D_800E6280.unk_118.unk_02 += func_800AE0D0() & 7;
            parameter_show_init();
            parameter_show_init();
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += D_800E7378 & 3;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A -= 0xC;
        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A < 0) {
            D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A = 0;
        }
        if (D_800E6280.unk_F5F == 6) {
            if ((u32) (get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = D_800E6280.unk_F5F;
            }
        } else {
            D_801405E4 = D_800E6280.unk_F5F;
        }
        break;
    case 0x20:
        k_sub_reset();
        sp44.h[0] = (D_80140550 & 0x1F) | (sp44.h[0] & 0xFFE0);
        sp44.h[0] = ((D_8014055C << 5) & 0xFE0) | (sp44.h[0] & 0xF01F);
        temp_t4 = (D_80140554 & 0x1F) | (sp44.b[2] & 0xFFE0);
        sp44.b[1] = (D_80140558 * 0x10) | (sp44.b[1] & 0xFF0F);
        sp44.b[2] = temp_t4;
        sp44.b[2] = (D_80140560 << 5) | (temp_t4 & 0x1F);
        temp_t2_2 = sp44.b[3] & 0xFFFE;
        temp_t7 = ((D_800E6280.unk_F5F * 2) & 0x1E) | (temp_t2_2 & 0xE1);
        sp44.b[3] = temp_t2_2;
        temp_t3_2 = temp_t7 | 0x20;
        sp44.b[3] = temp_t7;
        sp44.b[3] = temp_t3_2;
        sp44.b[3] = temp_t3_2 & 0x3F;
        func_8006B5BC(sp44);
        func_8006B0C8();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_801413C8[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_801413F8, (s32) D_80141458, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        func_800AE0F0(sp78, "それじゃ、");
        var_a2_2 = D_80140558;
        if (var_a2_2 == 0xD) {
            var_a2_2 = 1;
        }
        D_80140558 = var_a2_2;
        func_800AE100(sp78, D_800B3288[var_a2_2], var_a2_2);
        func_800AE100(sp78, "月");
        temp_a3_2 = D_80140554;
        if (temp_a3_2 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3_2 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3_2 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "の前で 待ち合わせということで。");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_80141680[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_801416B0, (s32) D_80141710, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        temp_a0_2 = D_801418D0[D_801405E4];
        if (*temp_a0_2 == 0x20) {
            D_800E6280.unk_110D = 0x2D;
        } else {
            sndisp(temp_a0_2, 0, 0xF);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 0x27:
        xa_wait();
        break;
    case 0x28:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        temp_a0_3 = D_8014197C[D_801405E4];
        if (*temp_a0_3 == 0x20) {
            D_800E6280.unk_110D = 0x2D;
        } else {
            sndisp(temp_a0_3, 0, (s32) D_800E6280.unk_F5F);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
        }
        func_80052060((s32) D_801419AC, (s32) D_80141A0C, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        temp_a0_4 = D_80141AB0[D_801405E4];
        if (*temp_a0_4 == 0x20) {
            D_800E6280.unk_110D = 0x2D;
        } else {
            sndisp(temp_a0_4, 0, 0xF);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 0x2B:
        xa_wait();
        break;
    case 0x2C:
        D_800E6280.unk_110D = 0x2D;
        break;
    case 0x2D:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x2E:
        xa_wait();
        break;
    case 0x2F:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6280.unk_0D4[0]);
        func_800AE0F0(sp78, "やったー。 ");
        func_800AE100(sp78, D_800B3288[D_80140558], D_80140558);
        func_800AE100(sp78, "月");
        temp_a3_3 = D_80140554;
        if (temp_a3_3 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3_3 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3_3 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "が、楽しみだぜ。 ");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x30:
        xa_wait();
        break;
    case 0x31:
        D_800E6280.unk_110D = 0xFF;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        if ((D_800E6280.unk_F5F == 6) && ((u32) (get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F) < 2U)) {
            D_801405E4 = 0xB;
        }
        var_t1 = D_801405E4;
        if (D_800E6280.unk_F5F == 4) {
            temp_lo = (s32) (D_80140554 + 6) / 7;
            if ((temp_lo == 3) && (D_80140558 == D_800E6280.unk_03F) && (D_800E6280.unk_044[0].unk_00[D_80140554] == 1)) {
                goto block_108;
            }
            if ((temp_lo == 3) && (D_80140558 != D_800E6280.unk_03F)) {
                var_t2 = var_t1 * 4;
                if (D_800E6280.unk_044[1].unk_00[D_80140554] == 1) {
block_108:
                    var_t1 = 0xC;
                    goto block_109;
                }
            } else {
                goto block_109;
            }
        } else {
block_109:
            var_t2 = var_t1 * 4;
        }
        D_801405E4 = var_t1;
        sndisp(*(D_80141D80 + var_t2), 0, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        func_80052060((s32) D_80141DB4, (s32) D_80141E1C, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        D_800E6280.unk_110D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp(D_80141FE0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 0x43:
        xa_wait();
        break;
    case 0x44:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_80142254[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_80142288, (s32) D_801422F0, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp(D_80142570[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x47:
        xa_wait();
        break;
    case 0x48:
        if (D_800E6280.unk_F5F == 9) {
            D_800E6280.unk_110D = 0x4D;
        } else {
            k_sub_reset();
            func_80044750(0x50B);
            sndi("ガチャ", 0, 0x2F);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 0x49:
        xa_wait();
        break;
    case 0x4A:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        func_80044750(0x50C);
        if (D_800E6280.unk_F5F == 5) {
            sndisp("言葉うつっちゃったよ。）", 0, 0x1F);
        } else {
            sndisp("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1F);
        }
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4B:
        xa_wait();
        break;
    case 0x4C:
        D_800E6280.unk_110D = 0xFF;
        break;
    case 0x4D:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp("あの…。 また今度、 誘って下さい…。", 0, (s32) D_800E6280.unk_F5F);
        func_80052060(0x02000163, 0x19000170, -1, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp("うん、そうするよ。 じゃあ。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x50:
        xa_wait();
        break;
    case 0x51:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x52:
        xa_wait();
        break;
    case 0x53:
        D_800E6280.unk_110D = 0x4A;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_80142798[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_801427C8, (s32) D_80142828, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp(D_801429E0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x63:
        xa_wait();
        break;
    case 0x64:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_80142B40[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_80142B70, (s32) D_80142BD0, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (*D_80142CB0[D_801405E4] == 0x20) {
            D_800E6280.unk_110D = 0x68;
        } else {
            k_sub_reset();
            gnsx(&D_800E6280.unk_0D4[0]);
            sndisp(D_80142CB0[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
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
        D_800E6280.unk_110D += 1;
        break;
    case 0x69:
        xa_wait();
        break;
    case 0x6A:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp("あーあ、間が悪かったかな。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x6B:
        xa_wait();
        break;
    case 0x6C:
        D_800E6280.unk_110D = 0xFF;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_80142E70[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_80142EA0, (s32) D_80142F00, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp(D_8014306C[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x83:
        xa_wait();
        break;
    case 0x84:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E6280.unk_F5F]);
        sndisp(D_801431E8[D_801405E4], 0, (s32) D_800E6280.unk_F5F);
        func_80052060((s32) D_80143218, (s32) D_80143278, (s32) D_801405E4, (s32) D_800E6280.unk_F5F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (*D_8014333C[D_801405E4] == 0x20) {
            D_800E6280.unk_110D = 0x88;
        } else {
            k_sub_reset();
            gnsx(&D_800E6280.unk_0D4[0]);
            sndisp(D_8014333C[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            D_800E6280.unk_110D += 1;
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
        D_800E6280.unk_110D += 1;
        break;
    case 0x89:
        xa_wait();
        break;
    case 0x8A:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        func_80044750(0x50C);
        sndisp("やっぱり、だめか…。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x8B:
        xa_wait();
        break;
    case 0x8C:
        D_800E6280.unk_110D = 0xFF;
        break;
    case 0xFD:
        xa_wait();
        break;
    case 0xFE:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        D_800E6280.unk_110D = 0xFD;
        break;
    default:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
}
