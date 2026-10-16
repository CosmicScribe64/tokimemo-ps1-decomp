void func_8006BC28(s32 arg0) {
    s16 sp2E;
    s16 sp2C;

    sp2E = D_8011ECD0->unk_26;
    sp2C = D_8011ECD0->unk_2A;
    func_80048F64(0);
    D_8011ECD0->unk_01 = 1;
    D_8011ECD0->unk_02 = 0x40;
    D_8011ECD0->unk_38 = 0;
    D_8011ECD0->unk_03 = 0x84;
    D_8011ECD0->unk_0C = sprite_frame_table;
    D_8011ECD0->unk_10 = D_800C9A14;
    D_8011ECD0->unk_34 = D_800C9730;
    D_8011ECD0->unk_14 = D_800C9A54;
    D_8011ECD0->unk_04 = 8;
    D_8011ECD0->unk_05 = 1;
    D_8011ECD0->unk_16 = 0;
    if ((arg0 == 0) || (arg0 != 1)) {
        if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECD0->unk_18 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
            goto block_10;
        }
    } else if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECD0->unk_18 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
block_10:
        D_8011ECD0->unk_18 = 0;
    }
    D_8011ECD0->unk_26 = sp2E;
    D_8011ECD0->unk_2A = sp2C;
}
