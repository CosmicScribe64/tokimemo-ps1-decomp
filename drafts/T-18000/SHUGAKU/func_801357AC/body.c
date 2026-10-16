void func_801357AC(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s16 var_v0;
    s32 temp_lo;

    func_80135430();
    load_palette(D_8013C2E0, 0x11, 1, 2, 0);
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 0x24;
        var_v0 += 1;
        *(s16 *)((unsigned char *)(D_801217D0 + temp_lo) + (2096)) = 0;
    } while (var_v0 < 6);
    func_800850D4(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    ((DisplayActor44 *)D_8011ECD0)[97].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xB;
    *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[98]) = *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[96]);
    ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x14;
    make_color_bar16(D_800E6280.unk_163C, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    load_palette((s32) D_800E6280.unk_163C, 0x1F, 1, 3, 0);
    D_800E6280.unk_1228[0x25] = 1;
    func_80046754(0x801C4000, 0x25, 0x80);
    func_8004284C();
}
