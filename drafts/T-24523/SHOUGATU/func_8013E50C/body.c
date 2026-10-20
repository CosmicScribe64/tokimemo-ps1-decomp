void func_8013E50C(void) {
    ShoShorts7 sp40;
    ShoShorts7 sp30;
    s16 i;

    sp40 = D_801460CC;
    sp30 = D_801460DC;
    D_80146050 = 0;
    if (D_800E6280.unk_1BC[9].unk_0C.f.b1 == 0 &&
        D_800E6280.unk_1BC[9].unk_02 >= 20 && (D_800E6280.unk_10F8 & 1)) {
        D_80146050 = 1;
    }
    D_800E6280.unk_F5F = 0;
    D_80145F2C = ((func_80051A68(0) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    func_800847B8(D_800E6280.unk_F5F);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
    for (i = 0; i < 7; i++) {
        if (D_800E6280.unk_114.unk_02 < sp40.v[i]) {
            break;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp30.v[i];
    /* FAKE: base+stride address matches the original addu (no +0x1BC); T-24137. */
    func_80084D3C((s32)&D_800E6280 + D_800E6280.unk_F5F * 0x38, &sp40);
    func_80085B3C(4, 0);
    func_8004284C();
}