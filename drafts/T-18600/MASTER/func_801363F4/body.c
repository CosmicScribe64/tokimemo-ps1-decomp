void func_801363F4(void) {
    s32 i;
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    MasterSprite44 *sp34;                           /* compiler-managed */
    MasterPoint2 *sp30;
    MasterPoint2 *var_v1_2;
    MasterSprite44 *var_v1;
    s16 *var_a0;
    s32 temp_hi;
    u8 temp_t1_2;
    u8 temp_t6;
    u8 temp_t8;
    u8 temp_t9;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {
    case 0:
        func_80044750(0xC1);
        cd_read_sectors(0x6EU, 0x801B4400, 0xBBA5);
        tpage_buf_clear();
        D_800B5BD4 = 0xA;
        D_8013C588 = 0;
        func_8004284C();
        break;
    case 1:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
        k_disp_start(2);
        if ((u8) D_8013C578 >= 3U) {
            set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に勝利した興奮が", 0);
            set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の体に", 0);
            set_kanji_string(-0x56, 0x52, 0, (s32) "　新たな力を与えた！！）", 0);
        } else {
            set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に敗北したくやしさが", 0);
            set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の、眠れる力を", 0);
            set_kanji_string(-0x56, 0x52, 0, (s32) "　呼びおこした！！）", 0);
        }
        func_8004284C();
        break;
    case 2:
        temp_t9 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t9;
        if ((temp_t9 >= 0x10U) && (check_end_k() != 0)) {
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            func_8004284C();
        }
        break;
    case 3:
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80133724();
            load_palette(D_8013C4D0, 0x10, 1, 0, 0);
            load_palette(D_8013C4D0, 0x11, 1, 0, 1);
            func_80048F64(0x60);
            ((MasterSprite44 *)D_80120650)->unk_01 = 0xB;
            ((MasterSprite44 *)D_80120650)->unk_02 = 0;
            ((MasterSprite44 *)D_80120650)->unk_38 = 0x01000000;
            ((MasterSprite44 *)D_80120650)->unk_03 = 4;
            ((MasterSprite44 *)D_80120650)->unk_0C = (u8 *) D_8013C4B8;
            ((MasterSprite44 *)D_80120650)->unk_10 = (u8 *) D_8013C4C0;
            ((MasterSprite44 *)D_80120650)->unk_34 = (u8 *) D_8013C4B0;
            ((MasterSprite44 *)D_80120650)->unk_14 = D_8013C4C8;
            ((MasterSprite44 *)D_80120650)->unk_16 = 0;
            ((MasterSprite44 *)D_80120650)->unk_18 = 0;
            ((MasterSprite44 *)D_80120650)->unk_06 = 1;
            ((MasterSprite44 *)D_80120650)->unk_43 = 0;
            ((MasterSprite44 *)D_80120650)->unk_26 = -0xA0;
            ((MasterSprite44 *)D_80120650)->unk_28.h[1] = -0x78;
            ((MasterSprite44 *)D_80120650)->unk_07 = 0;
            ((MasterSprite44 *)D_80120650)->unk_05 = 0x80;
            ((MasterSprite44 *)D_80120650)->unk_04 = 0;
            *(M2cBlock44 *)(&((MasterSprite44 *)D_80120650)[1]) = *(M2cBlock44 *)(((MasterSprite44 *)D_80120650));
            ((MasterSprite44 *)D_80120650)[1].unk_02 = 0x40;
            *(s32 *)&((MasterSprite44 *)D_80120650)[1].unk_0C = (s32) D_8013C4BC;
            *(s32 *)&((MasterSprite44 *)D_80120650)[1].unk_10 = (s32) D_8013C4C4;
            *(s32 *)&((MasterSprite44 *)D_80120650)[1].unk_34 = (s32) D_8013C4B4;
            ((MasterSprite44 *)D_80120650)[1].unk_14 = (s16) D_8013C4CC;
            var_v1 = &((MasterSprite44 *)D_80120650)[2];
            ((MasterSprite44 *)D_80120650)[1].unk_26 = (s16) ((func_800AE0D0() % 320) - 0xA0);
            ((MasterSprite44 *)D_80120650)[1].unk_28.h[1] = 0x3C;
            do {
                *(MasterSprite44 *)var_v1 = ((MasterSprite44 *)D_80120650)[1];
                sp34 = var_v1;
                var_v1 += 1;
                *(s16 *)((unsigned char *)var_v1 + (-44)) = (s16) (func_800AE0D0() % 10);
            } while ((u32) var_v1 < (u32) (&((MasterSprite44 *)D_80120650)[20]));
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
        set_kanji_string(-0x80, 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        func_80044750(0x203);
        func_8004284C();
        break;
    case 6:
        func_80137BF4();
        for (i = 0; i < 20; i++) {
            ((MasterSprite44 *)D_80120650)[i].unk_03 |= 0x80;
            ((MasterSprite44 *)D_80120650)[i].unk_07 += 8;
        }
        ((MasterSprite44 *)D_80120650)->unk_02 = 5;
        if ((u8) ((MasterSprite44 *)D_80120650)->unk_07 >= 0x80U) {
            ((MasterSprite44 *)D_80120650)->unk_07 = 0x80;
            ((MasterSprite44 *)D_80120650)[1].unk_07 = 0x80U;
            func_8004284C();
        }
        break;
    case 7:
        func_80137BF4();
        temp_t1_2 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t1_2;
        if ((temp_t1_2 >= 0x10U) && (check_end_k() != 0)) {
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            ((MasterSprite44 *)D_80120650)->unk_16 = 2;
            ((MasterSprite44 *)D_80120650)->unk_18 = 0;
            ((MasterSprite44 *)D_80120650)->unk_08 = 0;
            ((MasterSprite44 *)D_80120650)->unk_02 = 5;
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
        temp_t8 = (D_8013C588 + 1) & 0xFF;
        D_8013C588 = temp_t8;
        if ((temp_t8 >= 0x10U) && (check_end_k() != 0)) {
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            ((MasterSprite44 *)D_8011ECD0)[2].unk_03 = (u8) (((MasterSprite44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            func_8004284C();
        }
        break;
    case 10:
        func_80137BF4();
        for (i = 0; i < 20; i++) {
            ((MasterSprite44 *)D_80120650)[i].unk_07 -= 8;
        }
        if ((u8) ((MasterSprite44 *)D_80120650)->unk_07 < 9U) {
            for (i = 0; i < 20; i++) {
                ((MasterSprite44 *)D_80120650)[i].unk_03 &= ~0x80;
                ((MasterSprite44 *)D_80120650)[i].unk_07 = 0;
            }
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
    Default_Disp();
}
