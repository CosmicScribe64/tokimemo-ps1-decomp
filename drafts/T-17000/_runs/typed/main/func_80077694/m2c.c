void func_80077694(void) {
    s32 sp30;
    u8 var_v1;
    u8 var_v1_2;

    func_80048F64(0x60);
    var_v1 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1)) {
        addr_init_else_sd(2);
        var_v1_2 = 0;
    } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
        sp30 = (s32) var_v1;
        addr_init_club_sd(D_801230D0);
        var_v1_2 = var_v1;
        if (func_8006F524(4) != 0xF) {
            *(s8 *)((unsigned char *)D_8011ECD0 + (6597)) = 1;
            *(s8 *)((unsigned char *)D_8011ECD0 + (6598)) = 5;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6652)) = 0x01000000;
            *(s8 *)((unsigned char *)D_8011ECD0 + (6599)) = 0x84;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_80125CA8;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_80125CAC;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_80125CB0;
            *(s8 *)((unsigned char *)D_8011ECD0 + (6601)) = 2;
            if ((u8) D_800E6280.unk_F6E != 0) {
                *(s8 *)((unsigned char *)D_8011ECD0 + (6602)) = 0x82;
            }
            *(s8 *)((unsigned char *)D_8011ECD0 + (6663)) = 0x10;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6618)) = (s16) (var_v1_2 + 2);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6634)) = -0x18;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = -0x40;
        }
    } else {
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = 0;
            var_v1 = 0;
        }
        sp30 = (s32) var_v1;
        addr_init_holiday_sd(D_801230D0);
        var_v1_2 = var_v1;
    }
    sp30 = (s32) var_v1_2;
    load_palette(D_80125CA4, 0x10, 1, 1, /* extra? */ 0);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6530)) = 5;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6584)) = 0x01000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6531)) = 0x84;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6540)) = (s32) D_80125CA8;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6544)) = (s32) D_80125CAC;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6580)) = (s32) D_80125CB0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6533)) = 2;
    if ((u8) D_800E6280.unk_F6E != 0) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (6534)) = 0x82;
    }
    *(s8 *)((unsigned char *)D_8011ECD0 + (6595)) = 0x10;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = (s16) var_v1_2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0x50;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0x40;
    func_8004284C();
}
