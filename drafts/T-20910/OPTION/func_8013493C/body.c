void func_8013493C(void) {
    Arrs4x8 sp34;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s16 temp_t6;
    s32 temp_t0_2;
    u8 temp_t0;
    u8 temp_t2;
    u8 temp_t2_2;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t6_2;
    u8 temp_t8;
    u8 temp_t8_2;

    if (D_800E6280.unk_110D == 0) {
        icon_disp_switch(0);
        ((DisplayActor44 *)D_8011ECD0)[107].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[107].unk_03 & ~0x80);
        var_v0 = &((DisplayActor44 *)D_8011ECD0)[1];
        do {
            temp_t8 = var_v0[107].unk_03;
            temp_t0 = var_v0[108].unk_03;
            temp_t2 = var_v0[109].unk_03;
            temp_t4 = var_v0[110].unk_03;
            var_v0 += 4;
            var_v0[106].unk_03 = (s8) (temp_t4 & ~0x80);
            var_v0[105].unk_03 = (s8) (temp_t2 & ~0x80);
            var_v0[104].unk_03 = (s8) (temp_t0 & ~0x80);
            var_v0[103].unk_03 = (s8) (temp_t8 & ~0x80);
        } while (var_v0 != (&((DisplayActor44 *)D_8011ECD0)[9]));
        temp_t6 = ((s16) ((DisplayActor44 *)D_8011ECD0)[106].unk_1A / 9) * 8;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_1A = temp_t6;
        if (temp_t6 < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        icon_disp_switch(0);
        ((DisplayActor44 *)D_8011ECD0)[107].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[107].unk_03 & 0xFF7F);
        var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[1];
        do {
            temp_t2_2 = var_v0_2[107].unk_03;
            temp_t4_2 = var_v0_2[108].unk_03;
            temp_t6_2 = var_v0_2[109].unk_03;
            temp_t8_2 = var_v0_2[110].unk_03;
            var_v0_2 += 4;
            var_v0_2[106].unk_03 = (s8) (temp_t8_2 & ~0x80);
            var_v0_2[105].unk_03 = (s8) (temp_t6_2 & ~0x80);
            var_v0_2[104].unk_03 = (s8) (temp_t4_2 & ~0x80);
            var_v0_2[103].unk_03 = (s8) (temp_t2_2 & ~0x80);
        } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[9]));
        temp_t0_2 = (s32) (((DisplayActor44 *)D_8011ECD0)[106].unk_1A * 9) / 8;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_1C = (s16) ((s32) (((DisplayActor44 *)D_8011ECD0)[106].unk_1C * 9) / 8);
        ((DisplayActor44 *)D_8011ECD0)[106].unk_1A = (s16) temp_t0_2;
        if ((s16) temp_t0_2 >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&sp34);
        menu_set(0, 8, sp34.a, sp34.b, sp34.c, sp34.d);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
}
