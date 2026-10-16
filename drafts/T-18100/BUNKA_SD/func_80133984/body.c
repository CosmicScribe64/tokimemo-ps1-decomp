void func_80133984(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C6F4, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 0x20; i++) {
        func_80048F64(i + 0x60);
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_0C = D_8013C6EC;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_10 = D_8013C6F0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_8013C6E8;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_43 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 = 0;
    }
    func_801341A0(0x1F, 0xBF, 1);
    func_801341A0(1, 0, 1);
    load_tpage_buf_lock(5, D_8013C6E8[0], 1);
    load_tpage_buf_lock(6, D_8013C6E8[1], 1);
    load_tpage_buf_lock(7, D_8013C6E8[2], 1);
    load_tpage_buf_lock(8, D_8013C6E8[3], 1);
    load_tpage_buf_lock(9, D_8013C6E8[4], 1);
    load_tpage_buf_lock(0xA, D_8013C6E8[5], 1);
    load_tpage_buf_lock(0xB, D_8013C6E8[6], 1);
    load_tpage_buf_lock(0xC, D_8013C6E8[7], 1);
    load_tpage_buf_lock(0xD, D_8013C6E8[8], 1);
    load_tpage_buf_lock(0xE, D_8013C6E8[9], 1);
    load_tpage_buf_lock(0xF, D_8013C6E8[10], 1);
    load_tpage_buf_lock(0x25, D_8013C6E8[11], 1);
    load_tpage_buf_lock(0x26, D_8013C6E8[12], 1);
    load_tpage_buf_lock(0x27, D_8013C6E8[13], 1);
    load_tpage_buf_lock(0x28, D_8013C6E8[14], 1);
    load_tpage_buf_lock(0x29, D_8013C6E8[15], 1);
    load_tpage_buf_lock(0x2A, D_8013C6E8[16], 1);
    load_tpage_buf_lock(0x2B, D_8013C6E8[17], 1);
    load_tpage_buf_lock(0x2C, D_8013C6E8[18], 1);
    load_tpage_buf_lock(0x2D, D_8013C6E8[19], 1);
    load_tpage_buf_lock(0x2E, D_8013C6E8[20], 1);
}
