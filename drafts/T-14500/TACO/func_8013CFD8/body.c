s32 func_8013CFD8(void) {
    TcActor *var_v1;
    s16 temp_s3;
    s16 temp_s4;
    s32 temp_s0;
    s32 temp_t6;
    s32 var_a0;
    s32 var_a1;
    s32 var_s1;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 var_v0;

    temp_s3 = D_8015E770->unk0;
    temp_s4 = D_8015E772;
    var_s1 = 0x880;
    var_v0 = 0x10;
    var_v1 = D_8015EDB4 + 0x880;
    do {
        if ((var_v1->pad0[1] & 0x80) && (var_v1->unk84[3] & 1) && (var_v1->unk84[1] != 0) && (var_v1->unk84[0] != 0)) {
            temp_s0 = func_800AE0C0(temp_s3 - var_v1->unk74);
            var_a0 = 0x1000 - temp_s0;
            var_a1 = 0x1000 - func_800AE0C0(temp_s4 - *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s1]) + (118)));
            if (var_a0 < 0) {
                var_a0 = 0;
            }
            if (var_a1 < 0) {
                var_a1 = 0;
            }
            temp_t6 = (s32) (var_a0 + var_a1) / 256;
            if (temp_t6 != 0) {
                temp_v1 = &D_8015EDB4->pad0[var_s1];
                func_8013D2A0((s32) *(s16 *)((unsigned char *)temp_v1 + (116)), (s32) *(s16 *)((unsigned char *)temp_v1 + (118)), (s32) (s16) (*(s16 *)((unsigned char *)temp_v1 + (120)) - 0x200));
            }
            temp_v1_2 = &D_8015EDB4->pad0[var_s1];
            var_v0 = *(u8 *)((unsigned char *)temp_v1_2 + (133));
            if ((u32) temp_t6 < var_v0) {
                *(u8 *)((unsigned char *)temp_v1_2 + (133)) = (u8) (var_v0 - temp_t6);
            } else {
                *(u8 *)((unsigned char *)temp_v1_2 + (133)) = 0U;
            }
            var_v1 = (TcActor *) &D_8015EDB4->pad0[var_s1];
            if (var_v1->unk84[1] == 0) {
                var_v0 = func_801335A0(1, 0x508);
                var_v1 = (TcActor *) &D_8015EDB4->pad0[var_s1];
                D_8015EDF0 += D_8015F504[(u8) var_v1->unk84[2] % 17U];
            }
        }
        var_s1 += 0x88;
        var_v1 += 0x88;
    } while (var_s1 != 0x2200);
    return (s32) var_v0;
}
