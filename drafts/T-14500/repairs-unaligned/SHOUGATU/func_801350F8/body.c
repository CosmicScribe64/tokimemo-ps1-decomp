void func_801350F8(void) {
    u8 sp84[] = "おみくじを引きます。";
    u8 sp6C[] = "うおりゃあああ〜・・・";
    u8 sp5C[] = "こ、これは！！";
    UnalignedBytes35 sp38;
    s16 var_v1;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    u8 temp_t7_2;

    sp38 = D_80143E5C;
    switch (D_800E7384) {
    case 0:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp84, 0);
        D_80122CE4 = 1;
        D_8012067E = -4;
        D_800E7384 += 1;
        return;
    case 2:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_80143DDC = 0;
        D_8012071F |= 0x80;
        D_8012071E = 1;
        D_80120732 = 1;
        D_80120734 = 0;
        D_800E7384 += 1;
        return;
    case 3:
        D_80120674 += *(s32 *)((unsigned char *)D_8011ECD0 + (6572));
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0x6F) {
            D_80120732 = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) + 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= 0x15) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = 0x14;
            }
        } else if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0x70) {
            D_80120732 = 1;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) - 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0x14) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -0x14;
            }
        }
        var_v1 = *(s16 *)((unsigned char *)D_8011ECD0 + (6574));
        if ((var_v1 < -8) || (var_at = var_v1 < -0xC, ((var_v1 < 9) == 0))) {
            D_8012069C += 1;
            D_801206E0 += 1;
            var_v1 = D_8012067E;
            var_at = var_v1 < -0xC;
        }
        if ((var_at != 0) || (var_at_2 = var_v1 < -0x10, ((var_v1 < 0xD) == 0))) {
            D_8012069C += 2;
            D_801206E0 += 2;
            var_v1 = D_8012067E;
            var_at_2 = var_v1 < -0x10;
        }
        if ((var_at_2 != 0) || (var_v1 >= 0x11)) {
            D_8012069C += 4;
            D_801206E0 += 4;
        }
    case 1:
    case 7:
    case 9:
block_51:
        if (D_80122CE4 != 0) {
            if ((D_800E7208 & 0x400040) && (check_end_k() != 0) && (D_800E62BC == 2)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E7384 += 1;
                return;
            }
            if ((D_800E7208 & 0x200020) && (check_end_k() != 0)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E7384 += 1;
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
        if (((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0) && (-*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= *(s16 *)((unsigned char *)D_8011ECD0 + (6566)))) || ((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) <= 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= -*(s16 *)((unsigned char *)D_8011ECD0 + (6574))))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = 0;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
            D_800E7384 += 1;
            return;
        }
        *(s32 *)((unsigned char *)D_8011ECD0 + (6564)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6564)) + *(s32 *)((unsigned char *)D_8011ECD0 + (6572)));
        var_at_3 = *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < 0x70;
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0x6F) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = 0xA;
            var_at_3 = *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < 0x70;
        }
        if (var_at_3 == 0) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -0xA;
        }
        return;
    case 5:
        if (D_800E738D == 0) {
            *(s32 *)((unsigned char *)D_8011ECD0 + (6572)) = 0;
            D_80120680 = 0xFFFC0000;
            D_80120653 |= 0x10;
        }
        D_80120696 = 0;
        D_801206DA = 0;
        if (D_8012067A < -0xB4) {
            D_8012067A = -0xB4;
        }
        temp_t7_2 = (D_800E738D + 1) & 0xFF;
        if ((D_800E738D ^ 0x1E) == 0) {
            D_80120680 = 0;
            D_80120666 = (D_800E62BE == 0x61) + ((D_800E62BE == 0x62) * 4) + 1;
            D_80120652 = 5;
        }
        D_800E738D = temp_t7_2;
        if (temp_t7_2 == 0x4B) {
            D_80143DDC = 0;
            D_80120653 &= 0x7F;
            D_8012071F |= 0x80;
            D_8012071E = 9;
            D_80120732 = 2;
            D_80120734 = 0;
            func_80044750(0x500);
        }
        if ((u8) D_800E738D < 0x87U) {
            return;
        }
        func_801359FC();
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp5C, 0);
        D_800E7384 += 1;
        return;
    case 6:
        if (((D_80120652 != 1) | 8) != 0) {
            D_80122CE4 = 1;
            D_800E7384 += 1;
        }
        return;
    case 8:
        D_80120653 |= 0x80;
        D_80120652 = 0x40;
        D_80120668 = D_80143DD8 + 0xD;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
        D_8012067A = -0x78;
        D_80120680 = 0;
        if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
            func_800AE130(&sp6C, "%sだ・・・", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_8012071F |= 0x80;
            D_8012071E = 0x40;
            D_80120734 = 7;
        } else if (D_80143DD8 != 0) {
            func_800AE130(&sp6C, "おっ、%sだ。", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        } else {
            func_800AE130(&sp6C, "ラッキー！%sだ！！", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        }
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_800E7384 += 1;
        return;
    default:
        func_8004284C();
        goto block_51;
    }
}
