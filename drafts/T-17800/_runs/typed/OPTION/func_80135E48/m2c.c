void func_80135E48(void) {
    s16 sp64;
    s16 sp58;
    s16 sp4C;
    OptActor44 *temp_v0;
    s16 *temp_a0;
    s16 *temp_a1;
    s16 *temp_t0;
    s16 *temp_t1;
    s16 temp_a3;
    s16 temp_t8;
    s32 temp_v1;
    u32 temp_hi;

    if (D_800E6280.unk_110D == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 4;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x7FFF;
        return;
    }
    temp_a3 = 1 * 0x18;
    temp_t8 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 17) * 0x10;
    *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) (((s16) D_8011ECD0->unk0 / 17) * 0x10);
    *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t8;
    if (temp_t8 < 0x1001) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x1000;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x1000;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7300)) = 0xC;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7314)) = 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7318)) = -0x18;
        sp58 = 0x30;
        *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) | 0x80);
        sp4C = 0x10;
        sp64 = -0x18;
        temp_v1 = 1 * 2;
        temp_a0 = &(&sp70[0])[1];
        sp70[0] = 0x10;
        temp_v0 = &(D_8011ECD0 + 0x1980)[1];
        temp_a1 = &(&sp64)[1];
        temp_a1->unk0 = temp_a3 - 0x18;
        temp_a0->unk0 = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (772)) = (s16) (temp_v1 + 0xC);
        *(s16 *)((unsigned char *)temp_a0 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (790)) = (s16) temp_a1->unk0;
        *(u8 *)((unsigned char *)temp_v0 + (751)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (751)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (786)) = (s16) temp_a0->unk0;
        *(s16 *)((unsigned char *)temp_a1 + (2)) = temp_a3;
        *(s16 *)((unsigned char *)temp_v0 + (840)) = (s16) (temp_v1 + 0xE);
        *(s16 *)((unsigned char *)temp_v0 + (854)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (2));
        *(s16 *)((unsigned char *)temp_a0 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_a1 + (4)) = (s16) (temp_a3 + 0x18);
        *(s16 *)((unsigned char *)temp_v0 + (858)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (908)) = (s16) (temp_v1 + 0x10);
        *(u8 *)((unsigned char *)temp_v0 + (819)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (819)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (922)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s16 *)((unsigned char *)temp_a1 + (6)) = (s16) (temp_a3 + 0x30);
        temp_t1 = &(&sp4C)[1];
        *(u8 *)((unsigned char *)temp_v0 + (887)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (887)) | 0x80);
        *(s16 *)((unsigned char *)temp_a0 + (6)) = 0x10;
        temp_t0 = &(&sp58)[1];
        *(s16 *)((unsigned char *)temp_v0 + (926)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (4));
        *(s16 *)((unsigned char *)temp_v0 + (976)) = (s16) (temp_v1 + 0x12);
        *(s16 *)((unsigned char *)temp_t1 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (6)) = 0x10;
        temp_t1->unk0 = 0x10;
        *(u8 *)((unsigned char *)temp_v0 + (955)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (955)) | 0x80);
        *(s16 *)((unsigned char *)temp_t0 + (2)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (4)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (6)) = 0x30;
        temp_t0->unk0 = 0x30;
        *(s16 *)((unsigned char *)temp_v0 + (990)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s16 *)((unsigned char *)temp_v0 + (994)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (6));
        func_8004F870(1, 5, &sp70[0], &sp64, &sp58, &sp4C);
        temp_hi = (u8) D_800E6280.unk_F6F % 10U;
        switch (temp_hi) {
        default:
            *(s16 *)((unsigned char *)D_8011ECD0 + (7300)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7300)) + 1);
            D_800E6280.unk_1094 = 0;
            break;
        case 2:
            *(s16 *)((unsigned char *)D_8011ECD0 + (7368)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7368)) + 1);
            D_800E6280.unk_1094 = 1;
            break;
        case 3:
            *(s16 *)((unsigned char *)D_8011ECD0 + (7436)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7436)) + 1);
            D_800E6280.unk_1094 = 1;
            break;
        case 4:
            *(s16 *)((unsigned char *)D_8011ECD0 + (7504)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7504)) + 1);
            D_800E6280.unk_1094 = 1;
            break;
        case 6:
            *(s16 *)((unsigned char *)D_8011ECD0 + (7572)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (7572)) + 1);
            D_800E6280.unk_1094 = 1;
            break;
        }
        func_8004284C();
    }
}
