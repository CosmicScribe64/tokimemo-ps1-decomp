void func_80136724(void) {
    s32 sp17C;
    u8 sp78;
    s32 sp68[1];
    s32 sp48[1];
    u8 sp47;
    u8 sp46;
    u8 sp45;
    u8 sp44;                                        /* compiler-managed */
    s32 *var_a3;
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
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;

    cal_base_show();
    menu_bar_show(0);
    switch ((u8) D_800E738D) {
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        D_800E738D += 1;
        break;
    case 0x1:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        if (((u32) D_800E7384 >= 0x11U) && (D_800E7208 & 0x20)) {
            if (D_800E7313 == 0xA) {
                func_80044750(0x50D);
                D_800E738D = 2;
            } else if (D_800E7313 >= 0) {
                D_800E738D = 4;
                D_80140550 = (s32) D_800E7313;
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
        D_800E738D += 1;
        break;
    case 0x3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        menu_bar_show(0);
        if (((u32) D_800E7384 >= 0x11U) && (D_800E7208 & 0x20)) {
            if (D_800E7313 == 0xA) {
                func_80044750(0x50D);
                D_800E738D = 0;
            } else if (D_800E7313 >= 0) {
                D_800E738D = 4;
                if (D_800E7313 < 6) {
                    D_80140550 = D_800E7313 + 0xA;
                }
                if ((D_800E7313 == 6) || (D_800E7313 == 7)) {
                    if ((D_800E62BF == 6) || (D_800E62BF == 7) || (D_800E62BF == 8)) {
                        D_80140550 = D_800E7313 + 0xA;
                    } else {
                        D_80140550 = D_800E7313 + 0xD;
                    }
                }
                if (D_800E7313 == 9) {
                    D_80140550 = 0x12;
                }
            }
        }
        break;
    case 0x4:
        k_sub_reset();
        gnsx(&D_800E6354);
        var_a2 = D_80140558;
        if (var_a2 == 0xD) {
            var_a2 = 1;
        }
        D_80140558 = var_a2;
        func_800AE0F0(&sp78, *(&D_800B3288 + (var_a2 * 4)), var_a2);
        func_800AE100(&sp78, "月");
        temp_a3 = D_80140554;
        if (temp_a3 >= 0xA) {
            func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3 / 10) * 4)));
        }
        func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3 % 10) * 4)));
        func_800AE100(&sp78, "日");
        func_800AE100(&sp78, "に、 ");
        func_800AE100(&sp78, *(&D_800B342C + (D_80140550 * 4)));
        func_800AE100(&sp78, "へ 行かない？");
        sndisp(&sp78, 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x5:
        xa_wait();
        break;
    case 0x6:
        func_80053418(&sp68, &sp48);
        temp_t2 = (u8) *(u8 *)((unsigned char *)(sp + D_800E71DF) + (104)) >> 2;
        temp_t3 = (s16) D_800E6392 / 4;
        if (temp_t3 < (s32) temp_t2) {
            var_v1 = temp_t2;
        } else {
            var_v1 = (u32) temp_t3;
        }
        temp_v0 = *(&D_80141134 + ((D_800E71DF * 0x15) + D_80140550));
        if (temp_v0 != 0) {
            var_v1_2 = temp_v0 + var_v1;
        } else {
            var_v1_2 = 0;
        }
        if (D_800E6396 < 0x28) {
            var_v1_2 = (D_800E6396 + var_v1_2) - 0x28;
        }
        sp17C = var_v1_2;
        temp_a0 = func_800AE0D0(temp_t3) & 0xFF;
        temp_v0_2 = var_v1_2 - temp_a0;
        if (temp_a0 < var_v1_2) {
            D_800E738D = 0x20;
        } else {
            if (temp_v0_2 < 0x66) {
                D_800E738D = 0x40;
            } else if (temp_v0_2 < 0xB3) {
                D_800E738D = 0x60;
            } else {
                D_800E738D = 0x80;
            }
            D_800E639E += 9;
            D_800E639A += func_800AE0D0(temp_a0) & 7;
            parameter_show_init();
            parameter_show_init();
        }
        temp_v0_3 = &D_800E6280 + (D_800E71DF * 0x38);
        *(s16 *)((unsigned char *)temp_v0_3 + (450)) = (s16) (*(s16 *)((unsigned char *)temp_v0_3 + (450)) + (D_800E7378 & 3));
        temp_v0_4 = &D_800E6280 + (D_800E71DF * 0x38);
        *(s16 *)((unsigned char *)temp_v0_4 + (454)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (454)) - 0xC);
        temp_v0_5 = &D_800E6280 + (D_800E71DF * 0x38);
        if (*(s16 *)((unsigned char *)temp_v0_5 + (454)) < 0) {
            *(s16 *)((unsigned char *)temp_v0_5 + (454)) = 0;
        }
        if (D_800E71DF == 6) {
            if ((u32) (get_g_zyotai_s(D_800E71DF) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = D_800E71DF;
            }
        } else {
            D_801405E4 = D_800E71DF;
        }
        break;
    case 0x20:
        k_sub_reset();
        sp44 = (D_80140550 & 0x1F) | (sp44 & 0xFFE0);
        sp44 = ((D_8014055C << 5) & 0xFE0) | ((u16) sp44 & 0xF01F);
        temp_t4 = (D_80140554 & 0x1F) | (sp46 & 0xFFE0);
        sp45 = (D_80140558 * 0x10) | (sp45 & 0xFF0F);
        sp46 = temp_t4;
        sp46 = (D_80140560 << 5) | (temp_t4 & 0x1F);
        temp_t2_2 = sp47 & 0xFFFE;
        temp_t7 = ((D_800E71DF * 2) & 0x1E) | (temp_t2_2 & 0xE1);
        sp47 = temp_t2_2;
        temp_t3_2 = temp_t7 | 0x20;
        sp47 = temp_t7;
        sp47 = temp_t3_2;
        sp47 = temp_t3_2 & 0x3F;
        func_8006B5BC((s32) sp44);
        func_8006B0C8();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_801413C8 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801413F8, &D_80141458, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_800AE0F0(&sp78, "それじゃ、");
        var_a2_2 = D_80140558;
        if (var_a2_2 == 0xD) {
            var_a2_2 = 1;
        }
        D_80140558 = var_a2_2;
        func_800AE100(&sp78, *(&D_800B3288 + (var_a2_2 * 4)), var_a2_2);
        func_800AE100(&sp78, "月");
        temp_a3_2 = D_80140554;
        if (temp_a3_2 >= 0xA) {
            func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3_2 / 10) * 4)));
        }
        func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3_2 % 10) * 4)));
        func_800AE100(&sp78, "日");
        func_800AE100(&sp78, "に、 ");
        func_800AE100(&sp78, *(&D_800B342C + (D_80140550 * 4)));
        func_800AE100(&sp78, "の前で 待ち合わせということで。");
        sndisp(&sp78, 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_80141680 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801416B0, &D_80141710, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(&D_800E6354);
        temp_a0_2 = *(&D_801418D0 + (D_801405E4 * 4));
        if (*temp_a0_2 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_2, 0, 0xFU);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x27:
        xa_wait();
        break;
    case 0x28:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        temp_a0_3 = *(&D_8014197C + (D_801405E4 * 4));
        if (*temp_a0_3 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_3, 0, D_800E71DF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        func_80052060(&D_801419AC, &D_80141A0C, D_801405E4, D_800E71DF);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        k_sub_reset();
        gnsx(&D_800E6354);
        temp_a0_4 = *(&D_80141AB0 + (D_801405E4 * 4));
        if (*temp_a0_4 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_4, 0, 0xFU);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x2B:
        xa_wait();
        break;
    case 0x2C:
        D_800E738D = 0x2D;
        break;
    case 0x2D:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x2E:
        xa_wait();
        break;
    case 0x2F:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6354);
        func_800AE0F0(&sp78, "やったー。 ");
        func_800AE100(&sp78, *(&D_800B3288 + (D_80140558 * 4)), D_80140558);
        func_800AE100(&sp78, "月");
        temp_a3_3 = D_80140554;
        if (temp_a3_3 >= 0xA) {
            func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3_3 / 10) * 4)));
        }
        func_800AE100(&sp78, *(&D_800B3288 + ((temp_a3_3 % 10) * 4)));
        func_800AE100(&sp78, "日");
        func_800AE100(&sp78, "が、楽しみだぜ。 ");
        sndisp(&sp78, 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x30:
        xa_wait();
        break;
    case 0x31:
        D_800E738D = 0xFF;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        if ((D_800E71DF == 6) && ((u32) (get_g_zyotai_s(D_800E71DF) & 0x7F) < 2U)) {
            D_801405E4 = 0xB;
        }
        var_t1 = D_801405E4;
        var_a3 = &D_80140554;
        if (D_800E71DF == 4) {
            var_a3 = (s32 *) D_80140554;
            temp_lo = (s32) (var_a3 + 6) / 7;
            if ((temp_lo == 3) && (D_80140558 == D_800E62BF) && (*(u8 *)((unsigned char *)(&D_800E6280 + var_a3) + (68)) == 1)) {
                goto block_108;
            }
            if ((temp_lo == 3) && (D_80140558 != D_800E62BF)) {
                var_t2 = var_t1 * 4;
                if (*(u8 *)((unsigned char *)(&D_800E6280 + var_a3) + (132)) == 1) {
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
        sndisp(*(&D_80141D80 + var_t2), 0, D_800E71DF, (s32) var_a3);
        k_sub_disp_start(0);
        func_80052060(&D_80141DB4, &D_80141E1C, D_801405E4, D_800E71DF);
        D_800E738D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(*(&D_80141FE0 + (D_801405E4 * 4)), 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        /* fallthrough */
    case 0x43:
        xa_wait();
        break;
    case 0x44:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_80142254 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142288, &D_801422F0, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(*(&D_80142570 + (D_801405E4 * 4)), 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x47:
        xa_wait();
        break;
    case 0x48:
        if (D_800E71DF == 9) {
            D_800E738D = 0x4D;
        } else {
            k_sub_reset();
            func_80044750(0x50B);
            sndi("ガチャ", 0, 0x2F);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x49:
        xa_wait();
        break;
    case 0x4A:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_80044750(0x50C);
        if (D_800E71DF == 5) {
            sndisp("言葉うつっちゃったよ。）", 0, 0x1FU);
        } else {
            sndisp("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1FU);
        }
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x4B:
        xa_wait();
        break;
    case 0x4C:
        D_800E738D = 0xFF;
        break;
    case 0x4D:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp("あの…。 また今度、 誘って下さい…。", 0, D_800E71DF);
        func_80052060((s32 *)0x02000163, (s32 *)0x19000170, -1U, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp("うん、そうするよ。 じゃあ。", 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x50:
        xa_wait();
        break;
    case 0x51:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x52:
        xa_wait();
        break;
    case 0x53:
        D_800E738D = 0x4A;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_80142798 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801427C8, &D_80142828, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(*(&D_801429E0 + (D_801405E4 * 4)), 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x63:
        xa_wait();
        break;
    case 0x64:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_80142B40 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142B70, &D_80142BD0, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (**(&D_80142CB0 + (D_801405E4 * 4)) == 0x20) {
            D_800E738D = 0x68;
        } else {
            k_sub_reset();
            gnsx(&D_800E6354);
            sndisp(*(&D_80142CB0 + (D_801405E4 * 4)), 0, 0xFU);
            k_sub_disp_start(0);
            D_800E738D += 1;
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
        D_800E738D += 1;
        break;
    case 0x69:
        xa_wait();
        break;
    case 0x6A:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6354);
        sndisp("あーあ、間が悪かったかな。", 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x6B:
        xa_wait();
        break;
    case 0x6C:
        D_800E738D = 0xFF;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_80142E70 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142EA0, &D_80142F00, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(*(&D_8014306C + (D_801405E4 * 4)), 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x83:
        xa_wait();
        break;
    case 0x84:
        k_sub_reset();
        gnsx(*(&girl_short_name_table + (D_800E71DF * 4)));
        sndisp(*(&D_801431E8 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80143218, &D_80143278, D_801405E4, D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (**(&D_8014333C + (D_801405E4 * 4)) == 0x20) {
            D_800E738D = 0x88;
        } else {
            k_sub_reset();
            gnsx(&D_800E6354);
            sndisp(*(&D_8014333C + (D_801405E4 * 4)), 0, 0xFU);
            k_sub_disp_start(0);
            D_800E738D += 1;
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
        D_800E738D += 1;
        break;
    case 0x89:
        xa_wait();
        break;
    case 0x8A:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_80044750(0x50C);
        sndisp("やっぱり、だめか…。", 0, 0xFU);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x8B:
        xa_wait();
        break;
    case 0x8C:
        D_800E738D = 0xFF;
        break;
    case 0xFD:
        xa_wait();
        break;
    case 0xFE:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        D_800E738D = 0xFD;
        break;
    default:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
}
