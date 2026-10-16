void func_80135CD4(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C73C, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 6; i++) {
        func_80048F64(i + 0x60);
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_0C = D_8013C734;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_10 = D_8013C738;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_8013C730;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_16 = i + 3;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_43 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 = 0;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = D_80122EAC;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x8B;
    load_tpage_buf_lock(5, D_8013C730[0], 1);
    load_tpage_buf_lock(6, D_8013C730[1], 1);
    load_tpage_buf_lock(7, D_8013C730[2], 1);
    load_tpage_buf_lock(8, D_8013C730[3], 1);
    load_tpage_buf_lock(9, D_8013C730[4], 1);
    load_tpage_buf_lock(0xA, D_8013C730[5], 1);
    load_tpage_buf_lock(0x25, D_8013C730[6], 1);
    load_tpage_buf_lock(0x26, D_8013C730[7], 1);
    load_tpage_buf_lock(0x27, D_8013C730[8], 1);
    load_tpage_buf_lock(0x28, D_8013C730[9], 1);
    load_tpage_buf_lock(0x29, D_8013C730[10], 1);
}
