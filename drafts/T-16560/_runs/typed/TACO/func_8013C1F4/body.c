void func_8013C1F4(void) {
    Tc14 *var_s0;
    Tc14 *var_s0_2;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_s2_2;
    u8 temp_v0;
    u8 temp_v1;

    D_8015F500 = 0;
    var_s0 = D_8015EB30;
    if (!(D_8015EDB4->unk2 & 0x80)) {
loop_2:
        if ((var_s0->unk13 & 0x80) && (temp_s2 = func_800AE0C0(var_s0->unk0 - D_8015EDB4->unk74), temp_s1 = func_800AE0C0(var_s0->unk2 - D_8015EDB4->unk76), ((func_800AE0C0(var_s0->unk4 - D_8015EDB4->unk78) < 0x100) != 0)) && (temp_s1 < 0x80) && (temp_s2 < 0x80)) {
            var_s0->unk13 = 0;
            func_801335A0(1, 0x506);
            D_8015EDB4->unk2 |= 4;
            temp_v1 = D_8015EDB4->unk84[1];
            if (temp_v1 >= 5U) {
                D_8015EDB4->unk84[1] = temp_v1 - 4;
            } else {
                D_8015EDB4->unk84[1] = 0;
            }
            D_8015EDB4->unk7A = 0x10;
        } else {
            var_s0 += 1;
            if (var_s0 != D_8015EC70) {
                goto loop_2;
            }
        }
    }
    var_s0_2 = D_8015EB30;
    if (!(D_8015EDB4[8].unk2 & 0x80)) {
loop_13:
        if (var_s0_2->unk13 & 0x80) {
            temp_s2_2 = func_800AE0C0(var_s0_2->unk0 - D_8015EDB4[8].unk74);
            temp_s1_2 = func_800AE0C0(var_s0_2->unk2 - D_8015EDB4[8].unk76);
            if ((func_800AE0C0(var_s0_2->unk4 - D_8015EDB4[8].unk78) < 0x100) && (temp_s1_2 < 0x80) && (temp_s2_2 < 0x80)) {
                var_s0_2->unk13 = 0;
                temp_v0 = D_8015EDB4[8].unk84[1];
                if (temp_v0 >= 5U) {
                    D_8015EDB4[8].unk84[1] = (u8) (temp_v0 - 4);
                } else {
                    D_8015EDB4[8].unk84[1] = 0U;
                }
                D_8015EDB4[8].unk7A = 0x10;
                return;
            }
        }
        var_s0_2 += 1;
        if (var_s0_2 == D_8015EC70) {

        } else {
            goto loop_13;
        }
    }
}
