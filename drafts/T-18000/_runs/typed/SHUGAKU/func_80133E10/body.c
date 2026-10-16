void func_80133E10(void) {
    ShFnTbl44 sp2C;
    s32 temp_v1;
    s32 var_t1;
    u8 temp_a0;
    u8 temp_v0;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110A;
    sp2C = D_8013C008;
    if (var_v0 < 8U) {
        func_8006B900();
        var_v0 = D_800E6280.unk_110A;
        goto block_5;
    }
    if (var_v0 >= 0x1DU) {
        var_t1 = var_v0 * 4;
        if (var_v0 < 0x23U) {
            func_8006B900();
            var_v0 = D_800E6280.unk_110A;
            goto block_5;
        }
    } else {
block_5:
        var_t1 = var_v0 * 4;
    }
    (*(s32 (**)(s32))((unsigned char *)(sp + var_t1) + (44)))(0x80);
    temp_v0 = D_800E6280.unk_110A;
    temp_v1 = sp2C.f[temp_v0];
    if (&D_80085368 == temp_v1) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    if ((temp_v0 >= 0xDU) && (temp_v0 < 0x16U)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    temp_a0 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07 & 0xFF;
    if (func_800853FC == temp_v1) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = temp_a0;
        if (temp_a0 < 8U) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 & 0xFF7F);
        }
    }
}
