s32 func_8013CC40(void) {
    s32 var_s1;
    s32 var_v0;
    u8 *temp_s0;

    var_s1 = 0x880;
    var_v0 = 0x10;
    do {
        temp_s0 = &D_8015EDB4->unk0[var_s1];
        if ((*(u8 *)((unsigned char *)temp_s0 + (133)) == 0) && (*(u8 *)((unsigned char *)temp_s0 + (1)) & 0x80)) {
            if (!(D_800E7378 & 0x1F)) {
                func_801335A0(1, 0x508);
            }
            func_8013D1E0(*(TcPos *)((u8 *)temp_s0 + 116), *(TcPos *)((u8 *)temp_s0 + 124), 0);
            var_v0 = func_8013D1E0(*(TcPos *)((u8 *)temp_s0 + 116), *(TcPos *)((u8 *)temp_s0 + 124), 0);
            if (var_s1 == 0x440) {
                *(s8 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (133)) = (s8) (func_800AE0D0() & 0x3F);
                *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (116)) = (s16) (func_800AE0D0() % 100);
                var_v0 = func_800AE0D0();
                *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (118)) = (s16) (var_v0 % 100);
            }
        }
        var_s1 += 0x88;
    } while (var_s1 != 0x2200);
    return var_v0;
}
