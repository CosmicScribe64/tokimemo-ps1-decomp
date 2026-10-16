void func_80134938(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C71C, (i + 0x10) & 0xFF, 1, 1, i);
    }
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = D_8013C714;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = D_8013C718;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *)D_8013C710;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    for (i = 1; i < 5; i++) {
        func_80048F64(i + 0x60);
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_0C = D_8013C714;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_10 = D_8013C718;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_8013C710;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_16 = i - 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_43 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 = 0;
    }
    ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = D_80122EAC * 2 + 2;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_16 = D_80122EAC * 2 + 3;
    search_load_tpage_buf_lock(D_8013C710[11], 1);
    search_load_tpage_buf_lock(D_8013C710[12], 1);
    search_load_tpage_buf_lock(D_8013C710[13], 1);
    search_load_tpage_buf_lock(D_8013C710[14], 1);
}
