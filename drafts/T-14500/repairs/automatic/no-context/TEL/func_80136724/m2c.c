? func_80044750(?);                                 /* extern */
? func_80048E78();                                  /* extern */
? func_8004E58C();                                  /* extern */
? func_8004E788(?, ?, ?, ? *, s32);                 /* extern */
? func_8004E884(s32);                               /* extern */
? func_8004EA98();                                  /* extern */
? func_8004EAAC();                                  /* extern */
? func_8004EAD4(?);                                 /* extern */
s32 func_8004ECE0();                                /* extern */
? func_8004F984(?, s16, s16);                       /* extern */
? func_8004FC10(?);                                 /* extern */
? func_80050B54(?);                                 /* extern */
? func_80050DFC(? *);                               /* extern */
? func_80050E8C(u8 *, ?, u8, s32);                  /* extern */
? func_80051010(? *, ?, ?);                         /* extern */
s32 func_80051A68(u8);                              /* extern */
? func_80051DBC();                                  /* extern */
? func_80052060(? *, ? *, u8, u8);                  /* extern */
? func_8005215C();                                  /* extern */
? func_80053418(? *, ? *);                          /* extern */
? func_80057390(?);                                 /* extern */
? func_800639D8();                                  /* extern */
? func_80069128();                                  /* extern */
? func_8006B0C8();                                  /* extern */
? func_8006B5BC(s32);                               /* extern */
? func_80072B5C(?);                                 /* extern */
s32 func_800AE0D0(s32);                             /* extern */
? func_800AE0F0(u8 *, ? *, s32);                    /* extern */
? func_800AE100(u8 *, ? *, s32);                    /* extern */
? func_8013D88C(?, s32, s32, s32);                  /* extern */
extern ? D_800B3288;
extern ? D_800B342C;
extern ? D_800E6280;
extern u8 D_800E62BF;
extern ? D_800E6354;
extern s16 D_800E6392;
extern s16 D_800E6396;
extern s16 D_800E639A;
extern s16 D_800E639E;
extern u8 D_800E71DF;
extern s32 D_800E7208;
extern s8 D_800E7313;
extern s32 D_800E7378;
extern u32 D_800E7384;
extern u8 D_800E738D;
extern s16 D_8011ECF6;
extern s16 D_8011ECFA;
extern s32 D_80140550;
extern s32 D_80140554;
extern s32 D_80140558;
extern s32 D_8014055C;
extern s32 D_80140560;
extern u8 D_801405E4;
extern ? D_80141134;
extern ? D_801413C8;
extern ? D_801413F8;
extern ? D_80141458;
extern ? D_80141680;
extern ? D_801416B0;
extern ? D_80141710;
extern ? D_801418D0;
extern ? D_8014197C;
extern ? D_801419AC;
extern ? D_80141A0C;
extern ? D_80141AB0;
extern ? D_80141D80;
extern ? D_80141DB4;
extern ? D_80141E1C;
extern ? D_80141FE0;
extern ? D_80142254;
extern ? D_80142288;
extern ? D_801422F0;
extern ? D_80142570;
extern ? D_80142798;
extern ? D_801427C8;
extern ? D_80142828;
extern ? D_801429E0;
extern ? D_80142B40;
extern ? D_80142B70;
extern ? D_80142BD0;
extern ? D_80142CB0;
extern ? D_80142E70;
extern ? D_80142EA0;
extern ? D_80142F00;
extern ? D_8014306C;
extern ? D_801431E8;
extern ? D_80143218;
extern ? D_80143278;
extern ? D_8014333C;
extern ? girl_short_name_table;

void func_80136724(void) {
    s32 sp17C;
    u8 sp78;
    ? sp68;
    ? sp48;
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
    void *temp_v0_3;
    void *temp_v0_4;
    void *temp_v0_5;

    func_80069128();
    func_80050B54(0);
    switch ((u8) D_800E738D) {                      /* irregular */
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        func_8004EA98();
        func_8004E788(-0x80, 0x32, 0, "デートする場所を選んで下さい", 0);
        func_8004E884(func_8004ECE0());
        D_800E738D += 1;
        break;
    case 0x1:
        func_8004F984(0, D_8011ECF6, D_8011ECFA);
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
        func_8004EA98();
        func_8004E788(-0x80, 0x32, 0, "デートする場所を選んで下さい", 0);
        func_8004E884(func_8004ECE0());
        D_800E738D += 1;
        break;
    case 0x3:
        func_8004F984(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        func_80050B54(0);
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
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
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
        func_80050E8C(&sp78, 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x5:
        func_80051DBC();
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
            func_800639D8();
            func_800639D8();
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
            if ((u32) (func_80051A68(D_800E71DF) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = D_800E71DF;
            }
        } else {
            D_801405E4 = D_800E71DF;
        }
        break;
    case 0x20:
        func_8004EAAC();
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
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_801413C8 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801413F8, &D_80141458, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
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
        func_80050E8C(&sp78, 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x23:
        func_80051DBC();
        break;
    case 0x24:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_80141680 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801416B0, &D_80141710, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        temp_a0_2 = *(&D_801418D0 + (D_801405E4 * 4));
        if (*temp_a0_2 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            func_80050E8C(temp_a0_2, 0, 0xFU);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        break;
    case 0x27:
        func_80051DBC();
        break;
    case 0x28:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        temp_a0_3 = *(&D_8014197C + (D_801405E4 * 4));
        if (*temp_a0_3 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            func_80050E8C(temp_a0_3, 0, D_800E71DF);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        func_80052060(&D_801419AC, &D_80141A0C, D_801405E4, D_800E71DF);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        temp_a0_4 = *(&D_80141AB0 + (D_801405E4 * 4));
        if (*temp_a0_4 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            func_80050E8C(temp_a0_4, 0, 0xFU);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        break;
    case 0x2B:
        func_80051DBC();
        break;
    case 0x2C:
        D_800E738D = 0x2D;
        break;
    case 0x2D:
        func_8004EAAC();
        func_80044750(0x50B);
        func_80051010("ガチャ", 0, 0x2F);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x2E:
        func_80051DBC();
        break;
    case 0x2F:
        func_8004EAAC();
        func_80044750(0x50C);
        func_80050DFC(&D_800E6354);
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
        func_80050E8C(&sp78, 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x30:
        func_80051DBC();
        break;
    case 0x31:
        D_800E738D = 0xFF;
        break;
    case 0x40:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        if ((D_800E71DF == 6) && ((u32) (func_80051A68(D_800E71DF) & 0x7F) < 2U)) {
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
        func_80050E8C(*(&D_80141D80 + var_t2), 0, D_800E71DF, (s32) var_a3);
        func_8004EAD4(0);
        func_80052060(&D_80141DB4, &D_80141E1C, D_801405E4, D_800E71DF);
        D_800E738D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C(*(&D_80141FE0 + (D_801405E4 * 4)), 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        /* fallthrough */
    case 0x43:
        func_80051DBC();
        break;
    case 0x44:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_80142254 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142288, &D_801422F0, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C(*(&D_80142570 + (D_801405E4 * 4)), 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x47:
        func_80051DBC();
        break;
    case 0x48:
        if (D_800E71DF == 9) {
            D_800E738D = 0x4D;
        } else {
            func_8004EAAC();
            func_80044750(0x50B);
            func_80051010("ガチャ", 0, 0x2F);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        break;
    case 0x49:
        func_80051DBC();
        break;
    case 0x4A:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80044750(0x50C);
        if (D_800E71DF == 5) {
            func_80050E8C("言葉うつっちゃったよ。）", 0, 0x1FU);
        } else {
            func_80050E8C("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1FU);
        }
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x4B:
        func_80051DBC();
        break;
    case 0x4C:
        D_800E738D = 0xFF;
        break;
    case 0x4D:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C("あの…。 また今度、 誘って下さい…。", 0, D_800E71DF);
        func_80052060((? *)0x02000163, (? *)0x19000170, -1U, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C("うん、そうするよ。 じゃあ。", 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x50:
        func_80051DBC();
        break;
    case 0x51:
        func_8004EAAC();
        func_80044750(0x50B);
        func_80051010("ガチャ", 0, 0x2F);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x52:
        func_80051DBC();
        break;
    case 0x53:
        D_800E738D = 0x4A;
        break;
    case 0x60:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_80142798 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_801427C8, &D_80142828, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C(*(&D_801429E0 + (D_801405E4 * 4)), 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x63:
        func_80051DBC();
        break;
    case 0x64:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_80142B40 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142B70, &D_80142BD0, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (**(&D_80142CB0 + (D_801405E4 * 4)) == 0x20) {
            D_800E738D = 0x68;
        } else {
            func_8004EAAC();
            func_80050DFC(&D_800E6354);
            func_80050E8C(*(&D_80142CB0 + (D_801405E4 * 4)), 0, 0xFU);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        break;
    case 0x67:
        func_80051DBC();
        break;
    case 0x68:
        func_8004EAAC();
        func_80044750(0x50B);
        func_80051010("ガチャ", 0, 0x2F);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x69:
        func_80051DBC();
        break;
    case 0x6A:
        func_8004EAAC();
        func_80044750(0x50C);
        func_80050DFC(&D_800E6354);
        func_80050E8C("あーあ、間が悪かったかな。", 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x6B:
        func_80051DBC();
        break;
    case 0x6C:
        D_800E738D = 0xFF;
        break;
    case 0x80:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_80142E70 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80142EA0, &D_80142F00, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C(*(&D_8014306C + (D_801405E4 * 4)), 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x83:
        func_80051DBC();
        break;
    case 0x84:
        func_8004EAAC();
        func_80050DFC(*(&girl_short_name_table + (D_800E71DF * 4)));
        func_80050E8C(*(&D_801431E8 + (D_801405E4 * 4)), 0, D_800E71DF);
        func_80052060(&D_80143218, &D_80143278, D_801405E4, D_800E71DF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (**(&D_8014333C + (D_801405E4 * 4)) == 0x20) {
            D_800E738D = 0x88;
        } else {
            func_8004EAAC();
            func_80050DFC(&D_800E6354);
            func_80050E8C(*(&D_8014333C + (D_801405E4 * 4)), 0, 0xFU);
            func_8004EAD4(0);
            D_800E738D += 1;
        }
        break;
    case 0x87:
        func_80051DBC();
        break;
    case 0x88:
        func_8004EAAC();
        func_80044750(0x50B);
        func_80051010("ガチャ", 0, 0x2F);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x89:
        func_80051DBC();
        break;
    case 0x8A:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80044750(0x50C);
        func_80050E8C("やっぱり、だめか…。", 0, 0xFU);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 0x8B:
        func_80051DBC();
        break;
    case 0x8C:
        D_800E738D = 0xFF;
        break;
    case 0xFD:
        func_80051DBC();
        break;
    case 0xFE:
        func_8004EAAC();
        func_80044750(0x50B);
        func_80051010("ガチャ", 0, 0x2F);
        D_800E738D = 0xFD;
        break;
    default:
        func_8004E58C();
        func_80048E78();
        func_80057390(0);
        func_80072B5C(0);
        break;
    }
}
