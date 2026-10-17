void func_8005CA94(void) {
    s32 sp30;
    void *sp28;
    s32 temp_v1;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 *temp_a1;

    menu_check(1, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_26, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    menu_bar_show(1);
    temp_v1 = D_800E6280.unk_1094 + 1;
    sp30 = temp_v1;
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_8005C4CC(1);
        func_80042908(0);
        func_80042940(2);
        D_800E6280.unk_111B = 0;
    } else if ((D_800E6280.unk_F88 & 0x20) && (D_800E6280.unk_1094 != -1)) {
        temp_a1 = temp_v1 + &D_800E7D10;
        if (*(u8 *)((unsigned char *)temp_a1 + (4)) == 0) {
            temp_v0 = temp_v1 & 0xFF;
            D_800E6280.unk_1118 = temp_v0;
            sp28 = temp_a1;
            if (func_80054694((s32) temp_v0) == 1) {
                sp28 = temp_a1;
                func_80054704((s32) D_800E6280.unk_1118);
                *(u8 *)((unsigned char *)temp_a1 + (4)) = 0xFFU;
                func_80053CE0();
                func_8004284C();
            } else {
                func_80042940(0xF0);
            }
        } else if (temp_v1 == D_800E6280.unk0) {
            func_80053CE0();
            func_8004284C();
        } else {
            temp_v0_2 = temp_v1 & 0xFF;
            if ((u8) D_800E8BEE != 0) {
                D_800E7D36 = temp_v0_2;
                D_800E6280.unk_1118 = temp_v0_2;
                func_800696DC(0, 0);
                D_800E6280.unk_1094 = D_800E7D36 - 1;
            } else {
                D_800E6280.unk0 = temp_v0_2;
                func_80042940(0xC0);
            }
        }
    }
    k_disp_inc();
    func_8005C7FC();
}
