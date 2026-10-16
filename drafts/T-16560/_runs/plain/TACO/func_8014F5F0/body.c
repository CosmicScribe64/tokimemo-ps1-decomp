void func_8014F5F0(void) {
    s32 var_at;
    u32 temp_v1;
    u32 temp_v1_2;
    u8 temp_v0;

    temp_v1 = D_800E7384;
    if (temp_v1 < 0x64U) {
        func_8014FBD4(1);
        func_80151264(5U);
        func_8015443C(0x801A7000, 0xA, 0x100, 0x1EF);
    } else if (temp_v1 >= 0x64U) {
        var_at = temp_v1 < 0x226U;
        if (temp_v1 < 0x1F4U) {
            func_80151264(5U);
            func_8014FBD4((s32) ((u32) (D_800E7384 - 0x3C) / 40U));
        } else {
            goto block_6;
        }
    } else {
        var_at = temp_v1 < 0x226U;
block_6:
        if (var_at == 0) {
            if ((u8) D_800E62B6 < 0xFFU) {
                temp_v0 = D_800E62B8 + 1;
                D_800E62B7 = temp_v0;
                D_800E62B6 = temp_v0;
                D_800E7384 = temp_v1;
                D_800E62B8 = temp_v0;
                func_801517AC(temp_v1 - 0x226);
            } else {
                D_80122760 = 0x64;
                D_80122764 = 0x32;
                D_80122768 = -0x64;
                D_8012276C = 0x70;
                D_8012276D = 0x70;
                D_8012276E = 0x80;
                func_8009AD70(0, (s32) &D_80122760);
                D_80122770 = 0;
                D_80122774 = -0x64;
                D_80122778 = -0x64;
                D_8012277C = 0x40;
                D_8012277D = 0x40;
                D_8012277E = 0x40;
                func_8009AD70(1, (s32) &D_80122770);
                D_80122780 = -0x64;
                D_80122784 = 0x32;
                D_80122788 = -0x64;
                D_8012278C = 0x80;
                D_8012278D = 0x80;
                D_8012278E = 0x90;
                func_8009AD70(2, (s32) &D_80122780);
                D_800E7384 = 0;
                func_8004284C();
            }
        }
    }
    temp_v1_2 = D_800E7384;
    if (temp_v1_2 == 0x258) {
        func_801335A0(0, 0x502);
    }
    D_800E7384 = temp_v1_2 + 1;
}
