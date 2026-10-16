u8 func_8007437C(void) {
    s32 sp30;
    u8 temp_t0;
    u8 temp_v1;
    u8 var_t0;

    func_80048F64(0x60);
    func_80048F64(0x61);
    temp_t0 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        var_t0 = 0;
    } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
        sp30 = (s32) temp_t0;
        addr_init_club_sd(D_801230D0);
        var_t0 = temp_t0;
    } else {
        sp30 = (s32) temp_t0;
        addr_init_weekly_sd(D_801230D0);
        var_t0 = temp_t0;
        if ((D_801230D0 == 0xF) || (D_801230D0 == 0x10)) {
            var_t0 = 0;
            *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk0 * 4)) = 0;
        }
    }
    sp30 = (s32) var_t0;
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
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = (s16) var_t0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x40;
    if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 7) {
        temp_v1 = D_800E6280.unk_044[0].unk_00[D_800E6280.unk_040];
        if ((temp_v1 != 1) && (temp_v1 != 2) && (temp_v1 != 3) && (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_03 != 0xF)) {
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
            ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = (s16) (var_t0 + 2);
            ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0x18;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x40;
        }
    }
    return func_80042808();
}
