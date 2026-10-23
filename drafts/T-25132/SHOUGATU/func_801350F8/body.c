s32 func_801350F8(void) {
    /* FAKE: T-25132: the timer byte is read through a local pointer, so uopt keeps one register for it instead of
     * a promoted global plus a widened copy (as in DATE func_8013AF1C, T-21520). */
    GameState *g = &D_800E6280;
    u8 sp84[] = "おみくじを引きます。";
    u8 sp6C[] = "うおりゃあああ〜・・・";
    u8 sp5C[] = "こ、これは！！";
    ShoStr7x5 sp38;

    sp38 = D_80143E5C;
    switch (D_800E6280.unk_1104.w) {
    case 0:
        set_kanji_string(-0x68, 0x30, 0, sp84, 0);
        D_80122CE4 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -4;
        D_800E6280.unk_1104.w++;
        return 0;
    case 1:
        break;
    case 2:
        set_kanji_string(-0x68, 0x30, 0, sp6C, 0);
        D_80122CE4 = 1;
        D_80143DDC = 0;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_02 = 1;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = 1;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0;
        D_800E6280.unk_1104.w++;
        return 0;
    case 3:
        ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w += ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < -0x6F) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -((DisplayActor44 *)D_8011ECD0)[96].unk_2E;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = ((DisplayActor44 *)D_8011ECD0)[96].unk_2E + 2;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E > 0x14) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = 0x14;
            }
        } else if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0x70) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -((DisplayActor44 *)D_8011ECD0)[96].unk_2E;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = ((DisplayActor44 *)D_8011ECD0)[96].unk_2E - 2;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E < -0x14) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -0x14;
            }
        }
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E < -8 || ((DisplayActor44 *)D_8011ECD0)[96].unk_2E > 8) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_08 += 1;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_08 += 1;
        }
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E < -0xC || ((DisplayActor44 *)D_8011ECD0)[96].unk_2E > 0xC) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_08 += 2;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_08 += 2;
        }
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E < -0x10 || ((DisplayActor44 *)D_8011ECD0)[96].unk_2E > 0x10) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_08 += 4;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_08 += 4;
        }
        break;
    case 4:
        if ((((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0 &&
             -((DisplayActor44 *)D_8011ECD0)[96].unk_2E >= ((DisplayActor44 *)D_8011ECD0)[96].unk_26) ||
            (((DisplayActor44 *)D_8011ECD0)[96].unk_26 <= 0 &&
             ((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= -((DisplayActor44 *)D_8011ECD0)[96].unk_2E)) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0;
            D_80143DDC = 1;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_03 &= 0x7F;
            D_800E6280.unk_1104.w++;
        } else {
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w += ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < -0x6F) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = 0xA;
            }
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0x70) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -0xA;
            }
        }
        return 0;
    case 5:
        if (g->unk_110D == 0) {
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C = 0;
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0xFFFC0000;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 0x10;
        }
        ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 0;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < -0xB4) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0xB4;
        }
        if (g->unk_110D++ == 0x1E) {
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = (D_800E6280.unk_03E == 0x61) + (D_800E6280.unk_03E == 0x62) * 4 + 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
        }
        if (g->unk_110D == 0x4B) {
            D_80143DDC = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_03 &= 0x7F;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_03 |= 0x80;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_02 = 9;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = 2;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0;
            func_80044750(0x500);
        }
        if (g->unk_110D < 0x87) {
            return 0;
        }
        func_801359FC();
        set_kanji_string(-0x68, 0x30, 0, sp5C, 0);
        D_800E6280.unk_1104.w++;
        return 0;
    case 6:
        if ((((DisplayActor44 *)D_8011ECD0)[96].unk_02 != 1) | 8) {
            D_800E6280.unk_1104.w++;
            D_80122CE4 = 1;
        }
        return 0;
    case 7:
        break;
    case 8:
        /* FAKE: T-25132: an empty test of the selector makes ugen load it at the head of the case, so as1
         * schedules its `lui` before the first flag store, as the original does (1 swapped pair without it). */
        if (D_80143DD8 != 0) {
        }
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = D_80143DD8 + 0xD;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0;
        /* T-25132: the (s32) index reads the widened selector, the one register of the tests (a plain u8 index
         * gives uopt a second register for it). */
        if (D_80143DD8 == 3 || D_80143DD8 == 4) {
            sprintf(sp6C, "%sだ・・・", sp38.s[(s32)D_80143DD8]);
            ((DisplayActor44 *)D_8011ECD0)[99].unk_03 |= 0x80;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 7;
        } else if (D_80143DD8 != 0) {
            sprintf(sp6C, "おっ、%sだ。", sp38.s[(s32)D_80143DD8]);
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 6;
            D_80143DDC = 1;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_03 &= 0x7F;
        } else {
            sprintf(sp6C, "ラッキー！%sだ！！", sp38.s[(s32)D_80143DD8]);
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 6;
            D_80143DDC = 1;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_03 &= 0x7F;
        }
        set_kanji_string(-0x68, 0x30, 0, sp6C, 0);
        D_80122CE4 = 1;
        D_800E6280.unk_1104.w++;
        return 0;
    case 9:
        break;
    default:
        func_8004284C();
        break;
    }
    if (D_80122CE4 != 0) {
        if ((D_800E6280.unk_F88 & 0x400040) && check_end_k() && D_800E6280.unk_03C == 2) {
            k_reset(1);
            D_80122CE4 = 0;
            D_80122CF0 = 0;
            D_800CA2AC = 0;
            D_8011ED5B &= 0xFF7F;
            D_800E6280.unk_1104.w++;
            return 1;
        }
        if ((D_800E6280.unk_F88 & 0x200020) && check_end_k()) {
            k_reset(1);
            D_80122CE4 = 0;
            D_80122CF0 = 0;
            D_800CA2AC = 0;
            D_8011ED5B &= 0xFF7F;
            D_800E6280.unk_1104.w++;
            return 1;
        }
        if (check_end_k()) {
            D_8011ED5B |= 0x80;
            func_8006C934();
        } else {
            D_8011ED5B &= 0x7F;
        }
    }
}
