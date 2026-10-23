void func_8013EA98(void) {
    s16 i;
    s16 count;
    u32 status;
    ShoShorts7 sp50;
    ShoShorts7 sp40;

    sp50 = D_8014610C;
    sp40 = D_8014611C;
    count = 0;
    for (i = 0; i < 11;) {
        count += D_800E6280.unk_1BC[i++].unk_0C.f.b1 >= 1;
    }
    status = 0;
    /* FAKE (T-25501): the "* 0" terms in this loop are references that uopt
     * counts and ugen folds away. The b1 re-read makes the >= 9 test an alias
     * hole of the selector; the selector and counter terms set the colouring
     * order of the loop ranges (selector piece in t4, counter in v0). */
    do {
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 + status) % 10U + 1;
        status = (s16)(status + 1);
        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 || D_800E6280.unk_F5F < 9 + D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 * 0) {
            if (*(u8 *)&D_800E6280.unk_F5F != 5 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + D_800E6280.unk_F5F * 0 + status * 0) != 8) {
                if (D_800E6280.unk_F5F != 6 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + *(u8 *)&D_800E6280.unk_F5F * 0 + status * 0) != 9) {
                    if (D_80146054 != *(u8 *)&D_800E6280.unk_F5F) {
                        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1) {
                            break;
                        }
                        if (count < 3) {
                            break;
                        }
                    }
                }
            }
        }
    } while (1);
    /* FAKE (T-25501): the counter holds the forced choice so the compares
     * keep their operand order; the record read is an alias hole, and the
     * forced store and the next two reads go through the D_800E71DF alias
     * of unk_F5F, which gives the v1 piece of the original. */
    status = D_80145FE4;
    if ((status == 7 || status == 9) && D_80146054 != status + D_800E6280.unk_1BC[status * 0].unk_06 * 0) {
        D_800E71DF = status + *(u8 *)&D_800E6280.unk_F5F * 0;
    }
    D_80146058 = D_800E71DF;
    D_80145F2C = 0;
    /* FAKE: narrowing as in func_8013E6E4. T-25038 */
    status = (u8)func_80051A68(D_800E71DF);
    if (D_800E6280.unk_F5F == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    /* FAKE (T-25501): the byte view passes the selector itself, which pins
     * its range to a0 here and forces uopt to split the loop piece. */
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0);
        func_80042908(5);
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
        for (i = 0; i < 7; i++) {
            if (D_800E6280.unk_114.unk_02 < sp50.v[i]) {
                break;
            }
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp40.v[i];
        func_80085B3C(4, 0);
        func_8004284C();
    }
    check_para_limit();
}
