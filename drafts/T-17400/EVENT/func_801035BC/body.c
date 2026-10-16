void func_801035BC(void) {
    EvFnTbl44 sp28;
    s16 var_v0;
    s16 var_v0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 var_at;
    DisplayActor44 *temp_v1;

    sp28 = D_801228E0;
    func_80078950("%d\n", D_800B1AF6);
    sp28.f[D_800B1AF6](0x80);
    if (func_8004C984 == sp28.f[D_800B1AF6]) {
        var_v0 = 0;
        do {
            temp_lo = var_v0 * 0x44;
            var_v0 += 1;
            *(u8 *)((unsigned char *)((DisplayActor44 *)((u8 *)D_800E9620 + temp_lo)) + (6671)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (6535));
        } while (var_v0 < 8);
        var_at = (u8) D_800B1AF6 < 0x11U;
        if ((u8) *(u8 *)((unsigned char *)D_800E9620 + (6535)) < 9U) {
            var_v0_2 = 0;
            do {
                temp_lo_2 = var_v0_2 * 0x44;
                var_v0_2 += 1;
                temp_v1 = (DisplayActor44 *)((u8 *)D_800E9620 + temp_lo_2);
                temp_v1[98].unk_03 = (u8) (temp_v1[98].unk_03 & 0x7F);
            } while (var_v0_2 < 8);
            goto block_6;
        }
    } else {
block_6:
        var_at = (u8) D_800B1AF6 < 0x11U;
    }
    if (var_at == 0) {
        if ((u8) D_800B1AF6 < 0x14U) {
            func_80103774();
        }
    }
}
