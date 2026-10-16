void func_80136F3C(void) {
    DisplayActor44 *var_s0;
    s32 var_s1;
    s32 var_s1_2;

    var_s1 = 0;
    do {
        load_palette(D_8013CEE8, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        var_s0[96].unk_01 = 4;
        var_s0[96].unk_38 = 0x01000000;
        var_s0[96].unk_03 = 0;
        var_s0[96].unk_02 = 0x40;
        var_s0[96].unk_0C = (u8 *) D_8013CEE0;
        var_s0[96].unk_10 = (u8 *) D_8013CEE4;
        var_s0[96].unk_18 = 0;
        var_s0[96].unk_06 = 1;
        var_s0[96].unk_07 = 0;
        var_s0[96].unk_26 = -0xA0;
        var_s0[96].unk_2A = -0x78;
        var_s0[96].unk_05 = 0x80;
        var_s0[96].unk_43 = 0;
        var_s0[96].unk_04 = 0;
        var_s0[96].unk_34 = (u8 *)D_8013CEDC;
        var_s1_2 += 1;
        var_s0 += 1;
    } while (var_s1_2 != 0x40);
    func_801377D4(0, 0x40, 1);
    func_801377D4(0x1F, 0x44, 1);
    load_tpage_buf_lock(5, D_8013CEDC[0], 1);
    load_tpage_buf_lock(6, D_8013CEDC[1], 1);
    load_tpage_buf_lock(7, D_8013CEDC[2], 1);
    load_tpage_buf_lock(8, D_8013CEDC[3], 1);
    load_tpage_buf_lock(9, D_8013CEDC[4], 1);
    load_tpage_buf_lock(0xA, D_8013CEDC[5], 1);
    load_tpage_buf_lock(0x25, D_8013CEDC[6], 1);
    load_tpage_buf_lock(0x26, D_8013CEDC[7], 1);
    load_tpage_buf_lock(0x27, D_8013CEDC[8], 1);
    load_tpage_buf_lock(0x28, D_8013CEDC[9], 1);
    load_tpage_buf_lock(0x29, D_8013CEDC[10], 1);
    load_tpage_buf_lock(0x2A, D_8013CEDC[11], 1);
    load_tpage_buf_lock(0x15, D_8013CEDC[12], 1);
}
