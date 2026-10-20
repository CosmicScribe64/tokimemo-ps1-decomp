/* T-24515 corrected source-only reconstruction; not a verified match.
 * FAKE: the implicit integer fallthrough preserves exit liveness; original
 * return type is unknown. T-24515.
 */
s32 func_80138C1C(void) {
    s32 n;
    /* FAKE: inherited unused scalar home fits the target frame. T-24515. */
    s32 pad;
    u8 *p;
    s32 sp0;
    s32 sp1;
    u8 buf[0x18];
    u8 c;

    for (n = 0; n < 8; n++) {
        D_800E6280.unk_0D4[n] = 0;
        D_800E6280.unk_0DC[n] = 0;
    }
    for (n = 0; n < 0x10; n++) {
        D_800E6280.unk_0C4[n] = 0;
        D_800E6280.unk_0E4[n] = 0;
    }
    /* FAKE: volatile source views fix the two read evaluation points before
     * the copy loops; no volatile local spill is requested. T-24515. */
    sp0 = *(volatile u8 *)&D_800B3644[0];
    sp1 = *(volatile u8 *)&D_800B3644[1];
    n = 0;
    for (p = D_8014CC74; p < &D_8014CC74[6]; p += 2) {
        c = p[0];
        if ((c != sp0 || p[1] != sp1) && (c != 0 || p[1] != 0)) {
            D_800E6280.unk_0D4[n * 2] = c;
            D_800E6280.unk_0D4[n * 2 + 1] = p[1];
            n++;
        }
    }
    n = 0;
    for (p = D_8014CC7C; p < &D_8014CC7C[6]; p += 2) {
        c = p[0];
        if ((c != sp0 || p[1] != sp1) && (c != 0 || p[1] != 0)) {
            D_800E6280.unk_0DC[n * 2] = c;
            D_800E6280.unk_0DC[n * 2 + 1] = p[1];
            n++;
        }
    }
    n = 0;
    for (p = D_8014CC84; p != &D_8014CC84[10]; p += 2) {
        c = p[0];
        if ((c != sp0 || p[1] != sp1) && (c != 0 || p[1] != 0)) {
            D_800E6280.unk_0E4[n * 2] = c;
            D_800E6280.unk_0E4[n * 2 + 1] = p[1];
            n++;
        }
    }
    func_800AE090(buf, D_800E6280.unk_0D4, 8);
    func_800AE110(buf, D_800E6280.unk_0DC, 8);
    func_800AE090(D_800E6280.unk_0C4, buf, 0x10);
    ((GsWord *)&D_800E6280.unk_0F8)->date.month = D_8014CC90 + 1;
    ((GsWord *)&D_800E6280.unk_0F8)->date.day = D_8014CC94 + 1;
    ((GsWord *)&D_800E6280.unk_0F8)->date.unk_09 = D_8014CC98 + 1;
    D_800E6280.unk_1BC[0].unk_10.date.month = D_8014CC9C + 1;
    D_800E6280.unk_1BC[0].unk_10.date.day = D_8014CCA0 + 1;
    D_800E6280.unk_1BC[0].unk_10.date.unk_09 = D_8014CCA4 + 1;
}
