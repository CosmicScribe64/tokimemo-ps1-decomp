extern u16 D_800E6374;
extern s32 D_800E7208;
extern s8 D_800E7314;
extern u8 D_800E738A;

void func_8007866C(void) {
    u8 sp30;
    u32 temp_t3;

    menu_check(1, (s32) D_8011ECF6, (s32) D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    if (D_800E7208 & 0x40) {
        func_80042908(0);
        return;
    }
    if (D_800E7208 & 0x20) {
        switch (D_800E7314) {                       /* irregular */
        case 0:
            func_8004284C();
            return;
        case 1:
            func_80042940((D_800E738A + 2) & 0xFF);
            func_8004E58C();
            strcpy(&sp30, club_name_table[(u16) D_800E6374 >> 0xC]);
            strcat(&sp30, "部を退部した。");
            sndi(&sp30, 0, 0xFF);
            temp_t3 = (u16) D_800E6374 >> 0xC;
            D_800E6280.unk_66C[temp_t3].unk_03 |= 1;
            D_800E6280.unk_0F4.b[1] = (D_800E6280.unk_0F4.b[1] & 0xFF0F) | 0xB0;
            k_disp_start(1);
            break;
        }
    }
}
