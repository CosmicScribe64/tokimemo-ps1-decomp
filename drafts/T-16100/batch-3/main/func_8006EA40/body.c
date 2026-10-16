s32 func_8006EA40(void) {
    s16 sp2C[16];
    u8 sp4C[16];
    Rec38 *var_v1;
    s32 var_a0;
    s32 var_v0;

    func_80053418((s32) &sp4C, (s32) &sp2C[0]);
    var_a0 = 0;
    var_v1 = D_800E6280.unk_1BC;
    var_v0 = 0;
    do {
        if ((var_v1->unk_0C.w & 2) && ((&sp2C[0])[var_v0] >= 0x1F5)) {
            var_a0 += 1;
        }
        var_v0 += 1;
        var_v1 += 1;
    } while (var_v0 != 0xB);
    if ((D_800E62BE == 0x62) && (D_800E62BF == 2) && (D_800E62C0 == 0xF) && (var_a0 >= 2)) {
        if (((u32) ((u32) (D_800E66B0 << 0x14) >> 0x1D) >= 4U) && (D_800E6838 != 0) && (D_800E683A != 0) && !(D_800E67CC & 0x20000000) && ((rand() & 0xF) < 5)) {
            return 1;
        }
        /* Duplicate return node #16. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}
