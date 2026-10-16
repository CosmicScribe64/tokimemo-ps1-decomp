void func_8006612C(u8 *arg0) {
    DisplayActor44 *temp_v0;
    DisplayActor44 *var_s0;
    s16 var_v1;
    s32 temp_t6;
    s32 temp_v1;
    s32 var_s1;

    D_80125CA0 = -0x40;
    var_s1 = 0;
    if ((D_800B5B24 + 2) > 0) {
        do {
            func_80048F64(var_s1 + 3);
            var_s1 += 1;
        } while (var_s1 < (D_800B5B24 + 2));
        var_s1 = 0;
    }
    set_kanji_string(-0xCE, -0x98, 0U, arg0, 0);
    temp_v1 = strlen(arg0) / 2;
    var_v1 = (s16) temp_v1;
    if (((s16) temp_v1 + 2) > 0) {
        var_s0 = ((DisplayActor44 *)D_8011ECD0);
        D_800B5B24 = (s16) temp_v1;
        do {
            func_80048F64(var_s1 + 3);
            var_v1 = D_800B5B24;
            *(s8 *)((unsigned char *)var_s0 + (205)) = 2;
            *(s8 *)((unsigned char *)var_s0 + (206)) = 0x40;
            *(s32 *)((unsigned char *)var_s0 + (260)) = 0x40000000;
            *(s8 *)((unsigned char *)var_s0 + (207)) = 0xA4;
            *(u8 **)((unsigned char *)var_s0 + (216)) = sprite_frame_table;
            *(u8 **)((unsigned char *)var_s0 + (220)) = D_800C9A14;
            *(u8 **)((unsigned char *)var_s0 + (256)) = D_800C9730;
            temp_t6 = (var_v1 - var_s1) * 0xE;
            var_s1 += 1;
            var_s0 += 1;
            *(s8 *)((unsigned char *)var_s0 + (140)) = 8;
            *(s8 *)((unsigned char *)var_s0 + (141)) = 1;
            *(s16 *)((unsigned char *)var_s0 + (160)) = 0x72;
            *(s16 *)((unsigned char *)var_s0 + (174)) = (s16) ((temp_t6 + D_80125CA0) - 0x87);
            *(s16 *)((unsigned char *)var_s0 + (178)) = (s16) (D_80125CA0 - 0x51);
            *(s16 *)((unsigned char *)var_s0 + (156)) = (s16) D_800C9A54;
        } while (var_s1 < (var_v1 + 2));
    }
    temp_v0 = &((DisplayActor44 *)D_8011ECD0)[var_v1];
    *(s16 *)((unsigned char *)temp_v0 + (296)) = 0x71;
    *(s16 *)((unsigned char *)temp_v0 + (310)) = (s16) (*(s16 *)((unsigned char *)temp_v0 + (310)) + 2);
    ((DisplayActor44 *)D_8011ECD0)[3].unk_18 = 0x73;
    ((DisplayActor44 *)D_8011ECD0)[3].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[3].unk_26 - 2);
    D_800B5B24 = var_v1;
}
