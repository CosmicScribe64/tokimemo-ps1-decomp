void func_8014472C(void) {
    s16 var_v1;

    if (func_800460EC() & 4) {
        var_v1 = getCDlevel();
    } else {
        var_v1 = 0;
        if (((u32) D_800E6280.unk_F80 >> 0x10) != 0) {
            var_v1 = D_800E6280.unk_1100 & 0x3F;
        }
    }
    D_80125C80[D_800E6280.unk_10F8 & 3] = var_v1;
    if (func_800460EC() & 4) {
        if (!(((DisplayActor44 *)D_8011ECD0)[98].unk_02 & 1) || !(((DisplayActor44 *)D_8011ECD0)[98].unk_03 & 0x80)) {
            func_80048F64(0x62);
            ((DisplayActor44 *)D_8011ECD0)[98].unk_01 = 3;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0x11U;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = 0xACU;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_0C = 0x801B27B0;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_10 = 0x801B27D8;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[98].unk_34 = 0x801B27AC;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_16 = 1;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_26 = -0xA0;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_2A = -0x78;
            load_tpage_buf_lock(5, *(s32 *)0x801B27AC, 1);
        }
    } else {
        func_80048F64(0x62);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = 0U;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0U;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_08 = 0;
    }
}
