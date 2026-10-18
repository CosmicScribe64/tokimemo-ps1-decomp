/* NON_MATCHING: T-24071, private source-only diagnostic; production remains ASM. */
void func_80144DF0(void) {
    GameState *g = &D_800E6280;
    s32 sp28;
    s32 temp_v1;
    u8 temp_v0;
    u8 temp_v0_2;

    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    temp_v1 = g->unk_1094 + 1;
    sp28 = temp_v1;
    k_disp_inc();
    temp_v1 = sp28;
    if (g->unk_F88 & 0x20) {
        if (g->unk_1094 != -1) {
            if (D_800E7D14[temp_v1] == 0) {
                temp_v0 = temp_v1 & 0xFF;
                g->unk_1118 = temp_v0;
                if (func_80054694((s32) temp_v0) == 1) {
                    func_80053CE0();
                    func_8004284C();
                } else {
                    func_80042940(0xF0);
                }
            } else if (temp_v1 == g->unk_1118) {
                func_80053CE0();
                func_8004284C();
            } else {
                temp_v0_2 = temp_v1 & 0xFF;
                if ((u8) D_800E8BEE != 0) {
                    D_800E7D36 = temp_v0_2;
                    g->unk_1118 = temp_v0_2;
                    func_800696DC(0, 0);
                    func_8005C7C0();
                    g->unk_1094 = D_800E7D36 - 1;
                } else {
                    g->unk_1118 = temp_v0_2;
                    func_80042940(0xC0);
                }
            }
        }
    } else if (g->unk_F88 & 0x40) {
        func_80042908(2);
    }
    k_disp_inc();
    func_8005C7FC();
}
