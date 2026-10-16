void func_801342A8(void) {
    Arrs4x8 sp38;
    OptActor44 *var_v0;
    OptActor44 *var_v0_2;
    s16 temp_t8;
    s32 temp_t4;
    u8 temp_t2;
    u8 temp_t6;

    func_8006509C();
    icon_disp_switch(0);
    if (D_800E6280.unk_110D == 0) {
        icon_disp_switch(0);
        var_v0 = ((OptActor44 *)D_8011ECD0);
        do {
            temp_t6 = *(u8 *)((unsigned char *)var_v0 + (7279));
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (7211)) = (s8) (temp_t6 & ~0x80);
        } while (var_v0 != (&((OptActor44 *)D_8011ECD0)[3]));
        temp_t8 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 9) * 8;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t8;
        if (temp_t8 < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        icon_disp_switch(0);
        var_v0_2 = ((OptActor44 *)D_8011ECD0);
        do {
            temp_t2 = *(u8 *)((unsigned char *)var_v0_2 + (7279));
            var_v0_2 += 1;
            *(s8 *)((unsigned char *)var_v0_2 + (7211)) = (s8) (temp_t2 & ~0x80);
        } while (var_v0_2 != (&((OptActor44 *)D_8011ECD0)[3]));
        temp_t4 = (s32) (((OptActor44 *)D_8011ECD0)[106].unk_1A * 9) / 8;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = (s16) ((s32) (((OptActor44 *)D_8011ECD0)[106].unk_1C * 9) / 8);
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = (s16) temp_t4;
        if ((s16) temp_t4 >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&sp38);
        menu_set(0, 8, sp38.a, sp38.b, sp38.c, sp38.d);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
}
