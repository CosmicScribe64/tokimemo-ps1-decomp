s32 func_80132450(void) {
    s32 i;

    load_tpage_buf_lock(5, D_80139204[0], 1);
    load_tpage_buf_lock(0x25, D_80139204[1], 1);
    load_tpage_buf_lock(6, D_80139204[2], 1);
    load_tpage_buf_lock(7, 0x801BA000, 1);
    load_tpage_buf_lock(8, 0x801BA000, 1);
    load_tpage_buf_lock(9, 0x801BA000, 1);
    load_tpage_buf_lock(0x27, 0x801BA000, 1);
    load_tpage_buf_lock(0x28, 0x801BA000, 1);
    load_tpage_buf_lock(0x29, 0x801BA000, 1);
    for (i = 0; i < 64; i++) {
        func_80048F64(i + 96);
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_01 = 7;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_03 = 4;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_05 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_43 = 0x11;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_04 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_0C = (u8 *)D_80139208;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_10 = (u8 *)D_8013920C;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_34 = (u8 *)D_80139204;
    }
    ((DisplayActor44 *)D_8011ECD0)[154].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[154].unk_18 = 0x22;
    ((DisplayActor44 *)D_8011ECD0)[153].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[153].unk_18 = 0x20;
    ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 3;
    ((DisplayActor44 *)D_8011ECD0)[152].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[152].unk_18 = 0x1F;
    ((DisplayActor44 *)D_8011ECD0)[152].unk_26 = ((DisplayActor44 *)D_8011ECD0)[0].unk_26;
    ((DisplayActor44 *)D_8011ECD0)[152].unk_2A = ((DisplayActor44 *)D_8011ECD0)[0].unk_2A;
    for (i = 96; i < 116; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i].unk_01 = 3;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_18 = 0x2E;
    }
    ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[155].unk_18 = 0x2D;
    ((DisplayActor44 *)D_8011ECD0)[155].unk_38 = 0x51000000;
    ((DisplayActor44 *)D_8011ECD0)[155].unk_43 = 0x1E;
    ((DisplayActor44 *)D_8011ECD0)[144].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[144].unk_26 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[144].unk_2A = -8;
    ((DisplayActor44 *)D_8011ECD0)[145].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[145].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[145].unk_2A = -8;
    ((DisplayActor44 *)D_8011ECD0)[146].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[146].unk_26 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[146].unk_2A = 0x18;
    ((DisplayActor44 *)D_8011ECD0)[147].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[147].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[147].unk_2A = 0x18;
}
