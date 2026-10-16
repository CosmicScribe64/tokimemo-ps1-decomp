void func_801363F4(void) {
    MasterSprite44 *sp34;                           /* compiler-managed */
    MasterPoint2 *sp30;
    MasterPoint2 *var_v1_2;
    MasterSprite44 *var_v1;
    MasterSprite44 *var_v1_3;
    MasterSprite44 *var_v1_4;
    MasterSprite44 *var_v1_5;
    s16 *var_a0;
    s32 temp_hi;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t1_2;
    u8 temp_t2;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t7_2;
    u8 temp_t8;
    u8 temp_t8_2;
    u8 temp_t9;
    u8 temp_t9_2;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {
    case 0:
        func_80044750(0xC1);
        cd_read_sectors(0x6EU, 0x801B4400, 0xBBA5);
        func_80048390();
        D_800B5BD4 = 0xA;
        D_8013C588 = 0;
        func_8004284C();
        break;
    case 1:
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
        func_8004E884(2);
        if ((u8) D_8013C578 >= 3U) {
            func_8004E788(-0x56, 0x32, 0, (s32) "（大会に勝利した興奮が", 0);
            func_8004E788(-0x56, 0x42, 0, (s32) "　俺の体に", 0);
            func_8004E788(-0x56, 0x52, 0, (s32) "　新たな力を与えた！！）", 0);
        } else {
            func_8004E788(-0x56, 0x32, 0, (s32) "（大会に敗北したくやしさが", 0);
            func_8004E788(-0x56, 0x42, 0, (s32) "　俺の、眠れる力を", 0);
            func_8004E788(-0x56, 0x52, 0, (s32) "　呼びおこした！！）", 0);
        }
        func_8004284C();
        break;
    case 2:
        temp_t9 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t9;
        if ((temp_t9 >= 0x10U) && (func_8004ECB4() != 0)) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (func_8004ECB4() != 0)) {
            func_8004E9F4(1);
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x7F);
            func_8004284C();
        }
        break;
    case 3:
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80133724();
            func_80043914(D_8013C4D0, 0x10, 1, 0, 0);
            func_80043914(D_8013C4D0, 0x11, 1, 0, 1);
            func_80048F64(0x60);
            D_80120650->unk_01 = 0xB;
            D_80120650->unk_02 = 0;
            D_80120650->unk_38 = 0x01000000;
            D_80120650->unk_03 = 4;
            D_80120650->unk_0C = (u8 *) D_8013C4B8;
            D_80120650->unk_10 = (u8 *) D_8013C4C0;
            D_80120650->unk_34 = (u8 *) D_8013C4B0;
            D_80120650->unk_14 = D_8013C4C8;
            D_80120650->unk_16 = 0;
            D_80120650->unk_18 = 0;
            D_80120650->unk_06 = 1;
            D_80120650->unk_43 = 0;
            D_80120650->unk_26 = -0xA0;
            D_80120650->unk_28.h[1] = -0x78;
            D_80120650->unk_07 = 0;
            D_80120650->unk_05 = 0x80;
            D_80120650->unk_04 = 0;
            M2C_MEMCPY_ALIGNED(D_80120650 + 0x44, D_80120650, 0x44);
            *(s8 *)((unsigned char *)D_80120650 + (70)) = 0x40;
            *(s32 *)((unsigned char *)D_80120650 + (80)) = (s32) D_8013C4BC;
            *(s32 *)((unsigned char *)D_80120650 + (84)) = (s32) D_8013C4C4;
            *(s32 *)((unsigned char *)D_80120650 + (120)) = (s32) D_8013C4B4;
            *(s16 *)((unsigned char *)D_80120650 + (88)) = (s16) D_8013C4CC;
            var_v1 = D_80120650 + 0x88;
            *(s16 *)((unsigned char *)D_80120650 + (106)) = (s16) ((func_800AE0D0() % 320) - 0xA0);
            *(s16 *)((unsigned char *)D_80120650 + (110)) = 0x3C;
            do {
                M2C_MEMCPY_ALIGNED(var_v1, D_80120650 + 0x44, 0x44);
                sp34 = var_v1;
                var_v1 += 0x44;
                *(s16 *)((unsigned char *)var_v1 + (-44)) = (s16) (func_800AE0D0() % 10);
            } while ((u32) var_v1 < (u32) (D_80120650 + 0x550));
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
            func_8006492C(0);
            func_80065F34(0);
            D_8013C588 = 0xF;
            func_8004284C();
        }
        break;
    case 5:
        func_8004E788(-0x80, 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
        func_8004E884(2);
        func_8004E788(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        func_80044750(0x203);
        func_8004284C();
        break;
    case 6:
        func_80137BF4();
        D_80120650->unk_03 = D_80120650->unk_03 | 0x80;
        D_80120650->unk_07 = D_80120650->unk_07 + 8;
        *(u8 *)((unsigned char *)D_80120650 + (71)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (71)) | 0x80);
        *(u8 *)((unsigned char *)D_80120650 + (75)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (75)) + 8);
        *(u8 *)((unsigned char *)D_80120650 + (139)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (139)) | 0x80);
        *(u8 *)((unsigned char *)D_80120650 + (143)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (143)) + 8);
        *(u8 *)((unsigned char *)D_80120650 + (207)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (207)) | 0x80);
        *(u8 *)((unsigned char *)D_80120650 + (211)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (211)) + 8);
        var_v1_3 = D_80120650 + 0x110;
        do {
            temp_t9_2 = *(u8 *)((unsigned char *)var_v1_3 + (207));
            temp_t1 = *(u8 *)((unsigned char *)var_v1_3 + (143));
            temp_t0 = *(u8 *)((unsigned char *)var_v1_3 + (211));
            temp_t5 = *(u8 *)((unsigned char *)var_v1_3 + (75));
            temp_t7 = *(u8 *)((unsigned char *)var_v1_3 + (139));
            *(u8 *)((unsigned char *)var_v1_3 + (71)) = (u8) (*(u8 *)((unsigned char *)var_v1_3 + (71)) | 0x80);
            var_v1_3->unk_03 |= 0x80;
            var_v1_3->unk_07 += 8;
            var_v1_3 += 0x110;
            *(s8 *)((unsigned char *)var_v1_3 + (-133)) = (s8) (temp_t7 | 0x80);
            *(s8 *)((unsigned char *)var_v1_3 + (-197)) = (s8) (temp_t5 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-61)) = (s8) (temp_t0 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-129)) = (s8) (temp_t1 + 8);
            *(s8 *)((unsigned char *)var_v1_3 + (-65)) = (s8) (temp_t9_2 | 0x80);
        } while (var_v1_3 != (D_80120650 + 0x550));
        D_80120650->unk_02 = 5;
        if ((u8) D_80120650->unk_07 >= 0x80U) {
            D_80120650->unk_07 = 0x80;
            *(u8 *)((unsigned char *)D_80120650 + (75)) = 0x80U;
            func_8004284C();
        }
        break;
    case 7:
        func_80137BF4();
        temp_t1_2 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t1_2;
        if ((temp_t1_2 >= 0x10U) && (func_8004ECB4() != 0)) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (func_8004ECB4() != 0)) {
            func_8004E9F4(1);
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x7F);
            D_80120650->unk_16 = 2;
            D_80120650->unk_18 = 0;
            D_80120650->unk_08 = 0;
            D_80120650->unk_02 = 5;
            func_8004284C();
        }
        break;
    case 8:
        func_80137BF4();
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 2, (s32) "　　野球部奥義", 0);
        func_8004E788(-0x80, 0x42, 1, (s32) "　　　「超球百裂弾」", 0);
        func_8004E788(-0x80, 0x52, 0, (s32) "　　　　　　をマスターした。", 0);
        D_8013C588 = 0;
        func_8004284C();
        /* fallthrough */
    case 9:
        func_80137BF4();
        temp_t8 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t8;
        if ((temp_t8 >= 0x10U) && (func_8004ECB4() != 0)) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (func_8004ECB4() != 0)) {
            func_8004E9F4(1);
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x7F);
            func_8004284C();
        }
        break;
    case 10:
        func_80137BF4();
        D_80120650->unk_07 = D_80120650->unk_07 - 8;
        var_v1_4 = D_80120650 + 0x44;
        do {
            temp_t7_2 = var_v1_4->unk_07;
            var_v1_4 += 0x44;
            *(s8 *)((unsigned char *)var_v1_4 + (-61)) = (s8) (temp_t7_2 - 8);
        } while ((u32) var_v1_4 < (u32) (D_80120650 + 0x550));
        if ((u8) D_80120650->unk_07 < 9U) {
            D_80120650->unk_07 = 0;
            D_80120650->unk_03 = D_80120650->unk_03 & 0xFF7F;
            *(u8 *)((unsigned char *)D_80120650 + (75)) = 0U;
            *(u8 *)((unsigned char *)D_80120650 + (71)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (71)) & 0xFF7F);
            *(u8 *)((unsigned char *)D_80120650 + (143)) = 0U;
            *(u8 *)((unsigned char *)D_80120650 + (139)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (139)) & 0xFF7F);
            *(u8 *)((unsigned char *)D_80120650 + (211)) = 0U;
            *(u8 *)((unsigned char *)D_80120650 + (207)) = (u8) (*(u8 *)((unsigned char *)D_80120650 + (207)) & 0xFF7F);
            var_v1_5 = D_80120650 + 0x110;
            do {
                temp_t2 = *(u8 *)((unsigned char *)var_v1_5 + (71));
                temp_t3 = *(u8 *)((unsigned char *)var_v1_5 + (139));
                temp_t4 = *(u8 *)((unsigned char *)var_v1_5 + (207));
                temp_t8_2 = var_v1_5->unk_03;
                var_v1_5 += 0x110;
                *(s8 *)((unsigned char *)var_v1_5 + (-65)) = (s8) (temp_t4 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-133)) = (s8) (temp_t3 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-201)) = (s8) (temp_t2 & ~0x80);
                *(s8 *)((unsigned char *)var_v1_5 + (-197)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-129)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-61)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-265)) = 0;
                *(s8 *)((unsigned char *)var_v1_5 + (-269)) = (s8) (temp_t8_2 & ~0x80);
            } while (var_v1_5 != (D_80120650 + 0x550));
            D_800B5BD4 = 0xF;
            func_80041584();
            func_80044750(0xC1);
            func_8004284C();
        }
        break;
    case 11:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}
