void func_80134258(s32 arg0) {
    s32 sp64;
    s32 sp60;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_v0;
    s32 var_s1;
    u8 *var_s0;

    func_80134784((s32) &sp64, (s32) &sp60);
    var_s0 = D_8013984C;
    var_s1 = 0;
    do {
        if ((var_s0->unk0 == 0) && ((sp64 * 0xA) < (func_800AE0D0() % 100))) {
            temp_v0 = func_800AE0D0();
            if ((sp60 == 0) && ((temp_v0 % 100) >= 0x3D)) {
                var_s0->unk0 = 4;
                sp60 += 1;
            } else {
                temp_hi = func_800AE0D0() % 3;
                sp64 += 1;
                var_s0->unk0 = (s32) (temp_hi + 1);
            }
            temp_hi_2 = func_800AE0D0() % 100;
            if (temp_hi_2 < 0x14) {
                *(s32 *)((unsigned char *)var_s0 + (4)) = 0;
            } else if (temp_hi_2 < 0x3C) {
                *(s32 *)((unsigned char *)var_s0 + (4)) = 1;
            } else {
                *(s32 *)((unsigned char *)var_s0 + (4)) = 2;
            }
            temp_hi_3 = func_800AE0D0() % 100;
            if (temp_hi_3 < 0x14) {
                *(s32 *)((unsigned char *)var_s0 + (8)) = 0;
            } else if (temp_hi_3 < 0x3C) {
                *(s32 *)((unsigned char *)var_s0 + (8)) = 1;
            } else {
                *(s32 *)((unsigned char *)var_s0 + (8)) = 2;
            }
            if (var_s0->unk0 == 4) {
                *(s32 *)((unsigned char *)var_s0 + (4)) = 0;
            }
            *(s32 *)((unsigned char *)var_s0 + (12)) = 0;
            *(s32 *)((unsigned char *)var_s0 + (16)) = 0;
            func_801344F0(var_s1, arg0);
        }
        var_s1 += 1;
        var_s0 += 0x30;
    } while (var_s1 != 0xA);
}
