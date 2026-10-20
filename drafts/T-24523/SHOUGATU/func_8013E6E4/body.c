extern s32 D_80122D48;
void func_8013EE30(void);

void func_8013E6E4(void) {
    s16 i;
    s16 count;
    s32 status;
    ShoShorts7 sp50;
    ShoShorts7 sp40;


    sp50 = D_801460EC;
    sp40 = D_801460FC;
    count = 0;
    for (i = 0; i < 11; i++) {
        count += D_800E6280.unk_1BC[i].unk_0C.f.b1 >= 1;
    }
    i = 0;
    do {
        /* FAKE: inherited operand-order identity from the GYOZI template. T-24523 */
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 * 1 + i) % 10U + 1;
        func_800AE0B0("num%d girl%d\n", count, D_800E6280.unk_F5F);
        i++;
        if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 && D_800E6280.unk_F5F >= 9) {
            continue;
        }
        if (D_800E6280.unk_F5F == 5 && (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 8) {
            continue;
        }
        if (D_800E6280.unk_F5F == 6 && (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 9) {
            continue;
        }
        if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 && count >= 2) {
            continue;
        }
        break;
    } while (1);
    if (D_80146050 != 0) {
        D_800E6280.unk_F5F = 9;
    }
    D_80146054 = D_800E6280.unk_F5F;
    D_80145F2C = 0;
    /* FAKE: inherited redundant narrowing from the GYOZI template. T-24523 */
    status = (u8)func_80051A68(D_800E6280.unk_F5F);
    if (D_800E6280.unk_F5F == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    func_800847B8(D_800E6280.unk_F5F);
    if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 || (D_80146050 != 0 && D_800E6280.unk_F5F == 9)) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x11;
        D_80122D48 = 3;
        if (D_80146050 != 0) {
            func_80085B3C(8, D_800E6280.unk_F5F * 10);
        } else {
            func_80085B3C(4, 0);
        }
        func_80042908(5);
        return;
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
    for (i = 0; i < 7; i++) {
        if (D_800E6280.unk_114.unk_02 < sp50.v[i]) {
            break;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp40.v[i];
    func_80085B3C(4, 0);
    func_80084D3C();
    func_8004284C();
}
