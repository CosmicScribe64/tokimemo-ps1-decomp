u8 func_8007505C(void) {
    s32 sp30;
    u8 temp_v1;
    u8 var_v1;

    func_80048F64(0x60);
    temp_v1 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        var_v1 = 0;
    } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
        sp30 = (s32) temp_v1;
        addr_init_club_sd(D_801230D0);
        var_v1 = temp_v1;
        if (func_8006F524(4) != 0xF) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 1;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0x84;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_80125CA8;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_80125CAC;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_80125CB0;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_05 = 2;
            if ((u8) D_800E6280.unk_F6E != 0) {
                ((DisplayActor44 *)D_8011ECD0)[97].unk_06 = 0x82;
            }
            ((DisplayActor44 *)D_8011ECD0)[97].unk_43 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = (s16) (var_v1 + 2);
            ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0x18;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x40;
        }
    } else {
        sp30 = (s32) temp_v1;
        addr_init_holiday_sd(D_801230D0);
        var_v1 = temp_v1;
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            var_v1 = 0;
            *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk0 * 4)) = 0;
        }
    }
    sp30 = (s32) var_v1;
    load_palette(D_80125CA4, 0x10, 1, 1, /* extra? */ 0);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_80125CA8;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_80125CAC;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_80125CB0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 2;
    if ((u8) D_800E6280.unk_F6E != 0) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 0x82;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = (s16) var_v1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x40;
    if (D_800E6280.unk_69C[0].unk_00 == 0x42) {
        return func_80042808();
    }
    return func_80042908(1);
}
