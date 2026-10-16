? func_800AE130(? *, ? *, void *, ? *);             /* extern */

void func_801350F8(void) {
    ? sp84;
    ? sp6C;
    ? sp5C;
    ? sp38;
    ? *temp_t2;
    ? *temp_t7;
    ? *temp_t8;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    u8 temp_t7_2;

    temp_t7 = "おみくじを引きます。";
    sp84.unk0 = (s32) temp_t7->unk0;
    *(s32 *)((unsigned char *)(&sp84) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (8));
    *(s32 *)((unsigned char *)(&sp84) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (4));
    *(s32 *)((unsigned char *)(&sp84) + (16)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (16));
    temp_t2 = "うおりゃあああ〜・・・";
    *(u8 *)((unsigned char *)(&sp84) + (20)) = (u8) *(u8 *)((unsigned char *)temp_t7 + (20));
    *(s32 *)((unsigned char *)(&sp84) + (12)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (12));
    sp6C.unk0 = (s32) temp_t2->unk0;
    temp_t8 = "こ、これは！！";
    *(s32 *)((unsigned char *)(&sp6C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (4));
    *(s32 *)((unsigned char *)(&sp6C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (8));
    *(s32 *)((unsigned char *)(&sp6C) + (12)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (12));
    *(s32 *)((unsigned char *)(&sp6C) + (16)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (16));
    *(? *)((unsigned char *)(&sp6C) + (19)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)temp_t2 + (22)));
    sp5C.unk0 = (s32) temp_t8->unk0;
    *(s32 *)((unsigned char *)(&sp5C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t8 + (8));
    *(s32 *)((unsigned char *)(&sp5C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t8 + (4));
    *(? *)((unsigned char *)(&sp5C) + (11)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)temp_t8 + (14)));
    M2C_MEMCPY_ALIGNED(&sp38, &D_80143E5C, 0x20);
    *(? *)((unsigned char *)(&sp38 + 0x18) + (7)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)(&D_80143E5C.s[4][4]) + (10)));
    switch (D_800E6280.unk_1104.w) {
    case 0:
        func_8004E788(-0x68, 0x30, 0, (s32) &sp84, 0);
        D_80122CE4 = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -4;
        D_800E6280.unk_1104.w += 1;
        return;
    case 2:
        func_8004E788(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_80143DDC = 0;
        *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) | 0x80);
        *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 3:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6564)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6564)) + *(s32 *)((unsigned char *)D_8011ECD0 + (6572)));
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0x6F) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) + 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= 0x15) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = 0x14;
            }
        } else if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0x70) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = 1;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) - 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0x14) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -0x14;
            }
        }
        if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -8) || (var_at = *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0xC, ((*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < 9) == 0))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6604)) = (s16) (D_8011ECD0->unk0 + 1);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6672)) = (s16) (D_8011ECD0->unk0 + 1);
            var_at = *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0xC;
        }
        if ((var_at != 0) || (var_at_2 = *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0x10, ((*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < 0xD) == 0))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6604)) = (s16) (D_8011ECD0->unk0 + 2);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6672)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6672)) + 2);
            var_at_2 = *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0x10;
        }
        if ((var_at_2 != 0) || (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= 0x11)) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6604)) = (s16) (D_8011ECD0->unk0 + 4);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6672)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6672)) + 4);
        }
    case 1:
    case 7:
    case 9:
block_51:
        if (D_80122CE4 != 0) {
            if ((D_800E6280.unk_F88 & 0x400040) && (func_8004ECB4() != 0) && ((u8) D_800E6280.unk_03C == 2)) {
                func_8004E9F4(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0xFF7F);
                D_800E6280.unk_1104.w += 1;
                return;
            }
            if ((D_800E6280.unk_F88 & 0x200020) && (func_8004ECB4() != 0)) {
                func_8004E9F4(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0xFF7F);
                D_800E6280.unk_1104.w += 1;
                return;
            }
            if (func_8004ECB4() != 0) {
                *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) | 0x80);
                func_8006C934();
                return;
            }
            *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (D_8011ECD0->unk_00 & 0x7F);
        } else {
            return;
        }
        break;
    case 4:
        if (((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0) && (-*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= *(s16 *)((unsigned char *)D_8011ECD0 + (6566)))) || ((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) <= 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= -*(s16 *)((unsigned char *)D_8011ECD0 + (6574))))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = 0;
            D_80143DDC = 1;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0x7F);
            D_800E6280.unk_1104.w += 1;
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
        if (D_800E6280.unk_110D == 0) {
            *(s32 *)((unsigned char *)D_8011ECD0 + (6572)) = 0;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6576)) = 0xFFFC0000;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6531)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6531)) | 0x10);
        }
        *(s8 *)((unsigned char *)D_8011ECD0 + (6598)) = 0;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0;
        if (D_8011ECD0->unk0 < -0xB4) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0xB4;
        }
        temp_t7_2 = (D_800E6280.unk_110D + 1) & 0xFF;
        if ((D_800E6280.unk_110D ^ 0x1E) == 0) {
            *(s32 *)((unsigned char *)D_8011ECD0 + (6576)) = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = (s16) ((D_800E6280.unk_03E == 0x61) + ((D_800E6280.unk_03E == 0x62) * 4) + 1);
            *(u8 *)((unsigned char *)D_8011ECD0 + (6530)) = 5U;
        }
        D_800E6280.unk_110D = temp_t7_2;
        if (temp_t7_2 == 0x4B) {
            D_80143DDC = 0;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6531)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6531)) & 0x7F);
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) | 0x80);
            *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 9;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = 2;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0;
            func_80044750(0x500);
        }
        if ((u8) D_800E6280.unk_110D < 0x87U) {
            return;
        }
        func_801359FC();
        func_8004E788(-0x68, 0x30, 0, (s32) &sp5C, 0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 6:
        if (((*(u8 *)((unsigned char *)D_8011ECD0 + (6530)) != 1) | 8) != 0) {
            D_80122CE4 = 1;
            D_800E6280.unk_1104.w += 1;
        }
        return;
    case 8:
        *(u8 *)((unsigned char *)D_8011ECD0 + (6531)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6531)) | 0x80);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6530)) = 0x40U;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6552)) = (s16) (D_80143DD8 + 0xD);
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0x78;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6576)) = 0;
        if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
            func_800AE130(&sp6C, "%sだ・・・", &sp38 + (D_80143DD8 * 5), &sp6C);
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) | 0x80);
            *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 0x40;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 7;
        } else if (D_80143DD8 != 0) {
            func_800AE130(&sp6C, "おっ、%sだ。", &sp38 + (D_80143DD8 * 5), &sp6C);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 6;
            D_80143DDC = 1;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0x7F);
        } else {
            func_800AE130(&sp6C, "ラッキー！%sだ！！", &sp38 + (D_80143DD8 * 5), &sp6C);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 6;
            D_80143DDC = 1;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0x7F);
        }
        func_8004E788(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_800E6280.unk_1104.w += 1;
        return;
    default:
        func_8004284C();
        goto block_51;
    }
}
