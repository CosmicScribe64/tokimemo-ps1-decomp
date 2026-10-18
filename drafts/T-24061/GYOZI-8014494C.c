/* T-24061 source-only unmatched draft. See README.md for reproduction and negatives. */
/* FAKE: scratch s32 fall-through/setup-return and unsigned counter views recover the prefix; original interface unknown. No production/header changes. */
s32 func_8014494C(void) {
    func_80140050();
    if ((*(u32 *)&D_800F6474)++ == 0) {
        D_8012B8C0[2] = D_8012B8C0[0];
        D_8012B8C0[2] = D_8012B8C0[0];
        D_8012B8C0[2].unk_16 = 4;
    }
    if ((u16)D_800D9250 == 7 && (u16)D_800D9254 == 1 && func_8005B32C() == 0) {
        D_8012B8C0[2].unk_02 = 1;
        return;
    }
    D_8012B8C0[2].unk_02 = 0;
    D_8012B8C0[2].unk_18 = 0;
}
