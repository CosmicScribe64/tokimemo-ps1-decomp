/* NON_MATCHING: T-25501, 120 rows (funcdiff --resolve). Draft, not in src/.
 * The whole selection loop (0x8013EB0C-0x8013EC50) and the colouring order of
 * every loop range match the original. Left: the post-loop F5F load takes its
 * %hi through t6 where the original uses v1 (ugen temporary choice; every later
 * temporary is shifted by one register from there), and the FE4 == 9 compare
 * comes out as bne t0,v0 where the original has bne v0,t0. */
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
    do {
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 + status) % 10U + 1;
        status = (s16)(status + 1);
        /* FAKE: the "* 0" re-read of b1 makes the >= 9 block an alias hole of
         * the selector (uopt then leaves it out of the selector's live range,
         * ugen forwards the stored value as in the original). T-25501 */
        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 || D_800E6280.unk_F5F < 9 + D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 * 0) {
            /* FAKE: "+ x * 0" terms are references uopt counts and ugen folds:
             * two selector references put the selector's range between 6 and
             * 0x38 in the colouring order, two counter references put the
             * counter above b1 (v0/v1). T-25501 */
            if (*(u8 *)&D_800E6280.unk_F5F != 5 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + *(u8 *)&D_800E6280.unk_F5F * 0 + status * 0) != 8) {
                if (*(u8 *)&D_800E6280.unk_F5F != 6 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + *(u8 *)&D_800E6280.unk_F5F * 0 + status * 0) != 9) {
                    if ((*(u8 *)&D_80146054) != *(u8 *)&D_800E6280.unk_F5F) {
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
    /* FAKE: the "* 0" record read makes the == 9 block an alias hole and the
     * ">= 0" test (kept by uopt, folded by ugen) adds one flow-graph block;
     * together they bring the selector range to 9/6. T-25501 */
    if (((*(u8 *)&D_80145FE4) == 7 || (*(u8 *)&D_80145FE4) + D_800E6280.unk_1BC[(*(u8 *)&D_80145FE4) * 0].unk_06 * 0 == 9) && (*(u8 *)&D_80145FE4) >= 0 && (*(u8 *)&D_80146054) != (*(u8 *)&D_80145FE4)) {
        /* FAKE: store through the D_800E71DF alias (same byte as unk_F5F), so
         * uopt splits the selector range at L_EBE8 and not at this store. T-25501 */
        D_800E71DF = (*(u8 *)&D_80145FE4);
    }
    D_80146058 = D_800E6280.unk_F5F;
    D_80145F2C = 0;
    status = (u8)func_80051A68(*(u8 *)&D_800E6280.unk_F5F);
    if (D_800E6280.unk_F5F == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    func_800847B8(D_800E6280.unk_F5F);
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
