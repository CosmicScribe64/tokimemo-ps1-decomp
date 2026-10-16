void pre_syogatu1(void) {
    u8 sp16C;
    u8 sp6C;
    u8 sp5C;
    s32 var_s0;
    s32 var_s1;
    u8 *var_s2;

    switch (D_800E6280.unk_110D) {                  /* irregular */
    case 0:
        get_h_yuukou_table((s32) &sp16C);
        k_sub_reset();
        strcpy(&sp6C, (u8 *) (D_800B0A04 + 0x1A0));
        var_s1 = 0;
        var_s0 = 0;
        var_s2 = &sp16C;
        do {
            if (((u8) *var_s2 >= 0x4DU) && (D_800E6280.unk_1BC[var_s0].unk_0C.w & 2)) {
                if (var_s1 != 0) {
                    strcat(&sp6C, D_800B0A04 + 0x1AC);
                }
                get_g_name(&sp5C, var_s0);
                strcat(&sp6C, &sp5C);
                var_s1 += 1;
            }
            var_s0 += 1;
            var_s2 += 1;
        } while (var_s0 != 0xB);
        strcat(&sp6C, D_800B0A04 + 0x1B4);
        gnsx(D_800E6280.unk_0D4);
        sndisp((s32) &sp6C, 0, 0x1F);
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
