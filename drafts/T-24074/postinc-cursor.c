/* NON_MATCHING: T-24074 rejected post-increment cursor control, 241 rows/frame0x40. */
s32 func_80144224(void) {
    u32 sp30;
    s32 temp_a2;
    u32 temp_a3;
    s32 *order;
    s32 selected;

    D_801217D0[0].unk_00 = 0x80000000;
    D_801217D0[1].unk_00 = 0x80000000;
    func_80143968();
    temp_a3 = D_800E6280.unk_1104.u;
    sp30 = temp_a3 / 701U;
    if ((temp_a3 % 701U) == 1) {
        if (D_80150148 == 0) {
            func_801436E4((s32) D_800E6280.unk_F5F);
        } else if (D_80150148 < 0xF) {
            order = &D_8015014C[D_80150148];
            selected = *order++;
            if (D_800E6280.unk_F5F == selected) {
                selected = *order++;
                D_80150148++;
            }
            func_801436E4(selected);
        } else {
            func_801436E4((s32) D_800E6280.unk_F5F);
        }
        D_80150148 += 1;
        temp_a3 = (u32) D_800E6280.unk_1104.u;
    } else if ((s32) sp30 < 0xF) {
        if ((temp_a3 % 701U) == 2) {
            func_8014407C((s32) sp30);
            temp_a3 = (u32) D_800E6280.unk_1104.u;
        } else if ((temp_a3 % 701U) < 0xA0U) {
            temp_a2 = (temp_a3 % 701U) * 3;
            func_801441DC(2, temp_a2 / 4);
            func_801441DC(3, (s32) (temp_a2 - 0x3C) / 4);
            func_801441DC(4, (temp_a2 - 0x78) / 4);
            temp_a3 = (u32) D_800E6280.unk_1104.u;
        }
    }
    if (D_80150148 < 0x10) {
        if ((temp_a3 % 701U) >= 0x21EU) {
            temp_a2 = (0x2BD - (temp_a3 % 701U)) * 3;
            func_801441DC(2, (temp_a2 - 0x78) / 4);
            func_801441DC(3, (s32) (temp_a2 - 0x3C) / 4);
            func_801441DC(4, temp_a2 / 4);
            temp_a3 = (u32) D_800E6280.unk_1104.u;
        }
    }
    if ((temp_a3 >= 0x259U) && (func_800460EC() & 2)) {
        func_80042940(2);
    }
    func_8004B19C((s32) D_800E6280.unk_1104.u, 0, -0xA);
    func_8004B19C((s32) sp30, 0, 0);
    func_8004B19C(D_80150148, 0, 0xA);
    func_8004B19C(D_8015014C[D_80150148], 0, 0x14);
}
