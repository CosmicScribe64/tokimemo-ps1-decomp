u8 func_8013AF78(s32 arg0) {
    s32 sp24;
    s32 sp20;
    s32 sp28[1];
    s32 sp1C;
    s32 *var_v1;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v1_2;

    var_v1 = &sp1C;
    do {
        var_v1 += 1;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
    } while ((u32) var_v1 < (u32) &sp28);
    temp_t8 = ((DisplayActor44 *)D_8011ECD0)[117].unk_03 & 0x7F;
    temp_t6 = ((DisplayActor44 *)D_8011ECD0)[115].unk_03 & 0x7F;
    temp_t7 = ((DisplayActor44 *)D_8011ECD0)[116].unk_03 & 0x7F;
    temp_t9 = ((DisplayActor44 *)D_8011ECD0)[118].unk_03 & 0x7F;
    var_v0 = temp_t9;
    var_v1_2 = temp_t7;
    if (arg0 == 0) {
        ((DisplayActor44 *)D_8011ECD0)[117].unk_03 = temp_t8;
        ((DisplayActor44 *)D_8011ECD0)[115].unk_03 = temp_t6;
        ((DisplayActor44 *)D_8011ECD0)[116].unk_03 = temp_t7;
        ((DisplayActor44 *)D_8011ECD0)[118].unk_03 = temp_t9;
        return 0U;
    }
    *(s32 *)((unsigned char *)(sp + (((s32) D_8014D0D4 % 3) * 4)) + (28)) = 1;
    ((DisplayActor44 *)D_8011ECD0)[117].unk_03 = temp_t8;
    if (sp1C != 0) {
        var_v1_2 = (var_v1_2 | 0x80) & 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[117].unk_03 = (u8) (temp_t8 | 0x80);
    }
    ((DisplayActor44 *)D_8011ECD0)[115].unk_03 = temp_t6;
    if (sp20 != 0) {
        ((DisplayActor44 *)D_8011ECD0)[115].unk_03 = (u8) (temp_t6 | 0x80);
    }
    ((DisplayActor44 *)D_8011ECD0)[118].unk_03 = var_v0;
    ((DisplayActor44 *)D_8011ECD0)[116].unk_03 = var_v1_2;
    if (sp24 != 0) {
        var_v0 = (var_v0 | 0x80) & 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[118].unk_03 = var_v0;
        ((DisplayActor44 *)D_8011ECD0)[116].unk_03 = (u8) (var_v1_2 | 0x80);
    }
    return var_v0;
}
