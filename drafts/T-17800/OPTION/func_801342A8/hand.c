void func_801342A8(void) {
    s32 i;
    Arrs4x8 rows;

    func_8006509C();
    icon_disp_switch(0);
    if (D_800E6280.unk_110D == 0) {
        icon_disp_switch(0);
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        icon_disp_switch(0);
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&rows);
        func_8004F870(0, 8, rows.a, rows.b, rows.c, rows.d);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}
