void func_80136A2C(void) {
    EnObj *var_a0;
    s32 temp_v1;
    s32 temp_v1_2;

    if (D_800B3CFC != 0) {
        D_800B3CF8 = 0;
        D_800B3CFC = 0;
        D_80139B18 += 1;
    }
    var_a0 = D_8013984C;
    if (D_80139B18 > 0) {
        do {
            temp_v1 = var_a0->unk_00;
            if ((temp_v1 > 0) && (temp_v1 < 4)) {
                if (D_80139B18 < 3) {
                    var_a0->unk_00 = var_a0->unk_04 + 1;
                } else {
                    var_a0->unk_00 = 4;
                    var_a0->unk_04 = 0;
                }
            }
            temp_v1_2 = var_a0[1].unk_00;
            if ((temp_v1_2 > 0) && (temp_v1_2 < 4)) {
                if (D_80139B18 < 3) {
                    var_a0[1].unk_00 = var_a0[1].unk_04 + 1;
                } else {
                    var_a0[1].unk_00 = 4;
                    var_a0[1].unk_04 = 0;
                }
            }
            var_a0 += 2;
        } while (var_a0 != D_80139A2C);
        D_80139B18 = D_80139B18;
    }
}
