void func_8007866C(void) {
    u8 buf[72];
    GameState *gs;

    menu_check(1, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_26, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    menu_bar_show(1);
    func_8004FC10(1);
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_8004284C();
            return;
        case 1:
            func_80042940((D_800E6280.unk_110A + 2) & 0xFF);
            func_8004E58C();
            strcpy(buf, club_name_table[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            strcat(buf, "部を退部した。");
            sndi(buf, 0, 0xFF);
            gs = &D_800E6280;
            gs->unk_66C[D_800E6280.unk_0F4.h >> 12].unk_03 |= 1;
            gs->unk_0F4.b[1] = (gs->unk_0F4.b[1] & 0xFF0F) | 0xB0;
            k_disp_start(1);
            break;
        }
    }
}
