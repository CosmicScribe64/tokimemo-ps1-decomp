void func_80140040(void) {
    DisplayActor44 *temp_v0;
    DisplayActor44 *temp_v0_2;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_lo;
    s32 var_v0;

    var_v1 = 0;
    do {
        if ((u32) D_801470AC >= (u32) (var_v1 * 0x1E)) {
            temp_v0 = &((DisplayActor44 *)D_8011ECD0)[var_v1];
            temp_v0[99].unk_2E = -1;
            *(s16 *)((u8 *)&temp_v0[99] + 50) = 1;
        }
        temp_v0_2 = &((DisplayActor44 *)D_8011ECD0)[var_v1];
        *(s32 *)((u8 *)&temp_v0_2[99] + 40) = (s32) (*(s32 *)((u8 *)&temp_v0_2[99] + 40) + *(s32 *)((u8 *)&temp_v0_2[99] + 48));
        *(s32 *)((u8 *)&temp_v0_2[99] + 36) = (s32) (*(s32 *)((u8 *)&temp_v0_2[99] + 36) + *(s32 *)((u8 *)&temp_v0_2[99] + 44));
        if (temp_v0_2[99].unk_2A >= 0xA1) {
            temp_v0_2[99].unk_03 = (u8) (temp_v0_2[99].unk_03 & ~0x80);
        }
        var_v1 += 1;
    } while (var_v1 < 0xA);
    var_v0 = 0;
    var_v1_2 = 0;
    do {
        temp_lo = var_v1_2 * 0x44;
        var_v1_2 += 1;
        if (*(u8 *)((unsigned char *)((DisplayActor44 *)((u8 *)D_8011ECD0 + temp_lo)) + (6735)) & 0x80) {
            var_v0 = 1;
        }
    } while (var_v1_2 < 0xA);
    if (var_v0 == 0) {
        func_800AE0D0();
    }
    D_801470AC += 1;
}
