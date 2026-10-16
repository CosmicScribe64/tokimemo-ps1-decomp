u8 func_80152060(void) {
    s32 sp30;
    s32 sp2C;
    s16 var_v0;
    s32 temp_lo;
    s32 var_a1;
    s32 var_at;
    u8 temp_v0;
    u8 *temp_v1;

    sp2C = D_8015EF0C;
    *(s32 *)((unsigned char *)(&sp2C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EF0C) + (4));
    if (D_800E6280.unk_1104.w == 0) {
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 1;
        func_80046290(sp2C, sp30, 0xF);
        func_80044750(0xBF);
        func_80044750(0x300);
        D_800E6280.unk_1104.w += 1;
        goto block_15;
    }
    if ((func_80046094() == 9) && ((u32) D_800E6280.unk_1104.w >= 0x3DU)) {
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 0;
        func_8004284C();
        func_80044750(0x202);
        return 0U;
    }
    if ((D_800E6280.unk_F88 & 0x400040) && ((u32) D_800E6280.unk_1104.w >= 0x3DU)) {
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 0;
        func_8004284C();
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 3;
        func_80044750(0xB4);
        func_80044750(0x202);
        return 0U;
    }
    var_a1 = 0;
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 3;
        var_v0 += 1;
        temp_v1 = D_8015EE20 + (temp_lo * 2);
        var_at = var_v0 < 0xA7;
        if ((u32) D_800E6280.unk_1104.w >= (u32) (*(s32 *)temp_v1)) {
            var_at = var_v0 < 0xA7;
            if ((u32) *(s16 *)((unsigned char *)temp_v1 + (2)) >= (u32) D_800E6280.unk_1104.w) {
                var_a1 = 1;
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = (s16) *(s16 *)((unsigned char *)temp_v1 + (4));
                var_at = var_v0 < 0xA7;
            }
        }
    } while (var_at != 0);
    if (var_a1 == 0) {
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 3;
    }
block_15:
    if ((u32) D_800E6280.unk_1104.w >= 0x67CU) {
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 0;
    }
    if ((u32) D_800E6280.unk0 < 0x18E3U) {
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 1;
    }
    temp_v0 = func_800460EC();
    if (temp_v0 & 4) {
        D_800E6280.unk_1104.w += 1;
    }
    return temp_v0;
}
