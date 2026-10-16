void func_8013B78C(void) {
    s32 sp2C;
    s32 var_t7;
    u16 temp_v1;
    u16 var_v1;

    var_v1 = D_801499A0.unk_02;
    if (var_v1 >= 0x230U) {
        D_801499B2 = 0;
        func_80044750(0x802);
        var_v1 = 0;
    }
    D_801499A0.unk_02 = var_v1;
    func_8013C5C8(0x15000);
    func_8013C694();
    temp_v1 = D_801499A0.unk_02;
    var_t7 = temp_v1 & 0xC;
    if (!(temp_v1 & 4)) {
        func_8013C4E4(0xF000);
        var_t7 = D_801499A0.unk_02 & 0xC;
    }
    if (var_t7 == 0) {
        if ((temp_v1 >= 0x227U) && (temp_v1 < 0x258U)) {
            if ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6634)) + 0x1E) < *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6566))) {
                D_801499A0.unk_04 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6566)) - 0x1E;
            } else if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6566)) < (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6634)) - 0x1E)) {
                D_801499A0.unk_04 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6566)) + 0x1E;
            }
            D_801499A0.unk_06 = ((DisplayActor44 *)D_8011ECD0)->unk0;
        } else {
            sp2C = 0;
            *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) ((s16) D_80149968.unk_1C[0] >> 3);
            sp2C += D_80149968.unk_0C * 0x50;
            D_801499A0.unk_06 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
            D_801499A0.unk_04 = *(s16 *)((unsigned char *)(&sp2C) + (2));
        }
    }
    D_801499A0.unk_02 = temp_v1;
    if (temp_v1 != 0) {
        D_801499A0.unk_02 = temp_v1;
        if ((temp_v1 % 35U) == 0) {
            if (D_801499B2 == 0) {
                D_801499B2 = 1;
                func_80044750(0x800);
            } else {
                D_801499B2 = 0;
                func_80044750(0x801);
            }
            D_801499A0.unk_02 += 1;
        }
    }
}
