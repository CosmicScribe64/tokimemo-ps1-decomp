s32 func_80144224(void) {
    u32 sp30;
    s32 sp28;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v1;
    s32 var_a0;
    u32 temp_a3;
    u32 temp_hi;
    u32 temp_hi_2;
    u8 *temp_v0;

    D_801217D0->unk_00 = 0x80000000;
    D_801217F4 = 0x80000000;
    func_80143968();
    temp_a3 = D_800E7384;
    temp_hi = temp_a3 % 701U;
    sp30 = temp_a3 / 701U;
    if (temp_hi == 1) {
        temp_v1 = D_80150148;
        if (temp_v1 == 0) {
            func_801436E4((s32) D_800E71DF);
        } else if (temp_v1 < 0xF) {
            temp_v0 = (temp_v1 * 4) + &D_80150150;
            var_a0 = *(s32 *)((unsigned char *)temp_v0 + (-4));
            D_80150148 = temp_v1;
            if (D_800E71DF == var_a0) {
                var_a0 = (*(s32 *)temp_v0);
                D_80150148 = temp_v1 + 1;
            }
            func_801436E4(var_a0);
        } else {
            D_80150148 = temp_v1;
            func_801436E4((s32) D_800E71DF);
        }
        D_80150148 += 1;
    } else if ((s32) sp30 < 0xF) {
        if (temp_hi == 2) {
            func_8014407C();
        } else if (temp_hi < 0xA0U) {
            temp_a2 = temp_hi * 3;
            sp28 = temp_a2;
            func_801441DC(2, temp_a2 / 4, temp_a2, (s32) temp_a3);
            func_801441DC(3, (s32) (sp28 - 0x3C) / 4);
            func_801441DC(4, (s32) (sp28 - 0x78) / 4);
        }
    }
    if (D_80150148 < 0x10) {
        temp_hi_2 = temp_a3 % 701U;
        if (temp_hi_2 >= 0x21EU) {
            temp_a2_2 = (0x2BD - temp_hi_2) * 3;
            sp28 = temp_a2_2;
            func_801441DC(2, (s32) (temp_a2_2 - 0x78) / 4, temp_a2_2, (s32) temp_a3);
            func_801441DC(3, (s32) (sp28 - 0x3C) / 4);
            func_801441DC(4, sp28 / 4);
        }
    }
    if ((temp_a3 >= 0x259U) && (func_800460EC() & 2)) {
        func_80042940(2);
    }
    func_8004B19C((s32) D_800E7384, 0, -0xA);
    func_8004B19C((s32) sp30, 0, 0);
    func_8004B19C(D_80150148, 0, 0xA);
    func_8004B19C(D_8015014C[D_80150148], 0, 0x14);
}
