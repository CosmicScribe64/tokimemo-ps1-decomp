void func_80149B08(void) {
    s32 sp2C;
    s32 var_a0;
    s32 var_a1;

    var_a0 = 0;
    if (func_800460EC() & 4) {
        var_a0 = getCDlevel(0);
    }
    D_8014D1B4[D_800E6280.unk_10F8 & 3] = var_a0;
    D_8014D1B0 = var_a0;
    var_a1 = (s32) ((s32) (D_8014D1B4[3] + *D_8014D1B4 + D_8014D1B4[1] + D_8014D1B4[2]) / 4) / 10;
    if (var_a1 >= 4) {
        var_a1 = 3;
    }
    sp2C = var_a1;
    func_80048F64(0x61);
    if (func_800460EC() & 4) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 9;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_38 = 0x41000000;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0xA4;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = 0x80180258;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = 0x80180298;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = 0x8018024C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = (s16) (var_a1 + 6);
        ((DisplayActor44 *)D_8011ECD0)[97].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = (s16) ((DisplayActor44 *)D_8011ECD0)[96].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_01 = 9;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_38 = 0x41000000;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = 0xA4;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_0C = 0x80180258;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_10 = 0x80180298;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_34 = 0x8018024C;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = (s16) (var_a1 + 0xA);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_26 = (s16) ((DisplayActor44 *)D_8011ECD0)[96].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_02 = 0;
    }
}
