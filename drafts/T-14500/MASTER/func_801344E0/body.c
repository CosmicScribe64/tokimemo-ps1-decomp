void func_801344E0(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    u8 *sp34;                                       /* compiler-managed */
    MasterPoint2 *sp30;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    MasterPoint2 *var_v1_2;
    s16 *var_a0;
    s16 temp_t1;
    s16 temp_t1_2;
    s16 temp_t1_3;
    s16 temp_t3;
    s16 temp_t3_2;
    s16 temp_t3_3;
    s16 temp_t5;
    s16 temp_t5_2;
    s16 temp_t5_3;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t7_3;
    s32 *var_v1_3;
    s32 *var_v1_5;
    s32 temp_hi;
    s32 temp_v0;
    u8 *var_v1;
    u8 *var_v1_4;
    u8 temp_t0;
    u8 temp_t0_2;
    u8 temp_t1_4;
    u8 temp_t1_5;
    u8 temp_t2;
    u8 temp_t2_2;
    u8 temp_t3_4;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t5_4;
    u8 temp_t6;
    u8 temp_t7_4;
    u8 temp_t8;

    switch (D_800E738A) {
    case 0:
        if ((u8) D_8013C578 >= 3U) {
            D_800E699E = 0;
            temp_v0 = D_8013C570 + 1;
            D_8013C570 = temp_v0;
            switch (temp_v0) {
            case 1:
            case 2:
                break;
            case 3:
                D_800E643E += 0xF;
                var_v0 = &D_800E62B8;
                do {
                    temp_t3 = *(s16 *)((unsigned char *)var_v0 + (502));
                    temp_t5 = *(s16 *)((unsigned char *)var_v0 + (558));
                    temp_t7 = *(s16 *)((unsigned char *)var_v0 + (614));
                    temp_t1 = *(s16 *)((unsigned char *)var_v0 + (446));
                    var_v0 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0 + (390)) = (s16) (temp_t7 + 0xF);
                    *(s16 *)((unsigned char *)var_v0 + (334)) = (s16) (temp_t5 + 0xF);
                    *(s16 *)((unsigned char *)var_v0 + (278)) = (s16) (temp_t3 + 0xF);
                    *(s16 *)((unsigned char *)var_v0 + (222)) = (s16) (temp_t1 + 0xF);
                } while (var_v0 != &D_800E6558);
                break;
            case 4:
                D_800E643E += 3;
                var_v0_2 = &D_800E62B8;
                do {
                    temp_t3_2 = *(s16 *)((unsigned char *)var_v0_2 + (502));
                    temp_t5_2 = *(s16 *)((unsigned char *)var_v0_2 + (558));
                    temp_t7_2 = *(s16 *)((unsigned char *)var_v0_2 + (614));
                    temp_t1_2 = *(s16 *)((unsigned char *)var_v0_2 + (446));
                    var_v0_2 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_2 + (390)) = (s16) (temp_t7_2 + 3);
                    *(s16 *)((unsigned char *)var_v0_2 + (334)) = (s16) (temp_t5_2 + 3);
                    *(s16 *)((unsigned char *)var_v0_2 + (278)) = (s16) (temp_t3_2 + 3);
                    *(s16 *)((unsigned char *)var_v0_2 + (222)) = (s16) (temp_t1_2 + 3);
                } while (var_v0_2 != &D_800E6558);
                break;
            case 5:
                D_800E643E += 4;
                var_v0_3 = &D_800E62B8;
                do {
                    temp_t3_3 = *(s16 *)((unsigned char *)var_v0_3 + (502));
                    temp_t5_3 = *(s16 *)((unsigned char *)var_v0_3 + (558));
                    temp_t7_3 = *(s16 *)((unsigned char *)var_v0_3 + (614));
                    temp_t1_3 = *(s16 *)((unsigned char *)var_v0_3 + (446));
                    var_v0_3 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t7_3 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t5_3 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t3_3 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (222)) = (s16) (temp_t1_3 + 4);
                } while (var_v0_3 != &D_800E6558);
                break;
            }
            func_80072338();
            D_800E699C += 1;
            hizuke_init();
            func_80044750(0x200);
            func_80042808();
        } else {
            cd_read_sectors(0x6EU, 0x801B4400, 0xBBA5);
            tpage_buf_clear();
            D_800B5BD4 = 0xA;
            D_8013C588 = 0;
            func_8004284C();
        }
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) &D_800E6354, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に敗北したくやしさが", 0);
        set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の、眠れる力を", 0);
        set_kanji_string(-0x56, 0x52, 0, (s32) "　呼びおこした！！）", 0);
        func_8004284C();
        break;
    case 3:
        temp_t2 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t2;
        if ((temp_t2 >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            D_8013C588 = 0;
        }
        if (D_800E7208 & 0xF0) {
            k_reset(1);
            D_8011ED5B &= 0x7F;
            func_8004284C();
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80133724();
            load_palette(D_8013C4D0, 0x10, 1, 0, 0);
            load_palette(D_8013C4D0, 0x11, 1, 0, 1);
            func_80048F64(0x60);
            D_80120651 = 0xB;
            D_80120652 = 0;
            D_80120688 = 0x01000000;
            D_80120653 = 4;
            D_8012065C = (u8 *) D_8013C4B8;
            D_80120660 = (u8 *) D_8013C4C0;
            D_80120684 = (u8 *) D_8013C4B0;
            D_80120664 = D_8013C4C8;
            D_80120666 = 0;
            D_80120668 = 0;
            D_80120656 = 1;
            D_80120693 = 0;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            D_80120657 = 0;
            D_80120655 = 0x80;
            D_80120654 = 0;
            *(M2cBlock44 *)(D_80120694) = *(M2cBlock44 *)(D_80120650);
            D_80120696 = 0x40;
            D_801206A0 = D_8013C4BC;
            D_801206A4 = D_8013C4C4;
            D_801206C8 = D_8013C4B4;
            D_801206A8 = D_8013C4CC;
            var_v1 = D_801206D8;
            D_801206BA = (func_800AE0D0() % 320) - 0xA0;
            D_801206BE = 0x3C;
            do {
                *(M2cBlock44 *)(var_v1) = *(M2cBlock44 *)(D_80120694);
                sp34 = var_v1;
                var_v1 += 0x44;
                *(s16 *)((unsigned char *)var_v1 + (-44)) = (s16) (func_800AE0D0() % 10);
            } while ((u32) var_v1 < (u32) &D_80120BA0);
            var_v1_2 = D_8013C58C;
            var_a0 = D_8013C5B4;
            do {
                sp30 = var_v1_2;
                sp34 = var_a0;
                *var_a0 = (func_800AE0D0() % 5) + 1;
                temp_hi = func_800AE0D0() % 320;
                var_v1_2 = &var_v1_2[1];
                *(s16 *)((unsigned char *)var_v1_2 + (-2)) = 0x3C;
                var_a0 = &var_a0[1];
                *(s16 *)((unsigned char *)var_v1_2 + (-4)) = (s16) (temp_hi - 0xA0);
            } while (var_v1_2 != D_8013C5B4);
            hizuke_disp_switch(0);
            func_80065F34(0);
            D_8013C588 = 0xF;
            func_8004284C();
        }
        break;
    case 5:
        set_kanji_string(-0x80, 0x32, 0, (s32) &D_800E6354, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        func_80044750(0x203);
        func_8004284C();
        break;
    case 6:
        func_80137BF4();
        D_80120653 |= 0x80;
        D_80120657 += 8;
        D_80120697 |= 0x80;
        D_8012069B += 8;
        *(u8 *)((unsigned char *)D_801206D8 + (3)) = (u8) (*(u8 *)((unsigned char *)D_801206D8 + (3)) | 0x80);
        D_801206DF += 8;
        D_8012071F |= 0x80;
        D_80120723 += 8;
        var_v1_3 = &D_80120760;
        do {
            temp_t2_2 = *(u8 *)((unsigned char *)var_v1_3 + (207));
            temp_t4 = *(u8 *)((unsigned char *)var_v1_3 + (143));
            temp_t3_4 = *(u8 *)((unsigned char *)var_v1_3 + (211));
            temp_t8 = *(u8 *)((unsigned char *)var_v1_3 + (75));
            temp_t0 = *(u8 *)((unsigned char *)var_v1_3 + (139));
            *(u8 *)((unsigned char *)var_v1_3 + (71)) = (u8) (*(u8 *)((unsigned char *)var_v1_3 + (71)) | 0x80);
            *(u8 *)((unsigned char *)var_v1_3 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v1_3 + (3)) | 0x80);
            *(u8 *)((unsigned char *)var_v1_3 + (7)) = (u8) (*(u8 *)((unsigned char *)var_v1_3 + (7)) + 8);
            var_v1_3 += 0x110;
            *(s8 *)((unsigned char *)var_v1_3 + (-133)) = (s8) (temp_t0 | 0x80);
            *(s8 *)((unsigned char *)var_v1_3 + (-197)) = (s8) (temp_t8 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-61)) = (s8) (temp_t3_4 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-129)) = (s8) (temp_t4 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-65)) = (s8) (temp_t2_2 | 0x80);
        } while (var_v1_3 != &D_80120BA0);
        D_80120652 = 5;
        if ((u8) D_80120657 >= 0x80U) {
            D_80120657 = 0x80;
            D_8012069B = 0x80;
            func_8004284C();
        }
        break;
    case 7:
        func_80137BF4();
        temp_t4_2 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t4_2;
        if ((temp_t4_2 >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            D_8013C588 = 0;
        }
        if (D_800E7208 & 0xF0) {
            k_reset(1);
            D_8011ED5B &= 0x7F;
            D_80120666 = 2;
            D_80120668 = 0;
            D_80120658 = 0;
            D_80120652 = 5;
            func_8004284C();
        }
        break;
    case 8:
        func_80137BF4();
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 2, (s32) "　　野球部奥義", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "　　　「超球百裂弾」", 0);
        set_kanji_string(-0x80, 0x52, 0, (s32) "　　　　　　をマスターした。", 0);
        D_8013C588 = 0;
        func_8004284C();
        /* fallthrough */
    case 9:
        func_80137BF4();
        temp_t1_4 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t1_4;
        if ((temp_t1_4 >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            D_8013C588 = 0;
        }
        if (D_800E7208 & 0xF0) {
            k_reset(1);
            D_8011ED5B &= 0x7F;
            func_8004284C();
        }
        break;
    case 10:
        func_80137BF4();
        D_80120657 -= 8;
        var_v1_4 = D_80120694;
        do {
            temp_t0_2 = *(u8 *)((unsigned char *)var_v1_4 + (7));
            var_v1_4 += 0x44;
            *(s8 *)((unsigned char *)var_v1_4 + (-61)) = (s8) (temp_t0_2 - 8);
        } while ((u32) var_v1_4 < (u32) &D_80120BA0);
        if ((u8) D_80120657 < 9U) {
            D_80120657 = 0;
            D_80120653 &= 0xFF7F;
            D_8012069B = 0;
            D_80120697 &= 0xFF7F;
            D_801206DF = 0;
            *(u8 *)((unsigned char *)D_801206D8 + (3)) = (u8) (*(u8 *)((unsigned char *)D_801206D8 + (3)) & 0xFF7F);
            D_80120723 = 0;
            D_8012071F &= 0xFF7F;
            var_v1_5 = &D_80120760;
            do {
                temp_t5_4 = *(u8 *)((unsigned char *)var_v1_5 + (71));
                temp_t6 = *(u8 *)((unsigned char *)var_v1_5 + (139));
                temp_t7_4 = *(u8 *)((unsigned char *)var_v1_5 + (207));
                temp_t1_5 = *(u8 *)((unsigned char *)var_v1_5 + (3));
                var_v1_5 += 0x110;
                *(s8 *)((unsigned char *)var_v1_5 + (-65)) = (s8) (temp_t7_4 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-133)) = (s8) (temp_t6 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-201)) = (s8) (temp_t5_4 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-197)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-129)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-61)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-265)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-269)) = (s8) (temp_t1_5 & ~0x80);
            } while (var_v1_5 != &D_80120BA0);
            D_800B5BD4 = 0xF;
            func_80041584();
            func_8004284C();
        }
        break;
    case 11:
        D_800E699E |= 0x10;
        func_8008585C();
        set_dec_bri(0);
        func_8006D6E0();
        func_80072B5C(1);
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
