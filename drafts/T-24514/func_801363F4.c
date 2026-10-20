void func_801363F4(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
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
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            func_8004284C();
        }
        break;
    case 3:
        func_8004284C();
        break;
    case 4: {
        MasterPoint2 *point;
        s16 *speed;
        if (cd_read_sync_status() & 1) {
            func_80133724();
            load_palette(D_8013C4D0, 0x10, 1, 0, 0);
            load_palette(D_8013C4D0, 0x11, 1, 0, 1);
            func_80048F64(0x60);
            ((DisplayFixActor44 *)D_80120650)->unk_01 = 0xB;
            ((DisplayFixActor44 *)D_80120650)->unk_02 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_38 = 0x01000000;
            ((DisplayFixActor44 *)D_80120650)->unk_03 = 4;
            ((DisplayFixActor44 *)D_80120650)->unk_0C = (u8 *) D_8013C4B8;
            ((DisplayFixActor44 *)D_80120650)->unk_10 = (u8 *) D_8013C4C0;
            ((DisplayFixActor44 *)D_80120650)->unk_34 = (u8 *) D_8013C4B0;
            ((DisplayFixActor44 *)D_80120650)->unk_14 = D_8013C4C8;
            ((DisplayFixActor44 *)D_80120650)->unk_16 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_06 = 1;
            ((DisplayFixActor44 *)D_80120650)->unk_43 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_24.h[1] = -0xA0;
            ((DisplayFixActor44 *)D_80120650)->unk_28.h[1] = -0x78;
            ((DisplayFixActor44 *)D_80120650)->unk_07 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_05 = 0x80;
            ((DisplayFixActor44 *)D_80120650)->unk_04 = 0;
            ((DisplayFixActor44 *)D_80120650)[1] = ((DisplayFixActor44 *)D_80120650)[0];
            ((DisplayFixActor44 *)D_80120650)[1].unk_02 = 0x40;
            ((DisplayFixActor44 *)D_80120650)[1].unk_0C = (u8 *)D_8013C4BC;
            ((DisplayFixActor44 *)D_80120650)[1].unk_10 = (u8 *)D_8013C4C4;
            ((DisplayFixActor44 *)D_80120650)[1].unk_34 = (u8 *)D_8013C4B4;
            ((DisplayFixActor44 *)D_80120650)[1].unk_14 = (s16) D_8013C4CC;
            ((DisplayFixActor44 *)D_80120650)[1].unk_24.h[1] = (func_800AE0D0() % 320) - 0xA0;
            ((DisplayFixActor44 *)D_80120650)[1].unk_28.h[1] = 0x3C;
            for (i = 2; i < 20; i++) {
                ((DisplayFixActor44 *)D_80120650)[i] = ((DisplayFixActor44 *)D_80120650)[1];
                ((DisplayFixActor44 *)D_80120650)[i].unk_18 = func_800AE0D0() % 10;
            }
            point = D_8013C58C;
            speed = D_8013C5B4;
            do {
                *speed = (func_800AE0D0() % 5) + 1;
                    point->y = 0x3C;
                point->x = (func_800AE0D0() % 320) - 0xA0;
                point++;
                speed++;
            } while (point != (MasterPoint2 *)D_8013C5B4);
            hizuke_disp_switch(0);
            func_80065F34(0);
            D_8013C588 = 0xF;
            func_8004284C();
        }
        break;
    }
    case 5:
        set_kanji_string(-0x80, 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        func_80044750(0x203);
        func_8004284C();
        break;
    case 6:
        func_80137BF4();
        ((DisplayFixActor44 *)D_80120650)[0].unk_03 |= 0x80;
        ((DisplayFixActor44 *)D_80120650)[0].unk_07 += 8;
        for (i = 1; i < 20; i++) {
            ((DisplayFixActor44 *)D_80120650)[i].unk_03 |= 0x80;
            ((DisplayFixActor44 *)D_80120650)[i].unk_07 += 8;
        }
        ((DisplayFixActor44 *)D_80120650)->unk_02 = 5;
        if ((u8) ((DisplayFixActor44 *)D_80120650)->unk_07 >= 0x80U) {
            ((DisplayFixActor44 *)D_80120650)->unk_07 = 0x80;
            ((DisplayFixActor44 *)D_80120650)[1].unk_07 = 0x80U;
            func_8004284C();
        }
        break;
    case 7:
        func_80137BF4();
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            ((DisplayFixActor44 *)D_80120650)->unk_16 = 2;
            ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_08 = 0;
            ((DisplayFixActor44 *)D_80120650)->unk_02 = 5;
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
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 ^ 0x80);
            D_8013C588 = 0;
        }
        if ((D_800E6280.unk_F88 & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            ((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayFixActor44 *)D_8011ECD0)[2].unk_03 & 0x7F);
            func_8004284C();
        }
        break;
    case 10:
        func_80137BF4();
        ((DisplayFixActor44 *)D_80120650)[0].unk_07 -= 8;
        for (i = 1; i < 20; i++) {
            ((DisplayFixActor44 *)D_80120650)[i].unk_07 -= 8;
        }
        if ((u8) ((DisplayFixActor44 *)D_80120650)->unk_07 < 9U) {
            ((DisplayFixActor44 *)D_80120650)[0].unk_07 = 0;
            ((DisplayFixActor44 *)D_80120650)[0].unk_03 &= ~0x80;
            for (i = 1; i < 20; i++) {
                ((DisplayFixActor44 *)D_80120650)[i].unk_07 = 0;
                ((DisplayFixActor44 *)D_80120650)[i].unk_03 &= ~0x80;
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
