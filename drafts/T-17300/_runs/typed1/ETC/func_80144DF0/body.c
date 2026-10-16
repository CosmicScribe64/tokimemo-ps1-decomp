void func_80144DF0(void) {
    s32 sp28;
    s32 temp_v1;
    u8 temp_v0;
    u8 temp_v0_2;

    menu_check(1, *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (38)), *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)) + (42)));
    func_8004FC10(1);
    menu_bar_show(1);
    temp_v1 = *(s8 *)((unsigned char *)(&D_800E6280) + (4244)) + 1;
    sp28 = temp_v1;
    k_disp_inc();
    if (*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x20) {
        if (*(s8 *)((unsigned char *)(&D_800E6280) + (4244)) != -1) {
            if (*(&D_800E7D14 + temp_v1) == 0) {
                temp_v0 = temp_v1 & 0xFF;
                *(u8 *)((unsigned char *)(&D_800E6280) + (4376)) = temp_v0;
                if (func_80054694(temp_v0) == 1) {
                    func_80053CE0();
                    func_8004284C();
                } else {
                    func_80042940(0xF0);
                }
            } else if (temp_v1 == D_800E6280.unk0) {
                func_80053CE0(*(s32 *)((unsigned char *)(&D_800E6280) + (3976)));
                func_8004284C();
            } else {
                temp_v0_2 = temp_v1 & 0xFF;
                if (D_800E8BEE != 0) {
                    D_800E7D36 = temp_v0_2;
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4376)) = temp_v0_2;
                    func_800696DC(0, 0);
                    func_8005C7C0();
                    *(s8 *)((unsigned char *)(&D_800E6280) + (4244)) = (s8) (D_800E7D36 - 1);
                } else {
                    D_800E6280.unk0 = temp_v0_2;
                    func_80042940(0xC0);
                }
            }
        }
    } else if (*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x40) {
        func_80042908(2);
    }
    k_disp_inc();
    func_8005C7FC();
}
