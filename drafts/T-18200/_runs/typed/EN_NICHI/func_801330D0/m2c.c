void func_801330D0(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 var_at;
    s32 var_v1;

    temp_v0 = D_800E6280.unk_F80;
    var_v1 = 2;
    if (temp_v0 & 0x40) {
        var_v1 = 3;
    }
    if (D_800E6280.unk_F74 == 1) {
        temp_a0 = temp_v0 & ~0x60;
        switch (temp_a0) {                          /* irregular */
        case 0x1000:
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] - (var_v1 << 0x10));
            break;
        case 0x2000:
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] + (var_v1 << 0x10));
            break;
        case 0x4000:
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] + (var_v1 << 0x10));
            break;
        case 0x8000:
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] - (var_v1 << 0x10));
            break;
        case 0x3000:
            temp_v0_2 = var_v1 * 0xB505;
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] + temp_v0_2);
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] - temp_v0_2);
            break;
        case 0x6000:
            temp_v0_3 = var_v1 * 0xB505;
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] + temp_v0_3);
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] + temp_v0_3);
            break;
        case 0xC000:
            temp_v0_4 = var_v1 * 0xB505;
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] - temp_v0_4);
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] + temp_v0_4);
            break;
        case 0x9000:
            temp_v0_5 = var_v1 * 0xB505;
            D_8011ECD0->pad_24[0] = (s32) (D_8011ECD0->pad_24[0] - temp_v0_5);
            D_8011ECD0->pad_28[0] = (s32) (D_8011ECD0->pad_28[0] - temp_v0_5);
            break;
        }
    }
    if (D_8011ECD0->unk_26 < -0x8F) {
        D_8011ECD0->unk_26 = -0x90;
    }
    if (D_8011ECD0->unk_26 >= 0x90) {
        D_8011ECD0->unk_26 = 0x90;
    }
    var_at = D_8011ECD0->unk_2A < 0x68;
    if (D_8011ECD0->unk_2A < -0x67) {
        D_8011ECD0->unk_2A = -0x68;
        var_at = D_8011ECD0->unk_2A < 0x68;
    }
    if (var_at == 0) {
        D_8011ECD0->unk_2A = 0x68;
    }
    *(s16 *)((unsigned char *)D_8011ECD0 + (10374)) = (s16) D_8011ECD0->unk_26;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10378)) = D_8011ECD0->unk_2A;
}
