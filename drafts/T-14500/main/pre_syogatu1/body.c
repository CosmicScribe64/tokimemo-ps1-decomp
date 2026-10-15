s32 pre_syogatu1(void) {
    s32 var_s0;
    s32 var_s1;
    /* FAKE: two words recover the observed frame without moving the buffers. T-12030. */
    s32 stack_home[2];
    u8 sp16C[0xC];
    u8 sp6C[0x100];
    u8 sp5C[0x10];
    u8 *var_s2;

    switch (D_800E6280.unk_110D) {
    case 0:
        get_h_yuukou_table(sp16C);
        k_sub_reset();
        strcpy(sp6C, "やった！ ");
        var_s1 = 0;
        var_s0 = 0;
        var_s2 = sp16C;
        do {
            if (((u8) *var_s2 >= 0x4DU) && (D_800E6280.unk_1BC[var_s0].unk_0C.f.b1)) {
                if (var_s1 != 0) {
                    strcat(sp6C, "と、 ");
                }
                get_g_name(sp5C, var_s0);
                strcat(sp6C, sp5C);
                var_s1 += 1;
            }
            var_s0 += 1;
            var_s2 += 1;
        } while (var_s0 != 0xB);
        strcat(sp6C, "から、 年賀状をもらったラッキー。）");
        gnsx(D_800E6280.unk_0D4);
        sndisp(sp6C, 0, 0x1F);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        main_state_set(0x53);
        return;
    }
}
