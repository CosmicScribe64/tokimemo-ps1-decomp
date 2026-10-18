/* T-24029: typed source checkpoint, not matching; declarations are scratch-only. */
s32 func_801350F8(void) {
    s32 step = D_800E6280.unk_1104.w;
    u8 sp84[] = "おみくじを引きます。";
    u8 sp6C[] = "うおりゃあああ〜・・・";
    u8 sp5C[] = "こ、これは！！";
    ShoStr7x5 sp38;

    sp38 = D_80143E5C;
    switch (step) {
    case 0:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp84, 0);
        D_80122CE4 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -4;
        D_800E6280.unk_1104.w += 1;
        return;
    case 2:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_80143DDC = 0;
        D_8012071F |= 0x80;
        D_8012071E = 1;
        D_80120732 = 1;
        D_80120734 = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 3:
        ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w += ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < -0x6F) {
            D_80120732 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = (s16) -((DisplayActor44 *)D_8011ECD0)[96].unk_2E;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = (s16) (((DisplayActor44 *)D_8011ECD0)[96].unk_2E + 2);
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2E >= 0x15) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = 0x14;
            }
        } else if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0x70) {
            D_80120732 = 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = (s16) -((DisplayActor44 *)D_8011ECD0)[96].unk_2E;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = (s16) (((DisplayActor44 *)D_8011ECD0)[96].unk_2E - 2);
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
    case 1:
    case 7:
    case 9:
block_51:
        if (D_80122CE4 != 0) {
            if ((D_800E6280.unk_F88 & 0x400040) && (check_end_k() != 0) && (D_800E6280.unk_03C == 2)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E6280.unk_1104.w += 1;
                return;
            }
            if ((D_800E6280.unk_F88 & 0x200020) && (check_end_k() != 0)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E6280.unk_1104.w += 1;
                return;
            }
            if (check_end_k() != 0) {
                D_8011ED5B |= 0x80;
                func_8006C934();
                return;
            }
            D_8011ED5B &= 0x7F;
        } else {
            return;
        }
        break;
    case 4:
        if (((((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0) && (-((DisplayActor44 *)D_8011ECD0)[96].unk_2E >= ((DisplayActor44 *)D_8011ECD0)[96].unk_26)) || ((((DisplayActor44 *)D_8011ECD0)[96].unk_26 <= 0) && (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= -((DisplayActor44 *)D_8011ECD0)[96].unk_2E))) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
            D_800E6280.unk_1104.w += 1;
            return;
        }
        ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w = (s32) (((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w + ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C);
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < -0x6F) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = 0xA;
            }
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0x70) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2E = -0xA;
        }
        return;
    case 5:
        if (D_800E6280.unk_110D == 0) {
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_2C = 0;
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0xFFFC0000;
            D_80120653 |= 0x10;
        }
        D_80120696 = 0;
        D_801206DA = 0;
        if (D_8012067A < -0xB4) {
            D_8012067A = -0xB4;
        }
        if (D_800E6280.unk_110D++ == 0x1E) {
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0;
            D_80120666 = (D_800E6280.unk_03E == 0x61) + ((D_800E6280.unk_03E == 0x62) * 4) + 1;
            D_80120652 = 5;
        }
        if (D_800E6280.unk_110D == 0x4B) {
            D_80143DDC = 0;
            D_80120653 &= 0x7F;
            D_8012071F |= 0x80;
            D_8012071E = 9;
            D_80120732 = 2;
            D_80120734 = 0;
            func_80044750(0x500);
        }
        if ((u8) D_800E6280.unk_110D < 0x87U) {
            return;
        }
        func_801359FC();
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp5C, 0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 6:
        if (((D_80120652 != 1) | 8) != 0) {
            D_80122CE4 = 1;
            D_800E6280.unk_1104.w += 1;
        }
        return;
    case 8:
        D_80120653 |= 0x80;
        D_80120652 = 0x40;
        D_80120668 = D_80143DD8 + 0xD;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        D_8012067A = -0x78;
        ((DisplayFixActor44 *)D_8011ECD0)[96].unk_30 = 0;
        if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
            func_800AE130(sp6C, "%sだ・・・", sp38.s[D_80143DD8]);
            D_8012071F |= 0x80;
            D_8012071E = 0x40;
            D_80120734 = 7;
        } else if (D_80143DD8 != 0) {
            func_800AE130(sp6C, "おっ、%sだ。", sp38.s[D_80143DD8]);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        } else {
            func_800AE130(sp6C, "ラッキー！%sだ！！", sp38.s[D_80143DD8]);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        }
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_800E6280.unk_1104.w += 1;
        return;
    default:
        func_8004284C();
        goto block_51;
    }
    return;
}
