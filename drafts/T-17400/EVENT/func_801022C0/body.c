void func_801022C0(void) {
    EvFnTbl53 sp2C;
    s16 var_a0;
    s32 temp_lo;
    u8 var_a3;
    Rec24 *temp_v0;

    sp2C = D_801225A8;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    sp2C.f[D_800B1AF6](0x80);
    if (D_800B1AF6 == 0xE) {
        var_a0 = 0;
        if (!(D_800B1AE4 & 1)) {
            if (D_800F19AB == 0x80) {
                var_a3 = 0x60;
            } else {
                var_a3 = 0x80;
            }
            *(u8 *)((unsigned char *)D_800E9620 + (6535)) = var_a3;
            *(u8 *)((unsigned char *)D_800E9620 + (6603)) = var_a3;
            do {
                temp_lo = var_a0 * 0x24;
                var_a0 += 1;
                temp_v0 = (Rec24 *)((u8 *)D_800EC190 + temp_lo);
                temp_v0[2].unk_16 = var_a3;
                temp_v0[2].unk_15 = var_a3;
                temp_v0[2].unk_14 = var_a3;
            } while (var_a0 < 6);
            D_800F19AB = var_a3;
        }
    }
}
