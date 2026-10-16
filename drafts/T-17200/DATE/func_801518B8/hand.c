void func_801518B8(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        load_tpage_buf_lock(i + 5, ((s32 *)D_8015ED70)[i], 1);
    }
    func_80151460();
    load_palette(D_8015ED80, 0x11, 1, 2, 0);
    func_80084E90(D_8015ED84, D_8015ED88, D_8015ED8C, D_8015ED90, D_8015ED94, D_8015ED98);
    func_800850D4(D_8015ED74, D_8015ED78, D_8015ED70, D_8015ED7C);
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = 3;
    D_80122D10 = 2;
    D_80122D0C = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 3;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
    func_8004284C();
}
