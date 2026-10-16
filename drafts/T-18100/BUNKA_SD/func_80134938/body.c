void func_80134938(void) {
    DisplayActor44 *var_s0;
    s16 temp_t9;
    s32 temp_t0;
    s32 var_s1;
    s32 var_s1_2;

    var_s1 = 0;
    do {
        load_palette(D_8013C71C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x80;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6540)) = (u8 *) D_8013C714;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6544)) = (u8 *) D_8013C718;
    *(s32 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6580)) = (s32 *) D_8013C710;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    var_s0 = &((DisplayActor44 *)D_8011ECD0)[1];
    var_s1_2 = 1;
    do {
        func_80048F64(var_s1_2 + 0x60);
        var_s0[96].unk_01 = 4;
        var_s0[96].unk_38 = 0x01000000;
        var_s0[96].unk_03 = 0;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = (u8 *) D_8013C714;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = (u8 *) D_8013C718;
        temp_t9 = var_s1_2 - 1;
        var_s1_2 += 1;
        var_s0 += 1;
        var_s0[95].unk_16 = temp_t9;
        var_s0[95].unk_06 = 1;
        var_s0[95].unk_07 = 0;
        var_s0[95].unk_26 = -0xA0;
        var_s0[95].unk_2A = -0x78;
        var_s0[95].unk_05 = 0x80;
        var_s0[95].unk_43 = 0;
        var_s0[95].unk_04 = 0;
        *(s32 **)((unsigned char *)var_s0 + (6512)) = (s32 *) D_8013C710;
    } while (var_s1_2 != 5);
    temp_t0 = D_80122EAC * 2;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = (s16) (temp_t0 + 2);
    ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_16 = (s16) (temp_t0 + 3);
    search_load_tpage_buf_lock(D_8013C710[11], 1);
    search_load_tpage_buf_lock(D_8013C710[12], 1);
    search_load_tpage_buf_lock(D_8013C710[13], 1);
    search_load_tpage_buf_lock(D_8013C710[14], 1);
}
