s32 func_80136434(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    D_801208FB = 0;
}
